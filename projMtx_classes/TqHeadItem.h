//
//  TqHeadItem.h
//  hero
//
//  Created by yangjie on 2014/02/23.
//
//  铜雀台 pk 对象头像
//

#ifndef __hero__TqHeadItem__
#define __hero__TqHeadItem__

#include "cocos2d.h"

USING_NS_CC;

class TqHeadItem : public CCObject {
public:
    TqHeadItem() {}
    virtual ~TqHeadItem() {}
    
public:
    virtual bool init();
    CREATE_FUNC(TqHeadItem);
    
public:
    /*
     *  属性定义
     */
    CC_PROPERTY(unsigned int, m_position, Position);        // 位置
    CC_PROPERTY(unsigned int, m_type, Type);                // 类型
    CC_PROPERTY(unsigned int, m_id, Id);                    // id
    CC_PROPERTY(unsigned int, m_rank, Rank);                // 等级
    CC_PROPERTY(std::string, m_name, Name);                 // 名称
};

#endif /* defined(__hero__TqHeadItem__) */
