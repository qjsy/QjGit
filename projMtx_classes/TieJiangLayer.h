//
//  TieJiangLayer.h
//  hero
//
//  Created by 钱剑 on 14-3-10.
//
//

#ifndef __hero__TieJiangLayer__
#define __hero__TieJiangLayer__

#include "CCHeroCommonLayer.h"
USING_NS_CC;


class GameState;
class TieJiangLayer : public CCHeroCommonLayer {
public:
    TieJiangLayer();
    virtual ~TieJiangLayer();
    
public:
    virtual bool init();
    CREATE_FUNC(TieJiangLayer);
    
public:
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    /*初始化位置*/
    virtual void initPosition();
    /*移出动作*/
    virtual void moveOutTouchUICompent();
    /*恢复响应动作*/
    virtual void restoreTouchUICompent();
    
private:
    /*移出回调*/
    void _callbackForMoveOut();
    /*初始化UI组件*/
    void _initUi();
    /*初始化筛选按钮*/
    void _initFilterBtn();
    /*响应筛选按钮*/
    void _onBtnSelected(int);
public:
    
private:
    GameState* m_pGameState;
    
    /*filter btn*/
    CCSprite* m_font[4][2];
    CCSprite* m_btn[4][2];
    bool m_enableTouch[4];
    int m_touchBeganBtnidx;
    
};


#endif /* defined(__hero__TieJiangLayer__) */
