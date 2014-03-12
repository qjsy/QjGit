//
//  Battle.cpp
//  hero
//
//  Created by yangjie on 2013/12/22.
//

#include "Battle.h"
#include "AttackDataItem.h"
#include "HeroManager.h"
#include "HeroItem.h"
#include "HeroOfPlayerItem.h"
#include "MosterItem.h"
#include "InBattleItem.h"
#include "MosterInBattleItem.h"
#include "NetConnection.h"
#include "GameState.h"
#include "GamePlayer.h"
#include "BattleReportItem.h"

/* 静态实例变量 */
static Battle* s_Battle = NULL;

/*
 * 单例模式，获取 Battle 类唯一实例对象
 */
Battle* Battle::getInstance() {
    if (NULL == s_Battle) {
        s_Battle = new Battle();
    }
    return s_Battle;
}

/*
 *  构造函数
 */
Battle::Battle():
m_battlePkMode(BPM_PVE)
,m_isWin(false)
,m_InBattleHeros(NULL)
,m_InBattleEnemys(NULL)
,m_InBattleEnemyMosters(NULL)
,m_AttackDataItems(NULL)
,m_roundIndex(0)
,m_sequenceIndex(0)
,m_nBattleSpeed(1)
,m_countOfDropHeroCards(0)
,m_countOfDropEquips(0)
,m_countOfDropProps(0)
,m_remainTimes(0)
,m_targetName("")
,m_pvpEnemyCaptain(NULL)
,m_tqtAddMoney(0)
,m_isCWar(false)
,m_selectedCity(0)
,m_pNetConnection(NetConnection::getInstance())
,m_pGameState(GameState::getInstance())
,m_pGamePlayer(GamePlayer::getInstance())
,m_isInList(false)
{
    m_InBattleHeros = CCArray::createWithCapacity(MAX_IN_BATTLE_HEROS);
    m_InBattleHeros->retain();
    
    m_InBattleEnemys = CCArray::createWithCapacity(MAX_IN_BATTLE_HEROS);
    m_InBattleEnemys->retain();

    m_InBattleEnemyMosters = CCArray::createWithCapacity(MAX_IN_BATTLE_HEROS);
    m_InBattleEnemyMosters->retain();
    
    m_AttackDataItems = CCArray::createWithCapacity(32);
    m_AttackDataItems->retain();
    
    m_BattleReportItems = CCArray::createWithCapacity(16);
    m_BattleReportItems->retain();
}

/*
 *  析构函数
 */
Battle::~Battle() {
    clearAllHerosData(true);
    clearAllBattleData(true);
    clearAllBattleReport(true);
}

// 国战逻辑
void Battle::setSelectCity(unsigned int var) {
    m_selectedCity = var;
}

unsigned int Battle::getSelectCity() {
    return m_selectedCity;
}

/*
 *  功能：
 *      清除所有上战场的武将数据
 *  参数：
 *      isRelease       是否释放数据
 */
void Battle::clearAllHerosData(bool isRelease) {
    if (m_InBattleHeros) {
        m_InBattleHeros->removeAllObjects();
        if (isRelease) {
            CC_SAFE_RELEASE_NULL(m_InBattleHeros);
        }
    }
    if (m_InBattleEnemys) {
        m_InBattleEnemys->removeAllObjects();
        if (isRelease) {
            CC_SAFE_RELEASE_NULL(m_InBattleEnemys);
        }
    }
    if (m_InBattleEnemyMosters) {
        m_InBattleEnemyMosters->removeAllObjects();
        if (isRelease) {
            CC_SAFE_RELEASE_NULL(m_InBattleEnemyMosters);
        }
    }
}

/* 
 *  功能：
 *      清除所有上战场的敌人数据
 */
void Battle::clearAllEnemyData() {
    if (m_InBattleEnemys) {
        m_InBattleEnemys->removeAllObjects();
    }
    if (m_InBattleEnemyMosters) {
        m_InBattleEnemyMosters->removeAllObjects();
    }
}

/*
 *  功能：
 *      清除战场数据(pve)
 *  参数：
 *      clearRound      是否清除战斗回合数据
 *      isRelease       是否释放数据
 */
