//
//  ChosConfirm.cpp
//  hero
//
// 确认框
// 隶属于主场景
//

#include "ChosConfirm.h"
#include "customMessage.h"
#include "GameState.h"

// 主菜单大图文件
#define MAIN_MENU_IMAGE_FILE        "mainMenuIcon.pvr.ccz"
#define MAIN_MENU_LIST_FILE         "mainMenuIcon.plist"

#define OPENNING_ANI_DURING					0.3

#define MYH_IMAGE_OFFSET                    -10.0f		// “我的武将” 偏移量
#define RS_IMAGE_OFFSET                     -10.0f		// 返回 出售 按钮偏移量

#define SELECT_GOBATTLE                     116
#define SELECT_SELL                         112
#define SELECT_TUN                          118

#define CONTAINER_TAG 						1300
#define LL_MENU_TAG 						1301		//tipMenu左边容器tag值
#define MID_MENU_TAG 						1302		//..
#define RR_MENU_TAG 						1303		//..
#define LABEL_COUNT_TAG                     1304        // 选择的武将数量
#define LABEL_VALUE_TAG                     1305
#define ICON_COUNT_TAG                      1306
#define ICON_VALUE_TAG                      1307

#define MOVEOUT_ACTION_TAG                  1400
#define RESTORE_ACTION_TAG                  1401

#define NODE_TAG                            1402

/*
 *  构造函数
 */
ChosConfirm::ChosConfirm():
m_pGameState(GameState::getInstance()) {}

/*
 *  析构函数
 */
ChosConfirm::~ChosConfirm() {
}

/*
 *  初始化
 */
bool ChosConfirm::init() {
    // super init
    if (!CCLayer::init()) {
        return false;
    }

    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(MAIN_MENU_LIST_FILE, MAIN_MENU_IMAGE_FILE);
    
    //添加容器,所有随层同步移入主场景的内容全部加入容器中
    CCNode* pContainer = CCNode::create();
    pContainer->setAnchorPoint(CCPointZero);
    pContainer->setPosition(ccp(0, m_pGameState->getBottomOffset()));
    this->addChild(pContainer, 1, CONTAINER_TAG);

    //选择确认的背景
    CCSprite* pBg = CCSprite::createWithSpriteFrameName("menuIconlayerBackground.png");
    pBg->setAnchorPoint(ccp(0, 0));
    pBg->setPosition(ccp(0, -26));
    pContainer->addChild(pBg);
    
    //确认按钮
    CCMenuItemImage *pConfirm = CCMenuItemImage::create("CM_litBtn.png",
                                                        "CM_litBtn.png",
                                                        this,
                                                        menu_selector(ChosConfirm::chosConfirmOk));
    pConfirm->setAnchorPoint(CCPointZero);
    pConfirm->setPosition(ccp(500, 32));
    CCMenu* pMenu = CCMenu::create(pConfirm, NULL);
    pMenu->setAnchorPoint(ccp(0, 0));
    pMenu->setPosition(ccp(0, 0));
    pContainer->addChild(pMenu, 2);
    CCSprite* confirmFont = CCSprite::create("CM_confirm.png");
    confirmFont->setAnchorPoint(ccp(0, 0));
    confirmFont->setPosition(ccp(500 + (pConfirm->getContentSize().width - confirmFont->getContentSize().width) / 2, 32+
                                 (pConfirm->getContentSize().height - confirmFont->getContentSize().height) / 2));
    
    pContainer->addChild(confirmFont, 2);
    
    /* 数字背景图 */
    CCSprite* spCount = CCSprite::create("CM_BG1.png");
    spCount->setAnchorPoint(ccp(0, 0));
    spCount->setPosition(ccp(120, 40));
    pContainer->addChild(spCount);
    CCSprite* spValue = CCSprite::create("CM_BG2.png");
    spValue->setAnchorPoint(ccp(0.5, 0));
    spValue->setPosition(ccp(380, 40));
    pContainer->addChild(spValue);
    
    CCLabelTTF* l = CCLabelTTF::create("已选择数量", "Arial", 21);
    l->setAnchorPoint(ccp(0, 0));
    l->setPosition(ccp(10, 40));
    pContainer->addChild(l,3, ICON_COUNT_TAG);
    /*所选武将的数量*/
    CCLabelTTF* lCount = CCLabelTTF::create("0", "Arial", 23);
    lCount->setAnchorPoint(ccp(0.5, 0));
    lCount->setPosition(ccp(180, 45));
    pContainer->addChild(lCount,3,LABEL_COUNT_TAG);
    
    
    
    
    
    /*所选武将相应属性值*/
    CCLabelTTF* value = CCLabelTTF::create("0", "Arial", 23);
    value->setAnchorPoint(ccp(0.5, 0));
    value->setPosition(ccp(380, 45));
    pContainer->addChild(value,3,LABEL_VALUE_TAG);
    
    return true;
}

