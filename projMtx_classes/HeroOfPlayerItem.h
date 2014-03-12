//
//  HeroOfPlayerItem.h
//  hero
//
//  Created by yangjie on 2013/12/8.
//
//  玩家所属武将数据项
//

#ifndef __hero__HeroOfPlayerItem__
#define __hero__HeroOfPlayerItem__

#include "InBattleItem.h"

USING_NS_CC;

class HeroItem;
class HeroOfPlayerItem : public InBattleItem {
public:
    HeroOfPlayerItem() {}
    virtual ~HeroOfPlayerItem() {}
    
public:
    virtual bool init();
    
    CREATE_FUNC(HeroOfPlayerItem);

public:
    virtual inline bool isMonster() {return false;};
    
public:
    /* 比较大小，用于排序 (武将列表排序)*/
    static bool less(const CCObject*, const CCObject*);
    /* 出售列表排序 */
    static bool lessSellAndTun(const CCObject*, const CCObject*);
    /* 比较大小，用于排序 (pvp 武将列表排序) */
    static bool lessForPvp(const CCObject*, const CCObject*);

public:
    /* 获取攻击力 */
    unsigned int getAttack();
    /* 获取血量 */
    virtual int getHP();
    /* 是否是队长(pve) */
    bool isTeamLeader();
    /* 是否是队长(pvp) */
    bool isPvpTeamLeader();
    
    inline int getPvpHp() {return m_PvpHp;}
    inline void setPvpHp(const int pvpHp) {m_PvpHp = pvpHp;}
    
    ///////////////////////////////
    //  以下是玩家所属武将属性定义   //
    //////////////////////////////
    CC_PROPERTY(std::string, m_uniId, UniId);                               // uniId 唯一标识一个玩家所属武将
    CC_PROPERTY(unsigned int, m_level, Level);                              // 等级
    CC_PROPERTY(unsigned int, m_exp, Exp);                                  // 经验
    CC_PROPERTY(bool, m_isGoIntoBattle, GoIntoBattle);                      // 是否是上阵武将(pve)
    CC_PROPERTY(bool, m_isPvpGoIntoBattle, PvpGoIntoBattle);                // 是否是上阵武将(pvp)
    CC_PROPERTY(unsigned int, m_PosInBattle, PosInBattle);                  // 在战场上的位置
    CC_PROPERTY(unsigned int, m_weapon, Weapon);                            // 武器id
    CC_PROPERTY(unsigned int, m_armor, Armor);                              // 防具id
    CC_PROPERTY(bool, m_isFriendHero, FriendHero);                          // 是否是好友的武将
    CC_PROPERTY(bool, m_isLock, Lock);                                      // 是否上锁
    CC_PROPERTY(HeroItem*, m_pHeroItem, HeroItem);                          // 关联的武将数据
    
    /* 掉落物品相关 */
    CC_PROPERTY(bool, m_hasDrop, HasDrop);                                  // 是否有掉落物品
    CC_PROPERTY(std::string, m_dropItemType, DropItemType);                 // 掉落物品类别
    CC_PROPERTY(std::string, m_dropItemId, DropItemId);                     // 掉落物品 ID
    CC_PROPERTY(unsigned int, m_countOfdropItem, CountOfdropItem);          // 掉落物品数量
    CC_PROPERTY(unsigned int, m_dropItemLevel, DropItemLevel);              // 掉落物品 level

protected:
    bool m_isTeamLeader;
    int m_PvpHp;                                                            // pvp 模式下生命值
};

#endif /* defined(__hero__HeroOfPlayerItem__) */

