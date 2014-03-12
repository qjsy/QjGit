//
//  MainScene.cpp
//  hero
//
//  Created by yangjie on 2013/11/28.
//
//
#include "MainScene.h"
#include "BgOfMainSceneLayer.h"             // 背景
#include "MenuOfMainSceneLayer.h"           // 菜单选项
#include "MainCityLayer.h"                  // 主城
#include "HerosLayer.h"                     // 武将列表
#include "ShopLayer.h"                      // 商店
#include "FriendLayer.h"                    // 好友
#include "OptionLayer.h"                    // 选项
#include "GoBattleLayer.h"                  // 上阵武将层
#include "LevUpLayer.h"                     // 升级层 ，由主城层掉入的
#include "ZhuLuLayer.h"                     // 出征
#include "ChosFormLayer.h"
#include "ChosConfirm.h"                    // 确认层
#include "BattleScene.h"                    // 战斗场景
#include "YiZhanLayer.h"                    // 驿站层
#include "YiZhanShowLayer.h"                // 抽取英雄显示层
#include "HeroIntroduce.h"                  // 武将简介显示层
#include "SellHerosLayer.h"
#include "GamePlayer.h"
#include "LoadingBox.h"
#include "MessageBox.h"                     // 消息框
#include "TieJiangLayer.h"

#include "CwarChosCampLayer.h"

#include "PvpSummary.h"

#include "LevelDataItem.h"
#include "CopyDataItem.h"
#include "CopyManager.h"

#include "JianyuLayer.h"
#include "GuoZCityLayer.h"

#include "common.h"
#include "NetConnection.h"
#include "customMessage.h"

#include "HeroManager.h"
#include "HeroOfPlayerItem.h"

#include "Battle.h"
#include "GameState.h"
#include "ProcessData.h"
#include "CCHeroCommonLayer.h"

#include "TongQueLayer.h"
#include "GuoZhanLayer.h"

#include "CardBox.h"

// tag
#define BG_LAYER_TAG                    11      // “背景” 层 tag 值
#define MENUBAR_LAYER_TAG               12      // “菜单条” 层 tag 值

#define MAINCITY_LAYER_TAG              111     // “主城” 层 tag
#define HEROS_LAYER_TAG                 112     // “武将列表” 层 tag
#define SHOP_LAYER_TAG                  113     // “商城” 层 tag
#define FRIEND_LAYER_TAG                114     // “好友” 层 tag
#define OPTION_LAYER_TAG                115     // “选项” 层 tag
#define GOBAT_LAYER_TAG                 116     // “武将上阵” 层 tag
#define LEVUP_LAYER_TAG                 117     // “升级” 层 tag
#define ZHULU_LAYER_TAG                 118     // “出征” 层 tag
#define YIZHAN_LAYER_TAG                121     // “驿站” 层 tag
#define SELL_LAYER_TAG                  123     // “出售武将层”tag
#define YIZHAN_SHOW_TAG                 122
#define CHOSCONFIRM_LAYER_TAG           119     // “选择确认” 层 tag
#define CHOS_FORM_TAG                   120     // “阵型选择” 层 tag

#define JIANYU_LAYER                    200     // 监狱层 Tag
#define TONGQUE_LAYER                   201     // 铜雀台 Tag
#define GUOZHAN_LAYER                   202     // 国战 Tag
#define GUOZHAN_CITY_LAYER              203     // 国战城池

#define TIEJIANG_LAYER                  210     // 铁匠铺 Tag

#define HERO_INTRO_LAYER                204     // 武将简介层 Tag
//
#define CLOSING_ANI_DURING              0.2f
#define MASK_LAYER_TAG                  500

/*
 *  构造函数
 */
MainScene::MainScene():m_currLayerTag(MAINCITY_LAYER_TAG)
,m_pNotificationCenter(CCNotificationCenter::sharedNotificationCenter())
,m_pGameState(GameState::getInstance())
,m_NetConnection(NetConnection::getInstance())
,m_pBattle(Battle::getInstance())
,m_pGamePlayer(GamePlayer::getInstance())
,m_pHeroManager(HeroManager::getInstance())
{}

/*
 *  析构函数
 */
MainScene::~MainScene() {
    // 去除消息监听
}

/*
 *   主场景初始化，加载各个子层
 */
bool MainScene::init() {
    // super init first
    if (!CCScene::init()) {
        return false;
    }
    // 添加 "玩家信息层" 层
    CCLayer* pBgOfMainSceneLayer = BgOfMainSceneLayer::create();
    this->addChild(pBgOfMainSceneLayer, 10, BG_LAYER_TAG);
    
    //添加 "武将上阵" 层
    GoBattleLayer* pGoBattleLayer = GoBattleLayer::create();
    pGoBattleLayer->initPosition();
    this->addChild(pGoBattleLayer, 4, GOBAT_LAYER_TAG);
    
    // 添加 “菜单选项” 层
    CCLayer* pMenuOfMainSceneLayer = MenuOfMainSceneLayer::create();
    this->addChild(pMenuOfMainSceneLayer, 10, MENUBAR_LAYER_TAG);
    
    // 添加 “主城” 层
    CCLayer* pMainCityLayer = MainCityLayer::create();
    this->addChild(pMainCityLayer, 3, MAINCITY_LAYER_TAG);
    
    // 添加 “武将列表” 层
    HerosLayer* pHerosLayer = HerosLayer::create();
    pHerosLayer->initPosition();
    pHerosLayer->setVisible(false);
    this->addChild(pHerosLayer, 3, HEROS_LAYER_TAG);
    
    // 添加 "选择确认层"
    ChosConfirm* pChosConfirmLayer = ChosConfirm::create();
    this->addChild(pChosConfirmLayer, 3, CHOSCONFIRM_LAYER_TAG);
    pChosConfirmLayer->initPosition();
    pChosConfirmLayer->setVisible(false);
    
    // 添加 “商城” 层
    //    CCLayer* pShopLayer = ShopLayer::create();
    //    pSellLayer->setPosition(ccp(0, -1136));
    //    this->addChild(pShopLayer, 3, SHOP_LAYER_TAG);
    
    //    // 添加 “好友” 层
    //    CCLayer* pFriendLayer = FriendLayer::create();
    //    pFriendLayer->setVisible(false);
    //    this->addChild(pFriendLayer, 3, FRIEND_LAYER_TAG);
    
    //    // 添加 “选项” 层
    //    CCLayer* pOptionLayer = OptionLayer::create();
    //    pOptionLayer->setVisible(false);
    //    this->addChild(pOptionLayer, 3, OPTION_LAYER_TAG);
    
    /******************TEST****************/
    //    schedule(schedule_selector(MainScene::test), 3);
    
    
 
    
    return true;
}

