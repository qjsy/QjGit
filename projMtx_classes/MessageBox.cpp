//
//  MessageBox.cpp
//  hero
//
//  Created by 钱剑 on 14-1-14.
//
//

#include "MessageBox.h"
#include "MaskLayer.h"
#include "CardBox.h"
#include "HeroItem.h"
#include "CCStrokeLabel.h"

// tag
#define MESSAGE_BOX_TAG                 1111111
#define MESSAGE_LABEL_TAG               2222222
#define MASKLAYER_TAG                   3333333
#define MESSAGE_FOR_TQT_TAG             4444444

// value
#define DURATION                        0.5f
#define TEXT_PADDING_H                  50.0f
#define TEXT_PADDING_V                  100.0f
#define BTN_CONFIRM_H                   135.0f
#define BTN_CONFIRM_V                   80.0f

MessageBox::MessageBox()
{
    m_Scene = CCDirector::sharedDirector()->getRunningScene();
}

MessageBox::~MessageBox() {
    
}

bool MessageBox::init() {
    if (!CCLayer::init()) {
        CCLayer::init();
    }

    return true;
}

/* 单例模式 静态实例变量 */
static MessageBox* s_MessageBox = NULL;
MessageBox* MessageBox::getInstance() {
    if (NULL == s_MessageBox) {
        s_MessageBox = new MessageBox();
        s_MessageBox->init();
    }
    return s_MessageBox;
}

/*
 *弹出框
 * callfunc_selector
 */
void MessageBox::messageForBox(const char* title, int titleSize, const char* text, int textSize, CCObject* target, SEL_CallFunc callfun, float scale) {
    m_Scene = CCDirector::sharedDirector()->getRunningScene();
    if (m_Scene->getChildByTag(MESSAGE_BOX_TAG)) {
        m_Scene->removeChildByTag(MESSAGE_BOX_TAG);
    }
    if (m_Scene->getChildByTag(MASKLAYER_TAG)) {
        m_Scene->removeChildByTag(MASKLAYER_TAG);
    }
    m_Scene = CCDirector::sharedDirector()->getRunningScene();
    
    m_func = callfun;
    m_target = target;
    
    /* 背景图片 */
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCSprite* msgBg = CCSprite::create("CM_msgBox.png");
    m_Scene->addChild(msgBg, 99999999, MESSAGE_BOX_TAG);
    msgBg->setPosition(ccp(size.width / 2, size.height / 2));
    
    CCLabelTTF* pTitle = CCLabelTTF::create(title, "Arial", titleSize);
    msgBg->addChild(pTitle);
    pTitle->setAnchorPoint(ccp(0, 0));
    pTitle->setPosition(ccp((msgBg->getContentSize().width - pTitle->getContentSize().width) / 2,
                             msgBg->getContentSize().height - pTitle->getContentSize().height - 20.0f));
    
    CCLabelTTF* pText = CCLabelTTF::create(text, "Arial", textSize, CCSizeMake(msgBg->getContentSize().width - TEXT_PADDING_H * 2,
                                                                               msgBg->getContentSize().height - 170.0f), kCCTextAlignmentLeft);
    msgBg->addChild(pText);
    pText->setAnchorPoint(ccp(0, 0));
    pText->setPosition(ccp(TEXT_PADDING_H, TEXT_PADDING_V));
    
    /*确认按钮*/
    CCMenuItemImage* pConfirm = CCMenuItemImage::create("CM_LoginBtn.png", "CM_LoginBtnS.png", this, menu_selector(MessageBox::_callbackConfirm));
    pConfirm->setPosition(ccp(BTN_CONFIRM_H, BTN_CONFIRM_V));
    CCSprite* pConfirmFont = CCSprite::create("CM_netConfirm.png");
    pConfirmFont->setPosition(ccp(BTN_CONFIRM_H, BTN_CONFIRM_V));
    msgBg->addChild(pConfirmFont, 2);
    
    /*取消按钮*/
    CCMenuItemImage* pCancel = CCMenuItemImage::create("CM_LoginBtn.png", "CM_LoginBtnS.png", this, menu_selector(MessageBox::_callbackCancel));
    pCancel->setPosition(ccp(msgBg->getContentSize().width - BTN_CONFIRM_H, BTN_CONFIRM_V));
    CCSprite* pCancelFont = CCSprite::create("CM_netCancel.png");
    pCancelFont->setPosition(ccp(msgBg->getContentSize().width - BTN_CONFIRM_H, BTN_CONFIRM_V));
    msgBg->addChild(pCancelFont, 2);
    
    CCMenu* menu = CCMenu::create(pConfirm, pCancel, NULL);
    menu->setPosition(ccp(0, 0));
    menu->setTouchPriority(-130);
    msgBg->addChild(menu,1);
    
    /*遮罩层*/
    MaskLayer* pMask = MaskLayer::create();
    pMask->setAnchorPoint(ccp(0, 0));
    pMask->setPosition(ccp(0, 0));
    m_Scene->addChild(pMask, 99999998, MASKLAYER_TAG);
    
    /*缩放*/
    msgBg->setScale(scale);
}

