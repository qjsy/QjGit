//
//  JianyuLayer.cpp
//  hero
//
//  Created by 钱剑 on 2014／02／19.
//
//

#include <ctime>
#include "common.h"
#include "JianyuLayer.h"
#include "HeroItem.h"
#include "PrisonHeroItem.h"
#include "HeroManager.h"
#include "GameState.h"
#include "MessageBox.h"
#include "NetConnection.h"
#include "GamePlayer.h"
#include "CardBox.h"
#include "LoyaltyItem.h"
#include "HeroIntroduce.h"
#include "customMessage.h"
// 监狱系统大图文件
#define JY_IMAGE_FILE        "jiany.pvr.ccz"
#define JY_PLIST_FILE         "jiany.plist"

#define DEFAULT_ZOEDER              10                      // 默认的zorder
#define HEI_COLOR                   ccc3(0, 0, 0)
#define JIN_COLOR                   ccc3(255, 215, 0)
#define TIPMENU_V                   750.0f                  // tipMenu竖直高度
#define TYPE_IN                     1                       // 进入
#define TYPE_OUT                    2                       // 出
#define DURATION_TIME               0.3                     // 动画时间
#define LAYER_INIT_POSITION         ccp(320.0f, -850.0f)    // 初始化位置
#define PARENT_POSITION_Y           -20.0f

#define DIBIAN_IMAGE                "JY_diK.png"            // 底边显示条
#define NAMEBOX_IMAGE               "JY_nameBox.png"        // 姓名框
#define BTNNORMAL_IMAGE             "JY_btn.png"            // 按钮
#define BTNSELECT_IMAGE             "JY_btns.png"           // 选中按钮
#define ZHAOX_IMAGE                 "JY_zFont.png"          // 招降文字
#define SHIF_IMAGE                  "JY_sFont.png"          // 释放文字
#define ZICAI_IMAGE                 "JY_ziPlus.png"         // 紫色材料/暂时用于显示
#define GEM_IMAGE                   "JY_gem.png"            // 元宝图片
#define TIP_IMAGE                   "JY_tip.png"            // 左右翻动提示
#define TIEL_IMAGE                  "JY_tiel.png"           // 横铁链
#define ZUOX_IMAGE                  "JY_zuoTiel.png"        // 左斜铁链
#define YOUX_IMAGE                  "JY_youTiel.png"        // 右斜铁链
#define LOYAL_IMAGE                 "JY_loyal.png"          // 忠诚度
#define PROBG_IMAGE                 "JY_proBg.png"          // 进度条背景
#define PRO_IMAGE                   "JY_pro.png"            // 进度条
#define ZI_IMAGE                    "JY_zi.png"             // 暂时用于显示
#define LANCAI_IMAGE                "JY_lan.png"            // 暂时用于显示
#define JYFONT_IMAGE                "JY_font.png"           // 监狱文字
#define JYDIB_IMAGE                 "JY_dib.png"            // 监狱底板
#define JYBG_IMAGE                  "JY_bg.png"             // 监狱大背景
#define JYDNAG_IMAGE                "JY_dang.png"           // 大背景挡板
#define JY_KONGBAI                  "JY_kong.png"           // 透明层
#define BTNDISABLE_IMAGE            "JY_btnh.png"

// TAG
#define PARENT_TAG                  10                      // 监狱图片 FILE
#define PROGRESS_TAG                11                      // 忠诚度进度条 TAG
#define TIPMENU_LEFT_TAG            12                      // 左边tipMenu容器
#define TIPMENU_RIGHT_TAG           13                      // 右边tipMenu容器
#define TIPMENU_MID_TAG             14                      // 中间tipMenu容器
#define NODE_TAG                    15                      // 加入this的容器
#define ZICAILIAO_TAG               18                      // 紫色道具
#define LANCAILIAO_TAG              19                      // 蓝色材料
#define PROGRESS_TAG_BG             22                      // 进度条背景

///////////
#define PRISON_HERO_X               30.0f                   // 监狱武将显示位置
#define PRISON_HERO_Y               140.0f
#define PADDING_HERO                6.0f                    // 监狱武将头像之间的间隙
#define TOUCH_TIME_DISABLE_LIMIT    3.0f                    // touch 无效的时间上限
#define TOUCH_TIME_LOW_LIMIT        0.2f                    // touch 时间下限
#define TOUCH_TIME_UPPER_LIMIT      0.5f                    // touch 时间上限
#define MOVE_ONE_TIME               0.3f
#define TOUCH_HERO_INTRO_DELTA      3.0f                    // touch 结束和开始之间位移小于该值的话判定为弹出武将介绍

/*
 *  构造方法
 */
JianyuLayer::JianyuLayer()
:m_pJyCBN(NULL)
,m_pGameState(GameState::getInstance())
,m_pHeroManager(HeroManager::getInstance())
,m_touchHeroBeganTime(0.0f)
,m_pGamePlayer(GamePlayer::getInstance())
,m_currHeroSprite(NULL)
,m_heroSprite1(NULL)
,m_heroSprite2(NULL)
,m_curHeroIdx(-1)
,m_pSpriteFrameCache(CCSpriteFrameCache::sharedSpriteFrameCache())
,m_pDangSprite(NULL)
,m_isTrunPageing(false)
{
    /* 初始化精灵数组 */
    memset(m_btnSprite, '\0', sizeof(m_btnSprite));
    /* 初始化按钮位置 */
    m_btnPosition[0] = ccp(20.0f, 35.0f + PARENT_POSITION_Y); /* 释放 */
    m_btnEnable[0] = true;
    m_btnPosition[1] = ccp(275.0f, 35.0f + PARENT_POSITION_Y); /* 招降 */
    m_btnEnable[1] = true;
    m_btnPosition[2] = ccp(-80.0f, 170.0f + PARENT_POSITION_Y); /* 左滑 */
    m_btnEnable[2] = true;
    m_btnPosition[3] = ccp(380.0f, 170.0f + PARENT_POSITION_Y); /* 右滑 */
    m_btnEnable[3] = true;
    
    // 监狱武将大卡头像显示区域
    m_heroRect = CCRectMake(200.0f, 410.0f + m_pGameState->getBottomOffset(), 245.0f, 320.0f);
}

/* 
 *  析构函数
 */
JianyuLayer::~JianyuLayer() {
    m_pSpriteFrameCache->removeSpriteFramesFromFile(JY_PLIST_FILE);
    CCTextureCache* pTextureCache = CCTextureCache::sharedTextureCache();
    pTextureCache->removeTextureForKey(JY_IMAGE_FILE);
    
    // 释放武将整卡头像图片资源
    std::vector<std::string>::iterator iter;
    for (iter = m_prisonHerosImage.begin(); iter != m_prisonHerosImage.end(); iter++) {
        pTextureCache->removeTextureForKey((*iter).c_str());
    } /*for*/
    m_prisonHerosImage.clear();
    
    pTextureCache->removeTextureForKey("CM_left.png");
    pTextureCache->removeTextureForKey("CM_right.png");
    pTextureCache->removeTextureForKey("CM_mid.png");
    
    pTextureCache->removeTextureForKey("COM_whiBox.png");
    pTextureCache->removeTextureForKey("COM_greBox.png");
    pTextureCache->removeTextureForKey("COM_bluBox.png");
    pTextureCache->removeTextureForKey("COM_purBox.png");
    pTextureCache->removeTextureForKey("COM_oraBox.png");
    pTextureCache->removeTextureForKey("COM_whiJiao.png");
    pTextureCache->removeTextureForKey("COM_greJiao.png");
    pTextureCache->removeTextureForKey("COM_bluJiao.png");
    pTextureCache->removeTextureForKey("COM_purJiao.png");
    pTextureCache->removeTextureForKey("COM_oraJiao.png");
    pTextureCache->removeTextureForKey("COM_whiJob.png");
    pTextureCache->removeTextureForKey("COM_greJob.png");
    pTextureCache->removeTextureForKey("COM_bluJob.png");
    pTextureCache->removeTextureForKey("COM_purJob.png");
    pTextureCache->removeTextureForKey("COM_oraJob.png");
    pTextureCache->removeTextureForKey("CM_whi.png");
    pTextureCache->removeTextureForKey("CM_gre.png");
    pTextureCache->removeTextureForKey("CM_blu.png");
    pTextureCache->removeTextureForKey("CM_pur.png");
    pTextureCache->removeTextureForKey("CM_ora.png");
    pTextureCache->removeTextureForKey("JY_nameBox.png");
    
    this->unscheduleAllSelectors();
}

