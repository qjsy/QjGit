//
//  AttackDamageDataItem.h
//  hero
//
//  Created by yangjie on 2013/12/26.
//
//  攻击伤害数据
//

#ifndef __hero__AttackDamageDataItem__
#define __hero__AttackDamageDataItem__

#include "cocos2d.h"

USING_NS_CC;
class AttackDamageDataItem : public CCObject {
public:
    AttackDamageDataItem() {};
    virtual ~AttackDamageDataItem() {};
    
public:
    virtual bool init();
    
    CREATE_FUNC(AttackDamageDataItem);
    
    /*
     *  属性
     */
    CC_PROPERTY(unsigned int, m_harmer, Harmer);                        // 被伤害方  0:我方 1:敌方
    CC_PROPERTY(unsigned int, m_posIndexInBattle, PosIndexInBattle);    // 被伤害方（当时）所在位置索引
    CC_PROPERTY(std::string, m_harmerID, HarmerID);                     // 被伤害方 id 值
    CC_PROPERTY(int, m_damageValue, DamageValue);                       // 伤害值，0 表示 miss，负数表示加血
    CC_PROPERTY(bool, m_isCrit, Crit);                                  // 是否爆击
    CC_PROPERTY(int, m_remainHp, RemainHp);                             // 被伤害武将剩余生命值
};

#endif /* defined(__hero__AttackDamageDataItem__) */

