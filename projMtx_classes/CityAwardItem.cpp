//
//  CityAwardItem.cpp
//  hero
//
//  Created by yangjie on 2014/03/03.
//
//

#include "CityAwardItem.h"

/*
 *  构造函数
 */
CityAwardItem::CityAwardItem() {
    m_lv = 0;
    m_award = 0;
    m_t = 0.0f;
}

/*
 *  析构函数
 */
CityAwardItem::~CityAwardItem() {
    
}

unsigned int CityAwardItem::getLV() {return m_lv;}
void CityAwardItem::setLV(unsigned int var) {m_lv = var;}
unsigned int CityAwardItem::getAward() {return m_award;}
void CityAwardItem::setAward(unsigned int var) {m_award = var;}
float CityAwardItem::getT() {return m_t;}
void CityAwardItem::setT(float var) {m_t = var;}