/*
 *  初始化
 */
bool JianyuLayer::init() {
    if (!CCLayer::init()) {
        return false;
    }
    /* 可触摸 */
    this->setTouchEnabled(true);
    
    // 初始化 ui 组件
    _initUiComponent();
    
    // 初始化按钮
    _initBtn();

    m_prisonHerosImage.clear();
    unsigned int countOfPrisonHeros = m_pHeroManager->getCountOfPrisonHeros();
    if ( countOfPrisonHeros > 0) {
        /* 排序 */
        m_pHeroManager->sortPrisonHeros();
        
        PrisonHeroItem* pPrisonHeroItem = NULL;
        for (unsigned int i = 0; i < countOfPrisonHeros; i++) {
            pPrisonHeroItem = m_pHeroManager->getPrisonHeroItemByIndex(i);
            m_prisonHerosImage.push_back(pPrisonHeroItem->getHeroItem()->getBIcon());
        } /*for*/
    }
    
    /* 更新监狱武将 UI */
    _updatePrisonHeroItemUi();
    /* 更新玩家拥有的道具 */
    _updateGamePlayerProp();
    return true;
}

/* 初始化 UI 组件 */
void JianyuLayer::_initUiComponent() {
    /* 加载大图资源 */
    m_pSpriteFrameCache->addSpriteFramesWithFile(JY_PLIST_FILE, JY_IMAGE_FILE);
    this->addChild(m_pJyCBN = CCSpriteBatchNode::create(JY_IMAGE_FILE), 3);
    m_pJyCBN->setPosition(ccp(0, 20.0f));
    
    // 大背景挡板作为node默认zorder
    CCSprite* parent = CCSprite::createWithSpriteFrameName(JY_KONGBAI);
    parent->setAnchorPoint(ccp(0.5, 0));
    parent->setPosition(ccp(320.0f, m_pGameState->getBottomOffset() + 310.0f));
    m_pJyCBN->addChild(parent, 1, PARENT_TAG);
    
    CCPoint parentPosition = parent->getPosition();
    
    /* 初始化图片资源 0 nor, 1 sel, 2 hui */
    /* 监狱大背景 */
    CCSprite* bgOfJy = CCSprite::createWithSpriteFrameName(JYBG_IMAGE);
    bgOfJy->setAnchorPoint(ccp(0.5, 0));
    bgOfJy->setPosition(ccp(150.0f, -10.0f + PARENT_POSITION_Y));
    parent->addChild(bgOfJy, DEFAULT_ZOEDER + 2);
    /* 监狱底板 */
    CCSprite* diOfJy = CCSprite::createWithSpriteFrameName(JYDIB_IMAGE);
    diOfJy->setAnchorPoint(ccp(0.5, 0));
    diOfJy->setPosition(ccp(150.0f, -175.0f + PARENT_POSITION_Y));
    parent->addChild(diOfJy, DEFAULT_ZOEDER + 1);
    /* 底板底框 */
    CCSprite* dikOfJy = CCSprite::createWithSpriteFrameName(DIBIAN_IMAGE);
    dikOfJy->setAnchorPoint(ccp(0.5, 0));
    dikOfJy->setPosition(ccp(150.0f, -175.0f + PARENT_POSITION_Y));
    parent->addChild(dikOfJy, DEFAULT_ZOEDER + 2);
    /* 释放文字 */
    CCSprite* shifFont = CCSprite::createWithSpriteFrameName(SHIF_IMAGE);
    shifFont->setAnchorPoint(ccp(0.5, 0));
    shifFont->setPosition(ccp(20.0f, 17.0f + PARENT_POSITION_Y));
    parent->addChild(shifFont, DEFAULT_ZOEDER + 6);
    /* 招降文字 */
    CCSprite* zhaoxFont = CCSprite::createWithSpriteFrameName(ZHAOX_IMAGE);
    zhaoxFont->setAnchorPoint(ccp(0.5, 0));
    zhaoxFont->setPosition(ccp(230.0f, 17.0f + PARENT_POSITION_Y));
    parent->addChild(zhaoxFont, DEFAULT_ZOEDER + 6);
    /* 忠诚度文字 */
    CCSprite* loyalFont = CCSprite::createWithSpriteFrameName(LOYAL_IMAGE);
    loyalFont->setAnchorPoint(ccp(0.5, 0));
    loyalFont->setPosition(ccp(-45.0f, 65.0f));
    parent->addChild(loyalFont, DEFAULT_ZOEDER + 6);
    /* 忠诚度进度条 */
    CCNode* pNode = CCNode::create();               /* 不能加入批处理文件 */
    addChild(pNode, DEFAULT_ZOEDER + 6, NODE_TAG);
    CCSprite* proBg = CCSprite::createWithSpriteFrameName(PROBG_IMAGE);
    proBg->setAnchorPoint(CCPointZero);
    proBg->setPosition(ccp(193.0f, parentPosition.y + 70.0f));
    pNode->addChild(proBg, DEFAULT_ZOEDER + 2, PROGRESS_TAG_BG);
    
    pLoyalProgress = CCProgressTimer::create(CCSprite::createWithSpriteFrameName(PRO_IMAGE));
    pLoyalProgress->setType(kCCProgressTimerTypeBar);
    pLoyalProgress->setMidpoint(ccp(0, 0));
    pLoyalProgress->setBarChangeRate(ccp(1, 0));
    pLoyalProgress->setPercentage(100.0);
    pLoyalProgress->setAnchorPoint(CCPointZero);
    pLoyalProgress->setPosition(ccp(202.0f, parentPosition.y + 71.0f));
    pNode->addChild(pLoyalProgress, DEFAULT_ZOEDER + 2);
    /* 铁链 */
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            CCSprite* hTie = CCSprite::createWithSpriteFrameName(TIEL_IMAGE);
            hTie->setAnchorPoint(CCPointZero);
            hTie->setPosition(ccp(-165.0f + i * 430.0f, 200.0f + j * 100.0f));
            if (i == 1) {
                hTie->setFlipX(true);
            }
            parent->addChild(hTie, DEFAULT_ZOEDER + 2);
        }
    }
    CCSprite* zuoTie = CCSprite::createWithSpriteFrameName(ZUOX_IMAGE);
    zuoTie->setAnchorPoint(CCPointZero);
    zuoTie->setOpacity(220);
    zuoTie->setPosition(ccp(20.0f, 75.0f));
    parent->addChild(zuoTie, DEFAULT_ZOEDER + 2);
    CCSprite* youTie = CCSprite::createWithSpriteFrameName(YOUX_IMAGE);
    youTie->setAnchorPoint(CCPointZero);
    youTie->setOpacity(220);
    youTie->setPosition(ccp(20.0f, 75.0f));
    parent->addChild(youTie, DEFAULT_ZOEDER + 2);
    /* 紫色道具 */
    CCSprite* zi = CCSprite::createWithSpriteFrameName(ZI_IMAGE);
    zi->setAnchorPoint(CCPointZero);
    zi->setPosition(ccp(-120.0f, 405.0f + PARENT_POSITION_Y));
    parent->addChild(zi, DEFAULT_ZOEDER + 6);
    /* 蓝色道具 */
    CCSprite* lan = CCSprite::createWithSpriteFrameName(ZI_IMAGE);
    lan->setAnchorPoint(CCPointZero);
    lan->setPosition(ccp(-120.0f, 360.0f + PARENT_POSITION_Y));
    parent->addChild(lan, DEFAULT_ZOEDER + 6);
    
    /* 按钮所需材料显示 */
    pPropSPrite = CCSprite::createWithSpriteFrameName(ZICAI_IMAGE);
    pPropSPrite->setAnchorPoint(CCPointZero);
    pPropSPrite->setPosition(ccp(268.0f, 8.0f + PARENT_POSITION_Y));
    parent->addChild(pPropSPrite, DEFAULT_ZOEDER + 6);
    /*当材料不足的时候用元宝*/
    pGoldSprite = CCSprite::createWithSpriteFrameName(GEM_IMAGE);
    pGoldSprite->setAnchorPoint(CCPointZero);
    pGoldSprite->setPosition(ccp(268.0f, 8.0f + PARENT_POSITION_Y));
    parent->addChild(pGoldSprite, DEFAULT_ZOEDER + 6);
    
    CCSprite* btnGem = CCSprite::createWithSpriteFrameName(GEM_IMAGE);
    btnGem->setAnchorPoint(CCPointZero);
    btnGem->setPosition(ccp(265.0f, 32.0 + PARENT_POSITION_Y));
    parent->addChild(btnGem, DEFAULT_ZOEDER + 6);
    
    /* 系统文字 */
    char buffer[128];
    sprintf(buffer, "1:每天赠送蓝色游说令%d个,紫色游说令%d个。",2, 1);
    CCLabelTTF* l1 = CCLabelTTF::create(buffer, "Arial", 23);
    l1->setAnchorPoint(CCPointZero);
    l1->setColor(HEI_COLOR);
    l1->setPosition(ccp(45.0f, parentPosition.y - 43.0f + PARENT_POSITION_Y));
    pNode->addChild(l1, DEFAULT_ZOEDER + 2);
    sprintf(buffer, "2:招降失败会降低俘虏的忠诚度。");
    CCLabelTTF* l2 = CCLabelTTF::create(buffer, "Arial", 23);
    l2->setAnchorPoint(CCPointZero);
    l2->setColor(HEI_COLOR);
    l2->setPosition(ccp(45.0f, parentPosition.y - 73.0f + PARENT_POSITION_Y));
    pNode->addChild(l2, DEFAULT_ZOEDER + 2);
    sprintf(buffer, "3:忠诚度越低，招募成功率越高。");
    CCLabelTTF* l3 = CCLabelTTF::create(buffer, "Arial", 23);
    l3->setAnchorPoint(CCPointZero);
    l3->setColor(HEI_COLOR);
    l3->setPosition(ccp(45.0f, parentPosition.y - 103.0f + PARENT_POSITION_Y));
    pNode->addChild(l3, DEFAULT_ZOEDER + 2);
    sprintf(buffer, "4:释放俘虏，将获得一定的铜钱。");
    CCLabelTTF* l4 = CCLabelTTF::create(buffer, "Arial", 23);
    l4->setAnchorPoint(CCPointZero);
    l4->setColor(HEI_COLOR);
    l4->setPosition(ccp(45.0f, parentPosition.y - 133.0f + PARENT_POSITION_Y));
    pNode->addChild(l4, DEFAULT_ZOEDER + 2);
    /* 忠诚度文字进度 */
    pLoyaltyLabel = CCLabelTTF::create("", "Arial", 23);
    pLoyaltyLabel->setAnchorPoint(CCPointZero);
    pLoyaltyLabel->setPosition(ccp(480.0f, parentPosition.y + 90.0f + PARENT_POSITION_Y));
    pNode->addChild(pLoyaltyLabel, DEFAULT_ZOEDER + 2);
    /* 翻页 label */
    sprintf(buffer, "%d/%d", 99, 100);
    m_pagesLable = CCLabelTTF::create(buffer, "Arial", 19);
    m_pagesLable->setAnchorPoint(CCPointZero);
    m_pagesLable->setPosition(ccp(25.0f, parentPosition.y - 170.0f + PARENT_POSITION_Y));
    pNode->addChild(m_pagesLable, DEFAULT_ZOEDER + 2);
    /* 紫色道具数量 */
    sprintf(buffer, "x%d", 99);
    CCLabelTTF* l7 = CCLabelTTF::create(buffer, "Arial", 25);
    l7->setAnchorPoint(CCPointZero);
    l7->setColor(JIN_COLOR);
    l7->setPosition(ccp(95.0f, parentPosition.y + 410.0f + PARENT_POSITION_Y));
    pNode->addChild(l7, DEFAULT_ZOEDER + 2, ZICAILIAO_TAG);
    /* 蓝色道具数量 */
    sprintf(buffer, "x%d", 99);
    CCLabelTTF* l8 = CCLabelTTF::create(buffer, "Arial", 25);
    l8->setAnchorPoint(CCPointZero);
    l8->setColor(JIN_COLOR);
    l8->setPosition(ccp(95.0f, parentPosition.y + 360.0f + PARENT_POSITION_Y));
    pNode->addChild(l8, DEFAULT_ZOEDER + 2, LANCAILIAO_TAG);
    /* 所需道具数量 */
    pPropOrGold = CCLabelTTF::create("", "Arial", 19);
    pPropOrGold->setAnchorPoint(CCPointZero);
    pPropOrGold->setColor(JIN_COLOR);
    pPropOrGold->setPosition(ccp(470.0f, parentPosition.y + 10.0f + PARENT_POSITION_Y));
    pNode->addChild(pPropOrGold, DEFAULT_ZOEDER + 2);
    /* 所需铜币 */
    pCopperLabel = CCLabelTTF::create("", "Arial", 19);
    pCopperLabel->setAnchorPoint(CCPointZero);
    pCopperLabel->setColor(JIN_COLOR);
    pCopperLabel->setPosition(ccp(470.0f, parentPosition.y + 35.0f + PARENT_POSITION_Y));
    pNode->addChild(pCopperLabel, DEFAULT_ZOEDER + 2);
    
    // 挡板
    CCNode* pDangNode = CCNode::create();
    addChild(pDangNode, 1, 9999);
    
    m_pDangSprite = CCSprite::createWithSpriteFrameName(JYDNAG_IMAGE);
    m_pDangSprite->setAnchorPoint(ccp(0.5, 0.0f));
    m_pDangSprite->setPosition(ccp(320.0f, parentPosition.y));
    pDangNode->addChild(m_pDangSprite, 1);
    
    /* 释放 */
    parent->addChild(m_btnSprite[0][0] = CCSprite::createWithSpriteFrameName(BTNNORMAL_IMAGE), DEFAULT_ZOEDER + 3);
    parent->addChild(m_btnSprite[0][1] = CCSprite::createWithSpriteFrameName(BTNSELECT_IMAGE), DEFAULT_ZOEDER + 4);
    parent->addChild(m_btnSprite[0][2] = CCSprite::createWithSpriteFrameName(BTNDISABLE_IMAGE), DEFAULT_ZOEDER + 5);
    /* 招降 */
    parent->addChild(m_btnSprite[1][0] = CCSprite::createWithSpriteFrameName(BTNNORMAL_IMAGE), DEFAULT_ZOEDER + 3);
    parent->addChild(m_btnSprite[1][1] = CCSprite::createWithSpriteFrameName(BTNSELECT_IMAGE), DEFAULT_ZOEDER + 4);
    parent->addChild(m_btnSprite[1][2] = CCSprite::createWithSpriteFrameName(BTNDISABLE_IMAGE), DEFAULT_ZOEDER + 5);
    /* 左翻页 */
    parent->addChild(m_btnSprite[2][0] = CCSprite::createWithSpriteFrameName(TIP_IMAGE), DEFAULT_ZOEDER + 3);
    parent->addChild(m_btnSprite[2][1] = CCSprite::createWithSpriteFrameName(TIP_IMAGE), DEFAULT_ZOEDER + 4);
    parent->addChild(m_btnSprite[2][2] = CCSprite::createWithSpriteFrameName(TIP_IMAGE), DEFAULT_ZOEDER + 5);
    /* 右翻页 */
    parent->addChild(m_btnSprite[3][0] = CCSprite::createWithSpriteFrameName(TIP_IMAGE), DEFAULT_ZOEDER + 3);
    parent->addChild(m_btnSprite[3][1] = CCSprite::createWithSpriteFrameName(TIP_IMAGE), DEFAULT_ZOEDER + 4);
    parent->addChild(m_btnSprite[3][2] = CCSprite::createWithSpriteFrameName(TIP_IMAGE), DEFAULT_ZOEDER + 5);
    m_btnSprite[3][0]->setFlipX(true);
    m_btnSprite[3][1]->setFlipX(true);
    m_btnSprite[3][2]->setFlipX(true);
    
    /***** tipMenu非大图 ******/
    CCNode* leftNode = CCNode::create();
    addChild(leftNode, 1, TIPMENU_LEFT_TAG);
    CCSprite* pLeft = CCSprite::create("CM_left.png");
    pLeft->setAnchorPoint(ccp(0, 0));
    pLeft->setPosition(ccp(0.0f, TIPMENU_V + m_pGameState->getBottomOffset()));
    leftNode->addChild(pLeft);
    CCNode* rightNode = CCNode::create();
    addChild(rightNode, 1, TIPMENU_RIGHT_TAG);
    CCSprite* pRight = CCSprite::create("CM_right.png");
    pRight->setAnchorPoint(ccp(0, 0));
    pRight->setPosition(ccp(482.0f, TIPMENU_V + m_pGameState->getBottomOffset()));
    rightNode->addChild(pRight);
    CCNode* midNode = CCNode::create();
    addChild(midNode,1, TIPMENU_MID_TAG);
    CCSprite* pMid = CCSprite::create("CM_mid.png");
    pMid->setAnchorPoint(ccp(0, 0));
    pMid->setPosition(ccp(160.0f, TIPMENU_V + m_pGameState->getBottomOffset()));
    midNode->addChild(pMid);
    /* 监狱文字 */
    CCSprite* fontJy = CCSprite::createWithSpriteFrameName(JYFONT_IMAGE);
    fontJy->setAnchorPoint(ccp(0, 0));
    fontJy->setPosition(ccp(270.0f, TIPMENU_V + m_pGameState->getBottomOffset() + 8.0f));
    midNode->addChild(fontJy);
}

