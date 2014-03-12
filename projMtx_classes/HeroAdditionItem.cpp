//
//  HeroAdditionItem.cpp
//  hero
//
//  Created by yangjie on 2013/12/18.
//
//

#include "HeroAdditionItem.h"

/*
 *  初始化
 */
bool HeroAdditionItem::init() {
    m_expForLevUp = 0;
    m_expByFood = 0;
    m_goldByFood = 0;
    m_goldBySale = 0;
    
    return true;
}

/* 获取和设置武将升级所需经验 */
unsigned int HeroAdditionItem::getExpForLevUp() {return m_expForLevUp;}
void HeroAdditionItem::setExpForLevUp(unsigned int var) {m_expForLevUp = var;}
/* 获取和设置被吞噬产生的经验 */
unsigned int HeroAdditionItem::getExpByFood() {return m_expByFood;}
void HeroAdditionItem::setExpByFood(unsigned int var) {m_expByFood = var;}
/* 获取和设置吞噬每张卡所需的金币 */
unsigned int HeroAdditionItem::getGoldByFood() {return m_goldByFood;}
void HeroAdditionItem::setGoldByFood(unsigned int var) {m_goldByFood = var;}
/* 获取和设置出售获得金币 */
unsigned int HeroAdditionItem::getGoldBySale() {return m_goldBySale;}
void HeroAdditionItem::setGoldBySale(unsigned int var) {m_goldBySale = var;}
