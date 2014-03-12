#ifndef __hero__Demo__
#define __hero__Demo__

#include "cocos2d.h"

USING_NS_CC;

class Demo : public CCLayer {
public:
    Demo();
    virtual ~Demo();
    
public:
    virtual bool init();
    CREATE_FUNC(Demo);
    
public:
    // 开场动画
    void openningAni();
    //预留动作
    void option();
};

#endif /* defined(__hero__Demo__) */