//
//  herosManager.cpp
//  hero
//
//  Created by yangjie on 2013/12/03.
//
//

#include "HeroManager.h"
#include "HeroItem.h"
#include "HeroAdditionItem.h"
#include "HeroOfPlayerItem.h"
#include "HeroFhpItem.h"
#include "MosterItem.h"
#include "PrisonHeroItem.h"
#include "LoyaltyItem.h"
#include "TqHeadItem.h"
#include "GameState.h"

#define MODE_LIST                        0               // 武将浏览列表排序规则,不加参数为默认浏览模式
#define MODE_SELLANDTUN                  1               // 武将出售和被吞噬排序规则

/* 静态实例变量 */
static HeroManager* s_HeroManager = NULL;

/*
 * 单例模式，获取 HeroManager 类唯一实例对象
 */
HeroManager* HeroManager::getInstance() {
    if (NULL == s_HeroManager) {
        s_HeroManager = new HeroManager();
    }
    return s_HeroManager;
}

/*
 * 构造函数
 */
HeroManager::HeroManager():
m_herosOfCamp(CAMP_ALL),
m_isExceptLock(false),
m_isExceptGoIntoBattle(false)
{
    m_AllHerosByDict = CCDictionary::create();
    m_AllHerosByDict->retain();

    m_HerosOfPlayerByDict = CCDictionary::create();
    m_HerosOfPlayerByDict->retain();
    
    m_HerosOfPlayerByArray = CCArray::createWithCapacity(HEROS_OF_PLAYER_CONTAINER_INIT_CAPACTIY);
    m_HerosOfPlayerByArray->retain();
    
    m_PrisonHerosByArray = CCArray::createWithCapacity(32);
    m_PrisonHerosByArray->retain();
    
    m_CampHerosOfPlayerByArray = CCArray::createWithCapacity(HEROS_OF_PLAYER_CONTAINER_INIT_CAPACTIY);
    m_CampHerosOfPlayerByArray->retain();

    m_goIntoBattleHerosOfPlayerByArray = CCArray::createWithCapacity(MAX_GO_INTO_BATTLE_HEROS);
    m_goIntoBattleHerosOfPlayerByArray->retain();
    
    m_pvpGoIntoBattleHerosOfPlayerByArray = CCArray::createWithCapacity(MAX_GO_INTO_BATTLE_HEROS);
    m_pvpGoIntoBattleHerosOfPlayerByArray->retain();
    
    m_heroAdditionItems = CCArray::createWithCapacity(MAX_HERO_RARITY * MAX_HERO_LEVEL);
    m_heroAdditionItems->retain();
    
    m_TqtHeadByArray = CCArray::createWithCapacity(3);
    m_TqtHeadByArray->retain();
    
    m_heroFhps = CCDictionary::create();
    m_heroFhps->retain();
    
    m_MostersByDict = CCDictionary::create();
    m_MostersByDict->retain();
    
    m_heroLoyaltys = CCDictionary::create();
    m_heroLoyaltys->retain();
    
    for (int i = CAMP_WU; i < CAMP_ALL; i++) {
        m_hasCamp[i] = false;
    } /*for*/
}

/*
 * 析构函数
 */
HeroManager::~HeroManager() {
    removeAllHeroData(true);
}

/*
 *  功能：
 *      清除所有武将数据
 *  
 *  参数：
 *      isRelease   ->  是否释放资源
 */
void HeroManager::removeAllHeroData(bool isRelease) {
    if (m_AllHerosByDict) {
        m_AllHerosByDict->removeAllObjects();
        if (isRelease) {
            CC_SAFE_RELEASE_NULL(m_AllHerosByDict);
        }
    }
    if (m_HerosOfPlayerByDict) {
        m_HerosOfPlayerByDict->removeAllObjects();
        if (isRelease) {
            CC_SAFE_RELEASE_NULL(m_HerosOfPlayerByDict);
        }
    }
    if (m_HerosOfPlayerByArray) {
        m_HerosOfPlayerByArray->removeAllObjects();
        if (isRelease) {
            CC_SAFE_RELEASE_NULL(m_HerosOfPlayerByArray);
        }
    }
    if (m_PrisonHerosByArray) {
        m_PrisonHerosByArray->removeAllObjects();
        if (isRelease) {
            CC_SAFE_RELEASE_NULL(m_PrisonHerosByArray);
        }
    }
    if (m_CampHerosOfPlayerByArray) {
        m_CampHerosOfPlayerByArray->removeAllObjects();
        if (isRelease) {
            CC_SAFE_RELEASE_NULL(m_CampHerosOfPlayerByArray);
        }
    }
    if (m_goIntoBattleHerosOfPlayerByArray) {
        m_goIntoBattleHerosOfPlayerByArray->removeAllObjects();
        if (isRelease) {
            CC_SAFE_RELEASE_NULL(m_goIntoBattleHerosOfPlayerByArray);
        }
    }
    if (m_pvpGoIntoBattleHerosOfPlayerByArray) {
        m_pvpGoIntoBattleHerosOfPlayerByArray->removeAllObjects();
        if (isRelease) {
            CC_SAFE_RELEASE_NULL(m_pvpGoIntoBattleHerosOfPlayerByArray);
        }
    }
    if (m_heroAdditionItems) {
        m_heroAdditionItems->removeAllObjects();
        if (isRelease) {
            CC_SAFE_RELEASE_NULL(m_heroAdditionItems);
        }
    }
    if (m_heroFhps) {
        m_heroFhps->removeAllObjects();
        if (isRelease) {
            CC_SAFE_RELEASE_NULL(m_heroFhps);
        }
    }
    if (m_MostersByDict) {
        m_MostersByDict->removeAllObjects();
        if (isRelease) {
            CC_SAFE_RELEASE_NULL(m_MostersByDict);
        }
    }
    if (m_heroLoyaltys) {
        m_heroLoyaltys->removeAllObjects();
        if (isRelease) {
            CC_SAFE_RELEASE_NULL(m_heroLoyaltys);
        }
    }
    if (m_TqtHeadByArray) {
        m_TqtHeadByArray->removeAllObjects();
        if (isRelease) {
            CC_SAFE_RELEASE_NULL(m_TqtHeadByArray);
        }
    }
    
    for (int i = CAMP_WU; i < CAMP_ALL; i++) {
        m_hasCamp[i] = false;
    } /*for*/
    
    // 更新武将阵营
    updateCamp();
}

