//
//  ProcessDataFromServer.cpp
//  hero
//
//  Created by yangjie on 2013/12/12.
//
//

#include "ProcessData.h"
#include "Json.h"

#include "MessageBox.h"
#include "HeroManager.h"
#include "AttackDataItem.h"
#include "AttackDamageDataItem.h"
#include "Battle.h"
#include "BattleReportItem.h"
#include "HeroItem.h"
#include "HeroAdditionItem.h"
#include "HeroOfPlayerItem.h"
#include "MosterItem.h"
#include "MosterInBattleItem.h"
#include "HeroFhpItem.h"
#include "LevelDataItem.h"
#include "CopyDataItem.h"
#include "CopyManager.h"
#include "SkillItem.h"
#include "SkillManager.h"
#include "GamePlayer.h"
#include "common.h"
#include "NetConnection.h"
#include "GameState.h"
#include "DropItem.h"
#include "DropItemManager.h"
#include "PrisonHeroItem.h"
#include "LoyaltyItem.h"
#include "TqHeadItem.h"
#include "CityManager.h"
#include "CityItem.h"
#include "CityFunctionItem.h"
#include "CityAwardItem.h"
#include "HeroIntroduce.h"

/*静态实例变量*/
static ProcessData* s_ProcessData = NULL;

/*
 *  加载本地数据线程
 */
void* loadLocalDataThread(void* para) {
    ProcessData* pProcessData = static_cast<ProcessData*>(para);
    pProcessData->m_localDataIsReady = 0;
    
    // 加载本地配置数据
    ProcessData::loadLocalDataFromJson();
    
    // 置本地数据文件加载完成标志
    __sync_fetch_and_add(&pProcessData->m_localDataIsReady, 1);
    
    return NULL;
}

/* 获取单例 */
ProcessData* ProcessData::getInstance() {
    return NULL == s_ProcessData ? (s_ProcessData = new ProcessData) : s_ProcessData;
}

/* 启动加载本地数据的线程 */
void ProcessData::startLoadLocalDataThread() {
    pthread_t pthid;
    pthread_attr_t threadAttr;
    pthread_attr_init (&threadAttr);
    pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setstacksize(&threadAttr, 16384);
    // 开启加载本地数据线程
    pthread_create(&pthid, &threadAttr, loadLocalDataThread, this);
    pthread_attr_destroy(&threadAttr);
}

/*
 *  处理服务器回复的pve上阵武将数据
 */
static bool processGoIntoBattle(const char *szBuffer) {
    CCAssert(szBuffer, "invalid buffer");
    
    HeroManager* pHeroManager = HeroManager::getInstance();
    
    bool isSuccess = false;
    Json* pRootJson = Json_create(szBuffer);
    do {
        CCAssert(pRootJson, "invalid json data");
        
        Json* pSuccessJson = Json_getItem(pRootJson, "success");
        CC_BREAK_IF(1 != pSuccessJson->valueint);
        
        Json* pResultJson = Json_getItem(pRootJson, "result");
        Json* pHerosJson = Json_getItem(pResultJson, "hero");
        Json* pHeroItemJson = pHerosJson->child;
        
        std::string uniid[MAX_GO_INTO_BATTLE_HEROS];    // 保存pve上阵武将 uniid
        unsigned int numberOfGotoBattleHeros = 0;       // 保存pve上阵武将数量
        while (pHeroItemJson) {
            if (pHeroItemJson->valuestring) {
                uniid[numberOfGotoBattleHeros++].assign(pHeroItemJson->valuestring);
            }
            // 下一个上阵武将
            pHeroItemJson = pHeroItemJson->next;
        } /*while*/
        pHeroManager->updateGoIntoBattleHerosOfPlayer(numberOfGotoBattleHeros, uniid);
        
        isSuccess = true;
    } while (0);
    
    if (!isSuccess && pRootJson) {
        Json* pMsgJson = Json_getItem(pRootJson, "msg");
        CCLog("error msg = %s", pMsgJson->valuestring);
        MessageBox::getInstance()->messageTips(pMsgJson->valuestring);
    }
    if (pRootJson) {
        Json_dispose(pRootJson);
        pRootJson = NULL;
    }
    
    return isSuccess;
}

/*
 *  处理服务器回复的pvp上阵武将数据
 */
static bool processGoIntoBattlePvp(const char *szBuffer) {
    CCAssert(szBuffer, "invalid buffer");
    
    HeroManager* pHeroManager = HeroManager::getInstance();
    bool isSuccess = false;
    Json* pRootJson = Json_create(szBuffer);
    do {
        CCAssert(pRootJson, "invalid json");
        
        Json* pSuccessJson = Json_getItem(pRootJson, "success");
        CC_BREAK_IF(1 != pSuccessJson->valueint);
        
        Json* pResultJson = Json_getItem(pRootJson, "result");
        Json* pHerosJson = Json_getItem(pResultJson, "position");
        
        char* szHeroUid = NULL;
        char szPvpHeroBuffer[128];
        strcpy(szPvpHeroBuffer, pHerosJson->valuestring);
        
        std::string uniid[MAX_GO_INTO_BATTLE_HEROS];                // 保存pvp上阵武将 uniid
        unsigned int posIndexInBattle[MAX_GO_INTO_BATTLE_HEROS];    // 保存战队位置索引
        
        unsigned int posInBattle[] = {21,22,23,11,12,13};
        unsigned int tempIndex = 0;
        unsigned int countOfPvpGotoBattleHeros = 0;        // 保存pvp上阵武将数量
        szHeroUid = strtok(szPvpHeroBuffer, "|");
        while (szHeroUid) {
            if (strlen(szHeroUid) > 1) {
                uniid[countOfPvpGotoBattleHeros].assign(szHeroUid);
                posIndexInBattle[countOfPvpGotoBattleHeros] = posInBattle[tempIndex];
                countOfPvpGotoBattleHeros++;
            }
            tempIndex++;
            szHeroUid = strtok(NULL, "|");
        } /*while*/
        pHeroManager->updatePvpGoIntoBattleHerosOfPlayer(countOfPvpGotoBattleHeros, uniid, posIndexInBattle);
        
        isSuccess = true;
    } while (0);
    
    if (!isSuccess && pRootJson) {
        Json* pMsgJson = Json_getItem(pRootJson, "msg");
        CCLog("error msg = %s", pMsgJson->valuestring);
        MessageBox::getInstance()->messageTips(pMsgJson->valuestring);
    }
    if (pRootJson) {
        Json_dispose(pRootJson);
        pRootJson = NULL;
    }
    
    return isSuccess;
}

/*
 *  处理服务器回复的武将升级数据
 */
static bool processHeroLevUp(const char *szBuffer) {
    CCAssert(szBuffer, "invalid buffer");
    bool isSuccess = false;

    HeroManager* pHeroManager = HeroManager::getInstance();
    Json* pRootJson = Json_create(szBuffer);
    do {
        CCAssert(pRootJson, "invalid json data");
        
        Json* pSuccessJson = Json_getItem(pRootJson, "success");
        CC_BREAK_IF(1 != pSuccessJson->valueint);
        
        Json* pResultJson = Json_getItem(pRootJson, "result");
        Json* pHeroItemJson = Json_getItem(pResultJson, "target");
        
        Json* pHeroItemTagJson = Json_getItem(pHeroItemJson, "tag");
        Json* pHeroItemLevelJson = Json_getItem(pHeroItemJson, "level");
        Json* pHeroItemExpJson = Json_getItem(pHeroItemJson, "exp");
        Json* pHeroItemWeaponJson = Json_getItem(pHeroItemJson, "weapon");
        Json* pHeroItemArmorJson = Json_getItem(pHeroItemJson, "armor");
        Json* pHeroItemLockJson = Json_getItem(pHeroItemJson, "lock");
        
        HeroOfPlayerItem* pHeroOfPlayerItem = pHeroManager->getHeroDataByUniId(pHeroItemTagJson->valuestring);
        
        pHeroOfPlayerItem->setLevel(pHeroItemLevelJson->valueint);
        pHeroOfPlayerItem->setExp(pHeroItemExpJson->valueint);
        pHeroOfPlayerItem->setWeapon(pHeroItemWeaponJson->valueint);
        pHeroOfPlayerItem->setArmor(pHeroItemArmorJson->valueint);
        pHeroOfPlayerItem->setLock(pHeroItemLockJson->valueint == 1 ? true : false);
        
        isSuccess = true;
    } while (0);
    
    if (!isSuccess && pRootJson) {
        Json* pMsgJson = Json_getItem(pRootJson, "msg");
        CCLog("error msg = %s", pMsgJson->valuestring);
        MessageBox::getInstance()->messageTips(pMsgJson->valuestring);
    }
    if (pRootJson) {
        Json_dispose(pRootJson);
        pRootJson = NULL;
    }
    
    return isSuccess;
}

/*
 *  处理服务器回复的武将出售数据
 */
static bool processHeroSell(const char *szBuffer) {
    CCAssert(szBuffer, "invalid buffer");
    
    GamePlayer* pGamer = GamePlayer::getInstance();
    bool isSuccess = false;
    Json* pRootJson = Json_create(szBuffer);
    do {
        CCAssert(pRootJson, "invalid json data");
        Json* pSuccessJson = Json_getItem(pRootJson, "success");
        CC_BREAK_IF(1 != pSuccessJson->valueint);
        
        Json* pResultJson = Json_getItem(pRootJson, "result");
        Json* pMoneyJson = Json_getItem(pResultJson, "money");
        pGamer->setMoney(pMoneyJson->valueint + pGamer->getMoney());
        
        isSuccess = true;
    } while (0);
    
    if (!isSuccess && pRootJson) {
        Json* pMsgJson = Json_getItem(pRootJson, "msg");
        CCLog("error msg = %s", pMsgJson->valuestring);
        MessageBox::getInstance()->messageTips(pMsgJson->valuestring);
    }
    if (pRootJson) {
        Json_dispose(pRootJson);
        pRootJson = NULL;
    }
    
    return isSuccess;
}

/*
 *  处理服务器回复的武将升阶数据
 */
static bool processHeroEvoUp(const char *szBuffer) {
    CCAssert(szBuffer, "invalid buffer");
//    CCLog("武将升阶 = %s", szBuffer);
    
    HeroManager* pHeroManager = HeroManager::getInstance();
    bool isSuccess = false;
    Json* pRootJson = Json_create(szBuffer);
    do {
        CCAssert(pRootJson, "invalid json data");
        Json* pSuccessJson = Json_getItem(pRootJson, "success");
        CC_BREAK_IF(1 != pSuccessJson->valueint);
        
        Json* pResultJson = Json_getItem(pRootJson, "result");
        Json* pHeroItemJson = Json_getItem(pResultJson, "hero");
        Json* pHeroItemTagJson = Json_getItem(pHeroItemJson, "tag");
        Json* pIdTagJson = Json_getItem(pHeroItemJson, "id");
        Json* pHeroItemLevelJson = Json_getItem(pHeroItemJson, "level");
        Json* pHeroItemExpJson = Json_getItem(pHeroItemJson, "exp");
        Json* pHeroItemWeaponJson = Json_getItem(pHeroItemJson, "weapon");
        Json* pHeroItemArmorJson = Json_getItem(pHeroItemJson, "armor");
        Json* pHeroItemLockJson = Json_getItem(pHeroItemJson, "lock");
        
        HeroOfPlayerItem* pHeroOfPlayerItem = pHeroManager->getHeroDataByUniId(pHeroItemTagJson->valuestring);
        CCAssert(NULL == pHeroOfPlayerItem, "invalid data"); // 已经存在
        
        HeroItemOfPlayerFromServer hiopFS;
        hiopFS.uniId.assign(pHeroItemTagJson->valuestring);
        hiopFS.id = atoi(pIdTagJson->valuestring);
        hiopFS.level = pHeroItemLevelJson->valueint;
        hiopFS.exp = pHeroItemExpJson->valueint;
        hiopFS.weapon = pHeroItemWeaponJson->valueint;
        hiopFS.armor = pHeroItemArmorJson->valueint;
        hiopFS.isLock = pHeroItemLockJson->valueint == 1 ? true : false;
        
        // 增加武将记录
        pHeroManager->appendHeroOfPlayer(hiopFS);
        
        //////////////////////////////////////////
        //  升阶的是上阵武将的话需要更新全部上阵武将  //
        /////////////////////////////////////////
        Json* pHeroGoToBattleJson = Json_getItem(pResultJson, "fight");
        if (pHeroGoToBattleJson) {
            unsigned int GoIntoBattleHeroIndex = 0;     // 上阵武将索引号
            std::string uniid[MAX_GO_INTO_BATTLE_HEROS];
            Json* pGoIntoBattleHeroItemJson = pHeroGoToBattleJson->child;
            while (pGoIntoBattleHeroItemJson) {
                if (pGoIntoBattleHeroItemJson->valuestring && strlen(pGoIntoBattleHeroItemJson->valuestring) > 0) {
                    uniid[GoIntoBattleHeroIndex++] = pGoIntoBattleHeroItemJson->valuestring;
                }
                // 下一个上阵武将 tag
                pGoIntoBattleHeroItemJson = pGoIntoBattleHeroItemJson->next;
            } /*while*/
            
            if (GoIntoBattleHeroIndex > 0) {
                // 更新上阵武将
                pHeroManager->updateGoIntoBattleHerosOfPlayer(GoIntoBattleHeroIndex, uniid);
            }
        }
        GameState::getInstance()->setHeroEvoUp(pHeroItemTagJson->valuestring);
        
        isSuccess = true;
    } while (0);
    
    if (!isSuccess && pRootJson) {
        Json* pMsgJson = Json_getItem(pRootJson, "msg");
        CCLog("error msg = %s", pMsgJson->valuestring);
        MessageBox::getInstance()->messageTips(pMsgJson->valuestring);
    }
    if (pRootJson) {
        Json_dispose(pRootJson);
        pRootJson = NULL;
    }
    
    return isSuccess;
}

/*
 * 免费抽取英雄
 */
static bool processFree(const char *szBuffer) {
    CCAssert(szBuffer, "invalid buffer");

    HeroManager* pHeroManager = HeroManager::getInstance();
    bool isSuccess = false;
    Json* pRootJson = Json_create(szBuffer);
    do {
        CCAssert(pRootJson, "invalid json data");
        Json* pSuccessJson = Json_getItem(pRootJson, "success");
        CC_BREAK_IF(1 != pSuccessJson->valueint);
        
        Json* pResultJson = Json_getItem(pRootJson, "result");
        Json* pHItemJson = Json_getItem(pResultJson, "hero");
        Json* pHeroItemJson = pHItemJson->child;
        while (pHeroItemJson) {
            Json* pHeroItemTagJson = Json_getItem(pHeroItemJson, "tag");
            Json* pIdTagJson = Json_getItem(pHeroItemJson, "id");
            Json* pHeroItemLevelJson = Json_getItem(pHeroItemJson, "level");
            Json* pHeroItemExpJson = Json_getItem(pHeroItemJson, "exp");
            Json* pHeroItemWeaponJson = Json_getItem(pHeroItemJson, "weapon");
            Json* pHeroItemArmorJson = Json_getItem(pHeroItemJson, "armor");
            Json* pHeroItemLockJson = Json_getItem(pHeroItemJson, "lock");
            
            HeroItemOfPlayerFromServer hiopFS;
            hiopFS.uniId.assign(pHeroItemTagJson->valuestring);
            hiopFS.id = pIdTagJson->valueint;
            hiopFS.level = atoi(pHeroItemLevelJson->valuestring);
            hiopFS.exp = pHeroItemExpJson->valueint;
            hiopFS.weapon = pHeroItemWeaponJson->valueint;
            hiopFS.armor = pHeroItemArmorJson->valueint;
            hiopFS.isLock = pHeroItemLockJson->valueint == 1 ? true : false;
            hiopFS.isGoIntoBattle = false;
            
            // 增加武将记录
            pHeroManager->appendHeroOfPlayer(hiopFS);
            // 下一个
            pHeroItemJson = pHeroItemJson->next;
        } /*while*/
        
        isSuccess = true;
    } while(0);
    
    if (!isSuccess && pRootJson) {
        Json* pMsgJson = Json_getItem(pRootJson, "msg");
        CCLog("error msg = %s", pMsgJson->valuestring);
        MessageBox::getInstance()->messageTips(pMsgJson->valuestring);
    }
    if (pRootJson) {
        Json_dispose(pRootJson);
        pRootJson = NULL;
    }
    
    return isSuccess;
}

/* 
 ＊  付费抽一次 ffff
 */
static bool processNFree(const char *szBuffer) {
    CCAssert(szBuffer, "invalid buffer");
    bool isSuccess = false;
    
    HeroManager* pHeroManager = HeroManager::getInstance();
    Json* pRootJson = Json_create(szBuffer);
    do {
        CCAssert(pRootJson, "invalid json data");
        Json* pSuccessJson = Json_getItem(pRootJson, "success");
        CC_BREAK_IF(1 != pSuccessJson->valueint);
        
        Json* pResultJson = Json_getItem(pRootJson, "result");
        Json* pHItemJson = Json_getItem(pResultJson, "hero");
        Json* pHeroItemJson = pHItemJson->child;
        while (pHeroItemJson) {
            Json* pHeroItemTagJson = Json_getItem(pHeroItemJson, "tag");
            Json* pIdTagJson = Json_getItem(pHeroItemJson, "id");
            Json* pHeroItemLevelJson = Json_getItem(pHeroItemJson, "level");
            Json* pHeroItemExpJson = Json_getItem(pHeroItemJson, "exp");
            Json* pHeroItemWeaponJson = Json_getItem(pHeroItemJson, "weapon");
            Json* pHeroItemArmorJson = Json_getItem(pHeroItemJson, "armor");
            Json* pHeroItemLockJson = Json_getItem(pHeroItemJson, "lock");
            
            HeroItemOfPlayerFromServer hiopFS;
            hiopFS.uniId.assign(pHeroItemTagJson->valuestring);
            hiopFS.id = pIdTagJson->valueint;
            hiopFS.level = atoi(pHeroItemLevelJson->valuestring);
            hiopFS.exp = pHeroItemExpJson->valueint;
            hiopFS.weapon = pHeroItemWeaponJson->valueint;
            hiopFS.armor = pHeroItemArmorJson->valueint;
            hiopFS.isLock = pHeroItemLockJson->valueint == 1 ? true : false;
            hiopFS.isGoIntoBattle = false;
            
            // 增加背包武将记录
            pHeroManager->appendHeroOfPlayer(hiopFS);
            
            // 下一个
            pHeroItemJson = pHeroItemJson->next;
        } /*while*/
        
        isSuccess = true;
    } while(0);
    
    if (!isSuccess && pRootJson) {
        Json* pMsgJson = Json_getItem(pRootJson, "msg");
        CCLog("error msg = %s", pMsgJson->valuestring);
        MessageBox::getInstance()->messageTips(pMsgJson->valuestring);
    }
    if (pRootJson) {
        Json_dispose(pRootJson);
        pRootJson = NULL;
    }
    
    return isSuccess;
}

