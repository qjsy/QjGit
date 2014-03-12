//
//  GoBattleLayer.cpp
//  hero
//
// 上阵武将列表层（主界面首页）
// 隶属于主场景
//
//
//

#include "GoBattleLayer.h"
#include "CardBox.h"
#include "HeroManager.h"
#include "HeroItem.h"
#include "HeroOfPlayerItem.h"
#include "customMessage.h"
#include "GameState.h"
#include "Battle.h"
#include "common.h"

#define OPENNING_ANI_DURING 0.2

#define FIGHT_VALUE_TAG     1501                //战斗力 tag

#define BTN_OFSSET          110
#define BOX_OFSSET          110

#define ANIMATION_DURING    0.2

#define MOVEOUT_TAG         5100
#define MOVEIN_TAG          5101

#define JIAO1_TAG           10
#define JIAO2_TAG           11
#define JIAO3_TAG           12
#define JIAO4_TAG           13
#define RABOX_TAG           14
#define HERO_HEAD_TAG       15

#define GO_BATTLE_HERO_IMAGE_FILE       "goBattleHeros.pvr.ccz"             // 头像卡大图
#define GO_BATTLE_HERO_LIST_FILE        "goBattleHeros.plist"               // list 文件

/*
 *  构造函数
 */
GoBattleLayer::GoBattleLayer():m_isInScreen(true),m_pHeroManager(HeroManager::getInstance()),
m_pGameState(GameState::getInstance()),m_pBattle(Battle::getInstance()),
m_pSpriteFrameCache(CCSpriteFrameCache::sharedSpriteFrameCache()),
m_pSBN(NULL),m_pBgSprite(NULL),m_touchBlank(0)
{
    for (unsigned int i = 0; i < 5; i++) {
        m_heroSprite[i] = m_heroBgSprite[i] = NULL;
    } /*for*/
}

/*
 *  析构函数
 */
GoBattleLayer::~GoBattleLayer() {
    m_pSpriteFrameCache->removeSpriteFramesFromFile(GO_BATTLE_HERO_LIST_FILE);
    CCTextureCache::sharedTextureCache()->removeTextureForKey(GO_BATTLE_HERO_IMAGE_FILE);
}

/*
 *  初始化
 */
bool GoBattleLayer::init() {
    if (!CCLayer::init()) {
        return false;
    }
    this->setTouchEnabled(true);
    
    this->addChild(m_pSBN = CCSpriteBatchNode::create(GO_BATTLE_HERO_IMAGE_FILE), 1);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(GO_BATTLE_HERO_LIST_FILE, GO_BATTLE_HERO_IMAGE_FILE);

    // 背景图片
    m_pBgSprite = CCSprite::createWithSpriteFrameName("gobattle_M_2.png");
    m_pBgSprite->setAnchorPoint(CCPointZero);
    m_pBgSprite->setPosition(ccp(-3.0f, m_pGameState->getBottomOffset() + 615.0f));
    m_pSBN->addChild(m_pBgSprite, 1);
    
    /*
     *  初始显示所有 pve 上阵武将
     */
    m_heroSprite[0] = _getHeroSpriteByHeroItem(m_pHeroManager->getGoIntoBattleHeroDataByIndex(0));
    m_heroSprite[0]->setScale(1.3f);
    m_heroSprite[0]->setPosition(ccp(37.0f, 35.0f));
    m_pBgSprite->addChild(m_heroSprite[0], 5);
    
    // 队长标志
    CCSprite* flag = CCSprite::createWithSpriteFrameName("gobattle_M_flag.png");
    flag->setAnchorPoint(CCPointZero);
    flag->setPosition(ccp(33.0f, 125.0f));
    m_pBgSprite->addChild(flag, 11);
    
    unsigned int countOfPveGotoBattleHeros = m_pHeroManager->getGoIntoBattleHerosOfPlayerCount();
    for (unsigned int i = 0; i < countOfPveGotoBattleHeros - 1; i++) {
        HeroOfPlayerItem* pItem = m_pHeroManager->getGoIntoBattleHeroDataByIndex(i + 1);
        if (pItem) {
            m_heroSprite[i + 1] = _getHeroSpriteByHeroItem(pItem);
            m_heroSprite[i + 1]->setPosition(ccp(176.0f + i * BOX_OFSSET, 65.0f));
            m_pBgSprite->addChild(m_heroSprite[i + 1], 5);
        } else {
            m_heroSprite[i + 1] = NULL;
        }
    } /*for*/
    
    CCSpriteFrame* pHeroBlankSpriteFrame = m_pSpriteFrameCache->spriteFrameByName("gobattle_hero_blank.png");
    m_heroBgSprite[0] = CCSprite::createWithSpriteFrame(pHeroBlankSpriteFrame);
    m_heroBgSprite[0]->setAnchorPoint(CCPointZero);
    m_heroBgSprite[0]->setPosition(ccp(33.0f, 30.0f));
    m_heroBgSprite[0]->setScale(1.1);
    m_pBgSprite->addChild(m_heroBgSprite[0], 4);
    for (unsigned int i = 1; i < MAX_GO_INTO_BATTLE_HEROS; i++) {
        m_heroBgSprite[i] = CCSprite::createWithSpriteFrame(pHeroBlankSpriteFrame);
        m_heroBgSprite[i]->setScale(0.85);
        m_heroBgSprite[i]->setAnchorPoint(CCPointZero);
        m_heroBgSprite[i]->setPosition(ccp(63.0f + BTN_OFSSET * i, 62.0f));
        m_pBgSprite->addChild(m_heroBgSprite[i], 4);
    } /*for*/
    
    // 显示战斗力
    CCSprite* fight_box = CCSprite::createWithSpriteFrameName("gobattle_M_B_fight.png");
    fight_box->setAnchorPoint(CCPointZero);
    fight_box->setPosition(ccp(290.0f, 32.0f));
    m_pBgSprite->addChild(fight_box, 10);
    
    CCSprite* label_fight = CCSprite::createWithSpriteFrameName("gobattle_M_B_fightvalue.png");
    label_fight->setAnchorPoint(CCPointZero);
    label_fight->setPosition(ccp(5.0f, 4.0f));
    fight_box->addChild(label_fight, 1);
    
//    CCLabelTTF* fight_value = CCLabelTTF::create("99999", "Arial", 20);
//    fight_value->setAnchorPoint(ccp(0, 0));
//    fight_value->setPosition(ccp(83, 3));
//    fight_box->addChild(fight_value, 1, FIGHT_VALUE_TAG);
    
    return true;
}

