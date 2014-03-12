//
//  CopyDataItem.h
//  hero
//
//  Created by yangjie on 2013/12/23.
//
//  副本数据项
//

#ifndef __hero__CopyDataItem__
#define __hero__CopyDataItem__

#include "cocos2d.h"

USING_NS_CC;

class LevelDataItem;
class CopyDataItem : public CCObject {
public:
    CopyDataItem() {}
    virtual ~CopyDataItem();

public:
    virtual bool init();

public:
    /* 获取关卡数量 */
    inline unsigned int getCountOfLevel() {return m_levels->count();}
    
    /* 添加关卡数据 */
    void appendLevelData(const LevelDataItem*);
    /* 根据索引号获取关卡 */
    LevelDataItem* getLevelDataByIndex(const unsigned int);
    /* 根据id号获取关卡 */
    LevelDataItem* getLevelDataById(const unsigned int);
    
    CC_PROPERTY(unsigned int, m_id, Id);                            // 副本 id
    CC_PROPERTY(std::string, m_name, Name);                         // 副本名称
    CC_PROPERTY(unsigned int, m_ty,  Ty);                           //
    CC_PROPERTY(unsigned int, m_nc,  Nc);                           //
    CC_PROPERTY(unsigned int, m_sl,  Sl);                           //
    CC_PROPERTY(unsigned int, m_el,  El);                           //
    CC_PROPERTY(std::string, m_ds,  Ds);                            // 副本描述
    CC_PROPERTY(std::string, m_frameName, FrameName);               // 帧名称
    CC_PROPERTY(CCPoint, m_mapPosition, MapPosition);               // 地图位置
    CC_PROPERTY(std::string, m_dMapName, DMapName);                 // 点map名称

private:
    CCArray* m_levels;                  // 存放该副本所有关卡
};

#endif /* defined(__hero__CopyDataItem__) */

