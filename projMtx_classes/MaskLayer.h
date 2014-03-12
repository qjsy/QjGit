//
//  MaskLayer.h
//  hero
//
//  Created by 钱剑 on 14-1-13.
//
//

#ifndef __hero__MaskLayer__
#define __hero__MaskLayer__

#include "cocos2d.h"

USING_NS_CC;

class MaskLayer : public CCLayer {
    
public:
    MaskLayer();
    virtual ~MaskLayer();
    
public:
    virtual bool init();
    CREATE_FUNC(MaskLayer);
    
    virtual bool ccTouchBegan(CCTouch *pTouches, CCEvent *pEvent);
    virtual void registerWithTouchDispatcher();
};


#endif /* defined(__hero__MaskLayer__) */
