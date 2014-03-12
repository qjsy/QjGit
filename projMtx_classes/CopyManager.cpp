//
//  CopyManager.cpp
//  hero
//
//  Created by yangjie on 2013/12/23.
//
//

#include "CopyManager.h"
#include "CopyDataItem.h"
#include "LevelDataItem.h"

/* 静态实例变量 */
static CopyManager* s_CopyManager = NULL;

/*
 * 单例模式，获取 CopyManager 类唯一实例对象
 */
CopyManager* CopyManager::getInstance() {
    if (NULL == s_CopyManager) {
        s_CopyManager = new CopyManager();
    }
    return s_CopyManager;
}

/*
 *  构造函数
 */
CopyManager::CopyManager() {
    m_copys = CCDictionary::create();
    m_copys->retain();
}

/*
 *  析构函数
 */
CopyManager::~CopyManager() {
    removeAllCopyData(true);
}

/*
 *  功能：
 *      清除所有副本数据
 *
 *  参数：
 *      isRelease   ->  是否释放资源
 */
void CopyManager::removeAllCopyData(bool isRelease) {
    if (m_copys) {
        m_copys->removeAllObjects();
        if (isRelease) {
            m_copys->release();
            m_copys = NULL;
        }
    }
}

/* 
 *  功能：
 *      添加副本数据
 *  参数：
 *      pCopyDataItem   副本数据项
 */
void CopyManager::appendCopyData(CopyDataItem* pCopyDataItem) {
    CCAssert(m_copys && pCopyDataItem, "invalid data");
    
    m_copys->setObject(pCopyDataItem, pCopyDataItem->getId());
}

/*
 *  功能：
 *      添加关卡数据
 *  参数：
 *      copyId              关卡所属副本 id
 *      pLevelDataItem      关卡数据项目
 */
void CopyManager::appendLevelData(const unsigned int copyId, LevelDataItem* pLevelDataItem) {
    CCAssert(m_copys && pLevelDataItem, "invalid data");
    
    CopyDataItem* pCopyDataItem = getCopyData(copyId);
    if (pCopyDataItem) {
        pCopyDataItem->appendLevelData(pLevelDataItem);
    }
}

/*
 *  功能：
 *      获取副本的关卡数量
 *  参数：
 *      copyId      副本 id 号
 *  返回：
 *      返回副本关卡数量
 */
unsigned int CopyManager::getCountOfLevels(const unsigned int copyId) const {
    CCAssert(m_copys, "invalid data");
    
    CopyDataItem* pCopyDataItem = (CopyDataItem*)m_copys->objectForKey(copyId);
    if (!pCopyDataItem)
        return 0;
    return pCopyDataItem->getCountOfLevel();
}

/*
 *  功能：
 *      获取副本数据项
 *  参数：
 *      copyId      副本 id 号
 *  返回：
 *      成功返回副本数据项，失败返回 NULL
 */
CopyDataItem* CopyManager::getCopyData(const unsigned int copyId) {
    CCAssert(m_copys, "invalid data");
    
    return (CopyDataItem*)m_copys->objectForKey(copyId);
}

/*
 *  功能：
 *      根据关卡 id 获取关卡数据项
 *  参数：
 *      copyId      副本 id 号
 *  返回：
 *      成功返回关卡数据项，失败返回 NULL
 */
LevelDataItem* CopyManager::getLevelDataById(const unsigned int copyId, const unsigned int levelId) {
    CCAssert(m_copys, "invalid data");
    
    LevelDataItem* pLevelDataItem = NULL;
    CopyDataItem* pCopyDataItem = (CopyDataItem*)m_copys->objectForKey(copyId);
    
    do {
        CC_BREAK_IF(NULL == pCopyDataItem);
        pLevelDataItem = pCopyDataItem->getLevelDataById(levelId);
    } while (0);
    
    return pLevelDataItem;
}

/*
 *  功能：
 *      根据关卡在副本中的索引号获取关卡数据项
 *  参数：
 *      copyId      副本 id 号
 *
 *  返回：
 *      成功返回关卡数据项，失败返回 NULL
 */
LevelDataItem* CopyManager::getLevelDataByIndex(const unsigned int copyId, const unsigned int levelIndex) {
    CCAssert(m_copys, "invalid data");
    
    LevelDataItem* pLevelDataItem = NULL;
    CopyDataItem* pCopyDataItem = (CopyDataItem*)m_copys->objectForKey(copyId);
    
    do {
        CC_BREAK_IF(NULL == pCopyDataItem);
        pLevelDataItem = pCopyDataItem->getLevelDataByIndex(levelIndex);
    } while (0);
    
    return pLevelDataItem;
}