/*
 *  功能：
 *      添加武将数据
 *  参数：
 *      pHeroItem   ->  武将数据项
 */
void HeroManager::appendHeroItem(HeroItem* pHeroItem) {
    CCAssert(pHeroItem, "invalid heroitem");
    m_AllHerosByDict->setObject(pHeroItem, pHeroItem->getId());
}

/* 获取武将数据 */
HeroItem* HeroManager::getHeroItem(const unsigned int id) {
    return (HeroItem*)m_AllHerosByDict->objectForKey(id);
}

/*
 *  功能：
 *      添加武将扩展数据
 *  参数：
 *      pHeroItem   ->  武将数据项
 */
void HeroManager::appenHeroAdditionItem(HeroAdditionItem* pHeroAdditionItem) {
    CCAssert(pHeroAdditionItem, "invalid HeroAdditionItem");
    m_heroAdditionItems->addObject(pHeroAdditionItem);
}

/*
 *  功能：
 *      添加武将 fhp 数据
 *  参数：
 *      pHeroFhpItem    ->  fhp 数据项
 */
void HeroManager::appenHeroFhpItem(HeroFhpItem* pHeroFhpItem) {
    CCAssert(pHeroFhpItem, "invalid HeroAdditionItem");
    m_heroFhps->setObject(pHeroFhpItem, pHeroFhpItem->getRarity());
}

/*
 * 功能：
 *      添加玩家所属武将数据
 * 
 * 参数:
 *      heroItemOfPlayerFromServer      从服务器下载的玩家所属武将数据
 *
 * 返回：
 *      添加成功返回 true，失败返回 false
 */
bool HeroManager::appendHeroOfPlayer(const HeroItemOfPlayerFromServer& heroItemOfPlayerFromServer) {
    CCAssert(m_AllHerosByDict && m_HerosOfPlayerByArray && m_HerosOfPlayerByDict, "invaild data");
    
    bool isAppendSuccess = false;       // 是否添加成功标志
    do {
        // 查找关联武将数据
        HeroItem* heroItem = (HeroItem*)m_AllHerosByDict->objectForKey(heroItemOfPlayerFromServer.id);
        
        // 必须有关联武将数据
        CC_BREAK_IF(!heroItem);
        // 不能有重复的玩家所属武将数据
        CC_BREAK_IF(m_HerosOfPlayerByDict->objectForKey(heroItemOfPlayerFromServer.uniId) != NULL);
        
        // 生成一个玩家所属武将
        HeroOfPlayerItem* pHeroOfPlayerItem = HeroOfPlayerItem::create();
        
        // 设置 "uniid"
        pHeroOfPlayerItem->setUniId(heroItemOfPlayerFromServer.uniId);
        // 设置 "等级"
        pHeroOfPlayerItem->setLevel(heroItemOfPlayerFromServer.level);
        // 设置 "经验"
        pHeroOfPlayerItem->setExp(heroItemOfPlayerFromServer.exp);
        // 设置 "武器id"
        pHeroOfPlayerItem->setWeapon(heroItemOfPlayerFromServer.weapon);
        // 设置 "防具id"
        pHeroOfPlayerItem->setArmor(heroItemOfPlayerFromServer.armor);
        // 设置 "是否上阵"
        pHeroOfPlayerItem->setGoIntoBattle(heroItemOfPlayerFromServer.isGoIntoBattle);
        // 设置 "是否锁定"
        pHeroOfPlayerItem->setLock(heroItemOfPlayerFromServer.isLock);
        // 设置 "关联武将"
        pHeroOfPlayerItem->setHeroItem(heroItem);
        
        // 添加玩家所属武将数据
        m_HerosOfPlayerByDict->setObject((CCObject*)pHeroOfPlayerItem, heroItemOfPlayerFromServer.uniId);
        m_HerosOfPlayerByArray->addObject((CCObject*)pHeroOfPlayerItem);
        
        isAppendSuccess = true;
    } while (0);
    
    return isAppendSuccess;
}

