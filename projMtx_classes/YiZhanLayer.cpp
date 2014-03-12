#include "YiZhanLayer.h"
#include "NetConnection.h"
#include "common.h"
#include "customMessage.h"
#include "YiZhanShowLayer.h"
#include "GamePlayer.h"
#include "GameState.h"

#define GREEN_COLOR             ccc3(0, 255, 0)
#define RED_COLOR               ccc3(255, 0, 0)
#define YELLOW_COLOR            ccc3(255, 255, 0)

#define OPENNING_ANI_DURING     0.2

#define BTN_OFFSET_BG           210
#define BTN_OFFSET_BGICON       215
#define WINE_OFFSET             207
#define BTN_SCENE_OFFSET        200             // 按钮艰巨

#define CONTAINER_TAG           1
#define BTN_CHOU                100
#define BTN_SCENE               150             //  武将 宝物 国战寻宝场景切换
#define BTN_SCENE_SEL           155             //  选中状态
#define BTN_SCENE_FONT          160             //  场景切换文字显示
#define BTN_SCENE_MENU          165             //  场景切换按钮的 menu tag

#define COURAGE_REMAIN_TIMES    200             // 勇气值抽取剩余次数
#define COURAGE_REMAIN          500             // 剩余勇气值
#define GOLD_REMAIN             501             // 220剩余元宝
#define GOLD_REMAIN_TEN         502             // 2000剩余元宝
#define RMBONE_REMAIN_TIMES     210             // 人民币抽一次剩余次数
#define RMBTEN_REMAIN_TIMES     220             // 人民币抽十次剩余次数

#define TIELIAN_TAG             250             // 小
#define TIELIAN_TAG1            260             // 大
#define BGOFSHOWTIME            270             // 显示次数背景

#define MOVEOUT_TAG                         300
#define MOVEIN_TAG                          301

#define ANIMATION_DURING                    0.2

/*
 *  构造函数
 */
YiZhanLayer::YiZhanLayer():
m_pGameState(GameState::getInstance())
,m_pGamePlayer(GamePlayer::getInstance())
{}

/*
 *  析构函数
 */
YiZhanLayer::~YiZhanLayer() {
    CCTextureCache* pTextureCache = CCTextureCache::sharedTextureCache();
    
    pTextureCache->removeTextureForKey("MC_yiZ.png");
    pTextureCache->removeTextureForKey("MC_touM.png");
    pTextureCache->removeTextureForKey("MC_rmb1.png");
    pTextureCache->removeTextureForKey("MC_rmb1s.png");
    pTextureCache->removeTextureForKey("MC_courSerach.png");
    pTextureCache->removeTextureForKey("MC_wWine.png");
    pTextureCache->removeTextureForKey("MC_courage.png");
    pTextureCache->removeTextureForKey("MC_rmbSearch.png");
    pTextureCache->removeTextureForKey("MC_rmb10.png");
    pTextureCache->removeTextureForKey("MC_rmb10s.png");
    pTextureCache->removeTextureForKey("MC_rmbSearch1.png");
    pTextureCache->removeTextureForKey("MC_bWine.png");
    pTextureCache->removeTextureForKey("MC_oWine.png");
    pTextureCache->removeTextureForKey("MC_bgOfBtn.png");
    pTextureCache->removeTextureForKey("MC_yiZtip1.png");
    pTextureCache->removeTextureForKey("MC_yiZtip2.png");
    pTextureCache->removeTextureForKey("MC_yiZtip3.png");
    pTextureCache->removeTextureForKey("MC_fenge.png");
    pTextureCache->removeTextureForKey("MC_yiSmall.png");
    pTextureCache->removeTextureForKey("MC_yiBig.png");
    pTextureCache->removeTextureForKey("MC_tieLian.png");
    pTextureCache->removeTextureForKey("MC_tieLians.png");
    pTextureCache->removeTextureForKey("MC_fontSeach1.png");
    pTextureCache->removeTextureForKey("MC_fontSeach2.png");
    pTextureCache->removeTextureForKey("MC_fontSeach3.png");
    pTextureCache->removeTextureForKey("MC_fontSeach1h.png");
    pTextureCache->removeTextureForKey("MC_fontSeach2h.png");
    pTextureCache->removeTextureForKey("MC_fontSeach3h.png");
    pTextureCache->removeTextureForKey("MC_remainTimeBg.png");
}

/*
 *  初始化
 */
