//
//  herosManager.h
//  hero
//
//  Created by yangjie on 2013/11/28.
//
//  武将数据管理
//      （1）所有武将数据
//      （2）玩家所属武将数据
//      （1）玩家所属上阵武将数据
//

#ifndef hero_heroDM_h
#define hero_heroDM_h

#include "cocos2d.h"
#include "common.h"

USING_NS_CC;

class HeroItem;
class HeroAdditionItem;
class HeroOfPlayerItem;
class HeroFhpItem;
class MosterItem;
class PrisonHeroItem;
class LoyaltyItem;
class TqHeadItem;
class HeroManager {
private:    /*禁止外部 new*/
    HeroManager();
    
public:
    virtual ~HeroManager();
    
public:
    // 单例模式，获取本类唯一实例对象
    static HeroManager* getInstance();

public:
    /* 添加武将数据 */
    void appendHeroItem(HeroItem*);

    /* 获取武将数据 */
    HeroItem* getHeroItem(const unsigned int);
    
    /* 添加武将扩展数据 */
    void appenHeroAdditionItem(HeroAdditionItem*);
    
    /* 添加武将 fhp 数据 */
    void appenHeroFhpItem(HeroFhpItem*);
    
    /* 添加玩家所属武将 */
    bool appendHeroOfPlayer(const HeroItemOfPlayerFromServer&);
    
    /* 添加怪物数据 */
    void appendMosterItem(MosterItem*);
    
    /* 清除所有武将数据 */
    void removeAllHeroData(bool isRelease = false);
    
    /* 删除指定 uniid 的玩家所属武将 */
    bool removeHeroOfPlayer(const char[]);
    
    /* 删除指定 uniid 数组的玩家所属武将 */
    bool removeHeroOfPlayer(unsigned int, const char[][HERO_UNIID_BUFFER_SIZE]);

public:
    /* 根据指定 uniid 数组更新上阵武将数据 */
    void updateGoIntoBattleHerosOfPlayer(const unsigned int, std::string []);
    
    /* 根据指定 uniid 数组更新 pvp 上阵武将数据 */
    void updatePvpGoIntoBattleHerosOfPlayer(const unsigned int, std::string [], unsigned int[]);
    
    /* 更新阵营是否有武将 */
    void updateCamp();

    /* 设置武将所属阵营(并筛选该阵营武将) */
    void setCamp(const herosOfCamp, const bool, const bool);
    /* 排序监狱武将 */
    void sortPrisonHeros();

public:
    /* 获取当前玩家所属武将数量 */
    inline unsigned int getHerosOfPlayerCount() const {return m_HerosOfPlayerByArray->count(); }
    
    /* 获取当前玩家所属“当前阵营”中的武将数量 */
    inline unsigned int getCampHerosOfPlayerCount() const {
        return (m_herosOfCamp == CAMP_ALL && !m_isExceptGoIntoBattle && !m_isExceptGoIntoBattle) ?
        m_HerosOfPlayerByArray->count() : m_CampHerosOfPlayerByArray->count();
    }
    
    /* 获取当前玩家拥有的上阵武将数量(pve) */
    inline unsigned int getGoIntoBattleHerosOfPlayerCount() const {return m_goIntoBattleHerosOfPlayerByArray->count();}
    /* 获取当前玩家拥有的上阵武将数量(pvp) */
    inline unsigned int getPvpGoIntoBattleHerosOfPlayerCount() const {return m_pvpGoIntoBattleHerosOfPlayerByArray->count();}
    
    /* 根据索引号获取玩家所属武将(pve) */
    HeroOfPlayerItem* getHeroDataByIndex(const int) const;
    
    /* 根据索引号获取玩家所属武将(pvp) */
    HeroOfPlayerItem* getPvpGoIntoBattleHeroDataByIndex(const int) const;
    
    /* 根据稀有度和等级获取武将附加数据 */
    HeroAdditionItem* getAdditionHeroData(const unsigned int rarity, const unsigned int level) const;
    
    /* 根据索引号获取玩家所属"当前阵营"中的武将*/
    HeroOfPlayerItem* getCampHeroDataByIndex(const int) const;
    
    /* 根据 uniid 获取玩家所属武将信息*/
    HeroOfPlayerItem* getHeroDataByUniId(const std::string&);

    /* 根据 uniid 获取玩家所属武将信息*/
    HeroOfPlayerItem* getHeroDataByUniId(const char[]);
    