void MainScene::onEnter() {
    // super init
    CCScene::onEnter();
    
    m_NetConnection->setHandleNetDataDelegate(this);
    
    // 注册响应玩家菜单选择消息的回调方法 ON_MESSAGE_SHOWYIZHAN_STRING
    
    m_pNotificationCenter->addObserver(this, callfuncO_selector(MainScene::onMainCitySelect), ON_MESSAGE_MAINCITY_STRING, NULL);
    m_pNotificationCenter->addObserver(this,
                                       callfuncO_selector(MainScene::onHerosSelect), ON_MESSAGE_HEROS_STRING, NULL);
    m_pNotificationCenter->addObserver(this,
                        callfuncO_selector(MainScene::onPvpHerosSelect), ON_MESSAGE_HEROS_PVP_STRING, NULL);
    
    m_pNotificationCenter->addObserver(this, callfuncO_selector(MainScene::onShopSelect), ON_MESSAGE_SHOP_STRING, NULL);
    m_pNotificationCenter->addObserver(this, callfuncO_selector(MainScene::onFriendsSelect), ON_MESSAGE_FRIENDS_STRING, NULL);
    m_pNotificationCenter->addObserver(this, callfuncO_selector(MainScene::onOptionSelect), ON_MESSAGE_OPTIION_STRING, NULL);
    m_pNotificationCenter->addObserver(this, callfuncO_selector(MainScene::onLevupSelect), ON_MESSAGE_LEVUP_STRING, NULL);
    m_pNotificationCenter->addObserver(this, callfuncO_selector(MainScene::onZhuluSelect), ON_MESSAGE_ZHULU_STRING, NULL);
    
    m_pNotificationCenter->addObserver(this, callfuncO_selector(MainScene::onChosFormSelect), ON_MESSAGE_CHOSFORM_STRING, NULL);
    m_pNotificationCenter->addObserver(this, callfuncO_selector(MainScene::onPvpChosFormSelect), ON_MESSAGE_CHOSFORM_PVP_STRING, NULL);
    
    
    m_pNotificationCenter->addObserver(this, callfuncO_selector(MainScene::onShowChosConfirm),
                                       ON_MESSAGE_CHOSCONFIRM_SHOW_STRING, NULL);
    m_pNotificationCenter->addObserver(this, callfuncO_selector(MainScene::onHideChosConfirm),
                                       ON_MESSAGE_CHOSCONFIRM_HIDE_STRING, NULL);
    m_pNotificationCenter->addObserver(this, callfuncO_selector(MainScene::onChosHeroConfirmOK),
                                       ON_MESSAGE_CHOSCONFIRM_OK_STRING, NULL);
    m_pNotificationCenter->addObserver(this, callfuncO_selector(MainScene::onYiZhanSelect), ON_MESSAGE_YIZHAN_STRING, NULL);
    m_pNotificationCenter->addObserver(this, callfuncO_selector(MainScene::onYiZhanShowSelect), ON_MESSAGE_SHOWYIZHAN_STRING, NULL);
    
    /*loading*/
    m_pNotificationCenter->addObserver(this, callfuncO_selector(MainScene::showLoading), ON_MESSAGE_LOADING_MAINSCENE, NULL);
    m_pNotificationCenter->addObserver(this, callfuncO_selector(MainScene::closeLoading), ON_MESSAGE_SHUTDOWN_MAINSCENE, NULL);
    /*network*/
    m_pNotificationCenter->addObserver(this, callfuncO_selector(MainScene::network_exception), ON_MESSAGE_NETWORK_EXCEPTION, NULL);
    /*铁匠铺，出征沙场，铜雀台*/
    m_pNotificationCenter->addObserver(this, callfuncO_selector(MainScene::onChosTieJiang), ON_MESSAGE_TIEJIANG_STRING, NULL);
    m_pNotificationCenter->addObserver(this, callfuncO_selector(MainScene::onChosGuoZhan), ON_MESSAGE_GUOZHAN_STRING, NULL);
    m_pNotificationCenter->addObserver(this, callfuncO_selector(MainScene::onChosTongQue), ON_MESSAGE_TONGQUETAI_STRING, NULL);
    /* 监狱系统 */
    m_pNotificationCenter->addObserver(this, callfuncO_selector(MainScene::onChosJianYu), ON_MESSAGE_JIANYU_STRING, NULL);
    m_pNotificationCenter->addObserver(this, callfuncO_selector(MainScene::onResponseForMonvOUt), ON_MESSAGE_MOVEGOBATTLE_STRING, NULL);
    m_pNotificationCenter->addObserver(this, callfuncO_selector(MainScene::onChosGuoZhanCity), ON_MESSAGE_GUOZHANCITY_STRING, NULL); /* 国战城池kknd */
    
    
    m_pNotificationCenter->addObserver(this, callfuncO_selector(MainScene::onChosFormForCWar), ON_MESSAGE_CWARCHOSFORM_STRING, NULL); /**国战PVP*/
    m_pNotificationCenter->addObserver(this, callfuncO_selector(MainScene::returnFromCWarChos), ON_MESSAGE_CWARRETURN_STRING, NULL);
    
    m_pNotificationCenter->addObserver(this, callfuncO_selector(MainScene::onChosCamp), ON_MESSAGE_GZCHOSCAMP_STRING, NULL); /* 选择阵营 */
    /*判断是否是战斗返回到逐鹿层、国战或者铜雀台*/
    if (m_pGameState->getTagWhoCallMainScene() == 2)/*逐鹿层*/ {
        m_pNotificationCenter->postNotification(ON_MESSAGE_ZHULU_STRING);
    } else if (m_pGameState->getTagWhoCallMainScene() == 3)/*铜雀台*/ {
        m_pNotificationCenter->postNotification(ON_MESSAGE_TONGQUETAI_STRING);
    } else if (m_pGameState->getTagWhoCallMainScene() == 4)/*国战*/ {
        m_pNotificationCenter->postNotification(ON_MESSAGE_GUOZHAN_STRING);
    }
    
    /*武将列表的武将简介*/
    m_pNotificationCenter->addObserver(this, callfuncO_selector(MainScene::onChosHeroForIntro), ON_MESSAGE_HEROINTRO_STRING, NULL);
    /*武将简介点击确定按钮*/
    m_pNotificationCenter->addObserver(this, callfuncO_selector(MainScene::onChosHeroIntroOk), ON_MESSAGE_HEROINTRO_OK_STRING , NULL);
    /*监狱武将的简介*/
     m_pNotificationCenter->addObserver(this, callfuncO_selector(MainScene::onChosJianYuHeroForIntro), ON_MESSAGE_JIANY_HEROINTRO_STRING, NULL);
}

/*
 *
 */
void MainScene::onEnterTransitionDidFinish() {
    CCScene::onEnterTransitionDidFinish();
}

//
void MainScene::onExit() {
    // super
    CCScene::onExit();
    
    m_NetConnection->setHandleNetDataDelegate(NULL);
    
    // 去除响应玩家菜单选择消息
    m_pNotificationCenter->removeObserver(this, ON_MESSAGE_MAINCITY_STRING);
    m_pNotificationCenter->removeObserver(this, ON_MESSAGE_HEROS_STRING);
    m_pNotificationCenter->removeObserver(this, ON_MESSAGE_SHOP_STRING);
    m_pNotificationCenter->removeObserver(this, ON_MESSAGE_FRIENDS_STRING);
    m_pNotificationCenter->removeObserver(this, ON_MESSAGE_OPTIION_STRING);
    m_pNotificationCenter->removeObserver(this, ON_MESSAGE_LEVUP_STRING);
    m_pNotificationCenter->removeObserver(this, ON_MESSAGE_ZHULU_STRING);
    m_pNotificationCenter->removeObserver(this, ON_MESSAGE_CHOSFORM_STRING);
    m_pNotificationCenter->removeObserver(this, ON_MESSAGE_CHOSCONFIRM_SHOW_STRING);
    m_pNotificationCenter->removeObserver(this, ON_MESSAGE_CHOSCONFIRM_HIDE_STRING);
    m_pNotificationCenter->removeObserver(this, ON_MESSAGE_CHOSCONFIRM_OK_STRING);
    m_pNotificationCenter->removeObserver(this, ON_MESSAGE_YIZHAN_STRING);
    m_pNotificationCenter->removeObserver(this, ON_MESSAGE_SHOWYIZHAN_STRING);
    m_pNotificationCenter->removeObserver(this, ON_MESSAGE_LOADING_MAINSCENE);
    m_pNotificationCenter->removeObserver(this, ON_MESSAGE_SHUTDOWN_MAINSCENE);
    m_pNotificationCenter->removeObserver(this, ON_MESSAGE_NETWORK_EXCEPTION);
    m_pNotificationCenter->removeObserver(this, ON_MESSAGE_TIEJIANG_STRING);
    m_pNotificationCenter->removeObserver(this, ON_MESSAGE_GUOZHAN_STRING);
    m_pNotificationCenter->removeObserver(this, ON_MESSAGE_TONGQUETAI_STRING);
    m_pNotificationCenter->removeObserver(this, ON_MESSAGE_JIANYU_STRING);
    m_pNotificationCenter->removeObserver(this, ON_MESSAGE_MOVEGOBATTLE_STRING);
    m_pNotificationCenter->removeObserver(this, ON_MESSAGE_CHOSFORM_PVP_STRING);
    m_pNotificationCenter->removeObserver(this, ON_MESSAGE_HEROS_PVP_STRING);
    
    m_pNotificationCenter->removeObserver(this, ON_MESSAGE_CWARCHOSFORM_STRING);
    m_pNotificationCenter->removeObserver(this, ON_MESSAGE_CWARRETURN_STRING);
    m_pNotificationCenter->removeObserver(this, ON_MESSAGE_HEROINTRO_STRING);
    m_pNotificationCenter->removeObserver(this, ON_MESSAGE_HEROINTRO_OK_STRING);
    m_pNotificationCenter->removeObserver(this, ON_MESSAGE_JIANY_HEROINTRO_STRING);

}

/*
 *   响应玩家选中 “主城” 菜单
 */
void MainScene::onMainCitySelect(CCObject* obj) {
    if (MAINCITY_LAYER_TAG == m_currLayerTag) {
        return;
    }
    
    /* 设置主城调用*/
    m_pGameState->setIsMainCityCall(true);
    m_pBattle->setBattlePkMode(BPM_PVE);
    /*武将列表置空,不置空会出错*/
    m_pGameState->setTagWhoCallHeroSel(WCH_NONE);
    
    /*判断武将上阵层是否在场景内*/
    GoBattleLayer* pGoBattleLayer = (GoBattleLayer*)this->getChildByTag(GOBAT_LAYER_TAG);
    if (!pGoBattleLayer->isInScreen()) {
        pGoBattleLayer->restoreTouchUICompent();
    }
    
    /*移除原来层*/
    CCLayer* pOldLayer = (CCLayer*)this->getChildByTag(m_currLayerTag);
    if (m_currLayerTag == LEVUP_LAYER_TAG || m_currLayerTag == YIZHAN_LAYER_TAG
        || m_currLayerTag == ZHULU_LAYER_TAG || m_currLayerTag == JIANYU_LAYER) {
        CCHeroCommonLayer* pHeroCommonLayer = dynamic_cast<CCHeroCommonLayer*>(pOldLayer);
        pHeroCommonLayer->setRemoveSelf(true);
        pHeroCommonLayer->moveOutTouchUICompent();
    } else {
        CCHeroCommonLayer* pHeroCommonLayer = dynamic_cast<CCHeroCommonLayer*>(pOldLayer);
        pHeroCommonLayer->moveOutTouchUICompent();
    }
    
    /*显示当前层*/
    m_currLayerTag = MAINCITY_LAYER_TAG;
    MainCityLayer* pCurrLayer = (MainCityLayer*)this->getChildByTag(m_currLayerTag);
    pCurrLayer->restoreTouchUICompent();
    
    
    /*重置主场景*/
    onResetAllLayers();
    
}

