//
//  TongQueLayer.cpp
//  hero
//
//  Created by yangjie on 2014/02/19.
//
//

#include "TongQueLayer.h"
#include "GameState.h"
#include "Battle.h"
#include "common.h"
#include "customMessage.h"
#include "MessageBox.h"
#include "CardBox.h"
#include "HeroManager.h"
#include "TqHeadItem.h"
#include "GamePlayer.h"
#include "NetConnection.h"
#include "BattleReportItem.h"
#include "CCStrokeLabel.h"


#include "MosterItem.h"
#include "HeroItem.h"

#define TQT_IMAGE_FILE          "tqt.pvr.ccz"
#define TQT_LIST_FILE           "tqt.plist"

#define TQT_BG_TAG              101                     // 背景图 tag
#define NODE_TAG                102                     // node_tag
#define MYRANK_TAG              103                     // 我的排名
#define MYREMAIN_TIEMS_TAG      104                     // 今日剩余次数
#define RANK_FONT_TAG           105                     // 105 ~ 107
#define AWARD_REMIAN_TIMES_TAG  108                     // 领取奖励剩余时间
#define GET_AWARD_MONEY_TAG     109                     // 奖励获得的铜钱
#define POINT_TAG               110
#define RANK_BG                 120
#define RESET_TIME_FOR_FIGHT    131                     // 攻击冷却时间
#define RESET_TIME_FOR_FONT     132

#define NAME_FONT_TAG           125                     // 125 ~ 127

#define TQT_BG_Y                122.0f                  // 背景图 Y 坐标
#define DURATION_TIME           0.3
#define LAYER_INIT_POSITION     ccp(0.0f, -850.0f)    // 初始化位置
#define POINT_INIT_X            50.0f
#define POINT_MAX_X             592.0f
#define MOVE_V                  15.0f

#define BAI_COLOR               ccc3(255, 255, 255)


/*
 *  构造函数
 */
TongQueLayer::TongQueLayer():
m_pGameState(GameState::getInstance())
,m_pBattle(Battle::getInstance())
,m_pGamePlayer(GamePlayer::getInstance())
,m_timeForFight(GameState::getInstance()->getCd())
,m_isInReset(false)
{
    float offet = m_pGameState->getBottomOffset();
    for (int i = 0; i < 3; i++) {
        m_heroRect[i] = CCRectMake(30.0f + 220.0f * (2 - i), 285.0f + offet, 140.0f, 200.0f);
        m_rectEnable[i] = true;
    }
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(TQT_LIST_FILE, TQT_IMAGE_FILE);
}

/*
 *  析构函数
 */
TongQueLayer::~TongQueLayer() {
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(TQT_LIST_FILE);
    CCTextureCache::sharedTextureCache()->removeTextureForKey(TQT_IMAGE_FILE);
}

/*
 *  初始化
 */
bool TongQueLayer::init() {
    // super init
    if (!CCLayer::init()) {
        return false;
    }
    this->setTouchEnabled(true);
    this->addChild(m_pSBN = CCSpriteBatchNode::create(TQT_IMAGE_FILE));
    
    // 初始化 UI 组件位置
    _initUi();
    
    m_timeForAward = m_pGameState->getRemainTimes(TIME_TQT);
    if (m_timeForFight > 0) {
        m_isInReset = true;/*正在冷却*/
    }
    // 动态更新
    _updateRank();
    _initAwardBtn();
    _updateTimeDown();
    _scheduleForTimeDown();
    _updateCopper();
    _updatePoint();
    _updateReport();
    _updateTimeFight();/**/
    return true;
}

/* 服务器返回成功则调用 */
void TongQueLayer::updateAllTQ() {
    _initAwardBtn();
}

/* 更新排名、今日剩余、攻击次数 */
/* 更新我的排名排名底板 */
void TongQueLayer::_updateRank() {
    char buffer[16];
    // 排名
    CCStrokeLabel* pMyRank = (CCStrokeLabel*)this->getChildByTag(NODE_TAG)->getChildByTag(MYRANK_TAG);
    sprintf(buffer, "%d", m_pGamePlayer->getRank());
    pMyRank->setString(buffer);
    // 剩余次数
    CCLabelTTF* pRemainTimes = (CCLabelTTF*)this->getChildByTag(NODE_TAG)->getChildByTag(MYREMAIN_TIEMS_TAG);
    sprintf(buffer, "%d", m_pBattle->getRemainBattleTimes() );
    pRemainTimes->setString(buffer);
}

