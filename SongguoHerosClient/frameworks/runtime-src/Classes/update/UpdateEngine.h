//
//  UpdateEngine.h
//  sanguoGame
//
//  Created by fu.chenhao on 8/10/15.
//
//

#ifndef __sanguoGame__UpdateEngine__
#define __sanguoGame__UpdateEngine__

#include <string>
#include "cocos2d.h"

enum ErrorCode
{
    LOAD_VERSION_FAIL,
    NETWORK,
    CREATE_FILE,
    UNDOWNED,
    UNCOMPRESS,
};

class UpdateEngineDelegate
{
public:
    ~UpdateEngineDelegate();
    
    void onVersionLoaded(const std::string& versionConfig);
    void onDownload(int packageVersion);
    void onUncompress(int packageVersion);
    void onError(ErrorCode errorCode);
    void onProgress(int percent);
    void onSuccess();
    
    void setLuaCallbacks(const std::string& callbackMap);

private:
    std::string *_versionLoadedCallback = nullptr;
    std::string *_downloadCallback = nullptr;
    std::string *_uncompressCallback = nullptr;
    std::string *_errorCallback = nullptr;
    std::string *_progressCallback = nullptr;
    std::string *_successCallback = nullptr;
};

class UpdateEngine
{
public:
    struct UpdatePackage
    {
        int version;
        std::string url;
        std::string md5;
    };
    
    UpdateEngine();
    virtual ~UpdateEngine();
    
    static UpdateEngine* getInstance();
    
    void loadVersion(const std::string& versionFileUrl);
    void loadPackages(const std::string& packages, const std::string& storagePath);
    
    void setDelegate(UpdateEngineDelegate *delegate) { _delegate = delegate; }
    UpdateEngineDelegate* getDelegate() const { return _delegate ;}
    
    friend int updateProgressFunc(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded);
    
protected:
    bool downLoad();
    bool uncompress(std::string outFileName);
    void downloadAndUncompress();
    
    bool createDirectory(const char *path);
    
private:
    std::string _versionConfig;
    std::string _packageUrl;
    std::string _storagePath;
    std::string _downloadPath;
    
    std::deque<UpdatePackage> _updatePackages;
    
    void *_curl;    
    bool _isDownloading;
    unsigned int _connectionTimeout;
    UpdateEngineDelegate *_delegate;
    
    static UpdateEngine* s_sharedUpdateEngine;
};

extern "C"
{
    int setLuaCallbacks(lua_State *L);
    int loadVersion(lua_State *L);
    int loadPackages(lua_State *L);
}

#endif /* defined(__sanguoGame__UpdateEngine__) */
