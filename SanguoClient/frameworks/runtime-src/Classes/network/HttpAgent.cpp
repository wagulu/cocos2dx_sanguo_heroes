//
//  HttpAgent.cpp
//  Game
//
//  Created by fu.chenhao on 3/19/15.
//
//

#include "HttpAgent.h"
#include "CCLuaEngine.h"

extern "C"
{
#include "CCLuaEngine.h"
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
    
}

#if (TARGET_IPHONE_SIMULATOR) || (TARGET_OS_MAC)
    #define MAX_RETRY_TIMES (999999)
#else
    #define MAX_RETRY_TIMES (3)
#endif


// ==============================
// static variables

static HttpAgent s_agent_normal(false);
static HttpAgent s_agent_canBeIgnore(true);

//static HttpResponse *s_lastResponse_normal = NULL;
//static HttpResponse *s_lastResponse_canBeIgnore = NULL;



HttpAgent::HttpAgent(bool canBeIgnored):
m_canBeIgnored(canBeIgnored)
{
    m_httpClient = HttpClient::getInstance();
}

HttpAgent::~HttpAgent()
{
    
}

void HttpAgent::reset()
{
    m_pCurrentSegment = NULL;
    m_segmentQueue.clear();
}

void HttpAgent::send(const char* buffer, size_t len, const char* url, const ccHttpRequestCallback& callback)
{
    HttpRequest *request = new HttpRequest();
    request->setUrl(url);
    request->setRequestType(HttpRequest::Type::POST);
//    request->setRequestType(HttpRequest::Type::GET);
    request->setRequestData(buffer, len);
    request->setResponseCallback(CC_CALLBACK_2(HttpAgent::onHttpRequestCompleted, this));

    Segment* pSegment = new Segment();
    pSegment->m_url = url;
    pSegment->m_leftRetryTimes = MAX_RETRY_TIMES;
//    pSegment->m_canBeIgnored = canBeIgnored;
    pSegment->m_callback = callback;
    pSegment->m_pRequest = request;
    m_segmentQueue.push_back(pSegment);
    
//    request->setTag("a");//设置Tag
    //        network::HttpClient::getInstance()->send(request);//添加到HttpClient任务队列
    //        request->release();//释放连接
    //        request->setResponseCallback(ccHttpRequestCallback());
    
    trySendNext();

}

void HttpAgent::send_lua(const char* buffer, size_t len, const char* url, const char* luaHandler)
{
    HttpRequest *request = new HttpRequest();
    request->setUrl(url);
    request->setRequestType(HttpRequest::Type::POST);
    //    request->setRequestType(HttpRequest::Type::GET);
    request->setRequestData(buffer, len);
    request->setResponseCallback(CC_CALLBACK_2(HttpAgent::onHttpRequestCompleted, this));
    
    Segment* pSegment = new Segment();
    pSegment->m_url = url;
    pSegment->m_leftRetryTimes = MAX_RETRY_TIMES;
    //    pSegment->m_canBeIgnored = canBeIgnored;
//    pSegment->m_callback = callback;
    if(luaHandler != NULL)
    {
        pSegment->m_luaHandler = luaHandler;
    }
    pSegment->m_pRequest = request;
    m_segmentQueue.push_back(pSegment);
    
    //    request->setTag("a");//设置Tag
    //        network::HttpClient::getInstance()->send(request);//添加到HttpClient任务队列
    //        request->release();//释放连接
    //        request->setResponseCallback(ccHttpRequestCallback());
    
    trySendNext();
    
}

void HttpAgent::trySendNext()
{
    if(m_pCurrentSegment == NULL)
    {
        if(m_segmentQueue.size() > 0)
        {
            m_pCurrentSegment = m_segmentQueue[0];
            m_segmentQueue.erase(std::find(m_segmentQueue.begin(), m_segmentQueue.end(), m_pCurrentSegment));

            correctRequest(m_pCurrentSegment->m_pRequest);
            
            m_httpClient->send(m_pCurrentSegment->m_pRequest);
            m_pCurrentSegment->m_pRequest->release();
            
            CCLOG("<< http request info >>");
            CCLOG("send http request");
            CCLOG("url = %s", m_pCurrentSegment->m_pRequest->getUrl());
            CCLOG("data size = %zi", m_pCurrentSegment->m_pRequest->getRequestDataSize());
            CCLOG("data = %s", m_pCurrentSegment->m_pRequest->getRequestData());
        }
    }
}