bool YiZhanLayer::init() {
    // super init
    if (!CCLayer::init()) {
        return false;
    }

    CCNode* pNode = CCNode::create();
    pNode->setAnchorPoint(ccp(0, 0));
    pNode->setPosition(ccp(0, m_pGameState->getBottomOffset()));
    addChild(pNode, 1, CONTAINER_TAG);
    
    /*驿站背景*/
    CCSprite* yiBg = CCSprite::create("MC_yiZ.png");
    yiBg->setAnchorPoint(ccp(0, 0));
    yiBg->setPosition(ccp(0, 115));
    pNode->addChild(yiBg);
    CCSprite* bigBg = CCSprite::create("MC_touM.png");
    bigBg->setAnchorPoint(ccp(0, 0));
    bigBg->setPosition(ccp(0, 100));
    pNode->addChild(bigBg);
    
    CCMenuItemImage *pFree = CCMenuItemImage::create("MC_rmb1.png",
                                                          "MC_rmb1s.png",
                                                          this,
                                                          menu_selector(YiZhanLayer::chosForLucky));
    pFree->setTag(BTN_CHOU);
    pFree->setAnchorPoint(CCPointZero);
    pFree->setPosition(ccp(15, 287));
    CCMenu* pFreeMenu = CCMenu::create(pFree, NULL);
    pFreeMenu->setAnchorPoint(ccp(0, 0));
    pFreeMenu->setPosition(ccp(0, 0));
    pNode->addChild(pFreeMenu, 5);
    // 显示文字
    CCSprite* pFreeFont = CCSprite::create("MC_courSerach.png");
    pFreeFont->setAnchorPoint(ccp(0, 0));
    pFreeFont->setPosition(ccp(40, 287));
    pNode->addChild(pFreeFont, 6);
    /*酒碗ui*/
    CCMenuItemImage *pFree1 = CCMenuItemImage::create("MC_wWine.png",
                                                     "MC_wWine.png",
                                                     this,
                                                     menu_selector(YiZhanLayer::chosForLucky));
    pFree1->setTag(BTN_CHOU);
    pFree1->setAnchorPoint(CCPointZero);
    pFree1->setPosition(ccp(32, 350));
    CCMenu* pFreeMenu1 = CCMenu::create(pFree1, NULL);
    pFreeMenu1->setAnchorPoint(ccp(0, 0));
    pFreeMenu1->setPosition(ccp(0, 0));
    pNode->addChild(pFreeMenu1, 5);
    
    CCMenuItemImage *pNFree = CCMenuItemImage::create("MC_courage.png",
                                                     "MC_courages.png",
                                                     this,
                                                     menu_selector(YiZhanLayer::chosForLucky));
    pNFree->setTag(BTN_CHOU+1);
    pNFree->setAnchorPoint(CCPointZero);
    pNFree->setPosition(ccp(17 + WINE_OFFSET, 287));
    CCMenu* pNFreeMenu = CCMenu::create(pNFree, NULL);
    pNFreeMenu->setAnchorPoint(ccp(0, 0));
    pNFreeMenu->setPosition(ccp(0, 0));
    pNode->addChild(pNFreeMenu, 5);
    // 显示文字
    CCSprite* pNFreeFont = CCSprite::create("MC_rmbSearch.png");
    pNFreeFont->setAnchorPoint(ccp(0, 0));
    pNFreeFont->setPosition(ccp(40 + WINE_OFFSET, 287));
    pNode->addChild(pNFreeFont, 6);
    CCMenuItemImage *pNFree1 = CCMenuItemImage::create("MC_bWine.png",
                                                      "MC_bWine.png",
                                                      this,
                                                      menu_selector(YiZhanLayer::chosForLucky));
    pNFree1->setTag(BTN_CHOU+1);
    pNFree1->setAnchorPoint(CCPointZero);
    pNFree1->setPosition(ccp(32 + WINE_OFFSET, 350));
    CCMenu* pNFreeMenu1 = CCMenu::create(pNFree1, NULL);
    pNFreeMenu1->setAnchorPoint(ccp(0, 0));
    pNFreeMenu1->setPosition(ccp(0, 0));
    pNode->addChild(pNFreeMenu1, 5);
    
    CCMenuItemImage *pNFreeTen = CCMenuItemImage::create("MC_rmb10.png",
                                                     "MC_rmb10s.png",
                                                     this,
                                                     menu_selector(YiZhanLayer::chosForLucky));
    pNFreeTen->setTag(BTN_CHOU+2);
    pNFreeTen->setAnchorPoint(CCPointZero);
    pNFreeTen->setPosition(ccp(17 + WINE_OFFSET*2, 287));
    CCMenu* pNFreeTenMenu = CCMenu::create(pNFreeTen, NULL);
    pNFreeTenMenu->setAnchorPoint(ccp(0, 0));
    pNFreeTenMenu->setPosition(ccp(0, 0));
    pNode->addChild(pNFreeTenMenu, 5);
    // 显示文字
    CCSprite* pNFreeTenFont = CCSprite::create("MC_rmbSearch1.png");
    pNFreeTenFont->setAnchorPoint(ccp(0, 0));
    pNFreeTenFont->setPosition(ccp(40 + WINE_OFFSET*2, 287));
    pNode->addChild(pNFreeTenFont, 6);
    CCMenuItemImage *pNFreeTen1 = CCMenuItemImage::create("MC_oWine.png",
                                                         "MC_oWine.png",
                                                         this,
                                                         menu_selector(YiZhanLayer::chosForLucky));
    pNFreeTen1->setTag(BTN_CHOU+2);
    pNFreeTen1->setAnchorPoint(CCPointZero);
    pNFreeTen1->setPosition(ccp(32 + WINE_OFFSET*2, 350));
    CCMenu* pNFreeTenMenu1 = CCMenu::create(pNFreeTen1, NULL);
    pNFreeTenMenu1->setAnchorPoint(ccp(0, 0));
    pNFreeTenMenu1->setPosition(ccp(0, 0));
    pNode->addChild(pNFreeTenMenu1, 5);
    
    /* 按钮背景 */
    for (int i = 0; i < 3; ++i) {
        CCSprite* btnBg = CCSprite::create("MC_bgOfBtn.png");
        btnBg->setAnchorPoint(ccp(0, 0));
        btnBg->setPosition(ccp(8 + i * BTN_OFFSET_BG, 110));
        pNode->addChild(btnBg);
        CCString* s = CCString::createWithFormat("MC_yiZtip%d.png", i+1);
        CCSprite* btnBgIcon = CCSprite::create(s->getCString());
        btnBgIcon->setAnchorPoint(ccp(0, 0));
        btnBgIcon->setPosition(ccp(60 + i * BTN_OFFSET_BGICON, 155));
        pNode->addChild(btnBgIcon);
        CCSprite* spFenge = CCSprite::create("MC_fenge.png");
        spFenge->setAnchorPoint(ccp(0, 0));
        spFenge->setPosition(ccp(30 + i * BTN_OFFSET_BGICON, 210));
        pNode->addChild(spFenge);
    }
    
    /* 所需要的勇气值（剩余勇气值），元宝 */
    // 拥有的要勇气
    CCLabelTTF*labCourage1 = CCLabelTTF::create("当前勇气:", "Arial", 21);
    labCourage1->setAnchorPoint(ccp(0, 0));
    labCourage1->setPosition(ccp(35 + 0 * BTN_OFFSET_BGICON, 220));
    pNode->addChild(labCourage1);
    char buffer[64];
    sprintf(buffer, "%d", m_pGamePlayer->getCourage());
    CCLabelTTF* labCourage = CCLabelTTF::create(buffer, "Arial", 21);
    labCourage->setAnchorPoint(ccp(0, 0));
    labCourage->setPosition(ccp(130 + 0 * BTN_OFFSET_BGICON, 220));
    pNode->addChild(labCourage, 5);
    // 需要的勇气
    CCLabelTTF*labCourageRemain1 = CCLabelTTF::create("所需勇气:", "Arial", 21);
    labCourageRemain1->setAnchorPoint(ccp(0, 0));
    labCourageRemain1->setPosition(ccp(35 + 0 * BTN_OFFSET_BGICON, 250));
    pNode->addChild(labCourageRemain1);
    sprintf(buffer, "%d", m_pGamePlayer->getLuckCourage());
    CCLabelTTF* labCourageRemain = CCLabelTTF::create(buffer, "Arial", 21);
    labCourageRemain->setAnchorPoint(ccp(0, 0));
    labCourageRemain->setPosition(ccp(130 + 0 * BTN_OFFSET_BGICON, 250));
    pNode->addChild(labCourageRemain, 5, COURAGE_REMAIN);
    if (m_pGamePlayer->getCourage() < m_pGamePlayer->getLuckCourage()) {
        labCourageRemain->setColor(RED_COLOR);
    } else {
        labCourageRemain->setColor(GREEN_COLOR);
    }
    // 需要元宝
    CCLabelTTF* rmb11 = CCLabelTTF::create("所需元宝:", "Arial", 21);
    rmb11->setAnchorPoint(ccp(0, 0));
    rmb11->setPosition(ccp(35 + 1 * BTN_OFFSET_BGICON, 235));
    pNode->addChild(rmb11, 5);
    sprintf(buffer, "%d", m_pGamePlayer->getLuckRMB());
    CCLabelTTF* rmb1 = CCLabelTTF::create(buffer, "Arial", 21);
    rmb1->setAnchorPoint(ccp(0, 0));
    rmb1->setPosition(ccp(130 + 1 * BTN_OFFSET_BGICON, 235));
    pNode->addChild(rmb1, 5, GOLD_REMAIN);
    if (m_pGamePlayer->getGold() < m_pGamePlayer->getLuckRMB()) {
        rmb1->setColor(RED_COLOR);
    } else {
        rmb1->setColor(GREEN_COLOR);
    }
    // 抽10次所需元宝
    CCLabelTTF* rmb101 = CCLabelTTF::create("所需元宝:", "Arial", 21);
    rmb101->setAnchorPoint(ccp(0, 0));
    rmb101->setPosition(ccp(35 + 2 * BTN_OFFSET_BGICON, 235));
    pNode->addChild(rmb101, 5);
    sprintf(buffer, "%d", m_pGamePlayer->getLuckRMBBig());
    CCLabelTTF* rmb10 = CCLabelTTF::create(buffer, "Arial", 21);
    rmb10->setAnchorPoint(ccp(0, 0));
    rmb10->setPosition(ccp(130 + 2 * BTN_OFFSET_BGICON, 235));
    pNode->addChild(rmb10, 5, GOLD_REMAIN_TEN);
    if (m_pGamePlayer->getGold() < m_pGamePlayer->getLuckRMBBig()) {
        rmb10->setColor(RED_COLOR);
    } else {
        rmb10->setColor(GREEN_COLOR);
    }
    
    /*
     * 切换场景
     *      武将，宝物，国战寻宝
     */
    for (int i = 0; i < 3; ++i) {
        CCMenuItemImage *pHeroSearch = CCMenuItemImage::create("MC_yiSmall.png",
                                                               "MC_yiSmall.png",
                                                               this,
                                                               menu_selector(YiZhanLayer::chosTypeForSearch));
        pHeroSearch->setTag(BTN_SCENE + i);
        pHeroSearch->setAnchorPoint(ccp(0.5, 0));
        pHeroSearch->setPosition(ccp(95 + i * (BTN_SCENE_OFFSET-30), 705));
        CCMenu* pSearchMenu = CCMenu::create(pHeroSearch, NULL);
        pSearchMenu->setAnchorPoint(ccp(0, 0));
        pSearchMenu->setPosition(ccp(0, 0));
        pNode->addChild(pSearchMenu, 2, BTN_SCENE_MENU + i);
        pSearchMenu->setEnabled(false);
        /*选中*/
        CCSprite* pSearchSel = CCSprite::create("MC_yiBig.png");
        pSearchSel->setAnchorPoint(ccp(0.5, 0));
        pSearchSel->setPosition(ccp(94 + i * (BTN_SCENE_OFFSET-30), 685));
        pNode->addChild(pSearchSel, 3, BTN_SCENE_SEL + i);
        pSearchSel->setVisible(false);
        /*铁链*/
        CCSprite* spTieLian = CCSprite::create("MC_tieLian.png");
        spTieLian->setAnchorPoint(ccp(0, 0));
        spTieLian->setPosition(ccp(40 + i * (BTN_SCENE_OFFSET-30), 780));
        pNode->addChild(spTieLian, 1, TIELIAN_TAG1 + i);
        spTieLian->setVisible(false);
        CCSprite* spTieLian2 = CCSprite::create("MC_tieLian.png");
        spTieLian2->setAnchorPoint(ccp(0, 0));
        spTieLian2->setPosition(ccp(120 + i * (BTN_SCENE_OFFSET-30), 780));
        pNode->addChild(spTieLian2, 1, TIELIAN_TAG1 + i + 5);
        spTieLian2->setVisible(false);
        CCSprite* sTieLian = CCSprite::create("MC_tieLians.png");
        sTieLian->setAnchorPoint(ccp(0, 0));
        sTieLian->setPosition(ccp(40 + i * (BTN_SCENE_OFFSET-30), 780));
        pNode->addChild(sTieLian, 1, TIELIAN_TAG + i);
        sTieLian->setVisible(true);
        CCSprite* sTieLian2 = CCSprite::create("MC_tieLians.png");
        sTieLian2->setAnchorPoint(ccp(0, 0));
        sTieLian2->setPosition(ccp(120 + i * (BTN_SCENE_OFFSET-30), 780));
        pNode->addChild(sTieLian2, 1, TIELIAN_TAG + i + 5);
        sTieLian2->setVisible(true);
        
        /*文字*/
        CCString* font = NULL;
        if (i == 0) {
            font = CCString::createWithFormat("MC_fontSeach%d.png", i + 1);
        } else {
            font = CCString::createWithFormat("MC_fontSeach%dh.png", i + 1);
        }
        CCSprite* spFont = CCSprite::create(font->getCString());
        spFont->setAnchorPoint(ccp(0.5, 0));
        spFont->setPosition(ccp(95 + i * (BTN_SCENE_OFFSET-30), 715));
        pNode->addChild(spFont, 6, BTN_SCENE_FONT + i);
    }
    /*默认第一个*/
    CCSprite* pSearchSel = (CCSprite*)pNode->getChildByTag(BTN_SCENE_SEL);
    pSearchSel->setVisible(true);
    CCSprite* spFont = (CCSprite*)pNode->getChildByTag(BTN_SCENE_FONT);
    spFont->setScale(1.2);
    CCMenu* pMe = (CCMenu*)pNode->getChildByTag(BTN_SCENE_MENU);
    pMe->setEnabled(true);
    CCSprite* tie = (CCSprite*)pNode->getChildByTag(TIELIAN_TAG1);
    tie->setVisible(true);
    CCSprite* tie2 = (CCSprite*)pNode->getChildByTag(TIELIAN_TAG1 + 5);
    tie2->setVisible(true);
    CCSprite* ties = (CCSprite*)pNode->getChildByTag(TIELIAN_TAG);
    ties->setVisible(false);
    CCSprite* ties2 = (CCSprite*)pNode->getChildByTag(TIELIAN_TAG + 5);
    ties2->setVisible(false);
    
    
    /* 显示抽将剩余次数 */
    for (int i = 0; i < 3; ++i) {
        CCSprite* sp = CCSprite::create("MC_remainTimeBg.png");
        sp->setAnchorPoint(ccp(0, 0));
        sp->setPosition(ccp(145 + i * BTN_OFFSET_BGICON, 327));
        pNode->addChild(sp, 1, BGOFSHOWTIME + i);
    }
    int remainTimes = 0;
    remainTimes = floor(m_pGamePlayer->getCourage() / m_pGamePlayer->getLuckCourage());
    sprintf(buffer, "%d", remainTimes);
    CCLabelTTF* courageTimes = CCLabelTTF::create(buffer, "Arial", 20);
    courageTimes->setAnchorPoint(ccp(0.5, 0));
    courageTimes->setPosition(ccp(182 + 0 * BTN_OFFSET_BGICON, 344));
    pNode->addChild(courageTimes, 2, COURAGE_REMAIN_TIMES);
    if (remainTimes == 0) {
        CCSprite* spShow = (CCSprite*)pNode->getChildByTag(BGOFSHOWTIME);
        spShow->setVisible(false);
        courageTimes->setVisible(false);
    }
    remainTimes = floor(m_pGamePlayer->getGold() / m_pGamePlayer->getLuckRMB());
    sprintf(buffer, "%d", remainTimes);
    CCLabelTTF* rmbOneTimes = CCLabelTTF::create(buffer, "Arial", 20);
    rmbOneTimes->setAnchorPoint(ccp(0.5, 0));
    rmbOneTimes->setPosition(ccp(182 + 1 * BTN_OFFSET_BGICON, 344));
    pNode->addChild(rmbOneTimes, 2, RMBONE_REMAIN_TIMES);
    if (remainTimes == 0) {
        CCSprite* spShow = (CCSprite*)pNode->getChildByTag(BGOFSHOWTIME+1);
        spShow->setVisible(false);
        rmbOneTimes->setVisible(false);
    }
    remainTimes = floor(m_pGamePlayer->getGold() / m_pGamePlayer->getLuckRMBBig());
    sprintf(buffer, "%d", remainTimes);
    CCLabelTTF* rmbTenTimes = CCLabelTTF::create(buffer, "Arial", 20);
    rmbTenTimes->setAnchorPoint(ccp(0.5, 0));
    rmbTenTimes->setPosition(ccp(182 + 2 * BTN_OFFSET_BGICON, 344));
    pNode->addChild(rmbTenTimes, 2, RMBTEN_REMAIN_TIMES);
    if (remainTimes == 0) {
        CCSprite* spShow = (CCSprite*)pNode->getChildByTag(BGOFSHOWTIME+2);
        spShow->setVisible(false);
        rmbTenTimes->setVisible(false);
    }
    
    /* 武将抽取详细 */
    CCLabelTTF* des1 = CCLabelTTF::create("随机获得10个武将", "Arial", 20);
    des1->setColor(YELLOW_COLOR);
    des1->setAnchorPoint(ccp(0, 0));
    des1->setPosition(ccp(30 + 0 * BTN_OFFSET_BGICON, 170));
    pNode->addChild(des1);
    CCLabelTTF* des2 = CCLabelTTF::create("随机获得1个武将", "Arial", 20);
    des2->setColor(YELLOW_COLOR);
    des2->setAnchorPoint(ccp(0, 0));
    des2->setPosition(ccp(30 + 1 * BTN_OFFSET_BGICON, 170));
    pNode->addChild(des2);
    CCLabelTTF* des3 = CCLabelTTF::create("随机获得10个武将 必出紫将", "Arial", 20, CCSizeMake(180, 50), kCCTextAlignmentCenter);
    des3->setColor(RED_COLOR);
    des3->setAnchorPoint(ccp(0, 0));
    des3->setPosition(ccp(20 + 2 * BTN_OFFSET_BGICON, 145));
    pNode->addChild(des3);
    
    return true;
}

