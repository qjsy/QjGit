//
//  LevelDataItem.h
//  hero
//
//  Created by yangjie on 2013/12/23.
//
//  关卡数据项
//

#ifndef __hero__LevelDataItem__
#define __hero__LevelDataItem__

#include "cocos2d.h"

USING_NS_CC;

class LevelDataItem : public CCObject {
public:
    LevelDataItem() {}
    virtual ~LevelDataItem() {}
    
public:
    virtual bool init();

    /*
     *  属性
     */
    CC_PROPERTY(unsigned int, m_id, Id);            // 关卡 id
    CC_PROPERTY(std::string, m_name, Name);         // 名称
    CC_PROPERTY(unsigned int, m_mi, Mi);            //
    CC_PROPERTY(unsigned int, m_ni, Ni);            // 下一个关卡
    CC_PROPERTY(unsigned int, m_en, En);            //
    CC_PROPERTY(unsigned int, m_go, Go);            //
    CC_PROPERTY(unsigned int, m_ex, Ex);            //
    CC_PROPERTY(std::string, m_dMapName, DMapName);                 // 点map名称
    CC_PROPERTY(std::string, m_frameName, FrameName);               // 帧名称
    CC_PROPERTY(CCPoint, m_mapPosition, MapPosition);               // 地图位置
//    "n1":"|210101|||210101|210101",
//    "n2":"|210101||210101|210101|",
//    "n3":"210101||210101||210102|",
//    "dp":"2000,1,0.08"
    
};

#endif /* defined(__hero__LevelDataItem__) */
