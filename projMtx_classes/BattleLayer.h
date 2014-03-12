//
//  BattleLayer.h
//  battle
//
//  战斗
//

#ifndef __hero__BattleLayer__
#define __hero__BattleLayer__

#include "common.h"
#include "HandleNetDataDelegate.h"
#include "CCHeroCommonLayer.h"

USING_NS_CC;

class Battle;
class GameState;
class BattleTeam;
class HeroManager;
class BattleLayer : public CCHeroCommonLayer, public HandleNetDataDelegate {
public:
    BattleLayer();
    virtual ~BattleLayer();

    friend class BattleTeam;
    
public:
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual void registerWithTouchDispatcher(void);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    virtual void moveOutTouchUICompent();
    virtual void restoreTouchUICompent();
    
    CREATE_FUNC(BattleLayer);

public:
    /* 自动前进调度 */
    void auto_go_schedule(CCTime);
    
    /* 战场调度 */
    void battle_schedule(CCTime);
    
    /* 战斗结束调度 */
    void battleFinishSchedule(CCTime);
    
public:
    /* 处理服务器回应数据 */
    virtual void handleWSResponseData(const char *, const unsigned int, char*);
    
public:
    /* 加载地图 */
    void loadMap();

    /* 获取士兵动画 */
    CCAnimation* getSoldierAni(const unsigned int, const unsigned int);

    /* 游戏胜利回调方法 */
    void victoryCallback();
    
    /* 游戏失败回调方法 */
    void defeatCallback();
    
    /* 掉落动画完成回调 */
    void dropDoneCallback(CCNode*, void*);
    
private:
    /* 初始化 UI 组件 */
    void _initUiComponent();
    
    /* 初始化 Touch 区域 */
    void _initTouchArea();
    
    /* 初始化本方战队 */
    void _initOwnBattleTeam();
    /* 初始化战场 */
    void _initBattle();
    
    /* 加载所有士兵动画 */
    void _loadSoldierAni();
    
    /* 加载掉落动画 */
    void _loadDropAni();
    
    /* 初始化位置索引对应的区块左下角坐标点位置 */
    void _initPosByPosIndex();
    /* 根据位置索引获得该区块左下角坐标点位置 */
    CCPoint _getPosByPosIndex(const unsigned int);

    /* 根据位置索引获得对应战队 */
    BattleTeam* _getBattleTeam(const unsigned int);
    
    /* 根据参数获得对应战队 */
    BattleTeam* _getBattleTeam(const unsigned int, const bool, std::string);
    
    /* 重置本方队伍的位置（到它的原始位置）*/
    void _resetTeamPos();
    
    /* 获取技能精灵转动角度 */
    float _getSkillAngle(const CCPoint, const CCPoint) const;

    /* 提交本方阵型数据给服务器，获取下一回合战斗数据 */
    void nextFight();
    
    /* 清除掉落动画 */
    void _clearDropAni();
    
    /* 波数动画特效 */
    void roundEffect(const unsigned int);
    
    /* 战斗初始动画特效 */
    void battleInitEffect();
    
    /* 处理物品掉落 */
    void _processDrop(const BattleTeam*);
    
    /* 根据当前战场速度显示对应图像 */
    void _updateBattleSpeedIcon(const unsigned int);
    
    /* 调整战场速度 */
    void _adjustBattleSpeed();
    
private:
    /*
     *  调整本方战队队伍阵型相关
     */
    bool m_enableTouch;                     // 是否允许接收 touch 事件
    
    // touch 区域
    CCRect m_touchArea[MAX_IN_BATTLE_HEROS];
    // 本方区域队伍位置索引
    unsigned int m_ownTeamPosIndexs[MAX_IN_BATTLE_HEROS];
    CCPoint _touchBeganPoint;
    unsigned int _touchBeganIndex;
    
    CCSpriteBatchNode* m_battleCBN;
    CCSpriteBatchNode* m_teamCBN;           // 战队批处理精灵
    CCSpriteBatchNode* m_skillCBN;          // 技能批处理精灵

    GameState* m_pGameState;                // 游戏状态数据
    Battle* m_pBattle;                      // 关联的战场数据
    HeroManager* m_pHeroManager;            // 武将管理
    CCNotificationCenter* m_pNotificationCenter;
    
    unsigned int m_countOfInBattleHero;     // 本方武将数量
    unsigned int m_countOfInBattleEnemy;    // 敌方武将(怪物)数量
    
    /* 
     *  士兵动画
     */
    CCAnimation* m_soldierAni[7][3];
    
    /*
     *  战场队伍
     */
    CCArray* m_BattleTeamsArray;
    
    /*
     *  暂存本方战斗中死亡的队伍
     */
    CCArray* m_BattleDeadTeamsArray;
    
    /*
     *  battle event
     */
    CCArray* m_battleEvents;                // 存放所有战场事件
    unsigned int m_countOfWaitEvents;       // 要等待完成的战场事件数量
    unsigned int m_indexOfBattleEvents;     //
    
    CCSpriteBatchNode* m_pSkillBatchNode;   // 技能特效批处理精灵
    CCSpriteFrameCache* m_pSpriteFrameCache;
    CCDirector* m_pDirector;
    CCSprite* m_go;                         // 前进
    CCSprite* m_exit;                       // 退出
    
    /*
     *  掉落动画
     */
    CCArray* m_dropItems;                   // 存放掉落物品(精灵)
    CCAnimation* m_propDropAni;             // 道具掉落动画
    CCAnimation* m_heroDropAni;             // 武将掉落动画
    CCAnimation* m_equipDropAni;            // 装备掉落动画
    
    CCSprite* m_battleSpeedIcons[4];        // 存放战场速度图像
    
    CCPoint m_allOriginCoordinate[15];      // 战场各区块原点坐标位置
};

#endif /* defined(__hero__BattleLayer__) */


