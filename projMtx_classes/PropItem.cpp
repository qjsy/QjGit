//
//  PropItem.cpp
//  hero
//
//  Created by 杨杰 on 2014/01/12.
//
//

#include "PropItem.h"

/*
 *  构造函数
 */
PropItem::PropItem() {}

/*
 *  析构函数
 */
PropItem::~PropItem() {}

/*
 *  初始化
 */
bool PropItem::init() {
    m_id = 0;
    m_name = "";
    m_desc= "";
    m_type = 0;
    m_icon = "";
    
    return true;
}

unsigned int PropItem::getId() {return m_id;}
void PropItem::setId(unsigned int var) {m_id = var;}
std::string PropItem::getName() {return m_name;}
void PropItem::setName(std::string var) {m_name = var;}
std::string PropItem::getDesc() {return m_desc;}
void PropItem::setDesc(std::string var) {m_desc = var;}
unsigned int PropItem::getType() {return m_type;}
void PropItem::setType(unsigned int var) {m_type = var;}
std::string PropItem::getIcon() {return m_icon;}
void PropItem::setIcon(std::string var) {m_icon = var;}