void Battle::clearAllBattleData(const bool clearRound, const bool isRelease) {
    m_isWin = false;
    m_sequenceIndex = 0;
    
    if (clearRound) {
        m_roundIndex = 0;
        m_countOfDropHeroCards = 0;
        m_countOfDropEquips = 0;
        m_countOfDropProps = 0;
    }
    
    if (m_AttackDataItems) {
        m_AttackDataItems->removeAllObjects();
        if (isRelease) {
            CC_SAFE_RELEASE_NULL(m_AttackDataItems);
        }
    }
}

/*
 *  功能：
 *      删除所有战斗回合数据
 */
void Battle::clearAllAttackData() {
    if (m_AttackDataItems) {
        m_AttackDataItems->removeAllObjects();
    }
}

/* 清除所有战斗报告数据 */
void Battle::clearAllBattleReport(const bool isRelease) {
    if (m_BattleReportItems) {
        m_BattleReportItems->removeAllObjects();
        if (isRelease) {
            CC_SAFE_RELEASE_NULL(m_BattleReportItems);
        }
    }
}

/*
 *  功能：
 *      添加战斗报告
 *  参数：
 *      pHeroOfPlayerItem   武将数据项
 */
void Battle::appendBattleReport(BattleReportItem* pBattleReportItem) {
    CCAssert(pBattleReportItem, "invalid data");
    m_BattleReportItems->addObject(pBattleReportItem);
}

/*
 *  功能：
 *      根据索引号获取战斗报告
 *  参数：
 *      index   索引号
 *  返回：
 *      成功返回战斗报告，否则返回 NULL
 */
BattleReportItem* Battle::getBattleReportItemByIndex(const unsigned int index) const {
    if (m_BattleReportItems->count() == 0 || index > m_BattleReportItems->count() - 1)
        return NULL;
    return (BattleReportItem*)m_BattleReportItems->objectAtIndex(index);
}

/*
 *  功能：
 *      添加我方上阵武将
 *  参数：
 *      pHeroOfPlayerItem   要添加的武将数据项
 */
void Battle::appendInBattleHero(HeroOfPlayerItem* pHeroOfPlayerItem) {
    CCAssert(m_InBattleHeros && pHeroOfPlayerItem, "invalid data");
    m_InBattleHeros->addObject(pHeroOfPlayerItem);
}

/*
 *  功能：
 *      移除我方上阵武将
 *  参数：
 *      pHeroOfPlayerItem   要移除的武将数据项
 */
void Battle::removeInBattleHero(HeroOfPlayerItem* pHeroOfPlayerItem) {
    CCAssert(m_InBattleHeros && pHeroOfPlayerItem, "invalid data");
    m_InBattleHeros->removeObject(pHeroOfPlayerItem);
}

/*
 *  功能：
 *      添加敌方数据 (pvp)
 *  参数：
 *      pEnemyItem   敌方数据项
 */
void Battle::appendInBattleEnemyPvp(InBattleItem* pEnemyItem) {
    CCAssert(m_InBattleEnemys && pEnemyItem, "invalid data");
    m_InBattleEnemys->addObject(pEnemyItem);
}

/*
 *  功能：
 *      增加敌方怪物 (pve)
 *  参数：
 *      pEnemyMosterItem   敌方怪物数据项
 */
void Battle::appendInBattleEnemyMoster(MosterInBattleItem* pEnemyMosterItem) {
    CCAssert(m_InBattleEnemyMosters && pEnemyMosterItem, "invalid data");
    m_InBattleEnemyMosters->addObject(pEnemyMosterItem);
}

/*
 *  功能：
 *      添加攻击数据项
 *  参数：
 *      pAttackDataItem       攻击数据项
 */
void Battle::appendAttackDataItem(AttackDataItem* pAttackDataItem) {
    CCAssert(pAttackDataItem && m_AttackDataItems, "invalid data");
    m_AttackDataItems->addObject((CCObject*)pAttackDataItem);
}

/*
 *  功能：
 *      根据索引号获取我方武将数据项
 *  参数：
 *      index     索引
 *  返回：
 *      成功返回武将数据项，失败返回 NULL
 */