/* 抽卡成功调用 刷新数据(刷新类型) */
void YiZhanLayer::updateUi(unsigned int type) {
    char buffer[64];
    
    CCNode* pParent = (CCNode*)this->getChildByTag(CONTAINER_TAG);
    CCLabelTTF* l1 = (CCLabelTTF*)pParent->getChildByTag(COURAGE_REMAIN_TIMES);
    CCLabelTTF* l2 = (CCLabelTTF*)pParent->getChildByTag(RMBONE_REMAIN_TIMES);
    CCLabelTTF* l3 = (CCLabelTTF*)pParent->getChildByTag(RMBTEN_REMAIN_TIMES);
    CCLabelTTF* l11 = (CCLabelTTF*)pParent->getChildByTag(COURAGE_REMAIN);
    CCSprite* bgOfShow1 = (CCSprite*)pParent->getChildByTag(BGOFSHOWTIME);
    CCSprite* bgOfShow2 = (CCSprite*)pParent->getChildByTag(BGOFSHOWTIME+1);
    CCSprite* bgOfShow3 = (CCSprite*)pParent->getChildByTag(BGOFSHOWTIME+2);
    CCLabelTTF* needOne = (CCLabelTTF*)pParent->getChildByTag(GOLD_REMAIN);
    CCLabelTTF* needTen = (CCLabelTTF*)pParent->getChildByTag(GOLD_REMAIN_TEN);
    if (type == 1) {
        /* 当前勇气值 */
        int remainTimes = floor(m_pGamePlayer->getCourage() / m_pGamePlayer->getLuckCourage());
        if (remainTimes == 0) {
            bgOfShow1->setVisible(false);
            if (l1)
                l1->setVisible(false);
            sprintf(buffer, "%d", m_pGamePlayer->getCourage());
            if (l11) {
                l11->setString(buffer);
            }
        } else {
            bgOfShow1->setVisible(true);
            sprintf(buffer, "%d", remainTimes);
            if (l1)
                l1->setString(buffer);
            sprintf(buffer, "剩余勇气  %d", m_pGamePlayer->getCourage());
            if (l11)
                l11->setString(buffer);
        }
        
    } else if (type == 2) {
        /* 当前元宝 */
        int remainTimes = floor(m_pGamePlayer->getGold() / m_pGamePlayer->getLuckRMB());
        if (remainTimes == 0) {
            bgOfShow2->setVisible(false);
            bgOfShow3->setVisible(false);
            needOne->setColor(RED_COLOR);
            if (l2)
                l2->setVisible(false);
            if (l3)
                l3->setVisible(false);
        } else {
            if (m_pGamePlayer->getGold() < m_pGamePlayer->getLuckRMBBig()) {
                bgOfShow3->setVisible(false);
                needTen->setColor(RED_COLOR);
                if (l3)
                    l3->setVisible(false);
            } else {
                bgOfShow3->setVisible(true);
                if (l3) {
                    l3->setVisible(true);
                    int remainTimes1 = floor(m_pGamePlayer->getGold() / m_pGamePlayer->getLuckRMBBig());
                    sprintf(buffer, "%d", remainTimes1);
                    l3->setString(buffer);
                }
            }
            bgOfShow2->setVisible(true);
            sprintf(buffer, "%d", remainTimes);
            if (l2){
                l2->setString(buffer);
                l2->setVisible(true);
            }
        }
    } else if (type == 3) {
        /* 当前元宝 */
        int remainTimes = floor(m_pGamePlayer->getGold() / m_pGamePlayer->getLuckRMBBig());
        if (remainTimes == 0) {
            bgOfShow3->setVisible(false);
            needTen->setColor(RED_COLOR);
            if (l3) {
                l3->setVisible(false);
            }
            if (m_pGamePlayer->getGold() < m_pGamePlayer->getLuckRMB()) {
                bgOfShow2->setVisible(false);
                needOne->setColor(RED_COLOR);
                if (l2)
                    l2->setVisible(false);
            } else {
                bgOfShow2->setVisible(true);
                int remainTimes1 = floor(m_pGamePlayer->getGold() / m_pGamePlayer->getLuckRMB());
                sprintf(buffer, "%d", remainTimes1);
                if (l2)
                    l2->setString(buffer);
            }
        } else {
            bgOfShow3->setVisible(true);
            bgOfShow2->setVisible(true);
            int remainTimes1 = floor(m_pGamePlayer->getGold() / m_pGamePlayer->getLuckRMB());
            sprintf(buffer, "%d", remainTimes1);
            if (l2) {
                l2->setString(buffer);
                l2->setVisible(true);
            }
            sprintf(buffer, "%d", remainTimes);
            if (l3) {
                l3->setString(buffer);
                l3->setVisible(true);
            }
        }
    }
}

