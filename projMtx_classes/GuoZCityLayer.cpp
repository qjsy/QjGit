//
//  GuoZCityLayer.cpp
//  hero
//
//  Created by 钱剑 on 14-2-25.
//
//

#include "customMessage.h"
#include "GuoZCityLayer.h"
#include "GameState.h"
#include "Battle.h"
#include "NetConnection.h"
#include "GamePlayer.h"
#include "CityManager.h"
#include "CityItem.h"
#include "CityFunctionItem.h"
#include "CCStrokeLabel.h"
#include "MessageBox.h"

//file
#define GUOZ_IMAGE_FILE             "guozhan.pvr.ccz"
#define GUOZ_LIST_FILE              "guozhan.plist"

#define TYPE_FOR_ING                1                   //国战中
#define TYPE_FOR_WAIT               2                   //等待国战
#define TYPE_IN                     1                       // 进入
#define TYPE_OUT                    2                       // 出
#define GOLD_COLOR                  ccc3(218, 165, 105)
#define YELLOW_COLOR                ccc3(255, 255, 0)
#define GREEN_COLOR                 ccc3(0, 255, 0)
#define WHITE_COLOR                 ccc3(255, 255, 255)
#define DURATION_TIME               0.3
// tag
#define GUOZ_CITY_BG_TAG            1
#define NODE_TAG                    2
#define TIPMENU_LEFT_TAG            3                   // 左边tipMenu容器
#define TIPMENU_RIGHT_TAG           4                   // 右边tipMenu容器
#define TIPMENU_MID_TAG             5                   // 中间tipMenu容器
#define CHENGQ_FONT_TAG             6                   // 城墙
#define TOUSHI_FONT_TAG             7                   // 投石车
#define SHOUJUN_FONT_TAG            8                   // 守军
#define GUWU_FONT_TAG               9                   // 鼓舞加成
#define GUWUNEEDGOLD_FONT_TAG       10                  // 鼓舞所需元宝


// value
#define DURATION_TIME               0.3
#define INIT_LAYER_POSITION         ccp(0.0f, -850.0f)
#define GUOZ_CITY_BG_V              122.0f
#define TIPMENU_V                   750.0f              // tipMenu竖直高度



