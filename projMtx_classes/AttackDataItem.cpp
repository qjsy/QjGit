//
//  AttackDataItem.cpp
//  hero
//
//  Created by yangjie on 2013/12/22.
//
//

#include "AttackDataItem.h"
#include "AttackDamageDataItem.h"
#include "SkillItem.h"

/*
 *  构造函数
 */
AttackDataItem::AttackDataItem() {
    m_AttackDamageDataItems = CCArray::create();
    m_AttackDamageDataItems->retain();
}

/*
 *  析构函数
 */
AttackDataItem::~AttackDataItem() {
    m_AttackDamageDataItems->removeAllObjects();
    m_AttackDamageDataItems->release();
}

/*
 * 初始化
 */
bool AttackDataItem::init() {
    m_round = 0;
    m_sequence = 0;
    m_attacker = 0;
    m_attackerTag = "";
    m_completePos = 0;
    m_skillPos = 0;
    m_skillItem = NULL;
    
    return true;
}

// 获取和设置攻击所属回合
unsigned int AttackDataItem::getRound() {return m_round;}
void AttackDataItem::setRound(unsigned int var) {m_round = var;}
// 获取和设置本回合的第几轮攻击
unsigned int AttackDataItem::getSequence() {return m_sequence;}
void AttackDataItem::setSequence(unsigned int var) {m_sequence = var;}
// 获取和设置进攻方
unsigned int AttackDataItem::getAttacker() {return m_attacker;}
void AttackDataItem::setAttacker(unsigned int var) {m_attacker = var;}
// 进攻者进攻前（当时）所在位置
unsigned int AttackDataItem::getAttackerNowPos() {return m_attackerNowPos;}
void AttackDataItem::setAttackerNowPos(unsigned int var) {m_attackerNowPos = var;}
// 获取和设置进攻武将 tag
std::string AttackDataItem::getAttackerTag() {return m_attackerTag;}
void AttackDataItem::setAttackerTag(std::string var) {m_attackerTag = var;}
// 获取和设置进攻者放技能结束后所在位置
unsigned int AttackDataItem::getCompletePos() {return m_completePos;}
void AttackDataItem::setCompletePos(unsigned int var) {m_completePos = var;}
// 获取和设置放技能所在位置
unsigned int AttackDataItem::getSkillPos() {return m_skillPos;}
void AttackDataItem::setSkillPos(unsigned int var) {m_skillPos = var;}
// 获取和设置技能数据
SkillItem* AttackDataItem::getSkillItem() {return m_skillItem;}
void AttackDataItem::setSkillItem(SkillItem* var) {m_skillItem = var;}

/*
 *  功能：
 *      添加攻击伤害数据
 *  参数：
 *      pAttackDamageDataItem   攻击伤害数据
 */
void AttackDataItem::appendAttackDamageData(AttackDamageDataItem* pAttackDamageDataItem) {
    CCAssert(pAttackDamageDataItem, "invalid data");
    m_AttackDamageDataItems->addObject(pAttackDamageDataItem);
}

/* 
 *  功能：
 *      获取攻击伤害数据
 *  参数：
 *      index       伤害数据索引号
 *  返回：
 *      成功返回攻击伤害数据，否则返回 NULL
 */
AttackDamageDataItem* AttackDataItem::getAttackDamageData(const int index) {
    return index >= m_AttackDamageDataItems->count() ? NULL :
        static_cast<AttackDamageDataItem*>(m_AttackDamageDataItems->objectAtIndex(index));
}

