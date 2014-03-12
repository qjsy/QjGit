//
//  GuoZhanLayer.cpp
//  hero
//
//  Created by 钱剑 on 2014/02/25.
//
//

#include "common.h"
#include "customMessage.h"
#include "GuoZhanLayer.h"
#include "GameState.h"
#include "Battle.h"
#include "CityManager.h"
#include "CityItem.h"
#include "NetConnection.h"
#include "GamePlayer.h"
#include "CCStrokeLabel.h"

#define GUOZ_IMAGE_FILE             "guozhan.pvr.ccz"
#define GUOZ_LIST_FILE              "guozhan.plist"

#define CITY_OF_WEI                 1
#define CITY_OF_SHU                 2
#define CITY_OF_WU                  3

// TAG
#define GUOZ_BG_TAG                 1
#define REMAIN_CHANCE_TAG           2
#define NODE_TAG                    3
#define GUOZ_FADEIN_TAG             4
// value
#define INIT_LAYER_POSITION         ccp(0.0f, -850.0f)
#define DURATION_TIME               0.3f
#define GUOZ_BG_V                   125.0f
#define FONT_V                      560.0f
#define BTN_HIGHLIGHT_OFFSET_H      0.0f
#define BTN_HIGHLIGHT_OFFSET_V      8.0f

/*构造函数*/
GuoZhanLayer::GuoZhanLayer()
:m_pGameState(GameState::getInstance())
,m_pBattle(Battle::getInstance())
,m_pCityManager(CityManager::getInstance())
{
    
    memset(m_pProgress, '\0', sizeof(m_pProgress));
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(GUOZ_LIST_FILE, GUOZ_IMAGE_FILE);
        m_btnEnable[0] = true;
        m_btnEnable[1] = true;
        m_btnEnable[2] = true;
        m_btnPosition[0][0] = ccp(400.0f, 295.0f);
        m_btnPosition[0][1] = ccp(400.0f, 295.0f);
        m_btnPosition[0][2] = ccp(400.0f, 295.0f);
        m_btnPosition[1][0] = ccp(80.0f,165.0f);
        m_btnPosition[1][1] = ccp(80.0f,165.0f);
        m_btnPosition[1][2] = ccp(80.0f,165.0f);
        m_btnPosition[2][0] = ccp(340.0f, 85.0f);
        m_btnPosition[2][1] = ccp(340.0f, 85.0f);
        m_btnPosition[2][2] = ccp(340.0f, 85.0f);
}
/*析构函数*/
GuoZhanLayer::~GuoZhanLayer(){
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(GUOZ_LIST_FILE);
    CCTextureCache::sharedTextureCache()->removeTextureForKey(GUOZ_IMAGE_FILE);
}
/*初始化*/
bool GuoZhanLayer::init() {
    if (!CCLayer::init()) {
        return false;
    }
    this->setTouchEnabled(true);
    /*距离国战开始时间或者结束时间*/
    if (m_pGameState->getIsBeganGZ()) {
        m_timeForBeganFight = m_pGameState->getRemainTimes(TIME_GZEND);
    } else {
        m_timeForBeganFight = m_pGameState->getRemainTimes(TIME_GZBEGAN);
    }
    
    /*加载大图*/
    this->addChild(m_pSBN = CCSpriteBatchNode::create(GUOZ_IMAGE_FILE));
    /* 初始化UI */
    _initUi();
    
    
    return true;
}

