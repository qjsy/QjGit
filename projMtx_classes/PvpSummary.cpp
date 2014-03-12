//
//  PvpSummary.cpp
//  hero
//
//  Created by 钱剑 on 14-2-24.
//
//

#include "MainScene.h"
#include "customMessage.h"
#include "PvpSummary.h"
#include "GameState.h"
#include "CardBox.h"
#include "HeroItem.h"
#include "MosterInBattleItem.h"
#include "MosterItem.h"
#include "HeroManager.h"
#include "HeroOfPlayerItem.h"
#include "Battle.h"


// 上下款大图
#define MAIN_MENU_IMAGE_FILE        "mainMenuIcon.pvr.ccz"
#define MAIN_MENU_LIST_FILE         "mainMenuIcon.plist"
#define IMAGE_TOP                   "top.png"
#define IMAGE_BOTTOM                "bottom.png"
#define HEI_COLOR           ccc3(0, 0, 0)
//tag
#define CONTAINER           100                     // 层容器
#define COPPER_ANI          101                     // 铜币动画
//

/*构造函数*/
PvpSummary::PvpSummary()
:m_touchEnable(false)
,m_cop(0)
{}
/*析构函数*/
PvpSummary::~PvpSummary(){}
bool PvpSummary::init() {
    if (!CCLayer::init()) {
        return false;
    }
    
    /* 上下款 */
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(MAIN_MENU_LIST_FILE, MAIN_MENU_IMAGE_FILE);
    CCSpriteBatchNode* m_pMainMenuCBN = CCSpriteBatchNode::create(MAIN_MENU_IMAGE_FILE);
    this->addChild(m_pMainMenuCBN, 1);
    
    if (GameState::getInstance()->is1136()) /* 1136 分辨率 */ {
        // top 图片
        CCSprite *pTopSprite = CCSprite::createWithSpriteFrameName(IMAGE_TOP);
        pTopSprite->setAnchorPoint(CCPointZero);
        pTopSprite->setPosition(ccp(0.0f, CCDirector::sharedDirector()->getVisibleSize().height - pTopSprite->getContentSize().height));
        m_pMainMenuCBN->addChild(pTopSprite);
        
        // bottom 图片
        CCSprite *pBottomSprite = CCSprite::createWithSpriteFrameName(IMAGE_BOTTOM);
        pBottomSprite->setAnchorPoint(CCPointZero);
        pBottomSprite->setPosition(CCPointZero);
        m_pMainMenuCBN->addChild(pBottomSprite);
    }
    
    CCNode* pContainer = CCNode::create();
    pContainer->setPosition(ccp(0, GameState::getInstance()->getBottomOffset()));
    addChild(pContainer, 1, CONTAINER);
    
    // 结算背景按钮,更新到友情结算
    CCMenuItemImage *pNext = CCMenuItemImage::create( "HL_sellBg.png",
                                                     "HL_sellBg.png",
                                                     this,
                                                     menu_selector(PvpSummary::_click));
    pNext->setAnchorPoint(ccp(0, 0));
    pNext->setPosition(ccp(0, -25));
    CCMenu* menu = CCMenu::create(pNext, NULL);
    menu->setAnchorPoint(ccp(0, 0));
    menu->setPosition(ccp(0, 0));
    pContainer->addChild(menu);
    
    /* 显示标题 */
    CCSprite* spfontBg = CCSprite::create("SM_fontbg.png");
    spfontBg->setAnchorPoint(ccp(0.5, 0));
    spfontBg->setPosition(ccp(320, 720.0f));
    pContainer->addChild(spfontBg, 1);
    CCSprite* spfont = CCSprite::create("SM_battle.png");
    spfont->setAnchorPoint(ccp(0, 0));
    spfont->setPosition(ccp((spfontBg->getContentSize().width - spfont->getContentSize().width) / 2, (
                             spfontBg->getContentSize().height - spfont->getContentSize().height) / 2));
    spfontBg->addChild(spfont);
    
    /* 加载敌我武将 */
    HeroManager* pHeroManager = HeroManager::getInstance();
    HeroItem* pHeroItem = pHeroManager->getHeroItem(pHeroManager->
                                                    getPvpGoIntoBattleHeroDataByIndex(0)->getHeroItem()->getId());
    CardBox* pMyself = CardBox::create();
    pMyself->cardForTOngQue(pHeroItem);
    pMyself->setPosition(ccp(135.0f, 520.0f));
    pContainer->addChild(pMyself);
    /* vs */
    CCLabelTTF* pL = CCLabelTTF::create("VS", "Arial", 30);
    pL->setAnchorPoint(ccp(0.5, 0));
    pL->setColor(ccc3(255, 0, 0));
    pL->setPosition(ccp(315.0f, 580.0f));
    pContainer->addChild(pL);
    
    MosterItem* pMosterItem = NULL;
    HeroItem* pHeroItemEnemy = NULL;
    Battle* pBattle = Battle::getInstance();
    InBattleItem* pInBattleItem = pBattle->getPvpEnemyCaptain();
    CardBox* pEnemy  = CardBox::create();
    if (pInBattleItem->isMonster())/*怪物*/ {
        MosterInBattleItem* pMosterInBattleItem = (MosterInBattleItem*)pInBattleItem;
        pMosterItem = pMosterInBattleItem->getMosterItem();
        pEnemy->cardForTongQue(pMosterItem);
    } else {
        HeroOfPlayerItem* pHeroOfPlayerItem = (HeroOfPlayerItem*)pInBattleItem;
        pHeroItemEnemy = pHeroOfPlayerItem->getHeroItem();
        pEnemy->cardForTOngQue(pHeroItemEnemy);
    }
    pEnemy->setPosition(ccp(360.0f, 520.0f));
    pContainer->addChild(pEnemy);
    pBattle->clearPvpEnemyCaptain();
    
    /* 铜币显示 */
    CCSprite* labelCop = CCSprite::create("SM_copper.png");
    labelCop->setAnchorPoint(ccp(0, 0));
    labelCop->setPosition(ccp(80.0f, 390.0f));
    pContainer->addChild(labelCop, 2);
    pCop = CCLabelTTF::create("", "Arial", 27);
    pCop->setAnchorPoint(ccp(0, 0));
    pCop->setPosition(ccp(230.0f, 390.0f));
    pCop->setColor(HEI_COLOR);
    pContainer->addChild(pCop, 2, COPPER_ANI);

    
    
    _copAni();
    return true;
}



