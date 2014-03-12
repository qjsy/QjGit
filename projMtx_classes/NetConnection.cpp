//
//  NetConnection.cpp
//  hero
//
//  Created by yangjie on 2013-12-03.
//
//

#include "NetConnection.h"
#include "HandleNetDataDelegate.h"
#include "customMessage.h"

#define HTTP_CONNECTION_TIMEOUT     15                                  // 连接服务器超时时间
#define DEFAULT_REQUEST_TAG         "cooling"                           // 缺省 tag 值

/* 静态实例变量 */
static NetConnection* s_NetConnection = NULL;

/*
 * 默认处理网络数据的函数（输出到 log 窗口）
 *
 * 参数
 *      szNetworkData       网络数据
 *      data_size           数据大小
 *      szTag               tag 值
 */
static void defaultHandleNetworkData(const std::string &szNetworkData, const int data_size, char szTag[]) {
    if (data_size > 0) {
        // CCLog("recv size = %d", data_size);
        // CCLog("%s", szNetworkData.c_str());
    }
}

/*
 * 单例模式，获取 NetConnection 类唯一实例对象
 */
NetConnection* NetConnection::getInstance() {
    if (NULL == s_NetConnection) {
        s_NetConnection = new NetConnection();
        s_NetConnection->init();
    }
    return s_NetConnection;
}

/*
 * 构造函数
 */
NetConnection::NetConnection():m_isPending(false),m_pHandleNetDataDelegate(NULL) {
    for (int i = 0; i < sizeof(m_szWebServerUrl); i++) {
        m_szWebServerUrl[i] = '\0';
    }
}

/*
 * 析构函数
 */
NetConnection::~NetConnection() {}

/*
 * 初始化方法
 */
bool NetConnection::init() {
    /*...*/
    return true;
}

/*
 * 设置 web 服务器 Url 地址
 */
void NetConnection::setWebServerUrl(char szWebServerUrl[]) {
    if (szWebServerUrl) {
        for (int i = 0; i < MAX_WEB_SERVER_URL_SIZE; i++) {
            m_szWebServerUrl[i] = szWebServerUrl[i];
            if ('\0' == szWebServerUrl[i]) {
                break;
            }
        } /*for*/
    }
}

/*
 *  向 web 服务器提交 get 请求
 */
void NetConnection::commitGetRequestData(char szGetRequestData[], char szRequestTag[]) {
    if (m_isPending)
        return;

    CCHttpClient* httpClient = CCHttpClient::getInstance();
    CCHttpRequest* httpRequest = new CCHttpRequest();
    
    httpRequest->setRequestType(CCHttpRequest::kHttpGet);
    httpRequest->setUrl(m_szWebServerUrl[0] == '\0' ? url : m_szWebServerUrl);
    httpRequest->setResponseCallback(this, httpresponse_selector(NetConnection::onHttpRequestResponseCallback));
    httpRequest->setTag(szRequestTag ? szRequestTag : DEFAULT_REQUEST_TAG);
    
    httpClient->setTimeoutForConnect(HTTP_CONNECTION_TIMEOUT);
    httpClient->send(httpRequest);
    httpRequest->release();

    m_isPending = true;
}

/*
 *  向 web 服务器提交 post 请求
 *
 *  参数：
 *      szPosData       附加的 post 数据
 *      szRequestTag    tag 值
 */
void NetConnection::commitPostRequestData(char szPostRequestData[], char szRequestTag[]) {
    if (m_isPending)
        return;
    
    CCHttpClient* httpClient = CCHttpClient::getInstance();
    CCHttpRequest* httpRequest = new CCHttpRequest();
    httpRequest->setUrl(m_szWebServerUrl[0] == '\0' ? url : m_szWebServerUrl);
    httpRequest->setRequestType(CCHttpRequest::kHttpPost);
    httpRequest->setResponseCallback(this, (SEL_HttpResponse)httpresponse_selector(NetConnection::onHttpRequestResponseCallback));
    httpRequest->setRequestData(szPostRequestData, strlen(szPostRequestData));
    httpRequest->setTag(szRequestTag ? szRequestTag : DEFAULT_REQUEST_TAG);
    
    httpClient->setTimeoutForConnect(HTTP_CONNECTION_TIMEOUT);
    httpClient->send(httpRequest);
    httpRequest->release();
    
    m_isPending = true;
}

/*
 * 功能：
 *      响应 web 服务器请求回应的回调方法
 *  
 * 参数：
 *      client      http 客户
 *      response    http 响应
 * commitPostRequestData 调用回调，onHttpRequestResponseCallback 回调再调用 handleWSResponseData
 */
void NetConnection::onHttpRequestResponseCallback(CCHttpClient* client, CCHttpResponse* response) {
    m_isPending = false;
    
    int statusCode = response->getResponseCode();
    bool responseIsSuccess = false;
    
    do {
        CC_BREAK_IF(200 != statusCode);
        CC_BREAK_IF(!(responseIsSuccess = response->isSucceed()));
        
        char* tag = (char *)response->getHttpRequest()->getTag();
        
        // 获取 web 服务器响应数据
        std::vector<char> *responseData = response->getResponseData();
        int data_size = responseData->size();
        
        std::string res;
        res.assign( &(*responseData)[0], data_size);
        res += '\0';
        // 设置了代理，用代理调用 抽象类的handleWsResponseData
        if (m_pHandleNetDataDelegate) {
            m_pHandleNetDataDelegate->handleWSResponseData(res.c_str(), data_size + 1, tag);
        } else {
            defaultHandleNetworkData(res, data_size, tag);
        }
    } while (0);

    if (!responseIsSuccess) {
        CCLog("Received Error! statusCode = %d errorMessage = %s\n", statusCode, response->getErrorBuffer());
        CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_NETWORK_EXCEPTION);
        return ;
    }
}

///////
void NetConnection::setUUU(const char* varurl) {
    url = varurl;
}