HeroOfPlayerItem* Battle::getHeroDataByIndex(const unsigned int index) {
    return index >= m_InBattleHeros->count() ?
        NULL : static_cast<HeroOfPlayerItem*>(m_InBattleHeros->objectAtIndex(index));
}

/*
 *  功能：
 *      根据索引号获取敌方数据项
 *  参数：
 *      index     索引
 *  返回：
 *      成功返回敌方数据项，失败返回 NULL
 */
InBattleItem* Battle::getEnemyDataByIndex(const unsigned int index) {
    return index >= m_InBattleEnemys->count() ?
        NULL : static_cast<InBattleItem*>(m_InBattleEnemys->objectAtIndex(index));
}

/*
 *  功能：
 *      根据 id 和 位置获取敌方怪物数据项 (pve)
 *  参数：
 *      id      敌方怪物 id 值
 *      pos     敌方怪物在战场上的位置
 *  返回：
 *      成功返回敌方怪物数据项，失败返回 NULL
 */
MosterInBattleItem* Battle::getEnemyMosterDataByIdAndPos(const unsigned int id, const unsigned int pos) {
    bool isFound = false;
    MosterInBattleItem* pMosterItem = NULL;
    CCObject* item = NULL;
    CCARRAY_FOREACH(m_InBattleEnemyMosters, item) {
        pMosterItem = (MosterInBattleItem*)item;
        if (pMosterItem->getId() == id && pMosterItem->getPosInBattle() == pos) {
            isFound = true;
            break;
        }
    } /*for*/
    
    return isFound ? pMosterItem : NULL;
}

/*
 *  功能：
 *      根据索引号获取敌方怪物数据项
 *  参数：
 *      index     索引
 *  返回：
 *      成功返回敌方怪物数据项，失败返回 NULL
 */
MosterInBattleItem* Battle::getEnemyMosterDataByIndex(const unsigned int index) {
    return index >= m_InBattleEnemyMosters->count() ?
        NULL : static_cast<MosterInBattleItem*>(m_InBattleEnemyMosters->objectAtIndex(index));
}

/*
 *  功能：
 *      根据起始位置索引和终点位置索引获得下一个位置索引(水平方向优先)
 *  参数：
 *      startPosIndex    起始位置索引
 *      endPosIndex      终点位置索引
 *      dir              保存位移方向（队伍朝向）
 *  返回：
 *      下一个位置索引
 */
unsigned int Battle::getNextPosIndexHorizontal(const unsigned int startPosIndex, const unsigned int endPosIndex,
                                               unsigned int& dir) {
    CCAssert(startPosIndex != endPosIndex, "invalid posIndex");
    
    unsigned int nextPosIndex = 0;
    unsigned int endMod = endPosIndex % 10, startMod = startPosIndex % 10;
    if ( endMod != startMod ) {
        dir = endMod > startMod ? 2 : 4;
        nextPosIndex = startPosIndex + (endMod > startMod ? 1 : -1);
    } else {
        dir = endPosIndex > startPosIndex ? 1 : 3;
        nextPosIndex = startPosIndex + (endPosIndex > startPosIndex ? 10 : -10);
    }
    return nextPosIndex;
}

/*
 *  功能：
 *      根据起始位置索引和终点位置索引获得下一个位置索引(垂直方向优先)
 *  参数：
 *      startPosIndex    起始位置索引
 *      endPosIndex      终点位置索引
 *      dir              保存位移方向（队伍朝向）
 *  返回：
 *      下一个位置索引
 */
unsigned int Battle::getNextPosIndexVertical(const unsigned int startPosIndex,
                                           const unsigned int endPosIndex, unsigned int& dir) {
    CCAssert(startPosIndex != endPosIndex, "invalid posIndex");
    
    unsigned int nextPosIndex = 0;
    if (abs(endPosIndex - startPosIndex) > 3) {
        dir = endPosIndex > startPosIndex ? 1 : 3;
        nextPosIndex = startPosIndex + (endPosIndex > startPosIndex ? 10 : -10);
    } else {
        dir = endPosIndex > startPosIndex ? 2 : 4;
        nextPosIndex = startPosIndex + (endPosIndex > startPosIndex ? 1 : -1);
    }
    return nextPosIndex;
}