/* 
 *  付费抽10次
 */
static bool processNFreeTen(const char *szBuffer) {
    CCAssert(szBuffer, "invalid buffer");
//    CCLog("付费10抽 = %s", szBuffer);
    
    HeroManager* pHeroManager = HeroManager::getInstance();
    Json* pRootJson = Json_create(szBuffer);
    bool isSuccess = false;
    do {
        CCAssert(pRootJson, "invalid json data");
        Json* pSuccessJson = Json_getItem(pRootJson, "success");
        CC_BREAK_IF(1 != pSuccessJson->valueint);
        
        Json* pResultJson = Json_getItem(pRootJson, "result");
        Json* pHItemJson = Json_getItem(pResultJson, "hero");
        Json* pHeroItemJson = pHItemJson->child;
        while (pHeroItemJson) {
            Json* pHeroItemTagJson = Json_getItem(pHeroItemJson, "tag");
            Json* pIdTagJson = Json_getItem(pHeroItemJson, "id");
            Json* pHeroItemLevelJson = Json_getItem(pHeroItemJson, "level");
            Json* pHeroItemExpJson = Json_getItem(pHeroItemJson, "exp");
            Json* pHeroItemWeaponJson = Json_getItem(pHeroItemJson, "weapon");
            Json* pHeroItemArmorJson = Json_getItem(pHeroItemJson, "armor");
            Json* pHeroItemLockJson = Json_getItem(pHeroItemJson, "lock");
            
            HeroItemOfPlayerFromServer hiopFS;
            hiopFS.uniId.assign(pHeroItemTagJson->valuestring);
            hiopFS.id = pIdTagJson->valueint;
            hiopFS.level = atoi(pHeroItemLevelJson->valuestring);
            hiopFS.exp = pHeroItemExpJson->valueint;
            hiopFS.weapon = pHeroItemWeaponJson->valueint;
            hiopFS.armor = pHeroItemArmorJson->valueint;
            hiopFS.isLock = pHeroItemLockJson->valueint == 1 ? true : false;
            hiopFS.isGoIntoBattle = false;
            
            // 增加背包武将记录
            pHeroManager->appendHeroOfPlayer(hiopFS);
            
            // 下一个
            pHeroItemJson = pHeroItemJson->next;
        } /*while*/
        isSuccess = true;
    } while(0);
    
    if (!isSuccess && pRootJson) {
        Json* pMsgJson = Json_getItem(pRootJson, "msg");
        CCLog("error msg = %s", pMsgJson->valuestring);
        MessageBox::getInstance()->messageTips(pMsgJson->valuestring);
    }
    if (pRootJson) {
        Json_dispose(pRootJson);
        pRootJson = NULL;
    }
    return isSuccess;
}

/*
 *  处理服务器回复的武将出征、战斗相关数据
 */
static bool processHeroBattle(const char *szBuffer) {
    CCAssert(szBuffer, "invalid buffer");
    
    CCLog("武将出征 content = %s", szBuffer);
    
    Battle* pBattle = Battle::getInstance();
    HeroManager* pHeroManager = HeroManager::getInstance();
    SkillManager* pSkillManager = SkillManager::getInstance();
    DropItemManager* pDropItemManager = DropItemManager::getInstance();
    GameState* pGameState = GameState::getInstance();
    GamePlayer* pGamePlayer = GamePlayer::getInstance();
    
    bool isSuccess = false;
    Json* pRootJson = Json_create(szBuffer);
    do {
        CCAssert(pRootJson, "invalid json data");
        Json* pSuccessJson = Json_getItem(pRootJson, "success");
        CC_BREAK_IF(1 != pSuccessJson->valueint);
        
        // 先清除所有掉落物品
        pDropItemManager->removeAllDropItem();
        
        Json* pResultJson = Json_getItem(pRootJson, "result");
        Json* pFightJson = Json_getItem(pResultJson, "fight");
        
        // 战斗是否胜利
        Json* pWinJson = Json_getItem(pFightJson, "win");
        pBattle->setWin(pWinJson->valueint == 1);
        if (!pBattle->getWin()) {
            pGameState->setGetPrisonHero(0);
        }
        
        /* 经验获得 */
        Json* pExpJson = Json_getItem(pResultJson, "exp");
        if (pExpJson) {
            pGameState->setCopyForExp(atoi(pExpJson->valuestring));
        } else {
            pGameState->setCopyForExp(0);
        }
        Json* pMoneyJson = Json_getItem(pResultJson, "money");
        if (pMoneyJson) {
            pGameState->setCopyForMoney(atoi(pMoneyJson->valuestring));
            /* 获得的铜币 */
            pGamePlayer->setMoney(pGamePlayer->getMoney() + atoi(pMoneyJson->valuestring));
        } else {
            pGameState->setCopyForMoney(0);
        }
        Json* pEnJson = Json_getItem(pResultJson, "energy");
        if (pEnJson) {
            pGamePlayer->setEnergy(pEnJson->valueint);
        }
        Json* pEnTimeJson = Json_getItem(pResultJson, "energy_time");
        if (pEnTimeJson) {
            pGamePlayer->setEnergyTime(pEnTimeJson->valueint);
        }
        Json* pDropJson = Json_getItem(pResultJson, "drop");
        if (pDropJson) {
            /* 掉落武将 */
            Json* pDropHerosJson = Json_getItem(pDropJson, "hero");
            if (pDropHerosJson) {
                Json* pDropHeroJson = pDropHerosJson->child;
                while (pDropHeroJson) {
                    Json* pHeroIdJson = Json_getItem(pDropHeroJson, "id");
                    Json* pHeroTagJson = Json_getItem(pDropHeroJson, "tag");
                    Json* pHeroLevelJson = Json_getItem(pDropHeroJson, "level");
                    
                    /* 添加到背包 */
                    DropItem* pDropItem = DropItem::create();
                    pDropItem->setType(DT_HERO);
                    /*武将id*/
                    pDropItem->setId(atoi(pHeroIdJson->valuestring));
                    
                    HeroItemOfPlayerFromServer hiopFS;
                    hiopFS.uniId.assign(pHeroTagJson->valuestring);
                    hiopFS.id = atoi(pHeroIdJson->valuestring);
                    hiopFS.level = atoi(pHeroLevelJson->valuestring);
                    hiopFS.exp = 0;
                    hiopFS.weapon = 0;
                    hiopFS.armor = 0;
                    hiopFS.isLock = false;
                    hiopFS.isGoIntoBattle = false;
                    
                    pDropItemManager->appendDropItem(pDropItem);
                    
                    // 将掉落武将添加入玩家背包
                    pHeroManager->appendHeroOfPlayer(hiopFS);
                    
                    pDropHeroJson = pDropHeroJson->next;
                } /*while*/
                
                /* 掉落监狱武将*/
                Json* pDropPrisonJson = Json_getItem(pDropJson, "prison");
                if (pDropPrisonJson) {
                    Json* pDropPrisonsJson = pDropPrisonJson->child;
                    while (pDropPrisonsJson) {
                        Json* pId = Json_getItem(pDropPrisonsJson, "id");
                        pGameState->setGetPrisonHero(atoi(pId->valuestring));
                        pDropPrisonsJson = pDropPrisonsJson->next;
                    } /*while*/
                } else/*没有掉落监狱武将*/ {
                    pGameState->setGetPrisonHero(0);
                }
            }
        }
        
        /*
         *  敌方武将(pvp)、怪物(pve)
         */
        Json* pEnemiesJson = Json_getItem(pFightJson, "enemies");
        CCAssert(pEnemiesJson, "invalid json format");
        
        pBattle->clearAllEnemyData();
        Json* pEnemyJson = pEnemiesJson->child;
        while (pEnemyJson) {
            Json* pEnemyTagJson = Json_getItem(pEnemyJson, "tag");
            if (pEnemyTagJson) {
                Json* pEnemyHpJson = Json_getItem(pEnemyJson, "hp");
                MosterItem* pMosterItem = pHeroManager->getMosterDataById(atoi(pEnemyTagJson->valuestring));
                
                MosterInBattleItem* pMosterInBattleItem = MosterInBattleItem::create();
                pMosterInBattleItem->setId(atoi(pEnemyTagJson->valuestring));
                pMosterInBattleItem->setHP(pEnemyHpJson->valueint);
                pMosterInBattleItem->setPosInBattle(atoi(pEnemyJson->name));
                pMosterInBattleItem->setMosterItem(pMosterItem);
                
                /* 是否有掉落物品 */
                Json* pDropItemJson = Json_getItem(pEnemyJson, "drop");
                if (pDropItemJson) /* 有掉落物品 */ {
                    Json* pDropItemTypeJson = Json_getItem(pDropItemJson, "type");      // 掉落物品类别
                    Json* pDropItemIdJson = Json_getItem(pDropItemJson, "id");          // 掉落物品id
                    Json* pDropItemNumberJson = Json_getItem(pDropItemJson, "num");     // 掉落物品数量
                    Json* pDropItemLevelJson = Json_getItem(pDropItemJson, "level");    // 掉落物品等级
                    
                    pMosterInBattleItem->setHasDrop(true);
                    pMosterInBattleItem->setDropItemType(pDropItemTypeJson->valuestring);
                    pMosterInBattleItem->setDropItemId(pDropItemIdJson->valuestring);
                    pMosterInBattleItem->setCountOfdropItem(NULL == pDropItemNumberJson ? 1 :
                                                            atoi(pDropItemNumberJson->valuestring));
                    pMosterInBattleItem->setDropItemLevel(pDropItemLevelJson == NULL ? 1 : atoi(pDropItemLevelJson->valuestring));
                    
                }
                
                // 添加上阵怪物
                pBattle->appendInBattleEnemyMoster(pMosterInBattleItem);
            }
            
            // 下一个敌人
            pEnemyJson= pEnemyJson->next;
        } /*while*/
        
        /*
         *  战斗回合
         */
        Json* pRoundsJson = Json_getItem(pFightJson, "rounds");
        CCAssert(pRoundsJson, "invalid json format");
        unsigned int roundIndex = 0;
        Json* pRoundJson = pRoundsJson->child;
        while (pRoundJson) {
            unsigned int nSequence = 0;
            Json* pAttackDataItemJson = pRoundJson->child;
            while (pAttackDataItemJson) {
                Json* pAttackerJson = Json_getItemAt(pAttackDataItemJson, 0);
                Json* pAttackerNowPosJson = Json_getItemAt(pAttackDataItemJson, 1);
                Json* pAttackerTagJson = Json_getItemAt(pAttackDataItemJson, 2);
                Json* pCompletePosJson = Json_getItemAt(pAttackDataItemJson, 3);
                Json* pSkillPosJson = Json_getItemAt(pAttackDataItemJson, 4);
                Json* pSkillIdJson = Json_getItemAt(pAttackDataItemJson, 5);
                
                AttackDataItem* pAttackDataItem = AttackDataItem::create();
                pAttackDataItem->setRound(roundIndex);
                pAttackDataItem->setSequence(nSequence);
                pAttackDataItem->setAttacker(atoi(pAttackerJson->valuestring));
                pAttackDataItem->setAttackerNowPos(pAttackerNowPosJson->valueint);
                pAttackDataItem->setAttackerTag(pAttackerTagJson->valuestring);
                pAttackDataItem->setCompletePos(pCompletePosJson->valueint);
                pAttackDataItem->setSkillPos(pSkillPosJson->valueint);
                /*
                 *  技能数据
                 */
                SkillItem* pSkillItem = pSkillManager->getSkillItem(atoi(pSkillIdJson->valuestring));
                pAttackDataItem->setSkillItem(pSkillItem);
                
                /*
                 * 攻击伤害数据
                 */
                Json* pHarmerAboutJson = Json_getItemAt(pAttackDataItemJson, 6);
                Json* pHarmerItemJson = pHarmerAboutJson->child;
                char szAttackDamageData[128];
                while (pHarmerItemJson) {
                    AttackDamageDataItem* pAttackDamageDataItem = AttackDamageDataItem::create();
                    
                    strcpy(szAttackDamageData, pHarmerItemJson->valuestring);
                    char* pResult = strtok(szAttackDamageData, ",");    // 被伤害方 0:我方 1:敌方
                    pAttackDamageDataItem->setHarmer(atoi(pResult));
                    pResult = strtok(NULL, ",");                        // 被伤害武将(pvp)、怪物(pve) 当时所在位置
                    pAttackDamageDataItem->setPosIndexInBattle(atoi(pResult));
                    pResult = strtok(NULL, ",");                        // 被伤害武将(pvp)、怪物(pve) tag
                    pAttackDamageDataItem->setHarmerID(pResult);
                    pResult = strtok(NULL, ",");                        // 伤害值，0 表示 miss，负数表示加血
                    pAttackDamageDataItem->setDamageValue(atoi(pResult));
                    pResult = strtok(NULL, ",");                        // 是否爆击
                    pAttackDamageDataItem->setCrit(strcmp(pResult, "1") == 0);
                    pResult = strtok(NULL, ",");                        // 被伤害武将剩余生命值
                    pAttackDamageDataItem->setRemainHp(atoi(pResult));
                    
                    /*
                     *  添加攻击伤害数据
                     */
                    pAttackDataItem->appendAttackDamageData(pAttackDamageDataItem);
                    
                    // 下一个攻击伤害数据
                    pHarmerItemJson = pHarmerItemJson->next;
                } /*while*/
                
                /*
                 *  添加攻击数据
                 */
                pBattle->appendAttackDataItem(pAttackDataItem);
                
                nSequence++;
                
                // 下一个攻击数据
                pAttackDataItemJson = pAttackDataItemJson->next;
            } /*while*/
            
            roundIndex++;
            pRoundJson = pRoundJson->next;
        } /*while*/
        
        isSuccess = true;
    } while (0);
    
    if (!isSuccess && pRootJson) {
        Json* pMsgJson = Json_getItem(pRootJson, "msg");
        CCLog("error msg = %s", pMsgJson->valuestring);
        MessageBox::getInstance()->messageTips(pMsgJson->valuestring);
    }
    if (pRootJson) {
        Json_dispose(pRootJson);
        pRootJson = NULL;
    }
    
    return isSuccess;
}

/*
 *  处理服务器回复的铜雀台武将出征、战斗相关数据
 */
