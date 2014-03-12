//
//  NetConnection.h
//  hero
//
//  Created by yangjie on 2013-12-03.
//
//  网络连接类
//
//  负责和 web 服务器的交互
//

#ifndef __hero__NetConnection__
#define __hero__NetConnection__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

// web 服务器 url 地址大小
#define MAX_WEB_SERVER_URL_SIZE         256

class HandleNetDataDelegate;
class NetConnection : public CCObject {
private:    /*禁止外部 new*/
    NetConnection();

public:
    virtual ~NetConnection();

public:
    virtual bool init();
    CREATE_FUNC(NetConnection);
    
public:
    // 单例模式，获取本类唯一实例对象
    static NetConnection* getInstance();

public:
    /* 向 web 服务器提交 get 请求 */
    void commitGetRequestData(char [], char []);
    /* 向 web 服务器提交 post 请求 */
    void commitPostRequestData(char [], char []);
    
    /* 响应 web服务器回应请求的回调方法 */
    void onHttpRequestResponseCallback(CCHttpClient*, CCHttpResponse*);

public:
    /* 设置 web服务器 url 地址 */
    void setWebServerUrl(char szWebServerUrl[]);
    
    /* 设置处理 web 服务器回应数据的代理协议对象*/
    inline void setHandleNetDataDelegate(HandleNetDataDelegate *pHandleNetDataDelegate)
        {m_pHandleNetDataDelegate = pHandleNetDataDelegate;}

public:
    inline bool isPending() {return m_isPending;}
    
private:
    /* web server url 地址 */
    char m_szWebServerUrl[MAX_WEB_SERVER_URL_SIZE + 4];
    
    /* 
     * 是否处于 “未决” 状态
     * 所谓 “未决” 状态是指在向 web服务器提交
     * get 或 post 请求和直到从 web 服务器得
     * 到回复数据之间的状态
    */
    bool m_isPending;
    ///////////////
public:
    void setUUU(const char*);
    // 临时的服务器地址变量，之后要删除
    const char* url;
private:
    /* 处理 web 服务器回应数据的代理协议对象 */
    HandleNetDataDelegate* m_pHandleNetDataDelegate;
};

#endif /* defined(__hero__NetConnection__) */