void TongQueLayer::_updateCopper() {
    char buffer[16];
    // 排名
    CCLabelTTF* pMyRank = (CCLabelTTF*)this->getChildByTag(NODE_TAG)->getChildByTag(GET_AWARD_MONEY_TAG);
    sprintf(buffer, "x%d", m_pGamePlayer->getRewardMoney());
    pMyRank->setString(buffer);
}

void TongQueLayer::_updatePoint() {
    float curPoint = (1.0 - (float)m_pGamePlayer->getRank() / (float)m_pGamePlayer->getTotalRank()) * (POINT_MAX_X - POINT_INIT_X);
//    CCLog("curpoimnt = %f", curPoint);
    CCSprite* point = static_cast<CCSprite*>(m_pSBN->getChildByTag(TQT_BG_TAG)->getChildByTag(POINT_TAG));
    point->setPosition(ccp(POINT_INIT_X + curPoint , point->getPosition().y));
    float x = 1.0 - (float)m_pGamePlayer->getRank() / (float)m_pGamePlayer->getTotalRank();
//    CCLog("fffff = %f",x);
    if (x <= (5.0)/(18.5))/*白色*/ {
        for (int i = 0; i < 5; ++i) {
            CCSprite* p = static_cast<CCSprite*>(m_pSBN->getChildByTag(TQT_BG_TAG)->getChildByTag(RANK_BG + i));
            if (i == 0) {
                p->setVisible(true);
            } else {
                p->setVisible(false);
            }
        }
    } else if (x <= (9.5)/(18.5) && x > (5.0)/(18.5))/*绿色*/ {
        for (int i = 0; i < 5; ++i) {
            CCSprite* p = static_cast<CCSprite*>(m_pSBN->getChildByTag(TQT_BG_TAG)->getChildByTag(RANK_BG + i));
            if (i == 1) {
                p->setVisible(true);
            } else {
                p->setVisible(false);
            }
        }
    } else if (x <= (13.5)/(18.5) && x > (9.5)/(18.5))/*蓝色*/ {
        for (int i = 0; i < 5; ++i) {
            CCSprite* p = static_cast<CCSprite*>(m_pSBN->getChildByTag(TQT_BG_TAG)->getChildByTag(RANK_BG + i));
            if (i == 2) {
                p->setVisible(true);
            } else {
                p->setVisible(false);
            }
        }
    } else if (x <= (17.5)/(18.5) && x > (13.5)/(18.5))/*紫色*/ {
        for (int i = 0; i < 5; ++i) {
            CCSprite* p = static_cast<CCSprite*>(m_pSBN->getChildByTag(TQT_BG_TAG)->getChildByTag(RANK_BG + i));
            if (i == 3) {
                p->setVisible(true);
            } else {
                p->setVisible(false);
            }
        }
    } else /*橙色*/ {
        for (int i = 0; i < 5; ++i) {
            CCSprite* p = static_cast<CCSprite*>(m_pSBN->getChildByTag(TQT_BG_TAG)->getChildByTag(RANK_BG + i));
            if (i == 4) {
                p->setVisible(true);
            } else {
                p->setVisible(false);
            }
        }
    }
}

void TongQueLayer::_updateReport() {
    Battle* pBattle = Battle::getInstance();
    char buffer[128];
    for (int i = 0; i < pBattle->getCountOfBattleReport(); ++i) {
        BattleReportItem* pBattleReportItem = pBattle->getBattleReportItemByIndex(i);
        if (pBattleReportItem) {
            if (pBattleReportItem->getTp() == 1)/*发起人*/ {
                if (pBattleReportItem->getUd() == 1)/*挑战胜利*/ {
                    sprintf(buffer, "你战胜了%s,排名上升到%d,奖励:铜钱+%d",pBattleReportItem->getName().c_str(),
                            pBattleReportItem->getRank(),pBattleReportItem->getMoney());
                } else if (pBattleReportItem->getUd() == 2) {
                    sprintf(buffer, "你战败于%s,排名不变,奖励:铜钱+%d",pBattleReportItem->getName().c_str(), pBattleReportItem->getMoney());
                }
            } else {
                if (pBattleReportItem->getUd() == 2)/*被挑战胜利*/ {
                    sprintf(buffer, "%s战败于你,排名不变", pBattleReportItem->getName().c_str());
                } else {
                    sprintf(buffer, "%s战胜了你,排名下降到%d", pBattleReportItem->getName().c_str(), pBattleReportItem->getRank());
                }
            }
            m_reportLabel[i]->setString(buffer);
        } else {
            m_reportLabel[i]->setString("");
        }
    }
}

