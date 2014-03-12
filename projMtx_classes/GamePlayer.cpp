//
//  GamePlayer.cpp
//  hero
//
//  Created by ; on 2013/12/05.
//
//

//#include <stdlib.h>
#include "GamePlayer.h"
#include "Json.h"

//USING_NS_CC;

#define EXPFORLEVUP_JSON_FILE           "char.json"

// 静态变量
static GamePlayer* s_GamePlayer = NULL;

/*
 * 单例模式，获取 GamePlayer 类唯一实例对象
 */
GamePlayer* GamePlayer::getInstance() {
    if (NULL == s_GamePlayer) {
        s_GamePlayer = new GamePlayer();
    }
    return s_GamePlayer;
}

/*
 *  构造
 */
GamePlayer::GamePlayer() {
    m_login = false;
    m_uid = "";
    m_name = "";
    m_level = 0;
    m_exp = 0;
    m_money = 0;
    m_gold = 0;
    m_energy = 0;
    m_maxenergy = 0;
    m_energyTime = 0;
    m_rate = 0;
    m_newStatus = 0;
    m_serverTime = 0;
    m_maxHero = 0;
    m_energycycle = 0;
}

/*
 *  析构
 */
GamePlayer::~GamePlayer() {
    m_ItemsAboutPlayerLevUp.clear();
}

/*更新玩家信息*/
void GamePlayer::updateFromSvr(const char* pData){
    do {
        CC_BREAK_IF(pData);
        
        /*json 解析 */
        
        
    } while (0);
}

/* 
 *  加载本地玩家当前等级升级所需经验值数据 
 */
void GamePlayer::loadExpForLevUp() {
    m_ItemsAboutPlayerLevUp.clear();
    
    std::string path = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(EXPFORLEVUP_JSON_FILE);
    unsigned long size;
    char *content = (char*)cocos2d::CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "r", &size);
    CCAssert(content && size > 0, "invalid file");
    
   std::string text;
   text.assign(content, size);
   text += '\0';
   delete [] content;
   
   Json* pRootJson = Json_create(text.c_str());
   CCAssert(pRootJson, "invalid json file");
   
    Json* pCharJson = Json_getItem(pRootJson, "char");
    if (pCharJson) {
        Json* pCharItemJson = pCharJson->child; //取数组的孩子
           
        while (pCharItemJson) {
            ItemAboutPlayerLevUp itemAboutPlayerLevUp;
               
            Json* pLev = Json_getItem(pCharItemJson, "lv");
            CCAssert(pLev, "invalid level");
            itemAboutPlayerLevUp.level = atoi(pLev->valuestring);

            Json* pExp = Json_getItem(pCharItemJson, "ex");
            CCAssert(pExp, "invalid exp");
            itemAboutPlayerLevUp.expForLevUp = atoi(pExp->valuestring);
               
            m_ItemsAboutPlayerLevUp.insert(std::map<unsigned int,
                ItemAboutPlayerLevUp>::value_type(itemAboutPlayerLevUp.level, itemAboutPlayerLevUp));
               
            pCharItemJson = pCharItemJson->next;
        } /*while*/
    }
    Json_dispose(pRootJson);
}

// 根据等级获得经验
unsigned int GamePlayer::getExpByLev(const unsigned int lev) {
  //如果返回尾部迭代器，说明所给的lev没有找到对应的值--erase删除
    std::map<unsigned int, ItemAboutPlayerLevUp>::iterator iter = m_ItemsAboutPlayerLevUp.find(lev);
    return iter != m_ItemsAboutPlayerLevUp.end() ? (*iter).second.expForLevUp : 0;
}

