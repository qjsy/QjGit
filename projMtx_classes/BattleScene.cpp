#include "BattleScene.h"
#include "BattleLayer.h"
#include "customMessage.h"
#include "BattleSummary.h"
#include "BgOfMainSceneLayer.h"
#include "LoadingBox.h"
#include "NetConnection.h"
#include "PvpSummary.h"

#define BATTLE_LAYER_TAG					1000
#define BATTLESUMMARY_TAG                   1001
#define MASK_LAYER_TAG                      1002

/*
 * 构造函数
 */
BattleScene::BattleScene():m_pNetConnection(NetConnection::getInstance()) {}

/*
 * 析构函数
 */
BattleScene::~BattleScene() {}

/*
 * 战斗场景初始化
 */
bool BattleScene::init() {
	// super init first
	if (!CCScene::init()) {
		return false;
	}

	// 添加战斗层
	CCLayer* pBattleLayer = BattleLayer::create();
	this->addChild(pBattleLayer, 1, BATTLE_LAYER_TAG);
    
    return true;
}


/*
 * 进入战斗场景
 */
void BattleScene::onEnter() {
    CCScene::onEnter();

    CCNotificationCenter* pCCNotificationCenter = CCNotificationCenter::sharedNotificationCenter();
    pCCNotificationCenter->addObserver(this, callfuncO_selector(BattleScene::onBattleSummary), ON_MESSAGE_SUMMARY_STRING, NULL);
    pCCNotificationCenter->addObserver(this, callfuncO_selector(BattleScene::showLoading), ON_MESSAGE_LOADING_BATTLE, NULL);
    pCCNotificationCenter->addObserver(this, callfuncO_selector(BattleScene::closeLoading), ON_MESSAGE_SHUTDOWN_BATTLE, NULL);
    pCCNotificationCenter->addObserver(this, callfuncO_selector(BattleScene::network_exception),
                                       ON_MESSAGE_NETWORK_EXCEPTION, NULL);
    pCCNotificationCenter->addObserver(this, callfuncO_selector(BattleScene::onPvpBattleSummary),
                                       ON_MESSAGE_PVP_SUMMARY_STRING, NULL);
}

/*
 * 离开战斗场景
 */
void BattleScene::onExit() {
    CCScene::onExit();
    
    CCNotificationCenter* pCCNotificationCenter = CCNotificationCenter::sharedNotificationCenter();
    pCCNotificationCenter->removeObserver(this, ON_MESSAGE_SUMMARY_STRING);
    pCCNotificationCenter->removeObserver(this, ON_MESSAGE_LOADING_BATTLE);
    pCCNotificationCenter->removeObserver(this, ON_MESSAGE_SHUTDOWN_BATTLE);
    pCCNotificationCenter->removeObserver(this, ON_MESSAGE_NETWORK_EXCEPTION);
    pCCNotificationCenter->removeObserver(this, ON_MESSAGE_PVP_SUMMARY_STRING);
}

void BattleScene::onPvpBattleSummary(CCObject* obj) {
    this->removeChildByTag(BATTLE_LAYER_TAG);
    
    //// 战斗结算层
    CCLayer* pSummary = PvpSummary::create();
    this->addChild(pSummary, 100, BATTLESUMMARY_TAG);
    pSummary->setVisible(true);
    
    BgOfMainSceneLayer* pBgOfMainSceneLayer = BgOfMainSceneLayer::create();
    this->addChild(pBgOfMainSceneLayer, 10000);
}

void BattleScene::onBattleSummary(CCObject* obj) {
    this->removeChildByTag(BATTLE_LAYER_TAG);
    
    //// 战斗结算层
    CCLayer* pSummary = BattleSummary::create();
    this->addChild(pSummary, 100, BATTLESUMMARY_TAG);
    pSummary->setVisible(true);
    
    BgOfMainSceneLayer* pBgOfMainSceneLayer = BgOfMainSceneLayer::create();
    this->addChild(pBgOfMainSceneLayer, 10000);
}

/* 显示网络loading */
void BattleScene::loading_schedule(CCTime cctTime) {
    if (m_pNetConnection->isPending()) {
        addChild(LoadingBox::create(), 1000000, MASK_LAYER_TAG);
    }
    this->unschedule(schedule_selector(BattleScene::loading_schedule));
}

/*
 *  网络异常
 */
void BattleScene::network_exception(CCObject*) {
    LoadingBox* pLoadingBox = (LoadingBox*)this->getChildByTag(MASK_LAYER_TAG);
    if (pLoadingBox) {
        pLoadingBox->network_exception_schedule();
    }
}

void BattleScene::showLoading(CCObject*) {
    if (m_pNetConnection->isPending()) {
        this->schedule(schedule_selector(BattleScene::loading_schedule), 1.0f, 1, NETWORK_WAITBOX_WAITTIME);
    }
}

void BattleScene::closeLoading(CCObject*) {
    this->unschedule(schedule_selector(BattleScene::loading_schedule));
    
    if (this->getChildByTag(MASK_LAYER_TAG)) {
        this->removeChildByTag(MASK_LAYER_TAG);
    }
}
