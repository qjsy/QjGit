//
//  DropItemManager.h
//  hero
//
//  Created by yangjie on 2014/02/16.
//
//  掉落物品管理器
//

#ifndef __hero__DropItemManager__
#define __hero__DropItemManager__

#include "cocos2d.h"

USING_NS_CC;

class DropItem;
class DropItemManager {
private:    /*禁止外部 new*/
    DropItemManager();
    
public:
    virtual ~DropItemManager();
    
    /* 单例模式，获取 DropItemManager 类唯一实例对象 */
    static DropItemManager* getInstance();
    
public:
    /* 清除所有掉落物品 */
    void removeAllDropItem(bool isRelease = false);
    /* 添加掉落物品 */
    void appendDropItem(DropItem*);
    /* 获取掉落物品数量 */
    inline unsigned int getCountOfDropItems() const {return m_dropItems->count();}
    /* 获取掉落物品 */
    DropItem* getDropItem(const unsigned int);
    
private:
    CCArray* m_dropItems;       // 存储所有掉落物品
};

#endif /* defined(__hero__DropItemManager__) */