/* 
 *  清除所有监狱武将 
 */
void HeroManager::clearAllPrisonHeroItems() const {
    CCAssert(m_PrisonHerosByArray, "invaild data");
    m_PrisonHerosByArray->removeAllObjects();
}

/* 
 *  功能：
 *      添加监狱武将
 */
void HeroManager::appendPrisonHeroItem(const PrisonHeroItem* pPrisonHeroItem) const {
    CCAssert(m_PrisonHerosByArray && pPrisonHeroItem, "invaild data");
    m_PrisonHerosByArray->addObject((CCObject*)pPrisonHeroItem);
}

/*
 *  功能：
 *      添加武将忠诚度数据
 *  参数：
 *      pLoyaltyItem        要添加的忠诚度数据
 */
void HeroManager::appendLoyaltyItem(LoyaltyItem* pLoyaltyItem) {
    CCAssert(m_heroLoyaltys && pLoyaltyItem, "invalid loyalty data");
    m_heroLoyaltys->setObject(pLoyaltyItem, pLoyaltyItem->getId());
}

/* 
 *  功能：
 *      获取忠诚度数据
 *  参数：
 *      id      忠诚度 id
 *  返回：
 *      成功返回忠诚度数据，失败返回 NULL
 */
LoyaltyItem* HeroManager::getLoyaltyItem(const unsigned int id) const {
    CCAssert(m_heroLoyaltys, "invalid loyalty data");
    return (LoyaltyItem*)m_heroLoyaltys->objectForKey(id);
}

/*
 *  功能：
 *      根据索引号获取监狱武将
 *  参数：
 *      index       索引号
 *  返回：
 *      成功返回监狱武将数据，否则返回 NULL
 */
PrisonHeroItem* HeroManager::getPrisonHeroItemByIndex(const unsigned int index) const {
    CCAssert(m_PrisonHerosByArray, "invaild data");
    
    unsigned int countOfPrisoHeros = m_PrisonHerosByArray->count();
    if (0 == countOfPrisoHeros || index > countOfPrisoHeros - 1)
        return NULL;
    return (PrisonHeroItem*)m_PrisonHerosByArray->objectAtIndex(index);
}

/*
 *  功能：
 *      根据武将id号获取监狱武将
 *  参数：
 *      heroId       武将id
 *  返回：
 *      成功返回监狱武将数据，否则返回 NULL
 */
PrisonHeroItem* HeroManager::getPrisonHeroItemById(const unsigned int heroId) const {
    CCAssert(m_PrisonHerosByArray, "invaild data");
    
    PrisonHeroItem* pRetPrisonHeroItem = NULL;
    CCObject* pItem = NULL;
    CCARRAY_FOREACH(m_PrisonHerosByArray, pItem) {
        PrisonHeroItem* pTempItem = (PrisonHeroItem*)pItem;
        if (pTempItem->getHeroId() == heroId) {
            pRetPrisonHeroItem = pTempItem;
            break;
        }
    } /*for*/
    return pRetPrisonHeroItem;
}

/*
 *  功能：
 *      根据索引号删除监狱武将
 *  参数：
 *      index       索引号
 *  返回：
 *      成功返回true，否则返回 false
 */
bool HeroManager::removePrisonHeroItemByIndex(const unsigned int index) const {
    CCAssert(m_PrisonHerosByArray, "invaild data");
    
    bool isSuccessRemove = false;
    if (index < m_PrisonHerosByArray->count()) {
        m_PrisonHerosByArray->removeObjectAtIndex(index);
        isSuccessRemove = true;
    }
    return isSuccessRemove;
}

/*
 *  功能：
 *      根据武将id 删除监狱武将
 *  参数：
 *      heroId       武将id号
 *  返回：
 *      成功返回true，否则返回 false
 */
bool HeroManager::removePrisonHeroItemById(const unsigned int heroId) const {
    CCAssert(m_PrisonHerosByArray, "invaild data");
    
    CCObject* pItem = NULL;
    PrisonHeroItem* pRemovePrisonHeroItem = NULL;
    PrisonHeroItem* pTempPrisonHeroItem = NULL;
    CCARRAY_FOREACH(m_PrisonHerosByArray, pItem) {
        pTempPrisonHeroItem = (PrisonHeroItem*)pItem;
        if (pTempPrisonHeroItem->getHeroId() == heroId) {
            pRemovePrisonHeroItem = pTempPrisonHeroItem;
            break;
        }
    } /*for*/
    
    if (pRemovePrisonHeroItem) {
        m_PrisonHerosByArray->removeObject(pRemovePrisonHeroItem);
    }
    return pRemovePrisonHeroItem != NULL;
}

/*
 * 功能：
 *      添加怪物数据
 * 参数:
 *      pMosterItem      怪物数据
 */
