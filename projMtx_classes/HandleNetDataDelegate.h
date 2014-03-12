//
//  HandleNetDataDelegate.h
//  hero
//
//  Created by yangjie on 2013-12-03.
//
// 处理 web 服务器回应数据的代理协议（抽象类）
//

#ifndef hero_HandleNetDataDelegate_h
#define hero_HandleNetDataDelegate_h

class HandleNetDataDelegate {
public:
    /*处理 web 服务器响应 get 请求回应数据*/
    virtual void handleWSResponseData(const char *szData, const unsigned int dataSize, char* szTag) = 0;
};

#endif
