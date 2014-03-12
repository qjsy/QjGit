//
//  InBattleItem.h
//  hero
//
//  Created by yangjie on 2014/02/23.
//
//

#ifndef __hero__InBattleItem__
#define __hero__InBattleItem__

#include "cocos2d.h"

USING_NS_CC;

class InBattleItem : public CCObject {
public:
    InBattleItem();
    virtual ~InBattleItem() {}

public:
    virtual inline bool isMonster() = 0;
    virtual int getHP() = 0;
    
    /*
     * 属性定义
     */
    CC_PROPERTY(unsigned int, m_PosInBattle, PosInBattle);                  // 在战场上的位置
    /* 掉落物品相关 */
    CC_PROPERTY(bool, m_hasDrop, HasDrop);                                  // 是否有掉落物品
    CC_PROPERTY(std::string, m_dropItemType, DropItemType);                 // 掉落物品类别
    CC_PROPERTY(std::string, m_dropItemId, DropItemId);                     // 掉落物品 ID
    CC_PROPERTY(unsigned int, m_countOfdropItem, CountOfdropItem);          // 掉落物品数量
    CC_PROPERTY(unsigned int, m_dropItemLevel, DropItemLevel);              // 掉落物品 level
};

#endif /* defined(__hero__InBattleItem__) */
