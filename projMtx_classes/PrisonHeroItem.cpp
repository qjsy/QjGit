//
//  PrisonHeroItem.cpp
//  hero
//
//  Created by yangjie on 2014/02/20.
//
//

#include "PrisonHeroItem.h"
#include "HeroManager.h"
#include "HeroItem.h"

/*
 *  初始化
 */
bool PrisonHeroItem::init() {
    m_heroId = 0;
    m_pHeroItem = NULL;
    
    m_money = 0;
    m_devotion = 0;
//    m_propId = 0;
    
    return true;
}

unsigned int PrisonHeroItem::getHeroId() {return m_heroId;}
void PrisonHeroItem::setHeroId(unsigned int var) {
    HeroItem* pHeroItem = HeroManager::getInstance()->getHeroItem(var);
    CCAssert(pHeroItem, "invalid heroId");
    
    m_heroId = var;
    m_pHeroItem = pHeroItem;
}

unsigned int PrisonHeroItem::getMoney() {return m_money;}
void PrisonHeroItem::setMoney(unsigned int var) {m_money = var;}
unsigned int PrisonHeroItem::getDevotion() {return m_devotion;}
void PrisonHeroItem::setDevotion(unsigned int var) {m_devotion = var;}
unsigned int PrisonHeroItem::getNum() {return m_num;}
void PrisonHeroItem::setNum(unsigned int var) {m_num = var;}
PRISON_PROP PrisonHeroItem::getTypeProp() {return m_typeProp;}
void PrisonHeroItem::setTypeProp(PRISON_PROP var) {m_typeProp = var;}
/*此英雄下次招降所需的gold*/
unsigned int PrisonHeroItem::getGold() {return m_gold;}
void PrisonHeroItem::setGold(unsigned int var) {m_gold = var;}

/*
 * 监狱武将排序
 */
bool PrisonHeroItem::lessForPrison(const CCObject* pCCObj1, const CCObject* pCCObj2) {
    PrisonHeroItem* pItem1 = (PrisonHeroItem*)pCCObj1;
    PrisonHeroItem* pItem2 = (PrisonHeroItem*)pCCObj2;
    
    if (pItem1->getHeroItem()->getRarity() > pItem2->getHeroItem()->getRarity())/*武将1稀有度大于武将2*/ {
        return true;
    } else if (pItem1->getHeroItem()->getRarity() == pItem2->getHeroItem()->getRarity())/*稀有度相等*/ {
        // 忠诚度
        if (pItem1->getDevotion() < pItem2->getDevotion())/*武将1忠诚度大于武将2*/ {
            return true;
        } else if (pItem1->getDevotion() == pItem2->getDevotion())/*忠诚度相等*/ {
            // id
            if (pItem1->getHeroItem()->getId() > pItem2->getHeroItem()->getId()) {
                return false;
            } else {
                return  true;
            }
        } else {
            return false;
        }
    } else {
        return false;
    }
}

/* 根据武将id 获得释放武将获得铜钱 加载本地表 */
