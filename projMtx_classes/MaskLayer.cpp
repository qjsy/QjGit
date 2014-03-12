//
//  MaskLayer.cpp
//  hero
//
//  Created by 钱剑 on 14-1-13.
//
//

#include "MaskLayer.h"

/*
 *  构造函数
 */
MaskLayer::MaskLayer()
{
}

/*
 *  析构函数
 */
MaskLayer::~MaskLayer() {
}

bool MaskLayer::init() {
    if (!CCLayer::init()) {
        return false;
    }
    
    CCSprite* spBg = CCSprite::create("CM_modal.png");
    spBg->setAnchorPoint(ccp(0, 0));
    spBg->setPosition(ccp(0, 0));
    addChild(spBg);
    
    this->setTouchEnabled(true);
    
    return true;
}

bool MaskLayer::ccTouchBegan(CCTouch *pTouches, CCEvent *pEvent) {
    return true;
}

void MaskLayer::registerWithTouchDispatcher() {
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
}