/*
 *  功能：
 *      判断终点位置索引是否是起始位置索引的相邻位置索引
 *  参数：
 *      startPosIndex    起始位置索引
 *      endPosIndex      终点位置索引
 *      dir              保存位移方向（队伍朝向）
 *  返回：
 *      相邻返回 true，否则返回 false
 */
bool Battle::isNeighborPosIndex(const unsigned int startPosIndex, const unsigned int endPosIndex, unsigned int& dir) {
    CCAssert(startPosIndex != endPosIndex, "invalid posIndex");

    bool isNeighbor = false;
    if (abs(endPosIndex - startPosIndex) == 1) {
        dir = endPosIndex > startPosIndex ? 2 : 4;
        isNeighbor = true;
    } else if (abs(endPosIndex - startPosIndex) == 10) {
        dir = endPosIndex > startPosIndex ? 1 : 3;
        isNeighbor = true;
    }
    return isNeighbor;
}

/*
 *  功能：
 *      判断终点位置索引是否和起始位置索引是垂直位置关系
 *  参数：
 *      startPosIndex    起始位置索引
 *      endPosIndex      终点位置索引
 *      dir              保存位移方向（队伍朝向）
 *  返回：
 *      是返回 true，否则返回 false
 */
bool Battle::isVerticalPosIndex(const unsigned int startPosIndex, const unsigned int endPosIndex, unsigned int& dir) {
    CCAssert(startPosIndex != endPosIndex, "invalid posIndex");

    dir = endPosIndex > startPosIndex ? 1 : 3;
    return (abs(endPosIndex - startPosIndex) % 10 == 0);
}

/*
 *  功能：
 *      判断终点位置索引是否和起始位置索引是水平位置关系
 *  参数：
 *      startPosIndex    起始位置索引
 *      endPosIndex      终点位置索引
 *      dir              保存位移方向（队伍朝向）
 *  返回：
 *      是返回 true，否则返回 false
 */
bool Battle::isHorizontalPosIndex(const unsigned int startPosIndex, const unsigned int endPosIndex, unsigned int& dir) {
    CCAssert(startPosIndex != endPosIndex, "invalid posIndex");

    dir = endPosIndex > startPosIndex ? 2 : 4;
    return (abs(endPosIndex - startPosIndex) < 3);
}

/*
 * 提交 PVE 战斗请求
 */
void Battle::commitPveFightRequest() {
    unsigned int posInBattle[] = {21,22,23,11,12,13};
    char szPostBuffer[MAX_POSTTO_SERVER_BUFFER_SIZE];
    memset(szPostBuffer, '\0', MAX_POSTTO_SERVER_BUFFER_SIZE);
    
    // http tag
    char szHttpTag[] = HTTP_REQUEST_FIGHT_PVE_TAG;
    // 构造 post
    strcpy(szPostBuffer, "c=map&a=fight&position=");
    CCObject* pArrayObject;
    for (unsigned int i = 0; i < MAX_IN_BATTLE_HEROS; i++) {
        CCARRAY_FOREACH(m_InBattleHeros, pArrayObject) {
            HeroOfPlayerItem* pHeroOfPlayerItem = static_cast<HeroOfPlayerItem*>(pArrayObject);
            if (pHeroOfPlayerItem->getPosInBattle() == posInBattle[i]) {
                strcat(szPostBuffer, pHeroOfPlayerItem->getUniId().c_str());
                break;
            }
        } /*for*/
        if (i < 5)
            strcat(szPostBuffer, "|");
    } /*for*/
    char szCopyID[16];
    sprintf(szCopyID, "&copy=%d", m_pGameState->getCopyId());
    char szLevelID[16];
    sprintf(szLevelID, "&level=%d", m_pGameState->getLevId());
    strcat(szPostBuffer, szCopyID);
    strcat(szPostBuffer, szLevelID);
    strcat(szPostBuffer, "&uid=");
    strcat(szPostBuffer, m_pGamePlayer->getUid().c_str());
    
    // 提交 post
    CCLog("post buffer = %s", szPostBuffer);
    m_pNetConnection->commitPostRequestData(szPostBuffer, szHttpTag);
}

