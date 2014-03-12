//
//  LoginScene.h
//  hero
//
//  Created by 杨杰 on 13-11-28.
//
//

#ifndef __hero__LoginScene__
#define __hero__LoginScene__

#include "cocos2d.h"
#include "HandleNetDataDelegate.h"

//class LoginLayer;
USING_NS_CC;

class NetConnection;
class LoginScene : public cocos2d::CCScene {
public:
    LoginScene();
    virtual ~LoginScene();
    
public:
    virtual bool init();
    CREATE_FUNC(LoginScene);
    
public:
    virtual void onEnter();
    virtual void onExit();
    
public:
    void showLoading(CCObject*);
    void closeLoading(CCObject*);
    /* 显示网络loading */
    void loading_schedule(CCTime);
    void network_exception(CCObject*);
private:
    NetConnection* m_pNetConnection;
};

#endif /* defined(__hero__LoginScene__) */