/*
 *  初始化Ui
 */
void TongQueLayer::_initUi() {
    CCSprite* pBgSprite = CCSprite::createWithSpriteFrameName("tqt_background.png");
    pBgSprite->setAnchorPoint(CCPointZero);
    pBgSprite->setPosition(ccp(0.0f, m_pGameState->getBottomOffset() + TQT_BG_Y));
    m_pSBN->addChild(pBgSprite, 1, TQT_BG_TAG);
    
    CCSprite* pBottomSprite = CCSprite::createWithSpriteFrameName("tqt_bottom.png");
    pBottomSprite->setAnchorPoint(CCPointZero);
    pBottomSprite->setPosition(ccp(0.0f, 398.0f + MOVE_V));
    pBgSprite->addChild(pBottomSprite, 2);
    
    CCSprite* pColorSprite = CCSprite::createWithSpriteFrameName("tqt_color.png");
    pColorSprite->setAnchorPoint(CCPointZero);
    pColorSprite->setPosition(ccp(50.0f, 398.0f + MOVE_V));
    pBgSprite->addChild(pColorSprite, 1);
    
    // 我的排名
    CCSprite* pWdpmSprite = CCSprite::createWithSpriteFrameName("tqt_wodepaiming.png");
    pWdpmSprite->setPosition(ccp(98.0f, 450.0f + MOVE_V));
    pBgSprite->addChild(pWdpmSprite, 1);
    /* 排名底纹 */
    CCSprite* pWhite = CCSprite::createWithSpriteFrameName("tqt_white.png");
    pWhite->setPosition(ccp(325.0f, 450.0f + MOVE_V));
    pWhite->setVisible(false);
    pBgSprite->addChild(pWhite, 1, RANK_BG);
    CCSprite* pGreen = CCSprite::createWithSpriteFrameName("tqt_green.png");
    pGreen->setPosition(ccp(325.0f, 450.0f + MOVE_V));
    pGreen->setVisible(false);
    pBgSprite->addChild(pGreen, 1, RANK_BG + 1);
    CCSprite* pBlue = CCSprite::createWithSpriteFrameName("tqt_blue.png");
    pBlue->setPosition(ccp(325.0f, 450.0f + MOVE_V));
    pBlue->setVisible(false);
    pBgSprite->addChild(pBlue, 1, RANK_BG + 2);
    CCSprite* pPurple = CCSprite::createWithSpriteFrameName("tqt_violet.png");
    pPurple->setPosition(ccp(325.0f, 450.0f + MOVE_V));
    pPurple->setVisible(false);
    pBgSprite->addChild(pPurple, 1, RANK_BG +3);
    CCSprite* pOrange = CCSprite::createWithSpriteFrameName("tqt_orange.png");
    pOrange->setPosition(ccp(325.0f, 450.0f + MOVE_V));
    pOrange->setVisible(false);
    pBgSprite->addChild(pOrange, 1, RANK_BG + 4);
    
    
    //
    CCSprite* pJrsySprite = CCSprite::createWithSpriteFrameName("tqt_remaintime.png");
    pJrsySprite->setPosition(ccp(490.0f, 460.0f + MOVE_V));
    pBgSprite->addChild(pJrsySprite, 1);
    
    CCSprite* pGongJi = CCSprite::createWithSpriteFrameName("tqt_resettime.png");
    pGongJi->setPosition(ccp(490.0f, 440.0f + MOVE_V));
    pBgSprite->addChild(pGongJi, 1, RESET_TIME_FOR_FIGHT);
    if (m_timeForFight <= 0) {
        pGongJi->setVisible(false);
    } else {
        pGongJi->setVisible(true);
    }
    // 指针
    CCSprite* pZhiZhen = CCSprite::createWithSpriteFrameName("tqt_zhizheng.png");
    pZhiZhen->setPosition(ccp(55.0f, 405.0f + MOVE_V));
    pBgSprite->addChild(pZhiZhen, 10, POINT_TAG);
    
    
    
    // 奖励图标
    m_bgOfAward[0] = CCSprite::createWithSpriteFrameName("tqt_jianglibox.png");
    m_bgOfAward[0]->setPosition(ccp(532.0f, 88.0f));
    pBgSprite->addChild(m_bgOfAward[0], 1);
    m_btnEnable = true;
    m_bgOfAward[1] = CCSprite::createWithSpriteFrameName("tqt_jiangliHighLight.png");
    m_bgOfAward[1]->setPosition(ccp(532.0f, 88.0f));
    m_bgOfAward[1]->setVisible(false);
    pBgSprite->addChild(m_bgOfAward[1], 1);
    m_bgOfAward[2] = CCSprite::createWithSpriteFrameName("tqt_jiangliboxdisable.png");
    m_bgOfAward[2]->setPosition(ccp(532.0f, 88.0f));
    m_bgOfAward[2]->setVisible(false);
    pBgSprite->addChild(m_bgOfAward[2], 1);
    
    m_awardFont[0] = CCSprite::createWithSpriteFrameName("tqt_jiangli.png");
    m_awardFont[0]->setPosition(ccp(532.0f, 125.0f));
    pBgSprite->addChild(m_awardFont[0], 1);
    m_awardFont[1] = CCSprite::createWithSpriteFrameName("tqt_jiangli_gray.png");
    m_awardFont[1]->setPosition(ccp(532.0f, 125.0f));
    m_awardFont[1]->setVisible(false);
    pBgSprite->addChild(m_awardFont[1], 1);
    
    
    CCSprite* pDivH = CCSprite::createWithSpriteFrameName("tqt_divisionh.png");
    pDivH->setPosition(ccp(321.0f, 170.0f));
    pBgSprite->addChild(pDivH, 2);
    
    CCSprite* pDivV = CCSprite::createWithSpriteFrameName("tqt_divisionv.png");
    pDivV->setPosition(ccp(410.0f, 80.0f));
    pBgSprite->addChild(pDivV, 2);
    
    // 三个底座
    CCSprite* pBase1 = CCSprite::createWithSpriteFrameName("tqt_base.png");
    pBase1->setPosition(ccp(100.0f, 186.0f));
    pBgSprite->addChild(pBase1, 1);
    CCSprite* pZdl1 = CCSprite::createWithSpriteFrameName("tqt_zhandouli.png");
    pZdl1->setPosition(ccp(54.0f, 24.0f));
    pBase1->addChild(pZdl1, 1);
    CCSprite* pNo1 = CCSprite::createWithSpriteFrameName("tqt_no.png");
    pNo1->setPosition(ccp(64.0f, 232.0f + MOVE_V));
    pBase1->addChild(pNo1, 1);
    CCSprite* pBase2 = CCSprite::createWithSpriteFrameName("tqt_base.png");
    pBase2->setPosition(ccp(100.0f + 218.0f, 186.0f));
    pBgSprite->addChild(pBase2, 1);
    CCSprite* pZdl2 = CCSprite::createWithSpriteFrameName("tqt_zhandouli.png");
    pZdl2->setPosition(ccp(54.0f, 24.0f));
    pBase2->addChild(pZdl2, 1);
    CCSprite* pNo2 = CCSprite::createWithSpriteFrameName("tqt_no.png");
    pNo2->setPosition(ccp(64.0f, 232.0f + MOVE_V));
    pBase2->addChild(pNo2, 1);
    CCSprite* pBase3 = CCSprite::createWithSpriteFrameName("tqt_base.png");
    pBase3->setPosition(ccp(100.0f + 218.0f * 2.0f, 186.0f));
    pBgSprite->addChild(pBase3, 1);
    CCSprite* pZdl3 = CCSprite::createWithSpriteFrameName("tqt_zhandouli.png");
    pZdl3->setPosition(ccp(54.0f, 24.0f));
    pBase3->addChild(pZdl3, 1);
    CCSprite* pNo3 = CCSprite::createWithSpriteFrameName("tqt_no.png");
    pNo3->setPosition(ccp(64.0f, 232.0f + MOVE_V));
    pBase3->addChild(pNo3, 1);
    
    
    // 战报底板
    CCSprite* pDiBan = CCSprite::createWithSpriteFrameName("tqt_box.png");
    pDiBan->setPosition(ccp(230.0f, 86.0f));
    pBgSprite->addChild(pDiBan, 1);
    
    
    /* 系统文字 */
    // 我的排名
    CCNode* pNode = CCNode::create();
    pNode->setPosition(ccp(0, m_pGameState->getBottomOffset()));
    addChild(pNode, 3, NODE_TAG);
    CCStrokeLabel* pMyRank = CCStrokeLabel::create("", "Arial", 35);
    pMyRank->setAnchorPoint(ccp(0.5, 0));
    pMyRank->setPosition(ccp(320.0f, 555.0f));
    pNode->addChild(pMyRank, 1, MYRANK_TAG);
    CCLabelTTF* pRemainTimes = CCLabelTTF::create("", "Arial", 19);
    pRemainTimes->setPosition(ccp(555.0f, 580.0f  + MOVE_V));
    pNode->addChild(pRemainTimes, 1, MYREMAIN_TIEMS_TAG);
    char buffer[16];
    int min = 0;
    int sec = 0;
    sec = m_timeForFight % 60;
    min = m_timeForFight / 60;
    sprintf(buffer, "%02d:%02d", min, sec);
    m_timeForFightSch = CCLabelTTF::create(buffer, "Arial", 19);
    m_timeForFightSch->setPosition(ccp(570.0f, 560.0f  + MOVE_V));
    pNode->addChild(m_timeForFightSch, 1, RESET_TIME_FOR_FONT);
    if (m_timeForFight <= 0) {
        m_timeForFightSch->setVisible(false);
    } else {
        m_timeForFightSch->setVisible(true);
    }
    
    CCLabelTTF* pGetAwardMoney = CCLabelTTF::create("", "Arial", 23);
    pGetAwardMoney->setPosition(ccp(495.0f, 205.0f));
    pGetAwardMoney->setAnchorPoint(ccp(0, 0));
    pNode->addChild(pGetAwardMoney, 1, GET_AWARD_MONEY_TAG);
    // 领取奖励剩余时间
    CCLabelTTF* pAwardTimes = CCLabelTTF::create("", "Arial", 30);
    pAwardTimes->setPosition(ccp(530.0f, 185.0f));
    pNode->addChild(m_timeSchedule = pAwardTimes, 1, AWARD_REMIAN_TIMES_TAG);
    CCSprite* spGold = CCSprite::create("copper.png");
    spGold->setPosition(ccp(485.0f, 215.0f));
    pNode->addChild(spGold);
    
    //战报
    m_reportLabel[0] = CCLabelTTF::create("", "Arial", 19);
    m_reportLabel[0]->setAnchorPoint(ccp(0, 0));
    m_reportLabel[0]->setPosition(ccp(40.0f, 250.0f));
    pNode->addChild(m_reportLabel[0], 5);
    m_reportLabel[1] = CCLabelTTF::create("", "Arial", 19);
    m_reportLabel[1]->setAnchorPoint(ccp(0, 0));
    m_reportLabel[1]->setPosition(ccp(40.0f, 225.0f));
    pNode->addChild(m_reportLabel[1], 5);
    m_reportLabel[2] = CCLabelTTF::create("", "Arial", 19);
    m_reportLabel[2]->setPosition(ccp(40.0f, 200.0f));
    m_reportLabel[2]->setAnchorPoint(ccp(0, 0));
    pNode->addChild(m_reportLabel[2], 5);
    m_reportLabel[3] = CCLabelTTF::create("", "Arial", 19);
    m_reportLabel[3]->setPosition(ccp(40.0f, 175.0f));
    m_reportLabel[3]->setAnchorPoint(ccp(0, 0));
    pNode->addChild(m_reportLabel[3], 5);
    m_reportLabel[4] = CCLabelTTF::create("", "Arial", 19);
    m_reportLabel[4]->setPosition(ccp(40.0f, 150.0f));
    m_reportLabel[4]->setAnchorPoint(ccp(0, 0));
    pNode->addChild(m_reportLabel[4], 5);
    
    HeroManager* pHeroManager = HeroManager::getInstance();
    /* 初始化铜雀台可挑战武将 */
    for (int i = 0; i < 3; ++i) {
        CardBox* card = CardBox::create();
        card->setPosition(ccp(33.0f + i * 218.0f, 322.0f));
        
        pNode->addChild(card);
        TqHeadItem* pTqHeadItem = pHeroManager->getTqHeadItem(3 - i);
        if (pTqHeadItem->getType() == 1)/*玩家*/ {
            HeroItem* pHeroItem = pHeroManager->getHeroItem(pTqHeadItem->getId());
            card->cardForTOngQue(pHeroItem);
        } else/*NPC*/ {
            MosterItem* pMosterItem = pHeroManager->getMosterDataById(pTqHeadItem->getId());
            card->cardForTongQue(pMosterItem);
        }
    }
    
    /* 武将排名文字 */
    for (int i = 0; i < 3; ++i) {
        char buffer[16];
        TqHeadItem* pTqHeadItem = pHeroManager->getTqHeadItem(3 - i);
        sprintf(buffer, "%d", pTqHeadItem->getRank());
        CCLabelTTF* pRank = CCLabelTTF::create(buffer, "Arial", 21);
        pRank->setAnchorPoint(ccp(0, 0));
        pRank->setPosition(ccp(110.0f + i * 218.0f, 501.0f  + MOVE_V));
        pNode->addChild(pRank, 1, RANK_FONT_TAG + i);
        CCLabelTTF* pName = CCLabelTTF::create(pTqHeadItem->getName().c_str(), "Arial", 21);
        pName->setAnchorPoint(ccp(0, 0));
        pName->setPosition(ccp(66.0f + i * 218.0f, 478.0f  + MOVE_V));
        pNode->addChild(pName, 1, NAME_FONT_TAG + i);
    }
}