//
void GoBattleLayer::setVisible(bool visible) {
    CCLayer::setVisible(visible);
    if (visible)
        updateHeroUi();
}

//
void GoBattleLayer::registerWithTouchDispatcher(void) {
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, false);
}

//
bool GoBattleLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    CCPoint touchPoint = m_pBgSprite->convertTouchToNodeSpace(pTouch);
    m_touchBlank = 0;
    for (unsigned int i = 0; i < MAX_GO_INTO_BATTLE_HEROS; i++) {
        if (m_heroBgSprite[i]->boundingBox().containsPoint(touchPoint)) {
            m_touchBlank = i + 1;
            return true;
        }
    } /*for*/
    return false;
}

//
void GoBattleLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {
    
}

//
void GoBattleLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {
    CCPoint touchPoint = m_pBgSprite->convertTouchToNodeSpace(pTouch);
    for (unsigned int i = 0; i < MAX_GO_INTO_BATTLE_HEROS; i++) {
        if (m_heroBgSprite[i]->boundingBox().containsPoint(touchPoint) && i + 1 == m_touchBlank) {
            if (1 == m_touchBlank) {
                _selectCaptain();       // 选择队长
            } else {
                _selectTeamer();        // 选择队员
            }
            break;
        }
    } /*for*/
}

//
void GoBattleLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {
    m_touchBlank = 0;
}

void GoBattleLayer::moveOutTouchUICompent() {
    if (m_pBgSprite->getActionByTag(MOVEIN_TAG)) {
        m_pBgSprite->stopAllActions();
        m_pBgSprite->setPosition(ccp(0.0, 615.0f + m_pGameState->getBottomOffset()));
        CCSequence* pMoveOUtAction = CCSequence::create(CCMoveTo::create(ANIMATION_DURING, ccp(0, 975.0f + m_pGameState->getBottomOffset())),
                CCCallFunc::create(this, callfunc_selector(GoBattleLayer::moveOutCallback)), NULL);
        pMoveOUtAction->setTag(MOVEOUT_TAG);
        m_pBgSprite->runAction(pMoveOUtAction);
        return;
    }
    if (!m_pBgSprite->getActionByTag(MOVEOUT_TAG)) {
        CCSequence* pMoveOUtAction = CCSequence::create(CCMoveTo::create(ANIMATION_DURING, ccp(0, 975.0f + m_pGameState->getBottomOffset())), CCCallFunc::create(this, callfunc_selector(GoBattleLayer::moveOutCallback)), NULL);
        pMoveOUtAction->setTag(MOVEOUT_TAG);
        m_pBgSprite->runAction(pMoveOUtAction);
    }
    
    m_isInScreen = false;
}

