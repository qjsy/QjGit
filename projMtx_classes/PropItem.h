//
//  PropItem.h
//  hero
//
//  Created by yangjie on 2014/01/12.
//
//  道具
//

#ifndef __hero__PropItem__
#define __hero__PropItem__

#include "cocos2d.h"

USING_NS_CC;

class PropItem : public CCObject {
public:
    PropItem();
    virtual ~PropItem();
    
    virtual bool init();
    CREATE_FUNC(PropItem);

    /*
     *  属性定义
     */
    CC_PROPERTY(unsigned int, m_id, Id);
    CC_PROPERTY(std::string, m_name, Name);
    CC_PROPERTY(std::string, m_desc, Desc);
    CC_PROPERTY(unsigned int, m_type, Type);
    CC_PROPERTY(std::string, m_icon, Icon);
};


#endif /* defined(__hero__PropItem__) */