static bool processTqtBattle(const char *szBuffer) {
    CCAssert(szBuffer, "invalid buffer");
    
    CCLog("武将出征铜雀台 content = %s", szBuffer);
    
    unsigned nFightType = 0;        // 0 npc 1 user
    Battle* pBattle = Battle::getInstance();
    HeroManager* pHeroManager = HeroManager::getInstance();
    SkillManager* pSkillManager = SkillManager::getInstance();
    DropItemManager* pDropItemManager = DropItemManager::getInstance();
    GamePlayer* pGamePlayer = GamePlayer::getInstance();
    GameState* pGameState = GameState::getInstance();
    
    // 先清除所有掉落物品
    pDropItemManager->removeAllDropItem();
    
    bool isSuccess = false;
    Json* pRootJson = Json_create(szBuffer);
    CCAssert(pRootJson, "invalid battle data");
    do {
        CCAssert(pRootJson, "invalid json data");
        Json* pSuccessJson = Json_getItem(pRootJson, "success");
        CC_BREAK_IF(1 != pSuccessJson->valueint);
        
        Json* pResultJson = Json_getItem(pRootJson, "result");
        
        // 战斗类型
        Json* pFightType = Json_getItem(pResultJson, "fight_type");
        CCAssert(pFightType, "invalid json format");
        if (!strcmp(pFightType->valuestring, "user")) {
            nFightType = 1;
        } else {
            nFightType = 0;
        }
        
        //
        Json* pTargetNameJson = Json_getItem(pResultJson, "target_name");
        if (pTargetNameJson) {
            pBattle->setTargetName(pTargetNameJson->valuestring);
        }
        
        // 获取的铜钱数
        Json* pAddMoneyJson = Json_getItem(pResultJson, "add_money");
        if (pAddMoneyJson) {
            pGamePlayer->setMoney(pGamePlayer->getMoney() + pAddMoneyJson->valueint);
            pBattle->setTqtAddMoney(pAddMoneyJson->valueint);
        }
        
        // 获取的铜钱数
        Json* pRewMoney = Json_getItem(pResultJson, "day_reward_money");
        if (pRewMoney) {
            pGamePlayer->setMoney(pGamePlayer->getMoney() + pRewMoney->valueint);
            pGamePlayer->setRewardMoney(pRewMoney->valueint);
        }
        
        // 战斗报告
        Json* pBattleReportJson = Json_getItem(pResultJson, "report");
        if (pBattleReportJson) {
            pBattle->clearAllBattleReport();
            Json* pBattleReportJsonChild = pBattleReportJson->child;
            while (pBattleReportJsonChild) {
                Json* pTp = Json_getItem(pBattleReportJsonChild, "tp");
                Json* pName = Json_getItem(pBattleReportJsonChild, "nm");
                Json* pRank = Json_getItem(pBattleReportJsonChild, "rk");
                Json* pUd = Json_getItem(pBattleReportJsonChild, "ud");
                Json* pMoney = Json_getItem(pBattleReportJsonChild, "rw");
                Json* pTm = Json_getItem(pBattleReportJsonChild, "tm");
                
                BattleReportItem* pBattleReportItem = BattleReportItem::create();
                if (!strcmp(pTp->valuestring, "to")) {
                    pBattleReportItem->setTp(1);
                } else {
                    pBattleReportItem->setTp(2);
                }
                pBattleReportItem->setName(pName->valuestring);
                pBattleReportItem->setRank(pRank->valueint);
                pBattleReportItem->setUd(pUd->valueint);
                pBattleReportItem->setMoney(pMoney->valueint);
                pBattleReportItem->setTm(pTm->valueint);
                
                pBattle->appendBattleReport(pBattleReportItem);
                
                pBattleReportJsonChild = pBattleReportJsonChild->next;
            } /*while*/
        }
        
        Json* pMyRankJson = Json_getItem(pResultJson, "my_rank");
        if (pMyRankJson) /*最新排名*/ {
            pGamePlayer->setRank(pMyRankJson->valueint);
        }
        
        Json* pRemainTimes = Json_getItem(pResultJson, "times");
        if (pRemainTimes) /*剩余战斗次数*/{
            pBattle->setRemainBattleTimes(pRemainTimes->valueint);
        }
        
        Json* pCdJson= Json_getItem(pResultJson, "cd");
        if (pCdJson) /*战斗冷却时间*/{
            pGameState->setCd(pCdJson->valueint);
        }
        
        Json* pNewEnemyJson = Json_getItem(pResultJson, "new_enemies");
        if (pNewEnemyJson) /*本轮战斗结束后铜雀台新的pk对象*/ {
            Json* pNewEnemyJsonChild = pNewEnemyJson->child;
            
            unsigned int index = 0;
            pHeroManager->removeAllTqHeadItems();
            while (pNewEnemyJsonChild) {
                Json* pTypeJson = Json_getItem(pNewEnemyJsonChild, "type");
                Json* pNameJson = Json_getItem(pNewEnemyJsonChild, "name");
                Json* pHeadJson = Json_getItem(pNewEnemyJsonChild, "head");
                Json* pRankJson = Json_getItem(pNewEnemyJsonChild, "rank");
                
                TqHeadItem* pTqHeadItem = TqHeadItem::create();
                pTqHeadItem->setPosition(++index);
                pTqHeadItem->setId(pHeadJson->valueint);
                if (!strcmp(pTypeJson->valuestring, "npc")) {
                    pTqHeadItem->setType(2);
                } else {
                    pTqHeadItem->setType(1);
                }
                pTqHeadItem->setName(pNameJson->valuestring);
                pTqHeadItem->setRank(pRankJson->valueint);
                
                /* 添加铜雀台Item */
                pHeroManager->appendTqHeadItem(pTqHeadItem);
                
                pNewEnemyJsonChild = pNewEnemyJsonChild->next;
            } /*while*/
        }
        
        /*
         *  战斗相关数据
         */
        Json* pFightJson = Json_getItem(pResultJson, "fight");
        // 战斗是否胜利
        Json* pWinJson = Json_getItem(pFightJson, "win");
        pBattle->setWin(1 == pWinJson->valueint);
        
        /*
         *  敌方武将(pvp)、怪物(pve) //id为字符串
         */
        Json* pEnemiesJson = Json_getItem(pFightJson, "enemies");
        
        pBattle->clearAllEnemyData();
        Json* pEnemyJson = pEnemiesJson->child;
        unsigned int enemyIndex = 0;
        while (pEnemyJson) {
            Json* pEnemyIdJson = Json_getItem(pEnemyJson, "id");
            if (pEnemyIdJson) {
                Json* pEnemyTagJson = Json_getItem(pEnemyJson, "tag");
                Json* pEnemyHpJson = Json_getItem(pEnemyJson, "hp");
                
                InBattleItem* pInBattleItem = NULL;
                if (nFightType) /**/{
                    HeroItem* pHeroItem = pHeroManager->getHeroItem(pEnemyIdJson->valueint);
                    CCAssert(pHeroItem, "invalid heroItem id");
                    
                    HeroOfPlayerItem* pHeroOfPlayerItem = HeroOfPlayerItem::create();
                    pHeroOfPlayerItem->setUniId(pEnemyTagJson->valuestring);
                    pHeroOfPlayerItem->setPvpHp(pEnemyHpJson->valueint);
                    
                    pHeroOfPlayerItem->setHeroItem(pHeroItem);
                    pHeroOfPlayerItem->setPosInBattle(atoi(pEnemyJson->name));
                    
                    pInBattleItem = pHeroOfPlayerItem;
                } else /*怪物*/{
                    MosterItem* pMosterItem = pHeroManager->getMosterDataById(atoi(pEnemyIdJson->valuestring));
                    CCAssert(pMosterItem, "invalid moster id");
                    
                    MosterInBattleItem* pMosterInBattleItem = MosterInBattleItem::create();
                    pMosterInBattleItem->setId(atoi(pEnemyTagJson->valuestring));
                    pMosterInBattleItem->setHP(pEnemyHpJson->valueint);
                    pMosterInBattleItem->setPosInBattle(atoi(pEnemyJson->name));
                    pMosterInBattleItem->setMosterItem(pMosterItem);
                    
                    pInBattleItem = pMosterInBattleItem;
                }
                
                if (0 == enemyIndex++) /* 是敌方队长 */ {
                    // 保存敌方队长
                    pBattle->setPvpEnemyCaptain(pInBattleItem);
                }
                
                // 添加敌方战队
                pBattle->appendInBattleEnemyPvp(pInBattleItem);
            }
            
            // 下一个敌人
            pEnemyJson= pEnemyJson->next;
        } /*while*/
        
        /*
         *  战斗回合
         */
        Json* pRoundsJson = Json_getItem(pFightJson, "rounds");
        CCAssert(pRoundsJson, "invalid json format");
        unsigned int roundIndex = 0;
        Json* pRoundJson = pRoundsJson->child;
        while (pRoundJson) {
            unsigned int nSequence = 0;
            Json* pAttackDataItemJson = pRoundJson->child;
            while (pAttackDataItemJson) {
                Json* pAttackerJson = Json_getItemAt(pAttackDataItemJson, 0);
                Json* pAttackerNowPosJson = Json_getItemAt(pAttackDataItemJson, 1);
                Json* pAttackerTagJson = Json_getItemAt(pAttackDataItemJson, 2);
                Json* pCompletePosJson = Json_getItemAt(pAttackDataItemJson, 3);
                Json* pSkillPosJson = Json_getItemAt(pAttackDataItemJson, 4);
                Json* pSkillIdJson = Json_getItemAt(pAttackDataItemJson, 5);
                
                AttackDataItem* pAttackDataItem = AttackDataItem::create();
                pAttackDataItem->setRound(roundIndex);
                pAttackDataItem->setSequence(nSequence);
                pAttackDataItem->setAttacker(atoi(pAttackerJson->valuestring));
                pAttackDataItem->setAttackerNowPos(pAttackerNowPosJson->valueint);
                pAttackDataItem->setAttackerTag(pAttackerTagJson->valuestring);
                pAttackDataItem->setCompletePos(pCompletePosJson->valueint);
                pAttackDataItem->setSkillPos(pSkillPosJson->valueint);
                /*
                 *  技能数据
                 */
                pAttackDataItem->setSkillItem(pSkillManager->getSkillItem(atoi(pSkillIdJson->valuestring)));
                
                /*
                 * 攻击伤害数据
                 */
                Json* pHarmerAboutJson = Json_getItemAt(pAttackDataItemJson, 6);
                Json* pHarmerItemJson = pHarmerAboutJson->child;
                char szAttackDamageData[128];
                while (pHarmerItemJson) {
                    AttackDamageDataItem* pAttackDamageDataItem = AttackDamageDataItem::create();
                    
                    strcpy(szAttackDamageData, pHarmerItemJson->valuestring);
                    char* pResult = strtok(szAttackDamageData, ",");    // 被伤害方 0:我方 1:敌方
                    pAttackDamageDataItem->setHarmer(atoi(pResult));
                    pResult = strtok(NULL, ",");                        // 被伤害武将(pvp)、怪物(pve) 当时所在位置
                    pAttackDamageDataItem->setPosIndexInBattle(atoi(pResult));
                    pResult = strtok(NULL, ",");                        // 被伤害武将(pvp)、怪物(pve) tag
                    pAttackDamageDataItem->setHarmerID(pResult);
                    pResult = strtok(NULL, ",");                        // 伤害值，0 表示 miss，负数表示加血
                    pAttackDamageDataItem->setDamageValue(atoi(pResult));
                    pResult = strtok(NULL, ",");                        // 是否爆击
                    pAttackDamageDataItem->setCrit(strcmp(pResult, "1") == 0);
                    pResult = strtok(NULL, ",");                        // 被伤害武将剩余生命值
                    pAttackDamageDataItem->setRemainHp(atoi(pResult));
                    
                    /*
                     *  添加攻击伤害数据
                     */
                    pAttackDataItem->appendAttackDamageData(pAttackDamageDataItem);
                    
                    // 下一个攻击伤害数据
                    pHarmerItemJson = pHarmerItemJson->next;
                } /*while*/
                
                /*
                 *  添加攻击数据
                 */
                pBattle->appendAttackDataItem(pAttackDataItem);
                
                nSequence++;
                
                // 下一个攻击数据
                pAttackDataItemJson = pAttackDataItemJson->next;
            } /*while*/
            
            roundIndex++;
            pRoundJson = pRoundJson->next;
        } /*while*/
        
        isSuccess = true;
    } while (0);
    
    if (!isSuccess && pRootJson) {
        Json* pMsgJson = Json_getItem(pRootJson, "msg");
        CCLog("error msg = %s", pMsgJson->valuestring);
        MessageBox::getInstance()->messageTips(pMsgJson->valuestring);
    }
    if (pRootJson) {
        Json_dispose(pRootJson);
        pRootJson = NULL;
    }
    
    return isSuccess;
}

/*
 *  处理服务器回复的国战武将出征、战斗相关数据
 */
static bool processCWarBattle(const char *szBuffer) {
    CCAssert(szBuffer, "invalid buffer");
    
    CCLog("国战战斗数据 = %s", szBuffer);
    
    Battle* pBattle = Battle::getInstance();
    HeroManager* pHeroManager = HeroManager::getInstance();
    SkillManager* pSkillManager = SkillManager::getInstance();
    DropItemManager* pDropItemManager = DropItemManager::getInstance();
    GamePlayer* pGamePlayer = GamePlayer::getInstance();
    
    // 先清除所有掉落物品
    pDropItemManager->removeAllDropItem();
    pBattle->setInList(false);
    
    bool isSuccess = false;
    Json* pRootJson = Json_create(szBuffer);
    CCAssert(pRootJson, "invalid battle data");
    do {
        CCAssert(pRootJson, "invalid json data");
        Json* pSuccessJson = Json_getItem(pRootJson, "success");
        CC_BREAK_IF(1 != pSuccessJson->valueint);
        
        Json* pResultJson = Json_getItem(pRootJson, "result");
        Json* pTypeJson = Json_getItem(pResultJson, "type");

        // 不是战斗
        if (strcmp(pTypeJson->valuestring, "fight") != 0) {
            pBattle->setInList(true);
            isSuccess = true;
            break;
        }
        
        // 获取的铜钱数
        Json* pAddMoneyJson = Json_getItem(pResultJson, "add_money");
        if (pAddMoneyJson) {
            pGamePlayer->setMoney(pGamePlayer->getMoney() + pAddMoneyJson->valueint);
            pBattle->setCwarAddMoney(pBattle->getCwarAddMoney() + pAddMoneyJson->valueint);
        }
        
        /*
         *  战斗相关数据
         */
        Json* pFightJson = Json_getItem(pResultJson, "fight");
        // 战斗是否胜利
        Json* pWinJson = Json_getItem(pFightJson, "win");
        pBattle->setWin(1 == pWinJson->valueint);
        
        /*
         *  敌方武将(pvp)、怪物(pve) //id为字符串
         */
        Json* pEnemiesJson = Json_getItem(pFightJson, "enemies");
        pBattle->clearAllEnemyData();
        Json* pEnemyJson = pEnemiesJson->child;
        unsigned int enemyIndex = 0;
        while (pEnemyJson) {
            Json* pEnemyIdJson = Json_getItem(pEnemyJson, "id");
            if (pEnemyIdJson) {
                Json* pEnemyTagJson = Json_getItem(pEnemyJson, "tag");
                Json* pEnemyHpJson = Json_getItem(pEnemyJson, "hp");
                    
                HeroItem* pHeroItem = pHeroManager->getHeroItem(pEnemyIdJson->valueint);
                CCAssert(pHeroItem, "invalid heroItem id");
                        
                HeroOfPlayerItem* pHeroOfPlayerItem = HeroOfPlayerItem::create();
                pHeroOfPlayerItem->setUniId(pEnemyTagJson->valuestring);
                pHeroOfPlayerItem->setPvpHp(pEnemyHpJson->valueint);
                pHeroOfPlayerItem->setHeroItem(pHeroItem);
                pHeroOfPlayerItem->setPosInBattle(atoi(pEnemyJson->name));
                        
                if (0 == enemyIndex++) /* 是敌方队长 */ {
                    // 保存敌方队长
                    pBattle->setPvpEnemyCaptain((InBattleItem*)pHeroOfPlayerItem);
                }
                // 添加敌方战队
                pBattle->appendInBattleEnemyPvp((InBattleItem*)pHeroOfPlayerItem);
            }
            
            // 下一个敌人
            pEnemyJson= pEnemyJson->next;
        } /*while*/
        
        /*
         *  战斗回合
         */
        unsigned int roundIndex = 0;
        Json* pRoundsJson = Json_getItem(pFightJson, "rounds");
        Json* pRoundJson = pRoundsJson->child;
        while (pRoundJson) {
            unsigned int nSequence = 0;
            Json* pAttackDataItemJson = pRoundJson->child;
            while (pAttackDataItemJson) {
                Json* pAttackerJson = Json_getItemAt(pAttackDataItemJson, 0);
                Json* pAttackerNowPosJson = Json_getItemAt(pAttackDataItemJson, 1);
                Json* pAttackerTagJson = Json_getItemAt(pAttackDataItemJson, 2);
                Json* pCompletePosJson = Json_getItemAt(pAttackDataItemJson, 3);
                Json* pSkillPosJson = Json_getItemAt(pAttackDataItemJson, 4);
                Json* pSkillIdJson = Json_getItemAt(pAttackDataItemJson, 5);
                
                AttackDataItem* pAttackDataItem = AttackDataItem::create();
                pAttackDataItem->setRound(roundIndex);
                pAttackDataItem->setSequence(nSequence);
                pAttackDataItem->setAttacker(atoi(pAttackerJson->valuestring));
                pAttackDataItem->setAttackerNowPos(pAttackerNowPosJson->valueint);
                pAttackDataItem->setAttackerTag(pAttackerTagJson->valuestring);
                pAttackDataItem->setCompletePos(pCompletePosJson->valueint);
                pAttackDataItem->setSkillPos(pSkillPosJson->valueint);
                /*
                 *  技能数据
                 */
                pAttackDataItem->setSkillItem(pSkillManager->getSkillItem(atoi(pSkillIdJson->valuestring)));
                
                /*
                 * 攻击伤害数据
                 */
                Json* pHarmerAboutJson = Json_getItemAt(pAttackDataItemJson, 6);
                Json* pHarmerItemJson = pHarmerAboutJson->child;
                char szAttackDamageData[128];
                while (pHarmerItemJson) {
                    AttackDamageDataItem* pAttackDamageDataItem = AttackDamageDataItem::create();
                    
                    strcpy(szAttackDamageData, pHarmerItemJson->valuestring);
                    char* pResult = strtok(szAttackDamageData, ",");    // 被伤害方 0:我方 1:敌方
                    pAttackDamageDataItem->setHarmer(atoi(pResult));
                    pResult = strtok(NULL, ",");                        // 被伤害武将(pvp)、怪物(pve) 当时所在位置
                    pAttackDamageDataItem->setPosIndexInBattle(atoi(pResult));
                    pResult = strtok(NULL, ",");                        // 被伤害武将(pvp)、怪物(pve) tag
                    pAttackDamageDataItem->setHarmerID(pResult);
                    pResult = strtok(NULL, ",");                        // 伤害值，0 表示 miss，负数表示加血
                    pAttackDamageDataItem->setDamageValue(atoi(pResult));
                    pResult = strtok(NULL, ",");                        // 是否爆击
                    pAttackDamageDataItem->setCrit(strcmp(pResult, "1") == 0);
                    pResult = strtok(NULL, ",");                        // 被伤害武将剩余生命值
                    pAttackDamageDataItem->setRemainHp(atoi(pResult));
                    
                    /*
                     *  添加攻击伤害数据
                     */
                    pAttackDataItem->appendAttackDamageData(pAttackDamageDataItem);
                    
                    // 下一个攻击伤害数据
                    pHarmerItemJson = pHarmerItemJson->next;
                } /*while*/
                
                /*
                 *  添加攻击数据
                 */
                pBattle->appendAttackDataItem(pAttackDataItem);
                
                nSequence++;
                
                // 下一个攻击数据
                pAttackDataItemJson = pAttackDataItemJson->next;
            } /*while*/
            
            roundIndex++;
            pRoundJson = pRoundJson->next;
        } /*while*/
        
        isSuccess = true;
    } while (0);
    
    if (!isSuccess && pRootJson) {
        Json* pMsgJson = Json_getItem(pRootJson, "msg");
        CCLog("error msg = %s", pMsgJson->valuestring);
        MessageBox::getInstance()->messageTips(pMsgJson->valuestring);
    }
    if (pRootJson) {
        Json_dispose(pRootJson);
        pRootJson = NULL;
    }
    
    return isSuccess;
}

/*
 *  从本地 json 文件加载武将数据
 */