void MessageBox::_callbackConfirm() {
    if (m_Scene->getChildByTag(MASKLAYER_TAG)) {
        m_Scene->removeChildByTag(MASKLAYER_TAG);
    }
    if (m_Scene->getChildByTag(MESSAGE_BOX_TAG)) {
        m_Scene->removeChildByTag(MESSAGE_BOX_TAG);
    }
    /* 执行回调 */
    (m_target->*m_func)();
}

void MessageBox::_callbackCancel() {
    if (m_Scene->getChildByTag(MASKLAYER_TAG)) {
        m_Scene->removeChildByTag(MASKLAYER_TAG);
    }
    if (m_Scene->getChildByTag(MESSAGE_BOX_TAG)) {
        m_Scene->removeChildByTag(MESSAGE_BOX_TAG);
    }
    if (m_Scene->getChildByTag(MESSAGE_FOR_TQT_TAG)) {
        m_Scene->removeChildByTag(MESSAGE_FOR_TQT_TAG);
    }
}

/* 文字提示 */
void MessageBox::messageTips(const char* text, const int colorType, const float distance) {
    m_Scene = CCDirector::sharedDirector()->getRunningScene();
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    if (m_Scene) {
        CCLabelTTF* pLabel = CCLabelTTF::create(text, "Arial", 27);
        if (colorType == 0) {
            pLabel->setColor(RED_COLOR);
        } else if (colorType == 1) {
            pLabel->setColor(GREEN_COLOR);
        }
        
        m_Scene->addChild(pLabel, 999999999, MESSAGE_LABEL_TAG);
        pLabel->setPosition(ccp(size.width / 2, size.height / 2 - distance));
        pLabel->setOpacity(0);
        CCFiniteTimeAction * spawn1 =CCSpawn::create(CCMoveBy::create(DURATION - 0.1, ccp(0, 0.6*fabs(distance))), CCFadeIn::create(DURATION - 0.1), NULL);
        CCFiniteTimeAction * spawn2 =CCSpawn::create(CCMoveBy::create(DURATION - 0.2, ccp(0, 0.4*fabs(distance))), CCFadeOut::create(DURATION - 0.2), NULL);
        pLabel->runAction(CCSequence::create(spawn1, CCDelayTime::create(DURATION) ,spawn2,CCCallFuncN::create(this, callfuncN_selector(MessageBox::_callbackForRemoveSelf)) ,NULL));
    }
}

void MessageBox::_callbackForRemoveSelf() {
    if (m_Scene->getChildByTag(MESSAGE_LABEL_TAG)) {
        m_Scene->removeChildByTag(MESSAGE_LABEL_TAG);
    }
}