/*界面ui初始化*/
void GuoZhanLayer::_initUi() {
    /*字符串缓冲区*/
    char buffer[32];
    
    CCNode* pNode = CCNode::create();
    pNode->setPosition(ccp(0, m_pGameState->getBottomOffset()));
    addChild(pNode, 3, NODE_TAG);
    
    CCSprite* pBgSprite = CCSprite::createWithSpriteFrameName("gz_cityBg.png");
    pBgSprite->setAnchorPoint(CCPointZero);
    pBgSprite->setPosition(ccp(0.0f, m_pGameState->getBottomOffset() + GUOZ_BG_V));
    m_pSBN->addChild(pBgSprite, 1, GUOZ_BG_TAG);
    
    CCSprite* pFadeIn = CCSprite::createWithSpriteFrameName("gz_cityBg.png");
    pFadeIn->setAnchorPoint(CCPointZero);
    pFadeIn->setOpacity(0);
    pFadeIn->setPosition(ccp(0.0f, m_pGameState->getBottomOffset() + GUOZ_BG_V));
    m_pSBN->addChild(pFadeIn, 1, GUOZ_FADEIN_TAG);
    
    
    /*是否已经处于国战中*/
    m_isFighting = m_pGameState->getIsBeganGZ();
//    m_isFighting = true;
    if (m_isFighting)/*国战进行*/ {
        CCSprite* spBeganForGz = CCSprite::createWithSpriteFrameName("gz_end.png");
        spBeganForGz->setAnchorPoint(CCPointZero);
        spBeganForGz->setPosition(ccp(0.0f, FONT_V - GUOZ_BG_V + 30.0f));
        pBgSprite->addChild(spBeganForGz);
        
        // 守军数量
        CCSprite* numLy = CCSprite::createWithSpriteFrameName("gz_font_defenders.png");
        numLy->setAnchorPoint(CCPointZero);
        numLy->setScale(0.7);
        numLy->setPosition(ccp(420.0f, 430.0f));
        pBgSprite->addChild(numLy, 2);
        CCSprite* numCa = CCSprite::createWithSpriteFrameName("gz_font_defenders.png");
        numCa->setAnchorPoint(CCPointZero);
        numCa->setScale(0.7);
        numCa->setPosition(ccp(95.0f, 290.0f));
        pBgSprite->addChild(numCa, 2);
        CCSprite* numCl = CCSprite::createWithSpriteFrameName("gz_font_defenders.png");
        numCl->setAnchorPoint(CCPointZero);
        numCl->setScale(0.7);
        numCl->setPosition(ccp(355.0f, 210.0f));
        pBgSprite->addChild(numCl, 2);
        // 守军数量
        char buffer[16];
        CityItem* pCityItem = m_pCityManager->getCityItem(1);
        sprintf(buffer, "%d", pCityItem->getGuardNum());
        CCStrokeLabel* pLabelLy = CCStrokeLabel::create(buffer, "Arial", 23);
        pLabelLy->setAnchorPoint(CCPointZero);
        pLabelLy->setPosition(ccp(480.0f, 553.0f));
        pNode->addChild(pLabelLy);
        CityItem* pCityItem2 = m_pCityManager->getCityItem(2);
        sprintf(buffer, "%d", pCityItem2->getGuardNum());
        CCStrokeLabel* pLabelCa = CCStrokeLabel::create(buffer, "Arial", 23);
        pLabelCa->setAnchorPoint(CCPointZero);
        pLabelCa->setPosition(ccp(150.0f, 413.0f));
        pNode->addChild(pLabelCa);
        CityItem* pCityItem3 = m_pCityManager->getCityItem(1);
        sprintf(buffer, "%d", pCityItem3->getGuardNum());
        CCStrokeLabel* pLabelCl = CCStrokeLabel::create(buffer, "Arial", 23);
        pLabelCl->setAnchorPoint(CCPointZero);
        pLabelCl->setPosition(ccp(410.0f, 333.0f));
        pNode->addChild(pLabelCl);
        
        /*国战进行图片*/
        CCSprite* spCwarIng = CCSprite::createWithSpriteFrameName("gz_ing.png");
        spCwarIng->setAnchorPoint(CCPointZero);
        spCwarIng->setPosition(ccp(-25.0f, 300.0f));
        pBgSprite->addChild(spCwarIng);
        
        
    } else {
        CCSprite* spBeganForGz = CCSprite::createWithSpriteFrameName("gz_began.png");
        spBeganForGz->setAnchorPoint(CCPointZero);
        spBeganForGz->setPosition(ccp(0.0f, FONT_V - GUOZ_BG_V + 30.0f));
        pBgSprite->addChild(spBeganForGz);
    }
    CCSprite* spRemainForGz = CCSprite::createWithSpriteFrameName("gz_att_time.png");
    spRemainForGz->setAnchorPoint(CCPointZero);
    spRemainForGz->setPosition(ccp(335.0f, FONT_V - GUOZ_BG_V + 30.0f));
    pBgSprite->addChild(spRemainForGz);
    
    /*城池时间文字*/
    int hour = 0;
    int min  = 0;
    int sec = 0;
    sec = m_timeForBeganFight % 60;
    hour = (int)(m_timeForBeganFight / 3600);
    min  = (int)((m_timeForBeganFight % 3600) / 60);
    sprintf(buffer, "%02d:%02d:%02d", hour, min, sec);
    m_timeBeganLabel = CCStrokeLabel::create(buffer, "Arial", 23);
    m_timeBeganLabel->setAnchorPoint(CCPointZero);
    m_timeBeganLabel->setPosition(ccp(250.0f, FONT_V + 30.0f));
    pNode->addChild(m_timeBeganLabel);
    
    schedule(schedule_selector(GuoZhanLayer::_scheduleForTime), 1);
    /*剩余攻击城池次数*/
    sprintf(buffer, "%d", m_pBattle->getCwarTimes());
    CCStrokeLabel* tipsForChance = CCStrokeLabel::create(buffer, "Arial", 23);
    tipsForChance->setAnchorPoint(CCPointZero);
    tipsForChance->setPosition(ccp(570.0f, FONT_V + 27.0f));
    pNode->addChild(tipsForChance, 1, REMAIN_CHANCE_TAG);
    
    /*洛阳*/
    float allLy = 0;
    for (int i = 0; i < 3; i++) {
        allLy += m_pCityManager->getCityItem(1)->getCityScoreByIndex(i);
    }
    float allCa = 0;
    for (int i = 0; i < 3; i++) {
        allCa += m_pCityManager->getCityItem(2)->getCityScoreByIndex(i);
    }
    float allCl = 0;
    for (int i = 0; i < 3; i++) {
        allCl += m_pCityManager->getCityItem(3)->getCityScoreByIndex(i);
    }
    
    if (m_isFighting) {
        /*洛阳*/
        float greLy = ((m_pCityManager->getCityItem(1)->getCityScoreByIndex(2)) / allLy) * 100.0f;/*吴国比分*/
        float bluLy = ((m_pCityManager->getCityItem(1)->getCityScoreByIndex(1)) / allLy) * 100.0f;
        CCLog("吴国比分 = %f, 蜀国比分 = %f" ,greLy, bluLy);
        CCSprite* ly = _integralPro(greLy, bluLy);
        ly->setPosition(ccp(420.0f, 415.0f));
        pBgSprite->addChild(ly, 100);
        
        /*长安*/
        float greCa = ((m_pCityManager->getCityItem(2)->getCityScoreByIndex(2)) / allCa) * 100.0f;/*吴国比分*/
        float bluCa = ((m_pCityManager->getCityItem(2)->getCityScoreByIndex(1)) / allCa) * 100.0f;
        CCLog("吴国比分 = %f, 蜀国比分 = %f" ,greCa, bluCa);
        CCSprite* ca = _integralPro(greCa, bluCa);
        ca->setPosition(ccp(95.0f, 275.0f));
        pBgSprite->addChild(ca, 100);
        
        /*陈留*/
        float greCl = ((m_pCityManager->getCityItem(3)->getCityScoreByIndex(2)) / allCl) * 100.0f;/*吴国比分*/
        float bluCl = ((m_pCityManager->getCityItem(3)->getCityScoreByIndex(1)) / allCl) * 100.0f;
        CCLog("吴国比分 = %f, 蜀国比分 = %f" ,greCl, bluCl);
        CCSprite* cl = _integralPro(greCl, bluCl);
        cl->setPosition(ccp(355.0f, 195.0f));
        pBgSprite->addChild(cl, 100);
    }
    
    int typeLuoY = m_pCityManager->getCityItem(1)->getHolder();
    int typeChA  = m_pCityManager->getCityItem(2)->getHolder();
    int typeChL  = m_pCityManager->getCityItem(3)->getHolder();
    if (typeLuoY == CITY_OF_WEI) {
        pBgSprite->addChild(m_btnSprite[0][0] = CCSprite::createWithSpriteFrameName("gz_chA_blu.png"));
        m_btnSprite[0][0]->setAnchorPoint(CCPointZero);
        m_btnSprite[0][0]->setScale(0.7);
        pBgSprite->addChild(m_btnSprite[0][1] = CCSprite::createWithSpriteFrameName("gz_chA_blu.png"));
        m_btnSprite[0][1]->setAnchorPoint(CCPointZero);
        m_btnSprite[0][1]->setScale(0.7);
        pBgSprite->addChild(m_btnSprite[0][2] = CCSprite::createWithSpriteFrameName("gz_chA_blu.png"));
        m_btnSprite[0][2]->setAnchorPoint(CCPointZero);
        m_btnSprite[0][2]->setScale(0.7);
        m_btnSprite[0][0]->setUserObject(m_pCityManager->getCityItem(1));
        /*魏国旗帜*/
        CCSprite* p = CCSprite::createWithSpriteFrameName("gz_wei.png");
        p->setAnchorPoint(CCPointZero);
        p->setPosition(ccp(340.0f, 345.0f));
        pBgSprite->addChild(p);
        
        
    } else if (typeLuoY == CITY_OF_SHU) {
        pBgSprite->addChild(m_btnSprite[0][0] = CCSprite::createWithSpriteFrameName("gz_chA_red.png"));
        m_btnSprite[0][0]->setAnchorPoint(CCPointZero);
        m_btnSprite[0][0]->setScale(0.7);
        pBgSprite->addChild(m_btnSprite[0][1] = CCSprite::createWithSpriteFrameName("gz_chA_red.png"));
        m_btnSprite[0][1]->setAnchorPoint(CCPointZero);
        m_btnSprite[0][1]->setScale(0.7);
        pBgSprite->addChild(m_btnSprite[0][2] = CCSprite::createWithSpriteFrameName("gz_chA_red.png"));
        m_btnSprite[0][2]->setAnchorPoint(CCPointZero);
        m_btnSprite[0][2]->setScale(0.7);
        m_btnSprite[0][0]->setUserObject(m_pCityManager->getCityItem(1));
        /*蜀国旗帜*/
        CCSprite* p = CCSprite::createWithSpriteFrameName("gz_shu.png");
        p->setAnchorPoint(CCPointZero);
        p->setPosition(ccp(340.0f, 345.0f));
        pBgSprite->addChild(p);
    } else if (typeLuoY == CITY_OF_WU) {
        pBgSprite->addChild(m_btnSprite[0][0] = CCSprite::createWithSpriteFrameName("gz_chA_gre.png"));
        m_btnSprite[0][0]->setAnchorPoint(CCPointZero);
        m_btnSprite[0][0]->setScale(0.7);
        pBgSprite->addChild(m_btnSprite[0][1] = CCSprite::createWithSpriteFrameName("gz_chA_gre.png"));
        m_btnSprite[0][1]->setAnchorPoint(CCPointZero);
        m_btnSprite[0][1]->setScale(0.7);
        pBgSprite->addChild(m_btnSprite[0][2] = CCSprite::createWithSpriteFrameName("gz_chA_gre.png"));
        m_btnSprite[0][2]->setAnchorPoint(CCPointZero);
        m_btnSprite[0][2]->setScale(0.7);
        m_btnSprite[0][0]->setUserObject(m_pCityManager->getCityItem(1));
        /*吴国旗帜*/
        CCSprite* p = CCSprite::createWithSpriteFrameName("gz_wu.png");
        p->setAnchorPoint(CCPointZero);
        p->setPosition(ccp(340.0f, 345.0f));
        pBgSprite->addChild(p);
    } else/*中立*/ {
        pBgSprite->addChild(m_btnSprite[0][0] = CCSprite::createWithSpriteFrameName("gz_zhong_ca.png"));
        m_btnSprite[0][0]->setAnchorPoint(CCPointZero);
        m_btnSprite[0][0]->setScale(0.7);
        pBgSprite->addChild(m_btnSprite[0][1] = CCSprite::createWithSpriteFrameName("gz_zhong_ca.png"));
        m_btnSprite[0][1]->setAnchorPoint(CCPointZero);
        m_btnSprite[0][1]->setScale(0.7);
        pBgSprite->addChild(m_btnSprite[0][2] = CCSprite::createWithSpriteFrameName("gz_zhong_ca.png"));
        m_btnSprite[0][2]->setAnchorPoint(CCPointZero);
        m_btnSprite[0][2]->setScale(0.7);
        m_btnSprite[0][0]->setUserObject(m_pCityManager->getCityItem(1));
    }
    /*长安*/
    if (typeChA == CITY_OF_WEI) {
        pBgSprite->addChild(m_btnSprite[1][0] = CCSprite::createWithSpriteFrameName("gz_luoy_blu.png"));
        m_btnSprite[1][0]->setAnchorPoint(CCPointZero);
        m_btnSprite[1][0]->setScale(0.7);
        pBgSprite->addChild(m_btnSprite[1][1] = CCSprite::createWithSpriteFrameName("gz_luoy_blu.png"));
        m_btnSprite[1][1]->setAnchorPoint(CCPointZero);
        m_btnSprite[1][1]->setScale(0.7);
        pBgSprite->addChild(m_btnSprite[1][2] = CCSprite::createWithSpriteFrameName("gz_luoy_blu.png"));
        m_btnSprite[1][2]->setAnchorPoint(CCPointZero);
        m_btnSprite[1][2]->setScale(0.7);
        m_btnSprite[1][0]->setUserObject(m_pCityManager->getCityItem(2));
        /*魏国旗帜*/
        CCSprite* p = CCSprite::createWithSpriteFrameName("gz_wei.png");
        p->setAnchorPoint(CCPointZero);
        p->setPosition(ccp(20.0f, 210.0f));
        pBgSprite->addChild(p);
    } else if (typeChA == CITY_OF_SHU) {
        pBgSprite->addChild(m_btnSprite[1][0] = CCSprite::createWithSpriteFrameName("gz_luoy_red.png"));
        m_btnSprite[1][0]->setAnchorPoint(CCPointZero);
        m_btnSprite[1][0]->setScale(0.7);
        pBgSprite->addChild(m_btnSprite[1][1] = CCSprite::createWithSpriteFrameName("gz_luoy_red.png"));
        m_btnSprite[1][1]->setAnchorPoint(CCPointZero);
        m_btnSprite[1][1]->setScale(0.7);
        pBgSprite->addChild(m_btnSprite[1][2] = CCSprite::createWithSpriteFrameName("gz_luoy_red.png"));
        m_btnSprite[1][2]->setAnchorPoint(CCPointZero);
        m_btnSprite[1][2]->setScale(0.7);
        m_btnSprite[1][0]->setUserObject(m_pCityManager->getCityItem(2));
        /*蜀国旗帜*/
        CCSprite* p = CCSprite::createWithSpriteFrameName("gz_shu.png");
        p->setAnchorPoint(CCPointZero);
        p->setPosition(ccp(20.0f, 210.0f));
        pBgSprite->addChild(p);
    } else if (typeChA == CITY_OF_WU) {
        pBgSprite->addChild(m_btnSprite[1][0] = CCSprite::createWithSpriteFrameName("gz_luoy_gre.png"));
        m_btnSprite[1][0]->setAnchorPoint(CCPointZero);
        m_btnSprite[1][0]->setScale(0.7);
        pBgSprite->addChild(m_btnSprite[1][1] = CCSprite::createWithSpriteFrameName("gz_luoy_gre.png"));
        m_btnSprite[1][1]->setAnchorPoint(CCPointZero);
        m_btnSprite[1][1]->setScale(0.7);
        pBgSprite->addChild(m_btnSprite[1][2] = CCSprite::createWithSpriteFrameName("gz_luoy_gre.png"));
        m_btnSprite[1][2]->setAnchorPoint(CCPointZero);
        m_btnSprite[1][2]->setScale(0.7);
        m_btnSprite[1][0]->setUserObject(m_pCityManager->getCityItem(2));
        /*吴国旗帜*/
        CCSprite* p = CCSprite::createWithSpriteFrameName("gz_wu.png");
        p->setAnchorPoint(CCPointZero);
        p->setPosition(ccp(20.0f, 210.0f));
        pBgSprite->addChild(p);
    } else/*中立*/ {
        pBgSprite->addChild(m_btnSprite[1][0] = CCSprite::createWithSpriteFrameName("gz_zhong_ly.png"));
        m_btnSprite[1][0]->setAnchorPoint(CCPointZero);
        m_btnSprite[1][0]->setScale(0.7);
        pBgSprite->addChild(m_btnSprite[1][1] = CCSprite::createWithSpriteFrameName("gz_zhong_ly.png"));
        m_btnSprite[1][1]->setAnchorPoint(CCPointZero);
        m_btnSprite[1][1]->setScale(0.7);
        pBgSprite->addChild(m_btnSprite[1][2] = CCSprite::createWithSpriteFrameName("gz_zhong_ly.png"));
        m_btnSprite[1][2]->setAnchorPoint(CCPointZero);
        m_btnSprite[1][2]->setScale(0.7);
        m_btnSprite[1][0]->setUserObject(m_pCityManager->getCityItem(2));
    }
    /*陈留*/
    if (typeChL == CITY_OF_WEI) {
        pBgSprite->addChild(m_btnSprite[2][0] = CCSprite::createWithSpriteFrameName("gz_chl_blu.png"));
        m_btnSprite[2][0]->setAnchorPoint(CCPointZero);
        m_btnSprite[2][0]->setScale(0.7);
        pBgSprite->addChild(m_btnSprite[2][1] = CCSprite::createWithSpriteFrameName("gz_chl_blu.png"));
        m_btnSprite[2][1]->setAnchorPoint(CCPointZero);
        m_btnSprite[2][1]->setScale(0.7);
        pBgSprite->addChild(m_btnSprite[2][2] = CCSprite::createWithSpriteFrameName("gz_chl_blu.png"));
        m_btnSprite[2][2]->setAnchorPoint(CCPointZero);
        m_btnSprite[2][2]->setScale(0.7);
        m_btnSprite[2][0]->setUserObject(m_pCityManager->getCityItem(3));
        /*魏国旗帜*/
        CCSprite* p = CCSprite::createWithSpriteFrameName("gz_wei.png");
        p->setAnchorPoint(CCPointZero);
        p->setPosition(ccp(280.0f, 125.0f));
        pBgSprite->addChild(p);
    } else if (typeChL == CITY_OF_SHU) {
        pBgSprite->addChild(m_btnSprite[2][0] = CCSprite::createWithSpriteFrameName("gz_chl_red.png"));
        m_btnSprite[2][0]->setAnchorPoint(CCPointZero);
        m_btnSprite[2][0]->setScale(0.7);
        pBgSprite->addChild(m_btnSprite[2][1] = CCSprite::createWithSpriteFrameName("gz_chl_red.png"));
        m_btnSprite[2][1]->setAnchorPoint(CCPointZero);
        m_btnSprite[2][1]->setScale(0.7);
        pBgSprite->addChild(m_btnSprite[2][2] = CCSprite::createWithSpriteFrameName("gz_chl_red.png"));
        m_btnSprite[2][2]->setAnchorPoint(CCPointZero);
        m_btnSprite[2][2]->setScale(0.7);
        m_btnSprite[2][0]->setUserObject(m_pCityManager->getCityItem(3));
        /*蜀国旗帜*/
        CCSprite* p = CCSprite::createWithSpriteFrameName("gz_shu.png");
        p->setAnchorPoint(CCPointZero);
        p->setPosition(ccp(280.0f, 125.0f));
        pBgSprite->addChild(p);
    } else if (typeChL == CITY_OF_WU) {
        pBgSprite->addChild(m_btnSprite[2][0] = CCSprite::createWithSpriteFrameName("gz_chl_gre.png"));
        m_btnSprite[2][0]->setAnchorPoint(CCPointZero);
        m_btnSprite[2][0]->setScale(0.7);
        pBgSprite->addChild(m_btnSprite[2][1] = CCSprite::createWithSpriteFrameName("gz_chl_gre.png"));
        m_btnSprite[2][1]->setAnchorPoint(CCPointZero);
        m_btnSprite[2][1]->setScale(0.7);
        pBgSprite->addChild(m_btnSprite[2][2] = CCSprite::createWithSpriteFrameName("gz_chl_gre.png"));
        m_btnSprite[2][2]->setAnchorPoint(CCPointZero);
        m_btnSprite[2][2]->setScale(0.7);
        m_btnSprite[2][0]->setUserObject(m_pCityManager->getCityItem(3));
        /*吴国旗帜*/
        CCSprite* p = CCSprite::createWithSpriteFrameName("gz_wu.png");
        p->setAnchorPoint(CCPointZero);
        p->setPosition(ccp(280.0f, 125.0f));
        pBgSprite->addChild(p);
    } else/*中立*/ {
        pBgSprite->addChild(m_btnSprite[2][0] = CCSprite::createWithSpriteFrameName("gz_zhong_cl.png"));
        m_btnSprite[2][0]->setAnchorPoint(CCPointZero);
        m_btnSprite[2][0]->setScale(0.7);
        pBgSprite->addChild(m_btnSprite[2][1] = CCSprite::createWithSpriteFrameName("gz_zhong_cl.png"));
        m_btnSprite[2][1]->setAnchorPoint(CCPointZero);
        m_btnSprite[2][1]->setScale(0.7);
        pBgSprite->addChild(m_btnSprite[2][2] = CCSprite::createWithSpriteFrameName("gz_zhong_cl.png"));
        m_btnSprite[2][2]->setAnchorPoint(CCPointZero);
        m_btnSprite[2][2]->setScale(0.7);
        m_btnSprite[2][0]->setUserObject(m_pCityManager->getCityItem(3));
    }
    
    for (int i = 0; i < 3; ++i) {
        CityItem* pCityItem = (CityItem*)m_btnSprite[i][0]->getUserObject();
        CCStrokeLabel* pCityNm = CCStrokeLabel::create(pCityItem->getName().c_str(), "Arial", 23);
        pCityNm->setAnchorPoint(CCPointZero);
        if (i == 0) {
            pCityNm->setPosition(ccp(460.0f, 398.0f));
        } else if (i == 1) {
            pCityNm->setPosition(ccp(135.0f, 268.0f));
        } else if (i == 2) {
            pCityNm->setPosition(ccp(395.0f, 185.0f));
        }
        pNode->addChild(pCityNm);
    }
    
    /*初始化按钮位置*/
    _initBtn();
}

