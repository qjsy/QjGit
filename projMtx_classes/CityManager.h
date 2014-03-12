//
//  CityManager.h
//  hero
//
//  Created by yangjie on 2014/02/28.
//
//

#ifndef __hero__CityManager__
#define __hero__CityManager__

#include "cocos2d.h"

USING_NS_CC;

class CityItem;
class CityFunctionItem;
class CityAwardItem;
class CityManager {
private:    /*禁止外部 new*/
    CityManager();
    
public:
    virtual ~CityManager();
    
public:
    // 单例模式，获取本类唯一实例对象
    static CityManager* getInstance();
    
public:
    /* 添加城池数据 */
    bool appendCityItem(const unsigned int, CityItem*);
    /* 添加城池 Func 数据 */
    bool appendCityFunctionItem(const unsigned int, CityFunctionItem*);
    /* 添加城池 Award 数据 */
    bool appendCityAwardItem(const unsigned int, CityAwardItem*);
    
    /* 获取城池 */
    CityItem* getCityItem(const unsigned int);
    /* 获取城池 Func */
    CityFunctionItem* getCityFuncItem(const unsigned int);
    /* 获取城池 Award */
    CityAwardItem* getCityAward(const unsigned int);
    
private:
    CCDictionary* m_cityItems;
    CCDictionary* m_cityFuncItems;
    CCDictionary* m_cityAwardItems;
};

#endif /* defined(__hero__CityManager__) */
