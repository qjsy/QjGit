//
//  InBattleItem.cpp
//  hero
//
//  Created by yangjie on 2014/02/23.
//
//

#include "InBattleItem.h"

/*
 *  构造函数
 */
InBattleItem::InBattleItem() {
    m_PosInBattle = 0;
    
    m_hasDrop = false;
    m_dropItemType = "";
    m_dropItemId = "";
    m_countOfdropItem = 0;
    m_dropItemLevel = 0;
}

/* 获取和设置在战场上的位置 */
unsigned int InBattleItem::getPosInBattle() {return m_PosInBattle;}
void InBattleItem::setPosInBattle(unsigned int var) {m_PosInBattle = var;}
/* 获取和设置是否有掉落物品 */
bool InBattleItem::getHasDrop() {return m_hasDrop;}
void InBattleItem::setHasDrop(bool var) {m_hasDrop = var;}
/* 获取和设置掉落物品类别 */
std::string InBattleItem::getDropItemType() {return m_dropItemType;}
void InBattleItem::setDropItemType(std::string var) {m_dropItemType = var;}
/* 获取和设置掉落物品 ID */
std::string InBattleItem::getDropItemId() {return m_dropItemId;}
void InBattleItem::setDropItemId(std::string var) {m_dropItemId = var;}
/* 获取和设置掉落物品数量 */
unsigned int InBattleItem::getCountOfdropItem() {return m_countOfdropItem;}
void InBattleItem::setCountOfdropItem(unsigned int var) {m_countOfdropItem = var;}
/* 获取和设置掉落物品 level */
unsigned int InBattleItem::getDropItemLevel() {return m_dropItemLevel;}
void InBattleItem::setDropItemLevel(unsigned int var) {m_dropItemLevel = var;}

