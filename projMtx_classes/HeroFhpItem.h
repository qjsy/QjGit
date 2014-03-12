//
//  HeroFhpItem.h
//  hero
//
//  Created by yangjie on 2013/12/20.
//
//  武将稀有度所对应的要拥有每种职业的一个兵所需要的血量
//
//  武将职业对应兵种
//  1:步兵
//  2:骑兵
//  3:弓兵
//  4:医师
//  5:谋士
//  6:策士
//  7:都尉
//

#ifndef __hero__HeroFhpItem__
#define __hero__HeroFhpItem__

#include "cocos2d.h"

USING_NS_CC;

class HeroFhpItem : public CCObject {
public:
    HeroFhpItem() {}
    virtual ~HeroFhpItem() {}
    
public:
    virtual bool init();

    CC_PROPERTY(unsigned int, m_rarity, Rarity);            //  稀有度
    CC_PROPERTY(unsigned int, m_b, B);                      //  拥有一个步兵所需要的血量
    CC_PROPERTY(unsigned int, m_q, Q);                      //  拥有一个骑兵所需要的血量
    CC_PROPERTY(unsigned int, m_g, G);                      //  拥有一个弓兵所需要的血量
    CC_PROPERTY(unsigned int, m_m, M);                      //  拥有一个谋士所需要的血量
    CC_PROPERTY(unsigned int, m_c, C);                      //  拥有一个策士所需要的血量
    CC_PROPERTY(unsigned int, m_d, D);                      //  拥有一个都尉所需要的血量
    CC_PROPERTY(unsigned int, m_y, Y);                      //  拥有一个医师所需要的血量
};

#endif /* defined(__hero__HeroFhpItem__) */