void GoBattleLayer::moveOutCallback() {
    this->setVisible(false);
}

void GoBattleLayer::initPosition() {
    m_pBgSprite->setPosition(ccp(0.0f, 615.0f + m_pGameState->getBottomOffset()));
}

void GoBattleLayer::restoreTouchUICompent() {
    /*恢复可见*/
    this->setVisible(true);
    
    if (m_pBgSprite->getActionByTag(MOVEOUT_TAG)) {
        m_pBgSprite->stopAllActions();
        m_pBgSprite->setPosition(ccp(0, 975.0f + m_pGameState->getBottomOffset()));
        CCMoveTo* pMoveTo = CCMoveTo::create(ANIMATION_DURING, ccp(0.0f, 615.0f + m_pGameState->getBottomOffset()));
        pMoveTo->setTag(MOVEIN_TAG);
        m_pBgSprite->runAction(pMoveTo);
        return;
    }
    
    if (!m_pBgSprite->getActionByTag(MOVEIN_TAG)) {
        CCMoveTo* pMoveTo = CCMoveTo::create(ANIMATION_DURING, ccp(0.0f, 615.0f + m_pGameState->getBottomOffset()));
        pMoveTo->setTag(MOVEIN_TAG);
        m_pBgSprite->runAction(pMoveTo);
    }
    
    /* 将谁调用武将列表标志置空,不置空会出错 */
    m_pGameState->setTagWhoCallHeroSel(WCH_NONE);
    m_isInScreen = true;
}

/*
 *  选择队员
 */
void GoBattleLayer::_selectTeamer() {
    if (m_pBattle->getBattlePkMode() == BPM_PVE) {
        m_pGameState->setCurrTagWhoCallHeroSel(WCH_TEAMER);
        CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_HEROS_STRING);
    } else {
        m_pGameState->setCurrTagWhoCallHeroSel(WCH_TEAMER_PVP);
        CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_HEROS_PVP_STRING);
    }
}

/*
 *  选择队长
 */
void GoBattleLayer::_selectCaptain() {
    if (m_pBattle->getBattlePkMode() == BPM_PVE) {
        m_pGameState->setCurrTagWhoCallHeroSel(WCH_CAPTAIN);
        CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_HEROS_STRING);
    } else {
        m_pGameState->setCurrTagWhoCallHeroSel(WCH_CAPTAIN_PVP);
        CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_HEROS_PVP_STRING);
    }
}

/*
 *  更新上阵武将 ui 显示
 */
void GoBattleLayer::updateHeroUi() {
    unsigned int countOfPveGotoBattleHeros = 0;
    if (m_pBattle->getBattlePkMode() == BPM_PVE) {
        countOfPveGotoBattleHeros = m_pHeroManager->getGoIntoBattleHerosOfPlayerCount();
    } else {
        countOfPveGotoBattleHeros = m_pHeroManager->getPvpGoIntoBattleHerosOfPlayerCount();
    }
    
    // 队长
    HeroOfPlayerItem* pHeroOfPlayerItem = NULL;
    if (m_pBattle->getBattlePkMode() == BPM_PVE) {
        pHeroOfPlayerItem = m_pHeroManager->getGoIntoBattleHeroDataByIndex(0);
    } else {
        pHeroOfPlayerItem = m_pHeroManager->getPvpGoIntoBattleHeroDataByIndex(0);
    }
    m_heroSprite[0] = _updateHeroSpriteByHeroItem(pHeroOfPlayerItem, m_heroSprite[0]);
    
    for (unsigned int i = 0; i < MAX_GO_INTO_BATTLE_HEROS; i++) {
        if (m_pBattle->getBattlePkMode() == BPM_PVE) {
            pHeroOfPlayerItem = m_pHeroManager->getGoIntoBattleHeroDataByIndex(i + 1);
        } else {
            pHeroOfPlayerItem = m_pHeroManager->getPvpGoIntoBattleHeroDataByIndex(i + 1);
        }
        if (pHeroOfPlayerItem) {
            if (m_heroSprite[i + 1]) {
                m_heroSprite[i + 1] = _updateHeroSpriteByHeroItem(pHeroOfPlayerItem, m_heroSprite[i + 1]);
                m_heroSprite[i + 1]->setVisible(true);
            } else {
                m_heroSprite[i + 1] = _getHeroSpriteByHeroItem(pHeroOfPlayerItem);
                m_heroSprite[i + 1]->setPosition(ccp(176.0f + i * BOX_OFSSET, 65.0f));
                m_pBgSprite->addChild(m_heroSprite[i + 1], 5);
            }
        } else {
            if (m_heroSprite[i + 1]) {
                m_heroSprite[i + 1]->setVisible(false);
            }
        }
    } /*for*/
}

