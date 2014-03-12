//
//  GameState.cpp
//  hero
//
//  Created by yangjie on 2013/12/15.
//
//


#include "GameState.h"
#include "GamePlayer.h"

static GameState* s_GameState = NULL;

/* 计时线程 */
void* timerThreadFunc(void* para) {
    int rc = 0;
    struct timeval tv;
    
    GameState* pGameState = static_cast<GameState*>(para);
    while (pGameState->isTimer()) {
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        rc = select(0, NULL, NULL, NULL, &tv);
        
        pGameState->timerPerSecondCallBack();
    }
    
    return NULL;
}

/* 获取单例 */
GameState* GameState::getInstance() {
    return NULL == s_GameState ? (s_GameState = new GameState()): s_GameState;
}

/*
 *  构造函数
 */
GameState::GameState():
//m_SelHerosForLevEvoUp(SEL_FOOD)
m_tagWhoCallHeroSel(WCH_NONE)
,m_currWhoCallHeroSel(WCH_NONE)
,m_tagWhoCallMainScene(1)
,m_countOfFoodHeros(0)
,m_hasEndlessFight(false)
,m_getPrisonHero(0)
,m_selectCamp(0)
,m_isNewUser(true)
,m_timer(false)
,m_timerThread(NULL)
,m_pGamePlayer(GamePlayer::getInstance())
{
    m_is1136 = cocos2d::CCDirector::sharedDirector()->getVisibleSize().height > 960;
    m_ufTeamPos.clear();
    
    pthread_mutex_init(&m_timerTaskLock, NULL);
    m_timerTasks.clear();
}

/*
 *  析构函数
 */
GameState::~GameState() {
    m_ufTeamPos.clear();
    m_timerTasks.clear();
    
    m_timer = false;
    pthread_join(m_timerThread, NULL);
    pthread_mutex_destroy(&m_timerTaskLock);
}

/*启动计时线程*/
void GameState::startTimerThread() {
    pthread_attr_t threadAttr;
    pthread_attr_init (&threadAttr);
    pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_JOINABLE);
    pthread_attr_setstacksize(&threadAttr, 16384);
    m_timer = true;
    pthread_create(&m_timerThread, &threadAttr, timerThreadFunc, this);
    pthread_attr_destroy(&threadAttr);
}

/*停止计时线程*/
void GameState::shutdownTimerThread() {
    m_timer = false;
}

/*计时器每秒调用的回调方法*/
void GameState::timerPerSecondCallBack() {
    std::map<unsigned int, int>::iterator iter = m_timerTasks.begin();
    pthread_mutex_lock(&m_timerTaskLock);
    for (; iter != m_timerTasks.end(); iter++) {
        iter->second -= 1;
    }
    pthread_mutex_unlock(&m_timerTaskLock);
}

/*
 *  功能：
 *      添加计时任务
 *  参数：
 *      id              计时任务id
 *      remaintimes     剩余时间(秒)
 */
void GameState::appendTimerTask(const unsigned int id, int remainTimes) {
    pthread_mutex_lock(&m_timerTaskLock);
    m_timerTasks[id] = remainTimes;
    pthread_mutex_unlock(&m_timerTaskLock);
}

/*
 *  功能：
 *      移除计时任务
 *  参数：
 *      id      计时任务id
 */
void GameState::removeTimerTask(const unsigned int id) {
    pthread_mutex_lock(&m_timerTaskLock);
    m_timerTasks.erase(id);
    pthread_mutex_unlock(&m_timerTaskLock);
}

/*
 *  功能：
 *      获取指定计时任务剩余时间
 *  参数：
 *      id      计时任务 id
 */
int GameState::getRemainTimes(const unsigned int id) {
    int remainTimes = 0;
    
    pthread_mutex_lock(&m_timerTaskLock);
    std::map<unsigned int, int>::iterator iter = m_timerTasks.find(id);
    if (iter != m_timerTasks.end()) {
        remainTimes = iter->second;
    }
    pthread_mutex_unlock(&m_timerTaskLock);
    return remainTimes;
}

/* 选择的副本关卡id */
void GameState::setCopyId(unsigned int var) {
    m_id = var;
}
unsigned int GameState::getCopyId() {
    return m_id;
}
/* 当前的副本id */
void GameState::setCopyCurId(unsigned int var) {
    m_curId = var;
}
unsigned int GameState::getCopyCurId() {
    return m_curId;
}
/* 选择的关卡id */
void GameState::setLevId(unsigned int var) {
    m_lid = var;
}
unsigned int GameState::getLevId() {
    return m_lid;
}

