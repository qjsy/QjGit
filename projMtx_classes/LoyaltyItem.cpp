//
//  LoyaltyItem.cpp
//  hero
//
//  Created by yangjie on 2014/02/23.
//
//

#include "LoyaltyItem.h"

/*
 *  初始化
 */
bool LoyaltyItem::init() {
    m_id = 0;
    m_pu = 0;
    m_bl = 0;
    
    return true;
}

unsigned int LoyaltyItem::getId() {return m_id;}
void LoyaltyItem::setId(unsigned int var) {m_id = var;}
unsigned int LoyaltyItem::getPu() {return m_pu;}
void LoyaltyItem::setPu(unsigned int var) {m_pu = var;}
unsigned int LoyaltyItem::getBl() {return m_bl;}
void LoyaltyItem::setBl(unsigned int var) {m_bl = var;}