/*构造函数*/
GuoZCityLayer::GuoZCityLayer()
:m_pGameState(GameState::getInstance())
,m_pBattle(Battle::getInstance())
,m_pNetConnection(NetConnection::getInstance())
,m_pCityManager(CityManager::getInstance())
,m_pGamePlayer(GamePlayer::getInstance())
,m_isDefending(true)
,m_coolTimeLabel(NULL)
,m_isInList(1)
,m_minTime(0)
,m_fourState(5)
,m_thisTouch(true)
,m_typeOfAtkOrDef(0)
,m_defTime(0)
{
    memset(m_btn, '\0', sizeof(m_btn));
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(GUOZ_LIST_FILE, GUOZ_IMAGE_FILE);
}
/*析构函数*/
GuoZCityLayer::~GuoZCityLayer(){
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(GUOZ_LIST_FILE);
    CCTextureCache* pTextureCache = CCTextureCache::sharedTextureCache();
    pTextureCache->removeTextureForKey(GUOZ_IMAGE_FILE);
    /*没有加入批处理的图片*/
    pTextureCache->removeTextureForKey("CM_left.png");
    pTextureCache->removeTextureForKey("CM_right.png");
    pTextureCache->removeTextureForKey("CM_mid.png");
    pTextureCache->removeTextureForKey("LU_return.png");
    pTextureCache->removeTextureForKey("gz_kuang.png");
    pTextureCache->removeTextureForKey("gz_grePro.png");
    pTextureCache->removeTextureForKey("gz_oraPro.png");
    pTextureCache->removeTextureForKey("gz_bluPro.png");
}
/*初始化函数*/
bool GuoZCityLayer::init() {
    if (!CCLayer::init()) {
        return false;
    }
    this->setTouchEnabled(true);
    this->addChild(m_pSBN = CCSpriteBatchNode::create(GUOZ_IMAGE_FILE), 3);
    m_typeOfAtkOrDef = m_pGameState->getAwardType();
    m_listTime = m_pGameState->getRemainTimes(TIME_GZDEF);
    m_defTime = m_pBattle->getDefTime();
    m_timeForCooling = m_pGameState->getRemainTimes(TIME_GZCOOL);
    /*大地图背景*/
    CCSprite* bg = CCSprite::createWithSpriteFrameName("gz_cityBg.png");
    bg->setAnchorPoint(CCPointZero);
    bg->setPosition(ccp(0.0f, m_pGameState->getBottomOffset() + 125.0f));
    m_pSBN->addChild(bg);
    /**UI**/
    _initUi();
    /*初始化按钮*/
    _initAniAtkOrDef();
    
    
    return true;
}
/* 初始化UI组件 */
void GuoZCityLayer::_initUi(){
    if (m_pSBN->getChildByTag(GUOZ_CITY_BG_TAG)) {
        m_pSBN->removeChildByTag(GUOZ_CITY_BG_TAG);
    }
    // 国战是否开始
    unsigned int type = (m_pGameState->getIsBeganGZ() ? TYPE_FOR_ING : TYPE_FOR_WAIT);
    m_isDefending = (2 == m_pBattle->getCwarCityType());/*2是防守方*/
    
    if (TYPE_FOR_ING == type)/*国战中*/ {
        CCNode* pNode = CCNode::create();
        pNode->setAnchorPoint(CCPointZero);
        pNode->setPosition(ccp(0.0f, m_pGameState->getBottomOffset()));
        addChild(pNode, 4, NODE_TAG);
        
        CCSprite* pBgSprite = CCSprite::createWithSpriteFrameName("gz_bg.png");
        pBgSprite->setAnchorPoint(CCPointZero);
        pBgSprite->setPosition(ccp(0.0f, m_pGameState->getBottomOffset() + GUOZ_CITY_BG_V));
        m_pSBN->addChild(pBgSprite, 1, GUOZ_CITY_BG_TAG);
        // 上下款
        CCSprite* pXia = CCSprite::createWithSpriteFrameName("gz_xia.png");
        pXia->setAnchorPoint(CCPointZero);
        pXia->setPosition(ccp(0.0f, -10.0f));
        pBgSprite->addChild(pXia);
        CCSprite* pShang = CCSprite::createWithSpriteFrameName("gz_shang.png");
        pShang->setAnchorPoint(CCPointZero);
        pShang->setPosition(ccp(0.0f, 585.0f));
        pBgSprite->addChild(pShang);
        /*判断当前城池*/
        CCSprite* pCityDiBg = CCSprite::createWithSpriteFrameName("gz_cityDiBg.png");
        pCityDiBg->setAnchorPoint(CCPointZero);
        pCityDiBg->setPosition(ccp(35.0f, 425.0f));
        pBgSprite->addChild(pCityDiBg);
        CCSprite* pCityDi = CCSprite::createWithSpriteFrameName("gz_cityDi.png");
        pCityDi->setAnchorPoint(CCPointZero);
        pCityDi->setPosition(ccp(0.0f, 455.0f));
        pBgSprite->addChild(pCityDi);
        int camp = 0;
        if (m_pBattle->getSelectCity() == 1) {
            camp = m_pCityManager->getCityItem(1)->getHolder();
            _checkForWitch(camp, 1);
            /*城池名字*/
            CCLabelTTF* pName = CCLabelTTF::create("洛阳", "Arial", 30);
            pName->setAnchorPoint(ccp(0, 0));
            pName->setPosition(ccp(400.0f, 695.0f));
            pNode->addChild(pName);
        } else if (m_pBattle->getSelectCity() == 2) {
            camp = m_pCityManager->getCityItem(2)->getHolder();
            _checkForWitch(camp, 2);
            /*城池名字*/
            CCLabelTTF* pName = CCLabelTTF::create("长安", "Arial", 30);
            pName->setAnchorPoint(ccp(0, 0));
            pName->setPosition(ccp(400.0f, 695.0f));
            pNode->addChild(pName);
        } else if (m_pBattle->getSelectCity() == 3) {
            camp = m_pCityManager->getCityItem(3)->getHolder();
            _checkForWitch(camp, 3);
            /*城池名字*/
            CCLabelTTF* pName = CCLabelTTF::create("陈留", "Arial", 30);
            pName->setAnchorPoint(ccp(0, 0));
            pName->setPosition(ccp(400.0f, 695.0f));
            pNode->addChild(pName);
        }
        /*势力标志*/
        int shilitype = m_pCityManager->getCityItem(m_pBattle->getSelectCity())->getHolder();
        if (shilitype == 1) {
            CCSprite* pCamp = CCSprite::createWithSpriteFrameName("gz_wei.png");
            pCamp->setAnchorPoint(CCPointZero);
            pCamp->setPosition(ccp(320.0f, 555.0f));
            pBgSprite->addChild(pCamp);
        } else if (shilitype == 2) {
            CCSprite* pCamp = CCSprite::createWithSpriteFrameName("gz_shu.png");
            pCamp->setAnchorPoint(CCPointZero);
            pCamp->setPosition(ccp(320.0f, 555.0f));
            pBgSprite->addChild(pCamp);
        } else {
            CCSprite* pCamp = CCSprite::createWithSpriteFrameName("gz_wu.png");
            pCamp->setAnchorPoint(CCPointZero);
            pCamp->setPosition(ccp(320.0f, 555.0f));
            pBgSprite->addChild(pCamp);
        }
        
        /*玩家战斗积分比例*/
        float grePercent = 0.0f;
        float bluPercent = 0.0f;
        if (m_pBattle->getSelectCity() == 1) {
            float allLy = 0;
            for (int i = 0; i < 3; i++) {
                allLy += m_pCityManager->getCityItem(1)->getCityScoreByIndex(i);
            }
            grePercent = ((m_pCityManager->getCityItem(1)->getCityScoreByIndex(2)) / allLy) * 100.0f;/*吴国比分*/
            bluPercent = grePercent + ((m_pCityManager->getCityItem(1)->getCityScoreByIndex(1)) / allLy) * 100.0f;
        } else if (m_pBattle->getSelectCity() == 2) {
            float allCa = 0;
            for (int i = 0; i < 3; i++) {
                allCa += m_pCityManager->getCityItem(2)->getCityScoreByIndex(i);
            }
            grePercent = ((m_pCityManager->getCityItem(2)->getCityScoreByIndex(2)) / allCa) * 100.0f;/*吴国比分*/
            bluPercent = grePercent + ((m_pCityManager->getCityItem(2)->getCityScoreByIndex(1)) / allCa) * 100.0f;
        } else {
            float allCl = 0;
            for (int i = 0; i < 3; i++) {
                allCl += m_pCityManager->getCityItem(3)->getCityScoreByIndex(i);
            }
            grePercent = ((m_pCityManager->getCityItem(2)->getCityScoreByIndex(2)) / allCl) * 100.0f;/*吴国比分*/
            bluPercent = grePercent + ((m_pCityManager->getCityItem(2)->getCityScoreByIndex(1)) / allCl) * 100.0f;
        }
        
        CCSprite* pKuang = CCSprite::createWithSpriteFrameName("gz_kuang.png");
        pKuang->setAnchorPoint(CCPointZero);
        pKuang->setPosition(ccp(3.0f, 533.0f));
        pNode->addChild(pKuang, 4);
        CCProgressTimer* pGre = CCProgressTimer::create
        (CCSprite::createWithSpriteFrameName("gz_grePro.png"));
        pGre->setType(kCCProgressTimerTypeBar);
        pGre->setMidpoint(ccp(0, 0));
        pGre->setBarChangeRate(ccp(1, 0));
        
        pGre->setPercentage(grePercent);
        pGre->setAnchorPoint(CCPointZero);
        pGre->setPosition(ccp(23.0f, 543.0f));
        pNode->addChild(pGre, 3);
        CCProgressTimer* pBlu = CCProgressTimer::create
        (CCSprite::createWithSpriteFrameName("gz_bluPro.png"));
        pNode->addChild(pBlu, 2);
        pBlu->setType(kCCProgressTimerTypeBar);
        pBlu->setMidpoint(ccp(0, 0));
        pBlu->setBarChangeRate(ccp(1, 0));
        
        pBlu->setPercentage(bluPercent);
        pBlu->setAnchorPoint(CCPointZero);
        pBlu->setPosition(ccp(23.0f, 543.0f));
        CCProgressTimer* pOra = CCProgressTimer::create
        (CCSprite::createWithSpriteFrameName("gz_oraPro.png"));
        pNode->addChild(pOra, 1);
        pOra->setType(kCCProgressTimerTypeBar);
        pOra->setMidpoint(ccp(0, 0));
        pOra->setBarChangeRate(ccp(1, 0));
        pOra->setPercentage(100.0f);
        pOra->setAnchorPoint(CCPointZero);
        pOra->setPosition(ccp(23.0f, 543.0f));
        /*守军*/
        CCSprite* pDefense = CCSprite::createWithSpriteFrameName("gz_font_defenders.png");
        pDefense->setAnchorPoint(CCPointZero);
        pDefense->setPosition(ccp(80.0f, 380.0f));
        pBgSprite->addChild(pDefense);
        /*城墙*/
        CCSprite* pChenq = CCSprite::createWithSpriteFrameName("gz_rampart.png");
        pChenq->setAnchorPoint(CCPointZero);
        pChenq->setPosition(ccp(245.0f, 535.0f));
        pBgSprite->addChild(pChenq);
        /*投石车*/
        CCSprite* pTous = CCSprite::createWithSpriteFrameName("gz_font_tous.png");
        pTous->setAnchorPoint(CCPointZero);
        pTous->setPosition(ccp(220.0f, 495.0f));
        pBgSprite->addChild(pTous, 5);
        /*横条*/
        CCSprite* pHengt = CCSprite::createWithSpriteFrameName("gz_hengTiao.png");
        pHengt->setAnchorPoint(CCPointZero);
        pHengt->setPosition(ccp(0.0f, 360.0f));
        pBgSprite->addChild(pHengt);
        /*4个框*/
        for (int i = 0; i < 5; ++i) {
            CCSprite* pKunagInfo = CCSprite::createWithSpriteFrameName("gz_zhanlueBg.png");
            pKunagInfo->setAnchorPoint(CCPointZero);
            pKunagInfo->setScale(0.8);
            pKunagInfo->setPosition(ccp(320.0f + i * 60.0f, 425.0f));
            pBgSprite->addChild(pKunagInfo);
        }
        // 所选城池的功能预览
        CityItem* pCityItem = m_pCityManager->getCityItem(m_pBattle->getSelectCity());
        for (int i = 0; i < pCityItem->getNumberOfCityFuncs(); ++i) {
            CityFunctionItem* pCityFunctionItem = pCityItem->getCityFuncItemByIndex(i);
            
            CCSprite* pFIcon = CCSprite::createWithSpriteFrameName(pCityFunctionItem->getI().c_str());
            pFIcon->setAnchorPoint(ccp(0, 0));
            pFIcon->setPosition(ccp(318.0f + i * 60.0f, 430.0f));
            pFIcon->setScale(0.8);
            pBgSprite->addChild(pFIcon);
            
            CCStrokeLabel* pFName = CCStrokeLabel::create(pCityFunctionItem->getName().c_str(), "Arial", 15);
            pFName->setAnchorPoint(CCPointZero);
            pFName->setPosition(ccp(315.0f + i * 60.0f, 520.0f));
            pFName->setColor(YELLOW_COLOR);
            pNode->addChild(pFName);
        }
        
        /*鼓舞加成*/
        CCSprite* pCourage = CCSprite::createWithSpriteFrameName("gz_font_courages.png");
        pCourage->setAnchorPoint(CCPointZero);
        pCourage->setPosition(ccp(320.0f, 345.0f));
        pBgSprite->addChild(pCourage);
        /*元宝消耗*/
        CCSprite* pCourageGold = CCSprite::createWithSpriteFrameName("gz_gem.png");
        pCourageGold->setAnchorPoint(CCPointZero);
        pCourageGold->setPosition(ccp(375.0f, 220.0f));
        pBgSprite->addChild(pCourageGold);
        /*鼓舞文字*/
        CCSprite* pBtnFontCour = CCSprite::createWithSpriteFrameName("gz_font_courage.png");
        pBtnFontCour->setAnchorPoint(CCPointZero);
        pBtnFontCour->setPosition(ccp(420.0f, 253.0f));
        pBgSprite->addChild(pBtnFontCour, 3);
        /*分割线*/
        CCSprite* pFengg = CCSprite::createWithSpriteFrameName("gz_feng.png");
        pFengg->setAnchorPoint(CCPointZero);
        pFengg->setPosition(ccp(330.0f, 223.0f));
        pBgSprite->addChild(pFengg);
        
        /*防御文字*/
        m_pDefFont = CCSprite::createWithSpriteFrameName("gz_font_defense.png");
        m_pDefFont->setAnchorPoint(CCPointZero);
        m_pDefFont->setPosition(ccp(435.0f, 115.0f));
        m_pDefFont->setVisible(false);
        pBgSprite->addChild(m_pDefFont, 3);
        /*攻击文字*/
        m_pAttFont = CCSprite::createWithSpriteFrameName("gz_font_att.png");
        m_pAttFont->setAnchorPoint(CCPointZero);
        m_pAttFont->setPosition(ccp(435.0f, 118.0f));
        m_pAttFont->setVisible(false);
        pBgSprite->addChild(m_pAttFont, 3);
        /*战斗冷却时间*/
        CCSprite* pCooling = CCSprite::createWithSpriteFrameName("gz_font_cooling.png");
        pCooling->setAnchorPoint(CCPointZero);
        pCooling->setPosition(ccp(315.0f, 20.0f));
        pCooling->setScale(0.9);
        pBgSprite->addChild(pCooling);
        
        /*战报*/
        CCSprite* pReport = CCSprite::createWithSpriteFrameName("gz_talk_kuang.png");
        pReport->setAnchorPoint(CCPointZero);
        pReport->setPosition(ccp(20.0f, 20.0f));
        pBgSprite->addChild(pReport);
        //鼓舞按钮
        m_btn[0][0] = CCSprite::createWithSpriteFrameName("gz_btn.png");
        m_btn[0][0]->setAnchorPoint(CCPointZero);
        m_btn[0][0]->setPosition(ccp(340.0f, 250.0f));
        pBgSprite->addChild(m_btn[0][0]);
        m_btn[0][1] = CCSprite::createWithSpriteFrameName("gz_btn_s.png");
        m_btn[0][1]->setVisible(false);
        m_btn[0][1]->setAnchorPoint(CCPointZero);
        m_btn[0][1]->setPosition(ccp(340.0f, 250.0f));
        pBgSprite->addChild(m_btn[0][1]);
        /*战斗冷却重置*/
        m_btn[2][0] = CCSprite::createWithSpriteFrameName("gz_oraReset.png");
        m_btn[2][0]->setAnchorPoint(CCPointZero);
        m_btn[2][0]->setPosition(ccp(590.0f, 23.0f));
        pBgSprite->addChild(m_btn[2][0]);
        m_btn[2][1] = CCSprite::createWithSpriteFrameName("gz_oraReset_s.png");
        m_btn[2][1]->setAnchorPoint(CCPointZero);
        m_btn[2][1]->setVisible(false);
        m_btn[2][1]->setPosition(ccp(590.0f, 23.0f));
        pBgSprite->addChild(m_btn[2][1], 2);
        
        /*是否处于防御或者攻击状态*/
        if (m_isDefending)/*防守状态*/ {
            m_pDefFont->setVisible(true);
            m_pAttFont->setVisible(false);
            //防御按钮
            m_btn[1][0] = CCSprite::createWithSpriteFrameName("gz_defense.png");
            m_btn[1][0]->setAnchorPoint(CCPointZero);
            m_btn[1][0]->setPosition(ccp(385.0f, 40.0f));
            pBgSprite->addChild(m_btn[1][0], 2);
            m_btn[1][1] = CCSprite::createWithSpriteFrameName("gz_defense_s.png");
            m_btn[1][1]->setAnchorPoint(CCPointZero);
            m_btn[1][1]->setVisible(false);
            m_btn[1][1]->setPosition(ccp(385.0f, 40.0f));
            pBgSprite->addChild(m_btn[1][1]);
        } else {
            m_pDefFont->setVisible(false);
            m_pAttFont->setVisible(true);
            //攻击按钮
            m_btn[1][0] = CCSprite::createWithSpriteFrameName("gz_att.png");
            m_btn[1][0]->setAnchorPoint(CCPointZero);
            m_btn[1][0]->setPosition(ccp(385.0f, 45.0f));
            pBgSprite->addChild(m_btn[1][0], 2);
            m_btn[1][1] = CCSprite::createWithSpriteFrameName("gz_att_s.png");
            m_btn[1][1]->setAnchorPoint(CCPointZero);
            m_btn[1][1]->setVisible(false);
            m_btn[1][1]->setPosition(ccp(385.0f, 45.0f));
            pBgSprite->addChild(m_btn[1][1]);
        }
        
        /*信息文字*/
        /*城墙加成*/
        // qjqj
        char buffer[32];
        /*守军*/
        sprintf(buffer, "%d", m_pCityManager->getCityItem(m_pBattle->getSelectCity())->getGuardNum());
        CCLabelTTF* pShoujLabel = CCLabelTTF::create(buffer, "Arial", 27);
        pShoujLabel->setAnchorPoint(CCPointZero);
        pShoujLabel->setPosition(ccp(163.0f, 505.0f));
        pNode->addChild(pShoujLabel, 1, SHOUJUN_FONT_TAG);
        
        /*所需元宝*/
        sprintf(buffer, "×%d", m_pGameState->getCWarInspire());
        CCLabelTTF* pNeedGoldLabel = CCLabelTTF::create(buffer, "Arial", 25);
        pNeedGoldLabel->setAnchorPoint(CCPointZero);
        pNeedGoldLabel->setPosition(ccp(425.0f, 347.0f));
        pNode->addChild(pNeedGoldLabel, 1, GUWUNEEDGOLD_FONT_TAG);
        /*国战倒计时*/
        m_listTimeLabel = CCLabelTTF::create("", "Arial", 18);
        m_listTimeLabel->setAnchorPoint(ccp(0, 0));
        m_listTimeLabel->setPosition(ccp(490.0f, 300.0f));
        pNode->addChild(m_listTimeLabel);
        /*占领中*/
        m_pHold[0] = CCLabelTTF::create("占领中", "Arial", 27);
        m_pHold[0]->setAnchorPoint(ccp(0.5, 0));
        m_pHold[0]->setPosition(ccp(440.0f, 275.0f));
        m_pHold[0]->setColor(YELLOW_COLOR);
        m_pHold[0]->setOpacity(0);
        pNode->addChild(m_pHold[0]);
        m_pHold[1] = CCLabelTTF::create("点击退出", "Arial", 21);
        m_pHold[1]->setAnchorPoint(ccp(0.5, 0));
        m_pHold[1]->setPosition(ccp(443.0f, 240.0f));
        m_pHold[1]->setOpacity(0);
        pNode->addChild(m_pHold[1]);
        /*初始化领取国战奖励状态*/
        sprintf(buffer, "铜钱收入:%d",(int)((float)(m_defTime / 60)*m_pBattle
                                        ->getDefLevMny()*(1.0 + m_pBattle->getMnyBuff())));
        m_income[0] = CCLabelTTF::create(buffer, "Arial", 18);
        m_income[0]->setAnchorPoint(CCPointZero);
        m_income[0]->setPosition(ccp(490.0f, 260.0f));
        m_income[0]->setOpacity(0);
        pNode->addChild(m_income[0]);
        m_income[1] = CCLabelTTF::create("战功收入:", "Arial", 18);
        m_income[1]->setAnchorPoint(CCPointZero);
        m_income[1]->setPosition(ccp(490.0f, 220.0f));
        m_income[1]->setOpacity(0);
        pNode->addChild(m_income[1]);
        
        /*加成数据*/
        // 城墙
        if (m_pGameState->getHaveWall()) {
            sprintf(buffer, "生命+%.1f%%", m_pBattle->getWallHp());
            CCLabelTTF* pCqHp = CCLabelTTF::create(buffer, "Arial", 19);
            pCqHp->setAnchorPoint(CCPointZero);
            pCqHp->setPosition(ccp(325.0f, 662.0f));
            pNode->addChild(pCqHp);
            sprintf(buffer, "攻击+%.1f%%", m_pBattle->getWallAtk());
            CCLabelTTF* pCqAtk = CCLabelTTF::create(buffer, "Arial", 19);
            pCqAtk->setAnchorPoint(CCPointZero);
            pCqAtk->setPosition(ccp(425.0f, 662.0f));
            pNode->addChild(pCqAtk);
            sprintf(buffer, "铜币+%.1f%%", m_pBattle->getWallMoney());
            CCLabelTTF* pCqMny = CCLabelTTF::create(buffer, "Arial", 19);
            pCqMny->setAnchorPoint(CCPointZero);
            pCqMny->setPosition(ccp(525.0f, 662.0f));
            pNode->addChild(pCqMny);
        } else {
            sprintf(buffer, "生命+%.1f%%", 0.0f);
            CCLabelTTF* pCqHp = CCLabelTTF::create(buffer, "Arial", 19);
            pCqHp->setAnchorPoint(CCPointZero);
            pCqHp->setPosition(ccp(325.0f, 662.0f));
            pNode->addChild(pCqHp);
            sprintf(buffer, "攻击+%.1f%%", 0.0f);
            CCLabelTTF* pCqAtk = CCLabelTTF::create(buffer, "Arial", 19);
            pCqAtk->setAnchorPoint(CCPointZero);
            pCqAtk->setPosition(ccp(425.0f, 662.0f));
            pNode->addChild(pCqAtk);
            sprintf(buffer, "铜币+%.1f%%", 0.0f);
            CCLabelTTF* pCqMny = CCLabelTTF::create(buffer, "Arial", 19);
            pCqMny->setAnchorPoint(CCPointZero);
            pCqMny->setPosition(ccp(525.0f, 662.0f));
            pNode->addChild(pCqMny);
        }
        
        // 投石车
        if (m_pGameState->getHaveCar()) {
            sprintf(buffer, "生命+%.1f%%", m_pBattle->getHp());
            CCLabelTTF* pTsHp = CCLabelTTF::create(buffer, "Arial", 19);
            pTsHp->setAnchorPoint(CCPointZero);
            pTsHp->setPosition(ccp(325.0f, 622.0f));
            pNode->addChild(pTsHp);
            sprintf(buffer, "攻击+%.1f%%", m_pBattle->getAtk());
            CCLabelTTF* pTsAtk = CCLabelTTF::create(buffer, "Arial", 19);
            pTsAtk->setAnchorPoint(CCPointZero);
            pTsAtk->setPosition(ccp(425.0f, 622.0f));
            pNode->addChild(pTsAtk);
            sprintf(buffer, "铜币+%.1f%%", m_pBattle->getMoney());
            CCLabelTTF* pTsMny = CCLabelTTF::create(buffer, "Arial", 19);
            pTsMny->setAnchorPoint(CCPointZero);
            pTsMny->setPosition(ccp(525.0f, 622.0f));
            pNode->addChild(pTsMny);
        } else {
            sprintf(buffer, "生命+%.1f%%", 0.0f);
            CCLabelTTF* pTsHp = CCLabelTTF::create(buffer, "Arial", 19);
            pTsHp->setAnchorPoint(CCPointZero);
            pTsHp->setPosition(ccp(325.0f, 622.0f));
            pNode->addChild(pTsHp);
            sprintf(buffer, "攻击+%.1f%%", 0.0f);
            CCLabelTTF* pTsAtk = CCLabelTTF::create(buffer, "Arial", 19);
            pTsAtk->setAnchorPoint(CCPointZero);
            pTsAtk->setPosition(ccp(425.0f, 622.0f));
            pNode->addChild(pTsAtk);
            sprintf(buffer, "铜币+%.1f%%", 0.0f);
            CCLabelTTF* pTsMny = CCLabelTTF::create(buffer, "Arial", 19);
            pTsMny->setAnchorPoint(CCPointZero);
            pTsMny->setPosition(ccp(525.0f, 622.0f));
            pNode->addChild(pTsMny);
        }
        
        // 鼓舞
        if (m_pGameState->getHaveInspire()) {
            sprintf(buffer, "生命+%.1f%%", m_pBattle->getInspireHp());
            m_pIsHp = CCLabelTTF::create(buffer, "Arial", 19);
            m_pIsHp->setAnchorPoint(CCPointZero);
            m_pIsHp->setPosition(ccp(325.0f, 442.0f));
            pNode->addChild(m_pIsHp);
            sprintf(buffer, "攻击+%.1f%%", m_pBattle->getInspireAtk());
            m_pIsAtk = CCLabelTTF::create(buffer, "Arial", 19);
            m_pIsAtk->setAnchorPoint(CCPointZero);
            m_pIsAtk->setPosition(ccp(425.0f, 442.0f));
            pNode->addChild(m_pIsAtk);
            sprintf(buffer, "铜币+%.1f%%", m_pBattle->getInspireMny());
            m_pIsMny = CCLabelTTF::create(buffer, "Arial", 19);
            m_pIsMny->setAnchorPoint(CCPointZero);
            m_pIsMny->setPosition(ccp(525.0f, 442.0f));
            pNode->addChild(m_pIsMny);
        } else {
            sprintf(buffer, "生命+%.1f%%", 0.0f);
            m_pIsHp = CCLabelTTF::create(buffer, "Arial", 19);
            m_pIsHp->setAnchorPoint(CCPointZero);
            m_pIsHp->setPosition(ccp(325.0f, 442.0f));
            pNode->addChild(m_pIsHp);
            sprintf(buffer, "攻击+%.1f%%", 0.0f);
            m_pIsAtk = CCLabelTTF::create(buffer, "Arial", 19);
            m_pIsAtk->setAnchorPoint(CCPointZero);
            m_pIsAtk->setPosition(ccp(425.0f, 442.0f));
            pNode->addChild(m_pIsAtk);
            sprintf(buffer, "铜币+%.1f%%", 0.0f);
            m_pIsMny = CCLabelTTF::create(buffer, "Arial", 19);
            m_pIsMny->setAnchorPoint(CCPointZero);
            m_pIsMny->setPosition(ccp(525.0f, 442.0f));
            pNode->addChild(m_pIsMny);
        }
        
        m_labelCooling = CCLabelTTF::create("", "Arial", 23);
        m_labelCooling->setAnchorPoint(CCPointZero);
        m_labelCooling->setPosition(ccp(500.0f, 143.0f));
        m_labelCooling->setVisible(true);
        pNode->addChild(m_labelCooling, 100);
        schedule(schedule_selector(GuoZCityLayer::_coolForFight), 1);
        
        /*初始化按钮UI*/
//        _defOrAtt();
        
    } else if (TYPE_FOR_WAIT == type)/*国战未开始*/ {
        CCNode* pNode = CCNode::create();
        pNode->setAnchorPoint(CCPointZero);
        pNode->setPosition(ccp(0.0f, m_pGameState->getBottomOffset()));
        addChild(pNode, 4, NODE_TAG);
        
        CCSprite* pBgSprite = CCSprite::createWithSpriteFrameName("gz_bg.png");
        pBgSprite->setAnchorPoint(CCPointZero);
        pBgSprite->setPosition(ccp(0.0f, m_pGameState->getBottomOffset() + GUOZ_CITY_BG_V));
        m_pSBN->addChild(pBgSprite, 1, GUOZ_CITY_BG_TAG);
        // 上下款
        CCSprite* pXia = CCSprite::createWithSpriteFrameName("gz_xia.png");
        pXia->setAnchorPoint(CCPointZero);
        pXia->setPosition(ccp(0.0f, -10.0f));
        pBgSprite->addChild(pXia);
        CCSprite* pShang = CCSprite::createWithSpriteFrameName("gz_shang.png");
        pShang->setAnchorPoint(CCPointZero);
        pShang->setPosition(ccp(0.0f, 580.0f));
        pBgSprite->addChild(pShang);
        /*判断当前城池*/
        CCSprite* pCityDiBg = CCSprite::createWithSpriteFrameName("gz_cityDiBg.png");
        pCityDiBg->setAnchorPoint(CCPointZero);
        pCityDiBg->setPosition(ccp(35.0f, 425.0f));
        pBgSprite->addChild(pCityDiBg);
        CCSprite* pCityDi = CCSprite::createWithSpriteFrameName("gz_cityDi.png");
        pCityDi->setAnchorPoint(CCPointZero);
        pCityDi->setPosition(ccp(0.0f, 455.0f));
        pBgSprite->addChild(pCityDi);
        /*横条*/
        CCSprite* pHengt = CCSprite::createWithSpriteFrameName("gz_hengTiao.png");
        pHengt->setAnchorPoint(CCPointZero);
        pHengt->setPosition(ccp(0.0f, 415.0f));
        pBgSprite->addChild(pHengt);
        /*横条*/
        CCSprite* pHengt1 = CCSprite::createWithSpriteFrameName("gz_hengTiao.png");
        pHengt1->setAnchorPoint(CCPointZero);
        pHengt1->setPosition(ccp(0.0f, 220.0f));
        pBgSprite->addChild(pHengt1);
        int camp = 0;
        if (m_pBattle->getSelectCity() == 1) {
            camp = m_pCityManager->getCityItem(1)->getHolder();
            _checkForWitch(camp, 1);
        } else if (m_pBattle->getSelectCity() == 2) {
            camp = m_pCityManager->getCityItem(2)->getHolder();
            _checkForWitch(camp, 2);
        } else if (m_pBattle->getSelectCity() == 3) {
            camp = m_pCityManager->getCityItem(3)->getHolder();
            _checkForWitch(camp, 3);
        }
//        /*战略*/
//        CCSprite* pZhanlue = CCSprite::createWithSpriteFrameName("gz_font_zl.png");
//        pZhanlue->setAnchorPoint(CCPointZero);
//        pZhanlue->setPosition(ccp(85.0f, 370.0f));
//        pBgSprite->addChild(pZhanlue);
        /*建造文字*/
        CCSprite* pBtnFontCour = CCSprite::createWithSpriteFrameName("gz_font_build.png");
        pBtnFontCour->setAnchorPoint(CCPointZero);
        pBtnFontCour->setPosition(ccp(395.0f, 310.0f));
        pBgSprite->addChild(pBtnFontCour, 3);
        /*元宝消耗*/
        CCSprite* pCourageGold = CCSprite::createWithSpriteFrameName("gz_gem.png");
        pCourageGold->setAnchorPoint(CCPointZero);
        pCourageGold->setPosition(ccp(350.0f, 262.0f));
        pBgSprite->addChild(pCourageGold);
        /* 城池功能UI*/
        CCSprite* pFunc = CCSprite::createWithSpriteFrameName("gz_font_cityfunc.png");
        pFunc->setAnchorPoint(ccp(0.5, 0));
        pFunc->setPosition(ccp(320.0f, 180.0f));
        pBgSprite->addChild(pFunc);
        for (int i = 0; i < 5; ++i) {
            /* 城池TUAN图标 */
            CCSprite* pTuan = CCSprite::createWithSpriteFrameName("gz_tuan.png");
            pTuan->setAnchorPoint(ccp(0, 0));
            pTuan->setPosition(ccp(50.0f + i * 125.0f, 180.0f));
            pBgSprite->addChild(pTuan);
            if (i == 2) {
                pTuan->setVisible(false);
            }
            /*城池功能框*/
            CCSprite* pFuncKuang = CCSprite::createWithSpriteFrameName("gz_zhanlueBg.png");
            pFuncKuang->setAnchorPoint(ccp(0, 0));
            pFuncKuang->setPosition(ccp(36.0f + i * 125.0f, 110.0f));
            pBgSprite->addChild(pFuncKuang);
        }
        /*所选城池项,描述*/
        CityItem* pCityItem = m_pCityManager->getCityItem(m_pBattle->getSelectCity());
        for (int i = 0; i < pCityItem->getNumberOfCityFuncs(); ++i) {
            CityFunctionItem* pCityFunctionItem = pCityItem->getCityFuncItemByIndex(i);
            CCLabelTTF* pLabel = CCLabelTTF::create(pCityFunctionItem->getDes().c_str(), "Arial", 19,
                                                    CCSizeMake(95.0f, 95.0f), kCCTextAlignmentLeft);
            pLabel->setAnchorPoint(CCPointZero);
            pLabel->setPosition(ccp(25.0f + i * 125.0f, 110.0f));
            pLabel->setColor(GOLD_COLOR);
            pNode->addChild(pLabel);
            
            CCSprite* pFIcon = CCSprite::createWithSpriteFrameName(pCityFunctionItem->getI().c_str());
            pFIcon->setAnchorPoint(ccp(0, 0));
            pFIcon->setPosition(ccp(33.0f + i * 125.0f, 115.0f));
            pBgSprite->addChild(pFIcon);
            
            CCStrokeLabel* pFName = CCStrokeLabel::create(pCityFunctionItem->getName().c_str(), "Arial", 19);
            pFName->setAnchorPoint(CCPointZero);
            pFName->setPosition(ccp(32.0f + i * 125.0f, 205.0f));
            pFName->setColor(YELLOW_COLOR);
            pNode->addChild(pFName);
        }
        
        for (int i = 0; i < 4; ++i) {
            /*切割竖线*/
            CCSprite* pQieGe = CCSprite::createWithSpriteFrameName("gz_qieGe.png");
            pQieGe->setAnchorPoint(ccp(0, 0));
            pQieGe->setScaleY(0.7);
            pQieGe->setPosition(ccp(133.0f + i * 125.0f, 10.0f));
            pBgSprite->addChild(pQieGe);
        }
        /*花纹*/
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                CCSprite* pHua = CCSprite::createWithSpriteFrameName("gz_huawen.png");
                pHua->setAnchorPoint(ccp(0, 0));
                pHua->setPosition(ccp(0.0f + j * 555.0f, 180.0f + i * 205.0f));
                pBgSprite->addChild(pHua);
                if ((i == 0 && j == 0) || (i == 1 && j == 0)) {
                    pHua->setFlipX(true);
                }
            }
        }
        
        /*玩家势力标志和城池名字*/
        if (m_pBattle->getSelectCity() == 1) {
            _checkForWitch(1, 1);
            /*城池名字*/
            CCLabelTTF* pName = CCLabelTTF::create("洛阳", "Arial", 30);
            pName->setAnchorPoint(ccp(0, 0));
            pName->setPosition(ccp(400.0f, 680.0f));
            pNode->addChild(pName);
        } else if (m_pBattle->getSelectCity() == 2) {
            _checkForWitch(1, 2);
            /*城池名字*/
            CCLabelTTF* pName = CCLabelTTF::create("长安", "Arial", 30);
            pName->setAnchorPoint(ccp(0, 0));
            pName->setPosition(ccp(400.0f, 680.0f));
            pNode->addChild(pName);
        } else if (m_pBattle->getSelectCity() == 3) {
            _checkForWitch(1, 3);
            /*城池名字*/
            CCLabelTTF* pName = CCLabelTTF::create("陈留", "Arial", 30);
            pName->setAnchorPoint(ccp(0, 0));
            pName->setPosition(ccp(400.0f, 680.0f));
            pNode->addChild(pName);
        }
        
        /*势力标志*/
        int shilitype = m_pCityManager->getCityItem(m_pBattle->getSelectCity())->getHolder();
        if (shilitype == 1) {
            CCSprite* pCamp = CCSprite::createWithSpriteFrameName("gz_wei.png");
            pCamp->setAnchorPoint(CCPointZero);
            pCamp->setPosition(ccp(320.0f, 540.0f));
            pBgSprite->addChild(pCamp);
        } else if (shilitype == 2) {
            CCSprite* pCamp = CCSprite::createWithSpriteFrameName("gz_shu.png");
            pCamp->setAnchorPoint(CCPointZero);
            pCamp->setPosition(ccp(320.0f, 540.0f));
            pBgSprite->addChild(pCamp);
        } else {
            CCSprite* pCamp = CCSprite::createWithSpriteFrameName("gz_wu.png");
            pCamp->setAnchorPoint(CCPointZero);
            pCamp->setPosition(ccp(320.0f, 540.0f));
            pBgSprite->addChild(pCamp);
        }
        /*建造投石车/城墙*/
        /*城墙*,攻击*/
        CCSprite* pChenqAtt = CCSprite::createWithSpriteFrameName("gz_rampart.png");
        pChenqAtt->setAnchorPoint(CCPointZero);
        pChenqAtt->setPosition(ccp(245.0f, 490.0f));
        pBgSprite->addChild(pChenqAtt);
        /*投石车, 防守*/
        CCSprite* pToushiDef = CCSprite::createWithSpriteFrameName("gz_font_tous.png");
        pToushiDef->setAnchorPoint(CCPointZero);
        pToushiDef->setPosition(ccp(220.0f, 380.0f));
        pToushiDef->setVisible(false);
        pBgSprite->addChild(pToushiDef);
        CCSprite* pChenqDef = CCSprite::createWithSpriteFrameName("gz_rampart.png");
        pChenqDef->setAnchorPoint(CCPointZero);
        pChenqDef->setPosition(ccp(245.0f, 380.0f));
        pChenqDef->setVisible(false);
        pBgSprite->addChild(pChenqDef);
        /*战略*/
        CCSprite* pPicTouS = CCSprite::createWithSpriteFrameName("gz_tsc.png");
        pPicTouS->setAnchorPoint(CCPointZero);
        pPicTouS->setPosition(ccp(50.0f, 267.0f));
        pPicTouS->setVisible(false);
        pBgSprite->addChild(pPicTouS);
        CCSprite* pPicChenQ = CCSprite::createWithSpriteFrameName("gz_cq.png");
        pPicChenQ->setAnchorPoint(CCPointZero);
        pPicChenQ->setPosition(ccp(50.0f, 267.0f));
        pPicChenQ->setVisible(false);
        pBgSprite->addChild(pPicChenQ);
        
        if (!m_isDefending)/*如果是攻击方*/ {
            pToushiDef->setVisible(true);
            pChenqDef->setVisible(false);
            pPicTouS->setVisible(true);
            pPicChenQ->setVisible(false);
        } else {
            pToushiDef->setVisible(false);
            pChenqDef->setVisible(true);
            pPicTouS->setVisible(false);
            pPicChenQ->setVisible(true);
        }
        
        //建造
        m_btn[3][0]= CCSprite::createWithSpriteFrameName("gz_btn.png");
        m_btn[3][0]->setAnchorPoint(CCPointZero);
        m_btn[3][0]->setPosition(ccp(315.0f, 307.0f));
        pBgSprite->addChild(m_btn[3][0], 2);
        m_btn[3][1] = CCSprite::createWithSpriteFrameName("gz_btn_s.png");
        m_btn[3][1]->setAnchorPoint(CCPointZero);
        m_btn[3][1]->setVisible(false);
        m_btn[3][1]->setPosition(ccp(315.0f, 307.0f));
        pBgSprite->addChild(m_btn[3][1]);
        
        /*建造所需元宝*/
        char buffer[32];
        if (m_isDefending) {
            sprintf(buffer, "×%d", m_pGameState->getCWarBuildWall());
            m_needGoldForBuild = CCLabelTTF::create(buffer, "Arial", 23);
            m_needGoldForBuild->setAnchorPoint(CCPointZero);
            m_needGoldForBuild->setPosition(ccp(400.0f, 387.0f));
            pNode->addChild(m_needGoldForBuild);
            if (m_pGameState->getHaveWall()) {
                /*生命，攻击，铜钱城墙加成*/
                sprintf(buffer, "生命+%.1f%%", m_pBattle->getWallHp());
                m_hpAdditionC = CCLabelTTF::create(buffer, "Arial", 19);
                m_hpAdditionC->setAnchorPoint(CCPointZero);
                m_hpAdditionC->setPosition(ccp(325.0f, 510.0f));
                pNode->addChild(m_hpAdditionC);
                sprintf(buffer, "攻击+%.1f%%", m_pBattle->getWallAtk());
                m_atkAdditionC = CCLabelTTF::create(buffer, "Arial", 19);
                m_atkAdditionC->setAnchorPoint(CCPointZero);
                m_atkAdditionC->setPosition(ccp(425.0f, 510.0f));
                pNode->addChild(m_atkAdditionC);
                sprintf(buffer, "铜币+%.1f%%", m_pBattle->getWallMoney());
                m_mnyAdditionC = CCLabelTTF::create(buffer, "Arial", 19);
                m_mnyAdditionC->setAnchorPoint(CCPointZero);
                m_mnyAdditionC->setPosition(ccp(525.0f, 510.0f));
                pNode->addChild(m_mnyAdditionC);
                /*生命，攻击，铜钱投石车加成*/
                sprintf(buffer, "生命+%.1f%%", m_pBattle->getWallHp());
                m_hpAdditionT = CCLabelTTF::create(buffer, "Arial", 19);
                m_hpAdditionT->setAnchorPoint(CCPointZero);
                m_hpAdditionT->setPosition(ccp(325.0f, 620.0f));
                pNode->addChild(m_hpAdditionT);
                sprintf(buffer, "攻击+%.1f%%", m_pBattle->getWallAtk());
                m_atkAdditionT = CCLabelTTF::create(buffer, "Arial", 19);
                m_atkAdditionT->setAnchorPoint(CCPointZero);
                m_atkAdditionT->setPosition(ccp(425.0f, 620.0f));
                pNode->addChild(m_atkAdditionT);
                sprintf(buffer, "铜币+%.1f%%", m_pBattle->getWallMoney());
                m_mnyAdditionT = CCLabelTTF::create(buffer, "Arial", 19);
                m_mnyAdditionT->setAnchorPoint(CCPointZero);
                m_mnyAdditionT->setPosition(ccp(525.0f, 620.0f));
                pNode->addChild(m_mnyAdditionT);
            } else {
                /*生命，攻击，铜钱城墙加成*/
                sprintf(buffer, "生命+%.1f%%", 0.0);
                m_hpAdditionC = CCLabelTTF::create(buffer, "Arial", 19);
                m_hpAdditionC->setAnchorPoint(CCPointZero);
                m_hpAdditionC->setPosition(ccp(325.0f, 510.0f));
                pNode->addChild(m_hpAdditionC);
                sprintf(buffer, "攻击+%.1f%%", 0.0);
                m_atkAdditionC = CCLabelTTF::create(buffer, "Arial", 19);
                m_atkAdditionC->setAnchorPoint(CCPointZero);
                m_atkAdditionC->setPosition(ccp(425.0f, 510.0f));
                pNode->addChild(m_atkAdditionC);
                sprintf(buffer, "铜币+%.1f%%", 0.0);
                m_mnyAdditionC = CCLabelTTF::create(buffer, "Arial", 19);
                m_mnyAdditionC->setAnchorPoint(CCPointZero);
                m_mnyAdditionC->setPosition(ccp(525.0f, 510.0f));
                pNode->addChild(m_mnyAdditionC);
                /*生命，攻击，铜钱投石车加成*/
                sprintf(buffer, "生命+%.1f%%", 0.0);
                m_hpAdditionT = CCLabelTTF::create(buffer, "Arial", 19);
                m_hpAdditionT->setAnchorPoint(CCPointZero);
                m_hpAdditionT->setPosition(ccp(325.0f, 620.0f));
                pNode->addChild(m_hpAdditionT);
                sprintf(buffer, "攻击+%.1f%%", 0.0);
                m_atkAdditionT = CCLabelTTF::create(buffer, "Arial", 19);
                m_atkAdditionT->setAnchorPoint(CCPointZero);
                m_atkAdditionT->setPosition(ccp(425.0f, 620.0f));
                pNode->addChild(m_atkAdditionT);
                sprintf(buffer, "铜币+%.1f%%", 0.0);
                m_mnyAdditionT = CCLabelTTF::create(buffer, "Arial", 19);
                m_mnyAdditionT->setAnchorPoint(CCPointZero);
                m_mnyAdditionT->setPosition(ccp(525.0f, 620.0f));
                pNode->addChild(m_mnyAdditionT);
            }
        } else {
            sprintf(buffer, "×%d", m_pGameState->getCWarBuildCar());
            m_needGoldForBuild = CCLabelTTF::create(buffer, "Arial", 23);
            m_needGoldForBuild->setAnchorPoint(CCPointZero);
            m_needGoldForBuild->setPosition(ccp(400.0f, 387.0f));
            pNode->addChild(m_needGoldForBuild);
            
            if (m_pGameState->getHaveCar()) {
                /*生命，攻击，铜钱投石车加成*/
                sprintf(buffer, "生命+%.1f%%", m_pBattle->getHp());
                m_hpAdditionC = CCLabelTTF::create(buffer, "Arial", 19);
                m_hpAdditionC->setAnchorPoint(CCPointZero);
                m_hpAdditionC->setPosition(ccp(325.0f, 510.0f));
                pNode->addChild(m_hpAdditionC);
                sprintf(buffer, "攻击+%.1f%%", m_pBattle->getAtk());
                m_atkAdditionC = CCLabelTTF::create(buffer, "Arial", 19);
                m_atkAdditionC->setAnchorPoint(CCPointZero);
                m_atkAdditionC->setPosition(ccp(425.0f, 510.0f));
                pNode->addChild(m_atkAdditionC);
                sprintf(buffer, "铜币+%.1f%%", m_pBattle->getMoney());
                m_mnyAdditionC = CCLabelTTF::create(buffer, "Arial", 19);
                m_mnyAdditionC->setAnchorPoint(CCPointZero);
                m_mnyAdditionC->setPosition(ccp(525.0f, 510.0f));
                pNode->addChild(m_mnyAdditionC);
            } else {
                /*生命，攻击，铜钱投石车加成*/
                sprintf(buffer, "生命+%.1f%%", 0.0);
                m_hpAdditionC = CCLabelTTF::create(buffer, "Arial", 19);
                m_hpAdditionC->setAnchorPoint(CCPointZero);
                m_hpAdditionC->setPosition(ccp(325.0f, 510.0f));
                pNode->addChild(m_hpAdditionC);
                sprintf(buffer, "攻击+%.1f%%", 0.0);
                m_atkAdditionC = CCLabelTTF::create(buffer, "Arial", 19);
                m_atkAdditionC->setAnchorPoint(CCPointZero);
                m_atkAdditionC->setPosition(ccp(425.0f, 510.0f));
                pNode->addChild(m_atkAdditionC);
                sprintf(buffer, "铜币+%.1f%%", 0.0);
                m_mnyAdditionC = CCLabelTTF::create(buffer, "Arial", 19);
                m_mnyAdditionC->setAnchorPoint(CCPointZero);
                m_mnyAdditionC->setPosition(ccp(525.0f, 510.0f));
                pNode->addChild(m_mnyAdditionC);
            }
            
            if (m_pGameState->getHaveWall()) {
                /*生命，攻击，铜钱加成*/
                sprintf(buffer, "生命+%.1f%%", m_pBattle->getHp());
                m_hpAdditionT = CCLabelTTF::create(buffer, "Arial", 19);
                m_hpAdditionT->setAnchorPoint(CCPointZero);
                m_hpAdditionT->setPosition(ccp(325.0f, 620.0f));
                pNode->addChild(m_hpAdditionT);
                sprintf(buffer, "攻击+%.1f%%", m_pBattle->getAtk());
                m_atkAdditionT = CCLabelTTF::create(buffer, "Arial", 19);
                m_atkAdditionT->setAnchorPoint(CCPointZero);
                m_atkAdditionT->setPosition(ccp(425.0f, 620.0f));
                pNode->addChild(m_atkAdditionT);
                sprintf(buffer, "铜币+%.1f%%", m_pBattle->getMoney());
                m_mnyAdditionT = CCLabelTTF::create(buffer, "Arial", 19);
                m_mnyAdditionT->setAnchorPoint(CCPointZero);
                m_mnyAdditionT->setPosition(ccp(525.0f, 620.0f));
                pNode->addChild(m_mnyAdditionT);
            } else {
                /*生命，攻击，铜钱加成*/
                sprintf(buffer, "生命+%.1f%%", 0.0);
                m_hpAdditionT = CCLabelTTF::create(buffer, "Arial", 19);
                m_hpAdditionT->setAnchorPoint(CCPointZero);
                m_hpAdditionT->setPosition(ccp(325.0f, 620.0f));
                pNode->addChild(m_hpAdditionT);
                sprintf(buffer, "攻击+%.1f%%", 0.0);
                m_atkAdditionT = CCLabelTTF::create(buffer, "Arial", 19);
                m_atkAdditionT->setAnchorPoint(CCPointZero);
                m_atkAdditionT->setPosition(ccp(425.0f, 620.0f));
                pNode->addChild(m_atkAdditionT);
                sprintf(buffer, "铜币+%.1f%%", 0.0);
                m_mnyAdditionT = CCLabelTTF::create(buffer, "Arial", 19);
                m_mnyAdditionT->setAnchorPoint(CCPointZero);
                m_mnyAdditionT->setPosition(ccp(525.0f, 620.0f));
                pNode->addChild(m_mnyAdditionT);
            }
            
            
        }
    }
    
    
    
    /***** tipMenu非大图 ******/
    CCNode* leftNode = CCNode::create();
    addChild(leftNode, 5, TIPMENU_LEFT_TAG);
    CCMenuItemImage* pLeft = CCMenuItemImage::create("CM_left.png",
                                                     "CM_left.png",
                                                     this,
                                                     menu_selector(GuoZCityLayer::_returnForm));
    pLeft->setAnchorPoint(ccp(0, 0));
    pLeft->setPosition(ccp(0.0f, TIPMENU_V + m_pGameState->getBottomOffset()));
    CCMenu* menu = CCMenu::create(pLeft, NULL);
    menu->setTouchEnabled(m_thisTouch);
    menu->setAnchorPoint(ccp(0, 0));
    menu->setPosition(ccp(0, 0));
    leftNode->addChild(menu);
    CCNode* rightNode = CCNode::create();
    addChild(rightNode, 5, TIPMENU_RIGHT_TAG);
    CCSprite* pRight = CCSprite::create("CM_right.png");
    pRight->setAnchorPoint(ccp(0, 0));
    pRight->setPosition(ccp(482.0f, TIPMENU_V + m_pGameState->getBottomOffset()));
    rightNode->addChild(pRight);
    CCNode* midNode = CCNode::create();
    addChild(midNode,5, TIPMENU_MID_TAG);
    CCSprite* pMid = CCSprite::create("CM_mid.png");
    pMid->setAnchorPoint(ccp(0, 0));
    pMid->setPosition(ccp(165.0f, TIPMENU_V + m_pGameState->getBottomOffset()));
    midNode->addChild(pMid);
    /* 城市文字 */
    CCSprite* cityName = NULL;
    if (m_pBattle->getSelectCity() == 1) {
        cityName = CCSprite::createWithSpriteFrameName("gz_city_luoy.png");
    } else if (m_pBattle->getSelectCity() == 2) {
        cityName = CCSprite::createWithSpriteFrameName("gz_city_changa.png");
    } else {
        cityName = CCSprite::createWithSpriteFrameName("gz_city_chenl.png");
    }
    cityName->setAnchorPoint(ccp(0, 0));
    cityName->setPosition(ccp(275.0f, TIPMENU_V + m_pGameState->getBottomOffset() + 8.0f));
    midNode->addChild(cityName);
    /* 返回文字 */
    CCSprite* returnFont = CCSprite::create("LU_return.png");
    returnFont->setAnchorPoint(ccp(0, 0));
    returnFont->setPosition(ccp(15.0f, TIPMENU_V + m_pGameState->getBottomOffset() + 8.0f));
    leftNode->addChild(returnFont, 2);
}

