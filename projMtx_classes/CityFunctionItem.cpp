//
//  CityFunctionItem.cpp
//  hero
//
//  Created by yangjie on 2014/02/28.
//

#include "CityFunctionItem.h"

/*
 *  构造函数
 */
CityFunctionItem::CityFunctionItem() {
    m_id = 0;
    m_name = "";
    m_p = 0.0f;
}

unsigned int CityFunctionItem::getId() {return m_id;}
void CityFunctionItem::setId(unsigned int var) {m_id = var;}
std::string CityFunctionItem::getName() {return m_name;}
void CityFunctionItem::setName(std::string var) {m_name = var;}
float CityFunctionItem::getP() {return m_p;}
void CityFunctionItem::setP(float var) {m_p = var;}
std::string CityFunctionItem::getDes() {return m_des;}
void CityFunctionItem::setDes(std::string var) {m_des = var;}
std::string CityFunctionItem::getI() {return m_i;}
void CityFunctionItem::setI(std::string var) {m_i = var;}
