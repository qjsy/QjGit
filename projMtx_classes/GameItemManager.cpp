//
//  GameItemManager.cpp
//  hero
//
//  Created by yangjie on 2014/01/12.
//
//

#include "GameItemManager.h"
#include "PropItem.h"
#include "EquipItem.h"

/* 静态实例变量 */
static GameItemManager* s_GameItemManager = NULL;

/*
 * 单例模式，获取 HeroManager 类唯一实例对象
 */
GameItemManager* GameItemManager::getInstance() {
    if (NULL == s_GameItemManager) {
        s_GameItemManager = new GameItemManager();
    }
    return s_GameItemManager;
}

/*
 *  构造函数
 */
GameItemManager::GameItemManager() {
    m_PropsByDict = CCDictionary::create();
    m_PropsByDict->retain();
    
    m_EquipsByDict = CCDictionary::create();
    m_EquipsByDict->retain();
    
}

/*
 *  析构函数
 */
GameItemManager::~GameItemManager() {
    removeAllGameItemData(true);
}

/*
 *  功能：
 *      清除所有游戏物品数据
 *  参数：
 *      isRelease   是否 release
 */
void GameItemManager::removeAllGameItemData(const bool isRelease) {
    if (m_PropsByDict) {
        m_PropsByDict->removeAllObjects();
        if (isRelease) {
            CC_SAFE_RELEASE_NULL(m_PropsByDict);
        }
    }
    if (m_EquipsByDict) {
        m_EquipsByDict->removeAllObjects();
        if (isRelease) {
            CC_SAFE_RELEASE_NULL(m_EquipsByDict);
        }
    }
}

/*
 *  功能：
 *      增加道具
 *  参数：
 *      pPropItem       道具数据项
 */
void GameItemManager::appendPropItem(PropItem* pPropItem) {
    CCAssert(pPropItem != NULL, "invalid PropItem");
    m_PropsByDict->setObject(pPropItem, pPropItem->getId());
}

/* 
 *  功能：
 *      获取道具数据 
 *  参数：
 *      propId      道具 id
 *  返回：
 *      成功返回道具数据，否则返回 NULL
 */
PropItem* GameItemManager::getPropItem(const unsigned int propId) {
    return static_cast<PropItem*>(m_PropsByDict->objectForKey(propId));
}

/* 
 *  功能：
 *      增加装备
 *  参数：
 *      pEquipItem  装备数据
 */
void GameItemManager::appendEquipItem(EquipItem* pEquipItem) {
    CCAssert(pEquipItem != NULL, "invalid PropItem");
    m_EquipsByDict->setObject(pEquipItem, pEquipItem->getId());
}

/*
 *  功能：
 *      获取装备数据
 *  参数：
 *      装备 id
 *  返回：
 *      成功返回装备数据，否则返回 NULL
 */
EquipItem* GameItemManager::getEquipItem(const unsigned int equipId) {
    return static_cast<EquipItem*>(m_EquipsByDict->objectForKey(equipId));
}


