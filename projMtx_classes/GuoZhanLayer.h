//
//  GuoZhanLayer.h
//  hero
//
//  Created by 钱剑 on 14-2-25.
//
//

#ifndef __hero__GuoZhanLayer__
#define __hero__GuoZhanLayer__

#include "CCHeroCommonLayer.h"
#define CITY_COUNT                  3

class GameState;
class Battle;
class CityManager;
class CCStrokeLabel;
class GuoZhanLayer : public CCHeroCommonLayer {
public:
    GuoZhanLayer();
    virtual ~GuoZhanLayer();
    
public:
    virtual bool init();
    CREATE_FUNC(GuoZhanLayer);
    
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
    void updateAllOfGuoZhanLayer();
    
private:
    /*国战初始界面延迟显示*/
    void _delayShow();
    /*界面ui初始化*/
    void _initUi();
    /*初始化按钮UI*/
    void _initBtn();
    void _onBtnSelected(int);
    /*时间更新函数*/
    void _scheduleForTime();
    /*积分比例条*/
    CCSprite* _integralPro(const float green, const float blue);
private:
    CCSpriteBatchNode* m_pSBN;
    GameState* m_pGameState;
    Battle* m_pBattle;
    CityManager* m_pCityManager;
    
    CCSprite* m_btnSprite[CITY_COUNT][3];
    CCPoint m_btnPosition[CITY_COUNT][3];
    bool m_btnEnable[CITY_COUNT];
    int m_touchBeganBtnIdx;
    
    /*国战开始剩余时间*/
    int m_timeForBeganFight;
    CCStrokeLabel* m_timeBeganLabel;
    
    /*是否国战中*/
    bool m_isFighting;
    /*比例进度条*/
    CCProgressTimer* m_pProgress[3];
};




#endif /* defined(__hero__GuoZhanLayer__) */