void GoBattleLayer::actionForPvp() {
    CCSequence* pMoveOUtAction = CCSequence::create(CCMoveTo::create(ANIMATION_DURING / 2,
                                ccp(0, 975.0f + m_pGameState->getBottomOffset())),
                                CCCallFunc::create(this, callfunc_selector(GoBattleLayer::_callbackForAction)), NULL);
    m_pBgSprite->runAction(pMoveOUtAction);
}

void GoBattleLayer::_callbackForAction() {
    CCMoveTo* pMoveTo = CCMoveTo::create(ANIMATION_DURING / 2, ccp(0.0f, 615.0f + m_pGameState->getBottomOffset()));
    pMoveTo->setTag(MOVEIN_TAG);
    m_pBgSprite->runAction(pMoveTo);

    /* 更新上阵武将列表 */
    updateHeroUi();
    
    m_isInScreen = true;
}

/*
 *  功能：
 *      根据背包武将数据生成精灵
 *  参数：
 *      pHeroOfPlayerItem   武将数据
 *  返回：
 *      对应武将数据的精灵
 */
CCSprite* GoBattleLayer::_getHeroSpriteByHeroItem(HeroOfPlayerItem* pHeroOfPlayerItem) {
    CCAssert(pHeroOfPlayerItem, "invalid heroItem");
    
    char szBuffer[64];
    std::string heroPic = pHeroOfPlayerItem->getHeroItem()->getHIcon();
    unsigned int heroRarity = pHeroOfPlayerItem->getHeroItem()->getRarity();
//    unsigned int heroLevel = pHeroOfPlayerItem->getLevel();
    
    /* 稀有度背景 */
    if (1 == heroRarity) {
        strcpy(szBuffer, "HP_whi.png");
    } else if (2 ==heroRarity || 3 == heroRarity) {
        strcpy(szBuffer, "HP_gre.png");
    } else if (4 == heroRarity || 5 == heroRarity) {
        strcpy(szBuffer, "HP_blu.png");
    } else if (6 ==heroRarity || 7 == heroRarity || 8 == heroRarity) {
        strcpy(szBuffer, "HP_pur.png");
    } else {
        strcpy(szBuffer, "HP_ora.png");
    }
    CCSprite* pRarityBgSprite = CCSprite::createWithSpriteFrameName(szBuffer);
    pRarityBgSprite->setAnchorPoint(CCPointZero);
    
    /* 武将框 */
    if (1 == heroRarity) {
        strcpy(szBuffer, "HP_whiBox.png");
    } else if (2 == heroRarity || 3 == heroRarity) {
        strcpy(szBuffer, "HP_greBox.png");
    } else if (4 == heroRarity || 5 == heroRarity) {
        strcpy(szBuffer, "HP_bluBox.png");
    } else if (6 == heroRarity || 7 == heroRarity || 8 == heroRarity) {
        strcpy(szBuffer, "HP_purBox.png");
    } else {
        strcpy(szBuffer, "HP_oraBox.png");
    }
    
    CCSprite* pRarityBoxSprite = CCSprite::createWithSpriteFrameName(szBuffer);
    pRarityBoxSprite->setAnchorPoint(CCPointZero);
    pRarityBoxSprite->setPosition(CCPointZero);
    pRarityBgSprite->addChild(pRarityBoxSprite, 8, RABOX_TAG);
    
    /* 角标显示规律 */
    if (1 == heroRarity) {
        strcpy(szBuffer, "HP_whiJiao.png");
    } else if (2 == heroRarity || 3 == heroRarity) {
        strcpy(szBuffer, "HP_greJiao.png");
    } else if (4 == heroRarity || 5 == heroRarity) {
        strcpy(szBuffer, "HP_bluJiao.png");
    } else if (6 == heroRarity || 7 == heroRarity || 8 == heroRarity) {
        strcpy(szBuffer, "HP_purJiao.png");
    } else {
        strcpy(szBuffer, "HP_oraJiao.png");
    }
    
    CCSpriteFrame* pJiaoSpriteFrame = m_pSpriteFrameCache->spriteFrameByName(szBuffer);
    CCSprite* jiao1 = CCSprite::createWithSpriteFrame(pJiaoSpriteFrame);
    jiao1->setAnchorPoint(CCPointZero);
    jiao1->setPosition(ccp(4.0f, 5.0f));
    pRarityBgSprite->addChild(jiao1, 9, JIAO1_TAG);
    
    CCSprite* jiao2 = CCSprite::createWithSpriteFrame(pJiaoSpriteFrame);
    jiao2->setAnchorPoint(CCPointZero);
    jiao2->setPosition(ccp(66.0f, 5.0f));
    jiao2->setFlipX(true);
    pRarityBgSprite->addChild(jiao2, 9, JIAO2_TAG);
    CCSprite* jiao3 = CCSprite::createWithSpriteFrame(pJiaoSpriteFrame);
    jiao3->setAnchorPoint(CCPointZero);
    jiao3->setPosition(ccp(66.0f, 64.0f));
    jiao3->setFlipX(true);
    jiao3->setFlipY(true);
    pRarityBgSprite->addChild(jiao3, 9, JIAO3_TAG);
    CCSprite* jiao4 = CCSprite::createWithSpriteFrame(pJiaoSpriteFrame);
    jiao4->setAnchorPoint(CCPointZero);
    jiao4->setPosition(ccp(4.0f, 64.0f));
    jiao4->setFlipY(true);
    pRarityBgSprite->addChild(jiao4, 9, JIAO4_TAG);
    if ((heroRarity > 0 && heroRarity <= 2) || heroRarity == 4 || heroRarity == 6 || heroRarity == 9) {
        jiao1->setVisible(false);
        jiao2->setVisible(false);
        jiao3->setVisible(false);
        jiao4->setVisible(false);
    } else if (heroRarity == 3 || heroRarity == 5 || heroRarity == 8) {
        jiao1->setVisible(true);
        jiao2->setVisible(true);
        jiao3->setVisible(true);
        jiao4->setVisible(true);
    } else if (heroRarity == 7) {
        jiao1->setVisible(false);
        jiao2->setVisible(true);
        jiao3->setVisible(false);
        jiao4->setVisible(true);
    }
    
    CCSprite* pHeroHeadSprite = CCSprite::createWithSpriteFrameName(heroPic.c_str());
    pHeroHeadSprite->setAnchorPoint(CCPointZero);
    pHeroHeadSprite->setPosition(ccp(5.0f, 4.0f));
    pRarityBgSprite->addChild(pHeroHeadSprite, 7, HERO_HEAD_TAG);
    
    /*
     武将等级
     sprintf(szBuffer, "%d", heroLevel);
     if (isShowLev) {
     CCSprite* levLabel = CCSprite::create("CM_lev.png");
     levLabel->setAnchorPoint(ccp(0, 0));
     levLabel->setPosition(ccp(15, 0));
     pNode->addChild(levLabel, 10);
     
     CCStrokeLabel* levFont = CCStrokeLabel::create(buffer, "Arial", 17);
     levFont->setAnchorPoint(ccp(0, 0));
     levFont->setPosition(ccp(45, 0));
     pNode->addChild(levFont, 11);
     }
     */
    return pRarityBgSprite;
}