/*
 *  响应玩家选中 “主城-升级” 菜单
 */
void MainScene::onLevupSelect(CCObject* obj) {
    if (LEVUP_LAYER_TAG == m_currLayerTag) {
        return;
    }
    
    /*判断武将上阵层是否在场景内*/
    GoBattleLayer* pGoBattleLayer = (GoBattleLayer*)this->getChildByTag(GOBAT_LAYER_TAG);
    if (pGoBattleLayer->isInScreen()) {
        pGoBattleLayer->moveOutTouchUICompent();
    }
    
    /*移除原来层*/
    CCLayer* pOldLayer = (CCLayer*)this->getChildByTag(m_currLayerTag);
    CCHeroCommonLayer* pHeroCommonLayer = dynamic_cast<CCHeroCommonLayer*>(pOldLayer);
    pHeroCommonLayer->moveOutTouchUICompent();
    
    /*显示当前层*/
    m_currLayerTag = LEVUP_LAYER_TAG;
    LevUpLayer* pLevLayer = (LevUpLayer*)this->getChildByTag(LEVUP_LAYER_TAG);
    if (!pLevLayer) {
        pLevLayer = LevUpLayer::create();
        this->addChild(pLevLayer, 3, LEVUP_LAYER_TAG);
        pLevLayer->initPosition();
    }
    pLevLayer->restoreTouchUICompent();
}


/*
 *  响应玩家选中 “主城-出征” 菜单
 */
void MainScene::onZhuluSelect(CCObject* obj) {
    if (ZHULU_LAYER_TAG == m_currLayerTag) {
        return;
    }
    
    /*判断武将上阵层是否在场景内*/
    GoBattleLayer* pGoBattleLayer = (GoBattleLayer*)this->getChildByTag(GOBAT_LAYER_TAG);
    if (pGoBattleLayer->isInScreen()) {
        pGoBattleLayer->moveOutTouchUICompent();
    }
    
    /*移除原来层*/
    CCLayer* pOldLayer = (CCLayer*)this->getChildByTag(m_currLayerTag);
    CCHeroCommonLayer* pHeroCommonLayer = dynamic_cast<CCHeroCommonLayer*>(pOldLayer);
    pHeroCommonLayer->moveOutTouchUICompent();
    
    /*显示当前层*/
    m_currLayerTag = ZHULU_LAYER_TAG;
    ZhuLuLayer* pFbLayer = (ZhuLuLayer*)this->getChildByTag(ZHULU_LAYER_TAG);
    if (!pFbLayer) {
        pFbLayer = ZhuLuLayer::create();
        pFbLayer->initPosition();
        this->addChild(pFbLayer, 3, ZHULU_LAYER_TAG);
    }
    pFbLayer->restoreTouchUICompent();
}

/*选择驿站*/
void MainScene::onYiZhanSelect(CCObject* obj) {
    if (YIZHAN_LAYER_TAG == m_currLayerTag) {
        return;
    }
    
    /*判断武将上阵层是否在场景内*/
    GoBattleLayer* pGoBattleLayer = (GoBattleLayer*)this->getChildByTag(GOBAT_LAYER_TAG);
    if (pGoBattleLayer->isInScreen()) {
        pGoBattleLayer->moveOutTouchUICompent();
    }
    /*移除原来层*/
    CCLayer* pOldLayer = (CCLayer*)this->getChildByTag(m_currLayerTag);
    if (m_currLayerTag == YIZHAN_SHOW_TAG) {
        CCHeroCommonLayer* pHeroCommonLayer = dynamic_cast<CCHeroCommonLayer*>(pOldLayer);
        pHeroCommonLayer->setRemoveSelf(true);
        pHeroCommonLayer->moveOutTouchUICompent();
    } else {
        CCHeroCommonLayer* pHeroCommonLayer = dynamic_cast<CCHeroCommonLayer*>(pOldLayer);
        pHeroCommonLayer->moveOutTouchUICompent();
    }
    
    /*显示当前层*/
    m_currLayerTag = YIZHAN_LAYER_TAG;
    YiZhanLayer* pYiZhanLayer = (YiZhanLayer*)this->getChildByTag(YIZHAN_LAYER_TAG);
    if (!pYiZhanLayer) {
        pYiZhanLayer = YiZhanLayer::create();
        this->addChild(pYiZhanLayer, 3, YIZHAN_LAYER_TAG);
        pYiZhanLayer->initPosition();
    }
    pYiZhanLayer->restoreTouchUICompent();
}

/*驿站显示层*/
void MainScene::onYiZhanShowSelect(CCObject* obj) {
    if (YIZHAN_SHOW_TAG == m_currLayerTag) {
        return;
    }
    /*判断武将上阵层是否在场景内*/
    GoBattleLayer* pGoBattleLayer = (GoBattleLayer*)this->getChildByTag(GOBAT_LAYER_TAG);
    if (pGoBattleLayer->isInScreen()) {
        pGoBattleLayer->moveOutTouchUICompent();
    }
    /*移除原来层*/
    CCLayer* pOldLayer = (CCLayer*)this->getChildByTag(m_currLayerTag);
    CCHeroCommonLayer* pHeroCommonLayer = dynamic_cast<CCHeroCommonLayer*>(pOldLayer);
    pHeroCommonLayer->moveOutTouchUICompent();
    
    /*显示当前层*/
    m_currLayerTag = YIZHAN_SHOW_TAG;
    YiZhanShowLayer* pShowYiLayer = (YiZhanShowLayer*)this->getChildByTag(YIZHAN_SHOW_TAG);
    if (!pShowYiLayer) {
        pShowYiLayer = YiZhanShowLayer::create();
        this->addChild(pShowYiLayer, 5000, YIZHAN_SHOW_TAG);
        pShowYiLayer->initPosition();
    }
    pShowYiLayer->restoreTouchUICompent();
}

/* 从出征层选择武将阵型层 */
void MainScene::onChosFormSelect(CCObject* obj) {
    if (CHOS_FORM_TAG == m_currLayerTag) {
        return;
    }
    
    /*判断武将上阵层是否在场景内*/
    GoBattleLayer* pGoBattleLayer = (GoBattleLayer*)this->getChildByTag(GOBAT_LAYER_TAG);
    if (pGoBattleLayer->isInScreen()) {
        pGoBattleLayer->moveOutTouchUICompent();
    }
    /*移除原来层*/
    CCLayer* pOldLayer = (CCLayer*)this->getChildByTag(m_currLayerTag);
    CCHeroCommonLayer* pHeroCommonLayer = dynamic_cast<CCHeroCommonLayer*>(pOldLayer);
    pHeroCommonLayer->moveOutTouchUICompent();
    /*显示当前层*/
    m_currLayerTag = CHOS_FORM_TAG;
    
    ChosFormLayer* pCurrLayer = ChosFormLayer::create();
    this->addChild(pCurrLayer, 3, CHOS_FORM_TAG);
    pCurrLayer->openningAni();
    pCurrLayer->restoreTouchUICompent();
}

/* 从pvp层选择武将阵型层  */
void MainScene::onPvpChosFormSelect(CCObject* obj) {
    m_pGameState->setIsMainCityCall(false);
    if (CHOS_FORM_TAG == m_currLayerTag) {
        return;
    }
    
    /*判断武将上阵层是否在场景内*/
    GoBattleLayer* pGoBattleLayer = (GoBattleLayer*)this->getChildByTag(GOBAT_LAYER_TAG);
    if (pGoBattleLayer->isInScreen()) {
        pGoBattleLayer->moveOutTouchUICompent();
    }
    /*移除原来层*/
    CCLayer* pOldLayer = (CCLayer*)this->getChildByTag(m_currLayerTag);
    CCHeroCommonLayer* pHeroCommonLayer = dynamic_cast<CCHeroCommonLayer*>(pOldLayer);
    pHeroCommonLayer->moveOutTouchUICompent();
    
    Battle::getInstance()->setBattlePkMode(BPM_PVP);
    
    /*显示当前层*/
    m_currLayerTag = CHOS_FORM_TAG;
    ChosFormLayer* pCurrLayer = ChosFormLayer::create();
    this->addChild(pCurrLayer, 3, CHOS_FORM_TAG);
    pCurrLayer->openningAni();
    pCurrLayer->restoreTouchUICompent();
}