void GuoZCityLayer::updateAllOfGZCity() {
}

/*建造刷新*/
void GuoZCityLayer::updateForBuild() {
    char buffer[32];
    if (m_isDefending) {
        sprintf(buffer, "生命+%.1f%%", m_pBattle->getWallHp());
        m_hpAdditionC->setString(buffer);
        sprintf(buffer, "攻击+%.1f%%", m_pBattle->getWallAtk());
        m_atkAdditionC->setString(buffer);
        sprintf(buffer, "铜币+%.1f%%", m_pBattle->getWallMoney());
        m_mnyAdditionC->setString(buffer);
        sprintf(buffer, "生命+%.1f%%", m_pBattle->getWallHp());
        m_hpAdditionT->setString(buffer);
        sprintf(buffer, "攻击+%.1f%%", m_pBattle->getWallAtk());
        m_atkAdditionT->setString(buffer);
        sprintf(buffer, "铜币+%.1f%%", m_pBattle->getWallMoney());
        m_mnyAdditionT->setString(buffer);
    } else {
        sprintf(buffer, "生命+%.1f%%", m_pBattle->getWallHp());
        m_hpAdditionT->setString(buffer);
        sprintf(buffer, "攻击+%.1f%%", m_pBattle->getWallAtk());
        m_atkAdditionT->setString(buffer);
        sprintf(buffer, "铜币+%.1f%%", m_pBattle->getWallMoney());
        m_mnyAdditionT->setString(buffer);

        sprintf(buffer, "生命+%.1f%%", m_pBattle->getHp());
        m_hpAdditionC->setString(buffer);
        sprintf(buffer, "攻击+%.1f%%", m_pBattle->getAtk());
        m_atkAdditionC->setString(buffer);
        sprintf(buffer, "铜币+%.1f%%", m_pBattle->getMoney());
        m_mnyAdditionC->setString(buffer);
    }
    
}