/* 铜雀台message */
void MessageBox::messageForTqt(HeroItem* pItem) {
    m_Scene = CCDirector::sharedDirector()->getRunningScene();
    
    if (m_Scene->getChildByTag(MESSAGE_FOR_TQT_TAG)) {
        m_Scene->removeChildByTag(MESSAGE_FOR_TQT_TAG);
    }
    if (m_Scene->getChildByTag(MASKLAYER_TAG)) {
        m_Scene->removeChildByTag(MASKLAYER_TAG);
    }
    /* 背景图片 */
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCSprite* msgBg = CCSprite::create("CM_msgBox.png");
    m_Scene->addChild(msgBg, 99999999, MESSAGE_FOR_TQT_TAG);
    msgBg->setPosition(ccp(size.width / 2, size.height / 2));
    
    const char* title = "招降成功";
    CCLabelTTF* pTitle = CCLabelTTF::create(title, "Arial", 30);
    msgBg->addChild(pTitle);
    pTitle->setAnchorPoint(ccp(0, 0));
    pTitle->setPosition(ccp((msgBg->getContentSize().width - pTitle->getContentSize().width) / 2,
                            msgBg->getContentSize().height - pTitle->getContentSize().height - 20.0f));
    const char* context = "恭喜您，招降到 ";
    CCLabelTTF* pText = CCLabelTTF::create(context, "Arial", 25, CCSizeMake(msgBg->getContentSize().width - TEXT_PADDING_H * 2,
                                                                               msgBg->getContentSize().height - 170.0f), kCCTextAlignmentLeft);
    msgBg->addChild(pText);
    pText->setAnchorPoint(ccp(0, 0));
    pText->setPosition(ccp(TEXT_PADDING_H + 95.0f, TEXT_PADDING_V));
    CCLabelTTF* pHeroName = CCLabelTTF::create(pItem->getName().c_str(), "Arial", 25, CCSizeMake(msgBg->getContentSize().width - TEXT_PADDING_H * 2,
                                                                            msgBg->getContentSize().height - 170.0f), kCCTextAlignmentLeft);
    msgBg->addChild(pHeroName);
    int rarity = pItem->getRarity();
    if (rarity > 3 && rarity <= 5) {
        pHeroName->setColor(BLUE_COLOR);
    } else if (rarity > 5 && rarity <= 8) {
        pHeroName->setColor(PURPLE_COLOR);
    }
    pHeroName->setAnchorPoint(ccp(0, 0));
    pHeroName->setPosition(ccp(TEXT_PADDING_H + 275.0f, TEXT_PADDING_V));
    const char* tips = "提示: 招降到的武将已进入背包!";
    CCLabelTTF* pTips = CCLabelTTF::create(tips, "Arial", 25, CCSizeMake(msgBg->getContentSize().width - TEXT_PADDING_H * 2,
                                                                            msgBg->getContentSize().height - 170.0f), kCCTextAlignmentLeft);
    msgBg->addChild(pTips);
    pTips->setAnchorPoint(ccp(0, 0));
    pTips->setPosition(ccp(TEXT_PADDING_H, TEXT_PADDING_V - 115.0f));
    
    CardBox* pCard = CardBox::create();
    pCard->setCard(pItem);
    pCard->setPosition(ccp(30.0f, 175.0f));
    msgBg->addChild(pCard);
    
    /*遮罩层*/
    MaskLayer* pMask = MaskLayer::create();
    pMask->setAnchorPoint(ccp(0, 0));
    pMask->setPosition(ccp(0, 0));
    m_Scene->addChild(pMask, 99999998, MASKLAYER_TAG);
    
    /*取消按钮*/
    CCMenuItemImage* pCancel = CCMenuItemImage::create("CM_LoginBtn.png", "CM_LoginBtnS.png", this, menu_selector(MessageBox::_callbackCancel));
    pCancel->setPosition(ccp(256.0f, BTN_CONFIRM_V));
    CCSprite* pCancelFont = CCSprite::create("CM_netConfirm.png");
    pCancelFont->setPosition(ccp(256.0f, BTN_CONFIRM_V));
    msgBg->addChild(pCancelFont, 2);
    
    CCMenu* menu = CCMenu::create(pCancel, NULL);
    menu->setPosition(ccp(0, 0));
    menu->setTouchPriority(-130);
    msgBg->addChild(menu,1);
}

void MessageBox::messageTipsX(const char* text, const int colorType, const float distance) {
    m_Scene = CCDirector::sharedDirector()->getRunningScene();

    CCSize size = CCDirector::sharedDirector()->getWinSize();
    if (m_Scene) {
        CCStrokeLabel* pLabel = CCStrokeLabel::create(text, "Arial", 27);
        if (colorType == 0) {
            pLabel->setColor(RED_COLOR);
        } else if (colorType == 1) {
            pLabel->setColor(GREEN_COLOR);
        }
        
        m_Scene->addChild(pLabel, 999999999, MESSAGE_LABEL_TAG);
        pLabel->setAnchorPoint(ccp(0, 0));
        pLabel->setPosition(ccp(size.width / 2 - 100.0f, size.height / 2 - distance));
//        pLabel->setOpacity(0);
        CCFiniteTimeAction * spawn1 =CCSpawn::create(CCMoveBy::create(DURATION - 0.1, ccp(0, 0.6*fabs(distance))), CCFadeIn::create(DURATION - 0.1), NULL);
        CCFiniteTimeAction * spawn2 =CCSpawn::create(CCMoveBy::create(DURATION - 0.2, ccp(0, 0.4*fabs(distance))), CCFadeOut::create(DURATION - 0.2), NULL);
        pLabel->runAction(CCSequence::create(spawn1, CCDelayTime::create(DURATION) ,spawn2,CCCallFuncN::create(this, callfuncN_selector(MessageBox::_callbackForRemoveSelf)) ,NULL));
    }
}
