//
//  TongQueLayer.h
//  hero
//
//  Created by yangjie on 2014/02/19.
//
//  铜雀台
//

#ifndef __hero__TongQueLayer__
#define __hero__TongQueLayer__

#include "CCHeroCommonLayer.h"

#define  REPORT_CONUNT              5

class GameState;
class Battle;
class GamePlayer;
class TongQueLayer : public CCHeroCommonLayer {
public:
    TongQueLayer();
    virtual ~TongQueLayer();
    
public:
    virtual bool init();
    CREATE_FUNC(TongQueLayer);
    
public:
    virtual void registerWithTouchDispatcher(void);
    virtual bool ccTouchBegan(CCTouch*, CCEvent*);
    virtual void ccTouchMoved(CCTouch*, CCEvent*);
    virtual void ccTouchEnded(CCTouch*, CCEvent*);
    virtual void ccTouchCancelled(CCTouch*, CCEvent*);
    
public:
    /*初始化位置*/
    virtual void initPosition();
    /*移出动作*/
    virtual void moveOutTouchUICompent();
    /*恢复响应动作*/
    virtual void restoreTouchUICompent();
    
public:
    void updateAllTQ();
    
    inline void setTimeForAward(unsigned int var) {
        m_timeForAward = var;
    }
    inline unsigned int getTimeForAward() {
        return m_timeForAward;
    }
    
private:
    void _callbackForMoveOut();
    /* 更新排名 */
    void _updateRank();
    /* 更新奖励倒计时 */
    void _updateTimeDown();
    void _scheduleForTimeDown();
    /* 获得奖励 */
    void _getAward();
    /* 更新UI */
    void _initUi();
    
    /* 选中某个rect按钮 */
    void _onBtnSelect(int);
    /* 选中某个战报 */
    void _onReportSelect(int);
    /* 灰掉铜雀台 */
    void _disableAward();
    /* 初始化奖励按钮 */
    void _initAwardBtn();
    /* 更新奖励铜钱 */
    void _updateCopper();
    /* 更新指针 */
    void _updatePoint();
    /* 更新战报 */
    void _updateReport();
    /* 更新战斗冷却倒计时 */
    void _updateTimeFight();
    void _scheduleForFight();
private:
    CCSpriteBatchNode* m_pSBN;
    GameState* m_pGameState;
    Battle* m_pBattle;
    GamePlayer* m_pGamePlayer;
    
    CCSprite* m_awardFont[2];
    CCSprite* m_bgOfAward[3];
    bool m_btnEnable;
    
    CCRect m_heroRect[3];
    bool m_rectEnable[3];
    int m_touchBeganBtnIdx;
    
    int m_timeForAward;
    int m_timeForFight;/*战斗冷却时间*/
    
    CCLabelTTF* m_timeSchedule;
    CCLabelTTF* m_timeForFightSch;
    
    CCLabelTTF* m_reportLabel[REPORT_CONUNT];
    int m_touchBeganLabelIdx;
    
    /*是否处于冷却中*/
    bool m_isInReset;
};
#endif /* defined(__hero__TongQueLayer__) */


