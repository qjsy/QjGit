//
//  CityAwardItem.h
//  hero
//
//  Created by yangjie on 2014/0303.
//
//
//

#ifndef __hero__CityAwardItem__
#define __hero__CityAwardItem__

#include "cocos2d.h"

USING_NS_CC;

class CityAwardItem : public CCObject {
public:
    CityAwardItem();
    virtual ~CityAwardItem();
    
    CC_PROPERTY(unsigned int, m_lv, LV);
    CC_PROPERTY(unsigned int, m_award, Award);
    CC_PROPERTY(float, m_t, T);
};

#endif /* defined(__hero__CityAwardItem__) */
