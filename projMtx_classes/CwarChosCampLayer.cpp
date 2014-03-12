//
//  CwarChosCampLayer.cpp
//  hero
//
//  Created by 钱剑 on 14-3-4.
//
//

#include "CwarChosCampLayer.h"
#include "GameState.h"
#include "MaskLayer.h"
#include "NetConnection.h"
#include "GamePlayer.h"
#include "MessageBox.h"

#define BG_TAG                      1

/*构造函数*/
CwarChosCampLayer::CwarChosCampLayer()
:m_pGameState(GameState::getInstance())
{
    memset(m_btnSp, '\0', sizeof(m_btnSp));
}
/*析构函数*/
CwarChosCampLayer::~CwarChosCampLayer(){}
/*初始化*/
bool CwarChosCampLayer::init(){
    if (!CCLayer::init()) {
        return false;
    }
    this->setTouchEnabled(true);
    
    /*初始化UI*/
    CCSprite* bgOfChosCamp = CCSprite::create("CM_msgBox.png");
    bgOfChosCamp->setAnchorPoint(ccp(0.5, 0.5));
    bgOfChosCamp->setPosition(ccp(320.0f, 500.0f));
    this->addChild(bgOfChosCamp, 2, BG_TAG);
    /*按钮*/
    bgOfChosCamp->addChild(m_btnSp[0][0] = CCSprite::create("f1.png"), 8);
    m_btnSp[0][0]->setAnchorPoint(CCPointZero);
    m_btnSp[0][0]->setPosition(ccp(120.0f, 250.0f));
    bgOfChosCamp->addChild(m_btnSp[0][1] = CCSprite::create("HL_wu.png"), 9);
    m_btnSp[0][1]->setAnchorPoint(CCPointZero);
    m_btnSp[0][1]->setVisible(false);
    m_btnSp[0][1]->setPosition(ccp(120.0f, 250.0f));
    
    bgOfChosCamp->addChild(m_btnSp[1][0] = CCSprite::create("f2.png"), 8);
    m_btnSp[1][0]->setAnchorPoint(CCPointZero);
    m_btnSp[1][0]->setPosition(ccp(220.0f, 250.0f));
    bgOfChosCamp->addChild(m_btnSp[1][1] = CCSprite::create("HL_shu.png"), 9);
    m_btnSp[1][1]->setAnchorPoint(CCPointZero);
    m_btnSp[1][1]->setVisible(false);
    m_btnSp[1][1]->setPosition(ccp(220.0f, 250.0f));
    
    bgOfChosCamp->addChild(m_btnSp[2][0] = CCSprite::create("f3.png"), 8);
    m_btnSp[2][0]->setAnchorPoint(CCPointZero);
    m_btnSp[2][0]->setPosition(ccp(320.0f, 250.0f));
    bgOfChosCamp->addChild(m_btnSp[2][1] = CCSprite::create("HL_wei.png"), 9);
    m_btnSp[2][1]->setAnchorPoint(CCPointZero);
    m_btnSp[2][1]->setVisible(false);
    m_btnSp[2][1]->setPosition(ccp(320.0f, 250.0f));
    
    bgOfChosCamp->addChild(m_confirmSp[0] = CCSprite::create("CM_LoginBtn.png"), 8);
    m_confirmSp[0]->setAnchorPoint(CCPointZero);
    m_confirmSp[0]->setPosition(ccp(165, 45));
    bgOfChosCamp->addChild(m_confirmSp[1] = CCSprite::create("CM_LoginBtnS.png"), 9);
    m_confirmSp[1]->setAnchorPoint(CCPointZero);
    m_confirmSp[1]->setVisible(false);
    m_confirmSp[1]->setPosition(ccp(165, 45));
    CCSprite* spConfirm = CCSprite::create("CM_netConfirm.png");
    spConfirm->setAnchorPoint(ccp(0, 0));
    spConfirm->setPosition(ccp(215.0f, 47.0f));
    bgOfChosCamp->addChild(spConfirm, 10);
    
    
    MaskLayer* pMask = MaskLayer::create();
    pMask->setAnchorPoint(ccp(0, 0));
    pMask->setPosition(ccp(0, 0));
    addChild(pMask);
    
    return true;
}

void CwarChosCampLayer::registerWithTouchDispatcher(void){
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority - 1, false);
}

bool CwarChosCampLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent){
    bool validTouch = false;
    
    
    CCSprite* pBg = static_cast<CCSprite*>(this->getChildByTag(BG_TAG));
    CCPoint touchPoint = pBg->convertTouchToNodeSpace(pTouch);
    
    int selectedIdx = -1;
    for (int i = 0; i < 3; ++i) {
        if (m_btnSp[i][0]->boundingBox().containsPoint(touchPoint)) {
            _initBtn();
            selectedIdx = i + 1;
            m_btnSp[i][0]->setVisible(false);
            m_btnSp[i][1]->setVisible(true);
            validTouch = true;
            
            break;
        }
    }
    
    if (selectedIdx > 0) {
        _onSelectBtn(selectedIdx);
    }
    
    /*确认*/
    if (m_confirmSp[0]) {
        if (m_confirmSp[0]->boundingBox().containsPoint(touchPoint)) {
            m_confirmSp[0]->setVisible(false);
            m_confirmSp[1]->setVisible(true);
            validTouch = true;
        }
    }
    
    return validTouch;
}

