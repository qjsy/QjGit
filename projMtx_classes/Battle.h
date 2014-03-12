//
//  Battle.h
//  hero
//
//  Created by yangjie on 2013/12/22.
//
//  战场相关
//
//  战场位置说明:
//  敌方 1-6 个武将的位置索引顺序是：
//              51 52 53
//              41 42 43
//
//  攻击区域位置索引：
//              31 32 33
//
//  我方 1-6 个武将的位置索引顺序是：
//              21 22 23
//              11 12 13
//

#ifndef __hero__Battle__
#define __hero__Battle__

#include "cocos2d.h"
#include "common.h"

USING_NS_CC;

class HeroOfPlayerItem;
class MosterInBattleItem;
class InBattleItem;
class AttackDataItem;
class BattleReportItem;
class NetConnection;
class GameState;
class GamePlayer;
class Battle {
private:
    Battle();
public:
    virtual ~Battle();
    
    /* 获取 Battle 类唯一实例对象*/
    static Battle* getInstance();
    
    /*******国战逻辑********/
    CC_PROPERTY(unsigned int, m_selectedCity, SelectCity);      // 保存玩家进入的城池id
    
public:
    /* 清除所有上战场的武将数据 */
    void clearAllHerosData(bool isRelease = false);
    /* 清除所有上战场的敌人数据 */
    void clearAllEnemyData();
    
    /* 清除所有战场数据 */
    void clearAllBattleData(const bool clearRound = false, const bool isRelease = false);
    /* 清除所有战斗回合数据 */
    void clearAllAttackData();
    
    /* 增加我方上阵武将 */
    void appendInBattleHero(HeroOfPlayerItem*);
    /* 移除我方上阵武将 */
    void removeInBattleHero(HeroOfPlayerItem*);
    /* 增加敌方 (pvp) */
    void appendInBattleEnemyPvp(InBattleItem*);
    /* 增加敌方怪物 (pve) */
    void appendInBattleEnemyMoster(MosterInBattleItem*);
    /* 增加攻击数据项 */
    void appendAttackDataItem(AttackDataItem*);
    
    /* 根据索引号获取我方武将数据项 */
    HeroOfPlayerItem* getHeroDataByIndex(const unsigned int);
    /* 根据索引号获取敌方数据项 (pvp)*/
    InBattleItem* getEnemyDataByIndex(const unsigned int);
    /* 根据 id 和位置 获取敌方怪物数据项 (pve) */
    MosterInBattleItem* getEnemyMosterDataByIdAndPos(const unsigned int, const unsigned int);
    /* 根据索引号获取敌方武将数据项 (pve)*/
    MosterInBattleItem* getEnemyMosterDataByIndex(const unsigned int);
    /* 获取本方上阵武将的数量 */
    inline unsigned int getCountOfInBattleHero() const {return m_InBattleHeros->count();}
    /* 获取敌方上阵武将的数量 */
    inline unsigned int getCountOfInBattleEnemyHero() const {return m_InBattleEnemys->count();}
    /* 获取敌方上阵怪物的数量 */
    inline unsigned int getCountOfInBattleEnemyMoster() const {return m_InBattleEnemyMosters->count();}
    /* 获取攻击数据项数量 */
    inline unsigned int getCountOfAttackDataItem() const {return m_AttackDataItems->count();}
    /* 获取攻击数据项 */
    inline AttackDataItem* getAttackDataItem(const unsigned int index) {
        return index >= m_AttackDataItems->count() ? NULL : (AttackDataItem*)m_AttackDataItems->objectAtIndex(index);
    }
    /* 获取是否胜利 */
    inline bool getWin() const {return m_isWin;}
    /* 设置是否胜利 */
    inline void setWin(const bool isWin) {m_isWin = isWin;}
    /* 获取 pk 模式 */
    inline BattlePkMode getBattlePkMode() const {return m_battlePkMode;}
    /* 设置 pk 模式 */
    inline void setBattlePkMode(const BattlePkMode mode) {m_battlePkMode = mode;}
    /* 获取战斗波数 */
    inline unsigned int getRoundIndex() const {return m_roundIndex;}
    /* 设置战斗波数 */
    inline void setRoundIndex(const unsigned int roundIndex) {m_roundIndex = roundIndex;}
    /* 获取战斗回合数 */
    inline unsigned int getSequenceIndex() const {return m_sequenceIndex;}
    /* 设置战斗回合数 */
    inline void setSequenceIndex(const unsigned int roundIndex) {m_sequenceIndex = roundIndex;}
    /* 获取掉落的武将card数量 */
    inline unsigned int getCountOfDropHeroCards() const {return m_countOfDropHeroCards;}
    /* 设置掉落的武将card数量 */
    inline void setCountOfDropHeroCards(const unsigned int countOfDropHeroCards) {m_countOfDropHeroCards = countOfDropHeroCards;}
    /* 获取掉落的装备数量 */
    inline unsigned int getCountOfDropEquips() const {return m_countOfDropEquips;}
    /* 设置掉落的装备数量 */
    inline void setCountOfDropEquips(const unsigned int countOfDropEquips) {m_countOfDropEquips = countOfDropEquips;}
    /* 获取掉落的道具数量 */
    inline unsigned int getCountOfDropProps() const {return m_countOfDropProps;}
    /* 设置掉落的道具数量 */
    inline void setCountOfDropProps(const unsigned int countOfDropProps) {m_countOfDropProps = countOfDropProps;}
    /* 根据起始位置索引和终点位置索引获得下一个位置索引(水平方向优先) */
    unsigned int getNextPosIndexHorizontal(const unsigned int, const unsigned int, unsigned int&);
    /* 根据起始位置索引和终点位置索引获得下一个位置索引(垂直方向优先) */
    unsigned int getNextPosIndexVertical(const unsigned int, const unsigned int, unsigned int&);
    /* 判断目标位置索引是否相邻源位置索引 */
    bool isNeighborPosIndex(const unsigned int, const unsigned int, unsigned int&);
    /* 判断终点位置索引是否和起始位置索引是垂直位置关系 */
    bool isVerticalPosIndex(const unsigned int, const unsigned int, unsigned int&);
    /* 判断终点位置索引是否和起始位置索引是水平位置关系 */
    bool isHorizontalPosIndex(const unsigned int, const unsigned int, unsigned int&);
    /* 获取和设置战场速度 */
    inline unsigned int getBattleSpeed() const {return m_nBattleSpeed;}
    inline void setBattleSpeed(const float battleSpeed) {m_nBattleSpeed = battleSpeed;}
    