// pvp 国战
void MainScene::onChosFormForCWar(CCObject*) {
    m_pGameState->setIsMainCityCall(false);
    if (CHOS_FORM_TAG == m_currLayerTag) {
        return;
    }
    
    /*判断武将上阵层是否在场景内*/
    GoBattleLayer* pGoBattleLayer = (GoBattleLayer*)this->getChildByTag(GOBAT_LAYER_TAG);
    if (pGoBattleLayer->isInScreen()) {
        pGoBattleLayer->moveOutTouchUICompent();
    }
    /*移除原来层*/
    CCLayer* pOldLayer = (CCLayer*)this->getChildByTag(m_currLayerTag);
    CCHeroCommonLayer* pHeroCommonLayer = dynamic_cast<CCHeroCommonLayer*>(pOldLayer);
//    pHeroCommonLayer->moveOutTouchUICompent();
    pHeroCommonLayer->setVisible(false);
    
    Battle::getInstance()->setBattlePkMode(BPM_PVP);
    Battle::getInstance()->setCWar(true);
    
    /*显示当前层*/
    m_currLayerTag = CHOS_FORM_TAG;
    ChosFormLayer* pCurrLayer = ChosFormLayer::create();
    this->addChild(pCurrLayer, 3, CHOS_FORM_TAG);
    pCurrLayer->openningAni();
    pCurrLayer->restoreTouchUICompent();
}
void MainScene::returnFromCWarChos(CCObject*) {
    m_pGameState->setIsMainCityCall(false);
//    if (CHOS_FORM_TAG == m_currLayerTag) {
//        return;
//    }
    
    /*判断武将上阵层是否在场景内*/
    GoBattleLayer* pGoBattleLayer = (GoBattleLayer*)this->getChildByTag(GOBAT_LAYER_TAG);
    if (pGoBattleLayer->isInScreen()) {
        pGoBattleLayer->moveOutTouchUICompent();
    }
    /*移除原来层*/
    CCLayer* pOldLayer = (CCLayer*)this->getChildByTag(m_currLayerTag);
    CCHeroCommonLayer* pHeroCommonLayer = dynamic_cast<CCHeroCommonLayer*>(pOldLayer);
    pHeroCommonLayer->moveOutTouchUICompent();
//    pHeroCommonLayer->setVisible(false);
    
    Battle::getInstance()->setBattlePkMode(BPM_PVP);
    Battle::getInstance()->setCWar(true);
    
    /*显示当前层*/
    m_currLayerTag = GUOZHAN_CITY_LAYER;
    GuoZCityLayer* pLayer = (GuoZCityLayer*)this->getChildByTag(GUOZHAN_CITY_LAYER);
    pLayer->setVisible(true);
}


/*
 *  响应玩家选中 “武将列表” 菜单
 */
void MainScene::onHerosSelect(CCObject* obj) {
    
    /*重置主场景*/
    onResetAllLayers();
    m_pGameState->setIsMainCityCall(false);
    m_pBattle->setBattlePkMode(BPM_PVE);
    // 原来调用武将列表者
    WhoCallHeroList oldWch = m_pGameState->getTagWhoCallHeroSel();
    // 当前调用武将列表者
    WhoCallHeroList currWch = m_pGameState->getCurrTagWhoCallHeroSel();
    
    //    CCLog("oldWch = %d, currWch = %d", oldWch, currWch);
    
    if (oldWch == currWch) {
        if (WCH_SALE != currWch) {
            return;
        }
    }
    
    /*判断武将上阵层是否在场景内*/
    GoBattleLayer* pGoBattleLayer = (GoBattleLayer*)this->getChildByTag(GOBAT_LAYER_TAG);
    if (pGoBattleLayer->isInScreen()) {
        pGoBattleLayer->moveOutTouchUICompent();
    }
    
    /*移除原来层*/
    CCLayer* pOldLayer = (CCLayer*)this->getChildByTag(m_currLayerTag);
    if (m_currLayerTag == LEVUP_LAYER_TAG || m_currLayerTag == YIZHAN_LAYER_TAG
        || m_currLayerTag == ZHULU_LAYER_TAG || m_currLayerTag == JIANYU_LAYER) {
        
        CCHeroCommonLayer* pHeroCommonLayer = dynamic_cast<CCHeroCommonLayer*>(pOldLayer);
        pHeroCommonLayer->setRemoveSelf(WCH_BROWSE == currWch);
        pHeroCommonLayer->moveOutTouchUICompent();
    } else {
        CCHeroCommonLayer* pHeroCommonLayer = dynamic_cast<CCHeroCommonLayer*>(pOldLayer);
        pHeroCommonLayer->moveOutTouchUICompent();
    }
    
    /*显示当前层*/
    m_currLayerTag = HEROS_LAYER_TAG;
    HerosLayer* pCurrLayer = (HerosLayer*)this->getChildByTag(m_currLayerTag);
    
    switch (currWch) {
        case WCH_BROWSE:         // 武将浏览
        {
            pCurrLayer->restoreTouchUICompent();
        }
            break;
        case WCH_SALE:           // 武将出售
        {
            if (WCH_BROWSE == oldWch)/*武将列表调用出售*/ {
                pCurrLayer->restoreTouchUICompent();
            } else if (WCH_SALE == oldWch)/*出售返回*/ {
                if (m_pGameState->getHeroForSaleSuccess()) {
                    pCurrLayer->restoreTouchUICompent();
                } else {
                    pCurrLayer->restoreTouchUICompent();
                }
            }
        }
            break;
        case WCH_LEVUP:          // 武将升级
        {
            pCurrLayer->restoreTouchUICompent();
        }
            break;
        case WCH_EVOUP:          // 武将升阶
        {
            pCurrLayer->restoreTouchUICompent();
        }
            break;
        case WCH_FORFOOD:        // 武将被吞噬
        {
            pCurrLayer->restoreTouchUICompent();
        }
            break;
        case WCH_CAPTAIN:        // 选(上阵)队长
        {
            pCurrLayer->restoreTouchUICompent();
        }
            break;
        case WCH_TEAMER:          // 选(上阵)队员
        {
            pCurrLayer->restoreTouchUICompent();
        }
            break;
        default:
            break;
    }
    m_pGameState->setTagWhoCallHeroSel(currWch);/*原来的列表*/

}

/*
 *  响应玩家选中 “武将列表” 菜单
 */
void MainScene::onPvpHerosSelect(CCObject* obj) {
    m_pGameState->setIsMainCityCall(false);
    m_pBattle->setBattlePkMode(BPM_PVP);
    // 原来调用武将列表者
    WhoCallHeroList oldWch = m_pGameState->getTagWhoCallHeroSel();
    // 当前调用武将列表者
    WhoCallHeroList currWch = m_pGameState->getCurrTagWhoCallHeroSel();

    //    CCLog("oldWch = %d, currWch = %d", oldWch, currWch);
    
    if (oldWch == currWch) {
        if (WCH_SALE != currWch) {
            return;
        }
    }
    
    /*判断武将上阵层是否在场景内*/
    GoBattleLayer* pGoBattleLayer = (GoBattleLayer*)this->getChildByTag(GOBAT_LAYER_TAG);
    if (pGoBattleLayer->isInScreen()) {
        pGoBattleLayer->moveOutTouchUICompent();
    }
    
    /*移除原来层*/
    CCLayer* pOldLayer = (CCLayer*)this->getChildByTag(m_currLayerTag);
    if (m_currLayerTag == LEVUP_LAYER_TAG || m_currLayerTag == YIZHAN_LAYER_TAG
        || m_currLayerTag == ZHULU_LAYER_TAG || m_currLayerTag == JIANYU_LAYER) {
        
        CCHeroCommonLayer* pHeroCommonLayer = dynamic_cast<CCHeroCommonLayer*>(pOldLayer);
        pHeroCommonLayer->setRemoveSelf(WCH_BROWSE == currWch);
        pHeroCommonLayer->moveOutTouchUICompent();
    } else {
        CCHeroCommonLayer* pHeroCommonLayer = dynamic_cast<CCHeroCommonLayer*>(pOldLayer);
        pHeroCommonLayer->moveOutTouchUICompent();
    }
    
    /*显示当前层*/
    m_currLayerTag = HEROS_LAYER_TAG;
    HerosLayer* pCurrLayer = (HerosLayer*)this->getChildByTag(m_currLayerTag);
    
    switch (currWch) {
        case WCH_CAPTAIN_PVP:        // 选(上阵)队长 pvp
        {
            pCurrLayer->restoreTouchUICompent();
        }
            break;
        case WCH_TEAMER_PVP:          // 选(上阵)队员 pvp
        {
            pCurrLayer->restoreTouchUICompent();
        }
            break;
        default:
            break;
    }
    m_pGameState->setTagWhoCallHeroSel(currWch);/*原来的列表*/

}

/*
 * 确认武将选择
 */