/*鼓舞刷新*/
void GuoZCityLayer::updateForInspire() {
    char buffer[32];
    sprintf(buffer, "生命+%.1f%%", m_pBattle->getInspireHp());
    m_pIsHp->setString(buffer);
    sprintf(buffer, "攻击+%.1f%%", m_pBattle->getInspireAtk());
    m_pIsAtk->setString(buffer);
    sprintf(buffer, "铜币+%.1f%%", m_pBattle->getInspireMny());
    m_pIsMny->setString(buffer);

}

void GuoZCityLayer::_initBtn() {
    for (int i = 0; i < COUNT_OF_BTNS; ++i) {
        if (m_btn[i][0]) {
            m_btn[i][0]->setVisible(true);
            m_btn[i][1]->setVisible(false);
        }
    }
}

//
/*初始化位置*/
void GuoZCityLayer::initPosition(){
    CCSprite* parent = static_cast<CCSprite*>(m_pSBN->getChildByTag(GUOZ_CITY_BG_TAG));
    parent->setPosition(INIT_LAYER_POSITION);
    CCNode* pNode = (CCNode*)this->getChildByTag(NODE_TAG);
    if (pNode)
        pNode->setPosition(INIT_LAYER_POSITION);
}
/*移出动作*/
void GuoZCityLayer::moveOutTouchUICompent(){
    _tipMenu(TYPE_OUT);
    
    CCSprite* parent = static_cast<CCSprite*>(m_pSBN->getChildByTag(GUOZ_CITY_BG_TAG));
    parent->runAction(CCSequence::create(CCMoveTo::create(DURATION_TIME, INIT_LAYER_POSITION), CCCallFunc::create(this, callfunc_selector(GuoZCityLayer::_callbackForMoveOut)),NULL));
    CCNode* pNode = (CCNode*)this->getChildByTag(NODE_TAG);
    if (pNode)
        pNode->runAction(CCMoveTo::create(DURATION_TIME, INIT_LAYER_POSITION));
    
    /*当前为PVE状态*/
    m_pBattle->setBattlePkMode(BPM_PVE);
}
void GuoZCityLayer::_callbackForMoveOut(){
    this->removeFromParent();
    CCLog("国战城池层 从主场景移除");
}
/*恢复响应动作*/
void GuoZCityLayer::restoreTouchUICompent(){
    _tipMenu(TYPE_IN);
    
    CCSprite* parent = static_cast<CCSprite*>(m_pSBN->getChildByTag(GUOZ_CITY_BG_TAG));
    parent->runAction(CCMoveTo::create(DURATION_TIME, ccp(0.0f,
                                                          m_pGameState->getBottomOffset() + GUOZ_CITY_BG_V)));
    CCNode* pNode = (CCNode*)this->getChildByTag(NODE_TAG);
    if (pNode)
        pNode->runAction(CCMoveTo::create(DURATION_TIME, ccp(0.0f, m_pGameState->getBottomOffset())));
    
    /*当前为PVP状态*/
    m_pBattle->setBattlePkMode(BPM_PVP);
}
//
void GuoZCityLayer::registerWithTouchDispatcher(void){
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, false);
}
bool GuoZCityLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent){
    if (!m_thisTouch)
        return false;
    bool validTouch = false;
    CCSprite* pBg = static_cast<CCSprite*>(m_pSBN->getChildByTag(GUOZ_CITY_BG_TAG));
    CCPoint touchPoint = pBg->convertTouchToNodeSpace(pTouch);
    if (m_btn[0][0])/*鼓舞*/ {
        if (m_btn[0][0]->boundingBox().containsPoint(touchPoint)) {
            m_btn[0][0]->setVisible(false);
            m_btn[0][1]->setVisible(true);
            validTouch = true;
        }
    }
    if (m_btn[1][0])/*防御或者攻击*/ {
        if (m_btn[1][0]->boundingBox().containsPoint(touchPoint)) {
            m_btn[1][0]->setVisible(false);
            m_btn[1][1]->setVisible(true);
            validTouch = true;
        }
    }
    if (m_btn[2][0])/*冷却重置*/ {
        if (m_btn[2][0]->boundingBox().containsPoint(touchPoint)) {
            m_btn[2][0]->setVisible(false);
            m_btn[2][1]->setVisible(true);
            validTouch = true;
        }
    }
    if (m_btn[3][0])/*建造*/ {
        if (m_btn[3][0]->boundingBox().containsPoint(touchPoint)) {
            m_btn[3][0]->setVisible(false);
            m_btn[3][1]->setVisible(true);
            validTouch = true;
        }
    }

    return validTouch;
}
void GuoZCityLayer::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent){
    _initBtn();
    
    CCSprite* pBg = static_cast<CCSprite*>(m_pSBN->getChildByTag(GUOZ_CITY_BG_TAG));
    CCPoint touchPoint = pBg->convertTouchToNodeSpace(pTouch);
    if (m_btn[0][0])/*鼓舞*/ {
        if (m_btn[0][0]->boundingBox().containsPoint(touchPoint)) {
            m_btn[0][0]->setVisible(false);
            m_btn[0][1]->setVisible(true);
        }
    }
    if (m_btn[1][0])/*防御或者攻击*/ {
        if (m_btn[1][0]->boundingBox().containsPoint(touchPoint)) {
            m_btn[1][0]->setVisible(false);
            m_btn[1][1]->setVisible(true);
        }
    }
    if (m_btn[2][0])/*冷却重置*/ {
        if (m_btn[2][0]->boundingBox().containsPoint(touchPoint)) {
            m_btn[2][0]->setVisible(false);
            m_btn[2][1]->setVisible(true);
        }
    }
    if (m_btn[3][0])/*建造*/ {
        if (m_btn[3][0]->boundingBox().containsPoint(touchPoint)) {
            m_btn[3][0]->setVisible(false);
            m_btn[3][1]->setVisible(true);
        }
    }
    
}
void GuoZCityLayer::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent){
    CCSprite* pBg = static_cast<CCSprite*>(m_pSBN->getChildByTag(GUOZ_CITY_BG_TAG));
    CCPoint touchPoint = pBg->convertTouchToNodeSpace(pTouch);
    if (m_btn[0][0])/*鼓舞*/ {
        if (m_btn[0][0]->boundingBox().containsPoint(touchPoint)) {
            _courage();
        }
    }
    if (m_btn[1][0])/*防御或者攻击*/ {
        if (m_btn[1][0]->boundingBox().containsPoint(touchPoint)) {
            _defOrAtt();
        }
    }
    if (m_btn[2][0])/*冷却重置*/ {
        if (m_btn[2][0]->boundingBox().containsPoint(touchPoint)) {
            _resetAtt();
        }
    }
    if (m_btn[3][0])/*建造*/ {
        if (m_btn[3][0]->boundingBox().containsPoint(touchPoint)) {
            _build();
        }
    }
    _initBtn();
}
void GuoZCityLayer::ccTouchCancelled(CCTouch*, CCEvent*){_initBtn();}

