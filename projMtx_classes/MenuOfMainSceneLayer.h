//
//  MenuOfMainSceneLayer.h
//  hero
//
//  Created by yangjie on 2013/11/28.
//
//  主场景的菜单选择层
//  用于切换主场景的各功能选项
//

#ifndef __hero__MenuOfMainSceneLayer__
#define __hero__MenuOfMainSceneLayer__

#include "CCHeroCommonLayer.h"

USING_NS_CC;

class MenuOfMainSceneLayer : public CCHeroCommonLayer {
public:
    MenuOfMainSceneLayer();
    virtual ~MenuOfMainSceneLayer();
    
public:
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual void registerWithTouchDispatcher(void);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    CREATE_FUNC(MenuOfMainSceneLayer);

public:
    /* 初始化位置 */
    virtual void initPosition();
    
    /* 将能响应 touch 事件的 ui 组件移动到屏幕外面 */
    virtual void moveOutTouchUICompent();
    
    /* 恢复能响应 touch 事件的 ui 组件的位置 */
    virtual void restoreTouchUICompent();
    
    /* moveOut 动画完成回调 */
    void moveOutIsDoneCallback();
    
    /* restore 动画完成回调 */
    void restoreIsDoneCallback();
    
private:
    /* 响应玩家菜单选择 */
    void _onMenuSelected(const unsigned int);

private:
    bool m_enableTouch;                     // 是否响应 touch
    CCSpriteBatchNode* m_pMainMenuCBN;
    CCSprite* m_pMenuBackgroundSprite;      // 背景框
    CCSprite* m_menuSelected;
    CCSprite* m_pMenuBar[5];
    CCSprite* m_pMenuFont[5];               // 菜单文字
    CCSprite* m_pMenuBarBox[5];
    
    bool m_isMoveOut;                       // 是否正在 moveOut
    bool m_isRestore;                       // 是否正在 restore
    
    bool m_isInScreen;                       // 是否已经在场景内
};

#endif /* defined(__hero__MenuOfMainSceneLayer__) */