/*
 *  功能：
 *      根据背包武将数据更新精灵ui
 *  参数：
 *      pHeroOfPlayerItem   武将数据
 *      pBgSprite           背景精灵
 *  返回：
 *      对应武将数据的精灵
 */
CCSprite* GoBattleLayer::_updateHeroSpriteByHeroItem(HeroOfPlayerItem* pHeroOfPlayerItem, CCSprite* pBgSprite) {
    CCAssert(pHeroOfPlayerItem && pBgSprite, "invalid data");
    
    char szBuffer[64];
    std::string heroPic = pHeroOfPlayerItem->getHeroItem()->getHIcon();
    unsigned int heroRarity = pHeroOfPlayerItem->getHeroItem()->getRarity();
    //    unsigned int heroLevel = pHeroOfPlayerItem->getLevel();
    
    /* 稀有度背景 */
    if (1 == heroRarity) {
        strcpy(szBuffer, "HP_whi.png");
    } else if (2 ==heroRarity || 3 == heroRarity) {
        strcpy(szBuffer, "HP_gre.png");
    } else if (4 == heroRarity || 5 == heroRarity) {
        strcpy(szBuffer, "HP_blu.png");
    } else if (6 ==heroRarity || 7 == heroRarity || 8 == heroRarity) {
        strcpy(szBuffer, "HP_pur.png");
    } else {
        strcpy(szBuffer, "HP_ora.png");
    }
    pBgSprite->setTextureRect(m_pSpriteFrameCache->spriteFrameByName(szBuffer)->getRect());
    
    /* 武将框 */
    if (1 == heroRarity) {
        strcpy(szBuffer, "HP_whiBox.png");
    } else if (2 == heroRarity || 3 == heroRarity) {
        strcpy(szBuffer, "HP_greBox.png");
    } else if (4 == heroRarity || 5 == heroRarity) {
        strcpy(szBuffer, "HP_bluBox.png");
    } else if (6 == heroRarity || 7 == heroRarity || 8 == heroRarity) {
        strcpy(szBuffer, "HP_purBox.png");
    } else {
        strcpy(szBuffer, "HP_oraBox.png");
    }
    CCSprite* pRaBox= (CCSprite*)pBgSprite->getChildByTag(RABOX_TAG);
    pRaBox->setTextureRect(m_pSpriteFrameCache->spriteFrameByName(szBuffer)->getRect());
    
    /* 角标显示规律 */
    if (1 == heroRarity) {
        strcpy(szBuffer, "HP_whiJiao.png");
    } else if (2 == heroRarity || 3 == heroRarity) {
        strcpy(szBuffer, "HP_greJiao.png");
    } else if (4 == heroRarity || 5 == heroRarity) {
        strcpy(szBuffer, "HP_bluJiao.png");
    } else if (6 == heroRarity || 7 == heroRarity || 8 == heroRarity) {
        strcpy(szBuffer, "HP_purJiao.png");
    } else {
        strcpy(szBuffer, "HP_oraJiao.png");
    }
    
    CCSprite* jiao1 = (CCSprite*)pBgSprite->getChildByTag(JIAO1_TAG);
    jiao1->setTextureRect(m_pSpriteFrameCache->spriteFrameByName(szBuffer)->getRect());
    CCSprite* jiao2 = (CCSprite*)pBgSprite->getChildByTag(JIAO2_TAG);
    jiao2->setTextureRect(m_pSpriteFrameCache->spriteFrameByName(szBuffer)->getRect());
    CCSprite* jiao3 = (CCSprite*)pBgSprite->getChildByTag(JIAO3_TAG);
    jiao3->setTextureRect(m_pSpriteFrameCache->spriteFrameByName(szBuffer)->getRect());
    CCSprite* jiao4 = (CCSprite*)pBgSprite->getChildByTag(JIAO4_TAG);
    jiao4->setTextureRect(m_pSpriteFrameCache->spriteFrameByName(szBuffer)->getRect());
    /*稀有度*/
    if ((heroRarity > 0 && heroRarity <= 2) || heroRarity == 4 || heroRarity == 6 || heroRarity == 9) {
        jiao1->setVisible(false);
        jiao2->setVisible(false);
        jiao3->setVisible(false);
        jiao4->setVisible(false);
    } else if (heroRarity == 3 || heroRarity == 5 || heroRarity == 8) {
        jiao1->setVisible(true);
        jiao2->setVisible(true);
        jiao3->setVisible(true);
        jiao4->setVisible(true);
    } else if (heroRarity == 7) {
        jiao1->setVisible(false);
        jiao2->setVisible(true);
        jiao3->setVisible(false);
        jiao4->setVisible(true);
    }
    
    CCSprite* pHeroHeadSprite = (CCSprite*)pBgSprite->getChildByTag(HERO_HEAD_TAG);
    pHeroHeadSprite->setTextureRect(m_pSpriteFrameCache->spriteFrameByName(heroPic.c_str())->getRect());
    
    /*
     武将等级
     sprintf(szBuffer, "%d", heroLevel);
     if (isShowLev) {
     CCSprite* levLabel = CCSprite::create("CM_lev.png");
     levLabel->setAnchorPoint(ccp(0, 0));
     levLabel->setPosition(ccp(15, 0));
     pNode->addChild(levLabel, 10);
     
     CCStrokeLabel* levFont = CCStrokeLabel::create(buffer, "Arial", 17);
     levFont->setAnchorPoint(ccp(0, 0));
     levFont->setPosition(ccp(45, 0));
     pNode->addChild(levFont, 11);
     }
     */
    return pBgSprite;
}