/*
 *  移出动作
 */
void TongQueLayer::moveOutTouchUICompent() {
    CCSprite* parent = static_cast<CCSprite*>(m_pSBN->getChildByTag(TQT_BG_TAG));
    parent->runAction(CCSequence::create(CCMoveTo::create(DURATION_TIME, LAYER_INIT_POSITION), CCCallFunc::create(this, callfunc_selector(TongQueLayer::_callbackForMoveOut)),NULL));
    
    CCNode* pNode = (CCNode*)this->getChildByTag(NODE_TAG);
    pNode->runAction(CCMoveTo::create(DURATION_TIME, LAYER_INIT_POSITION));
    
    /* 回到PVE武将 */
    m_pBattle->setBattlePkMode(BPM_PVE);
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_MOVEGOBATTLE_STRING);
}

void TongQueLayer::_callbackForMoveOut() {
    this->removeFromParent();
//    CCLog("TONGQUE 从主场景移除");
}

/*
 *  恢复响应动作
 */
void TongQueLayer::restoreTouchUICompent() {
    m_pBattle->setBattlePkMode(BPM_PVP);
    m_pBattle->setCWar(false);
    CCSprite* parent = static_cast<CCSprite*>(m_pSBN->getChildByTag(TQT_BG_TAG));
    parent->runAction(CCMoveTo::create(DURATION_TIME, ccp(0.0f, m_pGameState->getBottomOffset() + TQT_BG_Y)));
    
    CCNode* pNode = (CCNode*)this->getChildByTag(NODE_TAG);
    pNode->runAction(CCMoveTo::create(DURATION_TIME, ccp(0, m_pGameState->getBottomOffset())));
}

