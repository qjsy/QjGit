//
//  ServerItem.h
//  hero
//
//  Created by yangjie on 2014/02/14.
//
//  服务器
//

#ifndef __hero__ServerItem__
#define __hero__ServerItem__

#include "cocos2d.h"
#include "common.h"

USING_NS_CC;

/*
 *  服务器信息
 */
class ServerItem : public CCObject {
public:
    ServerItem() {}
    virtual ~ServerItem() {}
    
public:
    virtual bool init();
    
    CREATE_FUNC(ServerItem);
    
public:
    
    ////////////////////////
    //  以下是服务器属性定义   //
    ///////////////////////
    CC_PROPERTY(unsigned int, m_id, Id);                            // 服务器id(唯一)
    CC_PROPERTY(std::string, m_name, Name);                         // 服务器名称
    CC_PROPERTY(std::string, m_url;, Url);                          // 服务器url
    CC_PROPERTY(unsigned int, m_state;, State);                     // 服务器状态
};

#endif /* defined(__hero__ServerItem__) */