/* 招降更新UI界面 */
void JianyuLayer::updateAllOfPrisonBySummon() {
    /* 更新道具数量显示 */
    _updateGamePlayerProp();
    
    /* 更新监狱武将ui */
    _updatePrisonHeroItemUi(!m_pGameState->getIsSummonSucess());
    m_pGameState->setIsSummonSucess(false);
}

/* 释放更新所有UI界面 */
void JianyuLayer::updateAllOfPrisonByFree() {
    /* 更新道具数量显示 */
    _updateGamePlayerProp();
    
    /* 更新监狱武将ui */
    _updatePrisonHeroItemUi();
}

void JianyuLayer::_tipMenu(int type) {
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

/* 初始化按钮 */
void JianyuLayer::_initBtn() {
    ((CCSprite*)m_pJyCBN->getChildByTag(PARENT_TAG))->setVisible(true);
    
    if (m_pHeroManager->getCountOfPrisonHeros() == 0) {
        for (unsigned int i = 0; i < JY_BTN_COUNT; ++i) {
            m_btnEnable[i] = false;
            m_btnSprite[i][0]->setVisible(false);
            m_btnSprite[i][0]->setPosition(m_btnPosition[i]);
            m_btnSprite[i][1]->setVisible(false);
            m_btnSprite[i][1]->setPosition(m_btnPosition[i]);
            m_btnSprite[i][2]->setVisible(true);
            m_btnSprite[i][2]->setPosition(m_btnPosition[i]);
        } /*for*/
    } else {
        for (unsigned int i = 0; i < JY_BTN_COUNT; ++i) {
            m_btnSprite[i][0]->setVisible(true);
            m_btnSprite[i][0]->setPosition(m_btnPosition[i]);
            m_btnSprite[i][1]->setVisible(false);
            m_btnSprite[i][1]->setPosition(m_btnPosition[i]);
            m_btnSprite[i][2]->setVisible(false);
            m_btnSprite[i][2]->setPosition(m_btnPosition[i]);
        } /*for*/
    }
}

/*
 *  响应菜单按钮选择
 */
void JianyuLayer::_onBtnSelect(const unsigned int btnIdx) {
    if (m_isTrunPageing)
        return;
    
    char buffer[64];
    switch (btnIdx) {
    case 1:             /* 释放按钮响应 */
        {
            unsigned int getMoney = 0;
            const char* title = "释放武将";
            PrisonHeroItem* pPrisonHeroItem = m_pHeroManager->getPrisonHeroItemByIndex(m_curHeroIdx);
            if (pPrisonHeroItem) {
                LoyaltyItem* pLoyaltyItem = m_pHeroManager->getLoyaltyItem(pPrisonHeroItem->getDevotion());
                CCAssert(pLoyaltyItem, "invalid data");
                if (pPrisonHeroItem->getTypeProp() == PURPLE_PROP) {
                    getMoney = pLoyaltyItem->getPu();
                } else if (pPrisonHeroItem->getTypeProp() == BLUE_PROP) {
                    getMoney = pLoyaltyItem->getBl();
                }
                sprintf(buffer, "释放%s将获得%d铜钱",pPrisonHeroItem->getHeroItem()->getName().c_str(),
                        getMoney);
                MessageBox::getInstance()->messageForBox(title, 30, buffer, 25, this,
                                                         callfunc_selector(JianyuLayer::_confirmFree));
            }
        }
        break;
    case 2:             /* 招降按钮响应 */
        {
            PrisonHeroItem* pPrisonHeroItem = m_pHeroManager->getPrisonHeroItemByIndex(m_curHeroIdx);
            if (NULL == pPrisonHeroItem)
                return;
                
            /*保存老的忠诚度*/
            m_oldLoyal = pPrisonHeroItem->getDevotion();
            
            if (pPrisonHeroItem->getTypeProp() == PURPLE_PROP) {
                int need = m_pGamePlayer->getPropZiNum() - pPrisonHeroItem->getNum();
                if (need < 0) {
                    int nextNeed = need * m_pGamePlayer->getPropZiXi();
                    pPrisonHeroItem->setGold(abs(nextNeed));
                } else {
                    pPrisonHeroItem->setGold(0);
                }
            } else /*蓝色道具*/ {
                int need = m_pGamePlayer->getPropLanNum() - pPrisonHeroItem->getNum();
                if (need < 0) {
                    int nextNeed = need * m_pGamePlayer->getPropLanXi();
                    pPrisonHeroItem->setGold(abs(nextNeed));
                } else {
                    pPrisonHeroItem->setGold(0);
                }
            }
            
            if (pPrisonHeroItem->getTypeProp() == PURPLE_PROP)/*紫色材料*/ {
                if (m_pGamePlayer->getPropZiNum() < pPrisonHeroItem->getNum())/*材料不足*/ {
                    if (m_pGamePlayer->getGold() < pPrisonHeroItem->getGold())/*招降武将的元宝不足*/ {
                        const char* title = "元宝不足";
                        MessageBox::getInstance()->messageTips(title);
                    } else {
                        const char* title = "招降！";
                        sprintf(buffer, "是否花费%d元宝来招降%s",pPrisonHeroItem->getGold(),
                                m_pHeroManager->getHeroItem(pPrisonHeroItem->getHeroId())->getName().c_str());
                        MessageBox::getInstance()->messageForBox(title, 30, buffer, 25,
                                                                 this, callfunc_selector(JianyuLayer::_confirmUseGold));
                    }
                } else {
                    NetConnection* pNetConnection = NetConnection::getInstance();
                    if (pPrisonHeroItem) {
                        char szPostBuffer[MAX_POSTTO_SERVER_BUFFER_SIZE];
                        memset(szPostBuffer, '\0', MAX_POSTTO_SERVER_BUFFER_SIZE);
                        strcpy(szPostBuffer, "c=prison&a=summon&hero=");
                        char szHttpTag[] = HTTP_ZHAOXIANG_HERO_TAG;
                        sprintf(buffer, "%d",pPrisonHeroItem->getHeroItem()->getId());
                        strcat(szPostBuffer, buffer);
                        strcat(szPostBuffer, "&uid=");
                        strcat(szPostBuffer, GamePlayer::getInstance()->getUid().c_str());
                        CCLog("post 释放 = %s", szPostBuffer);
                        pNetConnection->commitPostRequestData(szPostBuffer, szHttpTag);
                    }
                }
            } else {
                if (m_pGamePlayer->getPropLanNum() < pPrisonHeroItem->getNum())/*材料不足*/ {
                    if (m_pGamePlayer->getGold() < pPrisonHeroItem->getGold())/*招降武将的元宝不足*/ {
                        const char* title = "元宝不足";
                        MessageBox::getInstance()->messageTips(title);
                    } else {
                        const char* title = "招降！";
                        sprintf(buffer, "是否花费%d元宝来招降%s",pPrisonHeroItem->getGold(),
                                m_pHeroManager->getHeroItem(pPrisonHeroItem->getHeroId())->getName().c_str());
                        MessageBox::getInstance()->messageForBox(title, 30, buffer, 25,
                                                                 this, callfunc_selector(JianyuLayer::_confirmUseGold));
                    }
                } else {
                    NetConnection* pNetConnection = NetConnection::getInstance();
                    if (pPrisonHeroItem) {
                        char szPostBuffer[MAX_POSTTO_SERVER_BUFFER_SIZE];
                        memset(szPostBuffer, '\0', MAX_POSTTO_SERVER_BUFFER_SIZE);
                        strcpy(szPostBuffer, "c=prison&a=summon&hero=");
                        char szHttpTag[] = HTTP_ZHAOXIANG_HERO_TAG;
                        sprintf(buffer, "%d",pPrisonHeroItem->getHeroItem()->getId());
                        strcat(szPostBuffer, buffer);
                        strcat(szPostBuffer, "&uid=");
                        strcat(szPostBuffer, GamePlayer::getInstance()->getUid().c_str());
                        CCLog("post 释放 = %s", szPostBuffer);
                        pNetConnection->commitPostRequestData(szPostBuffer, szHttpTag);
                    }
                }
            }
        }
        break;
    case 3:             /* 左滑动按钮响应 */
        _turnPagePrisonHeroItem(1, 1);
        break;
    case 4:             /* 右滑动按钮响应 */
        _turnPagePrisonHeroItem(2, 1);
        break;
    default:
        break;
    } /*switch*/
}

/*
 *  释放确认
 */
void JianyuLayer::_confirmFree() {
    PrisonHeroItem* pPrisonHeroItem = m_pHeroManager->getPrisonHeroItemByIndex(m_curHeroIdx);
    if (pPrisonHeroItem) {
        NetConnection* pNetConnection = NetConnection::getInstance();
        char szHttpTag[] = HTTP_FREE_HERO_TAG;
        char szPostBuffer[MAX_POSTTO_SERVER_BUFFER_SIZE];
        memset(szPostBuffer, '\0', MAX_POSTTO_SERVER_BUFFER_SIZE);
        
        sprintf(szPostBuffer, "c=prison&a=letgo&hero=%d&uid=%s", pPrisonHeroItem->getHeroItem()->getId(),
                m_pGamePlayer->getUid().c_str());
        CCLog("post 释放 = %s", szPostBuffer);
        pNetConnection->commitPostRequestData(szPostBuffer, szHttpTag);
        
        /* 保存释放武将的id */
        m_pGameState->setFreeIdx(m_curHeroIdx);
    }
}

/*
 *  花费元宝
 */
void JianyuLayer::_confirmUseGold() {
    PrisonHeroItem* pPrisonHeroItem = m_pHeroManager->getPrisonHeroItemByIndex(m_curHeroIdx);
    if (pPrisonHeroItem) {
        char szHttpTag[] = HTTP_ZHAOXIANG_HERO_TAG;
        NetConnection* pNetConnection = NetConnection::getInstance();
        char szPostBuffer[MAX_POSTTO_SERVER_BUFFER_SIZE];
        memset(szPostBuffer, '\0', MAX_POSTTO_SERVER_BUFFER_SIZE);
        
        sprintf(szPostBuffer, "c=prison&a=summon&hero=%d&uid=%s", pPrisonHeroItem->getHeroItem()->getId(),
                m_pGamePlayer->getUid().c_str());
        CCLog("post 花费元宝 = %s", szPostBuffer);
        pNetConnection->commitPostRequestData(szPostBuffer, szHttpTag);
    }
}

/* 改变忠诚度动画 */
void JianyuLayer::_changeLoyalAni() {
    unschedule(schedule_selector(JianyuLayer::_schduleForLoyal));
    schedule(schedule_selector(JianyuLayer::_schduleForLoyal), 0.01f);
}

/*
 *  进度条调度
 */
void JianyuLayer::_schduleForLoyal() {
    char buffer[16];
    PrisonHeroItem* pPrisonHeroItem = m_pHeroManager->getPrisonHeroItemByIndex(m_curHeroIdx);
    CCAssert(pPrisonHeroItem, "invalid data");

    if (m_oldLoyal > pPrisonHeroItem->getDevotion()) {
        sprintf(buffer, "%d/100", m_oldLoyal);
        pLoyaltyLabel->setString(buffer);
        pLoyalProgress->setPercentage((float)m_oldLoyal);
        m_oldLoyal--;
    } else {
        if (m_oldLoyal == 0) {
            pLoyaltyLabel->setVisible(false);
        } else {
            sprintf(buffer, "%d/100", m_oldLoyal);
            pLoyaltyLabel->setString(buffer);
        }
        pLoyalProgress->setPercentage((float)m_oldLoyal);
        unschedule(schedule_selector(JianyuLayer::_schduleForLoyal));
    }
}

/* 
 *  功能：
 *      设置忠诚度UI所需元宝和道具
 *  参数：
 *      pPrisonHeroItem     监狱武将
 */
void JianyuLayer::_setLoyal(PrisonHeroItem* pPrisonHeroItem) {
    CCAssert(pPrisonHeroItem, "invalid pPrisonHeroItem");
    
    char buffer[16];
    sprintf(buffer, "%d/100", pPrisonHeroItem->getDevotion());
    pLoyaltyLabel->setString(buffer);
    pLoyalProgress->setPercentage((float)pPrisonHeroItem->getDevotion());
    /* needGem */
    sprintf(buffer, "%d", pPrisonHeroItem->getMoney());
    pCopperLabel->setString(buffer);
    
    if (pPrisonHeroItem->getGold() != 0)/*材料不足用元宝扣除*/ {
        pGoldSprite->setVisible(true);
        pPropSPrite->setVisible(false);
        sprintf(buffer, "x%d", pPrisonHeroItem->getGold());
        pPropOrGold->setString(buffer);
    } else {
        pGoldSprite->setVisible(false);
        pPropSPrite->setVisible(true);
        sprintf(buffer, "x%d", pPrisonHeroItem->getNum());
        pPropOrGold->setString(buffer);
    }
}

/*更新武将所需要的铜币装备或者元宝*/
void JianyuLayer::_updateNeed(PrisonHeroItem* pPrisonHeroItem) {
    CCAssert(pPrisonHeroItem, "invalid PrisonHeroItem");
    
    char buffer[16];
    /* needGem */
    sprintf(buffer, "%d", pPrisonHeroItem->getMoney());
    pCopperLabel->setString(buffer);
    
    if (pPrisonHeroItem->getGold() != 0)/*材料不足用元宝扣除*/ {
        pGoldSprite->setVisible(true);
        pPropSPrite->setVisible(false);
        sprintf(buffer, "x%d", pPrisonHeroItem->getGold());
        pPropOrGold->setString(buffer);
    } else {
        pGoldSprite->setVisible(false);
        pPropSPrite->setVisible(true);
        sprintf(buffer, "x%d", pPrisonHeroItem->getNum());
        pPropOrGold->setString(buffer);
    }
}

/*
 *  配合翻页的进度条动画
 */
void JianyuLayer::_runLoyal() {
    CCSprite* loyalBg = (CCSprite*)this->getChildByTag(NODE_TAG)->getChildByTag(PROGRESS_TAG_BG);
    pLoyaltyLabel->setOpacity(0);
    pLoyaltyLabel->runAction(CCFadeIn::create(MOVE_ONE_TIME));
    pLoyalProgress->setOpacity(0);
    pLoyalProgress->runAction(CCFadeIn::create(MOVE_ONE_TIME));
    loyalBg->setOpacity(0);
    loyalBg->runAction(CCFadeIn::create(MOVE_ONE_TIME));
}

/* 更新UI */
void JianyuLayer::_updateGamePlayerProp() {
    char buffer[16];
    sprintf(buffer, "x%d", m_pGamePlayer->getPropZiNum());
    CCLabelTTF* zi = (CCLabelTTF*)this->getChildByTag(NODE_TAG)->getChildByTag(ZICAILIAO_TAG);
    zi->setString(buffer);
    sprintf(buffer, "x%d", m_pGamePlayer->getPropLanNum());
    CCLabelTTF* lan = (CCLabelTTF*)this->getChildByTag(NODE_TAG)->getChildByTag(LANCAILIAO_TAG);
    lan->setString(buffer);
    
}

void JianyuLayer::_updateTips() {
    if (m_curHeroIdx == 0) {
        m_btnSprite[3][0]->setVisible(false);
        m_btnSprite[3][1]->setVisible(false);
        m_btnSprite[3][2]->setVisible(false);
        m_btnEnable[3] = false;
    } else {
        m_btnSprite[3][0]->setVisible(true);
        m_btnSprite[3][1]->setVisible(true);
        m_btnSprite[3][2]->setVisible(true);
        m_btnEnable[3] = true;
    }
}

/**重写父类层函数**/
void JianyuLayer::restoreTouchUICompent() {
    _tipMenu(TYPE_IN);
    
    CCSprite* parent = static_cast<CCSprite*>(m_pJyCBN->getChildByTag(PARENT_TAG));
    parent->runAction(CCMoveTo::create(DURATION_TIME, ccp(320.0f, m_pGameState->getBottomOffset() + 290.0f)));
    CCNode* pNode = (CCNode*)this->getChildByTag(NODE_TAG);
    pNode->runAction(CCMoveTo::create(DURATION_TIME, ccp(0, 0)));
    CCNode* pDangNode = (CCNode*)this->getChildByTag(9999);
    pDangNode->runAction(CCMoveTo::create(DURATION_TIME, ccp(0, 0)));
}

void JianyuLayer::moveOutTouchUICompent() {
    _tipMenu(TYPE_OUT);
    
    CCSprite* parent = static_cast<CCSprite*>(m_pJyCBN->getChildByTag(PARENT_TAG));
    parent->runAction(CCSequence::create(CCMoveTo::create(DURATION_TIME, LAYER_INIT_POSITION), CCCallFunc::create(this, callfunc_selector(JianyuLayer::_callbackForMoveOut)),NULL));
    CCNode* pNode = (CCNode*)this->getChildByTag(NODE_TAG);
    pNode->runAction(CCMoveTo::create(DURATION_TIME, ccp(0, LAYER_INIT_POSITION.y - m_pGameState->getBottomOffset() - 290.0f)));
    CCNode* pDangNode = (CCNode*)this->getChildByTag(9999);
    pDangNode->runAction(CCMoveTo::create(DURATION_TIME, ccp(0, LAYER_INIT_POSITION.y - m_pGameState->getBottomOffset() - 290.0f)));
    
}

//
void JianyuLayer::_callbackForMoveOut() {
    if (m_isRemoveSelf) {
        this->removeFromParent();
        CCLog("JianYuLayer 从主场景移除");
    }
}

//
void JianyuLayer::initPosition() {
    CCSprite* parent = static_cast<CCSprite*>(m_pJyCBN->getChildByTag(PARENT_TAG));
    parent->setPosition(LAYER_INIT_POSITION);
    CCNode* pNode = (CCNode*)this->getChildByTag(NODE_TAG);
    pNode->setPosition(ccp(0, LAYER_INIT_POSITION.y - m_pGameState->getBottomOffset() - 290.0f));
    CCNode* pDangNode = (CCNode*)this->getChildByTag(9999);
    pDangNode->setPosition(ccp(0, LAYER_INIT_POSITION.y - m_pGameState->getBottomOffset() - 290.0f));
}

/**重写层函数**/
void JianyuLayer::registerWithTouchDispatcher() {
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, false);
}