void MainScene::onChosHeroConfirmOK(CCObject* obj) {
    do {
        CC_BREAK_IF(HEROS_LAYER_TAG != m_currLayerTag);
        
        HerosLayer* pHeroLayer = (HerosLayer*)this->getChildByTag(HEROS_LAYER_TAG);
        pHeroLayer->setVisible(false);      // 隐藏武将选择
        pHeroLayer->moveOutTouchUICompent();
        
        // 获取在武将选择框中选择了几个武将
        char szHeroUniid[MAX_ALLOW_SELECT_HEROS][HERO_UNIID_BUFFER_SIZE];
        memset(szHeroUniid, '\0', sizeof(szHeroUniid));
        const unsigned int countOfSelHeros = pHeroLayer->getSelectedHeros(szHeroUniid);
        
        /*
         * 获取是谁调用了武将选择
         */
        WhoCallHeroList tagWhoCallHeroSel = m_pGameState->getTagWhoCallHeroSel();
        if (WCH_CAPTAIN == tagWhoCallHeroSel || WCH_TEAMER == tagWhoCallHeroSel) /*上阵武将PVE*/ {
            /*
             * 恢复主城显示
             */
            CCLayer* pMainCityLayer = (CCLayer*) this->getChildByTag(MAINCITY_LAYER_TAG);
            pMainCityLayer->setPosition(ccp(0, 0));
            pMainCityLayer->setVisible(true);
            m_currLayerTag = MAINCITY_LAYER_TAG;
            
            //            CC_BREAK_IF(countOfSelHeros == 0);  /*没有选择任何武将*/
            
            char szPostBuffer[MAX_POSTTO_SERVER_BUFFER_SIZE];
            memset(szPostBuffer, '\0', MAX_POSTTO_SERVER_BUFFER_SIZE);
            
            char szHttpTag[16];
            memset(szHttpTag, '\0', sizeof(szHttpTag));
            
            // http tag
            strcpy(szHttpTag, HTTP_REQUEST_SETHEROTEAM_TAG);
            
            // 构造 post 字符串
            if (pHeroLayer->isNoConfirm()) {
                strcpy(szPostBuffer, "c=hero&a=setLeader&leader=");
                strcat(szPostBuffer, szHeroUniid[0]);
            } else {
                strcpy(szPostBuffer, "c=hero&a=setTeam&team=");
                for (int i = 0; i < countOfSelHeros; i++) {
                    strcat(szPostBuffer, szHeroUniid[i]);
                    if (i < 3)
                        strcat(szPostBuffer, "|");
                }/*for*/
                
                if (countOfSelHeros < 3) /*不到三个武将，需补足 |*/{
                    for (int i = countOfSelHeros; i < 3; i++) {
                        strcat(szPostBuffer, "|");
                    } /*for*/
                }
            }
            
            strcat(szPostBuffer, "&uid=");
            strcat(szPostBuffer, m_pGamePlayer->getUid().c_str());
//            CCLog("post buffer = %s", szPostBuffer);
            
            // 提交 post
            m_NetConnection->commitPostRequestData(szPostBuffer, szHttpTag);
            m_pNotificationCenter->postNotification(ON_MESSAGE_LOADING_MAINSCENE);
            m_pNotificationCenter->postNotification(ON_MESSAGE_CHOSCONFIRM_HIDE_STRING);
        } else if (WCH_CAPTAIN_PVP == tagWhoCallHeroSel || WCH_TEAMER_PVP == tagWhoCallHeroSel) /*上阵武将PVP*/ {
            /*
             * 恢复铜雀台显示
             */
            if (m_pBattle->isCWar()) {
                m_pNotificationCenter->postNotification(ON_MESSAGE_GUOZHAN_STRING);
                m_currLayerTag = GUOZHAN_LAYER;
            } else {
                m_pNotificationCenter->postNotification(ON_MESSAGE_TONGQUETAI_STRING);
                m_currLayerTag = TONGQUE_LAYER;
            }
            
            char szPostBuffer[MAX_POSTTO_SERVER_BUFFER_SIZE];
            memset(szPostBuffer, '\0', MAX_POSTTO_SERVER_BUFFER_SIZE);
            
            char szHttpTag[16];
            memset(szHttpTag, '\0', sizeof(szHttpTag));
            
            // http tag
            strcpy(szHttpTag, HTTP_REQUEST_SETHEROTEAM_PVP_TAG);
            
            // 构造 post 字符串
            if (pHeroLayer->isNoConfirm()) {
                strcpy(szPostBuffer, "c=hero&a=setPVPLeader&leader=");
                strcat(szPostBuffer, szHeroUniid[0]);
            } else {
                strcpy(szPostBuffer, "c=hero&a=setPVPTeam&team=");
                for (int i = 0; i < countOfSelHeros; i++) {
                    strcat(szPostBuffer, szHeroUniid[i]);
                    if (i < 3)
                        strcat(szPostBuffer, "|");
                }/*for*/
                
                if (countOfSelHeros < 3) /*不到三个武将，需补足 |*/{
                    for (int i = countOfSelHeros; i < 3; i++) {
                        strcat(szPostBuffer, "|");
                    } /*for*/
                }
            }
            strcat(szPostBuffer, "&uid=");
            strcat(szPostBuffer, m_pGamePlayer->getUid().c_str());
//            CCLog("post buffer = %s", szPostBuffer);
            
            // 提交 post
            m_NetConnection->commitPostRequestData(szPostBuffer, szHttpTag);
            m_pNotificationCenter->postNotification(ON_MESSAGE_LOADING_MAINSCENE);
            m_pNotificationCenter->postNotification(ON_MESSAGE_CHOSCONFIRM_HIDE_STRING);
        } else if (WCH_LEVUP == tagWhoCallHeroSel || WCH_EVOUP == tagWhoCallHeroSel || WCH_FORFOOD == tagWhoCallHeroSel) /*武将升级*/ {
            // 判断武将上阵层是否在场景内
            GoBattleLayer* pGoBattleLayer = (GoBattleLayer*)this->getChildByTag(GOBAT_LAYER_TAG);
            if (pGoBattleLayer->isInScreen()) {
                pGoBattleLayer->moveOutTouchUICompent();
            }
            
            // 隐藏原来的层
            CCHeroCommonLayer* pHeroCommonLayer = dynamic_cast<CCHeroCommonLayer*>(this->getChildByTag(m_currLayerTag));
            CCAssert(pHeroCommonLayer, "invalid layer");
            pHeroCommonLayer->moveOutTouchUICompent();
            
            // 显示当前层
            m_currLayerTag = LEVUP_LAYER_TAG;
            LevUpLayer* pCurrLayer = (LevUpLayer*)this->getChildByTag(m_currLayerTag);
            CCAssert(pCurrLayer, "invalid leveupLayer");
            
            switch (tagWhoCallHeroSel) {
                case WCH_LEVUP:     // 选择升级武将
                    if (countOfSelHeros > 0) /*选择了武将*/
                        pCurrLayer->setHeroForLevUp(szHeroUniid[0]);
                    break;
                case WCH_EVOUP:     // 选择升阶武将
                    if (countOfSelHeros > 0) /*选择了武将*/
                        pCurrLayer->setHeroForEvoUp(szHeroUniid[0]);
                    break;
                case WCH_FORFOOD:      // 选择用于升级的被吞噬武将
                    pCurrLayer->setFoodHerosForLevup(countOfSelHeros, szHeroUniid);
                    break;
                default:
                    break;
            }
            pCurrLayer->setVisible(true);
            pCurrLayer->restoreTouchUICompent();
            m_pNotificationCenter->postNotification(ON_MESSAGE_CHOSCONFIRM_HIDE_STRING);
        } else if (m_pGameState->getCurrTagWhoCallHeroSel() == WCH_SALE)/*武将出售确认选择*/ {
            m_currLayerTag = SELL_LAYER_TAG;
            
            SellHerosLayer* pCurrLayer = SellHerosLayer::create();
            pCurrLayer->setVisible(false);
            this->addChild(pCurrLayer, 3, SELL_LAYER_TAG);
            pCurrLayer->openningAni();
            pCurrLayer->restoreTouchUICompent();
            pCurrLayer->setSellHeros(countOfSelHeros, szHeroUniid);
            pCurrLayer->restoreTouchUICompent();
            
            m_pNotificationCenter->postNotification(ON_MESSAGE_CHOSCONFIRM_HIDE_STRING);
        }
    } while (0);
}

/*
 * 显示确认选择框
 */
void MainScene::onShowChosConfirm(CCObject* obj) {
    ChosConfirm* pChosConfirmLayer = (ChosConfirm*)this->getChildByTag(CHOSCONFIRM_LAYER_TAG);
    CCAssert(pChosConfirmLayer, "invalid data");
    
    MenuOfMainSceneLayer* pMenuOfMainSceneLayer = (MenuOfMainSceneLayer*)this->getChildByTag(MENUBAR_LAYER_TAG);
    CCAssert(pMenuOfMainSceneLayer, "invalid data");
    
    pChosConfirmLayer->restoreTouchUICompent();
    pMenuOfMainSceneLayer->moveOutTouchUICompent();
}

// 隐藏确认选择框
void MainScene::onHideChosConfirm(CCObject* obj) {
    ChosConfirm* pChosConfirmLayer = (ChosConfirm*)this->getChildByTag(CHOSCONFIRM_LAYER_TAG);
    CCAssert(pChosConfirmLayer, "invalid data");
    
    MenuOfMainSceneLayer* pMenuOfMainSceneLayer = (MenuOfMainSceneLayer*)this->getChildByTag(MENUBAR_LAYER_TAG);
    CCAssert(pMenuOfMainSceneLayer, "invalid data");
    
    pChosConfirmLayer->moveOutTouchUICompent();
    pMenuOfMainSceneLayer->restoreTouchUICompent();
}

/******************************************************************************************************************************/


