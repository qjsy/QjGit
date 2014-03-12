#ifndef __hero__BattleSummary__
#define __hero__BattleSummary__

#include "cocos2d.h"

USING_NS_CC;

class BattleSummary : public CCLayer {
public:
    BattleSummary();
    virtual ~BattleSummary();
    
public:
    virtual bool init();
    CREATE_FUNC(BattleSummary);
    
public:
    virtual void setVisible(bool);

public:
    // 开场动画
    void openningAni();
    //预留动作
    void option();
    void updateUi();
    // 确认按钮，回到副本层
    void btnConfirm();
    
private:
    /*结算动画*/
    void _summaryAni();
    
    void _expAni();
    
    void _scheduleForExp();
    
    void _copAni();
    
    void _scheduleForCop();
    
    void _levAni();
    
    void _scheduleForLev();
    
    void _heroAni();
    
    void _scheduleForHero();
    
    void _prisonHeroAni();
    
    void _updateUiData();
    
private:
    bool m_isSummary;           // 是否是在进行战斗物品掉落结算，true 是。false 友情结算。
    int m_showExp;
    int m_cop;
    int m_lev;                  // 玩家当前等级
    int m_curLev;
    int m_oldLev;
    float m_pro;                  // 进度条变化
    int m_heroIdx;
    
    /* 不一样的我 */
    int m_curExp;                 // 玩家升级后当前的经验值
    int m_oldExp;                 // 玩家升级前的exp
};

#endif /* defined(__hero__BattleSummary__) */