void GuoZCityLayer::_courage(){
    CCLog("鼓舞");
    if (m_pGamePlayer->getGold() < m_pGameState->getCWarInspire()) {
        const char* title = "元宝不足，无法鼓舞";
        MessageBox::getInstance()->messageTips(title);
        return;
    }
    
    char szPostBuffer[MAX_POSTTO_SERVER_BUFFER_SIZE];
    memset(szPostBuffer, '\0', MAX_POSTTO_SERVER_BUFFER_SIZE);
    sprintf(szPostBuffer, "c=cwar&a=inspire&uid=%s",
            GamePlayer::getInstance()->getUid().c_str());
    char szHttpTag[] = HTTP_REQUEST_GUWU_TAG;
    CCLog("post 鼓舞 = %s", szPostBuffer);
    m_pNetConnection->commitPostRequestData(szPostBuffer, szHttpTag);
}
void GuoZCityLayer::_defOrAtt(){

    if (m_isDefending)/*防御状态*/ {
        CCLog("防御");
        if (m_isInList == 1) {
            CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_CWARCHOSFORM_STRING);
        } else {
            aniForAtkOrDef();
            
            
            char szPostBuffer[MAX_POSTTO_SERVER_BUFFER_SIZE];
            memset(szPostBuffer, '\0', MAX_POSTTO_SERVER_BUFFER_SIZE);
            sprintf(szPostBuffer, "c=cwar&a=reward&city=%d&uid=%s", m_pBattle->getSelectCity(),
                    GamePlayer::getInstance()->getUid().c_str());
            char szHttpTag[] = HTTP_REQUEST_REWARD_TAG;
            CCLog("post 鼓舞 = %s", szPostBuffer);
            m_pNetConnection->commitPostRequestData(szPostBuffer, szHttpTag);
        }
    } else {
        if (m_isInList == 1) {
            CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_CWARCHOSFORM_STRING);
        } else {
            aniForAtkOrDef();
            
            char szPostBuffer[MAX_POSTTO_SERVER_BUFFER_SIZE];
            memset(szPostBuffer, '\0', MAX_POSTTO_SERVER_BUFFER_SIZE);
            sprintf(szPostBuffer, "c=cwar&a=reward&city=%d&uid=%s", m_pBattle->getSelectCity(),
                    GamePlayer::getInstance()->getUid().c_str());
            char szHttpTag[] = HTTP_REQUEST_REWARD_TAG;
            CCLog("post 鼓舞 = %s", szPostBuffer);
            m_pNetConnection->commitPostRequestData(szPostBuffer, szHttpTag);
        }
    }
}
void GuoZCityLayer::_resetAtt(){
    return;
    CCLog("重置攻击");
    char szPostBuffer[MAX_POSTTO_SERVER_BUFFER_SIZE];
    memset(szPostBuffer, '\0', MAX_POSTTO_SERVER_BUFFER_SIZE);
    sprintf(szPostBuffer, "c=cwar&a=killcd&uid=%s",
            GamePlayer::getInstance()->getUid().c_str());
    char szHttpTag[] = HTTP_REQUEST_KILLCD_TAG;
    CCLog("post 重置攻击 = %s", szPostBuffer);
    m_pNetConnection->commitPostRequestData(szPostBuffer, szHttpTag);
}
void GuoZCityLayer::_build(){
    CCLog("建造");
    
    if (m_isDefending) {
        if (m_pGamePlayer->getGold() < m_pGameState->getCWarBuildWall()) {
            const char* title = "元宝不足，无法建造城墙";
            MessageBox::getInstance()->messageTips(title);
            return;
        }
        
        char szPostBuffer[MAX_POSTTO_SERVER_BUFFER_SIZE];
        memset(szPostBuffer, '\0', MAX_POSTTO_SERVER_BUFFER_SIZE);
        sprintf(szPostBuffer, "c=cwar&a=add&city=%d&type=wall&uid=%s", m_pBattle->getSelectCity(),
                GamePlayer::getInstance()->getUid().c_str());
        char szHttpTag[] = HTTP_REQUEST_BUILD_TAG;
        CCLog("post 建造 = %s", szPostBuffer);
        m_pNetConnection->commitPostRequestData(szPostBuffer, szHttpTag);
    } else {
        if (m_pGamePlayer->getGold() < m_pGameState->getCWarBuildCar()) {
            const char* title = "元宝不足,无法建造投石车";
            MessageBox::getInstance()->messageTips(title);
            return;
        }
        
        char szPostBuffer[MAX_POSTTO_SERVER_BUFFER_SIZE];
        memset(szPostBuffer, '\0', MAX_POSTTO_SERVER_BUFFER_SIZE);
        sprintf(szPostBuffer, "c=cwar&a=add&city=%d&type=car&uid=%s", m_pBattle->getSelectCity(),
                GamePlayer::getInstance()->getUid().c_str());
        char szHttpTag[] = HTTP_REQUEST_BUILD_TAG;
        CCLog("post 建造 = %s", szPostBuffer);
        m_pNetConnection->commitPostRequestData(szPostBuffer, szHttpTag);
    }
}

