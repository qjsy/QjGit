
#include "YiZhanShowLayer.h"
#include "common.h"
#include "customMessage.h"
#include "NetConnection.h"
#include "HeroManager.h"
#include "HeroItem.h"
#include "HeroOfPlayerItem.h"
#include "Json.h"
#include "CardBox.h"
#include "GamePlayer.h"
#include "GameState.h"

#define BTN_CHOU                    100
#define NODE_TAG                    1000
#define UPDATE_TAG                  1001
#define FREE_CHOU                   1
#define NFREE_CHOU                  2
#define NFREETEN_CHOU               3

#define MOVEOUT_TAG                         301
#define MOVEIN_TAG                          300

#define ANIMATION_DURING                    0.2

#define BGOFYIZHAN                          101

/*
 *  构造函数
 */
YiZhanShowLayer::YiZhanShowLayer():
m_pGameState(GameState::getInstance())
,m_pGamePlayer(GamePlayer::getInstance())
{}

/*
 *  析构函数
 */
YiZhanShowLayer::~YiZhanShowLayer() {
    CCTextureCache* pTextureCache = CCTextureCache::sharedTextureCache();
    
    pTextureCache->removeTextureForKey("MC_bgOfyiz.png");
    pTextureCache->removeTextureForKey("MC_remainTimeBg.png");
    pTextureCache->removeTextureForKey("MC_rmb1.png");
    pTextureCache->removeTextureForKey("MC_courSerach.png");
    pTextureCache->removeTextureForKey("CM_bigBtn.png");
    pTextureCache->removeTextureForKey("MC_courage.png");
    pTextureCache->removeTextureForKey("MC_rmbSearch.png");
    pTextureCache->removeTextureForKey("MC_rmbSearch1.png");
    pTextureCache->removeTextureForKey("MC_rmb10.png");
}

/*
 *  初始化
 */
bool YiZhanShowLayer::init() {
    // super init
    if (!CCLayer::init()) {
        return false;
    }
    this->setTouchEnabled(true);
    
    // 背景图片
    CCNode* pNode = CCNode::create();
    pNode->setAnchorPoint(ccp(0 ,0));
    pNode->setPosition(ccp(0, m_pGameState->getBottomOffset()));
    addChild(pNode, 1, NODE_TAG);
    
    CCSprite* spBg = CCSprite::create("MC_bgOfyiz.png");
    spBg->setAnchorPoint(ccp(0, 0));
    spBg->setPosition(ccp(0, -m_pGameState->getBottomOffset()));
    pNode->addChild(spBg,1,BGOFYIZHAN);

    return true;
}

void YiZhanShowLayer::setVisible(bool visible) {
    CCLayer::setVisible(visible);
    if (visible) {
        if (this->getChildByTag(NODE_TAG)->getChildByTag(UPDATE_TAG)) {
            this->getChildByTag(NODE_TAG)->removeChildByTag(UPDATE_TAG);
        }
    }
}

void YiZhanShowLayer::registerWithTouchDispatcher(void) {
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
}

bool YiZhanShowLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    return true;
}

void YiZhanShowLayer::returnNow() {
    CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_YIZHAN_STRING);
}