/*
 *  提交铜雀台(pvp)请求
 */
void Battle::commitTqtFightRequest() {
    unsigned int posInBattle[] = {21,22,23,11,12,13};
    char szPostBuffer[MAX_POSTTO_SERVER_BUFFER_SIZE];
    memset(szPostBuffer, '\0', MAX_POSTTO_SERVER_BUFFER_SIZE);
    
    // http tag
    char szHttpTag[] = HTTP_REQUEST_FIGHT_TQT_TAG;
    sprintf(szPostBuffer, "c=contest&a=fight&position=%d&team=", m_tqtTeamPosition);
    CCObject* pArrayObject;
    for (unsigned int i = 0; i < MAX_IN_BATTLE_HEROS; i++) {
        CCARRAY_FOREACH(m_InBattleHeros, pArrayObject) {
            HeroOfPlayerItem* pHeroOfPlayerItem = static_cast<HeroOfPlayerItem*>(pArrayObject);
            if (pHeroOfPlayerItem->getPosInBattle() == posInBattle[i]) {
                strcat(szPostBuffer, pHeroOfPlayerItem->getUniId().c_str());
                break;
            }
        } /*for*/
        if (i < 5)
            strcat(szPostBuffer, "|");
    } /*for*/
    strcat(szPostBuffer, "&uid=");
    strcat(szPostBuffer, m_pGamePlayer->getUid().c_str());
    
    // 提交 post
    CCLog("post tqt buffer = %s", szPostBuffer);
    m_pNetConnection->commitPostRequestData(szPostBuffer, szHttpTag);
}
    
/*
 *  提交国战(pvp)请求
 */
void Battle::commitCWarFightRequest() {
    CCLog("提交国战请求");
    
    unsigned int posInBattle[] = {21,22,23,11,12,13};
    char szPostBuffer[MAX_POSTTO_SERVER_BUFFER_SIZE];
    memset(szPostBuffer, '\0', MAX_POSTTO_SERVER_BUFFER_SIZE);
    
    // http tag
    char szHttpTag[] = HTTP_REQUEST_FIGHT_CWAR_TAG;
    // 构造 post
    sprintf(szPostBuffer, "c=cwar&a=battle&city=%d&team=", m_selectedCity);
    CCObject* pArrayObject = NULL;
    for (unsigned int i = 0; i < MAX_IN_BATTLE_HEROS; i++) {
        CCARRAY_FOREACH(m_InBattleHeros, pArrayObject) {
            HeroOfPlayerItem* pHeroOfPlayerItem = static_cast<HeroOfPlayerItem*>(pArrayObject);
            if (pHeroOfPlayerItem->getPosInBattle() == posInBattle[i]) {
                strcat(szPostBuffer, pHeroOfPlayerItem->getUniId().c_str());
                break;
            }
        } /*for*/
        if (i < 5)
            strcat(szPostBuffer, "|");
    } /*for*/
    strcat(szPostBuffer, "&uid=");
    strcat(szPostBuffer, m_pGamePlayer->getUid().c_str());
    
    // 提交 post
    CCLog("post cwar buffer = %s", szPostBuffer);
    m_pNetConnection->commitPostRequestData(szPostBuffer, szHttpTag);
}

/*
 *  功能：
 *      设置 pvp 敌方队长
 *  参数：
 *      pInBattleItem   pvp 敌方队长
 */
void Battle::setPvpEnemyCaptain(const InBattleItem* pInBattleItem) {
    CCAssert(pInBattleItem, "invalid data");
    m_pvpEnemyCaptain = const_cast<InBattleItem*>(pInBattleItem);
    m_pvpEnemyCaptain->retain();
}

/*
 *  清除 pvp 队长
 */
void Battle::clearPvpEnemyCaptain() {
    CC_SAFE_RELEASE_NULL(m_pvpEnemyCaptain);
}