void ChosConfirm::updateUi(int count, int value) {
    CCNode* pParent = this->getChildByTag(CONTAINER_TAG);
    if (pParent->getChildByTag(NODE_TAG)) {
        pParent->removeChildByTag(NODE_TAG);
    }
    CCNode* pNode = CCNode::create();
    pParent->addChild(pNode, 1, NODE_TAG);
    
    WhoCallHeroList whoCallHeroList  = m_pGameState->getCurrTagWhoCallHeroSel();
    if (WCH_TEAMER == whoCallHeroList) {
        CCLabelTTF* lCount = (CCLabelTTF*)this->getChildByTag(CONTAINER_TAG)->getChildByTag(LABEL_COUNT_TAG);
        char buffer[16];
        sprintf(buffer, "%d", count);
        lCount->setString(buffer);
        CCLabelTTF* v = (CCLabelTTF*)this->getChildByTag(CONTAINER_TAG)->getChildByTag(LABEL_VALUE_TAG);
        sprintf(buffer, "%d",value);
        v->setString(buffer);
        
        CCSprite* icon = CCSprite::create("atticon.png");
        icon->setAnchorPoint(ccp(0, 0));
        icon->setPosition(ccp(260, 47));
        pNode->addChild(icon, 3);
    } else if(WCH_SALE == whoCallHeroList) {
        char buffer[16];
        CCLabelTTF* lCount = (CCLabelTTF*)this->getChildByTag(CONTAINER_TAG)->getChildByTag(LABEL_COUNT_TAG);
        sprintf(buffer, "%d", count);
        lCount->setString(buffer);
        CCLabelTTF* v = (CCLabelTTF*)this->getChildByTag(CONTAINER_TAG)->getChildByTag(LABEL_VALUE_TAG);
        sprintf(buffer, "%d",value);
        v->setString(buffer);
        
        CCSprite* icon = CCSprite::create("copper.png");
        icon->setAnchorPoint(ccp(0, 0));
        icon->setPosition(ccp(260, 42));
        pNode->addChild(icon, 3);
    } else if (WCH_FORFOOD == whoCallHeroList) {
        CCLabelTTF* lCount = (CCLabelTTF*)this->getChildByTag(CONTAINER_TAG)->getChildByTag(LABEL_COUNT_TAG);
        char buffer[16];
        sprintf(buffer, "%d", count);
        lCount->setString(buffer);
        CCLabelTTF* v = (CCLabelTTF*)this->getChildByTag(CONTAINER_TAG)->getChildByTag(LABEL_VALUE_TAG);
        sprintf(buffer, "%d",value);
        v->setString(buffer);
        
        CCSprite* icon = CCSprite::create("label_exp.png");
        icon->setAnchorPoint(ccp(0, 0));
        icon->setPosition(ccp(260, 45));
        pNode->addChild(icon, 3);
    }
}

void ChosConfirm::chosConfirmOk() {
    CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_CHOSCONFIRM_OK_STRING);
}

/*
 *  进场动画
 */
void ChosConfirm::openningAni() {
    CCNode* pNode = (CCNode*)this->getChildByTag(CONTAINER_TAG);
    pNode->runAction(CCEaseBackOut::create(CCMoveTo::create(OPENNING_ANI_DURING, ccp(0, GameState::getInstance()->getBottomOffset()))));
}

void ChosConfirm::moveOutTouchUICompent() {
    CCNode* pNode = (CCNode*)this->getChildByTag(CONTAINER_TAG);
    pNode->setPosition(ccp(0.0f, GameState::getInstance()->getBottomOffset() - 130.0f));
    this->setVisible(false);
}

void ChosConfirm::restoreTouchUICompent() {
    /*恢复可见*/
    this->setVisible(true);
    
    CCNode* pNode = (CCNode*)this->getChildByTag(CONTAINER_TAG);
    pNode->setPosition(ccp(0, GameState::getInstance()->getBottomOffset()));
}

void ChosConfirm::initPosition() {
    CCNode* pNode = (CCNode*)this->getChildByTag(CONTAINER_TAG);
    pNode->setPosition(ccp(0, GameState::getInstance()->getBottomOffset() - 130.0f));
}

void ChosConfirm::moveOutCallback() {
    this->setVisible(false);
}