//
//  TqHeadItem.cpp
//  hero
//
//  Created by yangjie on 2014/02/23.
//
//

#include "TqHeadItem.h"

/*
 *  初始化
 */
bool TqHeadItem::init() {
    m_position = 0;
    m_type = 0;
    m_id = 0;
    m_rank = 0;
    m_name = "";
    
    return true;
}

unsigned int TqHeadItem::getPosition() {return m_position;}
void TqHeadItem::setPosition(unsigned int var) {m_position = var;}
unsigned int TqHeadItem::getType() {return m_type;}
void TqHeadItem::setType(unsigned int var) {m_type = var;}
unsigned int TqHeadItem::getId() {return m_id;}
void TqHeadItem::setId(unsigned int var) {m_id = var;}
unsigned int TqHeadItem::getRank() {return m_rank;}
void TqHeadItem::setRank(unsigned int var) {m_rank = var;}
std::string TqHeadItem::getName() {return m_name;}
void TqHeadItem::setName(std::string var) {m_name = var;}
