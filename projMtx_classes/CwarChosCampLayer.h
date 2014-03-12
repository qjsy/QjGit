
//
//  CwarChosCampLayer.h
//  hero
//
//  Created by 钱剑 on 14-3-4.
//
//

#ifndef __hero__CwarChosCampLayer__
#define __hero__CwarChosCampLayer__

#include <iostream>

#include "cocos2d.h"

USING_NS_CC;

class GameState;
class CwarChosCampLayer : public CCLayer {
public:
    CwarChosCampLayer();
    virtual ~CwarChosCampLayer();
    
public:
    virtual bool init();
    CREATE_FUNC(CwarChosCampLayer);
    
public:
    virtual void registerWithTouchDispatcher(void);
    virtual bool ccTouchBegan(CCTouch*, CCEvent*);
    virtual void ccTouchMoved(CCTouch*, CCEvent*);
    virtual void ccTouchEnded(CCTouch*, CCEvent*);
    virtual void ccTouchCancelled(CCTouch*, CCEvent*);
    
private:
    void _initBtn();
    void _onSelectBtn(int);
    void _chosConfirm();
private:
    GameState* m_pGameState;
    
    CCSprite* m_btnSp[3][2];
    CCSprite* m_confirmSp[2];
    
    int m_touchBeganIdx;
};

#endif /* defined(__hero__CwarChosCampLayer__) */