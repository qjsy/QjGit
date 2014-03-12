#ifndef __hero__BattleScene__
#define __hero__BattleScene__

#include "cocos2d.h"

USING_NS_CC;

class NetConnection;
class BattleScene : public cocos2d::CCScene
{
public:
    BattleScene();
    virtual ~BattleScene();
    
public:
    virtual bool init();
    CREATE_FUNC(BattleScene);
    
    virtual void onEnter();
    virtual void onExit();
    
public:
    void onBattleSummary(CCObject*);
    void showLoading(CCObject*);
    void closeLoading(CCObject*);
    void onPvpBattleSummary(CCObject*);
    /* 显示网络loading */
    void loading_schedule(CCTime);
    /* 网络异常 */
    void network_exception(CCObject*);
    
private:
    NetConnection* m_pNetConnection;
};

#endif