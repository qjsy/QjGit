//
//  LoadingBox.cpp
//  hero
//
//  Created by 钱剑 on 14-1-13.
//
//

#include "common.h"
#include "LoadingBox.h"

#define MODE_LOADING                1
#define LOADING_TAG                 100
#define ZORDER                      9999

#define MODEL_BG                    1000        // 模态框背景
#define LOADINGANI                  1001        // 登陆动画
#define TIPBOX                      1002        // 提示框
#define TIPCONFIRMBUTTON            1003        // 确认按钮
#define TIPCONFIRMBUTTONBG          1004        // 确认背景
#define TIPCONFIRMBUTTONBGS         1005        // 确认背景选中
#define TIPFORNETWORK               1006

/*
 *  构造函数
 */
LoadingBox::LoadingBox(){}

/*
 *  析构函数
 */
LoadingBox::~LoadingBox() {
    CCTextureCache::sharedTextureCache()->removeTextureForKey("CM_modal.png");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("CM_loading.png");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("CM_networkException.png");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("CM_netConfirm.png");
}

bool LoadingBox::init() {
    if (!CCLayer::init()) {
        return false;
    }
    this->setTouchEnabled(true);
    
    CCNode* pNode = CCNode::create();
    addChild(pNode, 1, LOADING_TAG);
    
    CCSprite* spBg = CCSprite::create("CM_modal.png");
    spBg->setAnchorPoint(ccp(0, 0));
    spBg->setPosition(ccp(0, 0));
    pNode->addChild(spBg, 1, MODEL_BG);
    
    /* 动画 */
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCSprite* loading = CCSprite::create("CM_loading.png");
    loading->setPosition(ccp(size.width / 2, size.height / 2));
    CCActionInterval * rotateto1 = CCRotateTo::create(1, 180);
    CCActionInterval * rotateto2 = CCRotateTo::create(1, 360);
    CCFiniteTimeAction* seq = CCSequence::create(rotateto1,rotateto2,NULL);
    CCActionInterval * repeatForever =CCRepeatForever::create((CCActionInterval* )seq);
    loading->runAction(repeatForever);
    pNode->addChild(loading, 10, LOADINGANI);
    
    
    CCSprite* spConfirm = CCSprite::create("CM_networkBad.png");
    spConfirm->setPosition(ccp(size.width / 2, size.height / 2 - 30));
    pNode->addChild(spConfirm, 2, TIPFORNETWORK);
    

    return true;
}

void LoadingBox::onExit() {
    CCLayer::onExit();
}

/* 网络异常调度 */
void LoadingBox::network_exception_schedule() {
    if (this->getChildByTag(LOADING_TAG)->getChildByTag(MODEL_BG)) {
        this->getChildByTag(LOADING_TAG)->removeChildByTag(MODEL_BG);
    }
    if (this->getChildByTag(LOADING_TAG)->getChildByTag(LOADINGANI)) {
        this->getChildByTag(LOADING_TAG)->removeChildByTag(LOADINGANI);
    }
    if (this->getChildByTag(LOADING_TAG)->getChildByTag(TIPFORNETWORK)) {
        this->getChildByTag(LOADING_TAG)->removeChildByTag(TIPFORNETWORK);
    }
    
    if (this->getChildByTag(LOADING_TAG)->getChildByTag(TIPBOX)) {
        this->getChildByTag(LOADING_TAG)->removeChildByTag(TIPBOX);
    }
    CCNode* pNode = CCNode::create();
    this->getChildByTag(LOADING_TAG)->addChild(pNode, 1, TIPBOX);
    
    CCSprite* tipsForException = CCSprite::create("CM_networkException.png");
    tipsForException->setAnchorPoint(ccp(0.5, 0.5));
    tipsForException->setPosition(ccp(320.0f, 536.0));
    pNode->addChild(tipsForException);
    
    CCSprite* spConfirm = CCSprite::create("CM_netConfirm.png");
    spConfirm->setAnchorPoint(ccp(0.5, 0.5));
    spConfirm->setPosition(ccp(320.0f, 483.0f));
    pNode->addChild(spConfirm, 2, TIPCONFIRMBUTTON);
    
    CCSprite* spConfirmBg = CCSprite::create("CM_LoginBtn.png");
    spConfirmBg->setAnchorPoint(ccp(0.5, 0.5));
    spConfirmBg->setPosition(ccp(320.0f, 485.0f));
    pNode->addChild(spConfirmBg, 1, TIPCONFIRMBUTTONBG);
    
    CCSprite* spConfirmBgS = CCSprite::create("CM_LoginBtnS.png");
    spConfirmBgS->setAnchorPoint(ccp(0.5, 0.5));
    spConfirmBgS->setPosition(ccp(320.0f, 485.0f));
    pNode->addChild(spConfirmBgS, 1, TIPCONFIRMBUTTONBGS);
    spConfirmBgS->setVisible(false);
    
}