/*
 *  监狱武将翻页调度
 */
void JianyuLayer::update(float dt) {
    unsigned int curHeroIdxBak = m_curHeroIdx;
    
    if (1 == m_turnPageDirection) /*右向左，向后翻页*/ {
        if (m_heroSprite1->getPosition().x <= PRISON_HERO_X - m_currHeroSprite->getContentSize().width - PADDING_HERO) {
            m_curHeroIdx++;
        }
    } else/*左向右，向前移动*/ {
        if (m_heroSprite1->getPosition().x >= PRISON_HERO_X + m_currHeroSprite->getContentSize().width + PADDING_HERO) {
            m_curHeroIdx--;
        }
    }
    
    if (m_curHeroIdx == m_turnPageTargetHeroIdx) /* 移动结束 */ {
        m_isTrunPageing = false;
        
        CardBox* pTempSprite = m_heroSprite1;
        m_heroSprite1 = m_heroSprite2;
        m_heroSprite2 = pTempSprite;
        
        m_heroSprite1->stopAllActions();
        m_heroSprite2->stopAllActions();
        m_heroSprite1->removeFromParent();
        m_heroSprite2->removeFromParent();
        m_heroSprite1 = NULL;
        m_heroSprite2 = NULL;
        
        // 更新 ui
        _updatePrisonHeroItemUi();
        // 停止调度
        this->unscheduleUpdate();
    } else if (curHeroIdxBak != m_curHeroIdx) /*继续滚动*/{
        CardBox* pTempSprite = m_heroSprite1;
        m_heroSprite1 = m_heroSprite2;
        m_heroSprite2 = pTempSprite;
        
        unsigned int nextShowHeroIndex = (1 == m_turnPageDirection ? m_curHeroIdx + 1 : m_curHeroIdx - 1);
        m_heroSprite2->setCardForPrison(m_pHeroManager->getPrisonHeroItemByIndex(nextShowHeroIndex)->getHeroItem());
        if (1 == m_turnPageDirection) {
            m_heroSprite2->setPosition(ccp(m_heroSprite1->getPosition().x + m_heroSprite1->getContentSize().width + PADDING_HERO,
                                           PRISON_HERO_Y));
        } else {
            m_heroSprite2->setPosition(ccp(m_heroSprite1->getPosition().x - m_heroSprite1->getContentSize().width - PADDING_HERO,
                                           PRISON_HERO_Y));
        }
        m_heroSprite2->setVisible(true);
        
        /* 更新翻页显示 */
        char szPageBuffer[32];
        sprintf(szPageBuffer, "%d/%d", m_curHeroIdx + 1, m_countOfPrisonHeros);
        m_pagesLable->setString(szPageBuffer);
        
        // 进度条动画
        _runLoyal();
    }
}

