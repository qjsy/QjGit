//
//  PrisonHeroItem.h
//  hero
//
//  Created by yangjie on 2014-2-20.
//
//  监狱武将
//

#ifndef __hero__PrisonHeroItem__
#define __hero__PrisonHeroItem__

#include "cocos2d.h"
#include "common.h"

USING_NS_CC;

/*
 *  监狱武将数据项
 */
class HeroItem;
class PrisonHeroItem : public CCObject {
public:
    PrisonHeroItem() {}
    virtual ~PrisonHeroItem() {}
    
public:
    virtual bool init();
    CREATE_FUNC(PrisonHeroItem);
    
public:
    /* 获取关联武将 */
    inline HeroItem* getHeroItem() const {return m_pHeroItem;}
    
    /* 监狱武将排序 */
    static bool lessForPrison(const CCObject*, const CCObject*);
    
    ///////////////////////////
    //  以下是监狱武将属性定义   //
    ///////////////////////////
    CC_PROPERTY(unsigned int, m_heroId, HeroId);                    // 武将id
    CC_PROPERTY(unsigned int, m_money, Money);                      // 金钱
    CC_PROPERTY(unsigned int, m_devotion, Devotion);                // 忠诚度
    CC_PROPERTY(unsigned int, m_num, Num);                          // 所需道具数量
//    CC_PROPERTY(bool, m_isPurpleProp, IsPurpleProp);              // 是否是紫色道具
    CC_PROPERTY(PRISON_PROP, m_typeProp, TypeProp);                 // 道具类型
    CC_PROPERTY(unsigned int, m_gold, Gold);                         // 需要元宝
private:
    HeroItem* m_pHeroItem;          // 关联的武将数据
};

#endif /* defined(__hero__PrisonHeroItem__) */

