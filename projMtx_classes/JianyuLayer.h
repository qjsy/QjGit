//
//  JianyuLayer.h
//  hero
//
//  Created by 钱剑 on 2014／02／19.
//
//

#ifndef __hero__JianyuLayer__
#define __hero__JianyuLayer__

#include "CCHeroCommonLayer.h"

USING_NS_CC;

#define JY_BTN_COUNT            4               // 菜单按钮数量

class GameState;
class HeroManager;
class GamePlayer;
class CardBox;
class PrisonHeroItem;
class JianyuLayer : public CCHeroCommonLayer {
public:
    JianyuLayer();
    virtual ~JianyuLayer();
    
public:
    virtual bool init();
    CREATE_FUNC(JianyuLayer);
    
public:
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    virtual void update(float dt);
    
    /*初始化位置*/
    virtual void initPosition();
    /*移出动作*/
    virtual void moveOutTouchUICompent();
    /*恢复响应动作*/
    virtual void restoreTouchUICompent();
    
public:
    /* 刷新监狱显示 */
    void updateAllOfPrisonBySummon();
    /* 释放武将刷新 */
    void updateAllOfPrisonByFree();

private:
    void _initUiComponent();                    /* 初始化 ui 组件 */
    void _initBtn();                            /* 初始化按钮 */
    void _onBtnSelect(const unsigned int btnIdx);
    void _tipMenu(int);                         /**/
    void _callbackForMoveOut();                 /* 移出回调 */
    void _changeLoyalAni();                     /* 改变忠诚度 */
    void _schduleForLoyal();
    void _setLoyal(PrisonHeroItem*);            /* 设置忠诚度UI */
    void _runLoyal();
    void _confirmFree();                        /* 确认释放武将 */
    void _updatePages();                        /* 更新武将个数 */
    void _updateGamePlayerProp();                           /* 更新UI显示 */
    void _updateTips();                         /* 更新tips */
    
    void _updateNeed(PrisonHeroItem*);
    
    void _confirmUseGold();                      /*确定话费元宝招降武将*/
    void _disableBtn(const unsigned int, const unsigned int);       /* 根据当前监狱武将数量和当前监狱武将索引确定按钮状态 */

    /* 更新监狱武将相关 ui */
    void _updatePrisonHeroItemUi(const bool loyaltyAni = false);
    /* 翻页 */
    void _turnPagePrisonHeroItem(const unsigned int, int);
    
private:
    GameState* m_pGameState;
    CCSpriteBatchNode* m_pJyCBN;                    /* 批处理精灵 */
    HeroManager* m_pHeroManager;                    /* 武将管理 */
    GamePlayer* m_pGamePlayer;
    CCSpriteFrameCache* m_pSpriteFrameCache;
    
    std::vector<std::string> m_prisonHerosImage;    /* 存放所有监狱武将头像文件名 */
    
    CCPoint m_btnPosition[JY_BTN_COUNT];            /* 按钮位置 */
    CCSprite* m_btnSprite[JY_BTN_COUNT][3];         /* 按钮精灵 */
    
    bool m_btnEnable[JY_BTN_COUNT];                 /* 按钮是否可点击 */
    
    int m_touchBeganBtnIdx;                         /* 点击起始索引 */
    
    CCRect m_heroRect;
    
    CCPoint m_touchHeroBeganPoint;                  /* 记录起始点位置 */
    unsigned int m_countOfMoves;                    /*  */
    
    CCSprite* m_pDangSprite;                    
    CardBox* m_currHeroSprite;                      /* 当前监狱武将大卡头像 */
    CardBox* m_heroSprite1;
    CardBox* m_heroSprite2;
    CCLabelTTF* m_pagesLable;
    
    unsigned long int m_touchHeroBeganTime;         /* 保存 touch 武将的开始时间 */
    
    bool m_isTrunPageing;                           /* 是否整在翻页 */
    unsigned int m_turnPageDirection;               /* 翻页的方向，1:右－>左(后翻) 2:左－>右(前翻) */
    unsigned int m_turnPageTargetHeroIdx;           /* 翻页结束时的监狱武将索引值 */
    
    unsigned int m_countOfPrisonHeros;              /* 当前监狱武将的数量 */
    int m_curHeroIdx;                               /* 当前显示的监狱武将索引(0) */
    int m_oldLoyal;                                 /* 忠诚度 */
    
    CCLabelTTF* pLoyaltyLabel;                      /* 忠诚度系统文字 */
    CCLabelTTF* pCopperLabel;                       /* 铜币 */
    CCLabelTTF* pPropOrGold;                        /* 材料或者元宝 */
    CCProgressTimer* pLoyalProgress;                /* 忠诚度进度条 */
    CCSprite* pGoldSprite;                          /* 所需要的元宝ICON */
    CCSprite* pPropSPrite;                          /* 所需要的材料ICON */
    
    
};

#endif /* defined(__hero__JianyuLayer__) */