/*
 *  触摸事件开始
 */
bool JianyuLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    if (m_isTrunPageing || 0 == m_countOfPrisonHeros) {
        return false;
    }
    
    m_touchBeganBtnIdx = -1;
    CCSprite* parent = static_cast<CCSprite*>(m_pJyCBN->getChildByTag(PARENT_TAG));
    CCPoint touchPoint = parent->convertTouchToNodeSpace(pTouch);
    CCPoint layerTouchPoint = this->convertTouchToNodeSpace(pTouch);
    
    /* 滑动区域控制 */
    if (m_heroRect.containsPoint(layerTouchPoint)) /* 玩家touch武将头像 */ {
        // 保存触摸起始位置
        m_touchHeroBeganPoint = layerTouchPoint;
        /* 保存触摸开始时间 */
        m_touchHeroBeganTime = clock();
        return true;
    }
    
    /* 检测是否 touch 了按钮 */
    for (unsigned int i = 0; i < JY_BTN_COUNT; ++i) {
        if (m_btnEnable[i] && m_btnSprite[i][0]->boundingBox().containsPoint(touchPoint)) {
            m_touchBeganBtnIdx = i;
            
            m_btnSprite[i][0]->setVisible(false);
            m_btnSprite[i][1]->setVisible(true);    // 显示按钮高亮
            break;
        }
    } /*for*/
    return m_touchBeganBtnIdx >= 0;
}

