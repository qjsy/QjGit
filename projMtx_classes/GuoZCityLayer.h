//
//  GuoZCityLayer.h
//  hero
//
//  Created by 钱剑 on 14-2-25.
//
//

#ifndef __hero__GuoZCityLayer__
#define __hero__GuoZCityLayer__

#include "CCHeroCommonLayer.h"

#define COUNT_OF_BTNS               4


class GameState;
class Battle;
class NetConnection;
class CityManager;
class GamePlayer;
class GuoZCityLayer : public CCHeroCommonLayer {
public:
    GuoZCityLayer();
    virtual ~GuoZCityLayer();
    
public:
    virtual bool init();
    CREATE_FUNC(GuoZCityLayer);
    
public:
    virtual void registerWithTouchDispatcher(void);
    virtual bool ccTouchBegan(CCTouch*, CCEvent*);
    virtual void ccTouchMoved(CCTouch*, CCEvent*);
    virtual void ccTouchEnded(CCTouch*, CCEvent*);
    virtual void ccTouchCancelled(CCTouch*, CCEvent*);
    virtual void setVisible(bool);
    
public:
    /*初始化位置*/
    virtual void initPosition();
    /*移出动作*/
    virtual void moveOutTouchUICompent();
    /*恢复响应动作*/
    virtual void restoreTouchUICompent();
    
public:
    void updateAllOfGZCity();
    /*建造刷新*/
    void updateForBuild();
    /*更新鼓舞加成数据*/
    void updateForInspire();
    /*防御或攻击动画*/
    void aniForAtkOrDef();
    void setInListStatus(int);
private:
    /*移出回调*/
    void _callbackForMoveOut();
    
    /*界面ui初始化*/
    void _initUi();
    /*初始化btn*/
    void _initBtn();
    /*鼓舞*/
    void _courage();
    void _defOrAtt();
    void _resetAtt();
    void _build();
    /*tipMenu*/
    void _tipMenu(int);
    /*返回按钮*/
    void _returnForm();
    /*更新系统文字信息*/
    void _updateFontInfo();
    /*冷却时间*/
    void _scheduleForCooling();
    /*检查属于哪个势力的哪个城池*/
    void _checkForWitch(int camp, int cityId);
    /*列表倒计时*/
    void _scheduleForAttOrDef();
    /*INIT*/
    void _initAniAtkOrDef();
    /*冷却计时*/
    void _coolForFight();
private:
    GameState* m_pGameState;
    Battle* m_pBattle;
    NetConnection* m_pNetConnection;
    CityManager* m_pCityManager;
    GamePlayer* m_pGamePlayer;
    
    bool m_thisTouch;
    
    CCSpriteBatchNode* m_pSBN;
    
    CCSprite* m_btn[COUNT_OF_BTNS][3];
    
    /*冷却时间*/
    CCLabelTTF* m_coolTimeLabel;
    /*列表占领倒计时*/
    CCLabelTTF* m_listTimeLabel;
    /*是否在战斗冷却中*/
    bool m_isCooling;
    /*是否处于防守状态*/
    bool m_isDefending;
    /*是否在防守列表中*/
    int m_typeOfAtkOrDef;
    int m_isInList;
    int m_listTime;
    int m_minTime;
    int m_defTime;/*防守时间累计*/
    /*初始化的4种状态*/
    int m_fourState;
    /*攻击和防御文字*/
    CCSprite* m_pAttFont;
    CCSprite* m_pDefFont;
    /*占领中*/
    CCLabelTTF* m_pHold[2];
    /*铜钱和战功收入*/
    CCLabelTTF* m_income[2];
    /*建造所需元宝*/
    CCLabelTTF* m_needGoldForBuild;
    /*生命加成*/
    CCLabelTTF* m_hpAdditionC;
    /*攻击加成*/
    CCLabelTTF* m_atkAdditionC;
    /*铜钱加成*/
    CCLabelTTF* m_mnyAdditionC;
    /*生命加成*/
    CCLabelTTF* m_hpAdditionT;
    /*攻击加成*/
    CCLabelTTF* m_atkAdditionT;
    /*铜钱加成*/
    CCLabelTTF* m_mnyAdditionT;
    // 鼓舞
    CCLabelTTF* m_pIsHp;
    CCLabelTTF* m_pIsAtk;
    CCLabelTTF* m_pIsMny;
    // 战斗冷却计时
    CCLabelTTF* m_labelCooling;
    int m_timeForCooling;
    
};

#endif /* defined(__hero__GuoZCityLayer__) */
