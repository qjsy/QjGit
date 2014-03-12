/* 
 *  阵型选择
 *      隶属于主场景
 */

#ifndef __hero__ChosFormLayer__
#define __hero__ChosFormLayer__

#include "CCHeroCommonLayer.h"
#include "common.h"

USING_NS_CC;

#define CFL_COUNT_OF_TEAM                           (MAX_IN_BATTLE_SOLDIERS + 8)  // 队伍成员数量

class HeroOfPlayerItem;
class GameState;
class HeroManager;
class Battle;
class GamePlayer;
class ChosFormLayer : public CCHeroCommonLayer {
public:
    ChosFormLayer();
    virtual ~ChosFormLayer();
    
public:
    virtual bool init();
    CREATE_FUNC(ChosFormLayer);

public:
    virtual void registerWithTouchDispatcher(void);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    virtual void setVisible(bool);
    
    
    /*移出动作*/
    virtual void moveOutTouchUICompent();
    /*恢复响应动作*/
    virtual void restoreTouchUICompent();

    
public:
    // 开场动画
    void openningAni();
    
    // 退出选择整型
    void exitChosForm();
    
    // 选择好友动作
    void chosFrid();
    
    // 出征
    void goToFight();
    // 出征 pve
    void goToFightPve();
    // 出征国战
    void goToFightCwar();
    // 出征铜雀台
    void goToFightTqt();
    
    // 选择阵型
    void chosForm(CCObject*);

private:
    /* 初始化 UI 组件 */
    void _initUiComponent();
    
    /* 加载士兵动画 */
    void _loadSoldierAni();

    /* 调整 team zorder */
    void _updateTeamZorder(const unsigned int, const unsigned int);
    
    /* 上阵武将关联战队 */
    void _attachTeam(const unsigned, HeroOfPlayerItem*);
    
private:
    // touch 区域
    CCRect m_touchArea[MAX_IN_BATTLE_HEROS];
    // 武将和士兵
    CCArray* m_team[MAX_IN_BATTLE_HEROS];
    // 存放所有位置
    CCPoint m_AllPos[MAX_IN_BATTLE_HEROS * CFL_COUNT_OF_TEAM];
    
    CCAnimation* m_soldierDj[7];        // 存放对应7种职业的士兵普通动画

    unsigned int _touchBeganIndex = 0;  // 
    CCPoint _touchBeganPoint;           // 触摸起始点
    
    /*
     *  士兵动画
     *  该数组保存所有职业的常规状态的士兵动画
     */
    CCAnimation* m_soldierAni[7];            // 7 种职业
    
    bool m_enableTouch;
    
    GameState* m_pGameState;
    HeroManager* m_pHeroManager;
    Battle* m_pBattle;
    GamePlayer* m_pGamePlayer;
    CCNotificationCenter* m_pNotificationCenter;
};

#endif /* defined(__hero__ChosFormLayer__) */