/*
 *
 */
void JianyuLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {
    _initBtn();
    CCSprite* parent = static_cast<CCSprite*>(m_pJyCBN->getChildByTag(PARENT_TAG));
    CCPoint touchPoint = parent->convertTouchToNodeSpace(pTouch);
    
    for (unsigned int i = 0; i < JY_BTN_COUNT; ++i) {
        if (m_btnEnable[i] && m_btnSprite[i][0]->boundingBox().containsPoint(touchPoint)) {
            m_btnSprite[i][0]->setVisible(false);
            m_btnSprite[i][1]->setVisible(true);
            break;
        }
    } /*for*/
}

/*
 *  touch 结束
 */
void JianyuLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {
    CCSprite* parent = static_cast<CCSprite*>(m_pJyCBN->getChildByTag(PARENT_TAG));
    CCPoint touchPoint = parent->convertTouchToNodeSpace(pTouch);
    CCPoint layerTouchPoint = this->convertTouchToNodeSpace(pTouch);
    
    if (m_touchBeganBtnIdx >= 0) /*touchBegan touch 了某个按钮*/{
        _initBtn();
        
        int selectBtnIdx = -1;
        for (unsigned int i = 0; i < JY_BTN_COUNT; ++i) {
            if (m_btnEnable[i] && m_btnSprite[i][0]->boundingBox().containsPoint(touchPoint)) {
                selectBtnIdx = i;
                break;
            }
        } /**/
        if (selectBtnIdx == m_touchBeganBtnIdx) {
            _onBtnSelect(selectBtnIdx + 1);
        }
        return;
    }
    
    /* 滑动区域控制 */
    /* 计算触摸结束时间和开始时间之间的时间差 */
    float touchTimeDelta = (float)((clock() - m_touchHeroBeganTime) / CLOCKS_PER_SEC);
    if (touchTimeDelta > TOUCH_TIME_DISABLE_LIMIT) /* touch 时间过长 */ {
        return;
    } else if (touchTimeDelta < TOUCH_TIME_LOW_LIMIT) {
        touchTimeDelta = TOUCH_TIME_LOW_LIMIT;
    } else if (touchTimeDelta > TOUCH_TIME_UPPER_LIMIT) {
        touchTimeDelta = TOUCH_TIME_UPPER_LIMIT;
    }
    // 计算 touch 开始位置和结束位置差
    float touchDelta = abs(layerTouchPoint.x - m_touchHeroBeganPoint.x);
    if (touchDelta < TOUCH_HERO_INTRO_DELTA) {
        PrisonHeroItem* pPrisonHeroItem = m_pHeroManager->getPrisonHeroItemByIndex(m_curHeroIdx);
        HeroItem* pHeroItem = pPrisonHeroItem->getHeroItem();
        if (pHeroItem) {
            /*获取武将id*/
            m_pGameState->setHeroIntroId(pHeroItem->getId());
            m_pGameState->setTagWhoCallHeroIntro(JY_LAYER) ;
            /*发送点击武将消息*/
            CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_JIANY_HEROINTRO_STRING);
        }
        return;
    }
    // 计算要滚动的武将数量
    unsigned int scrollOfHeros = (touchDelta / touchTimeDelta) /
        (m_currHeroSprite->getContentSize().width + PADDING_HERO);
    // 开始翻页
    _turnPagePrisonHeroItem(layerTouchPoint.x < m_touchHeroBeganPoint.x ? 1 : 2, (0 == scrollOfHeros) ? 1 : scrollOfHeros);
}

