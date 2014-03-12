//
//  MosterInBattleItem.h
//  hero
//
//  Created by yangjie on 2013/12/24.
//
//  战场上的怪物数据
//

#ifndef __hero__MosterInBattleItem__
#define __hero__MosterInBattleItem__

#include "InBattleItem.h"

USING_NS_CC;

class MosterItem;
class MosterInBattleItem : public InBattleItem {
public:
    MosterInBattleItem() {}
    virtual ~MosterInBattleItem() {}

public:
    virtual inline bool isMonster(){return true;};
    inline void setHP(unsigned int var) {m_hpInBattle = var;}
    virtual int getHP() {return m_hpInBattle;}
    
public:
    virtual bool init();
    CREATE_FUNC(MosterInBattleItem);

    
    /*
     * 属性定义
     */
    CC_PROPERTY(unsigned int, m_id, Id);                                    // id
    CC_PROPERTY(MosterItem*, m_mosterItem, MosterItem);                     // 关联的怪物数据

private:
    int m_hpInBattle;               // 生命值
};

#endif /* defined(__hero__MosterInBattleItem__) */
