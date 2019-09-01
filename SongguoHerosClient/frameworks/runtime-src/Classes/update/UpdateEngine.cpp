//
//  UpdateEngine.cpp
//  sanguoGame
//
//  Created by fu.chenhao on 8/10/15.
//
//

#include "UpdateEngine.h"
#include "ProjectConfig.h"

#include "cocos2d.h"
#include "json/rapidjson.h"
#include "json/document.h"

#include <thread>
#include <curl/curl.h>
#include <curl/easy.h>
#include <unzip/unzip.h>

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8) && (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#endif

#include "CCLuaEngine.h"
#include "base/CCScriptSupport.h"

using namespace std;
using namespace cocos2d;

#define TEMP_PACKAGE_FILE_NAME		"cocos2dx-update-temp.zip"

#define BUFFER_SIZE    8192
#define MAX_FILENAME   512

#define LOW_SPEED_LIMIT 1L
#define LOW_SPEED_TIME	5L

UpdateEngine* UpdateEngine::s_sharedUpdateEngine = nullptr;

UpdateEngine* UpdateEngine::getInstance()
{
    if (!s_sharedUpdateEngine)
    {
        s_sharedUpdateEngine = new UpdateEngine();
        s_sharedUpdateEngine->setDelegate(new UpdateEngineDelegate());
    }
    
    return s_sharedUpdateEngine;
}

UpdateEngine::UpdateEngine()
: _curl(nullptr), _connectionTimeout(5), _delegate(nullptr), _isDownloading(false)
{
}

UpdateEngine::~UpdateEngine()
{
    if (_delegate)
    {
        delete _delegate;
    }
}

static size_t getVersionCode(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    string* version = (string*)userdata;
    version->append((char*)ptr, size * nmemb);
    return (size * nmemb);
}

void UpdateEngine::loadVersion(const std::string& versionFileUrl)
{
    _curl = curl_easy_init();
    if (!_curl)
    {
        CCLOG("can not init curl");
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, this]{
            if (this->_delegate)
                this->_delegate->onError(ErrorCode::LOAD_VERSION_FAIL);
        });
        return;
    }
    
    _versionConfig.clear();
    
    CURLcode res;
    curl_easy_setopt(_curl, CURLOPT_URL, versionFileUrl.c_str());
    curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, getVersionCode);
    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &_versionConfig);
    if (_connectionTimeout) curl_easy_setopt(_curl, CURLOPT_CONNECTTIMEOUT, _connectionTimeout);
    curl_easy_setopt(_curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(_curl, CURLOPT_LOW_SPEED_LIMIT, LOW_SPEED_LIMIT);
    curl_easy_setopt(_curl, CURLOPT_LOW_SPEED_TIME, LOW_SPEED_TIME);
    res = curl_easy_perform(_curl);
    
    if (res != 0)
    {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, this]{
            if (this->_delegate)
                this->_delegate->onError(ErrorCode::NETWORK);
        });
        CCLOG("can not get version file content, error code is %d", res);
        return;
    }
    
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, this]{
        if (this->_delegate)
            this->_delegate->onVersionLoaded(_versionConfig);
    });
    CCLOG("version loaded: %s", _versionConfig.c_str());
}

void UpdateEngine::loadPackages(const std::string& packages, const std::string& storagePath)
{
    if (_isDownloading) return;
    
    _storagePath = storagePath;
    
    rapidjson::Document doc;
    doc.Parse<rapidjson::kParseDefaultFlags>(packages.c_str());
    
    if (doc.IsArray())
    {
        auto numPackages = doc.Size();
        for (int i = 0; i < numPackages; i++)
        {
            auto& packageInfo = doc[i];
            if (packageInfo.IsObject())
            {
                UpdatePackage package;
                package.version = packageInfo["version"].GetInt();
                package.url = packageInfo["url"].GetString();
                package.md5 = packageInfo["md5"].GetString();
                
                _updatePackages.push_back(package);
            }
        }
    }
    
    _isDownloading = true;
    
    //设置下载目录，不存在则创建目录
    _downloadPath = FileUtils::getInstance()->getWritablePath();
    _downloadPath += "download_temp/";
    createDirectory(_downloadPath.c_str());
   
    //开始下载任务
    auto t = std::thread(&UpdateEngine::downloadAndUncompress, this);
    t.detach();
}

