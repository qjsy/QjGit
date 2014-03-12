//
//  CityFunctionItem.h
//  hero
//
//  Created by yangjie on 2014/02/28.
//
//  城池功能项
//

#ifndef __hero__CityFunctionItem__
#define __hero__CityFunctionItem__

#include "cocos2d.h"

USING_NS_CC;

class CityFunctionItem : public CCObject {
public:
    CityFunctionItem();
    virtual ~CityFunctionItem() {}
    
    /*
     *  属性定义
     */
    CC_PROPERTY(unsigned int, m_id, Id);                        // 城池func id
    CC_PROPERTY(std::string , m_name, Name);                    // 城池func 名称
    CC_PROPERTY(float, m_p, P);                                 // 城池func p
    CC_PROPERTY(std::string, m_des, Des);                       // 城池func des
    CC_PROPERTY(std::string, m_i, I);                           // 城池func 功能图标
};



#endif /* defined(__hero__CityFunctionItem__) */