//
void JianyuLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {
    _initBtn();
}

/*
 * 功能：
 *      根据当前监狱武将数量和当前监狱武将索引确定按钮状态
 * 参数：
 *      countOfPrisonHeros      当前武将数量
 *      currPrisonHeroIndex     当前武将索引
 */
void JianyuLayer::_disableBtn(const unsigned int countOfPrisonHeros, const unsigned int currPrisonHeroIndex) {
    if (0 == countOfPrisonHeros) {
        for (unsigned int i = 0; i < JY_BTN_COUNT; ++i) {
            m_btnEnable[i] = false;
            m_btnSprite[i][0]->setVisible(false);
            m_btnSprite[i][0]->setPosition(m_btnPosition[i]);
            m_btnSprite[i][1]->setVisible(false);
            m_btnSprite[i][1]->setPosition(m_btnPosition[i]);
            m_btnSprite[i][2]->setVisible(true);
            m_btnSprite[i][2]->setPosition(m_btnPosition[i]);
        } /*for*/
    } else if (1 == countOfPrisonHeros) {
        for (unsigned int i = 2; i < 4; ++i) {
            m_btnEnable[i] = false;
            m_btnSprite[i][0]->setVisible(false);
            m_btnSprite[i][0]->setPosition(m_btnPosition[i]);
            m_btnSprite[i][1]->setVisible(false);
            m_btnSprite[i][1]->setPosition(m_btnPosition[i]);
            m_btnSprite[i][2]->setVisible(true);
            m_btnSprite[i][2]->setPosition(m_btnPosition[i]);
        }
    } else {
        if (0 == currPrisonHeroIndex) /*当前是第一个武将*/ {
            m_btnEnable[3] = false;
            m_btnSprite[3][0]->setVisible(false);
            m_btnSprite[3][0]->setPosition(m_btnPosition[3]);
            m_btnSprite[3][1]->setVisible(false);
            m_btnSprite[3][1]->setPosition(m_btnPosition[3]);
            m_btnSprite[3][2]->setVisible(true);
            m_btnSprite[3][2]->setPosition(m_btnPosition[3]);
        } else if (currPrisonHeroIndex == countOfPrisonHeros - 1) /*当前是最后一个武将*/ {
            m_btnEnable[2] = false;
            m_btnSprite[2][0]->setVisible(false);
            m_btnSprite[2][0]->setPosition(m_btnPosition[2]);
            m_btnSprite[2][1]->setVisible(false);
            m_btnSprite[2][1]->setPosition(m_btnPosition[2]);
            m_btnSprite[2][2]->setVisible(true);
            m_btnSprite[2][2]->setPosition(m_btnPosition[2]);
        } else {
            for (unsigned int i = 2; i < 4; ++i) {
                m_btnEnable[i] = true;
                m_btnSprite[i][0]->setVisible(true);
                m_btnSprite[i][0]->setPosition(m_btnPosition[i]);
                m_btnSprite[i][1]->setVisible(false);
                m_btnSprite[i][1]->setPosition(m_btnPosition[i]);
                m_btnSprite[i][2]->setVisible(false);
                m_btnSprite[i][2]->setPosition(m_btnPosition[i]);
            } /*for*/
        }
    }
}