void HttpAgent::correctRequest(HttpRequest* m_pRequest)
{
    bool needHttpContentEncode = false;

    std::string header = "";

    char* pRequestData = m_pCurrentSegment->m_pRequest->getRequestData();
    int requestDataLen = m_pCurrentSegment->m_pRequest->getRequestDataSize();
    char* pEncodeData = new char[requestDataLen];
    
    char t[4];

    for(int i = 0; i < requestDataLen; ++i)
    {
        unsigned char b = pRequestData[i];
        if(b >= 128)
        {
            needHttpContentEncode = true;

            b -= 128;
            pEncodeData[i] = b;

            memset(t, 0, sizeof(t));
            sprintf(t, "%i", i);
            
            if(header == "")
            {
                header = "CONTENT_ENCODE: ";
                header += t;
            }
            else
            {
                header += ", ";
                header += t;
            }
        }
        else
        {
            pEncodeData[i] = b;
        }
    }
    
    if(needHttpContentEncode)
    {
        m_pCurrentSegment->m_pRequest->setRequestData(pEncodeData, requestDataLen);
        
        std::vector<std::string> headers = m_pCurrentSegment->m_pRequest->getHeaders();
        headers.push_back(header);
        m_pCurrentSegment->m_pRequest->setHeaders(headers);
        
        CCLOG("HTTP_CONTENT_ENCODE = %s", header.c_str());
    }
    
    delete [] pEncodeData;
    pEncodeData = NULL;
}


void HttpAgent::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{
    if (response)
    {
        HttpRequest *request = response->getHttpRequest();
        
//        const ccHttpRequestCallback& callback = request->getCallback();
//        Ref* pTarget = request->getTarget();
//        SEL_HttpResponse pSelector = request->getSelector();
        
//        if (callback != nullptr)
//        {
//            callback(this, response);
//        }
//        else if (pTarget && pSelector)
//        {
//            (pTarget->*pSelector)(this, response);
//        }

        if(request == m_pCurrentSegment->m_pRequest)
        {
            if(response->isSucceed() || m_canBeIgnored)
            {
                if(m_pCurrentSegment->m_callback)
                {
                    m_pCurrentSegment->m_callback(m_httpClient, response);
                }
                callbackLua(sender, response);
                
                CC_SAFE_DELETE(m_pCurrentSegment);

                //continue
                trySendNext();
            }
            else
            {
                if(m_pCurrentSegment->m_leftRetryTimes-- > 0)
                {
                    CCLOG("<<   http resent");
                    
                    m_httpClient->send(m_pCurrentSegment->m_pRequest);
                }
                else
                {
                    if(m_pCurrentSegment->m_callback)
                    {
                        m_pCurrentSegment->m_callback(m_httpClient, response);
                    }
                    callbackLua(sender, response);

                    CC_SAFE_DELETE(m_pCurrentSegment);
                    
                    //blocked if fail
//                    trySendNext();
                }
            }
            
//            response->release();
            
        }
        else
        {
//            response->release();
//            request->release();
        }
        
//        response->release();

    }

    
}


