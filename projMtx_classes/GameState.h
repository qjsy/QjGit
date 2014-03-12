//
//  GameState.h
//  hero
//
//  Created by yangjie on 2013/12/15.
//
//  保存游戏运行中相关状态数据
//

#ifndef __hero__GameState__
#define __hero__GameState__

#include "common.h"
#include "cocos2d.h"

class GamePlayer;
class GameState {
private:
    GameState();
public:
    virtual ~GameState();
    
public:
    /* 获取单例 */
    static GameState* getInstance();
    
public:
    inline WhoCallHeroList getTagWhoCallHeroSel() {return m_tagWhoCallHeroSel;}
    inline void setTagWhoCallHeroSel(const WhoCallHeroList tagWhoCallHeroSel) {m_tagWhoCallHeroSel = tagWhoCallHeroSel;}
    inline WhoCallHeroList getCurrTagWhoCallHeroSel() {return m_currWhoCallHeroSel;}
    inline void setCurrTagWhoCallHeroSel(const WhoCallHeroList tagWhoCallHeroSel) {m_currWhoCallHeroSel = tagWhoCallHeroSel;}
    /* */
    inline int getTagWhoCallMainScene() const {return m_tagWhoCallMainScene;}
    inline void setTagWhoCallMainScene(const int tagWhoCallMainScene)  {m_tagWhoCallMainScene = tagWhoCallMainScene;}
    
    /*哪个层调用了武将简介*/
    inline WhoCallHeroIntro getTagWhoCallHeroIntro () {return m_TagWhoCallHeroIntro;}
    inline void setTagWhoCallHeroIntro(const WhoCallHeroIntro tagWhoCallHeroSel) {m_TagWhoCallHeroIntro = tagWhoCallHeroSel;}
    //    inline SelHerosForLevEvoUp getSelHerosForLevEvoUp() {return m_SelHerosForLevEvoUp;}
    //    inline void setSelHerosForLevEvoUp(SelHerosForLevEvoUp shfleu) {m_SelHerosForLevEvoUp = shfleu;}
    /* */
    inline void setHeroEvoUp(const char szHeroEvoUpUniId[]) {strcpy(m_szHeroEvoUpUniId, szHeroEvoUpUniId);}
    inline void getHeroEvoUp(char szHeroEvoUpUniId[]) {strcpy(szHeroEvoUpUniId, m_szHeroEvoUpUniId);}
    
    /* 获取当前设备是否 1136 分辨率 */
    inline bool is1136() const {return m_is1136;}
    
    /* 根据分辨率不同获取顶部框图偏移量 */
    inline float getTopOffset() const {return m_is1136 ? 100.0f : 0.0f;}
    
    /* 根据分辨率不同获取底部框图偏移量 */
    inline float getBottomOffset() const {return m_is1136 ? 90.0f : 0.0f;}
    
    /*游戏状态数据，计算抽取武将后的玩家信息数据*/
    void updateSearch(int);
    
    /* 玩家信息 */
    void updateForPlayer();
    
    // 存储选择的副本id
    CC_PROPERTY(unsigned int, m_id, CopyId);
    // 存储当前的副本id
    CC_PROPERTY(unsigned int, m_curId, CopyCurId);
    // 存储选择的关卡id
    CC_PROPERTY(unsigned int, m_lid, LevId);
    // 储存最大副本进度
    CC_PROPERTY(unsigned int, m_maxNorPve, MaxNorPve);
    // 存储玩家打副本获得经验
    CC_PROPERTY(unsigned int, m_copyForExp, CopyForExp);
    // 存储玩家打副本获得铜币
    CC_PROPERTY(unsigned int, m_copyForMoney, CopyForMoney);
    // 吞噬所需铜币
    CC_PROPERTY(unsigned int, m_tunNeedMoney, TunNeedMoney);
    // 进化所需铜币
    CC_PROPERTY(unsigned int, m_evoNeedMoney, EvoNeedMoney);
    //
    CC_PROPERTY(unsigned int, m_localMaxLev, LocalMaxLev);
    //
    CC_PROPERTY(unsigned int, m_localMaxCopy, LocalMaxCopy);
    // 监狱释放武将id
    CC_PROPERTY(unsigned int, m_freeIdx, FreeIdx);
    