void TongQueLayer::initPosition() {
    CCSprite* parent = static_cast<CCSprite*>(m_pSBN->getChildByTag(TQT_BG_TAG));
    parent->setPosition(LAYER_INIT_POSITION);
    
    CCNode* pNode = (CCNode*)this->getChildByTag(NODE_TAG);
    pNode->setPosition(LAYER_INIT_POSITION);
}


/*
 * 触摸有效
 */
void TongQueLayer::registerWithTouchDispatcher(void) {
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, false);
}

bool TongQueLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    bool validTouch = false;
    
    CCSprite* parent = static_cast<CCSprite*>(m_pSBN->getChildByTag(TQT_BG_TAG));
    CCPoint touchPoint = parent->convertTouchToNodeSpace(pTouch);
    /* 判断玩家点击奖励区域 */
    if (m_btnEnable && m_bgOfAward[0]->boundingBox().containsPoint(touchPoint)) {
        
        m_bgOfAward[0]->setVisible(false);
        m_bgOfAward[1]->setVisible(true);
        
        validTouch = true;
    }
    /* 层的点击坐标系 */
    CCPoint layerTouchPoint = this->convertTouchToNodeSpace(pTouch);
    m_touchBeganBtnIdx = -1;
    for (int i = 0; i < 3; ++i) {
        if (m_rectEnable[i] && m_heroRect[i].containsPoint(layerTouchPoint)) {
            m_touchBeganBtnIdx = i;
            validTouch = true;
            break;
        }
    } /*for*/
    
    /*响应战报*/
    CCNode* pNode = (CCNode*)this->getChildByTag(NODE_TAG);
    CCPoint nodeTouchPoint = pNode->convertTouchToNodeSpace(pTouch);
    m_touchBeganLabelIdx = -1;
    for (int i = 0; i < m_pBattle->getCountOfBattleReport(); ++i) {
        if (m_reportLabel[i]->boundingBox().containsPoint(nodeTouchPoint)) {
            m_reportLabel[i]->setColor(BLUE_COLOR);
            m_touchBeganLabelIdx = i;
            validTouch = true;
            break;
        }
    } /*for*/
    
    return  validTouch;
}

void TongQueLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {
    CCSprite* parent = static_cast<CCSprite*>(m_pSBN->getChildByTag(TQT_BG_TAG));
    CCPoint touchPoint = parent->convertTouchToNodeSpace(pTouch);
    /* 判断玩家点击奖励区域 */
    if (m_btnEnable && m_bgOfAward[0]->boundingBox().containsPoint(touchPoint)) {
        m_bgOfAward[0]->setVisible(false);
        m_bgOfAward[1]->setVisible(true);
    }
    /*响应战报*/
    CCNode* pNode = (CCNode*)this->getChildByTag(NODE_TAG);
    CCPoint nodeTouchPoint = pNode->convertTouchToNodeSpace(pTouch);
    for (int i = 0; i < m_pBattle->getCountOfBattleReport(); ++i) {
        if (m_reportLabel[i]->boundingBox().containsPoint(nodeTouchPoint)) {
            m_reportLabel[i]->setColor(BAI_COLOR);
        }
    } /*for*/
}

void TongQueLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {
    CCSprite* parent = static_cast<CCSprite*>(m_pSBN->getChildByTag(TQT_BG_TAG));
    CCPoint touchPoint = parent->convertTouchToNodeSpace(pTouch);
    /* 判断玩家点击奖励区域 */
    if (m_btnEnable && m_bgOfAward[0]->boundingBox().containsPoint(touchPoint)) {
        _getAward();
    }
    
    /* 层的点击坐标系 */
    CCPoint layerTouchPoint = this->convertTouchToNodeSpace(pTouch);
    int selectBtnIdx = -1;
    for (int i = 0; i < 3; ++i) {
        if (m_rectEnable[i] && m_heroRect[i].containsPoint(layerTouchPoint)) {
            m_touchBeganBtnIdx = i;
            selectBtnIdx = i + 1;
            break;
        }
    }
    if (selectBtnIdx > 0) {
        _onBtnSelect(selectBtnIdx);
        return;
    }
    
    int selectReportIdx = -1;
    CCNode* pNode = (CCNode*)this->getChildByTag(NODE_TAG);
    CCPoint nodeTouchPoint = pNode->convertTouchToNodeSpace(pTouch);
    for (int i = 0; i < m_pBattle->getCountOfBattleReport(); ++i) {
        if (m_reportLabel[i]->boundingBox().containsPoint(nodeTouchPoint)) {
            m_reportLabel[i]->setColor(BAI_COLOR);
            selectReportIdx = i + 1;
            break;
        }
    } /*for*/
    if (selectReportIdx > 0) {
        _onReportSelect(selectReportIdx);
        return;
    }
    
}