//
///*
// *   响应玩家选中 “好友” 菜单
// */
void MainScene::onFriendsSelect(CCObject* obj) {
    m_pGameState->setIsMainCityCall(false);
    //    if (FRIEND_LAYER_TAG == m_currLayerTag) {
    //        return;
    //    }
    //
    //    // 判断武将上阵层是否在场景内
    //    GoBattleLayer* pGoBattleLayer = (GoBattleLayer*)this->getChildByTag(GOBAT_LAYER_TAG);
    //    if (pGoBattleLayer->m_isInScreen) {
    //        pGoBattleLayer->moveOutTouchUICompent();
    //        pGoBattleLayer->m_isInScreen = false;
    //    }
    //    // 隐藏原来的层
    //    CCLayer* pOldLayer = (CCLayer*)this->getChildByTag(m_currLayerTag);
    ////    pOldLayer->setPosition(ccp(0, DISTANCE));
    //    if (m_currLayerTag == MAINCITY_LAYER_TAG){
    //        moveOut(pOldLayer);
    //    } else {
    //        pOldLayer->setVisible(false);
    //    }
    //
    //    // 显示当前层
    //    m_currLayerTag = FRIEND_LAYER_TAG;
    //    FriendLayer* pCurrLayer = (FriendLayer*)this->getChildByTag(m_currLayerTag);
    //    pCurrLayer->setVisible(true);
    //    pCurrLayer->openningAni();
    const char* title = "功能未开放";
    MessageBox::getInstance()->messageTips(title);
    
    //    CardBox* card = CardBox::create();
    //    HeroItem* pItem = HeroManager::getInstance()->getHeroItem(1001);
    //    card->setAnchorPoint(ccp(0.5, 0.5));
    //    card->setPosition(320, 500);
    //    card->setCardForPrison(pItem);
    //    addChild(card, 100000);
    
    
}

void MainScene::testForCallback() {
    
    CCLog("我是测试回调函数");
}
//
///*
// *  响应玩家选中 “选项” 菜单
// */
void MainScene::onOptionSelect(CCObject* obj) {
    m_pGameState->setIsMainCityCall(false);
    //    if (OPTION_LAYER_TAG == m_currLayerTag) {
    //        return;
    //    }
    //
    //    // 判断武将上阵层是否在场景内
    //    GoBattleLayer* pGoBattleLayer = (GoBattleLayer*)this->getChildByTag(GOBAT_LAYER_TAG);
    //    if (pGoBattleLayer->m_isInScreen) {
    //        pGoBattleLayer->moveOutTouchUICompent();
    //        pGoBattleLayer->m_isInScreen = false;
    //    }
    //    // 隐藏原来的层
    //    CCLayer* pOldLayer = (CCLayer*)this->getChildByTag(m_currLayerTag);
    ////    pOldLayer->setPosition(ccp(0, DISTANCE));
    //    if (m_currLayerTag == MAINCITY_LAYER_TAG){
    //        moveOut(pOldLayer);
    //    } else {
    //        pOldLayer->setVisible(false);
    //    }
    //
    //    // 显示当前层
    //    m_currLayerTag = OPTION_LAYER_TAG;
    //    OptionLayer* pCurrLayer = (OptionLayer*)this->getChildByTag(m_currLayerTag);
    //    pCurrLayer->setVisible(true);
    //    pCurrLayer->openningAni();
    
    const char* title = "功能未开放";
    MessageBox::getInstance()->messageTips(title);
}

//*
// *    响应玩家选中 “商城” 菜单
// *    测试副本按钮
// */
void MainScene::onShopSelect(CCObject* obj) {
// kknd
    
}

/******************************************************************************************************************/
/*
 *  网络异常
 */
void MainScene::network_exception(CCObject*) {
    LoadingBox* pLoadingBox = (LoadingBox*)this->getChildByTag(MASK_LAYER_TAG);
    if (pLoadingBox) {
        pLoadingBox->network_exception_schedule();
    }
}

/*
 * 功能：
 *      处理服务器回应数据
 * 功能：
 *      szData  服务器回应数据缓冲区
 *
 */
