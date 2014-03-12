//
//  AttackDataItem.h
//  hero
//
//  Created by yangjie on 2013/12/22.
//
//  攻击数据项
//

#ifndef __hero__AttackDataItem__
#define __hero__AttackDataItem__

#include "cocos2d.h"

USING_NS_CC;

class AttackDamageDataItem;
class SkillItem;
class AttackDataItem : public CCObject {
public:
    AttackDataItem();
    virtual ~AttackDataItem();

public:
    virtual bool init();
    
    CREATE_FUNC(AttackDataItem);
    
    /*
     *  属性
     */
    CC_PROPERTY(unsigned int, m_round, Round);                              // 攻击所属回合
    CC_PROPERTY(unsigned int, m_sequence, Sequence);                        // 本回合的第几轮攻击
    CC_PROPERTY(unsigned int, m_attacker, Attacker);                        // 进攻方  0:我方 1:对方
    CC_PROPERTY(unsigned int, m_attackerNowPos, AttackerNowPos);            // 进攻者进攻前（当时）所在位置
    CC_PROPERTY(std::string , m_attackerTag, AttackerTag);                  // 进攻者 tag
    CC_PROPERTY(unsigned int, m_completePos, CompletePos);                  // 进攻者放技能结束后所在位置
                                                                            // 为空的话返回 m_attackerNowPos
    CC_PROPERTY(unsigned int, m_skillPos, SkillPos);                        // 进攻者放技能所在位置
    CC_PROPERTY(SkillItem*, m_skillItem, SkillItem);                        // 技能

public:
    /* 添加攻击伤害数据 */
    void appendAttackDamageData(AttackDamageDataItem*);
    /* 获取本次攻击包含的攻击伤害数量 */
    inline unsigned int getCountOfAttackDamageData() {return m_AttackDamageDataItems->count();}
    /* 获取攻击伤害数据 */
    AttackDamageDataItem* getAttackDamageData(const int);
    
private:
    CCArray* m_AttackDamageDataItems;                       // 攻击伤害数据
};

#endif /* defined(__hero__AttackDataItem__) */