void HeroManager::appendMosterItem(MosterItem* pMosterItem) {
    CCAssert(m_MostersByDict && pMosterItem, "invaild data");
    m_MostersByDict->setObject(pMosterItem, pMosterItem->getId());
}

/* 
 *  功能：
 *      删除玩家所属武将
 *  参数：
 *      szHeroUniId     要删除武将的 uniid
 *  返回：
 *      删除成功返回 ture，失败返回 false
 */
bool HeroManager::removeHeroOfPlayer(const char szHeroUniId[]) {
    bool isRemoveSuccess = false;       // 是否删除成功
    
    do {
        CC_BREAK_IF(NULL == szHeroUniId);
        
        std::string uniidForRemove = szHeroUniId;
        m_HerosOfPlayerByDict->removeObjectForKey(uniidForRemove);
            
        m_HerosOfPlayerByArray->removeAllObjects();
        CCDictElement* pElement = NULL;
        CCDICT_FOREACH(m_HerosOfPlayerByDict, pElement) {
            m_HerosOfPlayerByArray->addObject(pElement->getObject());
        } /*for*/
        
        isRemoveSuccess = true;
    } while (0);
    
    return isRemoveSuccess;
}

/*
 *  功能：
 *      删除指定 uniid 数组的玩家所属武将
 *  参数：
 *      countOfRemoved  要删除的武将数量
 *      szHeroUniIds    要删除武将 uniid 数组
 *  返回：
 *      删除成功返回 ture，失败返回 false
 */
bool HeroManager::removeHeroOfPlayer(unsigned int countOfRemoved, const char szHeroUniIds[][HERO_UNIID_BUFFER_SIZE]) {
    bool isRemoveSuccess = false;       // 是否
    
    do {
        CC_BREAK_IF(0 == countOfRemoved || NULL == szHeroUniIds);
        
        for (int i = 0; i < countOfRemoved; i++) {
            std::string uniidForRemove = szHeroUniIds[i];
            m_HerosOfPlayerByDict->removeObjectForKey(uniidForRemove);
        } /*for*/
        
        m_HerosOfPlayerByArray->removeAllObjects();
        CCDictElement* pElement = NULL;
        CCDICT_FOREACH(m_HerosOfPlayerByDict, pElement) {
            m_HerosOfPlayerByArray->addObject(pElement->getObject());
        } /*for*/
        
        isRemoveSuccess = true;
    } while (0);
    
    return isRemoveSuccess;
}

/*
 * 功能：
 *      根据索引号获取玩家所属武将信息
 * 参数：
 *      index       索引号
 * 返回：
 *      成功返回指向玩家所属武将数据对象的指针，否则返回 NULL
 */
HeroOfPlayerItem* HeroManager::getHeroDataByIndex(const int index) const {
    CCAssert(m_HerosOfPlayerByArray && m_CampHerosOfPlayerByArray, "invalid hero data");
    
    if (0 == m_HerosOfPlayerByArray->count()) /*没有玩家所属武将数据*/
        return NULL;
    if (index > (m_HerosOfPlayerByArray->count() - 1)) /*索引数据越界*/
        return NULL;
    return (HeroOfPlayerItem*)m_HerosOfPlayerByArray->objectAtIndex(index);
}

/*
 * 功能：
 *      根据稀有度和等级获取武将附加数据
 * 参数：
 *      rarity          稀有度
 *      level           等级
 * 返回：
 *      成功返回指向武将附加数据对象的指针，否则返回 NULL
 */
HeroAdditionItem* HeroManager::getAdditionHeroData(const unsigned int rarity, const unsigned int level) const {
    // CCLog("level = %d", level);
    CCAssert(rarity > 0 && rarity <= MAX_HERO_RARITY, "invalid hero rarity");
    CCAssert(level > 0 && level <= MAX_HERO_LEVEL, "invalid hero level");
    
    if (0 == m_heroAdditionItems->count()) /*没有武将附属数据*/
        return NULL;
    return (HeroAdditionItem*)m_heroAdditionItems->objectAtIndex(
        (rarity - 1) * MAX_HERO_LEVEL + level - 1);
}

/*
 * 功能：
 *      根据索引号获取玩家所属”当前阵营“中的武将信息
 * 参数：
 *      index       索引号
 * 返回：
 *      成功返回指向玩家所属”当前阵营“中的武将数据对象的指针，否则返回 NULL
 */
HeroOfPlayerItem* HeroManager::getCampHeroDataByIndex(const int index) const {
    CCAssert(m_HerosOfPlayerByArray && m_CampHerosOfPlayerByArray, "invalid hero data");
    
//    CCArray *pArray = (m_herosOfCamp == CAMP_ALL && !m_isExceptLock && !m_isExceptGoIntoBattle)
//        ? m_HerosOfPlayerByArray : m_CampHerosOfPlayerByArray;
    if (0 == m_CampHerosOfPlayerByArray->count()) /*没有玩家所属武将数据*/
        return NULL;
    if (index > (m_CampHerosOfPlayerByArray->count() - 1)) /*索引数据越界*/
        return NULL;
    return (HeroOfPlayerItem*)m_CampHerosOfPlayerByArray->objectAtIndex(index);
}