/* 移除网络异常提示框 */
void LoadingBox::removeTips() {
    this->removeFromParent();
}

void LoadingBox::show() {
    this->setVisible(true);
}

void LoadingBox::hide() {
    this->setVisible(false);
}

void LoadingBox::registerWithTouchDispatcher(void) {
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority - 1, true);
}

bool LoadingBox::ccTouchBegan(CCTouch *pTouches, CCEvent *pEvent) {
    CCNode* pNode = (CCNode*)this->getChildByTag(LOADING_TAG)->getChildByTag(TIPBOX);
    if (pNode) {
        CCSprite* sp = (CCSprite*)pNode->getChildByTag(TIPCONFIRMBUTTON);
        CCSprite* sp1 = (CCSprite*)pNode->getChildByTag(TIPCONFIRMBUTTONBG);
        CCSprite* sp2 = (CCSprite*)pNode->getChildByTag(TIPCONFIRMBUTTONBGS);
        CCPoint touchPoint = pNode->convertTouchToNodeSpace(pTouches);
        if (sp) {
            if (sp->boundingBox().containsPoint(touchPoint)) {
                if (sp1)
                    sp1->setVisible(false);
                if (sp2)
                    sp2->setVisible(true);
            }
        }
    }
    return true;
}

void LoadingBox::ccTouchMoved(CCTouch *pTouches, CCEvent *pEvent) {
    CCNode* pNode = (CCNode*)this->getChildByTag(LOADING_TAG)->getChildByTag(TIPBOX);
    if (pNode) {
        CCSprite* sp = (CCSprite*)pNode->getChildByTag(TIPCONFIRMBUTTON);
        CCSprite* sp1 = (CCSprite*)pNode->getChildByTag(TIPCONFIRMBUTTONBG);
        CCSprite* sp2 = (CCSprite*)pNode->getChildByTag(TIPCONFIRMBUTTONBGS);
        CCPoint touchPoint = pNode->convertTouchToNodeSpace(pTouches);
        if (sp) {
            if (sp->boundingBox().containsPoint(touchPoint)) {
                if (sp1)
                    sp1->setVisible(true);
                if (sp2)
                    sp2->setVisible(false);
            }
        }
    }
}

void LoadingBox::ccTouchEnded(CCTouch *pTouches, CCEvent *pEvent) {
    CCNode* pNode = (CCNode*)this->getChildByTag(LOADING_TAG)->getChildByTag(TIPBOX);
    if (pNode) {
        CCSprite* sp = (CCSprite*)pNode->getChildByTag(TIPCONFIRMBUTTON);
        CCSprite* sp1 = (CCSprite*)pNode->getChildByTag(TIPCONFIRMBUTTONBG);
        CCSprite* sp2 = (CCSprite*)pNode->getChildByTag(TIPCONFIRMBUTTONBGS);
        CCPoint touchPoint = pNode->convertTouchToNodeSpace(pTouches);
        if (sp) {
            if (sp->boundingBox().containsPoint(touchPoint)) {
                if (sp1)
                    sp1->setVisible(true);
                if (sp2)
                    sp2->setVisible(false);
                removeTips();
            }
        }
    }
}
void LoadingBox::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {
    CCNode* pNode = (CCNode*)this->getChildByTag(LOADING_TAG)->getChildByTag(TIPBOX);
    if (pNode) {
        CCSprite* sp = (CCSprite*)pNode->getChildByTag(TIPCONFIRMBUTTON);
        CCSprite* sp1 = (CCSprite*)pNode->getChildByTag(TIPCONFIRMBUTTONBG);
        CCSprite* sp2 = (CCSprite*)pNode->getChildByTag(TIPCONFIRMBUTTONBGS);
        CCPoint touchPoint = pNode->convertTouchToNodeSpace(pTouch);
        if (sp) {
            if (sp->boundingBox().containsPoint(touchPoint)) {
                if (sp1)
                    sp1->setVisible(true);
                if (sp2)
                    sp2->setVisible(false);
            }
        }
    }
}

