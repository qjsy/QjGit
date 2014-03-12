//
//  LoginLayer.h
//  login
//


#ifndef hero_LoginLayer_h
#define hero_LoginLayer_h

#include "cocos2d.h"
#include "cocos-ext.h"
#include "HandleNetDataDelegate.h"

USING_NS_CC;
USING_NS_CC_EXT;

class GameState;
class LoginLayer : public CCLayer , public HandleNetDataDelegate , public CCScrollViewDelegate
{  
public:
    LoginLayer();
    virtual ~LoginLayer();
    virtual bool init();
    CREATE_FUNC(LoginLayer);
    
public:
    void gameInit();
    void option(CCObject*);
    
public:
    virtual void handleWSResponseData(const char*szData, const unsigned int dataSize, char* szTag);
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    void _inertiaScroll(CCTouch *pTouch, CCEvent *pEvent);
    void _autoScroll();
    void _initComponent();
    
private:
    GameState* m_pGameState;
    
    float m_lastTime;
    CCPoint m_lastPos;
    float m_xOffset;
    float m_tempPos;
    bool m_isMoving;
    CCScrollView* m_scrollView;
    
    std::string m_lastLoginSvr;
    unsigned int m_numOfSvr;
    
    bool m_hasLoginSuccess;                     // 是否已经登录成功
    bool m_hasLoadHerosFromLocalHost;           // 是否已经加载本机武将数据
    bool m_hasLoadHerosFromServer;              // 是否已经从服务器更新武将信息
    
    unsigned int m_serverid[2];
};

#endif