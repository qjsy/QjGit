//
//  CopyDataItem.cpp
//  hero
//
//  Created by yangjie on 2013/12/23.
//
//

#include "CopyDataItem.h"
#include "LevelDataItem.h"

/*
 *  析构函数
 */
CopyDataItem::~CopyDataItem() {
    if (m_levels) {
        m_levels->removeAllObjects();
        m_levels->release();
        m_levels = NULL;
    }
}

/*
 *  初始化
 */
bool CopyDataItem::init() {
    m_levels = CCArray::create();
    m_levels->retain();
    
    m_id = 0;
    m_name = "";
    m_ty = 0;
    m_nc = 0;
    m_sl = 0;
    m_el = 0;
    
    return true;
}

/* 
 *  功能：
 *      添加关卡数据
 *  参数：
 *      pLevelDataItem      关卡数据项
 */
void CopyDataItem::appendLevelData(const LevelDataItem* pLevelDataItem) {
    CCAssert(m_levels && pLevelDataItem, "invalid data");
    
    m_levels->addObject((CCObject*)pLevelDataItem);
}

/*
 *  功能：
 *      根据索引号获取关卡数据
 *  参数：
 *      index   索引号
 *  返回：
 *      成功返回关卡数据，失败返回 NULL
 */
LevelDataItem* CopyDataItem::getLevelDataByIndex(const unsigned int index) {
    CCAssert(m_levels, "invalid data");
    
    if (index >= m_levels->count())
        return NULL;
    return (LevelDataItem*)m_levels->objectAtIndex(index);
}

/*
 *  功能：
 *      根据 id 号获取关卡数据
 *  参数：
 *      id      关卡 id 号
 *  返回：
 *      成功返回关卡数据，失败返回 NULL
 */
LevelDataItem* CopyDataItem::getLevelDataById(const unsigned int id) {
    CCAssert(m_levels, "invalid data");
    
    LevelDataItem* pLevelDataItem = NULL;
    CCObject* item = NULL;
    CCARRAY_FOREACH(m_levels, item) {
        if (((LevelDataItem*)item)->getId() == id) {
            pLevelDataItem = (LevelDataItem*)item;
        }
    } /*for*/
    
    return pLevelDataItem;
}

/* 获取和设置副本 id */
unsigned int CopyDataItem::getId() {return m_id;}
void CopyDataItem::setId(unsigned int var) {m_id = var;}
/* 获取和设置副本名称 */
std::string CopyDataItem::getName() {return m_name;}
void CopyDataItem::setName(std::string var) {m_name = var;}
/* 获取和设置 */
unsigned int CopyDataItem::getTy() {return m_ty;}
void CopyDataItem::setTy(unsigned int var) {m_ty = var;}
/* 获取和设置 */
unsigned int CopyDataItem::getNc() {return m_nc;}
void CopyDataItem::setNc(unsigned int var) {m_nc = var;}
/* 获取和设置 */
unsigned int CopyDataItem::getSl() {return m_sl;}
void CopyDataItem::setSl(unsigned int var) {m_sl = var;}
/* 获取和设置 */
unsigned int CopyDataItem::getEl() {return m_el;}
void CopyDataItem::setEl(unsigned int var) {m_el = var;}
/* 获取和设置描述 */
std::string CopyDataItem::getDs() {return m_ds;}
void CopyDataItem::setDs(std::string var) {m_ds = var;}
/* 获取和设置帧名称 */
std::string CopyDataItem::getFrameName() {return m_frameName;}
void CopyDataItem::setFrameName(std::string var) {m_frameName = var;}
/* 获取和设置帧名称 */
std::string CopyDataItem::getDMapName() {return m_dMapName;}
void CopyDataItem::setDMapName(std::string var) {m_dMapName = var;}
/* 获取和设置帧名称 */
CCPoint CopyDataItem::getMapPosition() {return m_mapPosition;}
void CopyDataItem::setMapPosition(CCPoint var) {m_mapPosition = var;}

