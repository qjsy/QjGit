//
//  CityManager.cpp
//  hero
//
//  Created by yangjie on 2014/02/28.
//
//

#include "CityManager.h"
#include "CityItem.h"
#include "CityFunctionItem.h"
#include "CityAwardItem.h"

/* 静态实例变量 */
static CityManager* s_CityManager = NULL;

// 单例模式，获取本类唯一实例对象
CityManager* CityManager::getInstance() {
    if (NULL == s_CityManager) {
        s_CityManager = new CityManager();
    }
    return s_CityManager;
}

/*
 *  构造函数
 */
CityManager::CityManager() {
    m_cityItems = CCDictionary::create();
    m_cityItems->retain();
    
    m_cityFuncItems = CCDictionary::create();
    m_cityFuncItems->retain();
    
    m_cityAwardItems = CCDictionary::create();
    m_cityAwardItems->retain();
}

/*
 *  析构函数
 */
CityManager::~CityManager() {
    CC_SAFE_RELEASE_NULL(m_cityItems);
    CC_SAFE_RELEASE_NULL(m_cityFuncItems);
    CC_SAFE_RELEASE_NULL(m_cityAwardItems);
}

/*
 *  功能：
 *      添加城池数据
 *  参数：
 *      id              城池 id
 *      pCityItem       城池数据项
 *  返回：
 *      添加成功返回 true，失败返回 false
 */
bool CityManager::appendCityItem(const unsigned int id, CityItem* pCityItem) {
    CCAssert(pCityItem, "invalid CityItem");
    if (m_cityItems->objectForKey(id)) {
        return false;
    }
    m_cityItems->setObject(pCityItem, id);
    return true;
}

/* 
 *  功能：
 *      添加城池Func数据
 *  参数：
 *      id
 *      pCityFunctionItem
 *  返回：
 *      添加成功返回 true，失败返回 false
 */
bool CityManager::appendCityFunctionItem(const unsigned int id, CityFunctionItem* pCityFunctionItem) {
    CCAssert(pCityFunctionItem, "invalid CityItem");
    if (m_cityFuncItems->objectForKey(id)) {
        return false;
    }
    m_cityFuncItems->setObject(pCityFunctionItem, id);
    return true;
}

/*
 *  功能：
 *      添加城池 Award 数据
 *  参数：
 *      lv
 *      pCityAwardItem
 *  返回：
 *      添加成功返回 true，失败返回 false
 */
bool CityManager::appendCityAwardItem(const unsigned int lv, CityAwardItem* pCityAwardItem) {
    CCAssert(pCityAwardItem, "invalid CityItem");
    if (m_cityAwardItems->objectForKey(lv)) {
        return false;
    }
    m_cityAwardItems->setObject(pCityAwardItem, lv);
    return true;
}

/*
 *  功能：
 *      获取城池数据
 *  参数：
 *      id      城池id
 *  返回：
 *      成功返回城池数据，失败返回 NULL
 */
CityItem* CityManager::getCityItem(const unsigned int id) {
    return (CityItem*)m_cityItems->objectForKey(id);
}

/* 
 *  功能：
 *      获取城池Func 数据
 *  参数：
 *      id      城池Func id
 *  返回：
 *      成功返回城池Func数据，失败返回 NULL
 */
CityFunctionItem* CityManager::getCityFuncItem(const unsigned int id) {
    return (CityFunctionItem*)m_cityFuncItems->objectForKey(id);
}

/*
 *  功能：
 *      获取城池 Award 数据
 *  参数：
 *      lv      城池Func id
 *  返回：
 *      成功返回城池Award数据，失败返回 NULL
 */
CityAwardItem* CityManager::getCityAward(const unsigned int lv) {
    return (CityAwardItem*)m_cityAwardItems->objectForKey(lv);
}