/*
 * 功能：
 *      根据 uniid 获取玩家所属武将信息
 * 参数：
 *      uniId       武将的 uniid
 * 返回：
 *      成功返回指向玩家所属武将数据对象的指针，否则返回 NULL
 */
HeroOfPlayerItem* HeroManager::getHeroDataByUniId(const std::string& uniId) {
    CCAssert(m_HerosOfPlayerByDict, "invalid data point");
    return (HeroOfPlayerItem*)m_HerosOfPlayerByDict->objectForKey(uniId);
}

/*
 * 功能：
 *      根据 uniid 获取玩家所属武将信息
 * 参数：
 *      uniId       武将的 uniid
 * 返回：
 *      成功返回指向玩家所属武将数据对象的指针，否则返回 NULL
 */
HeroOfPlayerItem* HeroManager::getHeroDataByUniId(const char uniId[]) {
    CCAssert(m_HerosOfPlayerByDict, "invalid data point");

    if (NULL == uniId)
        return NULL;
    std::string strUniId = uniId;
    return (HeroOfPlayerItem*)m_HerosOfPlayerByDict->objectForKey(uniId);
}

/*
 * 功能：
 *      根据 uid 获取怪物数据
 * 参数：
 *      id       怪物 id
 * 返回：
 *      成功返回指向怪物数据对象的指针，否则返回 NULL
 */
MosterItem* HeroManager::getMosterDataById(const unsigned int id) {
    CCAssert(m_HerosOfPlayerByDict, "invalid data point");
    return (MosterItem*)m_MostersByDict->objectForKey(id);
}

/*
 * 功能：
 *      根据索引号获取上阵武将信息(pve)
 * 参数：
 *      index       索引号
 * 返回：
 *      成功返回指向上阵武将数据对象的指针，否则返回 NULL
 */
HeroOfPlayerItem* HeroManager::getGoIntoBattleHeroDataByIndex(const int index) {
    CCAssert(m_HerosOfPlayerByArray && m_goIntoBattleHerosOfPlayerByArray, "invalid data point");
    
    if (0 == m_goIntoBattleHerosOfPlayerByArray->count()) /*没有上阵武将数据*/
        return NULL;
    if (index > (m_goIntoBattleHerosOfPlayerByArray->count() - 1)) /*索引数据越界*/
        return NULL;
    return (HeroOfPlayerItem*)m_goIntoBattleHerosOfPlayerByArray->objectAtIndex(index);
}

/*
 * 功能：
 *      根据索引号获取上阵武将信息(pvp)
 * 参数：
 *      index       索引号
 * 返回：
 *      成功返回指向(pvp)上阵武将数据对象的指针，否则返回 NULL
 */
HeroOfPlayerItem* HeroManager::getPvpGoIntoBattleHeroDataByIndex(const int index) const {
    CCAssert(m_HerosOfPlayerByArray && m_goIntoBattleHerosOfPlayerByArray, "invalid data point");
    
    if (0 == m_pvpGoIntoBattleHerosOfPlayerByArray->count()) /*没有pvp上阵武将数据*/
        return NULL;
    if (index > (m_pvpGoIntoBattleHerosOfPlayerByArray->count() - 1)) /*索引数据越界*/
        return NULL;
    return (HeroOfPlayerItem*)m_pvpGoIntoBattleHerosOfPlayerByArray->objectAtIndex(index);
}

/*
 * 功能：
 *      根据指定 uniid 数组更新 pve 上阵武将数据
 * 参数：
 *      nCount      要更新的上阵武将数量
 *      uniid       上阵武将数组
 */
void HeroManager::updateGoIntoBattleHerosOfPlayer(const unsigned int nCount, std::string uniid[]) {
    CCAssert(m_goIntoBattleHerosOfPlayerByArray && m_HerosOfPlayerByDict , "invalid data point");
    CCAssert(nCount > 0 && nCount <= MAX_GO_INTO_BATTLE_HEROS, "invalid data");
    
    // 首先清除原有 pve 上阵武将数据
    CCObject* item = NULL;
    CCARRAY_FOREACH(m_goIntoBattleHerosOfPlayerByArray, item) {
        ((HeroOfPlayerItem*)item)->setGoIntoBattle(false);
        ((HeroOfPlayerItem*)item)->setPosInBattle(0);
    }
    m_goIntoBattleHerosOfPlayerByArray->removeAllObjects();
    
    // 根据 uniid 数组更新玩家所属武将的 “是否是上阵武将” 属性
    for (unsigned int i = 0; i < nCount; i++) {
        if (!uniid[i].empty() ) {
            HeroOfPlayerItem* pItem = (HeroOfPlayerItem*)m_HerosOfPlayerByDict->objectForKey(uniid[i]);
            CCAssert(pItem, "invaild item");
            pItem->setGoIntoBattle(true);
            m_goIntoBattleHerosOfPlayerByArray->addObject(pItem);
        }
    } /*for*/
    restoreBattleTeamPosIndex();
    saveBattleTeamPosIndex();
}

