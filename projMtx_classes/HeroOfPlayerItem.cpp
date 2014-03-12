//
//  HeroOfPlayerItem.cpp
//  hero
//
//  Created by yangjie on 2013/12/8.
//
//

#include "HeroItem.h"
#include "HeroOfPlayerItem.h"
#include "HeroManager.h"
#include "PrisonHeroItem.h"

/*
 * 初始化
 */
bool HeroOfPlayerItem::init() {
    m_uniId = "";
    m_isGoIntoBattle = false;
    m_isPvpGoIntoBattle = false;
    m_PosInBattle = 0;
    m_isTeamLeader = false;
    m_level = 0;
    m_exp = 0;
    m_weapon = 0;
    m_armor = 0;
    m_isFriendHero = false;
    m_isLock = false;
    m_pHeroItem = NULL;
    
    m_hasDrop = false;
    m_dropItemType = "";
    m_dropItemId = "";
    m_countOfdropItem = 0;
    m_dropItemLevel = 0;
    m_PvpHp = 0;
    
    return true;
}

/*
 * 比较两个武将的大小
 *
 * 规则如下
 *      队长排在最前面
 *      上阵武将排在最前面
 *      稀有度高的排在前面
 *      等级高的排在前面
 *
 * 参数
 *      pCCObj1     武将1
 *      pCCObj2     武将2
 *
 * 返回
 *      true 表示第前面的武将优先于后面的武将，false 则相反
 *
 */
