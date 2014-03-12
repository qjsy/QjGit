//
//  HeroFhpItem.cpp
//  hero
//
//  Created by yangjie on 2013/12/20.
//

#include "HeroFhpItem.h"

/*
 *  初始化
 */
bool HeroFhpItem::init() {
    m_rarity = 0;
    m_b = 0;
    m_q = 0;
    m_g = 0;
    m_m = 0;
    m_c = 0;
    m_d = 0;
    m_y = 0;
    
    return true;
}


/* 获取和设置稀有度 */
unsigned int HeroFhpItem::getRarity() {return m_rarity;}
void HeroFhpItem::setRarity(unsigned int var) {m_rarity = var;}
/*  */
unsigned int HeroFhpItem::getB() {return m_b;}
void HeroFhpItem::setB(unsigned int var) {m_b = var;}
/*  */
unsigned int HeroFhpItem::getQ() {return m_q;}
void HeroFhpItem::setQ(unsigned int var) {m_q = var;}
/*  */
unsigned int HeroFhpItem::getG() {return m_g;}
void HeroFhpItem::setG(unsigned int var) {m_g = var;}
/*  */
unsigned int HeroFhpItem::getM() {return m_m;}
void HeroFhpItem::setM(unsigned int var) {m_m = var;}
/*  */
unsigned int HeroFhpItem::getC() {return m_c;}
void HeroFhpItem::setC(unsigned int var) {m_c = var;}
/*  */
unsigned int HeroFhpItem::getD() {return m_d;}
void HeroFhpItem::setD(unsigned int var) {m_d = var;}
/*  */
unsigned int HeroFhpItem::getY() {return m_y;}
void HeroFhpItem::setY(unsigned int var) {m_y = var;}

