//
//  MosterItem.h
//  hero
//
//  Created by yangjie on 2013/12/24.
//
//

#ifndef __hero__MosterItem__
#define __hero__MosterItem__

#include "cocos2d.h"

USING_NS_CC;

class MosterItem : public CCObject {
public:
    MosterItem() {}
    virtual ~MosterItem() {}

public:
    virtual bool init();
    
    /*
     *  属性定义
     */
    CC_PROPERTY(unsigned int, m_id, Id);                            // id
    CC_PROPERTY(std::string , m_name, Name);                        // 名称
    CC_PROPERTY(unsigned int, m_rarity, Rarity);                    // 稀有度
    CC_PROPERTY(unsigned int, m_job, Job);                          // 职业
    CC_PROPERTY(std::string , m_fi, Fi);                            // 图片
    CC_PROPERTY(std::string , m_li, Li);                            // 图片
};

#endif /* defined(__hero__MosterItem__) */