void GuoZhanLayer::_initBtn() {
    for (int i = 0; i < CITY_COUNT; ++i) {
        m_btnSprite[i][0]->setVisible(true);
        m_btnSprite[i][0]->setPosition(m_btnPosition[i][0]);
        
//        CityItem* pCityItem = (CityItem*)m_btnSprite[i][0]->getUserObject();
//        CCAssert(pCityItem, "invalid cityItem");
        
        m_btnSprite[i][1]->setVisible(false);
        m_btnSprite[i][1]->setPosition(m_btnPosition[i][0]);
        m_btnSprite[i][2]->setVisible(false);
        m_btnSprite[i][2]->setPosition(m_btnPosition[i][0]);
    }
}

void GuoZhanLayer::_onBtnSelected(int idx) {
    unsigned int cityId = 0;
    switch (idx) {
        case 1:
        {
            cityId = 1;
            m_pBattle->setSelectCity(1);
        }
            break;
        case 2:
        {
            cityId = 2;
            m_pBattle->setSelectCity(2);
        }
            break;
        case 3:
        {
            cityId = 3;
            m_pBattle->setSelectCity(3);
        }
            break;
        default:
            break;
    }
    
    if (cityId > 0) {
        NetConnection* pNetConnection = NetConnection::getInstance();
        char szPostBuffer[MAX_POSTTO_SERVER_BUFFER_SIZE];
        memset(szPostBuffer, '\0', MAX_POSTTO_SERVER_BUFFER_SIZE);
        
        sprintf(szPostBuffer, "c=cwar&a=city&city=%d&uid=%s", cityId,
                GamePlayer::getInstance()->getUid().c_str());
        char szHttpTag[] = HTTP_REQUEST_ENTERCITY_TAG;
        CCLog("post 进入城池 = %s", szPostBuffer);
        pNetConnection->commitPostRequestData(szPostBuffer, szHttpTag);
    }
}