bool HeroOfPlayerItem::less(const CCObject* pCCObj1, const CCObject* pCCObj2) {
    CCAssert(pCCObj1 && pCCObj2, "invalid data");
    
    HeroOfPlayerItem* pItem1 = (HeroOfPlayerItem*)pCCObj1;
    HeroOfPlayerItem* pItem2 = (HeroOfPlayerItem*)pCCObj2;
    
    if (pItem1->isTeamLeader()) /*武将1是队长*/ {
        return true;
    }
    if (pItem2->isTeamLeader()) /*武将2是队长*/ {
        return false;
    }
    if (pItem1->getGoIntoBattle()) /*武将1是上阵武将*/{
        if (!pItem2->getGoIntoBattle()) /*武将2不是上阵武将*/ {
            return true;
        }
    }
    if (pItem2->getGoIntoBattle()) /*武将2是上阵武将*/{
        if (!pItem1->getGoIntoBattle()) /*武将1不是上阵武将*/ {
            return false;
        }
    }
    
    if (pItem1->getHeroItem()->getRarity() > pItem2->getHeroItem()->getRarity()) /*武将1稀有度大于武将2*/{
        return true;
    } else if (pItem1->getHeroItem()->getRarity() == pItem2->getHeroItem()->getRarity()) /*稀有度相等*/ {
        // 比较等级
        if (pItem1->getLevel() > pItem2->getLevel() ) {
            return true;
        } else if (pItem1->getLevel() == pItem2->getLevel()){
            // 比较ID
            if (pItem1->getHeroItem()->getId() < pItem2->getHeroItem()->getId()) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    } else /*武将1稀有度小于武将2*/{
        return false;
    }
}

bool HeroOfPlayerItem::lessForPvp(const CCObject* pCCObj1, const CCObject* pCCObj2) {
    CCAssert(pCCObj1 && pCCObj2, "invalid data");
    
    HeroOfPlayerItem* pItem1 = (HeroOfPlayerItem*)pCCObj1;
    HeroOfPlayerItem* pItem2 = (HeroOfPlayerItem*)pCCObj2;
    
    if (pItem1->isPvpTeamLeader()) /*武将1是队长*/ {
        return true;
    }
    if (pItem2->isPvpTeamLeader()) /*武将2是队长*/ {
        return false;
    }
    if (pItem1->getPvpGoIntoBattle()) /*武将1是上阵武将*/{
        if (!pItem2->getPvpGoIntoBattle()) /*武将2不是上阵武将*/ {
            return true;
        }
    }
    if (pItem2->getPvpGoIntoBattle()) /*武将2是上阵武将*/{
        if (!pItem1->getPvpGoIntoBattle()) /*武将1不是上阵武将*/ {
            return false;
        }
    }
    
    if (pItem1->getHeroItem()->getRarity() > pItem2->getHeroItem()->getRarity()) /*武将1稀有度大于武将2*/{
        return true;
    } else if (pItem1->getHeroItem()->getRarity() == pItem2->getHeroItem()->getRarity()) /*稀有度相等*/ {
        // 比较等级
        if (pItem1->getLevel() > pItem2->getLevel() ) {
            return true;
        } else if (pItem1->getLevel() == pItem2->getLevel()){
            // 比较ID
            if (pItem1->getHeroItem()->getId() < pItem2->getHeroItem()->getId()) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    } else /*武将1稀有度小于武将2*/{
        return false;
    }
}

/*
 * 出售武将排序
 *
 * 规则
 *      稀有度低的排在前面
 *      等级高的排在前面
 *
 * 参数
 *      pCCObj1     武将1
 *      pCCObj2     武将2
 *
 * 返回
 *      true 表示第前面的武将小于后面的武将，false 则相反
 *
 */

bool HeroOfPlayerItem::lessSellAndTun(const CCObject* pCCObj1, const CCObject* pCCObj2) {
    CCAssert(pCCObj1 && pCCObj2, "invalid data");

//    CCLog("排序规则222222");
    HeroOfPlayerItem* pItem1 = (HeroOfPlayerItem*)pCCObj1;
    HeroOfPlayerItem* pItem2 = (HeroOfPlayerItem*)pCCObj2;

    if (pItem1->getHeroItem()->getRarity() > pItem2->getHeroItem()->getRarity()) /*武将1稀有度大于武将2*/{
        return false;
    } else if (pItem1->getHeroItem()->getRarity() == pItem2->getHeroItem()->getRarity()) /*稀有度相等*/ {
        // 比较等级
        if (pItem1->getLevel() > pItem2->getLevel()) {
            return true;
        } else if (pItem1->getLevel() < pItem2->getLevel()) {
            return false;
        } else {
            if (pItem1->getHeroItem()->getId() > pItem1->getHeroItem()->getId()) {
                return true;
            } else {
                return false;
            }
        }
    } else /*武将1稀有度小于武将2*/{
        return true;
    }
}

/*
 * 功能：
 *      获取武将攻击力
 *
 * 公式：
 *      攻击力 ＝ 基础 + （等级 － 1）* 成长
 *
 * 返回：
 *      武将攻击力
 */
unsigned int HeroOfPlayerItem::getAttack() {
    CCAssert(m_pHeroItem, "Invalid HeroItem");
//    CCLog("hero level = %d", m_level);
    return m_pHeroItem->getBasicProperty() + (m_level - 1) * m_pHeroItem->getGrowthProperty();
}

/*
 * 功能：
 *      获取武将血量
 *
 * 公式：
 *      血量 = 基础血量 +（等级 － 1）* 成长
 *
 * 返回：
 *      武将血量
 */
int HeroOfPlayerItem::getHP() {
    CCAssert(m_pHeroItem, "Invalid HeroItem");
    return m_pHeroItem->getBasicHp() + ((float)(m_level - 1))* m_pHeroItem->getGrowthHp();
}

/* 获取玩家所属武将 uid 号 */
std::string HeroOfPlayerItem::getUniId() {return m_uniId;}
/* 设置玩家所属武将uid */
void HeroOfPlayerItem::setUniId(std::string var) {m_uniId = var;}
/* 获取玩家所属武将等级 */
unsigned int HeroOfPlayerItem::getLevel() {return m_level;}
/* 设置玩家所属武将等级 */
void HeroOfPlayerItem::setLevel(unsigned int var) {m_level = var;}
/* 获取武将经验 */
unsigned int HeroOfPlayerItem::getExp() {return m_exp;}
/* 设置武将经验 */
void HeroOfPlayerItem::setExp(unsigned int var) {m_exp = var;}
/* 获取和设置是否是上阵武将(pve) */
bool HeroOfPlayerItem::getGoIntoBattle() {return m_isGoIntoBattle;}
void HeroOfPlayerItem::setGoIntoBattle(bool var) {m_isGoIntoBattle = var;}
/* 获取和设置是否是上阵武将(pvp) */
bool HeroOfPlayerItem::getPvpGoIntoBattle() {return m_isPvpGoIntoBattle;}
void HeroOfPlayerItem::setPvpGoIntoBattle(bool var) {m_isPvpGoIntoBattle = var;}
/* 获取武将在战场上的位置 */
unsigned int HeroOfPlayerItem::getPosInBattle() {return m_PosInBattle;}
/* 设置武将在战场上的位置 */
void HeroOfPlayerItem::setPosInBattle(unsigned int var) {m_PosInBattle = var;}
/* 是否队长(pve) */
bool HeroOfPlayerItem::isTeamLeader() {
    bool isTeamLeader = false;
    HeroManager* pHeroManager = HeroManager::getInstance();
    if (pHeroManager->getGoIntoBattleHerosOfPlayerCount() > 0) {
        if (!strcmp(this->getUniId().c_str(), pHeroManager->getGoIntoBattleHeroDataByIndex(0)->getUniId().c_str())) {
            isTeamLeader = true;
        }
    }
    return isTeamLeader;
}
/* 是否队长(pvp) */
bool HeroOfPlayerItem::isPvpTeamLeader() {
    bool isTeamLeader = false;
    HeroManager* pHeroManager = HeroManager::getInstance();
    if (pHeroManager->getGoIntoBattleHerosOfPlayerCount() > 0) {
        if (!strcmp(this->getUniId().c_str(), pHeroManager->getPvpGoIntoBattleHeroDataByIndex(0)->getUniId().c_str())) {
            isTeamLeader = true;
        }
    }
    return isTeamLeader;
}
/* 获取和设置武器id */
unsigned int HeroOfPlayerItem::getWeapon() {return m_weapon;}
void HeroOfPlayerItem::setWeapon(unsigned int var) {m_weapon = var;}
/* 获取和设置防具id */
unsigned int HeroOfPlayerItem::getArmor() {return m_armor;}
void HeroOfPlayerItem::setArmor(unsigned int var) {m_armor = var;}
/* 获取和设置是否是好友武将 */
bool HeroOfPlayerItem::getFriendHero() {return m_isFriendHero;}
void HeroOfPlayerItem::setFriendHero(bool var) {m_isFriendHero = var;}
/* 获取和设置是否上锁 */
bool HeroOfPlayerItem::getLock() {return m_isLock;}
void HeroOfPlayerItem::setLock(bool var) {m_isLock = var;}
/* 获取和设置关联武将 */
HeroItem* HeroOfPlayerItem::getHeroItem() {return m_pHeroItem;}
void HeroOfPlayerItem::setHeroItem(HeroItem* var) {m_pHeroItem = var;}
/* 获取和设置是否有掉落物品 */
bool HeroOfPlayerItem::getHasDrop() {return m_hasDrop;}
void HeroOfPlayerItem::setHasDrop(bool var) {m_hasDrop = var;}
/* 获取和设置掉落物品类别 */
std::string HeroOfPlayerItem::getDropItemType() {return m_dropItemType;}
void HeroOfPlayerItem::setDropItemType(std::string var) {m_dropItemType = var;}
/* 获取和设置掉落物品类别 */
std::string HeroOfPlayerItem::getDropItemId() {return m_dropItemId;}
void HeroOfPlayerItem::setDropItemId(std::string var) {m_dropItemId = var;}
/* 获取和设置掉落物品数量 */
unsigned int HeroOfPlayerItem::getCountOfdropItem() {return m_countOfdropItem;}
void HeroOfPlayerItem::setCountOfdropItem(unsigned int var) {m_countOfdropItem = var;}
/* 获取和设置掉落物品 level */
unsigned int HeroOfPlayerItem::getDropItemLevel() {return m_dropItemLevel;}
void HeroOfPlayerItem::setDropItemLevel(unsigned int var) {m_dropItemLevel = var;}