/*tipMenu*/
void GuoZCityLayer::_tipMenu(int type) {
    if (TYPE_IN == type) {
        CCNode* left =  (CCNode*)this->getChildByTag(TIPMENU_LEFT_TAG);
        left->setPosition(ccp(-160.0f, 0));
        left->runAction(CCMoveTo::create(DURATION_TIME, ccp(0, 0)));
        CCNode* right =  (CCNode*)this->getChildByTag(TIPMENU_RIGHT_TAG);
        right->setPosition(ccp(160.0f, 0));
        right->runAction(CCMoveTo::create(DURATION_TIME, ccp(0, 0)));
        CCNode* mid =  (CCNode*)this->getChildByTag(TIPMENU_MID_TAG);
        mid->setPosition(ccp(0, 80.0f));
        mid->runAction(CCMoveTo::create(DURATION_TIME, ccp(0, 0)));
    } else if (TYPE_OUT == type){
        CCNode* left =  (CCNode*)this->getChildByTag(TIPMENU_LEFT_TAG);
        left->setPosition(ccp(0, 0));
        left->runAction(CCMoveTo::create(DURATION_TIME, ccp(-160.0f, 0)));
        CCNode* right =  (CCNode*)this->getChildByTag(TIPMENU_RIGHT_TAG);
        right->setPosition(ccp(0, 0));
        right->runAction(CCMoveTo::create(DURATION_TIME, ccp(160.0f, 0)));
        CCNode* mid =  (CCNode*)this->getChildByTag(TIPMENU_MID_TAG);
        mid->setPosition(ccp(0, 0));
        mid->runAction(CCMoveTo::create(DURATION_TIME, ccp(0, 80.0f)));
    }
}
/*返回上层*/
void GuoZCityLayer::_returnForm() {
    /*返回到国战初始界面拉数据*/
    NetConnection* pNetConnection = NetConnection::getInstance();
    char szPostBuffer[MAX_POSTTO_SERVER_BUFFER_SIZE];
    memset(szPostBuffer, '\0', MAX_POSTTO_SERVER_BUFFER_SIZE);
    strcpy(szPostBuffer, "c=cwar&a=enter");
    char szHttpTag[] = HTTP_REQUEST_ENTERCWAR_TAG;
    strcat(szPostBuffer, "&uid=");
    strcat(szPostBuffer, GamePlayer::getInstance()->getUid().c_str());
    CCLog("post 进入国战 = %s", szPostBuffer);
    pNetConnection->commitPostRequestData(szPostBuffer, szHttpTag);
}

