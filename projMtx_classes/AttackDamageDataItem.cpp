//
//  AttackDamageDataItem.cpp
//  hero
//
//  Created by yangjie on 2013/12/26.
//
//

#include "AttackDamageDataItem.h"

/*
 *  初始化
 */
bool AttackDamageDataItem::init() {
    m_harmer = 0;
    m_damageValue = 0;
    m_isCrit = false;
    m_remainHp = 0;
    
    return true;
}

/* 获取和设置被伤害方 */
unsigned int AttackDamageDataItem::getHarmer() {return m_harmer;}
void AttackDamageDataItem::setHarmer(unsigned int var) {m_harmer = var;}
/* 获取和设置被伤害方（当时）所在位置索引 */
unsigned int AttackDamageDataItem::getPosIndexInBattle() {return m_posIndexInBattle;}
void AttackDamageDataItem::setPosIndexInBattle(unsigned int var) {m_posIndexInBattle = var;}
/* 获取和设置被伤害方 id 值 */
std::string AttackDamageDataItem::getHarmerID() {return m_harmerID;}
void AttackDamageDataItem::setHarmerID(std::string var) {m_harmerID = var;}
/* 获取和设置伤害值 */
int AttackDamageDataItem::getDamageValue() {return m_damageValue;}
void AttackDamageDataItem::setDamageValue(int var) {m_damageValue = var;}
/* 获取和设置是否爆击 */
bool AttackDamageDataItem::getCrit() {return m_isCrit;}
void AttackDamageDataItem::setCrit(bool var) {m_isCrit = var;}
/* 获取和设置被伤害武将(怪物)剩余生命值 */
int AttackDamageDataItem::getRemainHp() {return m_remainHp;}
void AttackDamageDataItem::setRemainHp(int var) {m_remainHp = var;}