/*********/
//本地经验
unsigned int GamePlayer::getLocalExp(){return m_localExp;}
void GamePlayer::setLocalExp(unsigned int var) {m_localExp = var;}
//本地等级
unsigned int GamePlayer::getLocalLev(){return m_localLev;}
void GamePlayer::setLocalLev(unsigned int var) {m_localLev = var;}
/*********/
/* 获取是否已登录 */
bool GamePlayer::getLogin() {return m_login;}
/* 设置是否已登录 */
void GamePlayer::setLogin(bool var) {m_login = var;}
/* 获取用户 id */
std::string GamePlayer::getUid() {return m_uid;}
/* 设置用户 id */
void GamePlayer::setUid(std::string var) {m_uid = var;}
/* 获取用户姓名 */
std::string GamePlayer::getName() {return m_name;}
/* 设置用户姓名 */
void GamePlayer::setName(std::string var) {m_name = var;}
/* 获取用户等级 */
unsigned int GamePlayer::getLevel() {return m_level;}
/* 设置用户等级 */
void GamePlayer::setLevel(unsigned int var) {m_level = var;}
/* 获取用户经验 */
unsigned int GamePlayer::getExp() {return m_exp;}
/* 设置用户经验 */
void GamePlayer::setExp(unsigned int var) {m_exp = var;}
/* 获取用户铜钱 */
unsigned int GamePlayer::getMoney() {return m_money;}
/* 设置用户铜钱 */
void GamePlayer::setMoney(unsigned int var) {m_money = var;}
/* 获取用户元宝 */
unsigned int GamePlayer::getGold() {return m_gold;}
/* 设置用户元宝 */
void GamePlayer::setGold(unsigned int var) {m_gold = var;}
/* 获取体力 */
unsigned int GamePlayer::getEnergy() {return m_energy;}
/* 设置用户体力 */
void GamePlayer::setEnergy(unsigned int var) {m_energy = var;}
/*获取最大体力*/
unsigned int GamePlayer::getMaxEnergy() {return m_maxenergy;}
/*存储最大体力*/
void GamePlayer::setMaxEnergy(unsigned int var) {m_maxenergy = var;}
/* 获取体力回复时间 */
unsigned int GamePlayer::getEnergyTime() {return m_energyTime;}
/* 设置体力回复时间 */
void GamePlayer::setEnergyTime(unsigned int var) {m_energyTime = var;}
/* 获取战斗力 */
unsigned int GamePlayer::getRate() {return m_rate;}
/* 设置战斗力 */
void GamePlayer::setRate(unsigned int var) {m_rate = var;}
/* 获取新手引导状态 */
unsigned int GamePlayer::getNewStatus() {return m_newStatus;}
/* 设置新手引导状态 */
void GamePlayer::setNewStatus(unsigned int var) {m_newStatus = var;}
/* 获取 server time */
unsigned long GamePlayer::getServerTime() {return m_serverTime;}
/* 设置 server time */
void GamePlayer::setServerTime(unsigned long var) {m_serverTime = var;}
/* 获取 max hero 是玩家背包的上限*/
unsigned int GamePlayer::getMaxHero(){return m_maxHero;}
/* 设置 max hero*/
void GamePlayer::setMaxHero(unsigned int var){m_maxHero = var;}
/* 获取 energy_cycle*/
unsigned int GamePlayer::getEnergyCycle(){return m_energycycle;}
/*设置 energy_cycle*/
void GamePlayer::setEnergyCycle(unsigned int var){m_energycycle = var;}
// /*获取当前等级升级所需要经验*/
// unsigned int GamePlayer::getexpForLevUp(){return m_expForLevUp;}
// 设置当前等级升级所需要经验
// void GamePlayer::setexpForLevUp(unsigned int var){m_expForLevUp = var;}


/*玩家拥有的紫色和蓝色材料*/
unsigned int GamePlayer::getPropZiNum() {return m_propZiNum;}
void GamePlayer::setPropZiNum(unsigned int var){m_propZiNum = var;}
unsigned int GamePlayer::getPropLanNum() {return m_propLanNum;}
void GamePlayer::setPropLanNum(unsigned int var){m_propLanNum = var;}


/*获取当前等级最大好友数*/
unsigned int GamePlayer::getFd(){return m_friend;}
/*s设置当前等级最大好友数*/
void GamePlayer::setFd(unsigned int var){m_friend = var;}

/* 玩家免费抽话费勇气值 */
unsigned int GamePlayer::getLuckCourage() {return m_luckCourage;}
void GamePlayer::setLuckCourage(unsigned int var) {
    m_luckCourage = var;
}

/* 玩家付费抽话费勇气值 */
unsigned int GamePlayer::getLuckRMB() {return m_luckRMB;}
void GamePlayer::setLuckRMB(unsigned int var) {
    m_luckRMB = var;
}

/* 玩家付费抽10话费勇气值 */
unsigned int GamePlayer::getLuckRMBBig() {return m_luckRMBBig;}
void GamePlayer::setLuckRMBBig(unsigned int var) {
    m_luckRMBBig = var;
}

/*最大勇气值*/
unsigned int GamePlayer::getCourage() {
    return m_Courage;
}
void GamePlayer::setCourage(unsigned int var) {
    m_Courage = var;
}


// 玩家购买体力次数
void GamePlayer::setEnergyBuy(unsigned int var) {
    m_energyBuy = var;
}

unsigned int GamePlayer::getEnergyBuy() {
    return m_energyBuy;
}
// 玩家购买体力价格
void GamePlayer::setEnergyPrice(unsigned int var) {
    m_energyPrice = var;
}

unsigned int GamePlayer::getEnergyPrice() {
    return m_energyPrice;
}
///////铜雀台
void GamePlayer::setRank(unsigned int var) {
    m_rank = var;
}

unsigned int GamePlayer::getRank() {
    return m_rank;
}
//是否领取了奖励
void GamePlayer::setIsReward(bool var) {
    m_isReward = var;
}
bool GamePlayer::getIsReward() {
    return m_isReward;
}
//领取的铜钱
void GamePlayer::setRewardMoney(unsigned int var) {
    m_rewardMoney = var;
}
unsigned int GamePlayer::getRewardMoney() {
    return m_rewardMoney;
}

// 总人数
void GamePlayer::setTotalRank(unsigned int var) {
    m_totalRank = var;
}
unsigned int GamePlayer::getTotalRank() {
    return m_totalRank;
}


// 蓝色道具系数
void GamePlayer::setPropLanXi(unsigned int var) {
    m_propLanXi = var;
}
unsigned int GamePlayer::getPropLanXi() {
    return m_propLanXi;
}
// 紫色道具系数
void GamePlayer::setPropZiXi(unsigned int var) {
    m_propZiXi = var;
}
unsigned int GamePlayer::getPropZiXi() {
    return m_propLanXi;
}

/***************** 国战相关 *****************/
// 记录当前玩家势力
void GamePlayer::setPlayerOfCamp(playerOfCamp var) {
    m_playerOfCamp = var;
}
playerOfCamp GamePlayer::getPlayerOfCamp() {
    return m_playerOfCamp;
}
void GamePlayer::setOwnCity(unsigned int var) {
    m_ownCity = var;
}
unsigned int GamePlayer::getOwnCity() {
    return m_ownCity;
}