void ProcessData::loadLocalDataFromJson() {
    HeroManager* pHeroManager = HeroManager::getInstance();
    CopyManager* pCopyManager = CopyManager::getInstance();
    CityManager* pCityManager = CityManager::getInstance();
    GamePlayer* pGamePlayer = GamePlayer::getInstance();
    
    // 加载本地玩家当前等级升级所需经验值数据
    pGamePlayer->loadExpForLevUp();
    
    // 清空所有武将数据
    pHeroManager->removeAllHeroData();
    // 清空所有副本数据
    pCopyManager->removeAllCopyData();
    
    /*
     *  本地武将基本数据
     */
    std::string path = CCFileUtils::sharedFileUtils()->fullPathForFilename(HEROS_DATA_JSON_FILE);
    unsigned long size;
    char *content = (char*)CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "r", &size);
    if (content && size > 0) {
        std::string text;
        text.assign(content, size);
        text += '\0';
        delete [] content;
        
        //
        Json* pRootJson = Json_create(text.c_str());
        if (pRootJson) {
            Json* pHeroJson = Json_getItem(pRootJson, "hero");
            if (pHeroJson) {
                Json* pHeroItemJson = pHeroJson->child;
                
                while (pHeroItemJson) {
                    HeroItem* pHeroItem = new HeroItem();
                    pHeroItem->init();
                    
                    // id
                    Json* heroId = Json_getItem(pHeroItemJson, "id");
                    if (heroId) {
                        pHeroItem->setId(heroId->valueint);
                    }
                    // 姓名
                    Json* heroName = Json_getItem(pHeroItemJson, "n");
                    if (heroName) {
                        pHeroItem->setName(heroName->valuestring);
                    }
                    // 阵营
                    Json* heroCamp = Json_getItem(pHeroItemJson, "g");
                    if (heroCamp) {
                        pHeroItem->setCamp((herosOfCamp)heroCamp->valueint);
                    }
                    // icon
                    Json* heroIcon = Json_getItem(pHeroItemJson, "i");
                    if (heroIcon) {
                        pHeroItem->setIcon(heroIcon->valuestring);
                    }
                    //hicon
                    Json* heroHIcon = Json_getItem(pHeroItemJson, "hi");
                    if (heroHIcon) {
                        pHeroItem->setHIcon(heroHIcon->valuestring);
                    }
                    // cicon
                    Json* heroBIcon = Json_getItem(pHeroItemJson, "ci");
                    if (heroBIcon) {
                        pHeroItem->setBIcon(heroBIcon->valuestring);
                    }
                    // 武将战斗头像
                    Json* heroFIcon = Json_getItem(pHeroItemJson, "fi");
                    pHeroItem->setFIcon(heroFIcon->valuestring);
                    // r稀有度
                    Json* heroRarity = Json_getItem(pHeroItemJson, "r");
                    if (heroRarity) {
                        pHeroItem->setRarity(heroRarity->valueint);
                    }
                    // p职业
                    Json* heroJob = Json_getItem(pHeroItemJson, "p");
                    if (heroJob) {
                        pHeroItem->setJob(heroJob->valueint);
                    }
                    // bh基础hp
                    Json* heroBHp = Json_getItem(pHeroItemJson, "bh");
                    if (heroBHp) {
                        pHeroItem->setBasicHp(heroBHp->valueint);
                    }
                    
                    // 基础属性，取基础物理攻击、基础法术攻击、基础治疗三个中最大的一个
                    float basicProperty = 0.0f;
                    // ba基础物理攻击
                    Json* heroBa = Json_getItem(pHeroItemJson, "ba");
                    if (heroBa) {
                        pHeroItem->setBa(heroBa->valuefloat);
                        basicProperty = heroBa->valuefloat;
                    }
                    // bp基础法术攻击
                    Json* heroBp = Json_getItem(pHeroItemJson, "bp");
                    if (heroBp) {
                        pHeroItem->setBp(heroBp->valuefloat);
                        basicProperty = basicProperty < heroBp->valuefloat ? heroBp->valuefloat : basicProperty;
                    }
                    // be基础治疗
                    Json* heroBe = Json_getItem(pHeroItemJson, "be");
                    if (heroBe) {
                        pHeroItem->setBe(heroBe->valuefloat);
                        basicProperty = basicProperty < heroBe->valuefloat ? heroBe->valuefloat : basicProperty;
                        pHeroItem->setBasicProperty(basicProperty);
                    }
                    // 设置基础属性
                    pHeroItem->setBasicProperty(basicProperty);
                    
                    // bf基础物理防御
                    Json* heroBf = Json_getItem(pHeroItemJson, "bf");
                    if (heroBf) {
                        pHeroItem->setBf(heroBf->valuefloat);
                    }
                    
                    // bs基础法术防御
                    Json* heroBs = Json_getItem(pHeroItemJson, "bs");
                    if (heroBs) {
                        pHeroItem->setBs(heroBs->valuefloat);
                    }
                    // bc基础暴击
                    Json* heroBc = Json_getItem(pHeroItemJson, "bc");
                    if (heroBc) {
                        pHeroItem->setBc(heroBc->valuefloat);
                    }
                    // bm基础闪避
                    Json* heroBm = Json_getItem(pHeroItemJson, "bm");
                    if (heroBm) {
                        pHeroItem->setBm(heroBm->valuefloat);
                    }
                    // bi基础命中
                    Json* heroBi = Json_getItem(pHeroItemJson, "bi");
                    if (heroBi) {
                        pHeroItem->setBi(heroBi->valuefloat);
                    }
                    
                    // ih成长HP
                    Json* heroGrowHp = Json_getItem(pHeroItemJson, "ih");
                    if (heroGrowHp) {
                        pHeroItem->setGrowthHp(heroGrowHp->valuefloat);
                    }
                    
                    float growProperty = 0.0f;
                    // ia成长物理攻击
                    Json* heroIa = Json_getItem(pHeroItemJson, "ia");
                    if (heroIa) {
                        pHeroItem->setIa(heroIa->valuefloat);
                        growProperty = heroIa->valuefloat;
                    }
                    // ip成长法术攻击
                    Json* heroIp = Json_getItem(pHeroItemJson, "ip");
                    if (heroIp) {
                        pHeroItem->setIp(heroIp->valuefloat);
                        growProperty = growProperty < heroIp->valuefloat ? heroIp->valuefloat : growProperty;
                    }
                    // ie成长治疗
                    Json* heroIe = Json_getItem(pHeroItemJson, "ie");
                    if (heroIe) {
                        pHeroItem->setIe(heroIe->valuefloat);
                        growProperty = growProperty < heroIe->valuefloat ? heroIe->valuefloat : growProperty;
                        pHeroItem->setGrowthProperty(growProperty);
                    }
                    // if成长武力防御
                    Json* heroIf = Json_getItem(pHeroItemJson, "if");
                    if (heroIf) {
                        pHeroItem->setIf(heroIf->valuefloat);
                    }
                    // is成长法术防御
                    Json* heroIs = Json_getItem(pHeroItemJson, "is");
                    if (heroIs) {
                        pHeroItem->setIs(heroIs->valuefloat);
                    }
                    // ns普通技能
                    Json* heroNs = Json_getItem(pHeroItemJson, "ns");
                    if (heroNs) {
                        pHeroItem->setNs(heroNs->valueint);
                    }
                    // as主动技能
                    Json* heroAs = Json_getItem(pHeroItemJson, "as");
                    if (heroAs) {
                        pHeroItem->setAs(heroAs->valueint);
                    }
                    // ps被动技能
                    Json* heroPs = Json_getItem(pHeroItemJson, "ps");
                    if (heroPs) {
                        pHeroItem->setPs(heroPs->valueint);
                    }
                    // ul进化所需等级
                    Json* heroUl = Json_getItem(pHeroItemJson, "ul");
                    if (heroUl) {
                        pHeroItem->setUl(heroUl->valueint);
                    }
                    // ui进化所需道具
                    Json* heroUi = Json_getItem(pHeroItemJson, "ui");
                    if (heroUi) {
                        pHeroItem->setUi(heroUi->valueint);
                    }
                    // ug升级所需金币
                    Json* heroUg = Json_getItem(pHeroItemJson, "ug");
                    if (heroUg) {
                        pHeroItem->setUg(heroUg->valueint);
                    }
                    // ut升级产物
                    Json* heroUt = Json_getItem(pHeroItemJson, "ut");
                    if (heroUt) {
                        pHeroItem->setUt(heroUt->valueint);
                    }
                    // sn合成所需碎片
                    Json* heroSn = Json_getItem(pHeroItemJson, "sn");
                    if (heroSn) {
                        pHeroItem->setSn(heroSn->valueint);
                    }
                    // sg碎片合成所需金币
                    Json* heroSg = Json_getItem(pHeroItemJson, "sg");
                    if (heroSg) {
                        pHeroItem->setSg(heroSg->valueint);
                    }
                    // jj简介
                    Json* heroJj = Json_getItem(pHeroItemJson, "jj");
                    if (heroJj) {
                        pHeroItem->setJj(heroJj->valuestring);
                    }
                    
                    // 保存
                    pHeroManager->appendHeroItem(pHeroItem);
                    pHeroItem->release();
                    
                    // 取下一个子项
                    pHeroItemJson = pHeroItemJson->next;
                } /*while*/
            }
            Json_dispose (pRootJson);
        }
    }
    
    /*
     * 加载武将扩展数据
     */
    do {
        path = CCFileUtils::sharedFileUtils()->fullPathForFilename(HERO_ADDITION_JSON_FILE);
        content = (char*)CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "r", &size);
        CC_BREAK_IF(NULL == content || 0 == size);
        
        std::string text;
        text.assign(content, size);
        text += '\0';
        delete [] content;
        
        Json* pRootJson = Json_create(text.c_str());
        CC_BREAK_IF(NULL == pRootJson);
        
        char pValue[32];
        unsigned int count = 0;
        Json* pHeroDig = Json_getItem(pRootJson, "hero_dig");
        Json* pHeroRarity = pHeroDig->child;
        while (pHeroRarity) {
            Json* pHeroLevel = pHeroRarity->child;
            while(pHeroLevel) {
                HeroAdditionItem* pHeroAdditionItem = new HeroAdditionItem();
                pHeroAdditionItem->init();
                
                strcpy(pValue, pHeroLevel->valuestring);
                char* presult = strtok(pValue, "|");
                pHeroAdditionItem->setExpForLevUp(atoi(presult));       // 武将升级所需经验
                presult = strtok(NULL, "|");
                pHeroAdditionItem->setExpByFood(atoi(presult));         // 被吞噬产生的经验
                presult = strtok(NULL, "|");
                pHeroAdditionItem->setGoldByFood(atoi(presult));        // 吞噬每张卡所需的金币
                presult = strtok(NULL, "|");
                pHeroAdditionItem->setGoldBySale(atoi(presult));        // 出售获得金币
                // 添加武将扩展属性
                pHeroManager->appenHeroAdditionItem(pHeroAdditionItem);
                pHeroAdditionItem->release();
                
                // 下一个等级
                pHeroLevel = pHeroLevel->next;
                count++;
            } /*while*/
            
            // 下一个稀有度
            pHeroRarity = pHeroRarity->next;
        } /*while */
        Json_dispose (pRootJson);
        
        // 检查数量
        CCAssert(count == MAX_HERO_RARITY * MAX_HERO_LEVEL, "invalid count");
    } while (0);
    
    /*
     * 武将上阵所带兵种以及数量
     */
    do {
        path = CCFileUtils::sharedFileUtils()->fullPathForFilename(HERO_FHP_JSON_FILE);
        content = (char*)CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "r", &size);
        CC_BREAK_IF(NULL == content || 0 == size);
        
        std::string text;
        text.assign(content, size);
        text += '\0';
        delete [] content;
        
        Json* pRootJson = Json_create(text.c_str());
        CCAssert(pRootJson, "invalid HERO_FHP_JSON_FILE");
        
        Json* pHeroFhp = Json_getItem(pRootJson, "hero_fhp");
        Json* pHeroRarity = pHeroFhp->child;
        while (pHeroRarity) {
            Json* pItemB = Json_getItem(pHeroRarity, "b");
            Json* pItemQ = Json_getItem(pHeroRarity, "q");
            Json* pItemG = Json_getItem(pHeroRarity, "g");
            Json* pItemM = Json_getItem(pHeroRarity, "m");
            Json* pItemC = Json_getItem(pHeroRarity, "c");
            Json* pItemD = Json_getItem(pHeroRarity, "d");
            Json* pItemY = Json_getItem(pHeroRarity, "y");
            
            HeroFhpItem* pHeroFhpItem = new HeroFhpItem();
            pHeroFhpItem->init();
            
            pHeroFhpItem->setRarity(atoi(pHeroRarity->name));
            pHeroFhpItem->setB(atoi(pItemB->valuestring));
            pHeroFhpItem->setQ(atoi(pItemQ->valuestring));
            pHeroFhpItem->setG(atoi(pItemG->valuestring));
            pHeroFhpItem->setM(atoi(pItemM->valuestring));
            pHeroFhpItem->setC(atoi(pItemC->valuestring));
            pHeroFhpItem->setD(atoi(pItemD->valuestring));
            pHeroFhpItem->setY(atoi(pItemY->valuestring));
            
            // 添加
            pHeroManager->appenHeroFhpItem(pHeroFhpItem);
            pHeroFhpItem->release();
            
            //
            pHeroRarity = pHeroRarity->next;
        }
        Json_dispose (pRootJson);
    } while (0);
    
    /*
     * 副本关卡数据
     */
    do {
        path = CCFileUtils::sharedFileUtils()->fullPathForFilename(COPY_JSON_FILE);
        content = (char*)CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "r", &size);
        CC_BREAK_IF(NULL == content || 0 == size);
        
        std::string text;
        text.assign(content, size);
        text += '\0';
        delete [] content;
        
        Json* pRootJson = Json_create(text.c_str());
        CC_BREAK_IF(NULL == pRootJson);
        
        Json* pCopy = Json_getItem(pRootJson, "copy");
        Json* pLevel = Json_getItem(pRootJson, "pve_level");
        
        char szLevelId[512];
        Json* pCopyItem = pCopy->child;
        while (pCopyItem) {
            Json* pCopyId = Json_getItem(pCopyItem, "id");
            Json* pCopyName = Json_getItem(pCopyItem, "cn");
            Json* pCopyTy = Json_getItem(pCopyItem, "ty");
            Json* pCopyNc = Json_getItem(pCopyItem, "nc");
            Json* pCopySl = Json_getItem(pCopyItem, "sl");
            Json* pCopyEl = Json_getItem(pCopyItem, "el");
            Json* pCopyLe = Json_getItem(pCopyItem, "le");
            Json* pCopyDs = Json_getItem(pCopyItem, "ds");
            
            CopyDataItem* pCopyDataItem = new CopyDataItem;
            pCopyDataItem->init();
            
            pCopyDataItem->setId(atoi(pCopyId->valuestring));
            pCopyDataItem->setName(pCopyName->valuestring);
            pCopyDataItem->setTy(atoi(pCopyTy->valuestring));
            pCopyDataItem->setNc(atoi(pCopyNc->valuestring));
            pCopyDataItem->setSl(atoi(pCopySl->valuestring));
            pCopyDataItem->setEl(atoi(pCopyEl->valuestring));
            pCopyDataItem->setDs(pCopyDs->valuestring);
            
            /*
             *  添加副本数据项
             */
            pCopyManager->appendCopyData(pCopyDataItem);
            pCopyDataItem->release();
            
            strcpy(szLevelId, pCopyLe->valuestring);
            char* levelId = strtok(szLevelId, "|");
            while (levelId) {
                Json* pLevelItem = Json_getItem(pLevel, levelId);
                CCAssert(pLevelItem, "invalid level data");
                
                Json* pLevelName = Json_getItem(pLevelItem, "na");
                Json* pLevelMi = Json_getItem(pLevelItem, "mi");
                Json* pLevelNi = Json_getItem(pLevelItem, "ni");
                Json* pLevelEn = Json_getItem(pLevelItem, "en");
                Json* pLevelGo = Json_getItem(pLevelItem, "go");
                Json* pLevelEx = Json_getItem(pLevelItem, "ex");
                
                LevelDataItem* pLevelDataItem = new LevelDataItem;
                pLevelDataItem->init();
                
                pLevelDataItem->setId(atoi(levelId));
                pLevelDataItem->setName(pLevelName->valuestring);
                pLevelDataItem->setMi(atoi(pLevelMi->valuestring));
                pLevelDataItem->setNi(atoi(pLevelNi->valuestring));
                pLevelDataItem->setEn(atoi(pLevelEn->valuestring));
                pLevelDataItem->setGo(atoi(pLevelGo->valuestring));
                pLevelDataItem->setEx(atoi(pLevelEx->valuestring));
                
                /*
                 *  添加该副本所属关卡数据项
                 */
                pCopyManager->appendLevelData(atoi(pCopyId->valuestring), pLevelDataItem);
                pLevelDataItem->release();
                
                // 下一个关卡
                levelId = strtok(NULL, "|");
            } /*while*/
            
            pCopyItem = pCopyItem->next;
        } /*while*/
        
        Json_dispose (pRootJson);
    } while (0);
    
    /*
     * 怪物数据
     */
    do {
        path = CCFileUtils::sharedFileUtils()->fullPathForFilename(MOSTER_JSON_FILE);
        content = (char*)CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "r", &size);
        CC_BREAK_IF(NULL == content || 0 == size);
        
        std::string text;
        text.assign(content, size);
        text += '\0';
        delete [] content;
        
        Json* pRootJson = Json_create(text.c_str());
        CCAssert(pRootJson, "invalid monster item");
        
        Json* pMoster = Json_getItem(pRootJson, "moster");
        Json* pMosterJsonItem = pMoster->child;
        while (pMosterJsonItem) {
            Json* pId = Json_getItem(pMosterJsonItem, "id");
            Json* pName = Json_getItem(pMosterJsonItem, "n");
            Json* pRarity = Json_getItem(pMosterJsonItem, "r");
            Json* pJob = Json_getItem(pMosterJsonItem, "p");
            Json* pFi = Json_getItem(pMosterJsonItem, "fi");
            Json* pLi = Json_getItem(pMosterJsonItem, "li");
            
            MosterItem* pMosterItem = new MosterItem();
            pMosterItem->init();
            
            pMosterItem->setId(pId->valueint);
            pMosterItem->setName(pName->valuestring);
            pMosterItem->setRarity(pRarity->valueint);
            pMosterItem->setJob(pJob->valueint);
            pMosterItem->setFi(pFi->valuestring);
            pMosterItem->setLi(pLi->valuestring);
            
            /*
             *  添加怪物数据项
             */
            pHeroManager->appendMosterItem(pMosterItem);
            pMosterItem->release();
            
            pMosterJsonItem = pMosterJsonItem->next;
        } /*while*/
        Json_dispose (pRootJson);
    } while (0);
    
    /*
     * 技能数据
     */
    SkillManager* pSkillManager = SkillManager::getInstance();
    do {
        path = CCFileUtils::sharedFileUtils()->fullPathForFilename(SKILL_JSON_FILE);
        content = (char*)CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "r", &size);
        CC_BREAK_IF(NULL == content || 0 == size);
        
        std::string text;
        text.assign(content, size);
        text += '\0';
        delete [] content;
        
        Json* pRootJson = Json_create(text.c_str());
        CCAssert(pRootJson, "invalid skill data file");
        
        Json* pSkillJson = Json_getItem(pRootJson, "skill");
        Json* pSkillItemJson = pSkillJson->child;
        while (pSkillItemJson) {
            Json* pId = Json_getItem(pSkillItemJson, "id");
            Json* pName = Json_getItem(pSkillItemJson, "na");
            Json* pDesc = Json_getItem(pSkillItemJson, "de");
            Json* pC = Json_getItem(pSkillItemJson, "c");
            Json* pCm = Json_getItem(pSkillItemJson, "cm");
            Json* pCz = Json_getItem(pSkillItemJson, "cz");
            Json* pCs = Json_getItem(pSkillItemJson, "cs");
            Json* pBm = Json_getItem(pSkillItemJson, "bm");
            Json* pBz = Json_getItem(pSkillItemJson, "bz");
            Json* pBs = Json_getItem(pSkillItemJson, "bs");
            Json* pIs = Json_getItem(pSkillItemJson, "is");
            Json* pIsco = Json_getItem(pSkillItemJson, "isco");
            Json* pBp = Json_getItem(pSkillItemJson, "bp");
            Json* pCos = Json_getItem(pSkillItemJson, "cos");
            Json* pOs = Json_getItem(pSkillItemJson, "os");
            Json* pTm = Json_getItem(pSkillItemJson, "tm");
            Json* pTz = Json_getItem(pSkillItemJson, "tz");
            Json* pTs = Json_getItem(pSkillItemJson, "ts");
            Json* pTy = Json_getItem(pSkillItemJson, "ty");
            Json* pLv = Json_getItem(pSkillItemJson, "lv");
            Json* pAt = Json_getItem(pSkillItemJson, "at");
            Json* pAp = Json_getItem(pSkillItemJson, "ap");
            Json* pTt = Json_getItem(pSkillItemJson, "tt");
            Json* pTn = Json_getItem(pSkillItemJson, "tn");
            Json* pSf = Json_getItem(pSkillItemJson, "sf");
            Json* pSc = Json_getItem(pSkillItemJson, "sc");
            Json* pRt = Json_getItem(pSkillItemJson, "rt");
            
            SkillItem* pSkillItem = new SkillItem();
            pSkillItem->init();
            
            pSkillItem->setId(atoi(pId->valuestring));
            pSkillItem->setName(pName->valuestring);
            pSkillItem->setDesc(pDesc->valuestring);
            pSkillItem->setC(pC->valuestring);
            pSkillItem->setCm(pCm->valuestring);
            pSkillItem->setCz(atoi(pCz->valuestring));
            pSkillItem->setCs(atof(pCs->valuestring));
            pSkillItem->setBm(pBm->valuestring);
            pSkillItem->setBz(atoi(pBz->valuestring));
            pSkillItem->setBs(atof(pBs->valuestring));
            pSkillItem->setIs(atoi(pIs->valuestring) == 1);
            pSkillItem->setIsco(atoi(pIsco->valuestring) == 1);
            pSkillItem->setBp(atoi(pBp->valuestring));
            pSkillItem->setCos(atoi(pCos->valuestring));
            pSkillItem->setOs(atoi(pOs->valuestring));
            pSkillItem->setTm(pTm->valuestring);
            pSkillItem->setTz(atoi(pTz->valuestring));
            pSkillItem->setTs(atof(pTs->valuestring));
            pSkillItem->setTy(atoi(pTy->valuestring));
            pSkillItem->setLv(atoi(pLv->valuestring));
            pSkillItem->setAt(atoi(pAt->valuestring));
            pSkillItem->setAp(atoi(pAp->valuestring));
            pSkillItem->setTt(atoi(pTt->valuestring));
            pSkillItem->setTn(atoi(pTn->valuestring));
            pSkillItem->setSf(atoi(pSf->valuestring));
            pSkillItem->setSc(atoi(pSc->valuestring));
            pSkillItem->setRt(atoi(pRt->valuestring));
            // 添加技能数据
            pSkillManager->appendSkillItem(pSkillItem);
            pSkillItem->release();
            
            // 下一个技能
            pSkillItemJson = pSkillItemJson->next;
        } /*while*/
        
        // 被动技能
        Json* pPassiveSkillJson = Json_getItem(pRootJson, "passive_skill");
        Json* pPassiveSkillItemJson = pPassiveSkillJson->child;
        while (pPassiveSkillItemJson) {
            Json* pId = Json_getItem(pPassiveSkillItemJson, "id");
            Json* pName = Json_getItem(pPassiveSkillItemJson, "na");
            Json* pDesc = Json_getItem(pPassiveSkillItemJson, "de");
            Json* pTy = Json_getItem(pPassiveSkillItemJson, "ty");
            Json* pLv = Json_getItem(pPassiveSkillItemJson, "lv");
            
            SkillItem* pPassiveSkillItem = new SkillItem();
            pPassiveSkillItem->init();
            
            pPassiveSkillItem->setId(atoi(pId->valuestring));
            pPassiveSkillItem->setName(pName->valuestring);
            pPassiveSkillItem->setDesc(pDesc->valuestring);
            pPassiveSkillItem->setTy(atoi(pTy->valuestring));
            pPassiveSkillItem->setLv(atoi(pLv->valuestring));

            // 添加被动技能数据
            pSkillManager->appendSkillItem(pPassiveSkillItem);
            pPassiveSkillItem->release();
            
            // 下一个被动技能
            pPassiveSkillItemJson = pPassiveSkillItemJson->next;
        } /*while*/
        
        Json_dispose (pRootJson);
    } while (0);
    
    /* 监狱武将释放获得铜币数据 */
    do {
        path = CCFileUtils::sharedFileUtils()->fullPathForFilename(PRISON_JSON_FILE);
        content = (char*)CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "r", &size);
        CCAssert(content && size > 0, "invalid Loyalty file");
        
        std::string text;
        text.assign(content, size);
        text += '\0';
        delete [] content;
        
        Json* pRootJson = Json_create(text.c_str());
        CCAssert(pRootJson, "invalid Loyalty json file");
        
        Json* pPrison = Json_getItem(pRootJson, "release_prison");
        CCAssert(pPrison, "invalid prison data");
        Json* pReleaseJson = pPrison->child;
        while (pReleaseJson) {
            Json* pId = Json_getItem(pReleaseJson, "id");
            Json* pPu = Json_getItem(pReleaseJson, "pu");
            Json* pBl = Json_getItem(pReleaseJson, "bl");
            
            LoyaltyItem* pLoyaltyItem = new LoyaltyItem();
            pLoyaltyItem->init();
            
            pLoyaltyItem->setId(atoi(pId->valuestring));
            pLoyaltyItem->setPu(atoi(pPu->valuestring));
            pLoyaltyItem->setBl(atoi(pBl->valuestring));
            pHeroManager->appendLoyaltyItem(pLoyaltyItem);
            pLoyaltyItem->release();
            
            pReleaseJson = pReleaseJson->next;
        }
        Json_dispose (pRootJson);
    } while (0);
    
    /* 国战城池数据 */
    do {
        path = CCFileUtils::sharedFileUtils()->fullPathForFilename(CITY_JSON_FILE);
        content = (char*)CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "r", &size);
        CCAssert(content && size > 0, "invalid city file");
        
        std::string text;
        text.assign(content, size);
        text += '\0';
        delete [] content;
        
        Json* pRootJson = Json_create(text.c_str());
        CCAssert(pRootJson, "invalid city file");
        
        Json* pCFunctionJson = Json_getItem(pRootJson, "cfunction");
        CCAssert(pCFunctionJson, "invalid function");
        Json* pCFunctionChildJson = pCFunctionJson->child;
        while (pCFunctionChildJson) {
            Json* pId = Json_getItem(pCFunctionChildJson, "id");
            Json* pName = Json_getItem(pCFunctionChildJson, "na");
            Json* pP = Json_getItem(pCFunctionChildJson, "p");
            Json* pDes = Json_getItem(pCFunctionChildJson, "des");
            Json* pI = Json_getItem(pCFunctionChildJson, "i");
            
            CityFunctionItem* pCityFunctionItem = new CityFunctionItem();
            pCityFunctionItem->setId(atoi(pId->valuestring));
            pCityFunctionItem->setName(pName->valuestring);
            pCityFunctionItem->setP(atof(pP->valuestring));
            pCityFunctionItem->setDes(pDes->valuestring);
            pCityFunctionItem->setI(pI->valuestring);
            
            
            pCityManager->appendCityFunctionItem(pCityFunctionItem->getId(), pCityFunctionItem);
            
            // 下一个城池
            pCFunctionChildJson = pCFunctionChildJson->next;
        } /*while*/
        
        char szFuncBuffer[32];
        Json* pCwarJson = Json_getItem(pRootJson, "cwar");
        CCAssert(pCwarJson, "invalid cwar data");
        Json* pCwarChildJson = pCwarJson->child;
        while (pCwarChildJson) {
            Json* pId = Json_getItem(pCwarChildJson, "id");
            Json* pName = Json_getItem(pCwarChildJson, "na");
            Json* pFunc = Json_getItem(pCwarChildJson, "fu");
            
            CityItem* pCityItem = new CityItem();
            
            pCityItem->setId(atoi(pId->valuestring));
            pCityItem->setName(pName->valuestring);
            
            strcpy(szFuncBuffer, pFunc->valuestring);
            char* szFunc = strtok(szFuncBuffer, "|");
            while (szFunc) {
                CityFunctionItem* pCityFunctionItem = pCityManager->getCityFuncItem(atoi(szFunc));
                CCAssert(pCityFunctionItem, "invalid CityFunctionItem");
                
                pCityItem->appendCityFunc(pCityFunctionItem);
                
                szFunc = strtok(NULL, "|");
            } /*while*/
            pCityManager->appendCityItem(pCityItem->getId(), pCityItem);
            pCityItem->release();
            
            //
            pCwarChildJson = pCwarChildJson->next;
        } /*while*/
        
        Json* pCWarAwardJson = Json_getItem(pRootJson, "caward");
        CCAssert(pCWarAwardJson, "invalid caward data");
        Json* pCWarAwardChildJson = pCWarAwardJson->child;
        while (pCWarAwardChildJson) {
            Json* pLv = Json_getItem(pCWarAwardChildJson, "lv");
            Json* pAward = Json_getItem(pCWarAwardChildJson, "a");
            Json* pT = Json_getItem(pCWarAwardChildJson, "t");
            
            CityAwardItem* pCityAwardItem = new CityAwardItem();
            
            pCityAwardItem->setLV(atoi(pLv->valuestring));
            pCityAwardItem->setAward(atoi(pAward->valuestring));
            pCityAwardItem->setT(atof(pT->valuestring));
            
            pCityManager->appendCityAwardItem(pCityAwardItem->getLV(), pCityAwardItem);
            pCityAwardItem->release();
            
            //
            pCWarAwardChildJson = pCWarAwardChildJson->next;
        } /*while*/
        
        Json_dispose (pRootJson);
    } while (0);
}