void MainScene::handleWSResponseData(const char *szData, const unsigned int dataSize, char* szTag) {
    bool isSuccess = ProcessData::parseBufferFromSvr(szData, dataSize, szTag);
    
    if (!strcmp(szTag, HTTP_REQUEST_SETHEROTEAM_TAG)) /*更新上阵武将 pve*/ {
        GoBattleLayer* pGoBattleLayer = (GoBattleLayer*)this->getChildByTag(GOBAT_LAYER_TAG);
        pGoBattleLayer->updateHeroUi();
        pGoBattleLayer->restoreTouchUICompent();
        
    } else if (!strcmp(szTag, HTTP_REQUEST_SETHEROTEAM_PVP_TAG)) /*更新上阵武将 pvp*/ {
        GoBattleLayer* pGoBattleLayer = (GoBattleLayer*)this->getChildByTag(GOBAT_LAYER_TAG);
        pGoBattleLayer->updateHeroUi();
        pGoBattleLayer->restoreTouchUICompent();
    } else if (!strcmp(szTag, HTTP_REQUEST_LEVUPHERO_TAG)) /*升级武将*/ {
        // 删除用于升级被吞噬的武将
        LevUpLayer* pLevUpLayer = (LevUpLayer*)this->getChildByTag(LEVUP_LAYER_TAG);
        
        if (isSuccess) {
            char szUniId[MAX_FOOD_HEROS_FOR_LEVUP][HERO_UNIID_BUFFER_SIZE];
            memset(szUniId, '\0', sizeof(szUniId));
            
            unsigned int countOfFoodHerosForLevup = pLevUpLayer->getFoodHerosForLevup(szUniId);
            if (countOfFoodHerosForLevup > 0)
                m_pHeroManager->removeHeroOfPlayer(countOfFoodHerosForLevup, szUniId);
            pLevUpLayer->clearHeroData();
        }
        //
        pLevUpLayer->update(0);
        pLevUpLayer->actionForLevUp();
        // 升级后剩余铜币
        m_pGamePlayer->setMoney(m_pGamePlayer->getMoney() - m_pGameState->getTunNeedMoney());
    } else if (!strcmp(szTag, HTTP_REQUEST_EVOUPHERO_TAG)) /* 升阶武将 */ {
        // 删除用于升被吞噬的武将
        LevUpLayer* pLevUpLayer = (LevUpLayer*)this->getChildByTag(LEVUP_LAYER_TAG);
        
        if (isSuccess) {
            char szUniId[HERO_UNIID_BUFFER_SIZE];
            memset(szUniId, '\0', sizeof(szUniId));
            if (pLevUpLayer->getHeroForEvoUp(szUniId)) {
                m_pHeroManager->removeHeroOfPlayer(szUniId);
                pLevUpLayer->clearHeroData();
            }
            
            // 设置升阶后武将
            char szHeroEvoUpUniId[HERO_UNIID_BUFFER_SIZE];
            m_pGameState->getHeroEvoUp(szHeroEvoUpUniId);
            pLevUpLayer->setHeroEvoUp(szHeroEvoUpUniId);
        }
        pLevUpLayer->update(1);
        pLevUpLayer->actionForLevUp();
        // 进化后剩余铜币
        m_pGamePlayer->setMoney(m_pGamePlayer->getMoney() - m_pGameState->getEvoNeedMoney());
    } else if (!strcmp(szTag, HTTP_REQUEST_FIGHT_PVE_TAG)) {
        if (isSuccess) {
            /* 出征成功，消耗体力 */
            int enrgy = 0;
            LevelDataItem* pLevelDataItem = CopyManager::getInstance()->getLevelDataById(m_pGameState->getCopyId(),
                                                                                         m_pGameState->getLevId());
            enrgy = m_pGamePlayer->getEnergy() - pLevelDataItem->getEn();
            m_pGamePlayer->setEnergy(enrgy);
            
            CCDirector::sharedDirector()->replaceScene(BattleScene::create());
        }
    } else if (!strcmp(szTag, HTTP_REQUEST_FIGHT_TQT_TAG)) {
        if (isSuccess) {
            m_pBattle->setCWar(false);
            CCDirector::sharedDirector()->replaceScene(BattleScene::create());
        }
    } else if (!strcmp(szTag, HTTP_TONGQUETAI_REPLAY) /*铜雀台战斗回放*/) {
        if (isSuccess) {
            m_pBattle->setCWar(false);
//            CCDirector::sharedDirector()->replaceScene(BattleScene::create());
        }
    } else if(!strcmp(szTag, HTTP_REQUEST_YIFREE_TAG)) {
        if (isSuccess) {
            YiZhanShowLayer* pLayer = (YiZhanShowLayer*)this->getChildByTag(YIZHAN_SHOW_TAG);
            YiZhanLayer* pYiLayer = (YiZhanLayer*)this->getChildByTag(YIZHAN_LAYER_TAG);
            pLayer->setHeroDataFromSvr(szData);
            m_pGameState->updateSearch(1);
            pYiLayer->updateUi(1);/*消耗玩家勇气值(必须先执行)*/
            pLayer->update(1);
        }
    } else if(!strcmp(szTag, HTTP_REQUEST_YINFREE_TAG)) {
        if (isSuccess) {
            YiZhanShowLayer* pLayer = (YiZhanShowLayer*)this->getChildByTag(YIZHAN_SHOW_TAG);
            YiZhanLayer* pYiLayer = (YiZhanLayer*)this->getChildByTag(YIZHAN_LAYER_TAG);
            // CCLog("mainScene content = %s", szData);
            pLayer->setHeroDataFromSvr(szData);
            m_pGameState->updateSearch(2);
            pYiLayer->updateUi(2);/*消耗玩家人民币*/
            pLayer->update(2);
        }
    } else if (!strcmp(szTag, HTTP_REQUEST_YINFREETEN_TAG)) {
        if (isSuccess) {
            YiZhanShowLayer* pLayer = (YiZhanShowLayer*)this->getChildByTag(YIZHAN_SHOW_TAG);
            YiZhanLayer* pYiLayer = (YiZhanLayer*)this->getChildByTag(YIZHAN_LAYER_TAG);
            // CCLog("mainScene content = %s", szData);
            pLayer->setHeroDataFromSvr(szData);
            m_pGameState->updateSearch(3);
            pYiLayer->updateUi(3);/*消耗玩家人民币*/
            pLayer->update(3);
        }
    } else if (!strcmp(szTag, HTTP_REQUEST_SELLHERO_TAG)) {
        if (isSuccess) {
            SellHerosLayer* pLayer = (SellHerosLayer*)this->getChildByTag(SELL_LAYER_TAG);
            char szUniId[MAX_ALLOW_SELECT_HEROS][HERO_UNIID_BUFFER_SIZE];
            unsigned int countOfSell = pLayer->getSellHeros(szUniId);
            m_pHeroManager->removeHeroOfPlayer(countOfSell, szUniId);
            m_pGameState->setHeroForSaleSuccess(true);
            m_pGameState->setCurrTagWhoCallHeroSel(WCH_SALE);
            m_pNotificationCenter->postNotification(ON_MESSAGE_HEROS_STRING);
        }
    } else if (!strcmp(szTag, HTTP_REQUEST_BUYENERGY_TAG))/*购买体力*/ {
        if (isSuccess) {
            BgOfMainSceneLayer* pBgLayer = (BgOfMainSceneLayer*)this->getChildByTag(BG_LAYER_TAG);
            /* 购买刷新元宝 */
            pBgLayer->updateGold();
            m_pGamePlayer->setEnergyPrice(m_pGameState->getTempPrice());
            m_pGameState->setTempPrice(0);
            /* 恢复当前能接收的最大体力 */
            int maxEnergy = m_pGamePlayer->getMaxEnergy();
            m_pGamePlayer->setEnergy(maxEnergy);
            pBgLayer->updateEnergy();
        }
    } else if (!strcmp(szTag, HTTP_REQUEST_HEROLOCK_TAG))/* 锁武将 */ {
        if (isSuccess) {
            HeroIntroduce* pHeroIntroduce = (HeroIntroduce*)this->getChildByTag(HERO_INTRO_LAYER);
            pHeroIntroduce->setLockSuccess(true);
        }
    } else if (!strcmp(szTag, HTTP_REQUEST_JIANYUINIT_TAG))/* 初始化监狱武将*/ {
        if (isSuccess) {
            m_pNotificationCenter->postNotification(ON_MESSAGE_JIANYU_STRING);
        }
    } else if (!strcmp(szTag, HTTP_ZHAOXIANG_HERO_TAG))/* 招降监狱武将 */ {
        if (isSuccess) {
            JianyuLayer* pLayer = (JianyuLayer*)this->getChildByTag(JIANYU_LAYER);
            if (m_pGameState->getIsSummonSucess())/*招降成功*/ {
                HeroItem* pItem = m_pHeroManager->getHeroItem(m_pGameState->getSucessSummonId());
                MessageBox::getInstance()->messageForTqt(pItem);
            } else {
                char buffer[128];
                sprintf(buffer, "武将降低了%d", m_pGameState->getSubLoyalty());
                MessageBox::getInstance()->messageTips(buffer, 1, -100.0f);
            }
            pLayer->updateAllOfPrisonBySummon();
        }
    } else if (!strcmp(szTag, HTTP_FREE_HERO_TAG))/* 释放监狱武将 */ {
        if (isSuccess) {
            m_pHeroManager->removePrisonHeroItemByIndex(m_pGameState->getFreeIdx());
            /* 刷新界面 */
            JianyuLayer* pLayer = (JianyuLayer*)this->getChildByTag(JIANYU_LAYER);
            pLayer->updateAllOfPrisonByFree();
        }
    } else if (!strcmp(szTag, HTTP_ENTER_TONGQUETAI))/* 进入铜雀台 */ {
        if (isSuccess) {
            /* 重置武将上阵 */
            m_pNotificationCenter->postNotification(ON_MESSAGE_TONGQUETAI_STRING);
        }
    } else if (!strcmp(szTag, HTTP_TONGQUETAI_AWARD))/* 铜雀台奖励 */ {
        if (isSuccess) {
            TongQueLayer* pLayer = (TongQueLayer*)this->getChildByTag(TONGQUE_LAYER);
            pLayer->updateAllTQ();
        }
    } else if (!strcmp(szTag, HTTP_REQUEST_CHOSCAMP_TAG))/* 选择阵营 */ {//qjqj
        BgOfMainSceneLayer* pBgLayer = (BgOfMainSceneLayer*)this->getChildByTag(BG_LAYER_TAG);
        pBgLayer->confirmCamp();
        
        NetConnection* pNetConnection = NetConnection::getInstance();
        char szPostBuffer[MAX_POSTTO_SERVER_BUFFER_SIZE];
        memset(szPostBuffer, '\0', MAX_POSTTO_SERVER_BUFFER_SIZE);
        strcpy(szPostBuffer, "c=cwar&a=enter");
        char szHttpTag[] = HTTP_REQUEST_ENTERCWAR_TAG;
        strcat(szPostBuffer, "&uid=");
        strcat(szPostBuffer, GamePlayer::getInstance()->getUid().c_str());
        CCLog("post 进入国战 = %s", szPostBuffer);
        pNetConnection->commitPostRequestData(szPostBuffer, szHttpTag);
    } else if (!strcmp(szTag, HTTP_REQUEST_ENTERCWAR_TAG))/* 进入国战 */ {
        if (isSuccess) {
            if (m_pGameState->getIsNewUser()) {
                m_pNotificationCenter->postNotification(ON_MESSAGE_GZCHOSCAMP_STRING);
            } else {
                onChosGuoZhan(this);
            }
        }
    } else if (!strcmp(szTag, HTTP_REQUEST_ENTERCITY_TAG))/* 进入城池 */ {
        if (isSuccess) {
            onChosGuoZhanCity(this);
        }
    } else if (!strcmp(szTag, HTTP_REQUEST_BUILD_TAG))/* 国战建造 */ {
        if (isSuccess) {
            GuoZCityLayer* pLayer = (GuoZCityLayer*)this->getChildByTag(GUOZHAN_CITY_LAYER);
            pLayer->updateForBuild();
        }
    } else if (!strcmp(szTag, HTTP_REQUEST_GUWU_TAG))/* 国战建造 */ {
        if (isSuccess) {
            GuoZCityLayer* pLayer = (GuoZCityLayer*)this->getChildByTag(GUOZHAN_CITY_LAYER);
            pLayer->updateForInspire();
        }
    } else if (!strcmp(szTag, HTTP_REQUEST_FIGHT_CWAR_TAG))/* 国战出征 */ {
        if (isSuccess) {
            if (m_pBattle->isInList()) {
                const char* title = "您已经进入防守列表";
                MessageBox::getInstance()->messageTips(title);
                returnFromCWarChos(this);
            } else {
                CCDirector::sharedDirector()->replaceScene(BattleScene::create());
            }
        } else {
            returnFromCWarChos(this);
        }
    }
    /* 关闭loading */
    m_pNotificationCenter->postNotification(ON_MESSAGE_SHUTDOWN_MAINSCENE);
}

/* loading 调度 */
void MainScene::loading_schedule(CCTime ccTime) {
    if (m_NetConnection->isPending()) {
        addChild(LoadingBox::create(), 1000000, MASK_LAYER_TAG);
    }
    this->unschedule(schedule_selector(MainScene::loading_schedule));
}

/*loadign*/
void MainScene::showLoading(CCObject*) {
    if (m_NetConnection->isPending()) {
        this->schedule(schedule_selector(MainScene::loading_schedule), 1.0f, 1, NETWORK_WAITBOX_WAITTIME);
    }
}

void MainScene::closeLoading(CCObject*) {
    this->unschedule(schedule_selector(MainScene::loading_schedule));
    
    if (this->getChildByTag(MASK_LAYER_TAG)) {
        this->removeChildByTag(MASK_LAYER_TAG);
    }
}
/*铁匠铺*/
void MainScene::onChosTieJiang(CCObject*) {
    if (TIEJIANG_LAYER == m_currLayerTag) {
        return;
    }
    /*判断武将上阵层是否在场景内*/
    GoBattleLayer* pGoBattleLayer = (GoBattleLayer*)this->getChildByTag(GOBAT_LAYER_TAG);
    if (pGoBattleLayer->isInScreen()) {
        pGoBattleLayer->moveOutTouchUICompent();
    }
    /*移除原来层*/
    CCLayer* pOldLayer = (CCLayer*)this->getChildByTag(m_currLayerTag);
    CCHeroCommonLayer* pHeroCommonLayer = dynamic_cast<CCHeroCommonLayer*>(pOldLayer);
    pHeroCommonLayer->moveOutTouchUICompent();
    /*显示当前层*/
    m_currLayerTag = TIEJIANG_LAYER;
    TieJiangLayer* pLayer = (TieJiangLayer*)this->getChildByTag(TIEJIANG_LAYER);
    if (!pLayer) {
        pLayer = TieJiangLayer::create();
        pLayer->initPosition();
        this->addChild(pLayer, 3, TIEJIANG_LAYER);
    }
    pLayer->restoreTouchUICompent();
}

