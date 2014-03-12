//
//  ShopLayer.h
//  hero
//

#ifndef __hero__ShopLayer__
#define __hero__ShopLayer__

#include "CCHeroCommonLayer.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class ShopLayer : public CCHeroCommonLayer {
public:
    ShopLayer();
    virtual ~ShopLayer();
    
public:
    virtual bool init();
    CREATE_FUNC(ShopLayer);

public:
    /*开场动画*/
    virtual void openningAni();
    /*移出动作*/
    virtual void moveOutTouchUICompent();
    /*恢复响应动作*/
    virtual void restoreTouchUICompent();
    
private:
    bool m_is1136;
};

#endif /* defined(__hero__ShopLayer__) */