//
void YiZhanShowLayer::update(const int mode) {
    char buffer[16];
    CCNode* pParent = (CCNode*)this->getChildByTag(NODE_TAG);
    if (pParent->getChildByTag(UPDATE_TAG)) {
        pParent->removeChildByTag(UPDATE_TAG);
    }
    CCNode* pUpdate = CCNode::create();
    pUpdate->setAnchorPoint(ccp(0, 0));
    pUpdate->setPosition(ccp(0, 0));
    pParent->addChild(pUpdate, 1, UPDATE_TAG);
    if (FREE_CHOU == mode) {
        /*抽取次数*/
        CCSprite* sp = CCSprite::create("MC_remainTimeBg.png");
        sp->setAnchorPoint(ccp(0, 0));
        sp->setPosition(ccp(102, 195));
        pUpdate->addChild(sp);
        int times = 0;
        times = floor(m_pGamePlayer->getCourage() / m_pGamePlayer->getLuckCourage());
        sprintf(buffer, "%d", times);
        CCLabelTTF* labelTimes = CCLabelTTF::create(buffer, "Arial", 21);
        labelTimes->setAnchorPoint(ccp(0.5 ,0));
        labelTimes->setPosition(ccp(138, 212));
        if (times == 0) {
            sp->setVisible(false);
            labelTimes->setVisible(false);
        } else {
            sp->setVisible(true);
            labelTimes->setVisible(true);
        }
        pUpdate->addChild(labelTimes);
        
        CCMenuItemImage *pFree = CCMenuItemImage::create("MC_rmb1.png",
                                                         "MC_rmb1.png",
                                                         this,
                                                         menu_selector(YiZhanShowLayer::again));
        pFree->setTag(BTN_CHOU);
        pFree->setAnchorPoint(CCPointZero);
        pFree->setPosition(ccp(40, 155));
        CCMenu* pFreeMenu = CCMenu::create(pFree, NULL);
        pFreeMenu->setAnchorPoint(ccp(0, 0));
        pFreeMenu->setPosition(ccp(0, 0));
        pUpdate->addChild(pFreeMenu);
        CCSprite* l = CCSprite::create("MC_courSerach.png");
        l->setAnchorPoint(ccp(0, 0));
        l->setPosition(ccp(40 + (pFree->getContentSize().width - l->getContentSize().width) / 2, 157 +
                           (pFree->getContentSize().height - l->getContentSize().height) / 2));
        pUpdate->addChild(l, 2);
        
        CCMenuItemImage *pReturn = CCMenuItemImage::create("CM_bigBtn.png",
                                                           "CM_bigBtn.png",
                                                           this,
                                                           menu_selector(YiZhanShowLayer::returnNow));
        pReturn->setAnchorPoint(CCPointZero);
        pReturn->setPosition(ccp(350, 155));
        CCMenu* pReturnMenu = CCMenu::create(pReturn, NULL);
        pReturnMenu->setAnchorPoint(ccp(0, 0));
        pReturnMenu->setPosition(ccp(0, 0));
        pUpdate->addChild(pReturnMenu);
        CCLabelTTF* lReturn = CCLabelTTF::create("返回", "Arial", 23);
        lReturn->setAnchorPoint(ccp(0, 0));
        lReturn->setPosition(ccp(350 + (pReturn->getContentSize().width - lReturn->getContentSize().width) / 2, 155 +
                                 (pReturn->getContentSize().height - lReturn->getContentSize().height) / 2));
        pUpdate->addChild(lReturn, 2);
        
        CCAssert(m_heroFromSvr, "invalid buffer");
        int index = 0;
        Json* pRootJson = Json_create(m_heroFromSvr);
        do {
            CC_BREAK_IF(!pRootJson);
            Json* pSuccessJson = Json_getItem(pRootJson, "success");
            CC_BREAK_IF(!pSuccessJson);
            if (0 == pSuccessJson->valueint) {
                Json* pMsgJson = Json_getItem(pRootJson, "msg");
                CCLog("error msg = %s", pMsgJson->valuestring);
                CCLabelTTF* errLabel = CCLabelTTF::create(pMsgJson->valuestring, "Arial", 23);
                errLabel->setAnchorPoint(ccp(0, 0));
                errLabel->setPosition(ccp(150, 300));
                errLabel->setColor(ccc3(0,0,0));
                pUpdate->addChild(errLabel);
            }
            CC_BREAK_IF(1 != pSuccessJson->valueint);
            Json* pResultJson = Json_getItem(pRootJson, "result");
            Json* pHItemJson = Json_getItem(pResultJson, "hero");
            Json* pHeroItemJson = pHItemJson->child;
            
            while (pHeroItemJson) {
                if (pHeroItemJson) {
                    int i, j;
                    i = index % 5;
                    j = (index - i) / 5;
                    Json* pHeroItemTagJson = Json_getItem(pHeroItemJson, "tag");
                    CC_BREAK_IF(!pHeroItemTagJson);
                    Json* pIdTagJson = Json_getItem(pHeroItemJson, "id");
                    CC_BREAK_IF(!pIdTagJson);
                    Json* pHeroItemLevelJson = Json_getItem(pHeroItemJson, "level");
                    CC_BREAK_IF(!pHeroItemLevelJson);
                    
                    HeroOfPlayerItem* p = HeroManager::getInstance()->getHeroDataByUniId(pHeroItemTagJson->valuestring);
                    //                    CCSprite* spHero = CCSprite::create(p->getHeroItem()->getHIcon().c_str());
                    CardBox* card = CardBox::create();
                    card->setCard(2, p);
                    card->setScale(0.8);
                    card->setAnchorPoint(ccp(0, 0));
                    card->setPosition(ccp(35 + i * 115, j * 175 + 470));
                    pUpdate->addChild(card);
                }
                pHeroItemJson = pHeroItemJson->next;
                index++;
            }
        }while(0);
        if (pRootJson) {
            Json_dispose(pRootJson);
            pRootJson = NULL;
        }
    } else if (NFREE_CHOU == mode) {
        /*显示剩余次数*/
        CCSprite* sp = CCSprite::create("MC_remainTimeBg.png");
        sp->setAnchorPoint(ccp(0, 0));
        sp->setPosition(ccp(86, 195));
        pUpdate->addChild(sp);
        CCSprite* sp2 = CCSprite::create("MC_remainTimeBg.png");
        sp2->setAnchorPoint(ccp(0, 0));
        sp2->setPosition(ccp(285, 195));
        pUpdate->addChild(sp2);
        int remainTime = 0;
        remainTime = m_pGamePlayer->getGold() / m_pGamePlayer->getLuckRMB();
        sprintf(buffer, "%d", remainTime);
        CCLabelTTF* l1 = CCLabelTTF::create(buffer, "Arial", 21);
        l1->setAnchorPoint(ccp(0.5, 0));
        l1->setPosition(ccp(122, 212));
        if (remainTime == 0) {
            sp->setVisible(false);
            l1->setVisible(false);
        } else {
            sp->setVisible(true);
            l1->setVisible(true);
        }
        pUpdate->addChild(l1);
        remainTime = m_pGamePlayer->getGold() / m_pGamePlayer->getLuckRMBBig();
        sprintf(buffer, "%d", remainTime);
        CCLabelTTF* l2 = CCLabelTTF::create(buffer, "Arial", 21);
        l2->setAnchorPoint(ccp(0.5, 0));
        l2->setPosition(ccp(322, 212));
        if (remainTime == 0) {
            sp2->setVisible(false);
            l2->setVisible(false);
        } else {
            sp2->setVisible(true);
            l2->setVisible(true);
        }
        pUpdate->addChild(l2);
        
        
        CCMenuItemImage *pFree = CCMenuItemImage::create("MC_courage.png",
                                                         "MC_courage.png",
                                                         this,
                                                         menu_selector(YiZhanShowLayer::again));
        pFree->setTag(BTN_CHOU + 1);/*抽1次*/
        pFree->setAnchorPoint(CCPointZero);
        pFree->setPosition(ccp(25, 155));
        CCMenu* pFreeMenu = CCMenu::create(pFree, NULL);
        pFreeMenu->setAnchorPoint(ccp(0, 0));
        pFreeMenu->setPosition(ccp(0, 0));
        pUpdate->addChild(pFreeMenu);
        CCSprite* l = CCSprite::create("MC_rmbSearch.png");
        l->setAnchorPoint(ccp(0, 0));
        l->setPosition(ccp(25 + (pFree->getContentSize().width - l->getContentSize().width) / 2, 157 +
                           (pFree->getContentSize().height - l->getContentSize().height) / 2));
        pUpdate->addChild(l, 2);
        
        
        CCMenuItemImage *pFreeTen = CCMenuItemImage::create("MC_rmb10.png",
                                                            "MC_rmb10.png",
                                                            this,
                                                            menu_selector(YiZhanShowLayer::again));
        pFreeTen->setTag(BTN_CHOU + 2);/*抽10次*/
        pFreeTen->setAnchorPoint(ccp(0.5, 0));
        pFreeTen->setPosition(ccp(320, 155));
        CCMenu* pFreeTenMenu = CCMenu::create(pFreeTen, NULL);
        pFreeTenMenu->setAnchorPoint(ccp(0, 0));
        pFreeTenMenu->setPosition(ccp(0, 0));
        pUpdate->addChild(pFreeTenMenu);
        CCSprite* lTen = CCSprite::create("MC_rmbSearch1.png");
        lTen->setAnchorPoint(ccp(0.5, 0));
        lTen->setPosition(ccp(295 + (pFreeTen->getContentSize().width - lTen->getContentSize().width) / 2, 157 + (pFreeTen->getContentSize().height - lTen->getContentSize().height) / 2));
        pUpdate->addChild(lTen, 2);
        
        
        CCMenuItemImage *pReturn = CCMenuItemImage::create("CM_bigBtn.png",
                                                           "CM_bigBtn.png",
                                                           this,
                                                           menu_selector(YiZhanShowLayer::returnNow));
        pReturn->setAnchorPoint(CCPointZero);
        pReturn->setScale(0.7);
        pReturn->setPosition(ccp(450, 155));
        CCMenu* pReturnMenu = CCMenu::create(pReturn, NULL);
        pReturnMenu->setAnchorPoint(ccp(0, 0));
        pReturnMenu->setPosition(ccp(0, 0));
        pUpdate->addChild(pReturnMenu);
        CCLabelTTF* lReturn = CCLabelTTF::create("返回", "Arial", 23);
        lReturn->setAnchorPoint(ccp(0, 0));
        lReturn->setPosition(ccp(420 + (pReturn->getContentSize().width - lReturn->getContentSize().width) / 2, 145 +
                                 (pReturn->getContentSize().height - lReturn->getContentSize().height) / 2));
        pUpdate->addChild(lReturn, 2);
        
        CCAssert(m_heroFromSvr, "invalid buffer");
        Json* pRootJson = Json_create(m_heroFromSvr);
        do {
            CC_BREAK_IF(!pRootJson);
            Json* pSuccessJson = Json_getItem(pRootJson, "success");
            CC_BREAK_IF(!pSuccessJson);
            if (0 == pSuccessJson->valueint) {
                Json* pMsgJson = Json_getItem(pRootJson, "msg");
                CCLog("error msg = %s", pMsgJson->valuestring);
            }
            CC_BREAK_IF(1 != pSuccessJson->valueint);
            Json* pResultJson = Json_getItem(pRootJson, "result");
            CC_BREAK_IF(!pResultJson);
            Json* pHItemJson = Json_getItem(pResultJson, "hero");
            CC_BREAK_IF(!pHItemJson);
            Json* pHeroItemJson = pHItemJson->child;
            while (pHeroItemJson) {
                if (pHeroItemJson) {
                    
                    Json* pHeroItemTagJson = Json_getItem(pHeroItemJson, "tag");
                    CC_BREAK_IF(!pHeroItemTagJson);
                    Json* pIdTagJson = Json_getItem(pHeroItemJson, "id");
                    CC_BREAK_IF(!pIdTagJson);
                    Json* pHeroItemLevelJson = Json_getItem(pHeroItemJson, "level");
                    CC_BREAK_IF(!pHeroItemLevelJson);
                    
                    HeroOfPlayerItem* p = HeroManager::getInstance()->getHeroDataByUniId(pHeroItemTagJson->valuestring);
                    CardBox* card = CardBox::create();
                    card->setCard(3, p);
                    card->setAnchorPoint(ccp(0, 0));
                    card->setPosition(ccp(205, 500));
                    pUpdate->addChild(card);
                }
                pHeroItemJson = pHeroItemJson->next;
            }
        }while(0);
        if (pRootJson) {
            Json_dispose(pRootJson);
            pRootJson = NULL;
        }
    } else if (NFREETEN_CHOU == mode) {
        
        /*显示剩余次数*/
        CCSprite* sp = CCSprite::create("MC_remainTimeBg.png");
        sp->setAnchorPoint(ccp(0, 0));
        sp->setPosition(ccp(86, 195));
        pUpdate->addChild(sp);
        CCSprite* sp2 = CCSprite::create("MC_remainTimeBg.png");
        sp2->setAnchorPoint(ccp(0, 0));
        sp2->setPosition(ccp(285, 195));
        pUpdate->addChild(sp2);
        int remainTime = 0;
        remainTime = m_pGamePlayer->getGold() / m_pGamePlayer->getLuckRMB();
        sprintf(buffer, "%d", remainTime);
        CCLabelTTF* l1 = CCLabelTTF::create(buffer, "Arial", 21);
        l1->setAnchorPoint(ccp(0.5, 0));
        l1->setPosition(ccp(122, 212));
        if (remainTime == 0) {
            sp->setVisible(false);
            l1->setVisible(false);
        } else {
            sp->setVisible(true);
            l1->setVisible(true);
        }
        pUpdate->addChild(l1);
        remainTime = m_pGamePlayer->getGold() / m_pGamePlayer->getLuckRMBBig();
        sprintf(buffer, "%d", remainTime);
        CCLabelTTF* l2 = CCLabelTTF::create(buffer, "Arial", 21);
        l2->setAnchorPoint(ccp(0.5, 0));
        l2->setPosition(ccp(322, 212));
        if (remainTime == 0) {
            sp2->setVisible(false);
            l2->setVisible(false);
        } else {
            sp2->setVisible(true);
            l2->setVisible(true);
        }
        pUpdate->addChild(l2);
        
        CCMenuItemImage *pFree = CCMenuItemImage::create("MC_courage.png",
                                                         "MC_courage.png",
                                                         this,
                                                         menu_selector(YiZhanShowLayer::again));
        pFree->setTag(BTN_CHOU + 1);/*抽1次*/
        pFree->setAnchorPoint(CCPointZero);
        pFree->setPosition(ccp(25, 155));
        CCMenu* pFreeMenu = CCMenu::create(pFree, NULL);
        pFreeMenu->setAnchorPoint(ccp(0, 0));
        pFreeMenu->setPosition(ccp(0, 0));
        pUpdate->addChild(pFreeMenu);
        CCSprite* l = CCSprite::create("MC_rmbSearch.png");
        l->setAnchorPoint(ccp(0, 0));
        l->setPosition(ccp(25 + (pFree->getContentSize().width - l->getContentSize().width) / 2, 157 +
                           (pFree->getContentSize().height - l->getContentSize().height) / 2));
        pUpdate->addChild(l, 2);
        
        CCMenuItemImage *pFreeTen = CCMenuItemImage::create("MC_rmb10.png",
                                                            "MC_rmb10.png",
                                                            this,
                                                            menu_selector(YiZhanShowLayer::again));
        pFreeTen->setTag(BTN_CHOU + 2);/*抽10次*/
        pFreeTen->setAnchorPoint(ccp(0.5, 0));
        pFreeTen->setPosition(ccp(320, 155));
        CCMenu* pFreeTenMenu = CCMenu::create(pFreeTen, NULL);
        pFreeTenMenu->setAnchorPoint(ccp(0, 0));
        pFreeTenMenu->setPosition(ccp(0, 0));
        pUpdate->addChild(pFreeTenMenu);
        CCSprite* lTen = CCSprite::create("MC_rmbSearch1.png");
        lTen->setAnchorPoint(ccp(0.5, 0));
        lTen->setPosition(ccp(295 + (pFreeTen->getContentSize().width - lTen->getContentSize().width) / 2, 157 + (pFreeTen->getContentSize().height - lTen->getContentSize().height) / 2));
        pUpdate->addChild(lTen, 2);
        
        CCMenuItemImage *pReturn = CCMenuItemImage::create("CM_bigBtn.png",
                                                           "CM_bigBtn.png",
                                                           this,
                                                           menu_selector(YiZhanShowLayer::returnNow));
        pReturn->setAnchorPoint(CCPointZero);
        pReturn->setScale(0.7);
        pReturn->setPosition(ccp(450, 155));
        CCMenu* pReturnMenu = CCMenu::create(pReturn, NULL);
        pReturnMenu->setAnchorPoint(ccp(0, 0));
        pReturnMenu->setPosition(ccp(0, 0));
        pUpdate->addChild(pReturnMenu);
        CCLabelTTF* lReturn = CCLabelTTF::create("返回", "Arial", 23);
        lReturn->setAnchorPoint(ccp(0, 0));
        lReturn->setPosition(ccp(420 + (pReturn->getContentSize().width - lReturn->getContentSize().width) / 2, 145 +
                                 (pReturn->getContentSize().height - lReturn->getContentSize().height) / 2));
        pUpdate->addChild(lReturn, 2);
        
        CCAssert(m_heroFromSvr, "invalid buffer");
        int index = 0;
        Json* pRootJson = Json_create(m_heroFromSvr);
        do {
            CC_BREAK_IF(!pRootJson);
            Json* pSuccessJson = Json_getItem(pRootJson, "success");
            CC_BREAK_IF(!pSuccessJson);
            if (0 == pSuccessJson->valueint) {
                Json* pMsgJson = Json_getItem(pRootJson, "msg");
                CCLog("error msg = %s", pMsgJson->valuestring);
            }
            CC_BREAK_IF(1 != pSuccessJson->valueint);
            Json* pResultJson = Json_getItem(pRootJson, "result");
            Json* pHItemJson = Json_getItem(pResultJson, "hero");
            Json* pHeroItemJson = pHItemJson->child;
            while (pHeroItemJson) {
                int i, j;
                i = index % 5;
                j = (index - i) / 5;
                Json* pHeroItemTagJson = Json_getItem(pHeroItemJson, "tag");
                
                HeroOfPlayerItem* pHeroOfPlayerItem =
                    HeroManager::getInstance()->getHeroDataByUniId(pHeroItemTagJson->valuestring);
                CCAssert(pHeroOfPlayerItem, "invalid data");
                
                CardBox* card = CardBox::create();
                card->setCard(2, pHeroOfPlayerItem);
                card->setScale(0.8);
                card->setAnchorPoint(ccp(0, 0));
                card->setPosition(ccp(35 + i * 115, j * 175 + 470));
                pUpdate->addChild(card);
                
                pHeroItemJson = pHeroItemJson->next;
                index++;
            }
            
        } while(0);
        if (pRootJson) {
            Json_dispose(pRootJson);
            pRootJson = NULL;
        }
    }
}