/*
 * 功能：
 *      根据指定 uniid 数组更新pvp上阵武将数据
 * 参数：
 *      nCount          要更新的pvp上阵武将数量
 *      uniid           pvp上阵武将uniid数组
 *      posInBattle     pvp 上阵武将在战场的位置
 */
void HeroManager::updatePvpGoIntoBattleHerosOfPlayer(const unsigned int nCount,
                                                     std::string uniid[],
                                                     unsigned int posInBattles[]) {
    CCAssert(m_pvpGoIntoBattleHerosOfPlayerByArray && m_HerosOfPlayerByDict , "invalid data point");
    CCAssert(nCount > 0 && nCount <= MAX_GO_INTO_BATTLE_HEROS, "invalid data");
    
    // 首先清除原有pvp上阵武将数据
    CCObject* item = NULL;
    CCARRAY_FOREACH(m_pvpGoIntoBattleHerosOfPlayerByArray, item) {
        ((HeroOfPlayerItem*)item)->setPvpGoIntoBattle(false);
        ((HeroOfPlayerItem*)item)->setPosInBattle(0);
    }
    
    // 根据 uniid 数组更新玩家所属武将的 “是否是上阵武将” 属性
    m_pvpGoIntoBattleHerosOfPlayerByArray->removeAllObjects();
    for (unsigned int i = 0; i < nCount; i++) {
        if (!uniid[i].empty() ) {
            HeroOfPlayerItem* pItem = (HeroOfPlayerItem*)m_HerosOfPlayerByDict->objectForKey(uniid[i]);
            CCAssert(pItem, "invaild item");
            pItem->setPvpGoIntoBattle(true);
            pItem->setPosInBattle(posInBattles[i]);
            m_pvpGoIntoBattleHerosOfPlayerByArray->addObject(pItem);
        }
    } /*for*/
}

/*
 * 功能：
 *      是否存在指定阵营的武将
 * 参数：
 *      camp    阵营
 * 返回:
 *      存在的话返回 true，否则返回 false
 */
bool HeroManager::hasHerosOfCamp(herosOfCamp camp) {
    return m_hasCamp[camp];
}

/*
 *  功能：
 *      指定 uniid 的武将是否是上阵武将(pve)
 *  参数：
 *      uniid   武将 uniid
 *  返回：
 *      是上阵武将返回 true，否则返回 false
 */
bool HeroManager::isGoIntoBattleHero(const std::string& uniid) {
    CCAssert(m_goIntoBattleHerosOfPlayerByArray, "invalild data");
    
    bool isGIB = false;
    CCObject *pObject = NULL;
    CCARRAY_FOREACH(m_goIntoBattleHerosOfPlayerByArray, pObject) {
        if (!strcmp(((HeroOfPlayerItem*)pObject)->getUniId().c_str(), uniid.c_str())) {
            isGIB = true;
            break;
        }
    } /*for*/
    return isGIB;
}

/*
 *  功能：
 *      指定 uniid 的武将是否是上阵武将(pvp)
 *  参数：
 *      uniid   武将 uniid
 *  返回：
 *      是 pvp 上阵武将返回 true，否则返回 false
 */
bool HeroManager::isPvpGoIntoBattleHero(const std::string& uniid) {
    CCAssert(m_pvpGoIntoBattleHerosOfPlayerByArray, "invalild data");
    
    bool isGIB = false;
    CCObject *pObject = NULL;
    CCARRAY_FOREACH(m_pvpGoIntoBattleHerosOfPlayerByArray, pObject) {
        if (!strcmp(((HeroOfPlayerItem*)pObject)->getUniId().c_str(), uniid.c_str())) {
            isGIB = true;
            break;
        }
    } /*for*/
    return isGIB;
}

/*
 *  功能：
 *      指定 uniid 的武将是否队长(pve)
 *  参数：
 *      uniid   武将 uniid
 *  返回：
 *      是队长返回 true，否则返回 false
 */
bool HeroManager::isLeaderHero(const std::string& uniid) {
    CCAssert(m_goIntoBattleHerosOfPlayerByArray, "invalild data");
    
    HeroOfPlayerItem* pHeroOfPlayerItem = (HeroOfPlayerItem*)m_goIntoBattleHerosOfPlayerByArray->objectAtIndex(0);
    CCAssert(pHeroOfPlayerItem, "invalid data");
    
    bool isLeader = false;
    if (!strcmp(pHeroOfPlayerItem->getUniId().c_str(), uniid.c_str())) {
        isLeader = true;
    }
    return isLeader;
}

/*
 *  功能：
 *      指定 uniid 的武将是否队长(pvp)
 *  参数：
 *      uniid   武将 uniid
 *  返回：
 *      是队长返回 true，否则返回 false
 */
