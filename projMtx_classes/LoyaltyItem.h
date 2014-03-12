//
//  LoyaltyItem.h
//  hero
//
//  Created by yangjie on 2014/02/23.
//
//  忠诚度
//

#ifndef __hero__LoyaltyItem__
#define __hero__LoyaltyItem__

#include "cocos2d.h"

USING_NS_CC;

class LoyaltyItem : public CCObject {
public:
    LoyaltyItem() {}
    virtual ~LoyaltyItem() {}
    
public:
    virtual bool init();
    
public:
    /*
     *  属性定义
     */
    CC_PROPERTY(unsigned int, m_id, Id);        // 忠诚度 id
    CC_PROPERTY(unsigned int, m_pu, Pu);        //
    CC_PROPERTY(unsigned int, m_bl, Bl);        //
};


#endif /* defined(__hero__LoyaltyItem__) */
