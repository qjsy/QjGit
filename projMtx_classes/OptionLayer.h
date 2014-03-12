//
//  OptionLayer.h
//  shop
//


#ifndef __hero__OptionLayer__
#define __hero__OptionLayer__

#include "cocos2d.h"

USING_NS_CC;

class OptionLayer : public CCLayer {
public:
    OptionLayer();
    virtual ~OptionLayer();
    
public:
    virtual bool init();
    CREATE_FUNC(OptionLayer);

public:
    // 开场动画
    void openningAni();
    //预留动作
    void option();
private:
    bool m_is1136;

};

#endif /* defined(__hero__OptionLayer__) */