bool HeroManager::isPvpLeaderHero(const std::string& uniid) {
    CCAssert(m_pvpGoIntoBattleHerosOfPlayerByArray, "invalild data");
    
    HeroOfPlayerItem* pHeroOfPlayerItem = (HeroOfPlayerItem*)m_pvpGoIntoBattleHerosOfPlayerByArray->objectAtIndex(0);
    CCAssert(pHeroOfPlayerItem, "invalid data");
    
    bool isLeader = false;
    if (!strcmp(pHeroOfPlayerItem->getUniId().c_str(), uniid.c_str())) {
        isLeader = true;
    }
    return isLeader;
}

/*
 * 功能：
 *      更新 "阵营是否有武将"
 */
void HeroManager::updateCamp() {
    for (int i = CAMP_OTHER; i < CAMP_ALL; i++) {
        m_hasCamp[i] = false;
    } /*for*/
    
    do {
        CC_BREAK_IF(!m_CampHerosOfPlayerByArray || m_CampHerosOfPlayerByArray->count() == 0);
        
        CCObject *item = NULL;
        CCARRAY_FOREACH(m_CampHerosOfPlayerByArray, item) {
            m_hasCamp[((HeroOfPlayerItem*)item)->getHeroItem()->getCamp()] = true;
        } /*for*/
    } while (0);
}

/*
 *  功能：
 *      设置武将所属阵营(并筛选该阵营武将)
 *  参数：
 *      herosOfCamp             要筛选的阵营
 *      isExceptLock            是否排除锁定的武将
 *      isExceptGoIntoBattle    是否排除上阵的武将
 */
void HeroManager::setCamp(const herosOfCamp camp,
             const bool isExceptLock, const bool isExceptGoIntoBattle)
{
    CCAssert(m_HerosOfPlayerByArray && m_HerosOfPlayerByArray, "invaild hero data");
    do {
        m_CampHerosOfPlayerByArray->removeAllObjects();
        m_herosOfCamp = camp;
        m_isExceptLock = isExceptLock;
        m_isExceptGoIntoBattle = isExceptGoIntoBattle;
        
        CCObject *item = NULL;
        HeroOfPlayerItem* pHeroItem = NULL;
        CCARRAY_FOREACH(m_HerosOfPlayerByArray, item) {
            pHeroItem = (HeroOfPlayerItem*)item;
            if (isExceptLock && pHeroItem->getLock()) {
                continue;
            }
            if (isExceptGoIntoBattle && isGoIntoBattleHero(pHeroItem->getUniId())) {
                continue;
            }
            if (CAMP_ALL == camp) {
                m_CampHerosOfPlayerByArray->addObject(pHeroItem);
            } else {
                if (pHeroItem->getHeroItem()->getCamp() == m_herosOfCamp) {
                    m_CampHerosOfPlayerByArray->addObject(pHeroItem);
                }
            }
        } /*for*/
        
        // 排序
        if (m_CampHerosOfPlayerByArray->count() > 1) {
            int tag = GameState::getInstance()->getCurrTagWhoCallHeroSel();
            
            if (tag == WCH_SALE || tag == WCH_FORFOOD) {
                std::sort(m_CampHerosOfPlayerByArray->data->arr,
                      m_CampHerosOfPlayerByArray->data->arr + m_CampHerosOfPlayerByArray->data->num,
                      HeroOfPlayerItem::lessSellAndTun);
            } else if (tag == WCH_CAPTAIN_PVP || tag == WCH_TEAMER_PVP) {
                std::sort(m_CampHerosOfPlayerByArray->data->arr,
                          m_CampHerosOfPlayerByArray->data->arr + m_CampHerosOfPlayerByArray->data->num,
                          HeroOfPlayerItem::lessForPvp);
            } else {
                std::sort(m_CampHerosOfPlayerByArray->data->arr,
                          m_CampHerosOfPlayerByArray->data->arr + m_CampHerosOfPlayerByArray->data->num,
                          HeroOfPlayerItem::less);
            }
        }
    } while (0);
}

//
void HeroManager::sortPrisonHeros() {
    // 排序
    std::sort(m_PrisonHerosByArray->data->arr,
              m_PrisonHerosByArray->data->arr + m_PrisonHerosByArray->data->num,
              PrisonHeroItem::lessForPrison);
}

/*
 *  功能：
 *      计算可拥有士兵数量
 *  参数：
 *      rarity      稀有度
 *      job         职业
 *      hp          生命值
 *  返回：
 *      成功返回可拥有士兵数量
 */