void PvpSummary::_click() {
    if (!m_touchEnable) {
        return;
    }
    CCLog("可以点击");
    if (Battle::getInstance()->isCWar()) {
        GameState::getInstance()->setTagWhoCallMainScene(4);/*设置战斗调用返回主场景*/
    } else {
        GameState::getInstance()->setTagWhoCallMainScene(3);/*设置战斗调用返回主场景*/
    }
    
    CCDirector::sharedDirector()->replaceScene(MainScene::create());
    this->removeFromParent();
}

void PvpSummary::_copAni() {
    schedule(schedule_selector(PvpSummary::_scheduleForCop), 0.001);
}

void PvpSummary::_scheduleForCop() {
    char buffer[16];
    int getCop = 0;
    if (Battle::getInstance()->isCWar()) {
        getCop = Battle::getInstance()->getCwarAddMoney();
    } else {
        getCop = Battle::getInstance()->getTqtAddMoney();
    }
    
    if (m_cop < getCop) {
        m_cop += getCop / 30;
        sprintf(buffer, "%d", m_cop);
        pCop->setString(buffer);
    } else {
        sprintf(buffer, "%d", getCop);
        pCop->setString(buffer);
        m_touchEnable = true;/*此界面可以点击*/
        unschedule(schedule_selector(PvpSummary::_scheduleForCop));
    }
}