//
void YiZhanShowLayer::again(CCObject* pSender) {
    CCMenuItemImage* pItemImage = (CCMenuItemImage*)pSender;
    
    NetConnection* pNetConnection = NetConnection::getInstance();
    char szPostBuffer[MAX_POSTTO_SERVER_BUFFER_SIZE];
    memset(szPostBuffer, '\0', MAX_POSTTO_SERVER_BUFFER_SIZE);
    
    strcpy(szPostBuffer, "c=user&a=luckyHero&type=");
    switch (pItemImage->getTag() - BTN_CHOU) {
    case 0:
        {
            // 免费抽
            char szHttpTag[] = HTTP_REQUEST_YIFREE_TAG;
            strcat(szPostBuffer, "f10&uid=");
            strcat(szPostBuffer, m_pGamePlayer->getUid().c_str());
            pNetConnection->commitPostRequestData(szPostBuffer, szHttpTag);
            CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_SHOWYIZHAN_STRING);
            CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_LOADING_MAINSCENE);
        }
        break;
    case 1:
        {
            // 付费抽
            char szHttpTag[] = HTTP_REQUEST_YINFREE_TAG;
            strcat(szPostBuffer, "r1&uid=");
            strcat(szPostBuffer, m_pGamePlayer->getUid().c_str());
            pNetConnection->commitPostRequestData(szPostBuffer, szHttpTag);
            CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_SHOWYIZHAN_STRING);
            CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_LOADING_MAINSCENE);
        }
        break;
    case 2:
        {
            // 付费十连抽
            char szHttpTag[] = HTTP_REQUEST_YINFREETEN_TAG;
            strcat(szPostBuffer, "r10&uid=");
            strcat(szPostBuffer, m_pGamePlayer->getUid().c_str());
            pNetConnection->commitPostRequestData(szPostBuffer, szHttpTag);
            CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_SHOWYIZHAN_STRING);
            CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_LOADING_MAINSCENE);
        }
        break;
    default:
        break;
    }
}

