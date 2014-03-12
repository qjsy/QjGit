//
//  MainScene.h
//  hero
//
//  Created by YANGJIE on 2013/11/28.
//
//  游戏主场景
//
//  层次结构如下
//      MainScene               -- 主场景
//          BgOfMainSceneLayer  -- 背景层
//          CityLayer           -- 主城层
//              FbLayer         -- 副本层
//              LevUpLayer      -- 武将升阶层
//          HeroLayer           -- 英雄列表层
//          ShopLayer           -- 商店层
//          FridLayer           -- 好友列表层
//          ChosLayer           -- 功能选项层
//

#ifndef __hero__MainScene__
#define __hero__MainScene__

#include "cocos2d.h"
#include "HandleNetDataDelegate.h"

USING_NS_CC;

class NetConnection;
class GameState;
class MessageBox;
class Battle;
class GamePlayer;
class HeroManager;
class MainScene : public cocos2d::CCScene , HandleNetDataDelegate{
public:
    MainScene();
    virtual ~MainScene();
    
public:
    virtual bool init();
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    virtual void onExit();
    
    CREATE_FUNC(MainScene);

public:
    /* 处理服务器回应数据 */
    virtual void handleWSResponseData(const char *, const unsigned int, char*);
    
public:
    // 响应玩家选中 “主城” 菜单
    void onMainCitySelect(CCObject*);
    // 响应玩家选中 “武将列表” 菜单
    void onHerosSelect(CCObject*);
    // 响应玩家选中 “武将列表 pvp” 菜单
    void onPvpHerosSelect(CCObject*);
    // 响应玩家选中 “商城” 菜单
    void onShopSelect(CCObject*);
    // 响应玩家选中 “好友” 菜单
    void onFriendsSelect(CCObject*);
    // 响应玩家选中 “选项” 菜单
    void onOptionSelect(CCObject*);
    // 响应玩家点击主城层的升级按钮
    void onLevupSelect(CCObject*);
    // 响应玩家点击主城层的出征按钮
    void onZhuluSelect(CCObject*);
    // 响应玩家点击主城层驿站按钮
    void onYiZhanSelect(CCObject*);
    // 响应玩家抽取英雄弹出的界面
    void onYiZhanShowSelect(CCObject*);
    // 响应选择武将阵型层按钮
    void onChosFormSelect(CCObject*);
    // 响应选择武将阵型层按钮 pvp
    void onPvpChosFormSelect(CCObject*);
    // 显示确认选择框
    void onShowChosConfirm(CCObject*);
    // 隐藏确认选择框
    void onHideChosConfirm(CCObject*);
    // 选择武将确认
    void onChosHeroConfirmOK(CCObject*);
    
    // 响应玩家点击铁匠铺
    void onChosTieJiang(CCObject*);
    // 响应玩家点击国战
    void onChosGuoZhan(CCObject*);
    // 响应国战城池点击
    void onChosGuoZhanCity(CCObject*);
    // 响应玩家点击铜雀台
    void onChosTongQue(CCObject*);
    // 响应玩家点击监狱
    void onChosJianYu(CCObject*);
    // 响应玩家点击监狱
    void onResponseForMonvOUt(CCObject*);
    // 国战新用户选择阵营
    void onChosCamp(CCObject*);
    
    // 国战选择阵型
    void onChosFormForCWar(CCObject*);
    void returnFromCWarChos(CCObject*);
    
    
    
    /*用于loading蒙泰框*/
    void showLoading(CCObject*);
    void closeLoading(CCObject*);
    /* 显示网络loading */
    void loading_schedule(CCTime);
    void network_exception(CCObject*);
    
    void testForCallback();
    
    // 响应玩家点击武将，弹出武将简介
    void onChosHeroForIntro(CCObject*);
    void onChosHeroIntroOk(CCObject*);
    void onChosJianYuHeroForIntro(CCObject*);
    
    
    /***************点击主场景按钮，恢复初始状态*****************/
    void onResetAllLayers();
    /***************点击主场景按钮，恢复初始状态*****************/

    
    
private:
    unsigned int m_currLayerTag;            // 主场景当前层的 tag 值
    
    GameState* m_pGameState;
    CCNotificationCenter* m_pNotificationCenter;
    NetConnection* m_NetConnection;
    Battle* m_pBattle;
    GamePlayer* m_pGamePlayer;
    HeroManager* m_pHeroManager;
};

#endif /* defined(__hero__MainScene__) */