/*
 * type
 *      1,勇气寻将
 *      2,人民币寻将
 *      3,人民币寻10将
 */
void GameState::updateSearch(int type) {
    if (type == 1) /*勇气抽奖*/{
        int curCourage = m_pGamePlayer->getCourage() - m_pGamePlayer->getLuckCourage();
        if (curCourage >= 0) {
            m_pGamePlayer->setCourage(curCourage);
        }
    } else if (type == 2) /*人民币抽一次*/{
        int curMoney = m_pGamePlayer->getGold() - m_pGamePlayer->getLuckRMB();
        if (curMoney >= 0) {
            m_pGamePlayer->setGold(curMoney);
        }
    } else if (type == 3) /*人民币抽10次*/{
        int curMoney = m_pGamePlayer->getGold() - m_pGamePlayer->getLuckRMBBig();
        if (curMoney >= 0) {
            m_pGamePlayer->setGold(curMoney);
        }
    }
}

/* 玩家数据信息刷新 */
void GameState::updateForPlayer() {
    
}

//
unsigned int GameState::getFoodHersForLevUp(char szFoodHersForLevUp[][HERO_UNIID_BUFFER_SIZE]) {
    for (int i = 0; i < m_countOfFoodHeros; i++) {
        strcpy(szFoodHersForLevUp[i], m_szFoodHersForLevUp[i]);
    } /*for*/
    
    return m_countOfFoodHeros;
}

//
void GameState::setFoodHersForLevUp(const unsigned int countOfFoodHeros, char szFoodHersForLevUp[][HERO_UNIID_BUFFER_SIZE]) {
    m_countOfFoodHeros = countOfFoodHeros;
    memset(m_szFoodHersForLevUp, '\0', sizeof(m_szFoodHersForLevUp));
    for (int i = 0; i < countOfFoodHeros; i++) {
        strcpy(m_szFoodHersForLevUp[i], szFoodHersForLevUp[i]);
    } /*for*/
}

/* 存储普通副本最大进度.本地维护 */
void GameState::setMaxNorPve(unsigned int var) {
    m_maxNorPve = var;
}

unsigned int GameState::getMaxNorPve() {
    return m_maxNorPve;
}

/*
 *  功能:
 *      设置未结束战斗相关信息
 *  参数:
 *      copyId          未结束战斗副本 id
 *      levelId         未结束战斗关卡 id
 *      roundIndex      未结束战斗战斗波数
 */
void GameState::setEndlessFight(const unsigned int copyId, const unsigned int levelId, const unsigned int roundIndex) {
    m_hasEndlessFight = true;
    
    m_endlessCopyId = copyId;
    m_endlessLevelId = levelId;
    m_endlessRoundIndex = roundIndex;
    
//    cocos2d::CCLog("有未完成的战斗 copyid = %d levelid = %d round = %d", copyId, levelId, roundIndex);
}

void GameState::setNormalMax(const unsigned int copyId, const unsigned int levelId) {
    m_norCopyId = copyId;
    m_norLevId = levelId;
}


// 存储玩家打副本获得经验
void GameState::setCopyForExp(unsigned int var) {
    m_copyForExp = var;
}

unsigned int GameState::getCopyForExp() {
    return m_copyForExp;
}
// 存储玩家打副本获得铜币
void GameState::setCopyForMoney(unsigned int var) {
    m_copyForMoney = var;
}

unsigned int GameState::getCopyForMoney() {
    return m_copyForMoney;
}

// 吞噬所需铜币
void GameState::setTunNeedMoney(unsigned int var) {
    m_tunNeedMoney = var;
}
unsigned int GameState::getTunNeedMoney() {
    return m_tunNeedMoney;
}
// 进化所需铜币
void GameState::setEvoNeedMoney(unsigned int var) {
    m_evoNeedMoney = var;
}
unsigned int GameState::getEvoNeedMoney() {
    return m_evoNeedMoney;
}