/*
 *  回放点击的战斗报告
 */
void TongQueLayer::_onReportSelect(int idx) {
    if (idx >= 1 && idx <= 5) {
        NetConnection* pNetConnection = NetConnection::getInstance();
        char szPostBuffer[MAX_POSTTO_SERVER_BUFFER_SIZE];
        memset(szPostBuffer, '\0', MAX_POSTTO_SERVER_BUFFER_SIZE);
        char szHttpTag[] = HTTP_TONGQUETAI_REPLAY;
        
        sprintf(szPostBuffer, "c=contest&a=getReport&num=%d&uid=%s", idx, m_pGamePlayer->getUid().c_str());
        pNetConnection->commitPostRequestData(szPostBuffer, szHttpTag);
    }
}

//
void TongQueLayer::_onBtnSelect(int idx) {
    /*铜雀台挑战剩余次数不够*/
    if (m_pBattle->getRemainBattleTimes() <= 0 || m_isInReset) {
//        return;
    }
    if (idx >= 1 && idx <= 3) {
        m_pGameState->setAwardSaveTime();
        m_pBattle->setTqtTeamPosition(idx);
        CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_CHOSFORM_PVP_STRING);
    }
}

void TongQueLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {
    /* 玩家取消点击则回到初始化状态 */
    _initAwardBtn();
}