//
void YiZhanLayer::chosTypeForSearch(CCObject* pSender) {
    CCMenuItemImage* pItemImage = (CCMenuItemImage*)pSender;
    for (int i = 0; i < 3; ++i) {
        CCSprite* sp = (CCSprite*)this->getChildByTag(CONTAINER_TAG)->getChildByTag(BTN_SCENE_SEL + i);
        sp->setVisible(false);
        CCSprite* spFont = (CCSprite*)this->getChildByTag(CONTAINER_TAG)->getChildByTag(BTN_SCENE_FONT + i);
        spFont->setScale(1);
        CCSprite* tie = (CCSprite*)this->getChildByTag(CONTAINER_TAG)->getChildByTag(TIELIAN_TAG1 + i);
        CCSprite* tie2 = (CCSprite*)this->getChildByTag(CONTAINER_TAG)->getChildByTag(TIELIAN_TAG1 + i + 5);
        CCSprite* ties = (CCSprite*)this->getChildByTag(CONTAINER_TAG)->getChildByTag(TIELIAN_TAG + i);
        CCSprite* ties2 = (CCSprite*)this->getChildByTag(CONTAINER_TAG)->getChildByTag(TIELIAN_TAG + i + 5);
        tie->setVisible(false);
        tie2->setVisible(false);
        ties->setVisible(true);
        ties2->setVisible(true);
    }
    
    switch (pItemImage->getTag() - BTN_SCENE) {
    case 0:
        {
            CCSprite* sp = (CCSprite*)this->getChildByTag(CONTAINER_TAG)->getChildByTag(BTN_SCENE_SEL);
            sp->setVisible(true);
            CCSprite* spFont = (CCSprite*)this->getChildByTag(CONTAINER_TAG)->getChildByTag(BTN_SCENE_FONT);
            spFont->setScale(1.2);
            CCSprite* tie = (CCSprite*)this->getChildByTag(CONTAINER_TAG)->getChildByTag(TIELIAN_TAG1);
            CCSprite* tie2 = (CCSprite*)this->getChildByTag(CONTAINER_TAG)->getChildByTag(TIELIAN_TAG1 + 5);
            tie->setVisible(true);
            tie2->setVisible(true);
            CCSprite* ties = (CCSprite*)this->getChildByTag(CONTAINER_TAG)->getChildByTag(TIELIAN_TAG);
            CCSprite* ties2 = (CCSprite*)this->getChildByTag(CONTAINER_TAG)->getChildByTag(TIELIAN_TAG + 5);
            ties->setVisible(false);
            ties2->setVisible(false);
        }
        break;
    case 1:
        {
            CCSprite* sp = (CCSprite*)this->getChildByTag(CONTAINER_TAG)->getChildByTag(BTN_SCENE_SEL + 1);
            sp->setVisible(true);
            CCSprite* spFont = (CCSprite*)this->getChildByTag(CONTAINER_TAG)->getChildByTag(BTN_SCENE_FONT + 1);
            spFont->setScale(1.2);
            CCSprite* tie = (CCSprite*)this->getChildByTag(CONTAINER_TAG)->getChildByTag(TIELIAN_TAG1 + 1);
            CCSprite* tie2 = (CCSprite*)this->getChildByTag(CONTAINER_TAG)->getChildByTag(TIELIAN_TAG1 + 6);
            tie->setVisible(true);
            tie2->setVisible(true);
            CCSprite* ties = (CCSprite*)this->getChildByTag(CONTAINER_TAG)->getChildByTag(TIELIAN_TAG+1);
            CCSprite* ties2 = (CCSprite*)this->getChildByTag(CONTAINER_TAG)->getChildByTag(TIELIAN_TAG + 6);
            ties->setVisible(false);
            ties2->setVisible(false);
        }
        break;
    case 2:
        {
            CCSprite* sp = (CCSprite*)this->getChildByTag(CONTAINER_TAG)->getChildByTag(BTN_SCENE_SEL + 2);
            sp->setVisible(true);
            CCSprite* spFont = (CCSprite*)this->getChildByTag(CONTAINER_TAG)->getChildByTag(BTN_SCENE_FONT + 2);
            spFont->setScale(1.2);
            CCSprite* tie = (CCSprite*)this->getChildByTag(CONTAINER_TAG)->getChildByTag(TIELIAN_TAG1 + 2);
            CCSprite* tie2 = (CCSprite*)this->getChildByTag(CONTAINER_TAG)->getChildByTag(TIELIAN_TAG1 + 7);
            tie->setVisible(true);
            tie2->setVisible(true);
            CCSprite* ties = (CCSprite*)this->getChildByTag(CONTAINER_TAG)->getChildByTag(TIELIAN_TAG+2);
            CCSprite* ties2 = (CCSprite*)this->getChildByTag(CONTAINER_TAG)->getChildByTag(TIELIAN_TAG + 7);
            ties->setVisible(false);
            ties2->setVisible(false);
        }
        break;
    default:
            break;
    }
}