/* 添加未完成战斗的队伍索引值 */
void GameState::appendUfTeamPos(std::string uniid, const unsigned int posInBattle) {
    cocos2d::CCLog("tag = %s, posInBattle = %d", uniid.c_str(), posInBattle);
    
    m_ufTeamPos.insert(std::map<std::string, unsigned int>::value_type(uniid, posInBattle));
}

/*
 *  功能：
 *      获取未完成战斗的队伍索引值
 *  参数：
 *      uniid   上阵武将 uniid
 *  返回：
 *      成功返回位置索引，否则返回 0
 */
unsigned int GameState::getUfTeamPos(std::string uniid) {
    unsigned int posInBattle = 0;
    std::map<std::string, unsigned int>::iterator iter = m_ufTeamPos.find(uniid);
    if (iter != m_ufTeamPos.end()) {
        posInBattle = iter->second;
    }
    return posInBattle;
}

/* 清除未完成战斗的队伍索引值 */
void GameState::clearUfTeamPos() {
    m_ufTeamPos.clear();
}

void GameState::setLocalMaxCopy(unsigned int var) {
    m_norCopyId = var;
}

unsigned int GameState::getLocalMaxCopy() {
    return m_norCopyId;
}

void GameState::setLocalMaxLev(unsigned int var) {
    m_norLevId = var;
}

unsigned int GameState::getLocalMaxLev() {
    return m_norLevId;
}


/* 释放成功的武将idx */
void GameState::setFreeIdx(unsigned int var) {m_freeIdx = var;}
unsigned int GameState::getFreeIdx() {return m_freeIdx;}

// 招降是否成功
void GameState::setIsSummonSucess(bool var) {
    m_isSummonSucess = var;
}

bool GameState::getIsSummonSucess() {
    return m_isSummonSucess;
}


void GameState::setSubLoyalty(unsigned int var){
    m_subLoyalty = var;
}
unsigned int GameState::getSubLoyalty() {
    return m_subLoyalty;
}

void GameState::setSucessSummonId(unsigned int var) {
    m_sucessSummonId = var;
}
unsigned int GameState::getSucessSummonId() {
    return m_sucessSummonId;
}
// 是否成功释放
void GameState::setIsSucessShiF(bool var) {
    m_isSucessShiF = var;
}
bool GameState::getIsSucessShiF() {
    return m_isSucessShiF;
}

/*冷却时间*/
void GameState::setCd(unsigned int var) {
    m_cd = var;
}
unsigned int GameState::getCd() {
    return m_cd;
}

// 监狱逻辑//////
void GameState::setGetPrisonHero(unsigned int var) {
    m_getPrisonHero = var;
}
unsigned int GameState::getGetPrisonHero() {
    return m_getPrisonHero;
}

// 1吴国，2蜀国，3魏国
void GameState::setSelectCamp(unsigned int var) {
    m_selectCamp = var;
}

unsigned int GameState::getSelectCamp() {
    return m_selectCamp;
}
// 新老用户
void GameState::setIsNewUser(bool var) {
    m_isNewUser = var;
}
bool GameState::getIsNewUser() {
    return m_isNewUser;
}
// 国战建造
void GameState::setCWarBuildCar(unsigned int var) {
    m_cWarBuildCar = var;
}
unsigned int GameState::getCWarBuildCar() {
    return m_cWarBuildCar;
}
void GameState::setCWarBuildWall(unsigned int var) {
    m_cWarBuildWall = var;
}
unsigned int GameState::getCWarBuildWall() {
    return m_cWarBuildWall;
}
void GameState::setCWarInspire(unsigned int var) {
    m_cWarInspire = var;
}
unsigned int GameState::getCWarInspire() {
    return m_cWarInspire;
}
void GameState::setHaveCar(bool var) {m_haveCar = var;}
bool GameState::getHaveCar() {return m_haveCar;}
void GameState::setHaveWall(bool var) {m_haveWall = var;}
bool GameState::getHaveWall() {return m_haveWall;}
void GameState::setHaveInspire(bool var) {m_haveInspire = var;}
bool GameState::getHaveInspire() {return m_haveInspire;}
/*1，成功，2，失败，3列表中*/
void GameState::setAwardType(unsigned int var) {m_awardType = var;}
unsigned int GameState::getAwardType() {return m_awardType;}

/*暂存购买体力价格*/
void GameState::setTempPrice(unsigned int var) {
    m_tempPrice = var;
}
unsigned int GameState::getTempPrice() {
    return m_tempPrice;
}



