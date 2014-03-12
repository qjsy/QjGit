//
//  TieJiangLayer.cpp
//  hero
//
//  Created by 钱剑 on 14-3-10.
//
//

#include "TieJiangLayer.h"
#include "GameState.h"

#define BG_V            140.0f

#define BG_TAG          1

/*构造函数*/
TieJiangLayer::TieJiangLayer()
:m_pGameState(GameState::getInstance())
{
}
/*析构函数*/
TieJiangLayer::~TieJiangLayer(){}
/*初始化函数*/
bool TieJiangLayer::init() {
    if (!CCLayer::init()) {
        return false;
    }
    setTouchEnabled(true);
    
    _initUi();
    return true;
}

void TieJiangLayer::_initUi() {
    /*背包背景*/
    CCSprite* spBg = CCSprite::create("pp_bgOfbag.png");
    spBg->setAnchorPoint(CCPointZero);
    spBg->setPosition(ccp(0, BG_V + m_pGameState->getBottomOffset()));
    addChild(spBg, 1, BG_TAG);
    /*filter kuan*/
    CCSprite* kuan = CCSprite::create("pp_kuan.png");
    kuan->setAnchorPoint(CCPointZero);
    kuan->setPosition(ccp(0, 720.0f + m_pGameState->getBottomOffset()));
    addChild(kuan);
    /*page btm*/
    CCSprite* btm = CCSprite::create("pp_btm.png");
    btm->setAnchorPoint(CCPointZero);
    btm->setPosition(ccp(0, -25.0f));
    spBg->addChild(btm);
    /*filter btn*/
    m_font[0][0] = CCSprite::create("pp_prop.png");
    m_font[0][0]->setAnchorPoint(CCPointZero);
    m_font[0][0]->setPosition(ccp(123.0f, 615.0f));
    spBg->addChild(m_font[0][0], 4);
    m_font[0][1] = CCSprite::create("pp_prop_s.png");
    m_font[0][1]->setAnchorPoint(CCPointZero);
    m_font[0][1]->setPosition(ccp(123.0f, 615.0f));
    m_font[0][1]->setVisible(false);
    spBg->addChild(m_font[0][1], 5);
    m_font[1][0] = CCSprite::create("pp_forge.png");
    m_font[1][0]->setAnchorPoint(CCPointZero);
    m_font[1][0]->setPosition(ccp(123.0f + 110.0f * 1, 615.0f));
    spBg->addChild(m_font[1][0], 4);
    m_font[1][1] = CCSprite::create("pp_forge_s.png");
    m_font[1][1]->setAnchorPoint(CCPointZero);
    m_font[1][1]->setPosition(ccp(123.0f + 110.0f * 1, 615.0f));
    m_font[1][1]->setVisible(false);
    spBg->addChild(m_font[1][1], 5);
    m_font[2][0] = CCSprite::create("pp_purify.png");
    m_font[2][0]->setAnchorPoint(CCPointZero);
    m_font[2][0]->setPosition(ccp(123.0f + 110.0f * 2, 615.0f));
    spBg->addChild(m_font[2][0], 4);
    m_font[2][1] = CCSprite::create("pp_purify_s.png");
    m_font[2][1]->setAnchorPoint(CCPointZero);
    m_font[2][1]->setPosition(ccp(123.0f + 110.0f * 2, 615.0f));
    m_font[2][1]->setVisible(false);
    spBg->addChild(m_font[2][1], 5);
    m_font[3][0] = CCSprite::create("pp_bag.png");
    m_font[3][0]->setAnchorPoint(CCPointZero);
    m_font[3][0]->setPosition(ccp(123.0f + 110.0f * 3, 615.0f));
    spBg->addChild(m_font[3][0], 4);
    m_font[3][1] = CCSprite::create("pp_bg_s.png");
    m_font[3][1]->setAnchorPoint(CCPointZero);
    m_font[3][1]->setPosition(ccp(123.0f + 110.0f * 3, 615.0f));
    m_font[3][1]->setVisible(false);
    spBg->addChild(m_font[3][1], 5);
    
    for (int i = 0; i < 4; ++i) {
        m_btn[i][0] = CCSprite::create("pp_nor.png");
        m_btn[i][0]->setAnchorPoint(CCPointZero);
        m_btn[i][0]->setPosition(ccp(100.0f + 110.0f * i, 605.0f));
        spBg->addChild(m_btn[i][0]);
        m_btn[i][1] = CCSprite::create("pp_sel.png");
        m_btn[i][1]->setAnchorPoint(CCPointZero);
        m_btn[i][1]->setPosition(ccp(100.0f + 110.0f * i, 605.0f));
        m_btn[i][1]->setVisible(false);
        spBg->addChild(m_btn[i][1], 2);
        m_enableTouch[i] = true;
    }

    
}

void TieJiangLayer::_initFilterBtn() {
    for (int i = 0; i < 4; ++i) {
        m_btn[i][0]->setVisible(true);
        m_btn[i][1]->setVisible(false);
    }
}

void TieJiangLayer::_onBtnSelected(int idx) {
    switch (idx) {
        case 1:
        {
            CCLog("装备");
        }
            break;
        case 2:
        {
            CCLog("锻造");
        }
            break;
        case 3:
        {
            CCLog("精炼");
        }
            break;
        case 4:
        {
            CCLog("背包");
        }
            break;
        default:
            break;
    }
}

void TieJiangLayer::registerWithTouchDispatcher() {
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, false);
}
bool TieJiangLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    bool validTouch = false;
    m_touchBeganBtnidx = -1;
    CCSprite* pbg = (CCSprite*)this->getChildByTag(BG_TAG);
    CCPoint touchPoint = pbg->convertTouchToNodeSpace(pTouch);
    for (int i = 0; i < 4; ++i) {
        if (m_enableTouch[i] && m_btn[i][0]->boundingBox().containsPoint(touchPoint)) {
            m_touchBeganBtnidx = i;
            
            validTouch = true;
        }
    }
    
    return validTouch;
}
void TieJiangLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){}
void TieJiangLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    _initFilterBtn();
    CCSprite* pbg = (CCSprite*)this->getChildByTag(BG_TAG);
    CCPoint touchPoint = pbg->convertTouchToNodeSpace(pTouch);
    int selectIdx = -1;
    for (int i = 0; i < 4; ++i) {
        if (m_enableTouch[i] && m_btn[i][0]->boundingBox().containsPoint(touchPoint)) {
            selectIdx = i + 1;
            
            m_btn[i][0]->setVisible(false);
            m_btn[i][1]->setVisible(true);
            break;
        }
    }
    
    if (selectIdx > 0) {
        _onBtnSelected(selectIdx);
    }
    
}
void TieJiangLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){}
void TieJiangLayer::restoreTouchUICompent(){}
void TieJiangLayer::moveOutTouchUICompent(){}
void TieJiangLayer::_callbackForMoveOut(){}
void TieJiangLayer::initPosition(){}