void CwarChosCampLayer::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent){

    CCSprite* pBg = static_cast<CCSprite*>(this->getChildByTag(BG_TAG));
    CCPoint touchPoint = pBg->convertTouchToNodeSpace(pTouch);
    
    for (int i = 0; i < 3; ++i) {
        if (m_btnSp[i][0]->boundingBox().containsPoint(touchPoint)) {
//            _initBtn();
//            m_btnSp[i][0]->setVisible(false);
//            m_btnSp[i][1]->setVisible(true);
            
            break;
        }
    }
    
    /*确认*/
    if (m_confirmSp[0]) {
        if (m_confirmSp[0]->boundingBox().containsPoint(touchPoint)) {
            m_confirmSp[0]->setVisible(false);
            m_confirmSp[1]->setVisible(true);
        }
    }
}

void CwarChosCampLayer::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent){
    CCSprite* pBg = static_cast<CCSprite*>(this->getChildByTag(BG_TAG));
    CCPoint touchPoint = pBg->convertTouchToNodeSpace(pTouch);
    
    /*确认*/
    if (m_confirmSp[0]) {
        if (m_confirmSp[0]->boundingBox().containsPoint(touchPoint)) {
            _chosConfirm();
        }
    }
    m_confirmSp[0]->setVisible(true);
    m_confirmSp[1]->setVisible(false);
}

void CwarChosCampLayer::ccTouchCancelled(CCTouch*, CCEvent*){_initBtn();}

void CwarChosCampLayer::_initBtn() {
    for (int i = 0; i < 3; ++i) {
        if (m_btnSp[i][0]) {
            m_btnSp[i][0]->setVisible(true);
            m_btnSp[i][1]->setVisible(false);
        }
    }
    
    m_confirmSp[0]->setVisible(true);
    m_confirmSp[1]->setVisible(false);
}

void CwarChosCampLayer::_onSelectBtn(int idx) {
    switch (idx) {
        case 1:
        {
            CCLog("记录选择吴国的状态");
            m_pGameState->setSelectCamp(3);
        }
            break;
        case 2:
        {
            CCLog("记录选择蜀国的状态");
            m_pGameState->setSelectCamp(2);
        }
            break;
        case 3:
        {
            CCLog("记录选择魏国的状态");
            m_pGameState->setSelectCamp(1);
        }
            break;
            
        default:
            break;
    }
}

void CwarChosCampLayer::_chosConfirm() {
    int chosCamp = m_pGameState->getSelectCamp();
    NetConnection* pNetConnection = NetConnection::getInstance();
    GamePlayer* pGamePlayer = GamePlayer::getInstance();
    if (chosCamp == 1)/*吴国*/ {
        char szPostBuffer[MAX_POSTTO_SERVER_BUFFER_SIZE];
        memset(szPostBuffer, '\0', MAX_POSTTO_SERVER_BUFFER_SIZE);
        strcpy(szPostBuffer, "c=cwar&a=init");
        char szHttpTag[] = HTTP_REQUEST_CHOSCAMP_TAG;
        strcat(szPostBuffer, "&group=1");
        strcat(szPostBuffer, "&uid=");
        strcat(szPostBuffer, pGamePlayer->getUid().c_str());
        pNetConnection->commitPostRequestData(szPostBuffer, szHttpTag);
    } else if (chosCamp == 2)/*蜀国*/ {
        char szPostBuffer[MAX_POSTTO_SERVER_BUFFER_SIZE];
        memset(szPostBuffer, '\0', MAX_POSTTO_SERVER_BUFFER_SIZE);
        strcpy(szPostBuffer, "c=cwar&a=init");
        char szHttpTag[] = HTTP_REQUEST_CHOSCAMP_TAG;
        strcat(szPostBuffer, "&group=2");
        strcat(szPostBuffer, "&uid=");
        strcat(szPostBuffer, pGamePlayer->getUid().c_str());
        pNetConnection->commitPostRequestData(szPostBuffer, szHttpTag);
    } else if (chosCamp == 3)/*魏国*/ {
        char szPostBuffer[MAX_POSTTO_SERVER_BUFFER_SIZE];
        memset(szPostBuffer, '\0', MAX_POSTTO_SERVER_BUFFER_SIZE);
        strcpy(szPostBuffer, "c=cwar&a=init");
        char szHttpTag[] = HTTP_REQUEST_CHOSCAMP_TAG;
        strcat(szPostBuffer, "&group=3");
        strcat(szPostBuffer, "&uid=");
        strcat(szPostBuffer, pGamePlayer->getUid().c_str());
        pNetConnection->commitPostRequestData(szPostBuffer, szHttpTag);
    } else/*没有选择*/ {
        const char* title = "您没有选择阵营";
        MessageBox::getInstance()->messageTips(title);
    }
    
    
    this->removeFromParent();
}