/*更新文字信息*/
void GuoZCityLayer::_updateFontInfo() {
//    CCNode* pNode = (CCNode*)this->getChildByTag(NODE_TAG);
}

/*冷却时间调度*/
void GuoZCityLayer::_scheduleForCooling() {
    
}
//
void GuoZCityLayer::_checkForWitch(int camp, int cityId) {
    CCSprite* pBgSprite = (CCSprite*)m_pSBN->getChildByTag(GUOZ_CITY_BG_TAG);
    CCSprite* sp = NULL;
    if (cityId == 1)/*洛阳*/ {
        if (camp == 1)/*魏国*/ {
            sp = CCSprite::createWithSpriteFrameName("gz_chA_blu.png");
        } else if (camp == 2) {
            sp = CCSprite::createWithSpriteFrameName("gz_chA_red.png");
        } else if (camp == 3) {
            sp = CCSprite::createWithSpriteFrameName("gz_chA_gre.png");
        } else/*中立*/ {
            sp = CCSprite::createWithSpriteFrameName("gz_zhong_ca.png");
        }
    } else if (cityId == 2) {
        if (camp == 1)/*魏国*/ {
            sp = CCSprite::createWithSpriteFrameName("gz_luoy_blu.png");
        } else if (camp == 2) {
            sp = CCSprite::createWithSpriteFrameName("gz_luoy_red.png");
        } else if (camp == 3) {
            sp = CCSprite::createWithSpriteFrameName("gz_luoy_gre.png");
        } else/*中立*/ {
            sp = CCSprite::createWithSpriteFrameName("gz_zhong_ly.png");
        }
    } else {
        if (camp == 1)/*魏国*/ {
            sp = CCSprite::createWithSpriteFrameName("gz_chl_blu.png");
        } else if (camp == 2) {
            sp = CCSprite::createWithSpriteFrameName("gz_chl_red.png");
        } else if (camp == 3) {
            sp = CCSprite::createWithSpriteFrameName("gz_chl_gre.png");
        } else/*中立*/ {
            sp = CCSprite::createWithSpriteFrameName("gz_zhong_cl.png");
        }
    }
    sp->setAnchorPoint(CCPointZero);
    sp->setPosition(ccp(17.0f, 445.0f));
    pBgSprite->addChild(sp, 2);
}

void GuoZCityLayer::setVisible(bool visible) {
    CCLayer::setVisible(visible);
    m_thisTouch = visible;
}