/*
 * Create a direcotry is platform depended.
 */
bool UpdateEngine::createDirectory(const char *path)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    return FileUtils::getInstance()->createDirectory(_storagePath.c_str());
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    BOOL ret = CreateDirectoryA(path, nullptr);
    if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
    {
        return false;
    }
    return true;
#else
    mode_t processMask = umask(0);
    int ret = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
    umask(processMask);
    if (ret != 0 && (errno != EEXIST))
    {
        return false;
    }
    
    return true;
#endif
}

void UpdateEngine::downloadAndUncompress()
{
    while (_updatePackages.size() > 0)
    {
        //取出当前第一个需要下载的url
        auto& package = _updatePackages.front();
        _packageUrl = package.url;
        
        //通知文件下载
        int version = package.version;
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, this, version]{
            if (this->_delegate)
                this->_delegate->onDownload(version);
        });
        
        //开始下载，下载失败退出
        if (!downLoad())
        {
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, this]{
                if (this->_delegate)
                    this->_delegate->onError(ErrorCode::UNDOWNED);
            });
            break;
        }
        
        //通知文件压缩
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, this, version]{
            if (this->_delegate)
                this->_delegate->onUncompress(version);
        });
        
        //解压下载的zip文件
        string outFileName = _downloadPath + TEMP_PACKAGE_FILE_NAME;
        if (!uncompress(outFileName))
        {
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, this]{
                if (this->_delegate)
                    this->_delegate->onError(ErrorCode::UNCOMPRESS);
            });
            break;
        }
        //解压成功，任务出队列
        _updatePackages.pop_front();
        
        //删除本次下载的文件
        string zipfileName = _downloadPath + TEMP_PACKAGE_FILE_NAME;
        if (remove(zipfileName.c_str()) != 0)
        {
            CCLOG("can not remove downloaded zip file %s", zipfileName.c_str());
        }
        
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, this]{
            //如果更新任务已经完成，通知更新成功
            if(_updatePackages.size() <= 0 && this->_delegate)
                this->_delegate->onSuccess();
        });
    }
    
    curl_easy_cleanup(_curl);
    _isDownloading = false;
}

bool UpdateEngine::uncompress(string outFileName)
{
    //打开zip文件
    unzFile zipfile = unzOpen(outFileName.c_str());
    if (!zipfile)
    {
        CCLOG("can not open downloaded zip file %s", outFileName.c_str());
        return false;
    }
    
    // Get info about the zip file
    unz_global_info global_info;
    if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK)
    {
        CCLOG("can not read file global info of %s", outFileName.c_str());
        unzClose(zipfile);
        return false;
    }
    
    // Buffer to hold data read from the zip file
    char readBuffer[BUFFER_SIZE];
    
    CCLOG("start uncompressing");
    
    // Loop to extract all files.
    uLong i;
    for (i = 0; i < global_info.number_entry; ++i)
    {
        // Get info about current file.
        unz_file_info fileInfo;
        char fileName[MAX_FILENAME];
        if (unzGetCurrentFileInfo(zipfile,
                                  &fileInfo,
                                  fileName,
                                  MAX_FILENAME,
                                  nullptr,
                                  0,
                                  nullptr,
                                  0) != UNZ_OK)
        {
            CCLOG("can not read file info");
            unzClose(zipfile);
            return false;
        }
        
        const string fullPath = _storagePath + fileName;
        
        // Check if this entry is a directory or a file.
        const size_t filenameLength = strlen(fileName);
        if (fileName[filenameLength-1] == '/')
        {
            // Entry is a direcotry, so create it.
            // If the directory exists, it will failed scilently.
            if (!createDirectory(fullPath.c_str()))
            {
                CCLOG("can not create directory %s", fullPath.c_str());
                unzClose(zipfile);
                return false;
            }
        }
        else
        {
            //There are not directory entry in some case.
            //So we need to test whether the file directory exists when uncompressing file entry
            //, if does not exist then create directory
            const string fileNameStr(fileName);
            
            size_t startIndex=0;
            
            size_t index=fileNameStr.find("/",startIndex);
            
            while(index != std::string::npos)
            {
                const string dir=_storagePath+fileNameStr.substr(0,index);
                
                FILE *out = fopen(dir.c_str(), "r");
                
                if(!out)
                {
                    if (!createDirectory(dir.c_str()))
                    {
                        CCLOG("can not create directory %s", dir.c_str());
                        unzClose(zipfile);
                        return false;
                    }
                    else
                    {
                        CCLOG("create directory %s",dir.c_str());
                    }
                }
                else
                {
                    fclose(out);
                }
                
                startIndex=index+1;
                
                index=fileNameStr.find("/",startIndex);
                
            }
            
            
            
            // Entry is a file, so extract it.
            
            // Open current file.
            if (unzOpenCurrentFile(zipfile) != UNZ_OK)
            {
                CCLOG("can not open file %s", fileName);
                unzClose(zipfile);
                return false;
            }
            
            // Create a file to store current file.
            FILE *out = fopen(fullPath.c_str(), "wb");
            if (! out)
            {
                CCLOG("can not open destination file %s", fullPath.c_str());
                unzCloseCurrentFile(zipfile);
                unzClose(zipfile);
                return false;
            }
            
            // Write current file content to destinate file.
            int error = UNZ_OK;
            do
            {
                error = unzReadCurrentFile(zipfile, readBuffer, BUFFER_SIZE);
                if (error < 0)
                {
                    CCLOG("can not read zip file %s, error code is %d", fileName, error);
                    unzCloseCurrentFile(zipfile);
                    unzClose(zipfile);
                    return false;
                }
                
                if (error > 0)
                {
                    fwrite(readBuffer, error, 1, out);
                }
            } while(error > 0);
            
            fclose(out);
        }
        
        unzCloseCurrentFile(zipfile);
        
        // Goto next entry listed in the zip file.
        if ((i+1) < global_info.number_entry)
        {
            if (unzGoToNextFile(zipfile) != UNZ_OK)
            {
                CCLOG("can not read next file");
                unzClose(zipfile);
                return false;
            }
        }
    }
    
    CCLOG("end uncompressing");
    unzClose(zipfile);
    
    return true;
}

