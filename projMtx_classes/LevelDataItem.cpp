//
//  LevelDataItem.cpp
//  hero
//
//  Created by yangjie on 2013/12/23.
//
//

#include "LevelDataItem.h"

/*
 *  初始化
 */
bool LevelDataItem::init() {
    m_id = 0;
    m_name = "";
    m_mi = 0;
    m_ni = 0;
    m_en = 0;
    m_go = 0;
    m_ex = 0;
    
    return true;
}

/* 获取和设置关卡id */
unsigned int LevelDataItem::getId() {return m_id;}
void LevelDataItem::setId(unsigned int var) {m_id = var;}
/* 获取和设置关卡名称 */
std::string LevelDataItem::getName() {return m_name;}
void LevelDataItem::setName(std::string var) {m_name = var;}
/* 获取和设置 */
unsigned int LevelDataItem::getMi() {return m_mi;}
void LevelDataItem::setMi(unsigned int var) {m_mi = var;}
/* 获取和设置 */
unsigned int LevelDataItem::getNi() {return m_ni;}
void LevelDataItem::setNi(unsigned int var) {m_ni = var;}
/* 获取和设置 */
unsigned int LevelDataItem::getEn() {return m_en;}
void LevelDataItem::setEn(unsigned int var) {m_en = var;}
/* 获取和设置 */
unsigned int LevelDataItem::getGo() {return m_go;}
void LevelDataItem::setGo(unsigned int var) {m_go = var;}
/* 获取和设置 */
unsigned int LevelDataItem::getEx() {return m_ex;}
void LevelDataItem::setEx(unsigned int var) {m_ex = var;}
/* 获取和设置 .map */
std::string LevelDataItem::getDMapName() {return m_dMapName;}
void LevelDataItem::setDMapName(std::string var) {m_dMapName = var;}
/* 获取和设置帧名称 */
std::string LevelDataItem::getFrameName() {return m_frameName;}
void LevelDataItem::setFrameName(std::string var) {m_frameName = var;}
/* 获取和设置地图位置 */
CCPoint LevelDataItem::getMapPosition() {return m_mapPosition;}
void LevelDataItem::setMapPosition(CCPoint var) {m_mapPosition = var;}