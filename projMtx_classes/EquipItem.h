//
//  EquipItem.h
//  hero
//
//  Created by yangjie on 2014/01/12.
//
//  游戏装备
//

#ifndef __hero__EquipItem__
#define __hero__EquipItem__

#include "cocos2d.h"

USING_NS_CC;

class EquipItem : public CCObject {
public:
    EquipItem();
    virtual ~EquipItem();
    
    virtual bool init();
    CREATE_FUNC(EquipItem);
    
    /*
     *  属性定义
     */
    CC_PROPERTY(unsigned int, m_id, Id);
    CC_PROPERTY(std::string, m_name, Name);
};

#endif /* defined(__hero__EquipItem__) */