/*public:更新函数*/
void GuoZhanLayer::updateAllOfGuoZhanLayer() {
}

// 初始化层位置
void GuoZhanLayer::initPosition(){
    CCSprite* parent = static_cast<CCSprite*>(m_pSBN->getChildByTag(GUOZ_BG_TAG));
    parent->setPosition(INIT_LAYER_POSITION);
    CCNode* pNode = (CCNode*)this->getChildByTag(NODE_TAG);
    pNode->setPosition(INIT_LAYER_POSITION);
}
// 层移出
void GuoZhanLayer::moveOutTouchUICompent(){
    CCSprite* parent = static_cast<CCSprite*>(m_pSBN->getChildByTag(GUOZ_BG_TAG));
    parent->setPosition(INIT_LAYER_POSITION);
    CCNode* pNode = (CCNode*)this->getChildByTag(NODE_TAG);
    pNode->setPosition(INIT_LAYER_POSITION);
    
    /*当前为PVE状态*/
    m_pBattle->setBattlePkMode(BPM_PVE);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_MOVEGOBATTLE_STRING);
    
    this->removeFromParent();
    CCLog("国战地图层 从主场景移除");
}

// 层移入
void GuoZhanLayer::restoreTouchUICompent(){
    CCSprite* parent = static_cast<CCSprite*>(m_pSBN->getChildByTag(GUOZ_FADEIN_TAG));
    parent->runAction(CCSequence::create(CCFadeIn::create(DURATION_TIME), CCCallFunc::create(this, callfunc_selector(GuoZhanLayer::_delayShow)), NULL));
    /*当前为PVP状态*/
    m_pBattle->setBattlePkMode(BPM_PVP);
    m_pBattle->setCWar(true);
}
void GuoZhanLayer::_delayShow() {
    CCSprite* p = static_cast<CCSprite*>(m_pSBN->getChildByTag(GUOZ_FADEIN_TAG));
    p->setOpacity(0);
    CCSprite* parent = static_cast<CCSprite*>(m_pSBN->getChildByTag(GUOZ_BG_TAG));
    parent->setPosition(ccp(0.0f, m_pGameState->getBottomOffset() + GUOZ_BG_V));
    CCNode* pNode = (CCNode*)this->getChildByTag(NODE_TAG);
    pNode->setPosition(ccp(0, m_pGameState->getBottomOffset()));
}