//
void YiZhanShowLayer::restoreTouchUICompent() {
    /*恢复可见*/
    this->setVisible(true);
    
    CCNode* pParent = (CCNode*)this->getChildByTag(NODE_TAG);
    CCSprite* spBg = (CCSprite*)pParent->getChildByTag(BGOFYIZHAN);
    
    if (spBg->getActionByTag(MOVEOUT_TAG)) {
        spBg->stopAllActions();
        spBg->setOpacity(0);
        CCFadeIn* pMoveTo = CCFadeIn::create(ANIMATION_DURING);
        pMoveTo->setTag(MOVEIN_TAG);
        spBg->runAction(pMoveTo);
        return;
    }
    
    if (!spBg->getActionByTag(MOVEIN_TAG)) {
        CCFadeIn* pFadeto = CCFadeIn::create(ANIMATION_DURING);
        pFadeto->setTag(MOVEIN_TAG);
        spBg->runAction(pFadeto);
    }

}

//
void YiZhanShowLayer::moveOutTouchUICompent() {
    CCNode* pParent = (CCNode*)this->getChildByTag(NODE_TAG);
    CCSprite* spBg = (CCSprite*)pParent->getChildByTag(BGOFYIZHAN);
    
    if (spBg->getActionByTag(MOVEIN_TAG)) {
        spBg->stopAllActions();
        spBg->setOpacity(255);
        CCSequence* pMoveOUtAction = CCSequence::create(CCFadeOut::create(ANIMATION_DURING), CCCallFunc::create(this, callfunc_selector(YiZhanShowLayer::moveOutCallback)), NULL);
        pMoveOUtAction->setTag(MOVEOUT_TAG);
        spBg->runAction(pMoveOUtAction);
        return;
    }
    if (!spBg->getActionByTag(MOVEOUT_TAG)) {
        CCSequence* pMoveOUtAction = CCSequence::create(CCFadeOut::create(ANIMATION_DURING), CCCallFunc::create(this, callfunc_selector(YiZhanShowLayer::moveOutCallback)), NULL);
        pMoveOUtAction->setTag(MOVEOUT_TAG);
        spBg->runAction(pMoveOUtAction);
    }
}

void YiZhanShowLayer::moveOutCallback() {
    if (m_isRemoveSelf) {
        this->removeFromParent();
//        CCLog("YiZhanShowLayer 从主场景移除");
    }
}