/*
 *  处理服务器回复登录请求的结果数据
 */
bool ProcessData::processLoginResponseFromServer(const char *szData, const unsigned int dataSize) {
    CCAssert(szData && dataSize > 0, "no login response data");
    
    CCLog("玩家登录服务器回应数据 ---> %s", szData);
    
    HeroItemOfPlayerFromServer herosItemFromSvr;      // 保存从服务器下载的动态武将数据
    // “玩家“
    GamePlayer* pGamePlayer = GamePlayer::getInstance();
    /* 游戏状态 */
    GameState* pGameState = GameState::getInstance();
    /* 数据处理 */
    ProcessData* pProcessData = ProcessData::getInstance();
    
    // 等待本地数据加载完成
    while (true) {
        if (__sync_fetch_and_add(&pProcessData->m_localDataIsReady, 0) > 0) /*本地数据文件已经加载完成*/{
            break;
        }
    } /*while*/
    
    bool isLoginSuccessed = false;      // 是否登录成功
    Json* pRootJson = NULL;
    do {
        // 解析登录回复 json 数据
        pRootJson = Json_create(szData);
        CCAssert(pRootJson, "invalid login data");
        
        Json* pSuccJson = Json_getItem(pRootJson, "success");
        CCAssert(pSuccJson, "invalid json data");
        CC_BREAK_IF(1 != pSuccJson->valueint);
        
        Json* pResultJson = Json_getItem(pRootJson, "result");
        Json* pBaseJson = Json_getItem(pResultJson, "base_config");
        Json* pResultIdJson = Json_getItem(pResultJson, "uid");
        Json* pResultNameJson = Json_getItem(pResultJson, "name");
        Json* pResultLevelJson = Json_getItem(pResultJson, "level");
        Json* pResultExpJson = Json_getItem(pResultJson, "exp");
        Json* pResultMoneyJson = Json_getItem(pResultJson, "money");
        Json* pResultGoldJson = Json_getItem(pResultJson, "gold");
        Json* pCourageJson = Json_getItem(pResultJson, "courage");
        Json* pResultEnergyJson = Json_getItem(pResultJson, "energy");
        Json* pResultMaxEnergyJson = Json_getItem(pResultJson, "max_energy");
        Json* pResultEnergyTimeyJson = Json_getItem(pResultJson, "energy_time");
        Json* pResultEnergyBuyJson = Json_getItem(pResultJson, "energy_buy");
        Json* pResultEnergyPriceJson = Json_getItem(pResultJson, "energy_price");
        Json* pBaseCourageJson = Json_getItem(pBaseJson, "lucky_courage");
        Json* pBaseRmbJson = Json_getItem(pBaseJson, "lucky_rmb1");
        Json* pBaseRmb10Json = Json_getItem(pBaseJson, "lucky_rmb10");
        Json* pResultRateJson = Json_getItem(pResultJson, "rate");
        Json* pResultNewStatusJson = Json_getItem(pResultJson, "new_status");
        Json* pResultServerTimeJson = Json_getItem(pResultJson, "server_time");
        Json* pResultEnergyCycleJson = Json_getItem(pResultJson, "energy_cycle");
        Json* pResultMaxFriendJson = Json_getItem(pResultJson, "max_friend");
        Json* pResultMaxHeroJson = Json_getItem(pResultJson, "max_hero");
        // 玩家通关副本最大等级
        Json* pResultMaxPveJson = Json_getItem(pResultJson, "max_pve");
        Json* pMaxPveNormalJson = Json_getItem(pResultMaxPveJson, "normal");
        /*玩家所属阵营*/
        Json* pResultGroupJson = Json_getItem(pResultJson, "group");
        /*玩家拥有城池*/
        Json* pResultCityJson = Json_getItem(pResultJson, "own_city");
        //qjqj
        if (pResultGroupJson) {
            if (pResultGroupJson->valueint == 1) {
                pGamePlayer->setPlayerOfCamp(PLAYERCAMP_WEI);
            } else if (pResultGroupJson->valueint == 2) {
                pGamePlayer->setPlayerOfCamp(PLAYERCAMP_SHU);
            } else if (pResultGroupJson->valueint == 3) {
                pGamePlayer->setPlayerOfCamp(PLAYERCAMP_WU);
            } else {
                pGamePlayer->setPlayerOfCamp(PLAYERCAMP_NONE);
            }
        }
        Json* pCityJson = pResultCityJson->child;
        while (pCityJson) {
            pGamePlayer->appendOwnCity(pCityJson->valueint);
            
            pCityJson = pCityJson->next;
        }
        
        if (pMaxPveNormalJson) {
            pGameState->getInstance()->setNormalMax(Json_getItemAt(pMaxPveNormalJson, 0)->valueint,
                                                    Json_getItemAt(pMaxPveNormalJson, 1)->valueint);
        }
        // 设置玩家 id
        pGamePlayer->setUid(pResultIdJson->valueint);
        // 设置玩家姓名
        char buffer[16];
        sprintf(buffer, "Player %d", pResultIdJson->valueint);
        pGamePlayer->setName((int)strlen(pResultNameJson->valuestring) > 0 ? pResultNameJson->valuestring : buffer);
        // 设置玩家等级
        pGamePlayer->setLevel(pResultLevelJson->valueint);
        // 设置玩家经验值
        pGamePlayer->setExp(pResultExpJson->valueint);
        // 设置玩家元宝
        pGamePlayer->setGold(pResultGoldJson->valueint);
        // 设置玩家铜钱
        pGamePlayer->setMoney(pResultMoneyJson->valueint);
        // 设置玩家体力
        pGamePlayer->setEnergy(pResultEnergyJson->valueint);
        // 存储玩家最大体力
        pGamePlayer->setMaxEnergy(pResultMaxEnergyJson->valueint);
        // 体力回复时间
        pGamePlayer->setEnergyTime(pResultEnergyTimeyJson->valueint);
        // 战斗力
        pGamePlayer->setRate(pResultRateJson->valueint);
        // 新手引导状态
        pGamePlayer->setNewStatus(pResultNewStatusJson->valueint);
        // server 时间
        pGamePlayer->setServerTime(pResultServerTimeJson->valueint);
        //存储体力恢复周期
        pGamePlayer->setEnergyCycle(pResultEnergyCycleJson->valueint);
        //总共武将数，背包数
        pGamePlayer->setMaxHero(pResultMaxHeroJson->valueint);
        //玩家所拥有的好友上限
        pGamePlayer->setFd(pResultMaxFriendJson->valueint);
        //玩家一次免费抽卡所需要的勇气值
        pGamePlayer->setLuckCourage(pBaseCourageJson->valueint);
        //玩家一次付费抽卡所需要的元宝
        pGamePlayer->setLuckRMB(pBaseRmbJson->valueint);
        //玩家10次付费抽卡所需要的元宝
        pGamePlayer->setLuckRMBBig(pBaseRmb10Json->valueint);
        // 拥有的勇气值
        pGamePlayer->setCourage(pCourageJson->valueint);
        // 玩家购买体力次数
        pGamePlayer->setEnergyBuy(pResultEnergyBuyJson->valueint);
        // 玩家购买体力所需价格
        pGamePlayer->setEnergyPrice(pResultEnergyPriceJson->valueint);
        // 玩家登录成功标志
        pGamePlayer->setLogin(true);
        
        /*
         *  是否有未完成的战斗
         */
        Json* pEndlessFightJson = Json_getItem(pResultJson, "endless_fight");
        if (pEndlessFightJson) /*有未完成战斗*/ {
            // 保存未完成战斗信息
            pGameState->setEndlessFight(atoi(Json_getItemAt(pEndlessFightJson, 0)->valuestring),
                                        atoi(Json_getItemAt(pEndlessFightJson, 1)->valuestring),
                                        Json_getItemAt(pEndlessFightJson, 2)->valueint);
            // 获取未完成战斗的战队的位置
            Json* pUFJson = Json_getItem(pResultJson, "last_position");
            CCAssert(pUFJson, "invalid data");
            
            unsigned int ownTeamPos[] = {21, 22, 23, 11, 12, 13};
            char szUfTemmPos[128];
            strcpy(szUfTemmPos, pUFJson->valuestring);
            unsigned int posIndex = 0;
            char* pUfTeamPos = strtok(szUfTemmPos, "|");
            pGameState->clearUfTeamPos();
            while (pUfTeamPos) {
                if (strlen(pUfTeamPos) > 1) {
                    pGameState->appendUfTeamPos(pUfTeamPos, ownTeamPos[posIndex]);
                }
                posIndex++;
                pUfTeamPos = strtok(NULL, "|");
            } /*while*/
        }
        
        isLoginSuccessed = true;
    } while (0);
    
    if (!isLoginSuccessed && pRootJson) {
        Json* pMsgJson = Json_getItem(pRootJson, "msg");
        CCLog("error message = %s", pMsgJson->valuestring);
        MessageBox::getInstance()->messageTips(pMsgJson->valuestring);
    }
    if (pRootJson) {
        // 释放资源
        Json_dispose(pRootJson);
        pRootJson = NULL;
    }
    
    return isLoginSuccessed;
}