static size_t downLoadPackage(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    FILE *fp = (FILE*)userdata;
    size_t written = fwrite(ptr, size, nmemb, fp);
    return written;
}

int updateProgressFunc(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded)
{
    static int percent = 0;
    int tmp = (int)(nowDownloaded / totalToDownload * 100);
    if (percent != tmp)
    {
        percent = tmp;
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]{
            auto manager = static_cast<UpdateEngine*>(ptr);
            if (manager->_delegate)
                manager->_delegate->onProgress(percent);
        });
        CCLOG("downloading... %d%%", percent);
    }
    return 0;
}

bool UpdateEngine::downLoad()
{
    //创建临时存放的zip文件
    const string outFileName = _downloadPath + TEMP_PACKAGE_FILE_NAME;
    FILE* fp = fopen(outFileName.c_str(), "wb");
    if (!fp)
    {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, this]{
            if (this->_delegate)
                this->_delegate->onError(ErrorCode::CREATE_FILE);
        });
        CCLOG("can not create temp zip file %s", outFileName.c_str());
        return false;
    }
    
    //开始从服务器下载
    CURLcode res;
    curl_easy_setopt(_curl, CURLOPT_URL, _packageUrl.c_str());
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, downLoadPackage);
    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(_curl, CURLOPT_NOPROGRESS, false);
    curl_easy_setopt(_curl, CURLOPT_PROGRESSFUNCTION, updateProgressFunc);
    curl_easy_setopt(_curl, CURLOPT_PROGRESSDATA, this);
    curl_easy_setopt(_curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(_curl, CURLOPT_LOW_SPEED_LIMIT, LOW_SPEED_LIMIT);
    curl_easy_setopt(_curl, CURLOPT_LOW_SPEED_TIME, LOW_SPEED_TIME);
    
    res = curl_easy_perform(_curl);
    if (res != 0)
    {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, this]{
            if (this->_delegate)
                this->_delegate->onError(ErrorCode::NETWORK);
        });
        CCLOG("error when download package %s", _packageUrl.c_str());
        fclose(fp);
        return false;
    }
    CCLOG("succeed downloading package %s", _packageUrl.c_str());
    fclose(fp);
    return true;
}

UpdateEngineDelegate::~UpdateEngineDelegate()
{
    CC_SAFE_DELETE(_versionLoadedCallback);
    CC_SAFE_DELETE(_errorCallback);
    CC_SAFE_DELETE(_progressCallback);
    CC_SAFE_DELETE(_successCallback);
    CC_SAFE_DELETE(_downloadCallback);
    CC_SAFE_DELETE(_uncompressCallback);
}