// 动画接口
void GuoZCityLayer::aniForAtkOrDef() {
    if (m_isDefending) {
        if (m_isInList == 1) {
            m_income[0]->stopAllActions();
            m_income[1]->stopAllActions();
            m_btn[1][0]->runAction(CCMoveBy::create(DURATION_TIME, ccp(-80.0f, 0.0f)));
            m_btn[1][1]->runAction(CCMoveBy::create(DURATION_TIME, ccp(-80.0f, 0.0f)));
            m_pDefFont->runAction(CCSpawn::create(CCMoveBy::create(DURATION_TIME - 0.1, ccp(-40.0f, 0.0f)),
                                                  CCFadeOut::create(0.2),NULL));
            m_pHold[0]->setVisible(true);
            m_pHold[1]->setVisible(true);
            m_pHold[0]->runAction(CCSequence::create(CCDelayTime::create(0.2),
                                                     CCSpawn::create(CCMoveBy::create(0.1, ccp(-40.0f, 0.0f)),
                                                                     CCFadeIn::create(0.1),NULL), NULL));
            m_pHold[1]->runAction(CCSequence::create(CCDelayTime::create(0.2),
                                                     CCSpawn::create(CCMoveBy::create(0.1, ccp(-40.0f, 0.0f)),
                                                                     CCFadeIn::create(0.1),NULL), NULL));
            schedule(schedule_selector(GuoZCityLayer::_scheduleForAttOrDef), 1);
            m_income[0]->runAction(CCSequence::create(CCDelayTime::create(1.1), CCFadeIn::create(0.1),NULL));
            m_income[1]->runAction(CCSequence::create(CCDelayTime::create(1.2), CCFadeIn::create(0.1),NULL));
            
            m_isInList = 2;
        } else {
            m_income[0]->stopAllActions();
            m_income[1]->stopAllActions();
            m_btn[1][0]->runAction(CCMoveBy::create(DURATION_TIME, ccp(80.0f, 0.0f)));
            m_btn[1][1]->runAction(CCMoveBy::create(DURATION_TIME, ccp(80.0f, 0.0f)));
            m_pDefFont->runAction(CCSequence::create(CCDelayTime::create(0.1),
                                                     CCSpawn::create(CCMoveBy::create(0.2, ccp(40.0f, 0.0f)),
                                                                     CCFadeIn::create(0.2),NULL), NULL));
            m_pHold[0]->runAction(CCSequence::create(CCSpawn::create(CCMoveBy::create(0.1, ccp(40.0f, 0.0f)),
                                                                     CCFadeOut::create(0.1),NULL), NULL));
            m_pHold[1]->runAction(CCSequence::create(CCSpawn::create(CCMoveBy::create(0.1, ccp(40.0f, 0.0f)),
                                                                     CCFadeOut::create(0.1),NULL), NULL));
            m_listTimeLabel->setVisible(false);
            unschedule(schedule_selector(GuoZCityLayer::_scheduleForAttOrDef));
            m_income[0]->runAction(CCSequence::create(CCDelayTime::create(0.1), CCFadeOut::create(0.1), NULL));
            m_income[1]->runAction(CCSequence::create(CCDelayTime::create(0.3), CCFadeOut::create(0.1), NULL));
            
            m_isInList = 1;
        }
    } else {
        if (m_isInList == 1) {
            m_income[0]->stopAllActions();
            m_income[1]->stopAllActions();
            m_btn[1][0]->runAction(CCMoveBy::create(DURATION_TIME, ccp(-80.0f, 0.0f)));
            m_btn[1][1]->runAction(CCMoveBy::create(DURATION_TIME, ccp(-80.0f, 0.0f)));
            m_pAttFont->runAction(CCSpawn::create(CCMoveBy::create(DURATION_TIME - 0.1, ccp(-40.0f, 0.0f)),
                                                  CCFadeOut::create(0.2),NULL));
            m_pHold[0]->setVisible(true);
            m_pHold[1]->setVisible(true);
            m_pHold[0]->runAction(CCSequence::create(CCDelayTime::create(0.2),
                                                     CCSpawn::create(CCMoveBy::create(0.1, ccp(-40.0f, 0.0f)),
                                                                     CCFadeIn::create(0.1),NULL), NULL));
            m_pHold[1]->runAction(CCSequence::create(CCDelayTime::create(0.2),
                                                     CCSpawn::create(CCMoveBy::create(0.1, ccp(-40.0f, 0.0f)),
                                                                     CCFadeIn::create(0.1),NULL), NULL));
            schedule(schedule_selector(GuoZCityLayer::_scheduleForAttOrDef), 1);
            m_income[0]->runAction(CCSequence::create(CCDelayTime::create(1.1), CCFadeIn::create(0.1),NULL));
            m_income[1]->runAction(CCSequence::create(CCDelayTime::create(1.2), CCFadeIn::create(0.1),NULL));
            
            m_isInList = 2;
        } else {
            m_income[0]->stopAllActions();
            m_income[1]->stopAllActions();
            m_btn[1][0]->runAction(CCMoveBy::create(DURATION_TIME, ccp(80.0f, 0.0f)));
            m_btn[1][1]->runAction(CCMoveBy::create(DURATION_TIME, ccp(80.0f, 0.0f)));
            m_pAttFont->runAction(CCSequence::create(CCDelayTime::create(0.1),
                                                     CCSpawn::create(CCMoveBy::create(0.2, ccp(40.0f, 0.0f)),
                                                                     CCFadeIn::create(0.2),NULL), NULL));
            m_pHold[0]->runAction(CCSequence::create(CCSpawn::create(CCMoveBy::create(0.1, ccp(40.0f, 0.0f)),
                                                                     CCFadeOut::create(0.1),NULL), NULL));
            m_pHold[1]->runAction(CCSequence::create(CCSpawn::create(CCMoveBy::create(0.1, ccp(40.0f, 0.0f)),
                                                                     CCFadeOut::create(0.1),NULL), NULL));
            m_listTimeLabel->setVisible(false);
            unschedule(schedule_selector(GuoZCityLayer::_scheduleForAttOrDef));
            m_income[0]->runAction(CCSequence::create(CCDelayTime::create(0.1), CCFadeOut::create(0.1), NULL));
            m_income[1]->runAction(CCSequence::create(CCDelayTime::create(0.3), CCFadeOut::create(0.1), NULL));
            
            m_isInList = 1;
        }
    }
}
/*
 * 初始化动画状态
 * m_typeOfAtkOrDef
 *      1,成功 2， 失败 3，进行， 0 正常
*/
void GuoZCityLayer::_initAniAtkOrDef() {
    if (m_isDefending) {
        if (m_typeOfAtkOrDef == 0) {
            m_isInList = 1;
        } else if (m_typeOfAtkOrDef == 1) {
            m_pHold[0]->setString("防御成功");
            m_pHold[0]->setColor(YELLOW_COLOR);
            m_pHold[1]->setString("领取奖励");
            m_pHold[1]->setColor(GREEN_COLOR);
            m_pHold[0]->setOpacity(255);
            m_pHold[1]->setOpacity(255);
            m_btn[1][0]->setPosition(ccp(305.0f, 45.0f));
            m_btn[1][1]->setPosition(ccp(305.0f, 45.0f));
            m_pDefFont->setOpacity(0);
            m_pDefFont->setPosition(ccp(395.0f, 118.0f));
            m_pHold[0]->setVisible(true);
            m_pHold[1]->setVisible(true);
            m_pHold[0]->setPosition(ccp(400.0f, 275.0f));
            m_pHold[1]->setPosition(ccp(403.0f, 240.0f));
            m_income[0]->setOpacity(255);
            m_income[1]->setOpacity(255);
            schedule(schedule_selector(GuoZCityLayer::_scheduleForAttOrDef), 1);
            
            m_isInList = 2;
        } else if (m_typeOfAtkOrDef == 2) {
            m_pHold[0]->setString("防御失败");
            m_pHold[0]->setColor(RED_COLOR);
            m_pHold[1]->setString("领取奖励");
            m_pHold[1]->setColor(GREEN_COLOR);
            m_pHold[0]->setOpacity(255);
            m_pHold[1]->setOpacity(255);
            m_btn[1][0]->setPosition(ccp(305.0f, 45.0f));
            m_btn[1][1]->setPosition(ccp(305.0f, 45.0f));
            m_pDefFont->setOpacity(0);
            m_pDefFont->setPosition(ccp(395.0f, 118.0f));
            m_pHold[0]->setVisible(true);
            m_pHold[1]->setVisible(true);
            m_pHold[0]->setPosition(ccp(400.0f, 275.0f));
            m_pHold[1]->setPosition(ccp(403.0f, 240.0f));
            m_income[0]->setOpacity(255);
            m_income[1]->setOpacity(255);
            schedule(schedule_selector(GuoZCityLayer::_scheduleForAttOrDef), 1);
            
            m_isInList = 2;
        } else if (m_typeOfAtkOrDef == 3) {
            m_pHold[0]->setString("防御中");
            m_pHold[0]->setColor(YELLOW_COLOR);
            m_pHold[1]->setString("点击退出");
            m_pHold[1]->setColor(WHITE_COLOR);
            m_pHold[0]->setOpacity(255);
            m_pHold[1]->setOpacity(255);
            m_btn[1][0]->setPosition(ccp(305.0f, 45.0f));
            m_btn[1][1]->setPosition(ccp(305.0f, 45.0f));
            m_pDefFont->setOpacity(0);
            m_pDefFont->setPosition(ccp(395.0f, 118.0f));
            m_pHold[0]->setVisible(true);
            m_pHold[1]->setVisible(true);
            m_pHold[0]->setPosition(ccp(400.0f, 275.0f));
            m_pHold[1]->setPosition(ccp(403.0f, 240.0f));
            m_income[0]->setOpacity(255);
            m_income[1]->setOpacity(255);
            schedule(schedule_selector(GuoZCityLayer::_scheduleForAttOrDef), 1);
            
            m_isInList = 2;
        }
        
    } else {
        if (m_typeOfAtkOrDef == 0) {
            m_isInList = 1;
        } else if (m_typeOfAtkOrDef == 1) {
            m_pHold[0]->setString("占领成功");
            m_pHold[0]->setColor(YELLOW_COLOR);
            m_pHold[1]->setString("领取奖励");
            m_pHold[1]->setColor(GREEN_COLOR);
            m_pHold[0]->setOpacity(255);
            m_pHold[1]->setOpacity(255);
            m_btn[1][0]->setPosition(ccp(305.0f, 45.0f));
            m_btn[1][1]->setPosition(ccp(305.0f, 45.0f));
            m_pAttFont->setOpacity(0);
            m_pAttFont->setPosition(ccp(395.0f, 118.0f));
            m_pHold[0]->setVisible(true);
            m_pHold[1]->setVisible(true);
            m_pHold[0]->setPosition(ccp(400.0f, 275.0f));
            m_pHold[1]->setPosition(ccp(403.0f, 240.0f));
            m_income[0]->setOpacity(255);
            m_income[1]->setOpacity(255);
            schedule(schedule_selector(GuoZCityLayer::_scheduleForAttOrDef), 1);
            
            m_isInList = 2;
        } else if (m_typeOfAtkOrDef == 2) {
            m_pHold[0]->setString("占领失败");
            m_pHold[0]->setColor(RED_COLOR);
            m_pHold[1]->setString("领取奖励");
            m_pHold[1]->setColor(GREEN_COLOR);
            m_pHold[0]->setOpacity(255);
            m_pHold[1]->setOpacity(255);
            m_btn[1][0]->setPosition(ccp(305.0f, 45.0f));
            m_btn[1][1]->setPosition(ccp(305.0f, 45.0f));
            m_pAttFont->setOpacity(0);
            m_pAttFont->setPosition(ccp(395.0f, 118.0f));
            m_pHold[0]->setVisible(true);
            m_pHold[1]->setVisible(true);
            m_pHold[0]->setPosition(ccp(400.0f, 275.0f));
            m_pHold[1]->setPosition(ccp(403.0f, 240.0f));
            m_income[0]->setOpacity(255);
            m_income[1]->setOpacity(255);
            schedule(schedule_selector(GuoZCityLayer::_scheduleForAttOrDef), 1);
            
            m_isInList = 2;
        } else if (m_typeOfAtkOrDef == 3) {
            m_pHold[0]->setString("占领中");
            m_pHold[0]->setColor(YELLOW_COLOR);
            m_pHold[1]->setString("点击退出");
            m_pHold[1]->setColor(WHITE_COLOR);
            m_pHold[0]->setOpacity(255);
            m_pHold[1]->setOpacity(255);
            m_btn[1][0]->setPosition(ccp(305.0f, 45.0f));
            m_btn[1][1]->setPosition(ccp(305.0f, 45.0f));
            m_pAttFont->setOpacity(0);
            m_pAttFont->setPosition(ccp(395.0f, 118.0f));
            m_pHold[0]->setVisible(true);
            m_pHold[1]->setVisible(true);
            m_pHold[0]->setPosition(ccp(400.0f, 275.0f));
            m_pHold[1]->setPosition(ccp(403.0f, 240.0f));
            m_income[0]->setOpacity(255);
            m_income[1]->setOpacity(255);
            schedule(schedule_selector(GuoZCityLayer::_scheduleForAttOrDef), 1);
            
            m_isInList = 2;
        }
    }
}

void GuoZCityLayer::setInListStatus(int status) {
    m_isInList = status;
}
// 冷却计时
void GuoZCityLayer::_coolForFight() {
    char buffer[16];
    int min = 0;
    int sec = 0;
    min = (int)(m_timeForCooling / 60);
    sec = m_timeForCooling % 60;
    if (--m_timeForCooling < 0) {
        m_timeForCooling = 0;
        m_labelCooling->setVisible(false);
        m_isCooling = false;
        unschedule(schedule_selector(GuoZCityLayer::_coolForFight));
    } else {
        m_isCooling = true;
        sprintf(buffer, "%02d:%02d", min, sec);
        m_labelCooling->setString(buffer);
    }
}

/*列表中倒计时*/
void GuoZCityLayer::_scheduleForAttOrDef() {
    char buffer[32];
    int min = 0;
    int sec = 0;
    min = (int)(m_listTime / 60);
    sec = m_listTime % 60;
    if (--m_listTime < 0)/*倒计时完毕*/ {
        m_listTime = 0;
        m_listTimeLabel->setVisible(false);
        unschedule(schedule_selector(GuoZCityLayer::_scheduleForAttOrDef));
    } else {
        if (m_listTime != 0 && (m_listTime % 60 == 0))/*一分钟*/ {
            /*累计奖励*/
            int awardCop = (int)((float)(m_defTime / 60)*
                                 m_pBattle->getDefLevMny()*(1.0f + m_pBattle->getMnyBuff()));
            CCLog("deftime = %d, %f, %f", m_defTime, m_pBattle->getDefLevMny(), m_pBattle->getMnyBuff());
            sprintf(buffer, "铜钱收入%d",awardCop);
            m_income[0]->setString(buffer);
        }
        m_listTimeLabel->setVisible(true);
        sprintf(buffer, "剩余时间:%02d:%02d", min, sec);
        m_listTimeLabel->setString(buffer);
        m_defTime++;
    }
}