void MainScene::onChosGuoZhan(CCObject*) {
    if (GUOZHAN_LAYER == m_currLayerTag) {
        return;
    }
    /*特殊处理上阵武将*/
    GoBattleLayer* pGoBattleLayer = (GoBattleLayer*)this->getChildByTag(GOBAT_LAYER_TAG);
    if (pGoBattleLayer->isInScreen()) {
        pGoBattleLayer->actionForPvp();
    } else {
        pGoBattleLayer->restoreTouchUICompent();
    }
    
    /*移除原来层*/
    CCLayer* pOldLayer = (CCLayer*)this->getChildByTag(m_currLayerTag);
    CCHeroCommonLayer* pHeroCommonLayer = dynamic_cast<CCHeroCommonLayer*>(pOldLayer);
    pHeroCommonLayer->moveOutTouchUICompent();
    /*显示当前层*/
    m_currLayerTag = GUOZHAN_LAYER;
    GuoZhanLayer* pGuoZhanLayer = (GuoZhanLayer*)this->getChildByTag(GUOZHAN_LAYER);
    if (!pGuoZhanLayer) {
        pGuoZhanLayer = GuoZhanLayer::create();
        pGuoZhanLayer->initPosition();
        this->addChild(pGuoZhanLayer, 3, GUOZHAN_LAYER);
    }
    pGuoZhanLayer->restoreTouchUICompent();
}

void MainScene::onChosTongQue(CCObject*) {
    if (TONGQUE_LAYER == m_currLayerTag) {
        return;
    }
    /*特殊处理上阵武将*/
    GoBattleLayer* pGoBattleLayer = (GoBattleLayer*)this->getChildByTag(GOBAT_LAYER_TAG);
    if (pGoBattleLayer->isInScreen()) {
        pGoBattleLayer->actionForPvp();
    } else {
        pGoBattleLayer->restoreTouchUICompent();
    }
    
    /*移除原来层*/
    CCLayer* pOldLayer = (CCLayer*)this->getChildByTag(m_currLayerTag);
    CCHeroCommonLayer* pHeroCommonLayer = dynamic_cast<CCHeroCommonLayer*>(pOldLayer);
    pHeroCommonLayer->moveOutTouchUICompent();
    /*显示当前层*/
    m_currLayerTag = TONGQUE_LAYER;
    TongQueLayer* pLayer = (TongQueLayer*)this->getChildByTag(TONGQUE_LAYER);
    if (!pLayer) {
        pLayer = TongQueLayer::create();
        pLayer->initPosition();
        this->addChild(pLayer, 3, TONGQUE_LAYER);
    }
    pLayer->restoreTouchUICompent();
    
}
/* 监狱系统 */
void MainScene::onChosJianYu(CCObject*) {
    if (JIANYU_LAYER == m_currLayerTag) {
        return;
    }
    /*判断武将上阵层是否在场景内*/
    GoBattleLayer* pGoBattleLayer = (GoBattleLayer*)this->getChildByTag(GOBAT_LAYER_TAG);
    if (pGoBattleLayer->isInScreen()) {
        pGoBattleLayer->moveOutTouchUICompent();
    }
    /*移除原来层*/
    CCLayer* pOldLayer = (CCLayer*)this->getChildByTag(m_currLayerTag);
    CCHeroCommonLayer* pHeroCommonLayer = dynamic_cast<CCHeroCommonLayer*>(pOldLayer);
    pHeroCommonLayer->moveOutTouchUICompent();
    /*显示当前层*/
    m_currLayerTag = JIANYU_LAYER;
    JianyuLayer* pJyLayer = (JianyuLayer*)this->getChildByTag(JIANYU_LAYER);
    if (!pJyLayer) {
        pJyLayer = JianyuLayer::create();
        pJyLayer->initPosition();
        this->addChild(pJyLayer, 3, JIANYU_LAYER);
    }
//     m_pGameState->setTagWhoCallHeroIntro(JY_LAYER);
    pJyLayer->restoreTouchUICompent();
}

// 响应国战城池点击
void MainScene::onChosGuoZhanCity(CCObject*) {
    m_pGameState->setIsMainCityCall(false);
    if (GUOZHAN_CITY_LAYER == m_currLayerTag) {
        return;
    }
    /*判断武将上阵层是否在场景内*/
    GoBattleLayer* pGoBattleLayer = (GoBattleLayer*)this->getChildByTag(GOBAT_LAYER_TAG);
    if (pGoBattleLayer->isInScreen()) {
        pGoBattleLayer->moveOutTouchUICompent();
    }
    /*移除原来层*/
    CCLayer* pOldLayer = (CCLayer*)this->getChildByTag(m_currLayerTag);
    CCHeroCommonLayer* pHeroCommonLayer = dynamic_cast<CCHeroCommonLayer*>(pOldLayer);
    pHeroCommonLayer->moveOutTouchUICompent();
    /*显示当前层*/
    m_currLayerTag = GUOZHAN_CITY_LAYER;
    GuoZCityLayer* pGuoZCityLayer = (GuoZCityLayer*)this->getChildByTag(GUOZHAN_CITY_LAYER);
    if (!pGuoZCityLayer) {
        pGuoZCityLayer = GuoZCityLayer::create();
        pGuoZCityLayer->initPosition();
        this->addChild(pGuoZCityLayer, 3, GUOZHAN_CITY_LAYER);
    }
    pGuoZCityLayer->restoreTouchUICompent();
}

void MainScene::onResponseForMonvOUt(CCObject*) {
    //
    GoBattleLayer* pGoBattleLayer = (GoBattleLayer*)this->getChildByTag(GOBAT_LAYER_TAG);
    if (m_pGameState->getIsMainCityCall()) {
        pGoBattleLayer->actionForPvp();
    }
}

/*响应玩家在武将简介层点击武将，弹出武将简介*/
void  MainScene::onChosHeroForIntro(CCObject* obj){
    if (HERO_INTRO_LAYER == m_currLayerTag) {
        return;
    }

    /*显示当前层*/
    m_currLayerTag = HERO_INTRO_LAYER;
    HeroIntroduce* pHeroIntroduce = (HeroIntroduce*)this->getChildByTag(HERO_INTRO_LAYER);
    if (!pHeroIntroduce) {
        pHeroIntroduce = HeroIntroduce::create();
        this->addChild(pHeroIntroduce,20,HERO_INTRO_LAYER);
    }
    m_pGameState->setTagWhoCallHeroIntro(HEROLIST_LAYER);
    pHeroIntroduce->restoreTouchUICompent();
}
/*响应玩家在监狱层点击武将，弹出武将简介层*/
void MainScene::onChosJianYuHeroForIntro(CCObject* obj){
    if (HERO_INTRO_LAYER == m_currLayerTag) {
        return;
    }
    /*显示当前层*/
    m_currLayerTag = HERO_INTRO_LAYER;
    HeroIntroduce* pHeroIntroduce = (HeroIntroduce*)this->getChildByTag(HERO_INTRO_LAYER);
    if (!pHeroIntroduce) {
        pHeroIntroduce = HeroIntroduce::create();
        this->addChild(pHeroIntroduce,20,HERO_INTRO_LAYER);
    }
    m_pGameState->setTagWhoCallHeroIntro(JY_LAYER);
    pHeroIntroduce->restoreTouchUICompent();

    
}

/*响应玩家在武将简介层点击确定按钮*/
void MainScene::onChosHeroIntroOk(CCObject* obj){
    if (HEROS_LAYER_TAG == m_currLayerTag ||JIANYU_LAYER == m_currLayerTag) {
        return;
    }
    /*移除原来层*/
    CCLayer* pOldLayer = (CCLayer*)this->getChildByTag(m_currLayerTag);
    CCHeroCommonLayer* pHeroCommonLayer = dynamic_cast<CCHeroCommonLayer*>(pOldLayer);
    pHeroCommonLayer->moveOutTouchUICompent();
   // pHeroCommonLayer->setRemoveSelf(true);
    /*显示当前层*/
    if (m_pGameState->getTagWhoCallHeroIntro() ==  HEROLIST_LAYER) {
         m_currLayerTag = HEROS_LAYER_TAG;
         m_pGameState->setTagWhoCallHeroIntro(HEROLIST_LAYER);
    }else if(m_pGameState->getTagWhoCallHeroIntro() == JY_LAYER){
        m_currLayerTag = JIANYU_LAYER;
        m_pGameState->setTagWhoCallHeroIntro(JY_LAYER);
    }
   
}



// 选择阵营
void MainScene::onChosCamp(CCObject*) {
    CwarChosCampLayer* p = CwarChosCampLayer::create();
    addChild(p, 10000);
}


/*重置主场景*/
void MainScene::onResetAllLayers() {
    if (this->getChildByTag(GUOZHAN_CITY_LAYER)) {
        this->removeChildByTag(GUOZHAN_CITY_LAYER);
    }
}


