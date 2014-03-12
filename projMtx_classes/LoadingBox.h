//
//  LoadingBox.h
//  hero
//
//  Created by 钱剑 on 14-1-13.
//
//

#ifndef __hero__LoadingBox__
#define __hero__LoadingBox__

#include "cocos2d.h"

USING_NS_CC;

class LoadingBox : public CCLayer {
    
public:
    LoadingBox();
    virtual ~LoadingBox();
    
public:
    virtual bool init();
    CREATE_FUNC(LoadingBox);
    
public:
    virtual void registerWithTouchDispatcher();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouches, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouches, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
public:
    void show();
    void hide();
    
    /* 网络异常调度 */
    void network_exception_schedule();
    /* 移除网络异常提示框 */
    void removeTips();
};

#endif /* defined(__hero__LoadingBox__) */
