//
//  DropItem.cpp
//  hero
//
//  Created by yangnie on 2014/02/16.
//
//

#include "DropItem.h"

/*
 *  构造函数
 */
DropItem::DropItem() {}

/*
 *  析构函数
 */
DropItem::~DropItem() {}

/*
 *  初始化
 */
bool DropItem::init() {
    m_id = 0;
    m_type = DT_NONE;
    m_heroLevel = 0;
    
    return true;
}

unsigned int DropItem::getId() {return m_id;}
void DropItem::setId(unsigned int var) {m_id = var;}
DROP_ITEM_TYPE DropItem::getType() {return m_type;}
void DropItem::setType(DROP_ITEM_TYPE var) {m_type = var;}
unsigned int DropItem::getHeroLevel() {return m_heroLevel;}
void DropItem::setHeroLevel(unsigned int var) {m_heroLevel = var;}