void HttpAgent::callbackLua(HttpClient* client, HttpResponse *response)
{
    if(m_pCurrentSegment->m_luaHandler != "")
    {
        lua_State *L = cocos2d::LuaEngine::getInstance()->getLuaStack()->getLuaState();

        //获取lua里面的add函数并把它放到lua的栈顶
        lua_getglobal(L, "onHttpRequestResponse");

        lua_pushstring(L, m_pCurrentSegment->m_luaHandler.c_str());

        lua_newtable(L);

        int i = 1;

        lua_pushnumber(L, i++);    //key
        lua_pushboolean(L, response->isSucceed());
        lua_settable(L, -3);       //push key,value

        lua_pushnumber(L, i++);    //key
        lua_pushinteger(L, response->getResponseCode());
        lua_settable(L, -3);       //push key,value

        // dump data
        std::vector<char>* pData = response->getResponseData();
        long dataLen = pData->size();
        
        char* pBuffer = new char[dataLen];
        for (unsigned int i = 0; i < dataLen; i++)
        {
            pBuffer[i] = (*pData)[i];
        }
        
        lua_pushnumber(L, i++);    //key
        lua_pushlstring(L, pBuffer, dataLen);
        lua_settable(L, -3);       //push key,value
        
        CC_SAFE_DELETE_ARRAY(pBuffer);
        
        lua_pushnumber(L, i++);    //key
        lua_pushinteger(L, dataLen);
        lua_settable(L, -3);       //push key,value

        lua_call(L, 2, 0);

        //最后我们把返回值从栈顶拿掉
//        lua_pop(L, 0);
        
        
//        if(s_agent_normal.getClient() == client)
//        {
//            s_lastResponse_normal = response;
//        }
//        else if(s_agent_canBeIgnore.getClient() == client)
//        {
//            s_lastResponse_canBeIgnore = response;
//        }
        
//        cocos2d::EventCustom evt(m_pCurrentSegment->m_luaHandler.c_str());
//        cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(&evt);
        
//        lua_State* L = cocos2d::LuaEngine::getInstance()->getLuaStack()->getLuaState();
//        lua_getglobal(L, "onHttpResponse");
//        
//        /* exec the domain function. */
//        int error = lua_pcall(L, 0, 0, 0);
//        if (error)
//        {
//            fprintf(stderr, "%s\n", lua_tostring(L, -1));
//            lua_pop(L, 1);
//        }

    }

}

// ==============================
// global send queue

int sendHttp(const char* buffer, size_t len, const char* url, const ccHttpRequestCallback& callback, bool canBeIgnored)
{
    HttpAgent* pAgent = canBeIgnored? &s_agent_canBeIgnore: &s_agent_normal;
    pAgent->send((const char*)buffer, len, url, callback);
    
    return 1;
    
}

int sendHttp_lua(lua_State *L)
{
    const void* buffer = lua_tostring(L, 1);
    size_t len = lua_tointeger(L, 2);
    const char* url = lua_tostring(L, 3);
//    lua_CFunction callback = lua_tocfunction(L, 3);
    const char* handler = lua_tostring(L, 4);
    bool canBeIgnored = lua_toboolean(L, 5);
    
//    callback(cocos2d::LuaEngine::getInstance()->getLuaStack());
    
//    cocos2d::LuaEngine::getInstance()->getLuaStack()->executeFunctionByHandler(callback, 0)；
    
    char* strBuf = new char[len + 1];
    memcpy(strBuf, buffer, len);
    strBuf[len] = 0;
    
    HttpAgent* pAgent = canBeIgnored? &s_agent_canBeIgnore: &s_agent_normal;
    pAgent->send_lua((const char*)strBuf, len + 1, url, handler);
    
    delete [] strBuf;
    strBuf = NULL;

    return 1;
}


int resetHttpAgent(lua_State *L)
{
    s_agent_canBeIgnore.reset();
    s_agent_normal.reset();
    
    lua_pushinteger(L, 0);
    return 1;
}


/*
int getHttpResponseData(lua_State *L)
{
    bool canBeIgnore= lua_toboolean(L, 1);
    HttpResponse* lastResponse = canBeIgnore? s_lastResponse_canBeIgnore: s_lastResponse_normal;
    if(lastResponse)
    {
        lua_newtable(L);
        
        int i = 1;
        
        lua_pushnumber(L, i++);    //key
        lua_pushboolean(L, lastResponse->isSucceed());
        lua_settable(L, -3);       //push key,value
        
        lua_pushnumber(L, i++);    //key
        lua_pushinteger(L, lastResponse->getResponseCode());
        lua_settable(L, -3);       //push key,value
        
        // dump data
        std::vector<char>* pData = lastResponse->getResponseData();
        long dataLen = pData->size();

        char* pBuffer = new char[dataLen];
//        pBuffer[pData->size()] = 0;
        for (unsigned int i = 0; i < dataLen; i++)
        {
            pBuffer[i] = (*pData)[i];
        }

        lua_pushnumber(L, i++);    //key
        lua_pushlstring(L, pBuffer, dataLen);
//        lua_pushstring(L, lastResponse->getResponseDataString());
        lua_settable(L, -3);       //push key,value

        CC_SAFE_DELETE_ARRAY(pBuffer);

        lua_pushnumber(L, i++);    //key
        lua_pushinteger(L, dataLen);
        lua_settable(L, -3);       //push key,value

    }
    
    
    return 1;
}
*/

