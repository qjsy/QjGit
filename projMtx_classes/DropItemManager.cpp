//
//  DropItemManager.cpp
//  hero
//
//  Created by yangjie on 2014/02/16.
//
//

#include "DropItemManager.h"
#include "DropItem.h"

/* 静态实例变量 */
static DropItemManager* s_DropItemManager = NULL;

/*
 * 单例模式，获取 HeroManager 类唯一实例对象
 */
DropItemManager* DropItemManager::getInstance() {
    if (NULL == s_DropItemManager) {
        s_DropItemManager = new DropItemManager();
    }
    return s_DropItemManager;
}

/*
 *  构造函数函数
 */
DropItemManager::DropItemManager() {
    m_dropItems = CCArray::createWithCapacity(16);
    m_dropItems->retain();
}

/*
 *  析构函数
 */
DropItemManager::~DropItemManager() {
    removeAllDropItem(true);
}

/*
 *  功能：
 *      清除所有掉落物品数据
 *  参数：
 *      isRelease   ->  是否释放资源
 */
void DropItemManager::removeAllDropItem(bool isRelease) {
    if (m_dropItems) {
        m_dropItems->removeAllObjects();
        
        if (isRelease) {
            m_dropItems->release();
        }
    }
}

/* 
 *  功能；
 *      添加掉落物品
 *  参数:
 *      pDropItem   掉落物品
 */
void DropItemManager::appendDropItem(DropItem* pDropItem) {
    CCAssert(pDropItem, "invalid dropItem");
    m_dropItems->addObject(pDropItem);
}

/*
 *  功能：
 *      获取掉落物品 
 *  参数：
 *      index       索引号
 *  返回:
 *      成功返回掉落物品，否则返回 NULL
 */
DropItem* DropItemManager::getDropItem(const unsigned int index) {
    if (index < m_dropItems->count()) {
        return (DropItem*)m_dropItems->objectAtIndex(index);
    }
    return NULL;
}