    /* 获取士兵动画 */
    CCAnimation* getSoldierAni(const unsigned int, const unsigned int);
    /* 加载士兵动画 */
    void _loadSoldierAni();
    
    /* 提交 PVE 战斗请求 */
    void commitPveFightRequest();
    /* 提交铜雀台战斗请求*/
    void commitTqtFightRequest();
    /* 提交国战战斗请求 */
    void commitCWarFightRequest();
    
    inline unsigned int getTqtTeamPosition() {return m_tqtTeamPosition;}
    inline void setTqtTeamPosition(const unsigned int tqtTeamPosition) {m_tqtTeamPosition = tqtTeamPosition;}
    
    /* 清除所有战斗报告数据 */
    void clearAllBattleReport(const bool isRelease = false);
    /* 获取战斗报告数量 */
    inline unsigned int getCountOfBattleReport() const {return m_BattleReportItems->count();}
    /* 添加战斗报告 */
    void appendBattleReport(BattleReportItem*);
    /* 根据索引号获取战斗报告 */
    BattleReportItem* getBattleReportItemByIndex(const unsigned int) const;
    
    inline unsigned int getRemainBattleTimes() const {return m_remainTimes;}
    inline void setRemainBattleTimes(const unsigned int remainBattleTimes) {m_remainTimes = remainBattleTimes;}
    
    inline std::string getTargetName() const {return m_targetName;}
    inline void setTargetName(std::string targetName) {m_targetName = targetName;}
    
    /* 获取 pvp 敌方队长 */
    inline InBattleItem* getPvpEnemyCaptain() const {return m_pvpEnemyCaptain;}
    /* 设置 pvp 敌方队长 */
    void setPvpEnemyCaptain(const InBattleItem*);
    /* 清除 pvp 队长 */
    void clearPvpEnemyCaptain();
    
    /* 设置铜雀台战斗获取的铜钱数 */
    inline void setTqtAddMoney(const unsigned int tqtAddMoney) {m_tqtAddMoney = tqtAddMoney;}
    /* 获取铜雀台战斗获取的铜钱数 */
    inline unsigned int getTqtAddMoney() const {return m_tqtAddMoney;}
    
    /* 设置国战战斗获取的铜钱数 */
    inline void setCwarAddMoney(const unsigned int cwarAddMoney) {m_cwarAddMoney = cwarAddMoney;}
    /* 获取国战战斗获取的铜钱数 */
    inline unsigned int getCwarAddMoney() const {return m_cwarAddMoney;}
    
    /* 获取和设置国战标志 */
    inline bool isCWar() const {return m_isCWar;}
    inline void setCWar(const bool isCWar) {m_isCWar = isCWar;}
    
