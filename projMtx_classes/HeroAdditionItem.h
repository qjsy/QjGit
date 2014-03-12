//
//  HeroAdditionItem.h
//  hero
//
//  Created by yangjie on 2013/12/18.
//
//  武将附加数据项
//
//

#ifndef __hero__HeroAdditionItem__
#define __hero__HeroAdditionItem__

#include "cocos2d.h"

USING_NS_CC;

class HeroAdditionItem : public CCObject {
public:
    HeroAdditionItem() {}
    virtual ~HeroAdditionItem() {}
    
public:
    virtual bool init();
    
    CC_PROPERTY(unsigned int, m_expForLevUp, ExpForLevUp);                    // 武将升级所需经验
    CC_PROPERTY(unsigned int, m_expByFood, ExpByFood);                        // 被吞噬产生的经验
    CC_PROPERTY(unsigned int, m_goldByFood, GoldByFood);                      // 吞噬每张卡所需的金币
    CC_PROPERTY(unsigned int, m_goldBySale, GoldBySale);                      // 出售获得金币
};

#endif /* defined(__hero__HeroAdditionItem__) */
