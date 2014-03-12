//
//  CityItem.cpp
//  hero
//
//  Created by yangjie on 2014/02/28.
//
//

#include "CityItem.h"
#include "CityFunctionItem.h"

/*
 *  构造函数
 */
CityItem::CityItem() {
    m_id = 0;
    m_name = "";
    m_cityFuncs.clear();
}

/*
 *  析构函数
 */
CityItem::~CityItem() {
    m_cityFuncs.clear();
}

unsigned int CityItem::getId() {return m_id;}
void CityItem::setId(unsigned int var) {m_id = var;}

std::string CityItem::getName() {return m_name;}
void CityItem::setName(std::string var) {m_name = var;}
unsigned int CityItem::getGuardNum() {return m_guardNum;}
void CityItem::setGuardNum(unsigned int var) {m_guardNum = var;}
unsigned int CityItem::getHolder() {return m_holder;}
void CityItem::setHolder(unsigned int var) {m_holder = var;}

/*
 *  功能：
 *      添加城池 func
 *
 */
void CityItem::appendCityFunc(CityFunctionItem* pCityFunctionItem) {
    CCAssert(pCityFunctionItem, "invalid CityFunctionItem");
    m_cityFuncs.push_back(pCityFunctionItem);
}

/* 
 *  功能：
 *      根据 index 获取城池func
 *  参数：
 *      index       索引号
 *  返回：
 *      成功返回城池func，失败返回 NULL
 */
CityFunctionItem* CityItem::getCityFuncItemByIndex(const unsigned int index) {
    if (index >= m_cityFuncs.size())
        return NULL;
    return m_cityFuncs[index];
}


