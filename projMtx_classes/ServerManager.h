//
//  ServerManager.h
//  hero
//
//  Created by yangjie on 2014/02/17.
//
//

#ifndef __hero__ServerManager__
#define __hero__ServerManager__

#include "cocos2d.h"

USING_NS_CC;

class ServerItem;
class ServerManager {
private:    /*禁止外部 new*/
    ServerManager();
    
public:
    virtual ~ServerManager();
    
    /* 单例模式，获取 ServerManager 类唯一实例对象 */
    static ServerManager* getInstance();
    
public:
    /* 清除所有服务器信息 */
    void removeAllServerItem(const bool isRelease = false);
    /* 添加服务器 */
    bool appendServerItem(ServerItem*);
    /* 获取服务器数量 */
    inline unsigned int getCountOfServerItems() const {return m_serverItems->count();}
    /* 根据索引号 获取服务器信息 */
    ServerItem* getServerItemByIndex(const unsigned int) const;
    /* 根据服务器 id 获取服务器信息 */
    ServerItem* getServerItemById(const unsigned int) const;
    
private:
    CCArray* m_serverItems;       // 存储所有服务器信息
};

#endif /* defined(__hero__ServerManager__) */