/*
 *  处理从服务器下载的武将数据
 */
bool ProcessData::processHerosDataFromServer(const char *szData, const unsigned int dataSize) {
    CCAssert(szData && dataSize > 0, "invalid heros data");
    
    CCLog("玩家武将数据 = %s", szData);
    
    // 保存从服务器下载的动态武将数据
    HeroItemOfPlayerFromServer herosItemFromSvr;
    // 武将管理
    HeroManager* pHeroManager = HeroManager::getInstance();
    
    bool isSuccess = false;
    // 解析从服务器下载的玩家所属武将数据(json)
    Json* pRootJson = Json_create(szData);
    do {
        CCAssert(pRootJson, "invalid heros data");
        
        Json* pSuccJson = Json_getItem(pRootJson, "success");
        CC_BREAK_IF(1 != pSuccJson->valueint);
        
        Json* pResultJson = Json_getItem(pRootJson, "result");
        Json* pHeroJson = Json_getItem(pResultJson, "hero");
        
        std::string goIntoPveBattleHeroTags[MAX_GO_INTO_BATTLE_HEROS];
        unsigned int numberOfPveGoIntoBattleHeroTags = 0;
        std::string goIntoPvpBattleHeroTags[MAX_GO_INTO_BATTLE_HEROS];
        unsigned int pvpTeamPos[MAX_GO_INTO_BATTLE_HEROS];
        unsigned int numberOfPvpGoIntoBattleHeroTags = 0;
        
        Json* pHeroItemJson = pHeroJson->child;
        while (pHeroItemJson) {
            // tag
            Json* pHeroTagJson = Json_getItem(pHeroItemJson, "tag");
            // id
            Json* pHeroIdJson = Json_getItem(pHeroItemJson, "id");
            // level
            Json* pHeroLevelJson = Json_getItem(pHeroItemJson, "level");
            // exp
            Json* pHeroExplJson = Json_getItem(pHeroItemJson, "exp");
            // weapon
            Json* pHeroWeaponlJson = Json_getItem(pHeroItemJson, "weapon");
            // armor
            Json* pHeroArmorJson = Json_getItem(pHeroItemJson, "armor");
            // isLock
            Json* pHeroLockJson = Json_getItem(pHeroItemJson, "lock");
            // isGoIntoBattle
            Json* pHeroTypeJson = Json_getItem(pHeroItemJson, "type");
            
            herosItemFromSvr.uniId = pHeroTagJson->valuestring;
            herosItemFromSvr.id = pHeroIdJson->valueint;
            herosItemFromSvr.level = pHeroLevelJson->valueint;
            herosItemFromSvr.exp = pHeroExplJson->valueint;
            herosItemFromSvr.weapon = pHeroWeaponlJson->valueint;
            herosItemFromSvr.armor = pHeroArmorJson->valueint;
            herosItemFromSvr.isLock = (1 == pHeroLockJson->valueint);
            if (!strcmp(pHeroTypeJson->valuestring, "g"))/*上阵*/ {
                herosItemFromSvr.isGoIntoBattle = true;
                goIntoPveBattleHeroTags[numberOfPveGoIntoBattleHeroTags++] = pHeroTagJson->valuestring;
            } else {
                herosItemFromSvr.isGoIntoBattle = false;
            }
            
            // 增加玩家所属武将数据
            pHeroManager->appendHeroOfPlayer(herosItemFromSvr);
            
            // next one
            pHeroItemJson = pHeroItemJson->next;
        } /*while*/
        
        /*
         *  pvp 上阵武将
         */
        Json* pTeamJson = Json_getItem(pResultJson, "team");
        unsigned int posIndexInBattle[] = {21,22,23,11,12,13};
        char szPvpGoIntoBattleHerosBuffer[128];
        strcpy(szPvpGoIntoBattleHerosBuffer, pTeamJson->valuestring);
        
        unsigned int nTempIndex = 0;
        char* pPvpGoIntoBattleHeroBuffer = strtok(szPvpGoIntoBattleHerosBuffer, "|");
        while (pPvpGoIntoBattleHeroBuffer) {
            if (strlen(pPvpGoIntoBattleHeroBuffer) > 1) {
                goIntoPvpBattleHeroTags[numberOfPvpGoIntoBattleHeroTags].assign(pPvpGoIntoBattleHeroBuffer);
                pvpTeamPos[numberOfPvpGoIntoBattleHeroTags] = posIndexInBattle[nTempIndex];
                numberOfPvpGoIntoBattleHeroTags++;
            }
            nTempIndex++;
            pPvpGoIntoBattleHeroBuffer = strtok(NULL, "|");
        } /*while*/
        
        // 更新 pve 上阵武将数据
        pHeroManager->updateGoIntoBattleHerosOfPlayer(numberOfPveGoIntoBattleHeroTags, goIntoPveBattleHeroTags);
        // 更新 pvp 上阵武将数据
        pHeroManager->updatePvpGoIntoBattleHerosOfPlayer(numberOfPvpGoIntoBattleHeroTags, goIntoPvpBattleHeroTags, pvpTeamPos);
        
        CCLog("玩家所属武将数量 = %d 上阵武将(pve)数量 = %d 上阵武将(pvp)数量 = %d",
              pHeroManager->getHerosOfPlayerCount(),
              pHeroManager->getGoIntoBattleHerosOfPlayerCount(),
              pHeroManager->getPvpGoIntoBattleHerosOfPlayerCount());
        
        isSuccess = true;
    } while (0);
    
    if (!isSuccess && pRootJson) {
        Json* pMsgJson = Json_getItem(pRootJson, "msg");
        CCLog("error message = %s", pMsgJson->valuestring);
        MessageBox::getInstance()->messageTips(pMsgJson->valuestring);
    }
    if (pRootJson) {
        // 释放资源
        Json_dispose(pRootJson);
        pRootJson = NULL;
    }
    
    return isSuccess;
}

/*
 *  处理从服务器下载的购买体力数据
 */
static bool processBuyEnergy(const char *szBuffer) {
    CCAssert(szBuffer, "no buy energy data");
    //    CCLog("购买体力 = %s", szBuffer);
    
    // “玩家“
    GamePlayer* pGamePlayer = GamePlayer::getInstance();
    GameState* pGameState = GameState::getInstance();
    bool isSuccess = false;
    Json* pRootJson = NULL;
    do {
        pRootJson = Json_create(szBuffer);
        CCAssert(szBuffer, "invalid json data");
        
        Json* pSuccessJson = Json_getItem(pRootJson, "success");
        CCAssert(pSuccessJson, "invalid json data");
        CC_BREAK_IF(1 != pSuccessJson->valueint);
        
        Json* pResultJson = Json_getItem(pRootJson, "result");
        Json* pEnergyBuyJson = Json_getItem(pResultJson, "energy_buy");
        Json* pEnergyPriceJson = Json_getItem(pResultJson, "energy_price");
        
        pGamePlayer->setEnergyBuy(pEnergyBuyJson->valueint); /*剩余购买次数*/
//        pGamePlayer->setEnergyPrice(pEnergyPriceJson->valueint); /*购买价格*/
        pGameState->setTempPrice(pEnergyPriceJson->valueint);
        
        isSuccess = true;
    } while (0);
    
    if (!isSuccess && pRootJson) {
        Json* pMsgJson = Json_getItem(pRootJson, "msg");
        CCLog("error msg = %s", pMsgJson->valuestring);
        MessageBox::getInstance()->messageTips(pMsgJson->valuestring);
    }
    
    if (pRootJson) {
        // 释放资源
        Json_dispose(pRootJson);
        pRootJson = NULL;
    }
    
    return isSuccess;
}

/*
 *  处理服务器返回的lock数据
 */
static bool processLockHero(const char *szBuffer) {
    CCLog("解锁武将 = %s", szBuffer);
    bool isSuccess = false;
    Json* pRootJson = NULL;
    do {
        pRootJson = Json_create(szBuffer);
        CCAssert(pRootJson, "invalid data");
        
        Json* pSuccessJson = Json_getItem(pRootJson, "success");
        CC_BREAK_IF(pSuccessJson->valueint != 1);
        isSuccess = true;
    } while (0);
    
    if (!isSuccess && pRootJson) {
        Json* pMsgJson = Json_getItem(pRootJson, "msg");
        CCLog("error msg = %s", pMsgJson->valuestring);
        MessageBox::getInstance()->messageTips(pMsgJson->valuestring);
    }
    if (pRootJson) {
        // 释放资源
        Json_dispose(pRootJson);
        pRootJson = NULL;
    }
    
    return isSuccess;
}

/*
 *  初始化监狱武将
 */
static bool processForInitJy(const char *szBuffer) {
    CCAssert(szBuffer, "no buy energy data");
    //    CCLog("监狱数据 = %s", szBuffer);
    
    HeroManager* pHeroManager = HeroManager::getInstance();
    GamePlayer* pGamePlayer = GamePlayer::getInstance();
    
    bool isSuccess = false;
    Json* pRootJson = NULL;
    pHeroManager->clearAllPrisonHeroItems();
    do {
        pRootJson = Json_create(szBuffer);
        CCAssert(pRootJson, "invalid json data");
        
        Json* pSuccessJson = Json_getItem(pRootJson, "success");
        CCAssert(pSuccessJson, "invalid json data");
        CC_BREAK_IF(1 != pSuccessJson->valueint);
        
        Json* pResultJson = Json_getItem(pRootJson, "result");
        Json* pHerosJson = Json_getItem(pResultJson, "heros");
        Json* pBluePropJson = Json_getItem(pResultJson, "blue_prop");
        Json* pPurPropJson = Json_getItem(pResultJson, "purple_prop");
        Json* pBlueGoldJson = Json_getItem(pResultJson, "blue_prop_gold");
        Json* pPurGoldJson = Json_getItem(pResultJson, "purple_prop_gold");
        
        /* 存储玩家拥有紫色和蓝色道具的数量 */
        pGamePlayer->setPropZiNum(pPurPropJson->valueint);
        pGamePlayer->setPropLanNum(pBluePropJson->valueint);
        pGamePlayer->setPropZiXi(pPurGoldJson->valueint);
        pGamePlayer->setPropLanXi(pBlueGoldJson->valueint);
        
        PrisonHeroItem* pPrisonHeroItem = NULL;
        Json* pHeroItemJson = pHerosJson->child;
        while (pHeroItemJson) {
            Json* pHeroItemIdJson = Json_getItem(pHeroItemJson, "id");
            Json* pHeroItemLoyalJson = Json_getItem(pHeroItemJson, "loyalty");
            Json* pHeroItemMoneyJson = Json_getItem(pHeroItemJson, "money");
            Json* pHeroItemNeedProJson = Json_getItem(pHeroItemJson, "need_prop");
            Json* pNeedPropTyepJson = Json_getItem(pHeroItemNeedProJson, "type");
            Json* pNeedPropNumJson = Json_getItem(pHeroItemNeedProJson, "num");
            
            pPrisonHeroItem = PrisonHeroItem::create();
            // id
            pPrisonHeroItem->setHeroId(pHeroItemIdJson->valueint);
            // loyal
            pPrisonHeroItem->setDevotion(pHeroItemLoyalJson->valueint);
            // money
            pPrisonHeroItem->setMoney(pHeroItemMoneyJson->valueint);
            // 类型
            if (!strcmp(pNeedPropTyepJson->valuestring, "purple_prop")) {
                pPrisonHeroItem->setTypeProp(PURPLE_PROP);
                /*存储玩家需要扣除的元宝*/
                if (pPurPropJson->valueint - pNeedPropNumJson->valueint < 0) {
                    pPrisonHeroItem->setGold((pNeedPropNumJson->valueint - pPurPropJson->valueint) * pPurGoldJson->valueint);
                } else {
                    pPrisonHeroItem->setGold(0);
                }
                
            } else if(!strcmp(pNeedPropTyepJson->valuestring, "blue_prop"))/*蓝色道具*/ {
                pPrisonHeroItem->setTypeProp(BLUE_PROP);
                /*存储玩家需要扣除的元宝*/
                if (pBluePropJson->valueint - pNeedPropNumJson->valueint < 0) {
                    pPrisonHeroItem->setGold((pNeedPropNumJson->valueint - pBluePropJson->valueint) * pBlueGoldJson->valueint);
                } else {
                    pPrisonHeroItem->setGold(0);
                }
            }
            // 需要道具数量
            pPrisonHeroItem->setNum(pNeedPropNumJson->valueint);
            
            // 添加到监狱
            pHeroManager->appendPrisonHeroItem(pPrisonHeroItem);
            
            pHeroItemJson = pHeroItemJson->next;
        } /*while*/
        
        isSuccess = true;
    } while (0);
    
    if (!isSuccess && pRootJson) {
        Json* pMsgJson = Json_getItem(pRootJson, "msg");
        CCLog("error msg = %s", pMsgJson->valuestring);
        MessageBox::getInstance()->messageTips(pMsgJson->valuestring);
    }
    if (pRootJson) {
        // 释放资源
        Json_dispose(pRootJson);
        pRootJson = NULL;
    }
    
    return isSuccess;
}

/*
 *  释放监狱武将
 */
static bool processForFreeJy(const char *szBuffer) {
    CCAssert(szBuffer, "no buy energy data");
    //    CCLog("释放监狱武将 = %s", szBuffer);
    
    GamePlayer* pGamePlayer = GamePlayer::getInstance();
    bool isSuccess = false;
    Json* pRootJson = NULL;
    do {
        pRootJson = Json_create(szBuffer);
        CCAssert(pRootJson, "invalid json data");
        
        Json* pSuccessJson = Json_getItem(pRootJson, "success");
        CCAssert(pSuccessJson, "invalid json data");
        GameState::getInstance()->setIsSucessShiF(1 == pSuccessJson->valueint);
        CC_BREAK_IF(1 != pSuccessJson->valueint);
        
        Json* pResultJson = Json_getItem(pRootJson, "result");
        Json* pAddMoneyJson = Json_getItem(pResultJson, "add_money");
        
        pGamePlayer->setMoney(pGamePlayer->getMoney() + atoi(pAddMoneyJson->valuestring));
        
        // 释放时删除监狱武将
        
        isSuccess = true;
    } while (0);
    
    if (!isSuccess && pRootJson) {
        Json* pMsgJson = Json_getItem(pRootJson, "msg");
        CCLog("error msg = %s", pMsgJson->valuestring);
        MessageBox::getInstance()->messageTips(pMsgJson->valuestring);
    }
    if (pRootJson) {
        // 释放资源
        Json_dispose(pRootJson);
        pRootJson = NULL;
    }
    
    return isSuccess;
}

/*
 *  招降监狱武将失败返回最新武将信息，成功返回tag
 */
