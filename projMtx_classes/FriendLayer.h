//
//  FriendLayer.h
//  shop
//


#ifndef __hero__FriendLayer__
#define __hero__FriendLayer__

#include "cocos2d.h"

USING_NS_CC;

class FriendLayer : public CCLayer {
public:
    FriendLayer();
    virtual ~FriendLayer();
    
public:
    virtual bool init();
    CREATE_FUNC(FriendLayer);

public:
    // 开场动画
    void openningAni();
    //预留动作
    void option();
private:
    bool m_is1136;
};

#endif /* defined(__hero__FriendLayer__) */