void TongQueLayer::_initAwardBtn() {
    if (m_pGamePlayer->getIsReward())/*领过*/ {
        m_bgOfAward[0]->setVisible(false);
        m_bgOfAward[1]->setVisible(false);
        m_bgOfAward[2]->setVisible(true);
        m_awardFont[0]->setVisible(false);
        m_awardFont[1]->setVisible(true);
        m_btnEnable = false;
    } else {
        m_bgOfAward[0]->setVisible(true);
        m_bgOfAward[1]->setVisible(false);
        m_bgOfAward[2]->setVisible(false);
        m_awardFont[0]->setVisible(true);
        m_awardFont[1]->setVisible(false);
        m_btnEnable = true;
    }
    
    for (int i = 0; i < m_pBattle->getCountOfBattleReport(); ++i) {
        m_reportLabel[i]->setColor(BAI_COLOR);
    }
}

/* 奖励计时 */
void TongQueLayer::_updateTimeDown() {
    schedule(schedule_selector(TongQueLayer::_scheduleForTimeDown), 1);
}

void TongQueLayer::_scheduleForTimeDown() {
    char buffer[32];
    /* 更新下次领取时间 */
    int hour = 0;
    int min  = 0;
    int second = 0;
    
    second = m_timeForAward % 60;
    hour = (int)(m_timeForAward / 3600);
    min  = (int)((m_timeForAward % 3600) / 60);
    
    sprintf(buffer, "%02d:%02d:%02d", hour, min, second);
    m_timeSchedule->setString(buffer);
    if (--m_timeForAward <= 0) {
        m_timeForAward = 0;
        _initAwardBtn();
        unschedule(schedule_selector(TongQueLayer::_scheduleForTimeDown));
    }
}

void TongQueLayer::_updateTimeFight() {
    schedule(schedule_selector(TongQueLayer::_scheduleForFight), 1);
}
void TongQueLayer::_scheduleForFight() {
    char buffer[16];
    int min = 0;
    int sec = 0;
    sec = m_timeForFight % 60;
    min = m_timeForFight / 60;
    sprintf(buffer, "%02d:%02d", min, sec);
    m_timeForFightSch->setString(buffer);
    if (--m_timeForFight <= 0) {
        CCSprite* parent = static_cast<CCSprite*>(m_pSBN->getChildByTag(TQT_BG_TAG));
        CCSprite* spResetIcon = (CCSprite*)parent->getChildByTag(RESET_TIME_FOR_FIGHT);
        spResetIcon->setVisible(false);
        m_timeForFightSch->setVisible(false);
        m_timeForFight = 0;
        m_isInReset = false;/*不是在冷却中*/
        unschedule(schedule_selector(TongQueLayer::_scheduleForFight));
    }
}

/*领取奖励*/
void TongQueLayer::_getAward() {
    NetConnection* pNetConnection = NetConnection::getInstance();
    char szPostBuffer[MAX_POSTTO_SERVER_BUFFER_SIZE];
    memset(szPostBuffer, '\0', MAX_POSTTO_SERVER_BUFFER_SIZE);
    char szHttpTag[] = HTTP_TONGQUETAI_AWARD;
    
    sprintf(szPostBuffer, "c=contest&a=reward&uid=%s", m_pGamePlayer->getUid().c_str());
    pNetConnection->commitPostRequestData(szPostBuffer, szHttpTag);
}