void UpdateEngineDelegate::setLuaCallbacks(const std::string& callbackMap)
{
    rapidjson::Document doc;
    doc.Parse<rapidjson::kParseDefaultFlags>(callbackMap.c_str());
    
    if (doc.IsObject())
    {
        if (doc.HasMember("onVersionLoaded"))
        {
            _versionLoadedCallback = new std::string(doc["onVersionLoaded"].GetString());
        }
        if (doc.HasMember("onError"))
        {
            _errorCallback = new std::string(doc["onError"].GetString());
        }
        if (doc.HasMember("onProgress"))
        {
            _progressCallback = new std::string(doc["onProgress"].GetString());
        }
        if (doc.HasMember("onSuccess"))
        {
            _successCallback = new std::string(doc["onSuccess"].GetString());
        }
        if (doc.HasMember("onDownload"))
        {
            _downloadCallback = new std::string(doc["onDownload"].GetString());
        }
        if (doc.HasMember("onUncompress"))
        {
            _uncompressCallback = new std::string(doc["onUncompress"].GetString());
        }
    }
}

void UpdateEngineDelegate::onVersionLoaded(const std::string& versionConfig)
{
#if (USE_LUA_ENGINE)
    if (_versionLoadedCallback)
    {
        lua_State *L = cocos2d::LuaEngine::getInstance()->getLuaStack()->getLuaState();
        lua_getglobal(L, _versionLoadedCallback->c_str());
        lua_pushstring(L, versionConfig.c_str());
        lua_call(L, 1, 0);
    }
#endif
}

void UpdateEngineDelegate::onError(ErrorCode errorCode)
{
#if (USE_LUA_ENGINE)
    if (_errorCallback)
    {
        lua_State *L = cocos2d::LuaEngine::getInstance()->getLuaStack()->getLuaState();
        lua_getglobal(L, _errorCallback->c_str());
        lua_pushinteger(L, errorCode);
        lua_call(L, 1, 0);
    }
#endif
}

void UpdateEngineDelegate::onProgress(int percent)
{
#if (USE_LUA_ENGINE)
    if (_progressCallback)
    {
        lua_State *L = cocos2d::LuaEngine::getInstance()->getLuaStack()->getLuaState();
        lua_getglobal(L, _progressCallback->c_str());
        lua_pushinteger(L, percent);
        lua_call(L, 1, 0);
    }
#endif
}

void UpdateEngineDelegate::onSuccess()
{
#if (USE_LUA_ENGINE)
    if (_successCallback)
    {
        lua_State *L = cocos2d::LuaEngine::getInstance()->getLuaStack()->getLuaState();
        lua_getglobal(L, _successCallback->c_str());
        lua_call(L, 0, 0);
    }
#endif
}

void UpdateEngineDelegate::onDownload(int packageVersion)
{
#if (USE_LUA_ENGINE)
    if (_downloadCallback)
    {
        lua_State *L = cocos2d::LuaEngine::getInstance()->getLuaStack()->getLuaState();
        lua_getglobal(L, _downloadCallback->c_str());
        lua_pushinteger(L, packageVersion);
        lua_call(L, 1, 0);
    }
#endif
}

void UpdateEngineDelegate::onUncompress(int packageVersion)
{
#if (USE_LUA_ENGINE)
    if (_uncompressCallback)
    {
        lua_State *L = cocos2d::LuaEngine::getInstance()->getLuaStack()->getLuaState();
        lua_getglobal(L, _uncompressCallback->c_str());
        lua_pushinteger(L, packageVersion);
        lua_call(L, 1, 0);
    }
#endif
}

int setLuaCallbacks(lua_State *L)
{
    const char* str = lua_tostring(L, 1);
    UpdateEngine::getInstance()->getDelegate()->setLuaCallbacks(std::string(str));
    return 1;
}

int loadVersion(lua_State *L)
{
    const char* str = lua_tostring(L, 1);
    UpdateEngine::getInstance()->loadVersion(std::string(str));
    return 1;
}

int loadPackages(lua_State *L)
{
    const char* packages = lua_tostring(L, 1);
    const char* storagePath = lua_tostring(L, 2);
    UpdateEngine::getInstance()->loadPackages(packages, storagePath);
    return 1;
}