    /*购买体力暂存价格，购买成功后设置*/
    CC_PROPERTY(unsigned int, m_tempPrice, TempPrice);
    
    CC_PROPERTY(unsigned int, m_cd, Cd);
    // 记录招降成功或者失败
    CC_PROPERTY(bool, m_isSummonSucess, IsSummonSucess);
    CC_PROPERTY(unsigned int, m_subLoyalty, SubLoyalty);
    CC_PROPERTY(unsigned int, m_sucessSummonId, SucessSummonId);/*招降成功武将的id*/
    //记录释放武将是否成功
    CC_PROPERTY(bool, m_isSucessShiF, IsSucessShiF);
    // 战斗获得监狱武将ID
    CC_PROPERTY(unsigned int, m_getPrisonHero, GetPrisonHero);
    /*******国战逻辑********/
    // 存储国战选中势力的状态
    CC_PROPERTY(unsigned int, m_selectCamp, SelectCamp);
    // 国战建造所需要GOLD
    CC_PROPERTY(unsigned int, m_cWarBuildCar, CWarBuildCar);
    CC_PROPERTY(unsigned int, m_cWarBuildWall, CWarBuildWall);
    CC_PROPERTY(unsigned int, m_cWarInspire, CWarInspire);
    CC_PROPERTY(bool, m_haveWall, HaveWall);
    CC_PROPERTY(bool, m_haveCar, HaveCar);
    CC_PROPERTY(bool, m_haveInspire, HaveInspire);
    CC_PROPERTY(unsigned int , m_awardType, AwardType);
    // 储存是否为新用户
    CC_PROPERTY(bool, m_isNewUser, IsNewUser);
    /*武将简介uniid*/
    inline void setHeroIntro(std::string var) {m_heroIntroUUid = var;}
    inline std::string getHeroIntro() {return m_heroIntroUUid;}
    /*武将简介id*/
    inline void setHeroIntroId(unsigned int var) {m_heroIntroId = var;}
    inline int getHeroIntroId() {return m_heroIntroId;}
    
public:
    inline void setLevupHeroUUidForHeroList(std::string uuid) {m_szHeroLevUpUUid = uuid;}
    inline std::string getLevupHeroUUidForHeroList() {return m_szHeroLevUpUUid;}
    inline void setEvoupHeroUUidForHeroList(std::string uuid) {m_szHeroEvoUpUUid = uuid;}
    inline std::string getEvoupHeroUUidForHeroList() {return m_szHeroEvoUpUUid;}
    inline unsigned int getCountOfFoodHeros() const {return m_countOfFoodHeros;}
    void setFoodHersForLevUp(const unsigned int countOfFoodHeros, char szFoodHersForLevUp[][HERO_UNIID_BUFFER_SIZE]);
    unsigned int getFoodHersForLevUp(char szFoodHersForLevUp[][HERO_UNIID_BUFFER_SIZE]);
    inline bool getHeroForSaleSuccess() const {return m_isHeroForSaleSuccess;}
    inline void setHeroForSaleSuccess(const bool heroForSaleSuccess ) {m_isHeroForSaleSuccess = heroForSaleSuccess;}
    
    /* 是否有未结束战斗 */
    inline bool hasEndlessFight() const {return m_hasEndlessFight;}
    /* 获取未结束战斗副本id */
    inline unsigned int getEndlessFightCopyId() const {return m_endlessCopyId;}
    /* 获取未结束战斗关卡id */
    inline unsigned int getEndlessFightLevelId() const {return m_endlessLevelId;}
    /* 获取未结束战斗战斗波数 */
    inline unsigned int getEndlessFightRoundIndex() const {return m_endlessRoundIndex;}
    /* 设置未结束战斗相关信息 */
    void setEndlessFight(const unsigned int, const unsigned int, const unsigned int);
    /* 获取和设置玩家选择的服务器id */
    inline unsigned int getServerId() const {return m_serverid;}
    inline void setServerId(const unsigned int serverId)  {m_serverid = serverId;}
    