//
void GuoZhanLayer::registerWithTouchDispatcher(void){
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, false);
}
bool GuoZhanLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent){
    _initBtn();
    
    m_touchBeganBtnIdx = -1;
    CCSprite* pBg = static_cast<CCSprite*>(m_pSBN->getChildByTag(GUOZ_BG_TAG));
    CCPoint touchPoint = pBg->convertTouchToNodeSpace(pTouch);
    for (int i = 0; i < CITY_COUNT; ++i) {
        if (m_btnEnable[i] && m_btnSprite[i][0]->boundingBox().containsPoint(touchPoint)) {
            m_touchBeganBtnIdx = i;
            
            m_btnSprite[i][0]->setVisible(false);/*普通*/
            m_btnSprite[i][1]->setVisible(true);/*选中*/
            CCPoint btnPos = m_btnPosition[i][0];
            btnPos.x += BTN_HIGHLIGHT_OFFSET_H;
            btnPos.y += BTN_HIGHLIGHT_OFFSET_V;
            
            m_btnSprite[i][1]->setPosition(btnPos);
            
            break;
        }
    }
    
    return true;
}
void GuoZhanLayer::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent){
    _initBtn();
    
    CCSprite* pBg = static_cast<CCSprite*>(m_pSBN->getChildByTag(GUOZ_BG_TAG));
    CCPoint touchPoint = pBg->convertTouchToNodeSpace(pTouch);
    for (int i = 0; i < CITY_COUNT; ++i) {
        if (m_btnEnable[i] && m_btnSprite[i][0]->boundingBox().containsPoint(touchPoint)) {
            m_btnSprite[i][0]->setVisible(false);/*普通*/
            m_btnSprite[i][1]->setVisible(true);/*选中*/
            CCPoint btnPos = m_btnPosition[i][0];
            btnPos.x += BTN_HIGHLIGHT_OFFSET_H;
            btnPos.y += BTN_HIGHLIGHT_OFFSET_V;
            
            m_btnSprite[i][1]->setPosition(btnPos);
            
            break;
        }
    }
}
void GuoZhanLayer::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent){
    CCSprite* pBg = static_cast<CCSprite*>(m_pSBN->getChildByTag(GUOZ_BG_TAG));
    CCPoint touchPoint = pBg->convertTouchToNodeSpace(pTouch);
    int selectIdx = -1;
    for (int i = 0; i < CITY_COUNT; ++i) {
        if (m_btnEnable[i] && m_btnSprite[i][0]->boundingBox().containsPoint(touchPoint)) {
            selectIdx = i + 1;
            break;
        }
    }
    
    _initBtn();
    
    if (selectIdx > 0) {
        _onBtnSelected(selectIdx);
    }
}
void GuoZhanLayer::ccTouchCancelled(CCTouch*, CCEvent*){_initBtn();}