static bool processForZhaoXJy(const char *szBuffer) {
    CCAssert(szBuffer, "no buy energy data");
    CCLog("招降监狱武将 = %s", szBuffer);
    
    HeroManager* pHeroManager = HeroManager::getInstance();
    GameState* pGameState = GameState::getInstance();
    bool isSuccess = false;
    Json* pRootJson = NULL;
    do {
        pRootJson = Json_create(szBuffer);
        CCAssert(pRootJson, "invalid json data");
        
        Json* pSuccessJson = Json_getItem(pRootJson, "success");
        CCAssert(pSuccessJson, "invalid json data");
        CC_BREAK_IF(1 != pSuccessJson->valueint);
        
        Json* pResultJson = Json_getItem(pRootJson, "result");
        Json* pBlueNumJson = Json_getItem(pResultJson, "blue_prop");
        Json* pPurpNumJson = Json_getItem(pResultJson, "purple_prop");
        Json* pSubMoneyJson = Json_getItem(pResultJson, "sub_money");
//        Json* pSubPropTypeJson = Json_getItem(pResultJson, "prop_type");
//        Json* pSubPropJson = Json_getItem(pResultJson, "sub_prop");
        Json* pSubGoldJson = Json_getItem(pResultJson, "sub_gold");
        
        /* 招降成功失败，都会消耗道具和铜钱 */
        GamePlayer* pGamePlayer = GamePlayer::getInstance();
        pGamePlayer->setPropLanNum(pBlueNumJson->valueint);
        pGamePlayer->setPropZiNum(pPurpNumJson->valueint);
        //
        int curMoney = pGamePlayer->getMoney() - pSubMoneyJson->valueint;
        pGamePlayer->setMoney(curMoney);
        int curGold  = pGamePlayer->getGold() - pSubGoldJson->valueint;
        pGamePlayer->setGold(curGold);
        
        Json* pHerosJson = Json_getItem(pResultJson, "hero");
        if (pHerosJson)/*招降失败*/ {
            Json* pHeroItemIdJson = Json_getItem(pHerosJson, "id");
            Json* pHeroItemLoyalJson = Json_getItem(pHerosJson, "loyalty");
            Json* pHeroItemMoneyJson = Json_getItem(pHerosJson, "money");
            Json* pNeedPropJson = Json_getItem(pHerosJson, "need_prop");
//            Json* pNeedPropTypeJson = Json_getItem(pNeedPropJson, "type");
            Json* pNeedPropNumJson = Json_getItem(pNeedPropJson, "num");
            
            // 记录招降失败状态
            pGameState->setIsSummonSucess(false);
            // 记录降低了多少忠诚度
            int sub = pHeroManager->getPrisonHeroItemById(pHeroItemIdJson->valueint)->getDevotion() - pHeroItemLoyalJson->valueint;
            pGameState->setSubLoyalty(sub);
            /* 替换原来武将信息 */
            PrisonHeroItem* pPrisonHeroItem = pHeroManager->getPrisonHeroItemById(pHeroItemIdJson->valueint);
            pPrisonHeroItem->setMoney(pHeroItemMoneyJson->valueint);
            pPrisonHeroItem->setDevotion(pHeroItemLoyalJson->valueint);
            
            /* 需要道具更新 */
            pPrisonHeroItem->setNum(pNeedPropNumJson->valueint);
        }
        
        Json* pNewHeroJson = Json_getItem(pResultJson, "new_hero");
        if (pNewHeroJson)/*招降成功*/ {
            Json* pHeroItemTagJson = Json_getItem(pNewHeroJson, "tag");
            Json* pIdTagJson = Json_getItem(pNewHeroJson, "id");
            Json* pHeroItemLevelJson = Json_getItem(pNewHeroJson, "level");
            Json* pHeroItemExpJson = Json_getItem(pNewHeroJson, "exp");
            Json* pHeroItemWeaponJson = Json_getItem(pNewHeroJson, "weapon");
            Json* pHeroItemArmorJson = Json_getItem(pNewHeroJson, "armor");
            Json* pHeroItemLockJson = Json_getItem(pNewHeroJson, "lock");
            
            HeroItemOfPlayerFromServer hiopFS;
            hiopFS.uniId.assign(pHeroItemTagJson->valuestring);
            hiopFS.id = pIdTagJson->valueint;
            hiopFS.level = pHeroItemLevelJson->valueint;
            hiopFS.exp = pHeroItemExpJson->valueint;
            hiopFS.weapon = pHeroItemWeaponJson->valueint;
            hiopFS.armor = pHeroItemArmorJson->valueint;
            hiopFS.isLock = (1 == pHeroItemLockJson->valueint);
            hiopFS.isGoIntoBattle = false;
            
            pGameState->setSucessSummonId(pIdTagJson->valueint);
            // 记录招降成功状态
            pGameState->setIsSummonSucess(true);
            // 招降成功删除监狱武将
            pHeroManager->removePrisonHeroItemById(pIdTagJson->valueint);
            // 向本地背包添加监狱招降武将
            pHeroManager->appendHeroOfPlayer(hiopFS);
        }
        
        isSuccess = true;
    } while (0);
    
    if (!isSuccess && pRootJson) {
        Json* pMsgJson = Json_getItem(pRootJson, "msg");
        CCLog("error msg = %s", pMsgJson->valuestring);
        MessageBox::getInstance()->messageTips(pMsgJson->valuestring);
    }
    if (pRootJson) {
        // 释放资源
        Json_dispose(pRootJson);
        pRootJson = NULL;
    }
    return isSuccess;
}

/*
 *  处理服务器返回的进入铜雀台数据
 */
static bool processEnterTQT(const char *szBuffer) {
    CCAssert(szBuffer, "invalid data");
    CCLog("进入铜雀台 = %s", szBuffer);
    
    bool isSuccess = false;
    Json* pRootJson = NULL;
    
    HeroManager* pHeroManager = HeroManager::getInstance();
    Battle* pBattle = Battle::getInstance();
    GamePlayer* pGamePlayer = GamePlayer::getInstance();
    GameState* pGameState = GameState::getInstance();
    do {
        pRootJson = Json_create(szBuffer);
        CCAssert(pRootJson, "invalid data");
        
        Json* pSuccessJson = Json_getItem(pRootJson, "success");
        CCAssert(pSuccessJson, "invalid data");
        CC_BREAK_IF(pSuccessJson->valueint != 1);
        
        Json* pResultJson = Json_getItem(pRootJson, "result");
        Json* pTimesJson = Json_getItem(pResultJson, "times");
        Json* pCdJson = Json_getItem(pResultJson, "cd");
        
        // 保存剩余次数
        pBattle->setRemainBattleTimes(pTimesJson->valueint);
        // 保存cd
        pGameState->setCd(pCdJson->valueint);
        
        Json* pEnemiesJson = Json_getItem(pResultJson, "enemies");
        Json* pMyRankJson = Json_getItem(pResultJson, "my_rank");
        Json* pTotalRankJson = Json_getItem(pResultJson, "total_rank");
        Json* pStatusJson = Json_getItem(pResultJson, "day_reward_status");
        Json* pRewardMoney = Json_getItem(pResultJson, "day_reward_money");
        Json* pRewardTimes = Json_getItem(pResultJson, "day_reward_rest_time");
        
        // 战斗报告
        Json* pBattleReportJson = Json_getItem(pResultJson, "report");
        if (pBattleReportJson) {
            pBattle->clearAllBattleReport();
            Json* pBattleReportJsonChild = pBattleReportJson->child;
            
            BattleReportItem* pBattleReportItem = NULL;
            while (pBattleReportJsonChild) {
                Json* pTp = Json_getItem(pBattleReportJsonChild, "tp");
                Json* pName = Json_getItem(pBattleReportJsonChild, "nm");
                Json* pRank = Json_getItem(pBattleReportJsonChild, "rk");
                Json* pUd = Json_getItem(pBattleReportJsonChild, "ud");
                Json* pMoney = Json_getItem(pBattleReportJsonChild, "rw");
                Json* pTm = Json_getItem(pBattleReportJsonChild, "tm");
                
                pBattleReportItem = BattleReportItem::create();
                if (!strcmp(pTp->valuestring, "to")) {
                    pBattleReportItem->setTp(1);
                } else {
                    pBattleReportItem->setTp(2);
                }
                pBattleReportItem->setName(pName->valuestring);
                pBattleReportItem->setRank(pRank->valueint);
                pBattleReportItem->setUd(pUd->valueint);
                pBattleReportItem->setMoney(pMoney->valueint);
                pBattleReportItem->setTm(pTm->valueint);
                
                // 添加战斗报告
                pBattle->appendBattleReport(pBattleReportItem);
                
                // 下一个战斗报告
                pBattleReportJsonChild = pBattleReportJsonChild->next;
            } /*while*/
        }
        
        pGamePlayer->setRank(pMyRankJson->valueint);
        pGamePlayer->setIsReward(0 == pStatusJson->valueint); // 0 表示领过了 否则表示没有领取
        pGamePlayer->setRewardMoney(pRewardMoney->valueint);
        pGameState->appendTimerTask(TIME_TQT, pRewardTimes->valueint);
        pGamePlayer->setTotalRank(pTotalRankJson->valueint);
        
        Json* pTeamJson = Json_getItem(pResultJson, "team");
        if (pTeamJson) {
            unsigned int posIndexInBattle[] = {21,22,23,11,12,13};
            std::string uniid[MAX_GO_INTO_BATTLE_HEROS];
            unsigned int teamPos[MAX_GO_INTO_BATTLE_HEROS];
            unsigned int countOfPvpGotoBattleHeros = 0;
            
            char szBufferGoBattleHeros[128];
            strcpy(szBufferGoBattleHeros, pTeamJson->valuestring);
            
            char* pBufferGoBattleHeros = strtok(szBufferGoBattleHeros, "|");
            unsigned int nTempIndex = 0;
            while (pBufferGoBattleHeros) {
                if (strlen(pBufferGoBattleHeros) > 1) {
                    uniid[countOfPvpGotoBattleHeros].assign(pBufferGoBattleHeros);
                    teamPos[countOfPvpGotoBattleHeros] = posIndexInBattle[nTempIndex];
                    countOfPvpGotoBattleHeros++;
                }
                nTempIndex++;
                pBufferGoBattleHeros = strtok(NULL, "|");
            } /*while*/
            // 更新pvp上阵武将
            pHeroManager->updatePvpGoIntoBattleHerosOfPlayer(countOfPvpGotoBattleHeros, uniid, teamPos);
        }
        
        Json* pEnemyJson = pEnemiesJson->child;
        unsigned int index = 1;
        
        pHeroManager->removeAllTqHeadItems();
        while (pEnemyJson) {
            Json* pTypeJson = Json_getItem(pEnemyJson, "type");
            Json* pHeadJson = Json_getItem(pEnemyJson, "head");
            Json* pRankJson = Json_getItem(pEnemyJson, "rank");
            Json* pNameJson = Json_getItem(pEnemyJson, "name");
            
            TqHeadItem* pTqHeadItem = TqHeadItem::create();
            pTqHeadItem->setPosition(index);
            pTqHeadItem->setId(pHeadJson->valueint);
            if (!strcmp(pTypeJson->valuestring, "npc")) {
                pTqHeadItem->setType(2);
            } else {
                pTqHeadItem->setType(1);
            }
            pTqHeadItem->setRank(pRankJson->valueint);
            pTqHeadItem->setName(pNameJson->valuestring);
            
            /* 添加铜雀台Item */
            pHeroManager->appendTqHeadItem(pTqHeadItem);
            
            index++;
            pEnemyJson = pEnemyJson->next;
        } /*while*/
        
        isSuccess = true;
    } while (0);
    
    if (!isSuccess && pRootJson) {
        Json* pMsgJson = Json_getItem(pRootJson, "msg");
        CCLog("error msg = %s", pMsgJson->valuestring);
        MessageBox::getInstance()->messageTips(pMsgJson->valuestring);
    }
    if (pRootJson) {
        // 释放资源
        Json_dispose(pRootJson);
        pRootJson = NULL;
    }
    return isSuccess;
}

/*
 *  功能：
 *      处理服务器返回的领取铜雀台的数据
 */
static bool processTQTReplay(const char *szBuffer) {
    CCAssert(szBuffer, "invalid data");
    CCLog("铜雀台战斗回放 = %s", szBuffer);
    
    Battle* pBattle = Battle::getInstance();
    bool isSuccess = false;
    Json* pRootJson = NULL;
    do {
        pRootJson = Json_create(szBuffer);
        CCAssert(pRootJson, "invalid data");
        
        Json* pSuccessJson = Json_getItem(pRootJson, "success");
        CC_BREAK_IF(pSuccessJson->valueint != 1);
        
        /* 清除战场所有数据 */
        pBattle->clearAllBattleData(true);
        pBattle->clearAllHerosData();
        
        isSuccess = true;
    } while (0);
    
    if (!isSuccess && pRootJson) {
        Json* pMsgJson = Json_getItem(pRootJson, "msg");
        CCLog("error msg = %s", pMsgJson->valuestring);
        MessageBox::getInstance()->messageTips(pMsgJson->valuestring);
    }
    if (pRootJson) {
        // 释放资源
        Json_dispose(pRootJson);
        pRootJson = NULL;
    }
    
    return isSuccess;
}

/*
 *      处理服务器返回的领取铜雀台的数据
 */
static bool processAwardTQT(const char *szBuffer) {
    CCAssert(szBuffer, "invalid data");
    //    CCLog("铜雀台奖励 = %s", szBuffer);
    
    GameState* pGameState = GameState::getInstance();
    GamePlayer* pGamePlayer = GamePlayer::getInstance();
    bool isSuccess = false;
    Json* pRootJson = NULL;
    do {
        pRootJson = Json_create(szBuffer);
        CCAssert(pRootJson, "invalid data");
        
        Json* pSuccessJson = Json_getItem(pRootJson, "success");
        CC_BREAK_IF(pSuccessJson->valueint != 1);
        
        Json* pResultJson = Json_getItem(pRootJson, "result");
        Json* pRestime = Json_getItem(pResultJson, "rest_time");
        Json* pNewMoney = Json_getItem(pResultJson, "new_money");
        
        pGamePlayer->setMoney(pNewMoney->valueint);
        pGameState->appendTimerTask(TIME_TQT, pRestime->valueint);
        
        isSuccess = true;
    } while (0);
    
    if (!isSuccess && pRootJson) {
        Json* pMsgJson = Json_getItem(pRootJson, "msg");
        CCLog("error msg = %s", pMsgJson->valuestring);
        MessageBox::getInstance()->messageTips(pMsgJson->valuestring);
    }
    if (pRootJson) {
        // 释放资源
        Json_dispose(pRootJson);
        pRootJson = NULL;
    }
    
    return isSuccess;
}

/*
 *  处理服务器返回的进入国战的数据
 */
static bool processEnterCwar(const char *szBuffer) {
    CCLog("进入国战 = %s", szBuffer);
    
    GameState* pGameState = GameState::getInstance();
    Battle* pBattle = Battle::getInstance();
    CityManager* pCityManager = CityManager::getInstance();
    bool isSuccess = false;
    Json* pRootJson = NULL;
    do {
        pRootJson = Json_create(szBuffer);
        CCAssert(pRootJson, "invalid data");
        
        Json* pSuccessJson = Json_getItem(pRootJson, "success");
        CC_BREAK_IF(pSuccessJson->valueint != 1);
        
        Json* pResultJson = Json_getItem(pRootJson, "result");
        Json* pInitJson = Json_getItem(pResultJson, "init");
        pGameState->setIsNewUser(pInitJson->valueint == 1);
        
        Json* pStartJson = Json_getItem(pResultJson, "start_time");
        Json* pFightJson = Json_getItem(pResultJson, "fight_times");
        Json* pEndJson = Json_getItem(pResultJson, "end_time");
        Json* pCdJson = Json_getItem(pResultJson, "fight_cd");
        Json* pPrice = Json_getItem(pResultJson, "price");
        
        if (pStartJson)/*国战开始时间*/ {
            pGameState->appendTimerTask(TIME_GZBEGAN, pStartJson->valueint);
            pGameState->setIsBeganGZ(pStartJson->valueint == 0);/*国战还没开始*/
        }
        if (pEndJson)/*国战结束时间*/ {
            pGameState->appendTimerTask(TIME_GZEND, pEndJson->valueint);
        }
        // 战斗冷却时间
        if (pCdJson)
            pBattle->setCwarCd(pCdJson->valueint);
        
        if (pFightJson)
            pBattle->setCwarTimes(pFightJson->valueint);
        
        if (pPrice) {
            Json* pPriceWall = Json_getItem(pPrice, "wall");
            Json* pPriceCar = Json_getItem(pPrice, "car");
            Json* pPriceInspire = Json_getItem(pPrice, "inspire");
            pGameState->setCWarBuildWall(pPriceWall->valueint);
            pGameState->setCWarBuildCar(pPriceCar->valueint);
            pGameState->setCWarInspire(pPriceInspire->valueint);
        }
        
        /*城池数据*/
        Json* pCityDataJson = Json_getItem(pResultJson, "city_data");
        if (pCityDataJson) {
            Json* pCityDataChildJson = pCityDataJson->child;
            while (pCityDataChildJson) {
                CityItem* pCityItem = pCityManager->getCityItem(atoi(pCityDataChildJson->name));
                Json* pGuardNum = Json_getItem(pCityDataChildJson, "guard_num");
                Json* pHolder = Json_getItem(pCityDataChildJson, "hold");
                /*当前城池的守军数量*/
                pCityItem->setGuardNum(pGuardNum->valueint);
                pCityItem->setHolder(pHolder->valueint);/*0,中立，1，魏， 2， 蜀 3，吴*/
                
                Json* pScore = Json_getItem(pCityDataChildJson, "score");
                if (pScore) {
                    Json* pScoreChild = pScore->child;
                    while (pScoreChild) {
                        pCityItem->appendCityScore(pScoreChild->valueint);
                        
                        pScoreChild = pScoreChild->next;
                    }
                }
                
                pCityDataChildJson = pCityDataChildJson->next;
            }
        }
        
        
        isSuccess = true;
    } while (0);
    
    if (!isSuccess && pRootJson) {
        Json* pMsgJson = Json_getItem(pRootJson, "msg");
        CCLog("error msg = %s", pMsgJson->valuestring);
        MessageBox::getInstance()->messageTips(pMsgJson->valuestring);
    }
    if (pRootJson) {
        // 释放资源
        Json_dispose(pRootJson);
        pRootJson = NULL;
    }
    
    return isSuccess;
}

/*
 *  国战进入城池kknd
 */