unsigned int HeroManager::getCountOfSoldiers(const unsigned int rarity,
                const unsigned int job, const unsigned int hp ) {
    CCAssert(m_heroFhps, "invalid data");
    
    HeroFhpItem* pHeroFhpItem = (HeroFhpItem*)m_heroFhps->objectForKey(rarity);
    CCAssert(pHeroFhpItem, "invaild data");
    
    unsigned int countOfSoldiers = 0;
    switch (job) {
        case 1: // 1:步兵
            countOfSoldiers = (hp - hp % pHeroFhpItem->getB()) / pHeroFhpItem->getB() +
                (hp % pHeroFhpItem->getB() > 0 ? 1 : 0);
            break;
        case 2: // 2:骑兵
            countOfSoldiers = (hp - hp % pHeroFhpItem->getQ()) / pHeroFhpItem->getQ() +
            (hp % pHeroFhpItem->getQ() > 0 ? 1 : 0);
            break;
        case 3: // 3:弓兵
            countOfSoldiers = (hp - hp % pHeroFhpItem->getG()) / pHeroFhpItem->getG() +
            (hp % pHeroFhpItem->getG() > 0 ? 1 : 0);
            break;
        case 4: // 4:医师
            countOfSoldiers = (hp - hp % pHeroFhpItem->getY()) / pHeroFhpItem->getY() +
            (hp % pHeroFhpItem->getY() > 0 ? 1 : 0);
            break;
        case 5: // 5:谋士
            countOfSoldiers = (hp - hp % pHeroFhpItem->getM()) / pHeroFhpItem->getM() +
            (hp % pHeroFhpItem->getM() > 0 ? 1 : 0);
            break;
        case 6: // 6:策士
            countOfSoldiers = (hp - hp % pHeroFhpItem->getC()) / pHeroFhpItem->getC() +
            (hp % pHeroFhpItem->getC() > 0 ? 1 : 0);
            break;
        case 7: // 7:都尉
            countOfSoldiers = (hp - hp % pHeroFhpItem->getD()) / pHeroFhpItem->getD() +
            (hp % pHeroFhpItem->getD() > 0 ? 1 : 0);
            break;
        default:
            CCAssert(false, "invalid data");
            break;
    }
    return countOfSoldiers  > MAX_IN_BATTLE_SOLDIERS ? MAX_IN_BATTLE_SOLDIERS : countOfSoldiers;
}

/*
 *  功能：
 *      保存上阵武将的战队位置
 */
void HeroManager::saveBattleTeamPosIndex() {
    CCAssert(m_goIntoBattleHerosOfPlayerByArray, "invalid data");
    
    CCUserDefault* pUserDefault = CCUserDefault::sharedUserDefault();
    char saveKey[32];
    unsigned int posIndexInBattle = 0;
    for (unsigned int i = 0; i < MAX_GO_INTO_BATTLE_HEROS; i++) {
        if (i < m_goIntoBattleHerosOfPlayerByArray->count()) {
            posIndexInBattle = ((HeroOfPlayerItem*)m_goIntoBattleHerosOfPlayerByArray->objectAtIndex(i))->getPosInBattle();
        } else {
            posIndexInBattle = 0;
        }
        sprintf(saveKey, "%s%d%d", GOTO_BATTLE_TEAM_POS_KEY, i + 1, GameState::getInstance()->getServerId());
        pUserDefault->setIntegerForKey(saveKey, posIndexInBattle);
    } /*for*/
    pUserDefault->flush();
}

/* 
 *  恢复上阵武将的作战位置
 */
void HeroManager::restoreBattleTeamPosIndex() {
    CCAssert(m_goIntoBattleHerosOfPlayerByArray, "invalid data");
    
    CCUserDefault* pUserDefault = CCUserDefault::sharedUserDefault();
    char saveKey[32];
    unsigned int posIndexInBattle = 0;
    unsigned int nCountOfGotoBattleHeros = m_goIntoBattleHerosOfPlayerByArray->count();
    
    for (unsigned int i = 0; i < MAX_GO_INTO_BATTLE_HEROS; i++) {
        sprintf(saveKey, "%s%d%d", GOTO_BATTLE_TEAM_POS_KEY, i + 1, GameState::getInstance()->getServerId());
        posIndexInBattle = pUserDefault->getIntegerForKey(saveKey, 0);
        
        if (i < nCountOfGotoBattleHeros) {
            if (posIndexInBattle > 0) {
                ((HeroOfPlayerItem*)m_goIntoBattleHerosOfPlayerByArray->objectAtIndex(i))->setPosInBattle(posIndexInBattle);
            }
        }
    } /*for*/
}

/* 
 *  删除所有铜雀台头像
 */
void HeroManager::removeAllTqHeadItems() {
    m_TqtHeadByArray->removeAllObjects();
}

/*
 *  功能：
 *      添加铜雀台头像数据
 *  参数：
 *      pTqHeadItem   ->  铜雀台头像数据项
 */
void HeroManager::appendTqHeadItem(TqHeadItem* pTqHeadItem) {
    CCAssert(m_TqtHeadByArray && pTqHeadItem, "invalid heroitem");
    m_TqtHeadByArray->addObject(pTqHeadItem);
}

/* 
 *  获取铜雀台头像
 */
TqHeadItem* HeroManager::getTqHeadItem(const unsigned int position) {
    CCAssert(position >=1 && position <= 3, "invalid position");
    
    CCObject* pTempItem = NULL;
    TqHeadItem* pTqHeadItem = NULL;
    TqHeadItem* pRetTqHeadItem = NULL;
    
    CCARRAY_FOREACH(m_TqtHeadByArray, pTempItem) {
        pTqHeadItem = (TqHeadItem*)pTempItem;
        if (pTqHeadItem->getPosition() == position) {
            pRetTqHeadItem = pTqHeadItem;
            break;
        }
    } /*for*/
    return pRetTqHeadItem;
}