void GuoZhanLayer::_scheduleForTime() {
    char buffer[32];
    int hour = 0;
    int min  = 0;
    int sec = 0;
    sec = m_timeForBeganFight % 60;
    hour = (int)(m_timeForBeganFight / 3600);
    min  = (int)((m_timeForBeganFight % 3600) / 60);
    sprintf(buffer, "%02d:%02d:%02d", hour, min, sec);
    m_timeBeganLabel->setString(buffer);
    if (--m_timeForBeganFight <= 0) {
        m_timeForBeganFight = 0;
        unschedule(schedule_selector(GuoZhanLayer::_scheduleForTime));
    }
}

/*玩家积分比例*/
CCSprite* GuoZhanLayer::_integralPro(const float green, const float blue) {
    if (green < 0.0f || green > 100.0f) {
        return NULL;
    }
    if (blue < 0.0f || blue > 100.0f) {
        return NULL;
    }
    
    CCSprite* bg = CCSprite::createWithSpriteFrameName("gz_box_small.png");
    bg->setAnchorPoint(CCPointZero);
    CCSprite* bgPre = CCSprite::createWithSpriteFrameName("gz_box_small.png");
    bgPre->setAnchorPoint(CCPointZero);
    bgPre->setPosition(ccp(0.0f, 0.0f));
    bg->addChild(bgPre, 4);
    
    CCSprite* pGreen = CCSprite::createWithSpriteFrameName("gz_s_gre.png");
    pGreen->setAnchorPoint(CCPointZero);
    pGreen->setPosition(ccp(8.0f, 5.0f));
    pGreen->setScaleX(green / 100.0f);
    bg->addChild(pGreen, 3);
    
    CCSprite* pBlue = CCSprite::createWithSpriteFrameName("gz_s_blu.png");
    pBlue->setAnchorPoint(CCPointZero);
    pBlue->setPosition(ccp(8.0f, 5.0f));
    pBlue->setScaleX((green + blue) / 100.0f);
    bg->addChild(pBlue, 2);
    
    CCSprite* pOrange = CCSprite::createWithSpriteFrameName("gz_s_ora.png");
    pOrange->setAnchorPoint(CCPointZero);
    pOrange->setPosition(ccp(8.0f, 5.0f));
    bg->addChild(pOrange, 1);
    
    return bg;
}