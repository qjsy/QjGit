//
//  ServerManager.cpp
//  hero
//
//  Created by yangjie on 2014/02/17.
//
//

#include "ServerManager.h"
#include "ServerItem.h"

#define SERVER_NUMBERS          128         // 服务器数量

/* 服务器信息管理 */
static ServerManager* s_ServerManager = NULL;

/*
 *  获取服务器管理对象单例
 */
ServerManager* ServerManager::getInstance() {
    return NULL == s_ServerManager ? (s_ServerManager = new ServerManager()) : s_ServerManager;
}

/*
 *  构造函数
 */
ServerManager::ServerManager() {
    m_serverItems = CCArray::createWithCapacity(SERVER_NUMBERS);
    m_serverItems->retain();
}

/*
 *  析构函数
 */
ServerManager::~ServerManager() {
    removeAllServerItem(true);
}

/* 清除所有服务器信息 */
void ServerManager::removeAllServerItem(const bool isRelease) {
    if (m_serverItems) {
        m_serverItems->removeAllObjects();
        if (isRelease) {
            m_serverItems->release();
        }
    }
}

/* 
 *  功能:
 *      添加服务器
 *  参数：
 *      pServerItem     要添加的服务器
 *  返回:
 *      添加成功返回 true，否则返回 false
 */
bool ServerManager::appendServerItem(ServerItem* pServerItem) {
    CCAssert(pServerItem, "invalid data");
    
    bool hasRepeat = false;
    CCObject* pObject = NULL;
    ServerItem* pTempServerItem = NULL;
    CCARRAY_FOREACH(m_serverItems, pObject) {
         pTempServerItem = (ServerItem*)pObject;
        if (pServerItem->getId() == pTempServerItem->getId()) {
            hasRepeat = true;
            break;
        }
    } /*for*/
    
    if (!hasRepeat)
        m_serverItems->addObject(pServerItem);

    return !hasRepeat;
}

/* 
 *  功能：
 *      根据索引号 获取服务器信息 
 *  参数：
 *      index     索引号
 *  返回：
 *      成功返回服务器信息，否则返回 NULL
 */
ServerItem* ServerManager::getServerItemByIndex(const unsigned int index) const {
    CCAssert(m_serverItems, "invalid data");
    if (m_serverItems->count() == 0)
        return NULL;    // 没有服务器信息
    if (index > m_serverItems->count() - 1)
        return NULL;
    
    return (ServerItem*)m_serverItems->objectAtIndex(index);
}

/* 
 *  功能:
 *      根据服务器 id 获取服务器信息 
 *  参数:
 *      serverId    服务器id
 *  返回:
 *      成功返回服务器信息，否则返回 NULL
 */
ServerItem* ServerManager::getServerItemById(const unsigned int serverId) const {
    CCAssert(m_serverItems, "invalid data");
    
    CCObject* pObject = NULL;
    ServerItem* pTempServerItem = NULL;
    ServerItem* pRetServerItem = NULL;
    
    CCARRAY_FOREACH(m_serverItems, pObject) {
        pTempServerItem = (ServerItem*)pObject;
        if (pTempServerItem->getId() == serverId) {
            pRetServerItem = pTempServerItem;
            break;
        }
    } /*for*/
    return pRetServerItem;
}

