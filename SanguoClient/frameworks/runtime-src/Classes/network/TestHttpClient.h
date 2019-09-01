//
//  TestHttpClient.h
//  Game
//
//  Created by fu.chenhao on 3/18/15.
//
//

#ifndef __Game__TestHttpClient__
#define __Game__TestHttpClient__

#include <stdio.h>
#include <cocos2d.h>

#include "HttpAgent.h"
#include "HttpClient.h"
#include "HttpRequest.h"



using namespace cocos2d::network;

class TestHttpClient
{
public:
    
    TestHttpClient(){}
    
    void startTestAgent()
    {
        const char* content = "";

        HttpAgent *agent = new HttpAgent();
//        agent->send(content, sizeof(content), "http://dyss.dev.api.zjfygames.com/test.php", CC_CALLBACK_2(TestHttpClient::onHttpRequestCompleted, this));
        
        
//        sendHttp(content, sizeof(content), "http://dyss.dev.api.zjfygames.com/test.php", CC_CALLBACK_2(TestHttpClient::onHttpRequestCompleted, this), false);
        sendHttp(content, strlen(content),
                 "http://dyss.dev.api.zjfygames.com/adventure.pb?_a=stageConfirm&snId=2",
//                 "http://www.httpbin.org/post",
                 CC_CALLBACK_2(TestHttpClient::onHttpRequestCompleted, this), false);
    }
    
    void startTest()
    {
//        //获得CCHttpClient的实例，这里写成了单例类
//        HttpClient *httpClient = HttpClient::getInstance();
//        
//        //new一个CCHttpRequest,创建一个http请求
//        HttpRequest *request = new HttpRequest();
//        /*
//         
//         设置 HttpRequestType ,其中有Get 方式，Post方式，还有一个unknown(没想到他还给写错了,少写了一个字母n)
//         
//         typedef enum
//         {
//         kHttpGet,
//         kHttpPost,
//         kHttpUnkown,
//         } HttpRequestType;
//         
//         */
//        
//        //这里我们用的是Get方式
//        request->setRequestType(HttpRequest::Type::GET);
//        
//        //设置我们想要连接的URl地址，我们这里想要连接百度，也就是 http://www.baidu.com
//        request->setUrl("http://www.baidu.com");
//        
//        //设置回调函数，这个回调函数是说当服务器响应Get请求后下发的数据
//        request->setResponseCallback(this, callfuncND_selector(TestConnectToBaidu::httpRequestFinish));
//        
//        //设置tag，我们可以设置一个字符串作为tag，这个tag我们将在返回数据的response中得到
//        request->setTag("Connection_To_Baidu");
//        
//        //设置响应时间
//        httpClient->setTimeoutForConnect(30);
//        
//        //设置发送数据，当我们与服务器进行交互式，可以把数据放在这里，基本上都是Post方式向服务器发送数据，这里我们就不使用了
//        //    request->setRequestData("", 0);
//        
//        
//        //好了，请求包我们已经封装好了，下面我们就把它发送出去
//        httpClient->send(request);
//        
//        //记得把 new 出来的request请求release掉，不可以用autorelease
//        request->release();
        
        
        /////////////////
        
//        cocos2d::extension::HttpRequest* request = new cocos2d::extension::HttpRequest();
        
        //获得CCHttpClient的实例，这里写成了单例类
        HttpClient *httpClient = HttpClient::getInstance();
        
        //new一个CCHttpRequest,创建一个http请求
        HttpRequest *request = new HttpRequest();

        request->setRequestType(HttpRequest::Type::GET);
        request->setUrl("http://www.httpbin.org/get");
        
        request->setRequestData("abc", 3);
//        request->setResponseCallback(httpresponse_selector(TestHttpClient::onHttpRequestCompleted));
//        request->setResponseCallback(this,httpresponse_selector(TestHttpClient::onHttpRequestCompleted));
//        HttpRequest* request = new HttpRequest();//使用HttpRequest无参的构造函数，提供了一个很好的默认设置
//        request->setRequestType(cocos2d::network::HttpRequest::Type::GET);//设置连接方式，常用GET和POST
//        request->setUrl(url.c_str());//设置连接地址
        //设置回调，在onHttpRequestCompleted中读取数据
        request->setResponseCallback(CC_CALLBACK_2(TestHttpClient::onHttpRequestCompleted, this));
        request->setTag("a");//设置Tag
//        network::HttpClient::getInstance()->send(request);//添加到HttpClient任务队列
//        request->release();//释放连接
//        request->setResponseCallback(ccHttpRequestCallback());
        
        httpClient->send(request);
        
        request->release();
    }
    
    void onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
    {
        if (!response)
        {
            return;
        }
        // You can get original request type from: response->request->reqType
//        if (0 != strlen(response->getHttpRequest()->getTag()))
//        {
//            CCLOG("%s completed", response->getHttpRequest()->getTag());
//        }
        
        int statusCode = response->getResponseCode();
        char statusString[128] = {};
        sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());
//        _labelStatusCode->setString(statusString);
        CCLOG("response code: %s", statusString);

        if (!response->isSucceed())
        {
            CCLOG("response failed");
            CCLOG("error buffer: %s", response->getErrorBuffer());
            return;
        }
        
        // dump data
        std::vector<char> *buffer = response->getResponseData();
        int bufferLen = buffer->size();
        printf("Http Test, dump data, len = %i \n", bufferLen);
        char page[10240];
        memset(page, 0, sizeof(page));
        for (unsigned int i = 0; i < MIN(bufferLen, sizeof(page)); i++)
        {
            page[i] = (*buffer)[i];
//            printf("%i", (*buffer)[i]);
        }
        printf("%s\n", page);
        printf("=========end");
        printf("");
    }
    
};



#endif /* defined(__Game__TestHttpClient__) */