//
void YiZhanLayer::chosForLucky(CCObject* pSender) {
	CCMenuItemImage* pItemImage = (CCMenuItemImage*)pSender;
    
    NetConnection* pNetConnection = NetConnection::getInstance();
    char szPostBuffer[MAX_POSTTO_SERVER_BUFFER_SIZE];
    memset(szPostBuffer, '\0', MAX_POSTTO_SERVER_BUFFER_SIZE);
    
    strcpy(szPostBuffer, "c=user&a=luckyHero&type=");
    switch (pItemImage->getTag() - BTN_CHOU) {
    case 0:
        {
            /*判断是否满足抽取条件（弹框）*/
            if (m_pGamePlayer->getCourage() < m_pGamePlayer->getLuckCourage()) {
                return;
            }
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
            if (m_pGamePlayer->getGold() < m_pGamePlayer->getLuckRMB()) {
                return;
            }
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
            if (m_pGamePlayer->getGold() < m_pGamePlayer->getLuckRMBBig()) {
                return;
            }
            // 付费十连抽
            char szHttpTag[] = HTTP_REQUEST_YINFREETEN_TAG;
            strcat(szPostBuffer, "r10&uid=");
            strcat(szPostBuffer, m_pGamePlayer->getUid().c_str());
            CCLog("post tag = %s , post url = %s", szHttpTag, szPostBuffer);
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
void YiZhanLayer::moveOutTouchUICompent() {
    CCNode* pParent = (CCNode*)this->getChildByTag(CONTAINER_TAG);
    
    if (pParent->getActionByTag(MOVEIN_TAG)) {
        pParent->stopAllActions();
        pParent->setPosition(ccp(0, m_pGameState->getBottomOffset()));
        CCSequence* pMoveOUtAction = CCSequence::create(CCMoveTo::create(ANIMATION_DURING, ccp(0, -LAYER_OFFSET)), CCCallFunc::create(this, callfunc_selector(YiZhanLayer::moveOutCallback)), NULL);
        pMoveOUtAction->setTag(MOVEOUT_TAG);
        pParent->runAction(pMoveOUtAction);
        return;
    }
    if (!pParent->getActionByTag(MOVEOUT_TAG)) {
        CCSequence* pMoveOUtAction = CCSequence::create(CCMoveTo::create(ANIMATION_DURING, ccp(0, -LAYER_OFFSET)), CCCallFunc::create(this, callfunc_selector(YiZhanLayer::moveOutCallback)), NULL);
        pMoveOUtAction->setTag(MOVEOUT_TAG);
        pParent->runAction(pMoveOUtAction);
    }
}

//
void YiZhanLayer::restoreTouchUICompent() {
    /*恢复可见*/
    this->setVisible(true);
    CCNode* pParent = (CCNode*)this->getChildByTag(CONTAINER_TAG);
    
    if (pParent->getActionByTag(MOVEOUT_TAG)) {
        pParent->stopAllActions();
        pParent->setPosition(ccp(0, -LAYER_OFFSET));
        CCMoveTo* pMoveTo = CCMoveTo::create(ANIMATION_DURING, ccp(0, m_pGameState->getBottomOffset()));
        pMoveTo->setTag(MOVEIN_TAG);
        pParent->runAction(pMoveTo);
        return;
    }
    
    if (!pParent->getActionByTag(MOVEIN_TAG)) {
        CCMoveTo* pMoveTo = CCMoveTo::create(ANIMATION_DURING, ccp(0, m_pGameState->getBottomOffset()));
        pMoveTo->setTag(MOVEIN_TAG);
        pParent->runAction(pMoveTo);
    }
}

//
void YiZhanLayer::initPosition() {
    CCNode* pNode = (CCNode*)this->getChildByTag(CONTAINER_TAG);
    pNode->setPosition(ccp(0, -LAYER_OFFSET));
}

//
void YiZhanLayer::moveOutCallback() {
    if (m_isRemoveSelf) {
        this->removeFromParent();
//        CCLog("YiZhanLayer 从主场景移除");
    }
}

