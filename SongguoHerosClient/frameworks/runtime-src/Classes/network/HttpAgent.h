//
//  HttpAgent.h
//  Game
//
//  Created by fu.chenhao on 3/19/15.
//
//

#ifndef __Game__HttpAgent__
#define __Game__HttpAgent__

#include <stdio.h>
#include <cocos2d.h>

#include "network/HttpClient.h"
#include "network/HttpRequest.h"


using namespace cocos2d::network;

class HttpAgent
{
private:
    struct Segment
    {
        std::string m_url;
        int m_leftRetryTimes = 0;

        ccHttpRequestCallback m_callback = NULL;
        std::string m_luaHandler;
        
        HttpRequest* m_pRequest = NULL;
    };
    
    bool m_canBeIgnored = false;
    HttpClient *m_httpClient;

    Segment* m_pCurrentSegment = NULL;
    std::vector<Segment*> m_segmentQueue;
    
    void onHttpRequestCompleted(HttpClient *sender, HttpResponse *response);
    void trySendNext();
    void correctRequest(HttpRequest* m_pRequest);
    
    void callbackLua(HttpClient* client, HttpResponse *response);
    
public:
    HttpAgent(bool canBeIgnored = false);
    ~HttpAgent();
    
    inline HttpClient* getClient() { return m_httpClient; }
    
    void send(const char* buffer, size_t len, const char* url, const ccHttpRequestCallback& callback);
    void send_lua(const char* buffer, size_t len, const char* url, const char* luaHandler);

    void reset();
    
};


extern "C" int sendHttp(const char* buffer, size_t len, const char* url, const ccHttpRequestCallback& callback, bool canBeIgnored);
extern "C" int sendHttp_lua(lua_State *L);
extern "C" int resetHttpAgent(lua_State *L);

//extern "C" int getHttpResponseData(lua_State *L);


#endif /* defined(__Game__HttpAgent__) */