static bool processEnterCity(const char *szBuffer) {
    CCLog("进入城池 = %s", szBuffer);
    
    Battle* pBattle = Battle::getInstance();
    GameState* pGameState = GameState::getInstance();
    bool isSuccess = false;
    Json* pRootJson = NULL;
    do {
        pRootJson = Json_create(szBuffer);
        CCAssert(pRootJson, "invalid data");
        
        Json* pSuccessJson = Json_getItem(pRootJson, "success");
        CC_BREAK_IF(pSuccessJson->valueint != 1);
        
        Json* pResultJson = Json_getItem(pRootJson, "result");
        Json* pTypeJson = Json_getItem(pResultJson, "type");
        Json* pStartTimeJson = Json_getItem(pResultJson, "start_time");
        Json* pEndTimeJson = Json_getItem(pResultJson, "end_time");
        Json* pFightCdJson = Json_getItem(pResultJson, "fight_cd");
        Json* pWall = Json_getItem(pResultJson, "wall");
        Json* pCar = Json_getItem(pResultJson, "car");
        Json* pInspire = Json_getItem(pResultJson, "inspire");
        Json* pInList = Json_getItem(pResultJson, "in_list");
        Json* pDefTime = Json_getItem(pResultJson, "def_time");
        Json* pMnyBuff = Json_getItem(pResultJson, "money_buff");
        Json* pDefLevMny = Json_getItem(pResultJson, "def_level_money");
        Json* pDefMaxTime = Json_getItem(pResultJson, "def_max_time");
        
        if (pMnyBuff)
            pBattle->setMnyBuff(pMnyBuff->valuefloat);
        if (pDefLevMny)
            pBattle->setDefLevMny(atoi(pDefLevMny->valuestring));
        if (pDefTime)
            pBattle->setDefTime(pDefTime->valueint);
        
        int remainTimes = pDefMaxTime->valueint - pDefTime->valueint;
        
        pGameState->appendTimerTask(TIME_GZDEF, remainTimes);
        // 战斗冷却时间
        pGameState->appendTimerTask(TIME_GZCOOL, pFightCdJson->valueint);
        
        if (pInList->valueint == 0)/*不在列表中*/ {
            if (pDefMaxTime->valueint == pDefTime->valueint) {
                pGameState->setAwardType(1);
            } else if (pDefTime->valueint != 0 && pDefTime->valueint < pDefMaxTime->valueint)/*踢出*/ {
                pGameState->setAwardType(2);
            } else {
                pGameState->setAwardType(0);
            }
        } else {
            if (pDefTime->valueint == pDefMaxTime->valueint)/*成功*/ {
                pGameState->setAwardType(1);
            } else if (pDefTime->valueint < pDefMaxTime->valueint)/*进行中*/ {
                pGameState->setAwardType(3);
            }
        }
        
        pBattle->setCwarCityType(strcmp("attack", pTypeJson->valuestring) ? 2 : 1);/*2，防守 1，攻击*/
        // qjqj
        if (pStartTimeJson)/*国战开始时间*/ {
            pGameState->appendTimerTask(TIME_GZBEGAN, pStartTimeJson->valueint);
            pGameState->setIsBeganGZ(pStartTimeJson->valueint == 0);/*国战还没开始*/
        }
        if (pEndTimeJson) {
            pGameState->appendTimerTask(TIME_GZEND, pEndTimeJson->valueint);
            pGameState->setIsBeganGZ(pEndTimeJson->valueint != 0);
        }
        
        if (pWall) {
            if (pWall->child) {
                pGameState->setHaveWall(true);
                
                Json* pAtk = Json_getItem(pWall, "atk");
                Json* pHp = Json_getItem(pWall, "hp");
                Json* pMny = Json_getItem(pWall, "money");
                pBattle->setWallAtk(pAtk->valuefloat);
                pBattle->setWallHp(pHp->valuefloat);
                pBattle->setWallMoney(pMny->valuefloat);
            } else {
                pGameState->setHaveWall(false);
            }
        }
        if (pCar) {
            if (pCar->child) {
                pGameState->setHaveCar(true);
                
                Json* pAtk = Json_getItem(pCar, "atk");
                Json* pHp = Json_getItem(pCar, "hp");
                Json* pMny = Json_getItem(pCar, "money");
                pBattle->setAtk(pAtk->valuefloat);
                pBattle->setHp(pHp->valuefloat);
                pBattle->setMoney(pMny->valuefloat);
            } else {
                pGameState->setHaveCar(false);
            }
        }
        if (pInspire) {
            if (pInspire->child) {
                pGameState->setHaveInspire(true);
                
                Json* pAtk = Json_getItem(pInspire, "atk");
                Json* pHp = Json_getItem(pInspire, "hp");
                Json* pMny = Json_getItem(pInspire, "money");
                pBattle->setInspireAtk(pAtk->valuefloat);
                pBattle->setInspireHp(pHp->valuefloat);
                pBattle->setInspireMny(pMny->valuefloat);
            } else {
                pGameState->setHaveInspire(false);
            }
        }

        isSuccess = true;
    } while (0);
    
    if (!isSuccess && pRootJson) {
        Json* pMsgJson = Json_getItem(pRootJson, "msg");
        CCLog("error msg = %s", pMsgJson->valuestring);
        MessageBox::getInstance()->messageTips(pMsgJson->valuestring);
    }
    if (pRootJson) {
        // 释放资源
        Json_dispose(pRootJson);
        pRootJson = NULL;
    }
    
    return isSuccess;
}

/*
 *      国战选择阵营
 */
static bool processChosCampForCwar(const char *szBuffer) {
    CCLog("国战选择阵营 = %s", szBuffer);
    
    GamePlayer* pGamePlayer = GamePlayer::getInstance();
    bool isSuccess = false;
    Json* pRootJson = NULL;
    do {
        pRootJson = Json_create(szBuffer);
        CCAssert(pRootJson, "invalid data");
        
        Json* pSuccessJson = Json_getItem(pRootJson, "success");
        CC_BREAK_IF(pSuccessJson->valueint != 1);
        
        Json* pResultJson = Json_getItem(pRootJson, "result");
        Json* pGroupJson = Json_getItem(pResultJson, "group");
        if (pGroupJson->valueint == 1) {
            pGamePlayer->setPlayerOfCamp(PLAYERCAMP_WEI);
        } else if (pGroupJson->valueint == 2) {
            pGamePlayer->setPlayerOfCamp(PLAYERCAMP_SHU);
        } else if (pGroupJson->valueint == 3) {
            pGamePlayer->setPlayerOfCamp(PLAYERCAMP_WU);
        } else {
            pGamePlayer->setPlayerOfCamp(PLAYERCAMP_NONE);
        }
        
        isSuccess = true;
    } while (0);
    
    if (!isSuccess && pRootJson) {
        Json* pMsgJson = Json_getItem(pRootJson, "msg");
        CCLog("error msg = %s", pMsgJson->valuestring);
        MessageBox::getInstance()->messageTips(pMsgJson->valuestring);
    }
    if (pRootJson) {
        // 释放资源
        Json_dispose(pRootJson);
        pRootJson = NULL;
    }
    
    return isSuccess;
}

/*
 *  处理服务器返回的国战建造数据
 */
static bool processBuildForCwar(const char *szBuffer) {
    CCLog("国战建造 = %s", szBuffer);
    
    bool isSuccess = false;
    Json* pRootJson = NULL;
    Battle* pBattle = Battle::getInstance();
    GamePlayer* pGamePlayer = GamePlayer::getInstance();

    do {
        pRootJson = Json_create(szBuffer);
        CCAssert(pRootJson, "invalid data");
        
        Json* pSuccessJson = Json_getItem(pRootJson, "success");
        CC_BREAK_IF(pSuccessJson->valueint != 1);
        
        Json* pResultJson = Json_getItem(pRootJson, "result");
        Json* pWallJson = Json_getItem(pResultJson, "wall");
        Json* pCarJson = Json_getItem(pResultJson, "car");
        Json* pNeedGold = Json_getItem(pResultJson, "gold");
    
        /*玩家建造所需要的GOLD*/
        pGamePlayer->setGold(pNeedGold->valueint);
        if (pWallJson) {
            Json* pAtk = Json_getItem(pWallJson, "atk");
            Json* pHp = Json_getItem(pWallJson, "hp");
            Json* pMoney = Json_getItem(pWallJson, "money");
            if (pAtk)
                pBattle->setWallAtk(pAtk->valuefloat);
            if (pHp)
                pBattle->setWallHp(pHp->valuefloat);
            if (pMoney)
                pBattle->setWallMoney(pMoney->valuefloat);
        }
        if (pCarJson) {
            Json* pAtk = Json_getItem(pCarJson, "atk");
            Json* pHp = Json_getItem(pCarJson, "hp");
            Json* pMoney = Json_getItem(pCarJson, "money");
            if (pAtk)
                pBattle->setAtk(pAtk->valuefloat);
            if (pHp)
                pBattle->setHp(pHp->valuefloat);
            if (pMoney)
                pBattle->setMoney(pMoney->valuefloat);
        }
        
        isSuccess = true;
    } while (0);
    
    
    if (!isSuccess && pRootJson) {
        Json* pMsgJson = Json_getItem(pRootJson, "msg");
        CCLog("error msg = %s", pMsgJson->valuestring);
        MessageBox::getInstance()->messageTips(pMsgJson->valuestring);
    }
    if (pRootJson) {
        // 释放资源
        Json_dispose(pRootJson);
        pRootJson = NULL;
    }
    
    return isSuccess;
}

/*
 *  处理服务器返回的国战鼓舞数据
 */
static bool processGuWuForCwar(const char *szBuffer) {
    CCLog("国战鼓舞数据 = %s", szBuffer);
    
    Battle* pBattle = Battle::getInstance();
    GamePlayer* pGamePlayer = GamePlayer::getInstance();
    bool isSuccess = false;
    Json* pRootJson = NULL;
    do {
        pRootJson = Json_create(szBuffer);
        CCAssert(pRootJson, "invalid data");
        
        Json* pSuccessJson = Json_getItem(pRootJson, "success");
        CC_BREAK_IF(pSuccessJson->valueint != 1);
        
        Json* pResultJson = Json_getItem(pRootJson, "result");
        Json* pInspire = Json_getItem(pResultJson, "inspire");
        Json* pGold = Json_getItem(pResultJson, "gold");
        if (pInspire) {
            Json* pAtk = Json_getItem(pInspire, "atk");
            Json* pHp = Json_getItem(pInspire, "hp");
            Json* pMny = Json_getItem(pInspire, "money");
            
            pBattle->setInspireAtk(pAtk->valuefloat);
            pBattle->setInspireHp(pHp->valuefloat);
            pBattle->setInspireMny(pMny->valuefloat);
        }
        pGamePlayer->setGold(pGold->valueint);
        
        isSuccess = true;
    } while (0);
    
    if (!isSuccess && pRootJson) {
        Json* pMsgJson = Json_getItem(pRootJson, "msg");
        CCLog("error msg = %s", pMsgJson->valuestring);
        MessageBox::getInstance()->messageTips(pMsgJson->valuestring);
    }
    if (pRootJson) {
        // 释放资源
        Json_dispose(pRootJson);
        pRootJson = NULL;
    }
    
    return isSuccess;
}

/*
 *  处理服务器返回的国战战斗冷却数据
 */
static bool processCoolForCwar(const char *szBuffer) {
    CCLog("国战战斗冷却 = %s", szBuffer);
    
    bool isSuccess = false;
    Json* pRootJson = NULL;
    do {
        pRootJson = Json_create(szBuffer);
        CCAssert(pRootJson, "invalid data");
        
        Json* pSuccessJson = Json_getItem(pRootJson, "success");
        CC_BREAK_IF(pSuccessJson->valueint != 1);
        
//        Json* pResultJson = Json_getItem(pRootJson, "result");
//        Json* pWallJson = Json_getItem(pResultJson, "wall");
        
        isSuccess = true;
    } while (0);
    
    if (!isSuccess && pRootJson) {
        Json* pMsgJson = Json_getItem(pRootJson, "msg");
        CCLog("error msg = %s", pMsgJson->valuestring);
        MessageBox::getInstance()->messageTips(pMsgJson->valuestring);
    }
    if (pRootJson) {
        // 释放资源
        Json_dispose(pRootJson);
        pRootJson = NULL;
    }
    
    return isSuccess;
}
/*
 *  处理服务器返回的国战领取奖励
 */
static bool processCWarAward(const char *szBuffer) {
    CCLog("国战领取奖励 = %s", szBuffer);
    
    bool isSuccess = false;
    Json* pRootJson = NULL;
    do {
        pRootJson = Json_create(szBuffer);
        CCAssert(pRootJson, "invalid data");
        
        Json* pSuccessJson = Json_getItem(pRootJson, "success");
        CC_BREAK_IF(pSuccessJson->valueint != 1);
        
//        Json* pResultJson = Json_getItem(pRootJson, "result");
        
        
        isSuccess = true;
    } while (0);
    
    if (!isSuccess && pRootJson) {
        Json* pMsgJson = Json_getItem(pRootJson, "msg");
        CCLog("error msg = %s", pMsgJson->valuestring);
        MessageBox::getInstance()->messageTips(pMsgJson->valuestring);
    }
    if (pRootJson) {
        // 释放资源
        Json_dispose(pRootJson);
        pRootJson = NULL;
    }
    
    return isSuccess;
}




/*
 * 功能：
 *      解析从服务器接收的缓冲区数据
 * 参数：
 *      szBuffer        缓冲区数据
 *      bufferSize      缓冲区大小
 *      szTag           http 请求 tag 值
 * 返回：
 *      成功返回 true，失败返回 false
 */
bool ProcessData::parseBufferFromSvr(const char szBuffer[], const unsigned int bufferSize, char szTag[]) {
    bool isSuccess = false;
    do {
        CC_BREAK_IF(NULL == szBuffer || NULL == szTag);
        
        if (!strcmp(szTag, HTTP_REQUEST_SETHEROTEAM_TAG))           /* 更新上阵武将 pve*/ {
            isSuccess = processGoIntoBattle(szBuffer);
        } else if (!strcmp(szTag, HTTP_REQUEST_SETHEROTEAM_PVP_TAG)) {
            isSuccess = processGoIntoBattlePvp(szBuffer);
        } else if (!strcmp(szTag, HTTP_REQUEST_LEVUPHERO_TAG))      /* 武将升级 */ {
            isSuccess = processHeroLevUp(szBuffer);
        } else if (!strcmp(szTag, HTTP_REQUEST_EVOUPHERO_TAG))      /* 武将升阶 */ {
            isSuccess = processHeroEvoUp(szBuffer);
        } else if (!strcmp(szTag, HTTP_REQUEST_FIGHT_PVE_TAG))      /* pve 武将出征 */ {
            isSuccess = processHeroBattle(szBuffer);
        } else if (!strcmp(szTag, HTTP_REQUEST_FIGHT_TQT_TAG))      /* pvp(铜雀台) 武将出征 */ {
            isSuccess = processTqtBattle(szBuffer);
        } else if (!strcmp(szTag, HTTP_REQUEST_YIFREE_TAG))         /* 免费抽取武将 */{
            isSuccess = processFree(szBuffer);
        } else if (!strcmp(szTag, HTTP_REQUEST_YINFREE_TAG))        /* 付费抽1次*/{
            isSuccess = processNFree(szBuffer);
        } else if (!strcmp(szTag, HTTP_REQUEST_YINFREETEN_TAG))     /* 付费抽10次 */{
            isSuccess = processNFreeTen(szBuffer);
        } else if (!strcmp(szTag, HTTP_REQUEST_SELLHERO_TAG))       /* 武将出售 */ {
            isSuccess = processHeroSell(szBuffer);
        } else if (!strcmp(szTag, HTTP_REQUEST_BUYENERGY_TAG))      /* 体力购买 */ {
            isSuccess = processBuyEnergy(szBuffer);
        } else if (!strcmp(szTag, HTTP_REQUEST_HEROLOCK_TAG))       /* 上锁武将 */ {
            isSuccess = processLockHero(szBuffer);
        } else if (!strcmp(szTag, HTTP_REQUEST_JIANYUINIT_TAG))     /* 请求监狱系统 */ {
            isSuccess = processForInitJy(szBuffer);
        } else if (!strcmp(szTag, HTTP_FREE_HERO_TAG))              /* 释放监狱武将 */ {
            isSuccess = processForFreeJy(szBuffer);
        } else if (!strcmp(szTag, HTTP_ZHAOXIANG_HERO_TAG))         /* 招降监狱武将 */ {
            isSuccess = processForZhaoXJy(szBuffer);
        } else if (!strcmp(szTag, HTTP_ENTER_TONGQUETAI))           /* 进入铜雀台 */ {
            isSuccess = processEnterTQT(szBuffer);
        } else if (!strcmp(szTag, HTTP_TONGQUETAI_AWARD))           /* 铜雀台奖励领取 */ {
            isSuccess = processAwardTQT(szBuffer);
        } else if (!strcmp(szTag, HTTP_TONGQUETAI_REPLAY))          /* 铜雀台战斗回放 */ {
            isSuccess = processTQTReplay(szBuffer);
        } else if (!strcmp(szTag, HTTP_REQUEST_ENTERCWAR_TAG))      /* 进入国战 */ {
            isSuccess = processEnterCwar(szBuffer);
        } else if (!strcmp(szTag, HTTP_REQUEST_ENTERCITY_TAG))      /* 进入城池 */ {
            isSuccess = processEnterCity(szBuffer);
        } else if (!strcmp(szTag, HTTP_REQUEST_CHOSCAMP_TAG))       /* 选择阵营 */ {
            isSuccess = processChosCampForCwar(szBuffer);
        } else if (!strcmp(szTag, HTTP_REQUEST_BUILD_TAG))          /* 国战建造 */ {
            isSuccess = processBuildForCwar(szBuffer);
        } else if (!strcmp(szTag, HTTP_REQUEST_GUWU_TAG))           /* 国战鼓舞 */ {
            isSuccess = processGuWuForCwar(szBuffer);
        } else if (!strcmp(szTag, HTTP_REQUEST_KILLCD_TAG))         /* 国战战斗冷却 */ {
            isSuccess = processCoolForCwar(szBuffer);
        } else if (!strcmp(szTag, HTTP_REQUEST_FIGHT_CWAR_TAG))     /* pvp(国战)武将出征 */ {
            isSuccess = processCWarBattle(szBuffer);
        } else if (!strcmp(szTag, HTTP_REQUEST_REWARD_TAG))         /* pvp(国战)领取奖励 */ {
            isSuccess = processCWarAward(szBuffer);
        }
        
    } while (0);
    
    return isSuccess;
}

