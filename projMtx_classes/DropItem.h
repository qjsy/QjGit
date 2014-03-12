//
//  DropItem.h
//  hero
//
//  Created by yangjie on 2014/02/16.
//
//  掉落物品
//

#ifndef __hero__DropItem__
#define __hero__DropItem__

#include "cocos2d.h"
#

USING_NS_CC;

/*
 *  掉落物品类型
 */
typedef enum {
    DT_NONE = 0,
    DT_HERO = 1,    // 武将
    DT_PROP,        // 道具
    DT_EQUIP        // 装备
} DROP_ITEM_TYPE;

class DropItem : public CCObject {
public:
    DropItem();
    virtual ~DropItem();
    
    virtual bool init();
    CREATE_FUNC(DropItem);
    
    /*
     *  属性定义
     */
    CC_PROPERTY(unsigned int, m_id, Id);                    // 掉落物品id
    CC_PROPERTY(DROP_ITEM_TYPE, m_type, Type);              // 掉落物品类型
    CC_PROPERTY(unsigned int, m_heroLevel, HeroLevel);      // 掉落武将等级(掉落物品是武将)
};

#endif /* defined(__hero__DropItem__) */