    void setNormalMax(const unsigned int, const unsigned int);
    inline unsigned int getNormalMaxLevId() const {return m_norLevId;}
    inline unsigned int getNormalMaxCopyId() const {return m_norCopyId;}
    
    /* 添加未完成战斗的队伍索引值 */
    void appendUfTeamPos(std::string, const unsigned int);
    /* 获取未完成战斗的队伍索引值 */
    unsigned int getUfTeamPos(std::string);
    /* 清除未完成战斗的队伍索引值 */
    void clearUfTeamPos();
    
    inline void setIsMainCityCall(bool var) {m_isMainCityCall = var;}
    inline bool getIsMainCityCall() {return m_isMainCityCall;}
    
    /*记录国战开始和结束状态*/
    inline void setIsBeganGZ(bool var) {m_isBegan = var;}
    inline bool getIsBeganGZ() {return m_isBegan;}
    
    inline void setAwardSaveTime() {m_awardSaveTime = time(NULL);}
    inline time_t getAwardSaveTime() {return m_awardSaveTime;}
    
    /* 启动计时线程 */
    void startTimerThread();
    /* 停止计时线程 */
    void shutdownTimerThread();
    /* 计时器每秒调用的回调方法 */
    void timerPerSecondCallBack();
    /* 计时线程是否运行标记 */
    inline bool isTimer() const {return m_timer;}
    
    /* 添加计时任务 */
    void appendTimerTask(const unsigned int, int);
    /* 移除计时任务 */
    void removeTimerTask(const unsigned int);
    /* 获取指定计时任务的剩余时间 */
    int getRemainTimes(const unsigned int);
    
private:
    // 当前设备是否 1136 分辨率
    bool m_is1136;
    
    GamePlayer* m_pGamePlayer;      // 游戏玩家
    
    volatile bool m_timer;
    
    // 保存上一次武将列表调用者
    WhoCallHeroList m_tagWhoCallHeroSel;
    // 保存本次武将列表调用者
    WhoCallHeroList m_currWhoCallHeroSel;
    // 保存原已选择升级武将 uuid
    std::string m_szHeroLevUpUUid;
    // 保存原已选择升阶武将 uuid
    std::string m_szHeroEvoUpUUid;
    // 保存被吞噬武将数量
    unsigned int m_countOfFoodHeros;
    // 保存被吞噬武将 uuid
    char m_szFoodHersForLevUp[MAX_FOOD_HEROS_FOR_LEVUP][HERO_UNIID_BUFFER_SIZE];
    // 是否出售武将成功 true 出售成功, false 失败或是玩家取消出售
    bool m_isHeroForSaleSuccess;
    
    /*武将简介*/
    std::string m_heroIntroUUid;
    unsigned int m_heroIntroId;
    WhoCallHeroIntro m_TagWhoCallHeroIntro;
    /* 谁调用主场景
     * 1 登陆主场景
     * 2 战斗返回
     */
    int m_tagWhoCallMainScene;
    
    char m_szHeroEvoUpUniId[HERO_UNIID_BUFFER_SIZE];        // 保存武将升阶后得到的武将
    
    bool m_hasEndlessFight;                                 // 是否存在未结束战斗
    unsigned int m_endlessCopyId;                           // 未结束战斗副本id
    unsigned int m_endlessLevelId;                          // 未结束战斗关卡id
    unsigned int m_endlessRoundIndex;                       // 未结束战斗波数
    
    unsigned int m_serverid;
    
    unsigned int m_norCopyId;
    unsigned int m_norLevId;                                // 普通副本最大关卡
    
    bool m_isMainCityCall;
    bool m_isBegan;
    
    time_t m_awardSaveTime;                                 // 铜雀台奖励时间
    
    std::map<std::string, unsigned int> m_ufTeamPos;        // 存储未结束战斗的的战队的位置
    
    std::map<unsigned int, int>m_timerTasks;                // 存储计时任务
    pthread_t m_timerThread;
    pthread_mutex_t m_timerTaskLock;                        //
};

#endif /* defined(__hero__GameState__) */