    /*设置获取剩余次数*/
    inline void setCwarTimes(unsigned int var) {m_startTimeForCwar = var;}
    inline unsigned int getCwarTimes() {return m_startTimeForCwar;}
    /*设置获取城池类型*/
    inline void setCwarCityType(unsigned int var) {m_cityType = var;}
    inline unsigned int getCwarCityType() {return m_cityType;}
    /*国战城墙数量*/
    inline unsigned int getCwarWallNum() {return m_numOfWall;}
    inline void setCwarWallNum(unsigned int var) {m_numOfWall = var;}
    /*国战投石车数量*/
    inline unsigned int getCwarCarNum() {return m_numOfCar;}
    inline void setCwarCarNum(unsigned int var) {m_numOfCar = var;}
    /*国战战斗冷却时间*/
    inline void setCwarCd(int var) {m_gzFightCd = var;}
    inline int getCwarCd() {return m_gzFightCd;}
    inline void setAtk(float var) {m_atk = var;}
    inline float getAtk() {return m_atk;}
    inline void setHp(float var) {m_hp = var;}
    inline float getHp() {return m_hp;}
    inline void setMoney(float var) {m_money = var;}
    inline float getMoney() {return m_money;}
    inline void setWallAtk(float var) {m_wallAtk = var;}
    inline float getWallAtk() {return m_wallAtk;}
    inline void setWallHp(float var) {m_wallHp = var;}
    inline float getWallHp() {return m_wallHp;}
    inline void setWallMoney(float var) {m_wallMoney = var;}
    inline float getWallMoney() {return m_wallMoney;}
    inline void setInspireAtk(float var) {m_inspireAtk = var;}
    inline float getInspireAtk() {return m_inspireAtk;}
    inline void setInspireHp(float var) {m_inspireHp = var;}
    inline float getInspireHp() {return m_inspireHp;}
    inline void setInspireMny(float var) {m_inspireMoney = var;}
    inline float getInspireMny() {return m_inspireMoney;}
    inline void setMnyBuff(float var) {m_mnyBuff = var;}
    inline float getMnyBuff() {return m_mnyBuff;}
    inline void setDefLevMny(float var) {m_defLevMny = var;}
    inline float getDefLevMny() {return m_defLevMny;}
    
    inline bool isInList() const {return m_isInList;}
    inline void setInList(const bool isInList) {m_isInList = isInList;}
    inline int getDefTime() const {return m_defTime;}
    inline void setDefTime(const int isInList) {m_defTime = isInList;}
    
private:
    NetConnection* m_pNetConnection;
    GameState* m_pGameState;
    GamePlayer* m_pGamePlayer;
    BattlePkMode m_battlePkMode;                // 战场 pk 模式(pvp pve)
    bool m_isCWar;                              // 是否国战
    bool m_isInList;                            // 是否在防守列表
    
    /*
     *  在数组中的位置，对应它在本方阵营中的位置
     */
    CCArray* m_InBattleHeros;                   // 本方武将
    CCArray* m_InBattleEnemys;                  // 敌方战队(pvp)
    CCArray* m_InBattleEnemyMosters;            // 敌方怪物(pve)
    
    bool m_isWin;                               // 是否胜利
    CCArray* m_AttackDataItems;                 // 攻击数据
    
    CCArray* m_BattleReportItems;               // 战斗报告
    
    unsigned int m_roundIndex;                  // 战斗波数（每下载一次算一波）
    unsigned int m_sequenceIndex;               // 战斗回合数
    
    unsigned int m_countOfDropHeroCards;        // 掉落的武将card数量
    unsigned int m_countOfDropEquips;           // 掉落的装备数量
    unsigned int m_countOfDropProps;            // 掉落的道具数量
    
    unsigned int m_nBattleSpeed;                // 战斗速度 1:1倍速 2:2倍速 3:3倍速 4:3倍速 + auto
    
    unsigned int m_remainTimes;                 // 剩余战斗次数
    
    unsigned int m_tqtTeamPosition;
    
    std::string m_targetName;
    
    InBattleItem* m_pvpEnemyCaptain;            // pvp 敌方队长
    
    unsigned int m_tqtAddMoney;                 // 铜雀台战斗获得的铜钱数
    
    /*国战相关*/
    unsigned int m_startTimeForCwar;            // 距离国战开始剩余时间
    unsigned int m_fightTimesForCwar;           // 国战攻击次数剩余
    unsigned int m_cityType;
    unsigned int m_numOfWall;                   // 国战城墙
    unsigned int m_numOfCar;                    // 国战投石车
    int m_gzFightCd;
    float m_atk;                                // 建造加成攻击
    float m_hp;                                 // 建造加成血量
    float m_money;                              // 建造加成金钱
    float m_wallAtk;                                // 建造加成攻击
    float m_wallHp;                                 // 建造加成血量
    float m_wallMoney;                              // 建造加成金钱
    float m_inspireAtk;                                // 建造加成攻击
    float m_inspireHp;                                 // 建造加成血量
    float m_inspireMoney;                              // 建造加成金钱
    float m_mnyBuff;
    float m_defLevMny;
    int m_defTime;                              // 防守时间
    unsigned int m_cwarAddMoney;                // 国战战斗获得的铜钱数
    
};

#endif /* defined(__hero__Battle__) */