/* 
 *  更新监狱武将相关 ui
 *  参数：
 *      loyaltyAni  true 调度忠诚度动画    false 直接更新忠诚度ui
 */
void JianyuLayer::_updatePrisonHeroItemUi(const bool loyaltyAni) {
    // 获取监狱武将的数量
    m_countOfPrisonHeros = m_pHeroManager->getCountOfPrisonHeros();
    
    if (0 == m_countOfPrisonHeros) /*没有监狱武将*/ {
        m_curHeroIdx = -1;
        m_pagesLable->setVisible(false);
        
        if (m_currHeroSprite) {
            m_currHeroSprite->removeFromParent();
            m_currHeroSprite = NULL;
        }
        // 更新按钮状态
        _disableBtn(m_countOfPrisonHeros, m_curHeroIdx);
        // 更新按钮上的所需物品UI显示
        pLoyaltyLabel->setVisible(false);
        pCopperLabel->setVisible(false);
        pPropOrGold->setVisible(false);
        pLoyalProgress->setVisible(false);
        pGoldSprite->setVisible(false);
        pPropSPrite->setVisible(false);
        
        return;
    }
    
    // 更新当前监狱武将索引号
    m_curHeroIdx = m_curHeroIdx < 0 ? 0 : m_curHeroIdx;

    PrisonHeroItem* pPrisonHeroItem = m_pHeroManager->getPrisonHeroItemByIndex(m_curHeroIdx);
    while (NULL == pPrisonHeroItem && m_curHeroIdx > 0) {
        if ((pPrisonHeroItem = m_pHeroManager->getPrisonHeroItemByIndex(--m_curHeroIdx)) ) {
            break;
        }
    } /*while*/
    CCAssert(pPrisonHeroItem, "invalid PrisonHeroItem");
    
    // 更新按钮状态
    _disableBtn(m_countOfPrisonHeros, m_curHeroIdx);
    
    /* 更新头像大卡 */
    HeroItem* pHeroItem = m_pHeroManager->getHeroItem(pPrisonHeroItem->getHeroId());
    if (NULL == m_currHeroSprite) {
        m_currHeroSprite = CardBox::create();
        m_currHeroSprite->setCardForPrison(pHeroItem);
        m_pDangSprite->addChild(m_currHeroSprite, 2);
    } else {
        m_currHeroSprite->setCardForPrison(pHeroItem);
    }
    m_currHeroSprite->setVisible(true);
    m_currHeroSprite->setPosition(ccp(PRISON_HERO_X, PRISON_HERO_Y));
    
    /* 更新页数 */
    char szPageBuffer[16];
    m_pagesLable->setVisible(true);
    sprintf(szPageBuffer, "%d/%d", m_curHeroIdx + 1, m_countOfPrisonHeros);
    m_pagesLable->setString(szPageBuffer);
    
    if (loyaltyAni) {
        _changeLoyalAni();
    } else {
        /*更新忠诚度*/
        _setLoyal(pPrisonHeroItem);
    }
    
    /*更新所需物品*/
    _updateNeed(pPrisonHeroItem);
}

/* 
 *  功能：
 *      监狱武将翻页
 *  参数：
 *      turnPageDir     翻页方向    1:从右向左(往后翻) 2:从左向右(往前翻)
 *      turnPageNumbers 翻页数
 */
void JianyuLayer::_turnPagePrisonHeroItem(const unsigned int turnPageDir, int turnPageNumbers) {
    do {
        CC_BREAK_IF(m_isTrunPageing);                                                   // 正在翻页中
        CC_BREAK_IF(0 == m_countOfPrisonHeros);                                         // 没有监狱武将
        CC_BREAK_IF(1 == turnPageDir && m_curHeroIdx == (m_countOfPrisonHeros - 1));    // 已经是最后一页
        CC_BREAK_IF(2 == turnPageDir && m_curHeroIdx == 0);                             // 已经是第一页
        
        // 保存翻页方向
        m_turnPageDirection = turnPageDir;

        // 隐藏当前武将
        m_currHeroSprite->setVisible(false);
        
        /* 生成两个用于翻页的监狱武将 */
        PrisonHeroItem* pCurrPrisonHeroItem = m_pHeroManager->getPrisonHeroItemByIndex(m_curHeroIdx);
        PrisonHeroItem* pNextPagePrisonHeroItem = NULL;
        /* 计算翻页的目标页数 */
        if (1 == turnPageDir) /*往后翻*/{
            m_turnPageTargetHeroIdx = ((m_countOfPrisonHeros - 1) - m_curHeroIdx < turnPageNumbers) ?
                m_countOfPrisonHeros - 1 : m_curHeroIdx + turnPageNumbers;
            // 调整为实际的翻页数
            turnPageNumbers = m_turnPageTargetHeroIdx - m_curHeroIdx;
            // 获取后续监狱武将
            pNextPagePrisonHeroItem = m_pHeroManager->getPrisonHeroItemByIndex(m_curHeroIdx + 1);
        } else /*往前翻*/ {
            m_turnPageTargetHeroIdx = (m_curHeroIdx - turnPageNumbers < 0) ?
                0 : m_curHeroIdx - turnPageNumbers;
            // 调整为实际的翻页数
            turnPageNumbers = m_curHeroIdx - m_turnPageTargetHeroIdx;
            // 获取前面的监狱武将
            pNextPagePrisonHeroItem = m_pHeroManager->getPrisonHeroItemByIndex(m_curHeroIdx - 1);
        }
        m_heroSprite1 = CardBox::create();
        m_heroSprite1->setCardForPrison(pCurrPrisonHeroItem->getHeroItem());
        m_pDangSprite->addChild(m_heroSprite1, 2);
        m_heroSprite1->setPosition(ccp(PRISON_HERO_X, PRISON_HERO_Y));
    
        m_heroSprite2 = CardBox::create();
        m_heroSprite2->setCardForPrison(pNextPagePrisonHeroItem->getHeroItem());
        m_pDangSprite->addChild(m_heroSprite2, 2);
        
        // 开始 move action
        float move_delta = 0.0f;
        if (1 == turnPageDir) /*往后翻*/{
            m_heroSprite2->setPosition(ccp(PRISON_HERO_X + m_heroSprite2->getContentSize().width + PADDING_HERO, PRISON_HERO_Y));
            move_delta = (m_heroSprite1->getContentSize().width + PADDING_HERO) * turnPageNumbers;
            move_delta = -move_delta;
        } else /*往前翻*/ {
            m_heroSprite2->setPosition(ccp(PRISON_HERO_X - m_heroSprite2->getContentSize().width - PADDING_HERO, PRISON_HERO_Y));
            move_delta = (m_heroSprite1->getContentSize().width + PADDING_HERO) * turnPageNumbers;
        }
        m_heroSprite1->runAction(CCMoveBy::create(turnPageNumbers * MOVE_ONE_TIME, ccp(move_delta, 0.0f)));
        m_heroSprite2->runAction(CCMoveBy::create(turnPageNumbers * MOVE_ONE_TIME, ccp(move_delta, 0.0f)));
        
        // 忠诚度进度条动画
        _runLoyal();
        // 开始翻页调度
        this->scheduleUpdate();
        // 标记正在移动
        m_isTrunPageing = true;
    } while (0);
}

/*
    头像大卡
    忠诚度进度条
    紫色材料
    蓝色材料
    再次招降所需金币
    再次招降所需元宝或材料
    左右翻页
    页数
 */