    /* 根据索引号获取上阵武将信息*/
    HeroOfPlayerItem* getGoIntoBattleHeroDataByIndex(const int);
    
    /* 根据 id 获取怪物数据 */
    MosterItem* getMosterDataById(const unsigned int);
    
    /* 清除所有监狱武将 */
    void clearAllPrisonHeroItems() const;
    /* 获取监狱武将数量 */
    inline unsigned int getCountOfPrisonHeros() const {return m_PrisonHerosByArray->count();}
    /* 添加监狱武将 */
    void appendPrisonHeroItem(const PrisonHeroItem*) const;
    /* 根据索引号获取监狱武将 */
    PrisonHeroItem* getPrisonHeroItemByIndex(const unsigned int) const;
    /* 根据武将id号获取监狱武将 */
    PrisonHeroItem* getPrisonHeroItemById(const unsigned int) const;
    /* 根据武将索引号清除监狱武将 */
    bool removePrisonHeroItemByIndex(const unsigned int) const;
    /* 根据武将id清除监狱武将 */
    bool removePrisonHeroItemById(const unsigned int) const;
    
    /* 添加武将忠诚度数据 */
    void appendLoyaltyItem(LoyaltyItem*);
    /* 获取忠诚度数据*/
    LoyaltyItem* getLoyaltyItem(const unsigned int) const;
    
    /* 计算可拥有士兵数量 */
    unsigned int getCountOfSoldiers(const unsigned int, const unsigned int, const unsigned int);
    
    /* 是否存在指定阵营的武将 */
    bool hasHerosOfCamp(herosOfCamp);
    
    /* 获取当前阵营 */
    inline herosOfCamp getCamp() const {return m_herosOfCamp;}
    
    /* 指定 uniid 的武将是否是上阵武将 */
    bool isGoIntoBattleHero(const std::string& uniid);

    /* 指定 uniid 的武将是否是 pvp 上阵武将 */
    bool isPvpGoIntoBattleHero(const std::string& uniid);
    
    /* 指定 uniid 的武将是否队长(PVE) */
    bool isLeaderHero(const std::string& uniid);
    /* 指定 uniid 的武将是否队长(PVP) */
    bool isPvpLeaderHero(const std::string& uniid);
    
    /* 删除所有铜雀台头像 */
    void removeAllTqHeadItems();
    /* 添加铜雀台头像 */
    void appendTqHeadItem(TqHeadItem*);
    /* 获取铜雀台头像 */
    TqHeadItem* getTqHeadItem(const unsigned int);
    
    /* 保存上阵武将的作战位置 */
    void saveBattleTeamPosIndex();
    /* 恢复上阵武将的作战位置 */
    void restoreBattleTeamPosIndex();
    
private:
    CCDictionary *m_AllHerosByDict;                 // 所有武将(uid 为 key)
    
    /*
     * 玩家所属武将
     * 
     * 备注：
     */
    CCDictionary* m_HerosOfPlayerByDict;            // 玩家所属所有武将(词典)
    CCArray* m_HerosOfPlayerByArray;                // 玩家所属所有武将(数组)
    CCArray* m_CampHerosOfPlayerByArray;            // 玩家所属指定阵营的武将(数组)
    
    /*
     * 监狱武将
     */
    CCArray* m_PrisonHerosByArray;                  // 监狱武将数据(数组)
    CCDictionary* m_heroLoyaltys;                   // 武将忠诚度
    
    // 铜雀台头像
    CCArray* m_TqtHeadByArray;                      // 
    
    CCArray* m_goIntoBattleHerosOfPlayerByArray;    // 玩家拥有的所有上阵武将(数组)
    CCArray* m_pvpGoIntoBattleHerosOfPlayerByArray; // 玩家拥有的所有 pvp 上阵武将(数组)
    
    CCDictionary* m_MostersByDict;                  // 存放所有怪物
    
    /*
     * 武将上阵带的不同的兵种以及数量
     * 武将稀有度不同，每种职业可带兵的数量也不同
     */
    CCDictionary* m_heroFhps;                       // 武将所带职业兵种key 是稀有度
    
    /*
     *  武将扩展属性
     */
    CCArray* m_heroAdditionItems;
    
    /*
     * 武将阵营
     */
    bool m_hasCamp[CAMP_ALL];                       // 指定阵营是否有武将
    herosOfCamp m_herosOfCamp;                      // 当前阵营
    bool m_isExceptLock;                            // 是否排除锁定的武将
    bool m_isExceptGoIntoBattle;                    // 是否排除上阵武将
};

#endif
