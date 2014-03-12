//
//  EquipItem.cpp
//  hero
//
//  Created by 杨杰 on 14-1-12.
//
//

#include "EquipItem.h"

/*
 *  构造函数
 */
EquipItem::EquipItem() {}

/*
 *  析构函数
 */
EquipItem::~EquipItem() {}

/*
 *  初始化
 */
bool EquipItem::init() {
    m_id = 0;
    m_name = "";
    
    return true;
}

unsigned int EquipItem::getId() {return m_id;}
void EquipItem::setId(unsigned int var) {m_id = var;}
std::string EquipItem::getName() {return m_name;}
void EquipItem::setName(std::string var) {m_name = var;}