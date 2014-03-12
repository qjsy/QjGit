//
//  LoginScene.cpp
//  hero
//
//  Created by yangjie on 2013/11/28.
//
//

#include "LoginScene.h"
#include "LoginLayer.h"
#include "Demo.h"
#include "customMessage.h"
#include "LoadingBox.h"
#include "NetConnection.h"
#include "common.h"

#define LOADING_TAG             100

/*
 *  构造方法
 */
LoginScene::LoginScene():m_pNetConnection(NetConnection::getInstance()) {}

/*
 *  析构函数
 */
LoginScene::~LoginScene() {}

/*
 *   登录场景初始化
 */
bool LoginScene::init() {
    // super init first
    if (!CCScene::init()) {
        return false;
    }

    // LoginLayer* login = LoginLayer::create();
    // addChild(login);
    Demo* demo = Demo::create();
    addChild(demo);
    
    return true;
}

void LoginScene::onEnter() {
    CCScene::onEnter();
    
    CCNotificationCenter* pCCNotificationCenter = CCNotificationCenter::sharedNotificationCenter();
    pCCNotificationCenter->addObserver(this, callfuncO_selector(LoginScene::showLoading), ON_MESSAGE_LOADING_NOW, NULL);
    pCCNotificationCenter->addObserver(this, callfuncO_selector(LoginScene::closeLoading), ON_MESSAGE_SHUTDOWN_LOADING, NULL);
    pCCNotificationCenter->addObserver(this, callfuncO_selector(LoginScene::network_exception), ON_MESSAGE_NETWORK_EXCEPTION, NULL);
}

void LoginScene::onExit() {
    CCNotificationCenter* pCCNotificationCenter = CCNotificationCenter::sharedNotificationCenter();
    pCCNotificationCenter->removeObserver(this, ON_MESSAGE_LOADING_NOW);
    pCCNotificationCenter->removeObserver(this, ON_MESSAGE_SHUTDOWN_LOADING);
    pCCNotificationCenter->removeObserver(this, ON_MESSAGE_NETWORK_EXCEPTION);

    //
    
    CCScene::onExit();
}

/* 显示网络loading */
void LoginScene::loading_schedule(CCTime cctTime) {
    if (m_pNetConnection->isPending()) {
        addChild(LoadingBox::create(), 100, LOADING_TAG);
    }
    this->unschedule(schedule_selector(LoginScene::loading_schedule));
}

void LoginScene::showLoading(CCObject* obj) {
    if (m_pNetConnection->isPending()) {
        this->schedule(schedule_selector(LoginScene::loading_schedule), 1.0f, 1, NETWORK_WAITBOX_WAITTIME);
    }
}

void LoginScene::closeLoading(CCObject*) {
    this->unschedule(schedule_selector(LoginScene::loading_schedule));
    if (this->getChildByTag(LOADING_TAG)) {
        this->removeChildByTag(LOADING_TAG);
    }
}

/*
 *  网络异常
 */
void LoginScene::network_exception(CCObject*) {
    this->unschedule(schedule_selector(LoginScene::loading_schedule));
    LoadingBox* pLoadingBox = (LoadingBox*)this->getChildByTag(LOADING_TAG);
    if (!pLoadingBox) {
        pLoadingBox = LoadingBox::create();
        addChild(pLoadingBox, 100, LOADING_TAG);
    }
    pLoadingBox->network_exception_schedule();
}
