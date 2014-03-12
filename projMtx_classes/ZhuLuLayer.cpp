//
//  ZhuLuLayer.cpp
//  hero
//
// 逐鹿层 隶属于主城
//

#include <ctime>
#include "CCStrokeLabel.h"
#include "ZhuLuLayer.h"
#include "GameState.h"
#include "CopyManager.h"
#include "CopyDataItem.h"
#include "LevelDataItem.h"
#include "customMessage.h"
#include "GamePlayer.h"
#include "MessageBox.h"
#include "NetConnection.h"

#define SCROLLVIEW_OFFSETX                  0.0f                        // 滚动视图的x偏移
#define SCROLLVIEW_OFFSETY                  0.0f                        // 滚动视图y偏移
#define COPY_COUNT                          8                           // 副本数量
#define OPENNING_ANI_DURING                 0.2                         // 动画执行时间
#define BTNOFFSET                           140
#define BOUNCE_LIMIT                        100                         //拖动限制

#define HUI_COLOR                           ccc3(100, 100, 85)

/* TAG值宏变量 */
#define CONTAINER_TAG                       1000                        // 整个层的TAG
#define MENU_TAG                            1100                        // 需要update的层TAG

#define FILTER_NORMAL                       1200                        // 副本难度筛选按钮背景的TAG
#define FILTER_DIFFIC                       1201                        // ..
#define FILTER_HELL                         1202                        // ..
#define FILTER_ACTIVI                       1203                        // ..

#define NORMAL                              1204                        // 筛选按钮TAG
#define DIFFIC                              1205                        // ..
#define HELL                                1206                        // ..
#define ACTIVI                              1207                        // ..

#define COPY_BTN_TAG                        1300                        // 副本按钮TAG(1~COPY_COUNT)

#define COPY_NODE                           1800                        // 每块副本内容TAG
#define COPY_MAP_TAG                        1900                        // 副本地图(1900~2499)

#define COPY_NAME_TAG                       2500                        // 副本名字TAG(2900~2999)
#define LEV_MAP_TAG                         3200                        // 关卡地图
#define LEV_NAME_TAG                        3400                        // 关卡名字tag
#define POWER_INFO_TAG                      3500                        // 体力信息
#define LEVEL_BTN_TAG                       10000                       // 关卡按钮TAG(50000~..)

#define LITMAPNODE                          4500
#define MAPNODE                             4000 

#define LL_MENU_TAG                         4600                        // 返回按钮
#define MID_MENU_TAG                        4601                        // 返回按钮
#define RR_MENU_TAG                         4602                        // 返回按钮

#define RESTORE_ACTION_TAG                  4650
#define MOVEOUT_ACTION_TAG                  4651

/*
 *  构造函数
 */
ZhuLuLayer::ZhuLuLayer()
:m_lastTime(0),
m_lastPos(CCPointZero),
m_tempPos(0),
m_xOffset(0),
m_diffcult(0),
m_isMvoing(false),
m_enableTouch(false),
m_isUpdateOne(false)
{}

/*
 *  析构函数
 */
ZhuLuLayer::~ZhuLuLayer() {
    CCTextureCache* pTextureCache = CCTextureCache::sharedTextureCache();
    
    pTextureCache->removeTextureForKey("FB_bigMapBg.png");
    pTextureCache->removeTextureForKey("FB_kuan.png");
    pTextureCache->removeTextureForKey("FB_norBg.png");
    pTextureCache->removeTextureForKey("FB_selBg.png");
    pTextureCache->removeTextureForKey("FB_nor.png");
    pTextureCache->removeTextureForKey("FB_norh.png");
    pTextureCache->removeTextureForKey("FB_jin.png");
    pTextureCache->removeTextureForKey("FB_jinh.png");
    pTextureCache->removeTextureForKey("FB_di.png");
    pTextureCache->removeTextureForKey("FB_dih.png");
    pTextureCache->removeTextureForKey("FB_act.png");
    pTextureCache->removeTextureForKey("FB_acth.png");
    pTextureCache->removeTextureForKey("FB_xdb.png");
    pTextureCache->removeTextureForKey("FB_zhulu_font.png");
    pTextureCache->removeTextureForKey("FB_dangb.png");
    pTextureCache->removeTextureForKey("FB_tianc.png");
    pTextureCache->removeTextureForKey("FB_dib.png");
    pTextureCache->removeTextureForKey("CM_left.png");
    pTextureCache->removeTextureForKey("LU_return.png");
    pTextureCache->removeTextureForKey("CM_mid.png");
    pTextureCache->removeTextureForKey("CM_right.png");
    pTextureCache->removeTextureForKey("FB_showJinh.png");
    pTextureCache->removeTextureForKey("FB_norx.png");
    pTextureCache->removeTextureForKey("FB_litMapBg.png");
    pTextureCache->removeTextureForKey("FB_litMapBg.png");
    pTextureCache->removeTextureForKey("FB_power_font.png");
    pTextureCache->removeTextureForKey("FB_power_font.png");
    
    
    
    pTextureCache->removeTextureForKey("FB_levbtn101s.png");
    pTextureCache->removeTextureForKey("FB_levbtn101s.png");
    pTextureCache->removeTextureForKey("FB_levbtn104s.png");
    pTextureCache->removeTextureForKey("FB_levbtn107s.png");
    pTextureCache->removeTextureForKey("FB_levbtn201s.png");
    pTextureCache->removeTextureForKey("FB_levbtn202s.png");
    pTextureCache->removeTextureForKey("FB_levbtn202s1.png");
    pTextureCache->removeTextureForKey("FB_levbtn203s.png");
    pTextureCache->removeTextureForKey("FB_levbtn204s.png");
    pTextureCache->removeTextureForKey("FB_levbtn205s.png");
    pTextureCache->removeTextureForKey("FB_levbtn207s.png");
    pTextureCache->removeTextureForKey("FB_levbtn208s.png");
    pTextureCache->removeTextureForKey("FB_levbtn208s1.png");
    pTextureCache->removeTextureForKey("FB_levbtn209s.png");
    pTextureCache->removeTextureForKey("FB_levbtn210s.png");
    pTextureCache->removeTextureForKey("FB_levbtn210s1.png");
    pTextureCache->removeTextureForKey("FB_levbtn211s.png");
    pTextureCache->removeTextureForKey("FB_levbtn212s.png");
    pTextureCache->removeTextureForKey("FB_levbtn212s1.png");
    pTextureCache->removeTextureForKey("FB_levbtn213s.png");
    pTextureCache->removeTextureForKey("FB_levbtn213s1.png");
    pTextureCache->removeTextureForKey("FB_levbtn214s.png");
    pTextureCache->removeTextureForKey("FB_levbtn215s.png");
    pTextureCache->removeTextureForKey("FB_levbtn302s.png");
    pTextureCache->removeTextureForKey("FB_levbtn303s.png");
    pTextureCache->removeTextureForKey("FB_levbtn303s1.png");
    pTextureCache->removeTextureForKey("FB_levbtn304s.png");
    pTextureCache->removeTextureForKey("FB_levbtn305s.png");
    pTextureCache->removeTextureForKey("FB_levbtn305s1.png");
    pTextureCache->removeTextureForKey("FB_levbtn306s.png");
    pTextureCache->removeTextureForKey("FB_levbtn306s1.png");
    pTextureCache->removeTextureForKey("FB_levbtn307s.png");
    pTextureCache->removeTextureForKey("FB_levbtn309s.png");
    pTextureCache->removeTextureForKey("FB_levbtn311s.png");
    pTextureCache->removeTextureForKey("FB_levbtn312s.png");
    pTextureCache->removeTextureForKey("FB_levbtn313s.png");
    pTextureCache->removeTextureForKey("FB_levbtn3014s.png");
    pTextureCache->removeTextureForKey("FB_levbtn3015s.png");
    pTextureCache->removeTextureForKey("FB_levbtn402s.png");
    pTextureCache->removeTextureForKey("FB_levbtn403s.png");
    pTextureCache->removeTextureForKey("FB_levbtn404s.png");
    pTextureCache->removeTextureForKey("FB_levbtn404s1.png");
    pTextureCache->removeTextureForKey("FB_levbtn405s.png");
    pTextureCache->removeTextureForKey("FB_levbtn406s.png");
    pTextureCache->removeTextureForKey("FB_levbtn407s.png");
    pTextureCache->removeTextureForKey("FB_levbtn407s1.png");
    pTextureCache->removeTextureForKey("FB_levbtn408s.png");
    pTextureCache->removeTextureForKey("FB_levbtn409s.png");
    pTextureCache->removeTextureForKey("FB_levbtn409s1.png");
    pTextureCache->removeTextureForKey("FB_levbtn411s.png");
    pTextureCache->removeTextureForKey("FB_levbtn411s1.png");
    pTextureCache->removeTextureForKey("FB_levbtn501s.png");
    pTextureCache->removeTextureForKey("FB_levbtn502s.png");
    pTextureCache->removeTextureForKey("FB_levbtn503s.png");
    pTextureCache->removeTextureForKey("FB_levbtn504s.png");
    pTextureCache->removeTextureForKey("FB_levbtn505s.png");
    pTextureCache->removeTextureForKey("FB_levbtn506s.png");
    pTextureCache->removeTextureForKey("FB_levbtn506s1.png");
    pTextureCache->removeTextureForKey("FB_levbtn507s.png");
    pTextureCache->removeTextureForKey("FB_levbtn508s.png");
    pTextureCache->removeTextureForKey("FB_levbtn510s.png");
    pTextureCache->removeTextureForKey("FB_levbtn511s1.png");
    pTextureCache->removeTextureForKey("FB_levbtn513s.png");
    pTextureCache->removeTextureForKey("FB_levbtn513s1.png");
    pTextureCache->removeTextureForKey("FB_levbtn512s2.png");
    pTextureCache->removeTextureForKey("FB_levbtn512s1.png");
    pTextureCache->removeTextureForKey("FB_levbtn514s.png");
    pTextureCache->removeTextureForKey("FB_levbtn515s.png");
    pTextureCache->removeTextureForKey("FB_levbtn515s1.png");
    pTextureCache->removeTextureForKey("FB_levbtn516s.png");
    pTextureCache->removeTextureForKey("FB_levbtn516s1.png");
    pTextureCache->removeTextureForKey("FB_levbtn519s.png");
    pTextureCache->removeTextureForKey("FB_levbtn518s.png");
    pTextureCache->removeTextureForKey("FB_levbtn520s.png");
    pTextureCache->removeTextureForKey("FB_levbtn601s.png");
    pTextureCache->removeTextureForKey("FB_levbtn602s.png");
    pTextureCache->removeTextureForKey("FB_levbtn604s.png");
    pTextureCache->removeTextureForKey("FB_levbtn606s.png");
    pTextureCache->removeTextureForKey("FB_levbtn607s.png");
    pTextureCache->removeTextureForKey("FB_levbtn606s1.png");
    pTextureCache->removeTextureForKey("FB_levbtn605s.png");
    pTextureCache->removeTextureForKey("FB_levbtn608s.png");
    pTextureCache->removeTextureForKey("FB_levbtn608s1.png");
    pTextureCache->removeTextureForKey("FB_levbtn609s.png");
    pTextureCache->removeTextureForKey("FB_levbtn611s.png");
    pTextureCache->removeTextureForKey("FB_levbtn611s1.png");
    pTextureCache->removeTextureForKey("FB_levbtn610s.png");
    pTextureCache->removeTextureForKey("FB_levbtn612s.png");
    pTextureCache->removeTextureForKey("FB_levbtn614s.png");
    pTextureCache->removeTextureForKey("FB_levbtn614s1.png");
    pTextureCache->removeTextureForKey("FB_levbtn614s2.png");
    pTextureCache->removeTextureForKey("FB_levbtn615s.png");
    pTextureCache->removeTextureForKey("FB_levbtn701s.png");
    pTextureCache->removeTextureForKey("FB_levbtn703s.png");
    pTextureCache->removeTextureForKey("FB_levbtn705s1.png");
    pTextureCache->removeTextureForKey("FB_levbtn705s.png");
    pTextureCache->removeTextureForKey("FB_levbtn706s.png");
    pTextureCache->removeTextureForKey("FB_levbtn707s.png");
    pTextureCache->removeTextureForKey("FB_levbtn708s.png");
    pTextureCache->removeTextureForKey("FB_levbtn708s1.png");
    pTextureCache->removeTextureForKey("FB_levbtn709s.png");
    pTextureCache->removeTextureForKey("FB_levbtn709s1.png");
    pTextureCache->removeTextureForKey("FB_levbtn710s.png");
    pTextureCache->removeTextureForKey("FB_levbtn711s.png");
    pTextureCache->removeTextureForKey("FB_levbtn711s1.png");
    pTextureCache->removeTextureForKey("FB_levbtn714s.png");
    pTextureCache->removeTextureForKey("FB_levbtn715s.png");
    pTextureCache->removeTextureForKey("FB_levbtn801s.png");
    pTextureCache->removeTextureForKey("FB_levbtn802s.png");
    pTextureCache->removeTextureForKey("FB_levbtn803s.png");
    pTextureCache->removeTextureForKey("FB_levbtn803s1.png");
    pTextureCache->removeTextureForKey("FB_levbtn804s.png");
    pTextureCache->removeTextureForKey("FB_levbtn804s1.png");
    pTextureCache->removeTextureForKey("FB_levbtn805s.png");
    pTextureCache->removeTextureForKey("FB_levbtn805s1.png");
    pTextureCache->removeTextureForKey("FB_levbtn807s.png");
    pTextureCache->removeTextureForKey("FB_levbtn807s1.png");
    pTextureCache->removeTextureForKey("FB_levbtn806s.png");
    pTextureCache->removeTextureForKey("FB_levbtn810s1.png");
    pTextureCache->removeTextureForKey("FB_levbtn810s.png");
    pTextureCache->removeTextureForKey("FB_levbtn809s.png");
    pTextureCache->removeTextureForKey("FB_levbtn809s1.png");
    pTextureCache->removeTextureForKey("FB_levbtn811s.png");
    pTextureCache->removeTextureForKey("FB_levbtn814s.png");
    pTextureCache->removeTextureForKey("FB_levbtn815s.png");
    pTextureCache->removeTextureForKey("FB_levbtn815s1.png");
    pTextureCache->removeTextureForKey("FB_levbtn816s1.png");
    pTextureCache->removeTextureForKey("FB_levbtn816s2.png");
    pTextureCache->removeTextureForKey("FB_levbtn818s.png");
    pTextureCache->removeTextureForKey("FB_levbtn819s.png");
    pTextureCache->removeTextureForKey("FB_levbtn819s1.png");
    pTextureCache->removeTextureForKey("FB_levbtn817s.png");
    pTextureCache->removeTextureForKey("FB_levbtn817s1.png");
    pTextureCache->removeTextureForKey("FB_levbtn820s.png");
    pTextureCache->removeTextureForKey("FB_levbtn821s.png");
    pTextureCache->removeTextureForKey("FB_levbtn824s.png");
    pTextureCache->removeTextureForKey("FB_levbtn823s.png");
    pTextureCache->removeTextureForKey("FB_levbtn823s1.png");
    
    
    for (int i = 0; i < 8; ++i) {
        pTextureCache->removeTextureForKey("FB_bmap%d.png");
        pTextureCache->removeTextureForKey("FB_mapFont%d.png");
        pTextureCache->removeTextureForKey("FB_btnLay%d.png");
        pTextureCache->removeTextureForKey("FB_font_tip%d.png");
    }
    
    for (int j = 0; j < 8; ++j) {
        for (int i = 1; i < 25; ++i) {
            CCString* str = CCString::createWithFormat("FB_lit%d.png", (j + 1) * 100 + i);
            pTextureCache->removeTextureForKey(str->getCString());
            CCString* str1 = CCString::createWithFormat("FB_levbtn%d.png", (j + 1) * 100 + i);
            pTextureCache->removeTextureForKey(str1->getCString());
            CCString* str2 = CCString::createWithFormat("FB_hj%d.png", (j + 1) * 100 + i);
            pTextureCache->removeTextureForKey(str2->getCString());
        } 
    }
}

/*
 *  初始化
 */
bool ZhuLuLayer::init() {
    // super init
    if (!CCLayer::init()) {
        return false;
    }
    
    //添加容器,所有随层同步移入主场景的内容全部加入容器中
    CCNode* pContainer = CCNode::create();
    this->addChild(pContainer, 1, CONTAINER_TAG);
    
    /*************************************************************
     出征页面，所有随层同步移动的加入容器，否则加入this
     *************************************************************/
    
    ////////首先更新在大地图///////////
    update(0);
    ////////首先更新在大地图///////////
    
    return true;
}

/***************************************************
 更新函数，初始化更新在 mode = 0，大地图. mode = 1 小地图
 ***************************************************/
void ZhuLuLayer::update(int mode) {
    if (mode == 0) {
        m_isUpdateOne = false;
        tipMenu(false);
    } else if (mode == 1) {
        m_isUpdateOne = true;
        tipMenu(true);
    }
    
    const float bottomOffset = GameState::getInstance()->getBottomOffset();
    
    /*
     *  地图
     */
    if (this->getChildByTag(CONTAINER_TAG)->getChildByTag(MENU_TAG)) {
        this->getChildByTag(CONTAINER_TAG)->removeChildByTag(MENU_TAG, true);
    }
    CCNode* pNode = CCNode::create();
    if (0 == mode) {
        // 副本页面背景
        CCSprite* pBgSprite = CCSprite::create("FB_bigMapBg.png");
        pBgSprite->setAnchorPoint(CCPointZero);
        pBgSprite->setPosition(ccp(0.0f, 124.0f + bottomOffset));
        pNode->addChild(pBgSprite, 1);
        
        /* 添加地图框边界挡板 */
        CCSprite* pXdb = CCSprite::create("FB_xdb.png");
        pXdb->setAnchorPoint(CCPointZero);
        pXdb->setPosition(ccp(0.0f, bottomOffset + 125.0f));
        pNode->addChild(pXdb, 1000);
        CCSprite* pXdb2 = CCSprite::create("FB_xdb.png");
        pXdb2->setFlipX(true);
        pXdb2->setAnchorPoint(CCPointZero);
        pXdb2->setPosition(ccp(632.0f, bottomOffset + 125.0f));
        pNode->addChild(pXdb2, 1000);
        
        // 添加筛选按钮
        CCSprite* pKuan = CCSprite::create("FB_kuan.png");
        pKuan->setAnchorPoint(ccp(0, 0));
        pKuan->setPosition(ccp(0, 735 + bottomOffset));
        pNode->addChild(pKuan, 1);
        
        // 选择难度按钮背景、按钮被选中
        for (int i = 0; i < 4; ++i) {
            CCSprite* difficBg = CCSprite::create("FB_norBg.png");
            difficBg->setAnchorPoint(ccp(0, 0));
            difficBg->setPosition(ccp(35 + i * BTNOFFSET, 735.0f + bottomOffset));
            pNode->addChild(difficBg, 2);
        }
        CCSprite* selBg1 = CCSprite::create("FB_selBg.png");
        selBg1->setVisible(true);
        selBg1->setAnchorPoint(ccp(0, 0));
        selBg1->setPosition(ccp(35 ,735.0f + bottomOffset));
        pNode->addChild(selBg1, 2, FILTER_NORMAL);
        CCSprite* selBg2 = CCSprite::create("FB_selBg.png");
        selBg2->setVisible(false);
        selBg2->setAnchorPoint(ccp(0, 0));
        selBg2->setPosition(ccp(35 + BTNOFFSET, 735.0f + bottomOffset));
        pNode->addChild(selBg2, 2, FILTER_DIFFIC);
        CCSprite* selBg3 = CCSprite::create("FB_selBg.png");
        selBg3->setVisible(false);
        selBg3->setAnchorPoint(ccp(0, 0));
        selBg3->setPosition(ccp(35 + BTNOFFSET * 2 ,735.0f + bottomOffset));
        pNode->addChild(selBg3, 2, FILTER_HELL);
        CCSprite* selBg4 = CCSprite::create("FB_selBg.png");
        selBg4->setVisible(false);
        selBg4->setAnchorPoint(ccp(0, 0));
        selBg4->setPosition(ccp(35 + BTNOFFSET * 3 ,735.0f + bottomOffset));
        pNode->addChild(selBg4, 2, FILTER_ACTIVI);
        
        
        // 普通难度
        CCMenuItemImage *pNormalItem = CCMenuItemImage::create("FB_nor.png",
                                                               "FB_nor.png",
                                                               "FB_norh.png",
                                                               this,
                                                               menu_selector(ZhuLuLayer::filterCallback));
        pNormalItem->setTag(NORMAL);
        pNormalItem->setAnchorPoint(CCPointZero);
        pNormalItem->setPosition(ccp(30.0f, 735.0f + bottomOffset));
        // 困难难度
        CCMenuItemImage *pDifficultItem = CCMenuItemImage::create("FB_jin.png",
                                                                  "FB_jin.png",
                                                                  "FB_jinh.png",
                                                                  this,
                                                                  menu_selector(ZhuLuLayer::filterCallback));
        pDifficultItem->setTag(DIFFIC);
        pDifficultItem->setEnabled(false);
        pDifficultItem->setAnchorPoint(CCPointZero);
        pDifficultItem->setPosition(ccp(30.0f + BTNOFFSET, 735.0f + bottomOffset));
        // 地狱难度
        CCMenuItemImage *pHellItem = CCMenuItemImage::create("FB_di.png",
                                                             "FB_di.png",
                                                             "FB_dih.png",
                                                             this,
                                                             menu_selector(ZhuLuLayer::filterCallback));
        pHellItem->setTag(HELL);
        pHellItem->setEnabled(false);
        pHellItem->setAnchorPoint(CCPointZero);
        pHellItem->setPosition(ccp(30.0f + BTNOFFSET * 2, 735.0f + bottomOffset));
        // 活动副本
        CCMenuItemImage *pActivityItem = CCMenuItemImage::create("FB_act.png",
                                                                 "FB_act.png",
                                                                 "FB_acth.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::filterCallback));
        pActivityItem->setTag(ACTIVI);
        pActivityItem->setEnabled(false);
        pActivityItem->setAnchorPoint(CCPointZero);
        pActivityItem->setPosition(ccp(70.0f + BTNOFFSET * 3, 750.0f + bottomOffset));
        CCMenu* pMenu = CCMenu::create(pNormalItem, pDifficultItem, pHellItem, pActivityItem, NULL);
        pMenu->setAnchorPoint(ccp(0, 0));
        pMenu->setPosition(ccp(0, 0));
        pNode->addChild(pMenu, 5);
        
        
        /*
         * 地图
         * 添加副本选择按钮
         * 限制玩家进入等级，限制显示。
         */
        // 第一个副本
        char buffer[32];/*缓冲区*/
        
        int playerLev = GamePlayer::getInstance()->getLevel();/*玩家等级*/
        
        /* 添加滚动视图 */
        if (playerLev > 27 && playerLev < 33) {
            m_scrollView = CCScrollView::create(CCSizeMake(810, 960));
            m_scrollView->setContentSize(CCSizeMake(810, 960));
            //        m_scrollView->setViewSize(CCSizeMake(950, 960));
        } else if (playerLev > 32) {
            m_scrollView = CCScrollView::create(CCSizeMake(1110, 960));
            m_scrollView->setContentSize(CCSizeMake(1110, 960));
            //        m_scrollView->setViewSize(CCSizeMake(950, 960));
        } else {
            m_scrollView = CCScrollView::create(CCSizeMake(640, 960));
            m_scrollView->setContentSize(CCSizeMake(640, 960));
            //        m_scrollView->setViewSize(CCSizeMake(950, 960));
        }
        
        m_scrollView->setAnchorPoint(CCPointZero);
        m_scrollView->setPosition(ccp(SCROLLVIEW_OFFSETX, bottomOffset - 75.0f));
        m_scrollView->setDirection(kCCScrollViewDirectionHorizontal);
        m_scrollView->setBounceable(false);
        m_scrollView->setDelegate(this);
        pNode->addChild(m_scrollView, 100);
        
        /* 大地图名称 */
        CCSprite* bigMapFont = CCSprite::create("FB_zhulu_font.png");
        bigMapFont->setAnchorPoint(ccp(0.5f, 0.0f));
        bigMapFont->setPosition(ccp(340.0f, bottomOffset + 635.0f));
        pNode->addChild(bigMapFont, 10);
        
        /* 地图node 存放地图背景和所有scrollview内容 */
        CCNode* pMapNode = CCNode::create();
        pMapNode->setAnchorPoint(CCPointZero);
        
        /* 普通副本图片资源加载 */
        for (int i = 0; i < COPY_COUNT; ++i) {
            
            /* 存放每块内容 dddd*/
            CCNode* pOneNode = CCNode::create();
            pOneNode->setAnchorPoint(ccp(0, 0));
            pMapNode->addChild(pOneNode, 150 + i, COPY_NODE + i);
            
            /* 背景地图 */
            sprintf(buffer, "FB_bmap%d.png", i+1);
            CCSprite* mapBg = CCSprite::create(buffer);
            mapBg->setAnchorPoint(ccp(0, 0));
//            mapBg->setColor(ccc3(255, 0, 0));//kknd
            pOneNode->addChild(mapBg, 1, COPY_MAP_TAG+i);
            
            /* 副本名 */
            sprintf(buffer, "FB_mapFont%d.png",i+1);
            CCSprite* pName = CCSprite::create(buffer);
            pName->setAnchorPoint(ccp(0, 0));
            pOneNode->addChild(pName, 200, COPY_NAME_TAG+i);
            
            
            /* 透明按钮层 */
            sprintf(buffer, "FB_btnLay%d.png", i+1);
            CCMenuItemImage *pBtn = CCMenuItemImage::create(buffer,
                                                            buffer,
                                                            this,
                                                            menu_selector(ZhuLuLayer::chosCopy));
            pBtn->setTag(COPY_BTN_TAG + 1 + i);
            pBtn->setAnchorPoint(ccp(0, 0));
            pBtn->setPosition(0, 0);
            CCMenu* pBtnMenu = CCMenu::create(pBtn, NULL);
            pBtnMenu->setAnchorPoint(ccp(0, 0));
            pBtnMenu->setPosition(0, 0);
            pOneNode->addChild(pBtnMenu, 199, COPY_BTN_TAG+200+i);
            
            
            /* 无法进入副本锁显示，和提示等级 */
            int showLev = CopyManager::getInstance()->getCopyData(i+1)->getSl();/* 显示等级 */
            int enterLev = CopyManager::getInstance()->getCopyData(i+1)->getEl();/* 进入等级 */
            CCSprite* mapLock = CCSprite::create("CM_mapLock.png");
            mapLock->setAnchorPoint(ccp(0, 0));
            mapLock->setPosition(ccp(60, -50));
            mapLock->setVisible(false);
            pName->addChild(mapLock, 200);
            sprintf(buffer, "进入等级%d", enterLev);
            CCLabelTTF* tipEnterLev = CCLabelTTF::create(buffer, "Arial", 21);
            tipEnterLev->setColor(ccc3(255, 0, 0));
            tipEnterLev->setAnchorPoint(ccp(0, 0));
            tipEnterLev->setPosition(ccp(40, -80));
            tipEnterLev->setVisible(false);
            pName->addChild(tipEnterLev, 200);

            if (playerLev < showLev) {
                pOneNode->setVisible(false);
            }
            /* 回调时判断 */
            if (((i + 1 > GameState::getInstance()->getNormalMaxCopyId()))) {
                mapBg->setColor(HUI_COLOR);
                pName->setColor(HUI_COLOR);
                mapLock->setVisible(true);
                if (playerLev < enterLev) {
                    tipEnterLev->setVisible(true);
                }
            }
        }/*for*/
        pMapNode->setPosition(ccp(0, 0));
        m_scrollView->getContainer()->addChild(pMapNode, 1, MAPNODE);
        /*
         * 调整副本以及地图背景位置
         * p1: 透明层按钮
         * spr1: 地图背景
         * pFont1: 副本名
         */
        CCMenu* p1 = (CCMenu*)pMapNode->getChildByTag(COPY_NODE)->getChildByTag(COPY_BTN_TAG+200);
        p1->setPosition(20, 455);
        CCSprite* spr1 = (CCSprite*)pMapNode->getChildByTag(COPY_NODE)->getChildByTag(COPY_MAP_TAG);
        spr1->setPosition(ccp(10, 416));
        CCSprite* pFont1 = (CCSprite*)pMapNode->getChildByTag(COPY_NODE)->getChildByTag(COPY_NAME_TAG);
        pFont1->setPosition(ccp(5, 580));
        CCMenu* p2 = (CCMenu*)pMapNode->getChildByTag(COPY_NODE+1)->getChildByTag(COPY_BTN_TAG+200+1);
        p2->setPosition(25, 245);
        CCSprite* spr2 = (CCSprite*)pMapNode->getChildByTag(COPY_NODE+1)->getChildByTag(COPY_MAP_TAG+1);
        spr2->setPosition(ccp(10, 230));
        CCSprite* pFont2 = (CCSprite*)pMapNode->getChildByTag(COPY_NODE+1)->getChildByTag(COPY_NAME_TAG+1);
        pFont2->setPosition(ccp(35, 350));
        CCMenu* p3 = (CCMenu*)pMapNode->getChildByTag(COPY_NODE+2)->getChildByTag(COPY_BTN_TAG+200+2);
        p3->setPosition(215, 445);
        CCSprite* spr3 = (CCSprite*)pMapNode->getChildByTag(COPY_NODE+2)->getChildByTag(COPY_MAP_TAG+2);
        spr3->setPosition(ccp(183, 406));
        CCSprite* pFont3 = (CCSprite*)pMapNode->getChildByTag(COPY_NODE+2)->getChildByTag(COPY_NAME_TAG+2);
        pFont3->setPosition(ccp(180, 580));
        CCMenu* p4 = (CCMenu*)pMapNode->getChildByTag(COPY_NODE+3)->getChildByTag(COPY_BTN_TAG+200+3);
        p4->setPosition(275, 255);
        CCSprite* spr4 = (CCSprite*)pMapNode->getChildByTag(COPY_NODE+3)->getChildByTag(COPY_MAP_TAG+3);
        spr4->setPosition(ccp(174, 231));
        CCSprite* pFont4 = (CCSprite*)pMapNode->getChildByTag(COPY_NODE+3)->getChildByTag(COPY_NAME_TAG+3);
        pFont4->setPosition(ccp(290, 350));
        CCMenu* p5 = (CCMenu*)pMapNode->getChildByTag(COPY_NODE+4)->getChildByTag(COPY_BTN_TAG+200+4);
        p5->setPosition(410, 460);
        CCSprite* spr5 = (CCSprite*)pMapNode->getChildByTag(COPY_NODE+4)->getChildByTag(COPY_MAP_TAG+4);
        spr5->setPosition(ccp(374, 439));
        CCSprite* pFont5 = (CCSprite*)pMapNode->getChildByTag(COPY_NODE+4)->getChildByTag(COPY_NAME_TAG+4);
        pFont5->setPosition(ccp(410, 580));
        CCMenu* p6 = (CCMenu*)pMapNode->getChildByTag(COPY_NODE+5)->getChildByTag(COPY_BTN_TAG+200+5);
        p6->setPosition(530, 245);
        CCSprite* spr6 = (CCSprite*)pMapNode->getChildByTag(COPY_NODE+5)->getChildByTag(COPY_MAP_TAG+5);
        spr6->setPosition(ccp(495, 235));
        CCSprite* pFont6 = (CCSprite*)pMapNode->getChildByTag(COPY_NODE+5)->getChildByTag(COPY_NAME_TAG+5);
        pFont6->setPosition(ccp(550, 350));
        CCMenu* p7 = (CCMenu*)pMapNode->getChildByTag(COPY_NODE+6)->getChildByTag(COPY_BTN_TAG+200+6);
        p7->setPosition(630, 470);
        CCSprite* spr7 = (CCSprite*)pMapNode->getChildByTag(COPY_NODE+6)->getChildByTag(COPY_MAP_TAG+6);
        spr7->setPosition(ccp(580, 420));
        CCSprite* pFont7 = (CCSprite*)pMapNode->getChildByTag(COPY_NODE+6)->getChildByTag(COPY_NAME_TAG+6);
        pFont7->setPosition(ccp(700, 580));
        CCMenu* p8 = (CCMenu*)pMapNode->getChildByTag(COPY_NODE+7)->getChildByTag(COPY_BTN_TAG+200+7);
        p8->setPosition(785, 240);
        CCSprite* spr8 = (CCSprite*)pMapNode->getChildByTag(COPY_NODE+7)->getChildByTag(COPY_MAP_TAG+7);
        spr8->setPosition(ccp(765, 236));
        CCSprite* pFont8 = (CCSprite*)pMapNode->getChildByTag(COPY_NODE+7)->getChildByTag(COPY_NAME_TAG+7);
        pFont8->setPosition(ccp(800, 350));
    } else if (1 == mode) {
        char buffer[64];
        /* 挡板 */
        CCSprite* pDang = CCSprite::create("FB_dangb.png");
        pDang->setAnchorPoint(CCPointZero);
        pDang->setPosition(ccp(0.0f, bottomOffset + 125.0f));
        pNode->addChild(pDang, 100);
        /* 添加边界挡板 */
        CCSprite* pXdb = CCSprite::create("FB_xdb.png");
        pXdb->setFlipX(true);
        pXdb->setAnchorPoint(CCPointZero);
        pXdb->setPosition(ccp(632.0f, bottomOffset + 125.0f));
        pNode->addChild(pXdb, 1000);
        /* 按钮填充板 */
        CCSprite* spTianc = CCSprite::create("FB_tianc.png");
        spTianc->setAnchorPoint(CCPointZero);
        spTianc->setPosition(ccp(0.0f ,bottomOffset + 735.0f));
        pNode->addChild(spTianc, 10);
        /* 文字底板 */
        CCSprite* spDi = CCSprite::create("FB_dib.png");
        spDi->setAnchorPoint(CCPointZero);
        spDi->setPosition(ccp(10.0f, bottomOffset + 160.0f));
        pNode->addChild(spDi, 101);
        
        /* 返回按钮 */
        CCNode* pLLContainer = CCNode::create();
        this->addChild(pLLContainer, 1, LL_MENU_TAG);
        CCNode* pMIDContainer = CCNode::create();
        this->addChild(pMIDContainer, 1, MID_MENU_TAG);
        CCNode* pRRContainer = CCNode::create();
        this->addChild(pRRContainer, 1, RR_MENU_TAG);

        CCMenuItemImage *pReturnItem = CCMenuItemImage::create("CM_left.png",
                                                               "CM_left.png",
                                                               this,
                                                               menu_selector(ZhuLuLayer::
                                                                             option));
        pReturnItem->setAnchorPoint(CCPointZero);
        pReturnItem->setPosition(ccp(0.0f, bottomOffset + 753.0f));
        CCMenu* pLMenu = CCMenu::create(pReturnItem, NULL);
        pLMenu->setAnchorPoint(CCPointZero);
        pLMenu->setPosition(CCPointZero);
        pLLContainer->addChild(pLMenu, 2);
        /* 返回文字 */
        CCSprite* returnFont = CCSprite::create("LU_return.png");
        returnFont->setAnchorPoint(CCPointZero);
        returnFont->setPosition(ccp(17.0f, bottomOffset + 758.0f));
        pLLContainer->addChild(returnFont,2);
        /* 中间标签 */
        CCSprite* pMiddleImage = CCSprite::create("CM_mid.png");
        pMiddleImage->setAnchorPoint(CCPointZero);
        pMiddleImage->setPosition(ccp(165.0f, bottomOffset + 753.0f));
        pMIDContainer->addChild(pMiddleImage, 2);
        // 所选择副本难度
        if (m_diffcult == 0) {
            strcpy(buffer, "FB_showNor.png");
        } else if (m_diffcult == 1) {
            strcpy(buffer, "FB_showJin.png");
        }
        CCSprite* pMyhImage = CCSprite::create(buffer);
        pMyhImage->setAnchorPoint(CCPointZero);
        pMyhImage->setPosition(ccp(235.0f, bottomOffset + 761.0f));
        pMIDContainer->addChild(pMyhImage, 3);
        /* 右按钮 */
        CCMenuItemImage *pQie = CCMenuItemImage::create("CM_right.png",
                                                             "CM_right.png",
                                                             this,
                                                             menu_selector(ZhuLuLayer::
                                                                           optionQie));
        CCMenu* pQieMenu = CCMenu::create(pQie, NULL);
        pQie->setAnchorPoint(ccp(0, 0));
        pQie->setEnabled(false);
        pQieMenu->setAnchorPoint(CCPointZero);
        pQieMenu->setPosition(ccp(485.0f, bottomOffset + 753.0f));
        pRRContainer->addChild(pQieMenu, 2);
        if (m_diffcult == 0) {
            strcpy(buffer, "FB_showJinh.png");
        } else if (m_diffcult == 1) {
            strcpy(buffer, "FB_norx.png");
        }
        CCSprite* spFont = CCSprite::create(buffer);
        spFont->setAnchorPoint(CCPointZero);
        spFont->setPosition(ccp(545.0f, bottomOffset + 769.0f));
        pRRContainer->addChild(spFont, 2);

        /*
         * 关卡,根据关卡数量设置滚动视图大小
         * 已有选择副本
         */
        int maxShowPve = GameState::getInstance()->getNormalMaxLevId() + 1;
        int levelCount = CopyManager::getInstance()->getCopyData(GameState::getInstance()->getCopyId())->getCountOfLevel();
        /* 添加滚动视图 */
        if (false) {
            m_scrollView = CCScrollView::create(CCSizeMake(640, 960));
            m_scrollView->setContentSize(CCSizeMake(640, 960));
        } else if (GameState::getInstance()->getCopyId() == 1 || GameState::getInstance()->getCopyId() % 50 == 1) {
            if (maxShowPve < 103) {
                m_scrollView = CCScrollView::create(CCSizeMake(640, 960));
                m_scrollView->setContentSize(CCSizeMake(640, 960));
            } else if(maxShowPve >= 103 && maxShowPve < 106) {
                m_scrollView = CCScrollView::create(CCSizeMake(800, 960));
                m_scrollView->setContentSize(CCSizeMake(800, 960));
            } else if (maxShowPve >= 106 && maxShowPve < 107) {
                m_scrollView = CCScrollView::create(CCSizeMake(925, 960));
                m_scrollView->setContentSize(CCSizeMake(925, 960));
            } else if (maxShowPve >= 107 && maxShowPve < 108) {
                m_scrollView = CCScrollView::create(CCSizeMake(1045, 960));
                m_scrollView->setContentSize(CCSizeMake(1045, 960));
            } else {
                m_scrollView = CCScrollView::create(CCSizeMake(1150, 960));
                m_scrollView->setContentSize(CCSizeMake(1150, 960));
            }
        } else if (GameState::getInstance()->getCopyId() == 2 || GameState::getInstance()->getCopyId() % 50 == 2) {
            if (maxShowPve < 204) {
                m_scrollView = CCScrollView::create(CCSizeMake(640, 960));
                m_scrollView->setContentSize(CCSizeMake(640, 960));
            } else if (maxShowPve >= 204 && maxShowPve < 206) {
                m_scrollView = CCScrollView::create(CCSizeMake(700, 960));
                m_scrollView->setContentSize(CCSizeMake(700, 960));
            } else if (maxShowPve >= 206 && maxShowPve < 209) {
                m_scrollView = CCScrollView::create(CCSizeMake(965, 960));
                m_scrollView->setContentSize(CCSizeMake(965, 960));
            } else if (maxShowPve >= 209 && maxShowPve < 210) {
                m_scrollView = CCScrollView::create(CCSizeMake(1130, 960));
                m_scrollView->setContentSize(CCSizeMake(1130, 960));
            } else if (maxShowPve >= 210 && maxShowPve < 211) {
                m_scrollView = CCScrollView::create(CCSizeMake(1255, 960));
                m_scrollView->setContentSize(CCSizeMake(1255, 960));
            } else {
                m_scrollView = CCScrollView::create(CCSizeMake(1630, 960));
                m_scrollView->setContentSize(CCSizeMake(1630, 960));
            }
        } else if (GameState::getInstance()->getCopyId() == 3 || GameState::getInstance()->getCopyId() % 50 == 3) {
            if (maxShowPve < 5) {
                m_scrollView = CCScrollView::create(CCSizeMake(640, 960));
                m_scrollView->setContentSize(CCSizeMake(640, 960));
            } else if (maxShowPve >= 305 && maxShowPve < 306) {
                m_scrollView = CCScrollView::create(CCSizeMake(765, 960));
                m_scrollView->setContentSize(CCSizeMake(765, 960));
            } else if (maxShowPve >= 306 && maxShowPve < 309) {
                m_scrollView = CCScrollView::create(CCSizeMake(1030, 960));
                m_scrollView->setContentSize(CCSizeMake(1030, 960));
            } else if (maxShowPve >= 309 && maxShowPve < 311) {
                m_scrollView = CCScrollView::create(CCSizeMake(1275, 960));
                m_scrollView->setContentSize(CCSizeMake(1275, 960));
            } else if (maxShowPve >= 311 && maxShowPve < 312) {
                m_scrollView = CCScrollView::create(CCSizeMake(1445, 960));
                m_scrollView->setContentSize(CCSizeMake(1445, 960));
            } else if (maxShowPve >= 312 && maxShowPve < 314) {
                m_scrollView = CCScrollView::create(CCSizeMake(1575, 960));
                m_scrollView->setContentSize(CCSizeMake(1575, 960));
            } else {
                m_scrollView = CCScrollView::create(CCSizeMake(1650, 960));
                m_scrollView->setContentSize(CCSizeMake(1650, 960));
            }
        } else if (GameState::getInstance()->getCopyId() == 4 || GameState::getInstance()->getCopyId() % 50 == 4) {
            if (maxShowPve < 405) {
                m_scrollView = CCScrollView::create(CCSizeMake(640, 960));
                m_scrollView->setContentSize(CCSizeMake(640, 960));
            } else if (maxShowPve >= 405 && maxShowPve < 407) {
                m_scrollView = CCScrollView::create(CCSizeMake(895, 960));
                m_scrollView->setContentSize(CCSizeMake(895, 960));
            } else if (maxShowPve >= 407 && maxShowPve < 409) {
                m_scrollView = CCScrollView::create(CCSizeMake(1185, 960));
                m_scrollView->setContentSize(CCSizeMake(1185, 960));
            } else if (maxShowPve >= 409 && maxShowPve < 411) {
                m_scrollView = CCScrollView::create(CCSizeMake(1495, 960));
                m_scrollView->setContentSize(CCSizeMake(1495, 960));
            } else {
                m_scrollView = CCScrollView::create(CCSizeMake(1630, 960));
                m_scrollView->setContentSize(CCSizeMake(1630, 960));
            }
        } else if (GameState::getInstance()->getCopyId() == 5 || GameState::getInstance()->getCopyId() % 50 == 5) {
            if (maxShowPve  < 504) {
                m_scrollView = CCScrollView::create(CCSizeMake(640, 960));
                m_scrollView->setContentSize(CCSizeMake(640, 960));
            } else if (maxShowPve >= 504 && maxShowPve < 505) {
                m_scrollView = CCScrollView::create(CCSizeMake(745, 960));
                m_scrollView->setContentSize(CCSizeMake(745, 960));
            } else if (maxShowPve >= 505 && maxShowPve < 506) {
                m_scrollView = CCScrollView::create(CCSizeMake(845, 960));
                m_scrollView->setContentSize(CCSizeMake(845, 960));
            } else if (maxShowPve >= 506 && maxShowPve < 508) {
                m_scrollView = CCScrollView::create(CCSizeMake(1085, 960));
                m_scrollView->setContentSize(CCSizeMake(1085, 960));
            } else if (maxShowPve >= 508 && maxShowPve < 510) {
                m_scrollView = CCScrollView::create(CCSizeMake(1235, 960));
                m_scrollView->setContentSize(CCSizeMake(1235, 960));
            } else if (maxShowPve >= 510 && maxShowPve < 512) {
                m_scrollView = CCScrollView::create(CCSizeMake(1535, 960));
                m_scrollView->setContentSize(CCSizeMake(1535, 960));
            } else if (maxShowPve >= 512 && maxShowPve <  514) {
                m_scrollView = CCScrollView::create(CCSizeMake(1735, 960));
                m_scrollView->setContentSize(CCSizeMake(1735, 960));
            } else if (maxShowPve >= 514 && maxShowPve < 515) {
                m_scrollView = CCScrollView::create(CCSizeMake(1905, 960));
                m_scrollView->setContentSize(CCSizeMake(1905, 960));
            } else if (maxShowPve >= 515 && maxShowPve < 516) {
                m_scrollView = CCScrollView::create(CCSizeMake(1985, 960));
                m_scrollView->setContentSize(CCSizeMake(1985, 960));
            } else if (maxShowPve >= 516 && maxShowPve < 517) {
                m_scrollView = CCScrollView::create(CCSizeMake(2145, 960));
                m_scrollView->setContentSize(CCSizeMake(2145, 960));
            } else if (maxShowPve >= 517 && maxShowPve < 519) {
                m_scrollView = CCScrollView::create(CCSizeMake(2355, 960));
                m_scrollView->setContentSize(CCSizeMake(2355, 960));
            } else {
                m_scrollView = CCScrollView::create(CCSizeMake(2590, 960));
                m_scrollView->setContentSize(CCSizeMake(2590, 960));
            }
        } else if (GameState::getInstance()->getCopyId() == 6 || GameState::getInstance()->getCopyId() % 50 == 6) {
            if (maxShowPve < 605) {
                m_scrollView = CCScrollView::create(CCSizeMake(640, 960));
                m_scrollView->setContentSize(CCSizeMake(640, 960));
            } else if (maxShowPve >= 605 && maxShowPve < 607) {
                m_scrollView = CCScrollView::create(CCSizeMake(755, 960));
                m_scrollView->setContentSize(CCSizeMake(755, 960));
            } else if (maxShowPve >= 607 && maxShowPve < 609) {
                m_scrollView = CCScrollView::create(CCSizeMake(930, 960));
                m_scrollView->setContentSize(CCSizeMake(930, 960));
            } else if (maxShowPve >= 609 && maxShowPve < 610) {
                m_scrollView = CCScrollView::create(CCSizeMake(1090, 960));
                m_scrollView->setContentSize(CCSizeMake(1090, 960));
            } else if (maxShowPve >= 610 && maxShowPve < 611) {
                m_scrollView = CCScrollView::create(CCSizeMake(1160, 960));
                m_scrollView->setContentSize(CCSizeMake(1160, 960));
            } else if (maxShowPve >= 611 && maxShowPve < 613) {
                m_scrollView = CCScrollView::create(CCSizeMake(1385, 960));
                m_scrollView->setContentSize(CCSizeMake(1385, 960));
            } else if (maxShowPve >= 613 && maxShowPve < 614) {
                m_scrollView = CCScrollView::create(CCSizeMake(1515, 960));
                m_scrollView->setContentSize(CCSizeMake(1515, 960));
            } else {
                m_scrollView = CCScrollView::create(CCSizeMake(1655, 960));
                m_scrollView->setContentSize(CCSizeMake(1655, 960));
            }
        } else if (GameState::getInstance()->getCopyId() == 7 || GameState::getInstance()->getCopyId() % 50 == 7) {
            if (maxShowPve < 705) {
                m_scrollView = CCScrollView::create(CCSizeMake(640, 960));
                m_scrollView->setContentSize(CCSizeMake(640, 960));
            } else if (maxShowPve >= 705 && maxShowPve < 706) {
                m_scrollView = CCScrollView::create(CCSizeMake(740, 960));
                m_scrollView->setContentSize(CCSizeMake(740, 960));
            } else if (maxShowPve >= 706 && maxShowPve < 708) {
                m_scrollView = CCScrollView::create(CCSizeMake(885, 960));
                m_scrollView->setContentSize(CCSizeMake(885, 960));
            } else if (maxShowPve >= 708 && maxShowPve < 709) {
                m_scrollView = CCScrollView::create(CCSizeMake(1075, 960));
                m_scrollView->setContentSize(CCSizeMake(1075, 960));
            } else if (maxShowPve >= 709 && maxShowPve < 710) {
                m_scrollView = CCScrollView::create(CCSizeMake(1170, 960));
                m_scrollView->setContentSize(CCSizeMake(1170, 960));
            } else if (maxShowPve >= 710 && maxShowPve < 712) {
                m_scrollView = CCScrollView::create(CCSizeMake(1335, 960));
                m_scrollView->setContentSize(CCSizeMake(1335, 960));
            } else if (maxShowPve >= 712 && maxShowPve < 714) {
                m_scrollView = CCScrollView::create(CCSizeMake(1485, 960));
                m_scrollView->setContentSize(CCSizeMake(1485, 960));
            } else {
                m_scrollView = CCScrollView::create(CCSizeMake(1650, 960));
                m_scrollView->setContentSize(CCSizeMake(1650, 960));
            }
        } else if (GameState::getInstance()->getCopyId() == 8 || GameState::getInstance()->getCopyId() % 50 == 8) {
            if (maxShowPve < 804) {
                m_scrollView = CCScrollView::create(CCSizeMake(640, 960));
                m_scrollView->setContentSize(CCSizeMake(640, 960));
            } else if (maxShowPve >= 804 && maxShowPve < 806) {
                m_scrollView = CCScrollView::create(CCSizeMake(735, 960));
                m_scrollView->setContentSize(CCSizeMake(735, 960));
            } else if (maxShowPve >= 806 && maxShowPve < 808) {
                m_scrollView = CCScrollView::create(CCSizeMake(1015, 960));
                m_scrollView->setContentSize(CCSizeMake(1015, 960));
            } else if (maxShowPve >= 808 && maxShowPve < 811) {
                m_scrollView = CCScrollView::create(CCSizeMake(1285, 960));
                m_scrollView->setContentSize(CCSizeMake(1285, 960));
            } else if (maxShowPve >= 811 && maxShowPve < 813) {
                m_scrollView = CCScrollView::create(CCSizeMake(1495, 960));
                m_scrollView->setContentSize(CCSizeMake(1495, 960));
            } else if (maxShowPve >= 813 && maxShowPve < 815) {
                m_scrollView = CCScrollView::create(CCSizeMake(1715, 960));
                m_scrollView->setContentSize(CCSizeMake(1715, 960));
            } else if (maxShowPve >= 815 && maxShowPve < 817) {
                m_scrollView = CCScrollView::create(CCSizeMake(1865, 960));
                m_scrollView->setContentSize(CCSizeMake(1865, 960));
            } else if (maxShowPve >= 817 && maxShowPve < 819) {
                m_scrollView = CCScrollView::create(CCSizeMake(2100, 960));
                m_scrollView->setContentSize(CCSizeMake(2100, 960));
            } else if (maxShowPve >= 819 && maxShowPve < 821) {
                m_scrollView = CCScrollView::create(CCSizeMake(2280, 960));
                m_scrollView->setContentSize(CCSizeMake(2280, 960));
            } else if (maxShowPve >= 821 && maxShowPve < 823) {
                m_scrollView = CCScrollView::create(CCSizeMake(2480, 960));
                m_scrollView->setContentSize(CCSizeMake(2480, 960));
            } else {
                m_scrollView = CCScrollView::create(CCSizeMake(2600, 960));
                m_scrollView->setContentSize(CCSizeMake(2600, 960));
            }
        }
        m_scrollView->setAnchorPoint(CCPointZero);
        m_scrollView->setPosition(ccp(SCROLLVIEW_OFFSETX, bottomOffset - 75.0f));
        m_scrollView->setDirection(kCCScrollViewDirectionHorizontal);
        m_scrollView->setBounceable(false);
        m_scrollView->setDelegate(this);
        pNode->addChild(m_scrollView, 10);
        // 小地图
        CCSprite* pBgSprite = CCSprite::create("FB_litMapBg.png");
        pBgSprite->setAnchorPoint(CCPointZero);
        pBgSprite->setPosition(ccp(0.0f, 124.0f + bottomOffset));
        pNode->addChild(pBgSprite, 1);
        
        CCNode* litMapNode = CCNode::create();
        litMapNode->setAnchorPoint(ccp(0, 0));
        for (int i = 1; i < levelCount + 1; ++i) {
            /* 添加背景地图 */
            sprintf(buffer, "FB_lit%d.png",GameState::getInstance()->getCopyId() * 100 + i);
            CCSprite* spBg = CCSprite::create(buffer);
            spBg->setAnchorPoint(CCPointZero);
            litMapNode->addChild(spBg, 1, LEV_MAP_TAG + i);
            
            /* 按钮 */
            sprintf(buffer, "FB_levbtn%d.png", GameState::getInstance()->getCopyId() * 100 + i);
            CCMenuItemImage *pChosLev = CCMenuItemImage::create(buffer,
                                                                buffer,
                                                                this,
                                                                menu_selector(ZhuLuLayer::chosLev));
            pChosLev->setTag(GameState::getInstance()->getCopyId() * 100 + i + LEVEL_BTN_TAG);
            pChosLev->setAnchorPoint(ccp(0, 0));
            pChosLev->setPosition(ccp(0, 0));
            CCMenu* pMenu = CCMenu::create(pChosLev, NULL);
            pMenu->setAnchorPoint(ccp(0 ,0));
            pMenu->setPosition(ccp(0 ,0));
            litMapNode->addChild(pMenu, 100, LEVEL_BTN_TAG + 20000 + i);
            
            /* 关卡名字 */
            LevelDataItem* pItem = CopyManager::getInstance()->getCopyData(GameState::getInstance()->getCopyId())
            ->getLevelDataById(GameState::getInstance()->getCopyId() * 100 + i);
            sprintf(buffer, "FB_hj%d.png", GameState::getInstance()->getCopyId() * 100 + i);
            CCSprite* nameLabel = CCSprite::create(buffer);
            nameLabel->setAnchorPoint(ccp(0, 0));
            litMapNode->addChild(nameLabel, 101, LEV_NAME_TAG + i);
            
            /* 存放体力信息 */
            CCNode* pTwoNode = CCNode::create();
            pTwoNode->setAnchorPoint(ccp(0, 0));
            litMapNode->addChild(pTwoNode, 101, POWER_INFO_TAG + i);
            /* 进入关卡所需要的体力 */
            int entery = pItem->getEn();
            sprintf(buffer, "%d", entery);
            CCStrokeLabel* enLabel = CCStrokeLabel::create(buffer, "Arial", 27);
            enLabel->setAnchorPoint(ccp(0, 0));
            enLabel->setPosition(ccp(85, 20));
            enLabel->setColor(ccc3(127, 255, 0));
            pTwoNode->addChild(enLabel);

            /* 体力标签 */
            CCSprite* spPower = CCSprite::create("FB_power_font.png");
            spPower->setAnchorPoint(ccp(0, 0));
            spPower->setPosition(ccp(0, 20));
            pTwoNode->addChild(spPower);
            
//            CCSprite* mapLock = CCSprite::create("CM_mapLock.png");
//            mapLock->setAnchorPoint(ccp(0, 0));
//            mapLock->setPosition(ccp(60, -50));
//            mapLock->setVisible(false);
//            pTwoNode->addChild(mapLock, 200);
        
            /* 显示最大等级下一等级关卡 */
            int nextPve = 0;
            LevelDataItem* pNextItem = CopyManager::getInstance()->getCopyData(GameState::getInstance()->getNormalMaxCopyId())
            ->getLevelDataById(GameState::getInstance()->getNormalMaxLevId());
            if (pNextItem) {
                nextPve = pNextItem->getNi();/*0*/
                if (nextPve != 0) {
                    if ((GameState::getInstance()->getCopyId() * 100 + i) > nextPve) {
                        spBg->setVisible(false);
                        nameLabel->setVisible(false);
                        pTwoNode->setVisible(false);
                    } else if ((GameState::getInstance()->getCopyId() * 100 + i) == nextPve) {
                        spBg->setVisible(true);
                        spBg->setColor(HUI_COLOR);
                        nameLabel->setVisible(true);
                        nameLabel->setColor(HUI_COLOR);
                        pTwoNode->setVisible(true);
                        enLabel->setColor(HUI_COLOR);
                        spPower->setColor(HUI_COLOR);
                    } else {
                        spBg->setVisible(true);
                        nameLabel->setVisible(true);
                        pTwoNode->setVisible(true);
                    }
                }
            }
            
            /* 设置不可见
             * 按钮
             * 关卡名
             * 体力信息
             * 背景地图
             */
            
        }/*for*/
        /* 黄巾之乱抽出重复按钮 */
        if ( GameState::getInstance()->getCopyId() == 1 ) {
            CCMenuItemImage *pChosLev = CCMenuItemImage::create("FB_levbtn101s.png",
                                                                "FB_levbtn101s.png",
                                                                this,
                                                                menu_selector(ZhuLuLayer::chosLev));
            pChosLev->setTag(GameState::getInstance()->getCopyId() * 100 + 1 + LEVEL_BTN_TAG);
            pChosLev->setAnchorPoint(ccp(0, 0));
            pChosLev->setPosition(ccp(335, 377));
            CCMenu* menu = CCMenu::create(pChosLev, NULL);
            menu->setAnchorPoint(ccp(0 ,0));
            menu->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu, 1);
            CCMenuItemImage *pChosLev1 = CCMenuItemImage::create("FB_levbtn104s.png",
                                                                 "FB_levbtn104s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev1->setTag(GameState::getInstance()->getCopyId() * 100 + 4 + LEVEL_BTN_TAG);
            pChosLev1->setAnchorPoint(ccp(0, 0));
            pChosLev1->setPosition(ccp(590, 330));
            CCMenu* menu1 = CCMenu::create(pChosLev1, NULL);
            menu1->setAnchorPoint(ccp(0 ,0));
            menu1->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu1, 1);
            CCMenuItemImage *pChosLev2 = CCMenuItemImage::create("FB_levbtn107s.png",
                                                                 "FB_levbtn107s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev2->setTag(GameState::getInstance()->getCopyId() * 100 + 7 + LEVEL_BTN_TAG);
            pChosLev2->setAnchorPoint(ccp(0, 0));
            pChosLev2->setPosition(ccp(700, 245));
            CCMenu* menu2 = CCMenu::create(pChosLev2, NULL);
            menu2->setAnchorPoint(ccp(0 ,0));
            menu2->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu2, 1);
            CCMenuItemImage *pChosLev3 = CCMenuItemImage::create("FB_levbtn109s.png",
                                                                 "FB_levbtn109s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev3->setTag(GameState::getInstance()->getCopyId() * 100 + 9 + LEVEL_BTN_TAG);
            pChosLev3->setAnchorPoint(ccp(0, 0));
            pChosLev3->setPosition(ccp(975, 340));
            CCMenu* menu3 = CCMenu::create(pChosLev3, NULL);
            menu3->setAnchorPoint(ccp(0 ,0));
            menu3->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu3, 1);
        }
        /*****************************************************/
        /* 董卓讨伐战抽出重复按钮 */
        if ( GameState::getInstance()->getCopyId() == 2 ) {
            CCMenuItemImage *pChosLev = CCMenuItemImage::create("FB_levbtn201s.png",
                                                                "FB_levbtn201s.png",
                                                                this,
                                                                menu_selector(ZhuLuLayer::chosLev));
            pChosLev->setTag(GameState::getInstance()->getCopyId() * 100 + 1 + LEVEL_BTN_TAG);
            pChosLev->setAnchorPoint(ccp(0, 0));
            pChosLev->setPosition(ccp(345, 480));
            CCMenu* menu = CCMenu::create(pChosLev, NULL);
            menu->setAnchorPoint(ccp(0 ,0));
            menu->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu, 1);
            CCMenuItemImage *pChosLevs = CCMenuItemImage::create("FB_levbtn202s.png",
                                                                "FB_levbtn202s.png",
                                                                this,
                                                                menu_selector(ZhuLuLayer::chosLev));
            pChosLevs->setTag(GameState::getInstance()->getCopyId() * 100 + 2 + LEVEL_BTN_TAG);
            pChosLevs->setAnchorPoint(ccp(0, 0));
            pChosLevs->setPosition(ccp(260, 295));
            CCMenu* menus = CCMenu::create(pChosLevs, NULL);
            menus->setAnchorPoint(ccp(0 ,0));
            menus->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus, 1);
            CCMenuItemImage *pChosLevs1 = CCMenuItemImage::create("FB_levbtn202s1.png",
                                                                "FB_levbtn202s1.png",
                                                                this,
                                                                menu_selector(ZhuLuLayer::chosLev));
            pChosLevs1->setTag(GameState::getInstance()->getCopyId() * 100 + 2 + LEVEL_BTN_TAG);
            pChosLevs1->setAnchorPoint(ccp(0, 0));
            pChosLevs1->setPosition(ccp(300, 300));
            CCMenu* menus1 = CCMenu::create(pChosLevs1, NULL);
            menus1->setAnchorPoint(ccp(0 ,0));
            menus1->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus1, 1);
            CCMenuItemImage *pChosLev1 = CCMenuItemImage::create("FB_levbtn203s.png",
                                                                 "FB_levbtn203s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev1->setTag(GameState::getInstance()->getCopyId() * 100 + 3 + LEVEL_BTN_TAG);
            pChosLev1->setAnchorPoint(ccp(0, 0));
            pChosLev1->setPosition(ccp(500, 475));
            CCMenu* menu1 = CCMenu::create(pChosLev1, NULL);
            menu1->setAnchorPoint(ccp(0 ,0));
            menu1->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu1, 1);
            CCMenuItemImage *pChosLev2 = CCMenuItemImage::create("FB_levbtn204s.png",
                                                                 "FB_levbtn204s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev2->setTag(GameState::getInstance()->getCopyId() * 100 + 4 + LEVEL_BTN_TAG);
            pChosLev2->setAnchorPoint(ccp(0, 0));
            pChosLev2->setPosition(ccp(490, 295));
            CCMenu* menu2 = CCMenu::create(pChosLev2, NULL);
            menu2->setAnchorPoint(ccp(0 ,0));
            menu2->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu2, 1);
            CCMenuItemImage *pChosLev3 = CCMenuItemImage::create("FB_levbtn205s.png",
                                                                 "FB_levbtn205s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev3->setTag(GameState::getInstance()->getCopyId() * 100 + 5 + LEVEL_BTN_TAG);
            pChosLev3->setAnchorPoint(ccp(0, 0));
            pChosLev3->setPosition(ccp(360, 290));
            CCMenu* menu3 = CCMenu::create(pChosLev3, NULL);
            menu3->setAnchorPoint(ccp(0 ,0));
            menu3->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu3, 1);
            CCMenuItemImage *pChosLevs2 = CCMenuItemImage::create("FB_levbtn207s.png",
                                                                 "FB_levbtn207s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLevs2->setTag(GameState::getInstance()->getCopyId() * 100 + 7 + LEVEL_BTN_TAG);
            pChosLevs2->setAnchorPoint(ccp(0, 0));
            pChosLevs2->setPosition(ccp(820, 425));
            CCMenu* menus2 = CCMenu::create(pChosLevs2, NULL);
            menus2->setAnchorPoint(ccp(0 ,0));
            menus2->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus2, 1);
            CCMenuItemImage *pChosLevs3 = CCMenuItemImage::create("FB_levbtn208s.png",
                                                                  "FB_levbtn208s.png",
                                                                  this,
                                                                  menu_selector(ZhuLuLayer::chosLev));
            pChosLevs3->setTag(GameState::getInstance()->getCopyId() * 100 + 8 + LEVEL_BTN_TAG);
            pChosLevs3->setAnchorPoint(ccp(0, 0));
            pChosLevs3->setPosition(ccp(615, 295));
            CCMenu* menus3 = CCMenu::create(pChosLevs3, NULL);
            menus3->setAnchorPoint(ccp(0 ,0));
            menus3->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus3, 1);
            CCMenuItemImage *pChosLevs4 = CCMenuItemImage::create("FB_levbtn208s1.png",
                                                                  "FB_levbtn208s1.png",
                                                                  this,
                                                                  menu_selector(ZhuLuLayer::chosLev));
            pChosLevs4->setTag(GameState::getInstance()->getCopyId() * 100 + 8 + LEVEL_BTN_TAG);
            pChosLevs4->setAnchorPoint(ccp(0, 0));
            pChosLevs4->setPosition(ccp(800, 250));
            CCMenu* menus4 = CCMenu::create(pChosLevs4, NULL);
            menus4->setAnchorPoint(ccp(0 ,0));
            menus4->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus4, 1);
            CCMenuItemImage *pChosLevs5 = CCMenuItemImage::create("FB_levbtn209s.png",
                                                                  "FB_levbtn209s.png",
                                                                  this,
                                                                  menu_selector(ZhuLuLayer::chosLev));
            pChosLevs5->setTag(GameState::getInstance()->getCopyId() * 100 + 9 + LEVEL_BTN_TAG);
            pChosLevs5->setAnchorPoint(ccp(0, 0));
            pChosLevs5->setPosition(ccp(910, 595));
            CCMenu* menus5 = CCMenu::create(pChosLevs5, NULL);
            menus5->setAnchorPoint(ccp(0 ,0));
            menus5->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus5, 1);
            CCMenuItemImage *pChosLev4 = CCMenuItemImage::create("FB_levbtn210s.png",
                                                                 "FB_levbtn210s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev4->setTag(GameState::getInstance()->getCopyId() * 100 + 10 + LEVEL_BTN_TAG);
            pChosLev4->setAnchorPoint(ccp(0, 0));
            pChosLev4->setPosition(ccp(880, 250));
            CCMenu* menu4 = CCMenu::create(pChosLev4, NULL);
            menu4->setAnchorPoint(ccp(0 ,0));
            menu4->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu4, 1);
            CCMenuItemImage *pChosLevs6 = CCMenuItemImage::create("FB_levbtn210s1.png",
                                                                 "FB_levbtn210s1.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLevs6->setTag(GameState::getInstance()->getCopyId() * 100 + 10 + LEVEL_BTN_TAG);
            pChosLevs6->setAnchorPoint(ccp(0, 0));
            pChosLevs6->setPosition(ccp(835, 350));
            CCMenu* menus6 = CCMenu::create(pChosLevs6, NULL);
            menus6->setAnchorPoint(ccp(0 ,0));
            menus6->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus6, 1);
            CCMenuItemImage *pChosLevs7 = CCMenuItemImage::create("FB_levbtn211s.png",
                                                                  "FB_levbtn211s.png",
                                                                  this,
                                                                  menu_selector(ZhuLuLayer::chosLev));
            pChosLevs7->setTag(GameState::getInstance()->getCopyId() * 100 + 11 + LEVEL_BTN_TAG);
            pChosLevs7->setAnchorPoint(ccp(0, 0));
            pChosLevs7->setPosition(ccp(1250, 580));
            CCMenu* menus7 = CCMenu::create(pChosLevs7, NULL);
            menus7->setAnchorPoint(ccp(0 ,0));
            menus7->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus7, 1);
            CCMenuItemImage *pChosLev5 = CCMenuItemImage::create("FB_levbtn212s.png",
                                                                 "FB_levbtn212s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev5->setTag(GameState::getInstance()->getCopyId() * 100 + 12 + LEVEL_BTN_TAG);
            pChosLev5->setAnchorPoint(ccp(0, 0));
            pChosLev5->setPosition(ccp(1085, 495));
            CCMenu* menu5 = CCMenu::create(pChosLev5, NULL);
            menu5->setAnchorPoint(ccp(0 ,0));
            menu5->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu5, 1);
            CCMenuItemImage *pChosLevs8 = CCMenuItemImage::create("FB_levbtn212s1.png",
                                                                 "FB_levbtn212s1.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLevs8->setTag(GameState::getInstance()->getCopyId() * 100 + 12 + LEVEL_BTN_TAG);
            pChosLevs8->setAnchorPoint(ccp(0, 0));
            pChosLevs8->setPosition(ccp(1205, 435));
            CCMenu* menus8 = CCMenu::create(pChosLevs8, NULL);
            menus8->setAnchorPoint(ccp(0 ,0));
            menus8->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus8, 1);
            CCMenuItemImage *pChosLev6 = CCMenuItemImage::create("FB_levbtn213s.png",
                                                                 "FB_levbtn213s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev6->setTag(GameState::getInstance()->getCopyId() * 100 + 13 + LEVEL_BTN_TAG);
            pChosLev6->setAnchorPoint(ccp(0, 0));
            pChosLev6->setPosition(ccp(1175, 345));
            CCMenu* menu6 = CCMenu::create(pChosLev6, NULL);
            menu6->setAnchorPoint(ccp(0 ,0));
            menu6->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu6, 1);
            CCMenuItemImage *pChosLev7 = CCMenuItemImage::create("FB_levbtn213s1.png",
                                                                 "FB_levbtn213s1.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev7->setTag(GameState::getInstance()->getCopyId() * 100 + 13 + LEVEL_BTN_TAG);
            pChosLev7->setAnchorPoint(ccp(0, 0));
            pChosLev7->setPosition(ccp(1400, 380));
            CCMenu* menu7 = CCMenu::create(pChosLev7, NULL);
            menu7->setAnchorPoint(ccp(0 ,0));
            menu7->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu7, 1);
            CCMenuItemImage *pChosLevs9 = CCMenuItemImage::create("FB_levbtn213s2.png",
                                                                 "FB_levbtn213s2.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLevs9->setTag(GameState::getInstance()->getCopyId() * 100 + 13 + LEVEL_BTN_TAG);
            pChosLevs9->setAnchorPoint(ccp(0, 0));
            pChosLevs9->setPosition(ccp(1235, 390));
            CCMenu* menus9 = CCMenu::create(pChosLevs9, NULL);
            menus9->setAnchorPoint(ccp(0 ,0));
            menus9->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus9, 1);
            CCMenuItemImage *pChosLev8 = CCMenuItemImage::create("FB_levbtn214s.png",
                                                                 "FB_levbtn214s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev8->setTag(GameState::getInstance()->getCopyId() * 100 + 14 + LEVEL_BTN_TAG);
            pChosLev8->setAnchorPoint(ccp(0, 0));
            pChosLev8->setPosition(ccp(1300, 515));
            CCMenu* menu8 = CCMenu::create(pChosLev8, NULL);
            menu8->setAnchorPoint(ccp(0 ,0));
            menu8->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu8, 1);
            CCMenuItemImage *pChosLev9 = CCMenuItemImage::create("FB_levbtn215s.png",
                                                                 "FB_levbtn215s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev9->setTag(GameState::getInstance()->getCopyId() * 100 + 15 + LEVEL_BTN_TAG);
            pChosLev9->setAnchorPoint(ccp(0, 0));
            pChosLev9->setPosition(ccp(1335, 250));
            CCMenu* menu9 = CCMenu::create(pChosLev9, NULL);
            menu9->setAnchorPoint(ccp(0 ,0));
            menu9->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu9, 1);
        }
        /*****************************************************/
        /* 界桥之战抽出重复按钮 */
        if ( GameState::getInstance()->getCopyId() == 3 ) {
            CCMenuItemImage *pChosLevs = CCMenuItemImage::create("FB_levbtn302s.png",
                                                                "FB_levbtn302s.png",
                                                                this,
                                                                menu_selector(ZhuLuLayer::chosLev));
            pChosLevs->setTag(GameState::getInstance()->getCopyId() * 100 + 2 + LEVEL_BTN_TAG);
            pChosLevs->setAnchorPoint(ccp(0, 0));
            pChosLevs->setPosition(ccp(260, 295));
            CCMenu* menus = CCMenu::create(pChosLevs, NULL);
            menus->setAnchorPoint(ccp(0 ,0));
            menus->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus, 1);
            CCMenuItemImage *pChosLevs1 = CCMenuItemImage::create("FB_levbtn303s.png",
                                                                 "FB_levbtn303s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLevs1->setTag(GameState::getInstance()->getCopyId() * 100 + 3 + LEVEL_BTN_TAG);
            pChosLevs1->setAnchorPoint(ccp(0, 0));
            pChosLevs1->setPosition(ccp(450, 595));
            CCMenu* menus1 = CCMenu::create(pChosLevs1, NULL);
            menus1->setAnchorPoint(ccp(0 ,0));
            menus1->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus1, 1);
            CCMenuItemImage *pChosLevs2 = CCMenuItemImage::create("FB_levbtn303s1.png",
                                                                 "FB_levbtn303s1.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLevs2->setTag(GameState::getInstance()->getCopyId() * 100 + 3 + LEVEL_BTN_TAG);
            pChosLevs2->setAnchorPoint(ccp(0, 0));
            pChosLevs2->setPosition(ccp(295, 440));
            CCMenu* menus2 = CCMenu::create(pChosLevs2, NULL);
            menus2->setAnchorPoint(ccp(0 ,0));
            menus2->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus2, 1);
            CCMenuItemImage *pChosLevs3 = CCMenuItemImage::create("FB_levbtn304s.png",
                                                                  "FB_levbtn304s.png",
                                                                  this,
                                                                  menu_selector(ZhuLuLayer::chosLev));
            pChosLevs3->setTag(GameState::getInstance()->getCopyId() * 100 + 4 + LEVEL_BTN_TAG);
            pChosLevs3->setAnchorPoint(ccp(0, 0));
            pChosLevs3->setPosition(ccp(540, 250));
            CCMenu* menus3 = CCMenu::create(pChosLevs3, NULL);
            menus3->setAnchorPoint(ccp(0 ,0));
            menus3->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus3, 1);
            
            CCMenuItemImage *pChosLev = CCMenuItemImage::create("FB_levbtn305s.png",
                                                                "FB_levbtn305s.png",
                                                                this,
                                                                menu_selector(ZhuLuLayer::chosLev));
            pChosLev->setTag(GameState::getInstance()->getCopyId() * 100 + 5 + LEVEL_BTN_TAG);
            pChosLev->setAnchorPoint(ccp(0, 0));
            pChosLev->setPosition(ccp(470, 510));
            CCMenu* menu = CCMenu::create(pChosLev, NULL);
            menu->setAnchorPoint(ccp(0 ,0));
            menu->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu, 1);
            CCMenuItemImage *pChosLevs4 = CCMenuItemImage::create("FB_levbtn305s1.png",
                                                                "FB_levbtn305s1.png",
                                                                this,
                                                                menu_selector(ZhuLuLayer::chosLev));
            pChosLevs4->setTag(GameState::getInstance()->getCopyId() * 100 + 5 + LEVEL_BTN_TAG);
            pChosLevs4->setAnchorPoint(ccp(0, 0));
            pChosLevs4->setPosition(ccp(545, 470));
            CCMenu* menus4 = CCMenu::create(pChosLevs4, NULL);
            menus4->setAnchorPoint(ccp(0 ,0));
            menus4->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus4, 1);
            CCMenuItemImage *pChosLev1 = CCMenuItemImage::create("FB_levbtn306s.png",
                                                                 "FB_levbtn306s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev1->setTag(GameState::getInstance()->getCopyId() * 100 + 6 + LEVEL_BTN_TAG);
            pChosLev1->setAnchorPoint(ccp(0, 0));
            pChosLev1->setPosition(ccp(615, 300));
            CCMenu* menu1 = CCMenu::create(pChosLev1, NULL);
            menu1->setAnchorPoint(ccp(0 ,0));
            menu1->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu1, 1);
            CCMenuItemImage *pChosLevs5 = CCMenuItemImage::create("FB_levbtn306s1.png",
                                                                 "FB_levbtn306s1.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLevs5->setTag(GameState::getInstance()->getCopyId() * 100 + 6 + LEVEL_BTN_TAG);
            pChosLevs5->setAnchorPoint(ccp(0, 0));
            pChosLevs5->setPosition(ccp(570, 375));
            CCMenu* menus5 = CCMenu::create(pChosLevs5, NULL);
            menus5->setAnchorPoint(ccp(0 ,0));
            menus5->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus5, 1);
            CCMenuItemImage *pChosLevs6 = CCMenuItemImage::create("FB_levbtn307s.png",
                                                                 "FB_levbtn307s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLevs6->setTag(GameState::getInstance()->getCopyId() * 100 + 7 + LEVEL_BTN_TAG);
            pChosLevs6->setAnchorPoint(ccp(0, 0));
            pChosLevs6->setPosition(ccp(905, 655));
            CCMenu* menus6 = CCMenu::create(pChosLevs6, NULL);
            menus6->setAnchorPoint(ccp(0 ,0));
            menus6->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus6, 1);
            CCMenuItemImage *pChosLev2 = CCMenuItemImage::create("FB_levbtn309s.png",
                                                                 "FB_levbtn309s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev2->setTag(GameState::getInstance()->getCopyId() * 100 + 9 + LEVEL_BTN_TAG);
            pChosLev2->setAnchorPoint(ccp(0, 0));
            pChosLev2->setPosition(ccp(965, 640));
            CCMenu* menu2 = CCMenu::create(pChosLev2, NULL);
            menu2->setAnchorPoint(ccp(0 ,0));
            menu2->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu2, 1);
            CCMenuItemImage *pChosLev4 = CCMenuItemImage::create("FB_levbtn311s.png",
                                                                 "FB_levbtn311s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev4->setTag(GameState::getInstance()->getCopyId() * 100 + 11 + LEVEL_BTN_TAG);
            pChosLev4->setAnchorPoint(ccp(0, 0));
            pChosLev4->setPosition(ccp(1140, 510));
            CCMenu* menu4 = CCMenu::create(pChosLev4, NULL);
            menu4->setAnchorPoint(ccp(0 ,0));
            menu4->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu4, 1);
            CCMenuItemImage *pChosLevs7 = CCMenuItemImage::create("FB_levbtn312s.png",
                                                                 "FB_levbtn312s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLevs7->setTag(GameState::getInstance()->getCopyId() * 100 + 12 + LEVEL_BTN_TAG);
            pChosLevs7->setAnchorPoint(ccp(0, 0));
            pChosLevs7->setPosition(ccp(1385, 255));
            CCMenu* menus7 = CCMenu::create(pChosLevs7, NULL);
            menus7->setAnchorPoint(ccp(0 ,0));
            menus7->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus7, 1);
            CCMenuItemImage *pChosLevs8 = CCMenuItemImage::create("FB_levbtn313s.png",
                                                                  "FB_levbtn313s.png",
                                                                  this,
                                                                  menu_selector(ZhuLuLayer::chosLev));
            pChosLevs8->setTag(GameState::getInstance()->getCopyId() * 100 + 13 + LEVEL_BTN_TAG);
            pChosLevs8->setAnchorPoint(ccp(0, 0));
            pChosLevs8->setPosition(ccp(1435, 475));
            CCMenu* menus8 = CCMenu::create(pChosLevs8, NULL);
            menus8->setAnchorPoint(ccp(0 ,0));
            menus8->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus8, 1);
            CCMenuItemImage *pChosLevs9 = CCMenuItemImage::create("FB_levbtn314s.png",
                                                                  "FB_levbtn314s.png",
                                                                  this,
                                                                  menu_selector(ZhuLuLayer::chosLev));
            pChosLevs9->setTag(GameState::getInstance()->getCopyId() * 100 + 14 + LEVEL_BTN_TAG);
            pChosLevs9->setAnchorPoint(ccp(0, 0));
            pChosLevs9->setPosition(ccp(1420, 320));
            CCMenu* menus9 = CCMenu::create(pChosLevs9, NULL);
            menus9->setAnchorPoint(ccp(0 ,0));
            menus9->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus9, 1);
            CCMenuItemImage *pChosLev5 = CCMenuItemImage::create("FB_levbtn315s.png",
                                                                 "FB_levbtn315s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev5->setTag(GameState::getInstance()->getCopyId() * 100 + 15 + LEVEL_BTN_TAG);
            pChosLev5->setAnchorPoint(ccp(0, 0));
            pChosLev5->setPosition(ccp(1455, 595));
            CCMenu* menu5 = CCMenu::create(pChosLev5, NULL);
            menu5->setAnchorPoint(ccp(0 ,0));
            menu5->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu5, 1);
        }
        /******************************************************/
        /* 襄阳之战抽出重复按钮 */
        if ( GameState::getInstance()->getCopyId() == 4 ) {
            CCMenuItemImage *pChosLev = CCMenuItemImage::create("FB_levbtn402s.png",
                                                                "FB_levbtn402s.png",
                                                                this,
                                                                menu_selector(ZhuLuLayer::chosLev));
            pChosLev->setTag(GameState::getInstance()->getCopyId() * 100 + 2 + LEVEL_BTN_TAG);
            pChosLev->setAnchorPoint(ccp(0, 0));
            pChosLev->setPosition(ccp(295, 250));
            CCMenu* menu = CCMenu::create(pChosLev, NULL);
            menu->setAnchorPoint(ccp(0 ,0));
            menu->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu, 1);
            CCMenuItemImage *pChosLevs = CCMenuItemImage::create("FB_levbtn403s.png",
                                                                "FB_levbtn403s.png",
                                                                this,
                                                                menu_selector(ZhuLuLayer::chosLev));
            pChosLevs->setTag(GameState::getInstance()->getCopyId() * 100 + 3 + LEVEL_BTN_TAG);
            pChosLevs->setAnchorPoint(ccp(0, 0));
            pChosLevs->setPosition(ccp(470, 545));
            CCMenu* menus = CCMenu::create(pChosLevs, NULL);
            menus->setAnchorPoint(ccp(0 ,0));
            menus->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus, 1);
            CCMenuItemImage *pChosLevs1 = CCMenuItemImage::create("FB_levbtn404s.png",
                                                                 "FB_levbtn404s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLevs1->setTag(GameState::getInstance()->getCopyId() * 100 + 4 + LEVEL_BTN_TAG);
            pChosLevs1->setAnchorPoint(ccp(0, 0));
            pChosLevs1->setPosition(ccp(395, 260));
            CCMenu* menus1 = CCMenu::create(pChosLevs1, NULL);
            menus1->setAnchorPoint(ccp(0 ,0));
            menus1->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus1, 1);
            CCMenuItemImage *pChosLevs2 = CCMenuItemImage::create("FB_levbtn404s1.png",
                                                                 "FB_levbtn404s1.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLevs2->setTag(GameState::getInstance()->getCopyId() * 100 + 4 + LEVEL_BTN_TAG);
            pChosLevs2->setAnchorPoint(ccp(0, 0));
            pChosLevs2->setPosition(ccp(405, 465));
            CCMenu* menus2 = CCMenu::create(pChosLevs2, NULL);
            menus2->setAnchorPoint(ccp(0 ,0));
            menus2->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus2, 1);
            CCMenuItemImage *pChosLev7 = CCMenuItemImage::create("FB_levbtn405s.png",
                                                                 "FB_levbtn405s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev7->setTag(GameState::getInstance()->getCopyId() * 100 + 5 + LEVEL_BTN_TAG);
            pChosLev7->setAnchorPoint(ccp(0, 0));
            pChosLev7->setPosition(ccp(595, 400));
            CCMenu* menu7 = CCMenu::create(pChosLev7, NULL);
            menu7->setAnchorPoint(ccp(0 ,0));
            menu7->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu7, 1);
            CCMenuItemImage *pChosLev1 = CCMenuItemImage::create("FB_levbtn406s.png",
                                                                 "FB_levbtn406s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev1->setTag(GameState::getInstance()->getCopyId() * 100 + 6 + LEVEL_BTN_TAG);
            pChosLev1->setAnchorPoint(ccp(0, 0));
            pChosLev1->setPosition(ccp(680, 340));
            CCMenu* menu1 = CCMenu::create(pChosLev1, NULL);
            menu1->setAnchorPoint(ccp(0 ,0));
            menu1->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu1, 1);
            CCMenuItemImage *pChosLevs3 = CCMenuItemImage::create("FB_levbtn407s.png",
                                                                 "FB_levbtn407s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLevs3->setTag(GameState::getInstance()->getCopyId() * 100 + 7 + LEVEL_BTN_TAG);
            pChosLevs3->setAnchorPoint(ccp(0, 0));
            pChosLevs3->setPosition(ccp(790, 445));
            CCMenu* menus3 = CCMenu::create(pChosLevs3, NULL);
            menus3->setAnchorPoint(ccp(0 ,0));
            menus3->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus3, 1);
            CCMenuItemImage *pChosLevs4 = CCMenuItemImage::create("FB_levbtn407s1.png",
                                                                 "FB_levbtn407s1.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLevs4->setTag(GameState::getInstance()->getCopyId() * 100 + 7 + LEVEL_BTN_TAG);
            pChosLevs4->setAnchorPoint(ccp(0, 0));
            pChosLevs4->setPosition(ccp(1000, 395));
            CCMenu* menus4 = CCMenu::create(pChosLevs4, NULL);
            menus4->setAnchorPoint(ccp(0 ,0));
            menus4->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus4, 1);
            CCMenuItemImage *pChosLev2 = CCMenuItemImage::create("FB_levbtn408s.png",
                                                                 "FB_levbtn408s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev2->setTag(GameState::getInstance()->getCopyId() * 100 + 8 + LEVEL_BTN_TAG);
            pChosLev2->setAnchorPoint(ccp(0, 0));
            pChosLev2->setPosition(ccp(990, 255));
            CCMenu* menu2 = CCMenu::create(pChosLev2, NULL);
            menu2->setAnchorPoint(ccp(0 ,0));
            menu2->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu2, 1);
            CCMenuItemImage *pChosLevs5 = CCMenuItemImage::create("FB_levbtn409s1.png",
                                                                 "FB_levbtn409s1.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLevs5->setTag(GameState::getInstance()->getCopyId() * 100 + 9 + LEVEL_BTN_TAG);
            pChosLevs5->setAnchorPoint(ccp(0, 0));
            pChosLevs5->setPosition(ccp(1410, 640));
            CCMenu* menus5 = CCMenu::create(pChosLevs5, NULL);
            menus5->setAnchorPoint(ccp(0 ,0));
            menus5->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus5, 1);
            CCMenuItemImage *pChosLev3 = CCMenuItemImage::create("FB_levbtn409s.png",
                                                                 "FB_levbtn409s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev3->setTag(GameState::getInstance()->getCopyId() * 100 + 9 + LEVEL_BTN_TAG);
            pChosLev3->setAnchorPoint(ccp(0, 0));
            pChosLev3->setPosition(ccp(1095, 480));
            CCMenu* menu3 = CCMenu::create(pChosLev3, NULL);
            menu3->setAnchorPoint(ccp(0 ,0));
            menu3->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu3, 1);
            
            CCMenuItemImage *pChosLev4 = CCMenuItemImage::create("FB_levbtn411s.png",
                                                                 "FB_levbtn411s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev4->setTag(GameState::getInstance()->getCopyId() * 100 + 11 + LEVEL_BTN_TAG);
            pChosLev4->setAnchorPoint(ccp(0, 0));
            pChosLev4->setPosition(ccp(1470, 630));
            CCMenu* menu4 = CCMenu::create(pChosLev4, NULL);
            menu4->setAnchorPoint(ccp(0 ,0));
            menu4->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu4, 1);
            CCMenuItemImage *pChosLev5 = CCMenuItemImage::create("FB_levbtn411s1.png",
                                                                 "FB_levbtn411s1.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev5->setTag(GameState::getInstance()->getCopyId() * 100 + 11 + LEVEL_BTN_TAG);
            pChosLev5->setAnchorPoint(ccp(0, 0));
            pChosLev5->setPosition(ccp(1340, 400));
            CCMenu* menu5 = CCMenu::create(pChosLev5, NULL);
            menu5->setAnchorPoint(ccp(0 ,0));
            menu5->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu5, 1);
        }
        /****************************************************/
        /* 平江东抽出重复按钮 */
        if ( GameState::getInstance()->getCopyId() == 5 ) {
            CCMenuItemImage *pChosLev = CCMenuItemImage::create("FB_levbtn501s.png",
                                                                "FB_levbtn501s.png",
                                                                this,
                                                                menu_selector(ZhuLuLayer::chosLev));
            pChosLev->setTag(GameState::getInstance()->getCopyId() * 100 + 1 + LEVEL_BTN_TAG);
            pChosLev->setAnchorPoint(ccp(0, 0));
            pChosLev->setPosition(ccp(260, 395));
            CCMenu* menu = CCMenu::create(pChosLev, NULL);
            menu->setAnchorPoint(ccp(0 ,0));
            menu->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu, 1);
            CCMenuItemImage *pChosLev1 = CCMenuItemImage::create("FB_levbtn502s.png",
                                                                 "FB_levbtn502s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev1->setTag(GameState::getInstance()->getCopyId() * 100 + 2 + LEVEL_BTN_TAG);
            pChosLev1->setAnchorPoint(ccp(0, 0));
            pChosLev1->setPosition(ccp(250, 250));
            CCMenu* menu1 = CCMenu::create(pChosLev1, NULL);
            menu1->setAnchorPoint(ccp(0 ,0));
            menu1->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu1, 1);
            CCMenuItemImage *pChosLev2 = CCMenuItemImage::create("FB_levbtn503s.png",
                                                                 "FB_levbtn503s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev2->setTag(GameState::getInstance()->getCopyId() * 100 + 3 + LEVEL_BTN_TAG);
            pChosLev2->setAnchorPoint(ccp(0, 0));
            pChosLev2->setPosition(ccp(405, 440));
            CCMenu* menu2 = CCMenu::create(pChosLev2, NULL);
            menu2->setAnchorPoint(ccp(0 ,0));
            menu2->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu2, 1);
            CCMenuItemImage *pChosLev3 = CCMenuItemImage::create("FB_levbtn504s.png",
                                                                 "FB_levbtn504s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev3->setTag(GameState::getInstance()->getCopyId() * 100 + 4 + LEVEL_BTN_TAG);
            pChosLev3->setAnchorPoint(ccp(0, 0));
            pChosLev3->setPosition(ccp(585, 345));
            CCMenu* menu3 = CCMenu::create(pChosLev3, NULL);
            menu3->setAnchorPoint(ccp(0 ,0));
            menu3->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu3, 1);
            CCMenuItemImage *pChosLevs = CCMenuItemImage::create("FB_levbtn505s.png",
                                                                 "FB_levbtn505s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLevs->setTag(GameState::getInstance()->getCopyId() * 100 + 5 + LEVEL_BTN_TAG);
            pChosLevs->setAnchorPoint(ccp(0, 0));
            pChosLevs->setPosition(ccp(655, 415));
            CCMenu* menus = CCMenu::create(pChosLevs, NULL);
            menus->setAnchorPoint(ccp(0 ,0));
            menus->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus, 1);
            CCMenuItemImage *pChosLev4 = CCMenuItemImage::create("FB_levbtn506s.png",
                                                                 "FB_levbtn506s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev4->setTag(GameState::getInstance()->getCopyId() * 100 + 6 + LEVEL_BTN_TAG);
            pChosLev4->setAnchorPoint(ccp(0, 0));
            pChosLev4->setPosition(ccp(615, 250));
            CCMenu* menu4 = CCMenu::create(pChosLev4, NULL);
            menu4->setAnchorPoint(ccp(0 ,0));
            menu4->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu4, 1);
            CCMenuItemImage *pChosLev5 = CCMenuItemImage::create("FB_levbtn506s1.png",
                                                                 "FB_levbtn506s1.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev5->setTag(GameState::getInstance()->getCopyId() * 100 + 6 + LEVEL_BTN_TAG);
            pChosLev5->setAnchorPoint(ccp(0, 0));
            pChosLev5->setPosition(ccp(705, 250));
            CCMenu* menu5 = CCMenu::create(pChosLev5, NULL);
            menu5->setAnchorPoint(ccp(0 ,0));
            menu5->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu5, 1);
            CCMenuItemImage *pChosLevs1 = CCMenuItemImage::create("FB_levbtn507s.png",
                                                                 "FB_levbtn507s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLevs1->setTag(GameState::getInstance()->getCopyId() * 100 + 7 + LEVEL_BTN_TAG);
            pChosLevs1->setAnchorPoint(ccp(0, 0));
            pChosLevs1->setPosition(ccp(790, 525));
            CCMenu* menus1 = CCMenu::create(pChosLevs1, NULL);
            menus1->setAnchorPoint(ccp(0 ,0));
            menus1->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus1, 1);
            CCMenuItemImage *pChosLevs2 = CCMenuItemImage::create("FB_levbtn508s.png",
                                                                  "FB_levbtn508s.png",
                                                                  this,
                                                                  menu_selector(ZhuLuLayer::chosLev));
            pChosLevs2->setTag(GameState::getInstance()->getCopyId() * 100 + 8 + LEVEL_BTN_TAG);
            pChosLevs2->setAnchorPoint(ccp(0, 0));
            pChosLevs2->setPosition(ccp(965, 475));
            CCMenu* menus2 = CCMenu::create(pChosLevs2, NULL);
            menus2->setAnchorPoint(ccp(0 ,0));
            menus2->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus2, 1);
            CCMenuItemImage *pChosLev6 = CCMenuItemImage::create("FB_levbtn510s.png",
                                                                 "FB_levbtn510s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev6->setTag(GameState::getInstance()->getCopyId() * 100 + 10 + LEVEL_BTN_TAG);
            pChosLev6->setAnchorPoint(ccp(0, 0));
            pChosLev6->setPosition(ccp(1145, 425));
            CCMenu* menu6 = CCMenu::create(pChosLev6, NULL);
            menu6->setAnchorPoint(ccp(0 ,0));
            menu6->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu6, 1);
            CCMenuItemImage *pChosLev8 = CCMenuItemImage::create("FB_levbtn511s1.png",
                                                                 "FB_levbtn511s1.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev8->setTag(GameState::getInstance()->getCopyId() * 100 + 11 + LEVEL_BTN_TAG);
            pChosLev8->setAnchorPoint(ccp(0, 0));
            pChosLev8->setPosition(ccp(1160, 265));
            CCMenu* menu8 = CCMenu::create(pChosLev8, NULL);
            menu8->setAnchorPoint(ccp(0 ,0));
            menu8->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu8, 1);
            CCMenuItemImage *pChosLev9 = CCMenuItemImage::create("FB_levbtn513s.png",
                                                                 "FB_levbtn513s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev9->setTag(GameState::getInstance()->getCopyId() * 100 + 13 + LEVEL_BTN_TAG);
            pChosLev9->setAnchorPoint(ccp(0, 0));
            pChosLev9->setPosition(ccp(1435, 390));
            CCMenu* menu9 = CCMenu::create(pChosLev9, NULL);
            menu9->setAnchorPoint(ccp(0 ,0));
            menu9->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu9, 1);
            CCMenuItemImage *pChosLevs4 = CCMenuItemImage::create("FB_levbtn513s1.png",
                                                                 "FB_levbtn513s1.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLevs4->setTag(GameState::getInstance()->getCopyId() * 100 + 13 + LEVEL_BTN_TAG);
            pChosLevs4->setAnchorPoint(ccp(0, 0));
            pChosLevs4->setPosition(ccp(1440, 255));
            CCMenu* menus4 = CCMenu::create(pChosLevs4, NULL);
            menus4->setAnchorPoint(ccp(0 ,0));
            menus4->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus4, 1);
            CCMenuItemImage *pChosLev10 = CCMenuItemImage::create("FB_levbtn512s.png",
                                                                 "FB_levbtn512s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev10->setTag(GameState::getInstance()->getCopyId() * 100 + 12 + LEVEL_BTN_TAG);
            pChosLev10->setAnchorPoint(ccp(0, 0));
            pChosLev10->setPosition(ccp(1525, 405));
            CCMenu* menu10 = CCMenu::create(pChosLev10, NULL);
            menu10->setAnchorPoint(ccp(0 ,0));
            menu10->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu10, 1);
            CCMenuItemImage *pChosLevs3 = CCMenuItemImage::create("FB_levbtn512s1.png",
                                                                  "FB_levbtn512s1.png",
                                                                  this,
                                                                  menu_selector(ZhuLuLayer::chosLev));
            pChosLevs3->setTag(GameState::getInstance()->getCopyId() * 100 + 12 + LEVEL_BTN_TAG);
            pChosLevs3->setAnchorPoint(ccp(0, 0));
            pChosLevs3->setPosition(ccp(1490, 465));
            CCMenu* menus3 = CCMenu::create(pChosLevs3, NULL);
            menus3->setAnchorPoint(ccp(0 ,0));
            menus3->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus3, 1);
            CCMenuItemImage *pChosLev11 = CCMenuItemImage::create("FB_levbtn514s.png",
                                                                  "FB_levbtn514s.png",
                                                                  this,
                                                                  menu_selector(ZhuLuLayer::chosLev));
            pChosLev11->setTag(GameState::getInstance()->getCopyId() * 100 + 14 + LEVEL_BTN_TAG);
            pChosLev11->setAnchorPoint(ccp(0, 0));
            pChosLev11->setPosition(ccp(1630, 470));
            CCMenu* menu11 = CCMenu::create(pChosLev11, NULL);
            menu11->setAnchorPoint(ccp(0 ,0));
            menu11->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu11, 1);
            CCMenuItemImage *pChosLev12 = CCMenuItemImage::create("FB_levbtn515s.png",
                                                                  "FB_levbtn515s.png",
                                                                  this,
                                                                  menu_selector(ZhuLuLayer::chosLev));
            pChosLev12->setTag(GameState::getInstance()->getCopyId() * 100 + 15 + LEVEL_BTN_TAG);
            pChosLev12->setAnchorPoint(ccp(0, 0));
            pChosLev12->setPosition(ccp(1620, 250));
            CCMenu* menu12 = CCMenu::create(pChosLev12, NULL);
            menu12->setAnchorPoint(ccp(0 ,0));
            menu12->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu12, 1);
            CCMenuItemImage *pChosLevs5 = CCMenuItemImage::create("FB_levbtn515s1.png",
                                                                  "FB_levbtn515s1.png",
                                                                  this,
                                                                  menu_selector(ZhuLuLayer::chosLev));
            pChosLevs5->setTag(GameState::getInstance()->getCopyId() * 100 + 15 + LEVEL_BTN_TAG);
            pChosLevs5->setAnchorPoint(ccp(0, 0));
            pChosLevs5->setPosition(ccp(1815, 415));
            CCMenu* menus5 = CCMenu::create(pChosLevs5, NULL);
            menus5->setAnchorPoint(ccp(0 ,0));
            menus5->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus5, 1);
            CCMenuItemImage *pChosLev13 = CCMenuItemImage::create("FB_levbtn516s.png",
                                                                  "FB_levbtn516s.png",
                                                                  this,
                                                                  menu_selector(ZhuLuLayer::chosLev));
            pChosLev13->setTag(GameState::getInstance()->getCopyId() * 100 + 16 + LEVEL_BTN_TAG);
            pChosLev13->setAnchorPoint(ccp(0, 0));
            pChosLev13->setPosition(ccp(1910, 460));
            CCMenu* menu13 = CCMenu::create(pChosLev13, NULL);
            menu13->setAnchorPoint(ccp(0 ,0));
            menu13->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu13, 1);
            CCMenuItemImage *pChosLevs6 = CCMenuItemImage::create("FB_levbtn516s1.png",
                                                                  "FB_levbtn516s1.png",
                                                                  this,
                                                                  menu_selector(ZhuLuLayer::chosLev));
            pChosLevs6->setTag(GameState::getInstance()->getCopyId() * 100 + 16 + LEVEL_BTN_TAG);
            pChosLevs6->setAnchorPoint(ccp(0, 0));
            pChosLevs6->setPosition(ccp(1845, 525));
            CCMenu* menus6 = CCMenu::create(pChosLevs6, NULL);
            menus6->setAnchorPoint(ccp(0 ,0));
            menus6->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus6, 1);
            CCMenuItemImage *pChosLev14 = CCMenuItemImage::create("FB_levbtn519s.png",
                                                                  "FB_levbtn519s.png",
                                                                  this,
                                                                  menu_selector(ZhuLuLayer::chosLev));
            pChosLev14->setTag(GameState::getInstance()->getCopyId() * 100 + 19 + LEVEL_BTN_TAG);
            pChosLev14->setAnchorPoint(ccp(0, 0));
            pChosLev14->setPosition(ccp(2275, 250));
            CCMenu* menu14 = CCMenu::create(pChosLev14, NULL);
            menu14->setAnchorPoint(ccp(0 ,0));
            menu14->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu14, 1);
            CCMenuItemImage *pChosLev15 = CCMenuItemImage::create("FB_levbtn518s.png",
                                                                  "FB_levbtn518s.png",
                                                                  this,
                                                                  menu_selector(ZhuLuLayer::chosLev));
            pChosLev15->setTag(GameState::getInstance()->getCopyId() * 100 + 18 + LEVEL_BTN_TAG);
            pChosLev15->setAnchorPoint(ccp(0, 0));
            pChosLev15->setPosition(ccp(2105, 570));
            CCMenu* menu15 = CCMenu::create(pChosLev15, NULL);
            menu15->setAnchorPoint(ccp(0 ,0));
            menu15->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu15, 1);
            CCMenuItemImage *pChosLev16 = CCMenuItemImage::create("FB_levbtn520s.png",
                                                                  "FB_levbtn520s.png",
                                                                  this,
                                                                  menu_selector(ZhuLuLayer::chosLev));
            pChosLev16->setTag(GameState::getInstance()->getCopyId() * 100 + 20 + LEVEL_BTN_TAG);
            pChosLev16->setAnchorPoint(ccp(0, 0));
            pChosLev16->setPosition(ccp(2245, 465));
            CCMenu* menu16 = CCMenu::create(pChosLev16, NULL);
            menu16->setAnchorPoint(ccp(0 ,0));
            menu16->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu16, 1);
        }
        /**************************************************/
        /* 下邳之战抽出重复按钮 */
        if ( GameState::getInstance()->getCopyId() == 6 ) {
            CCMenuItemImage *pChosLevs = CCMenuItemImage::create("FB_levbtn601s.png",
                                                                "FB_levbtn601s.png",
                                                                this,
                                                                menu_selector(ZhuLuLayer::chosLev));
            pChosLevs->setTag(GameState::getInstance()->getCopyId() * 100 + 1 + LEVEL_BTN_TAG);
            pChosLevs->setAnchorPoint(ccp(0, 0));
            pChosLevs->setPosition(ccp(120, 495));
            CCMenu* menus = CCMenu::create(pChosLevs, NULL);
            menus->setAnchorPoint(ccp(0 ,0));
            menus->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus, 1);
            CCMenuItemImage *pChosLevs1 = CCMenuItemImage::create("FB_levbtn602s.png",
                                                                 "FB_levbtn602s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLevs1->setTag(GameState::getInstance()->getCopyId() * 100 + 2 + LEVEL_BTN_TAG);
            pChosLevs1->setAnchorPoint(ccp(0, 0));
            pChosLevs1->setPosition(ccp(195, 245));
            CCMenu* menus1 = CCMenu::create(pChosLevs1, NULL);
            menus1->setAnchorPoint(ccp(0 ,0));
            menus1->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus1, 1);
            CCMenuItemImage *pChosLevs2 = CCMenuItemImage::create("FB_levbtn604s.png",
                                                                  "FB_levbtn604s.png",
                                                                  this,
                                                                  menu_selector(ZhuLuLayer::chosLev));
            pChosLevs2->setTag(GameState::getInstance()->getCopyId() * 100 + 4 + LEVEL_BTN_TAG);
            pChosLevs2->setAnchorPoint(ccp(0, 0));
            pChosLevs2->setPosition(ccp(445, 575));
            CCMenu* menus2 = CCMenu::create(pChosLevs2, NULL);
            menus2->setAnchorPoint(ccp(0 ,0));
            menus2->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus2, 1);
            CCMenuItemImage *pChosLev = CCMenuItemImage::create("FB_levbtn606s.png",
                                                                "FB_levbtn606s.png",
                                                                this,
                                                                menu_selector(ZhuLuLayer::chosLev));
            pChosLev->setTag(GameState::getInstance()->getCopyId() * 100 + 6 + LEVEL_BTN_TAG);
            pChosLev->setAnchorPoint(ccp(0, 0));
            pChosLev->setPosition(ccp(505, 335));
            CCMenu* menu = CCMenu::create(pChosLev, NULL);
            menu->setAnchorPoint(ccp(0 ,0));
            menu->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu, 1);
            CCMenuItemImage *pChosLevs3 = CCMenuItemImage::create("FB_levbtn607s.png",
                                                                "FB_levbtn607s.png",
                                                                this,
                                                                menu_selector(ZhuLuLayer::chosLev));
            pChosLevs3->setTag(GameState::getInstance()->getCopyId() * 100 + 7 + LEVEL_BTN_TAG);
            pChosLevs3->setAnchorPoint(ccp(0, 0));
            pChosLevs3->setPosition(ccp(645, 500));
            CCMenu* menus3 = CCMenu::create(pChosLevs3, NULL);
            menus3->setAnchorPoint(ccp(0 ,0));
            menus3->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus3, 1);
            CCMenuItemImage *pChosLev1 = CCMenuItemImage::create("FB_levbtn606s1.png",
                                                                 "FB_levbtn606s1.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev1->setTag(GameState::getInstance()->getCopyId() * 100 + 6 + LEVEL_BTN_TAG);
            pChosLev1->setAnchorPoint(ccp(0, 0));
            pChosLev1->setPosition(ccp(535, 260));
            CCMenu* menu1 = CCMenu::create(pChosLev1, NULL);
            menu1->setAnchorPoint(ccp(0 ,0));
            menu1->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu1, 1);
            CCMenuItemImage *pChosLev2 = CCMenuItemImage::create("FB_levbtn605s.png",
                                                                 "FB_levbtn605s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev2->setTag(GameState::getInstance()->getCopyId() * 100 + 5 + LEVEL_BTN_TAG);
            pChosLev2->setAnchorPoint(ccp(0, 0));
            pChosLev2->setPosition(ccp(610, 655));
            CCMenu* menu2 = CCMenu::create(pChosLev2, NULL);
            menu2->setAnchorPoint(ccp(0 ,0));
            menu2->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu2, 1);
            CCMenuItemImage *pChosLev3 = CCMenuItemImage::create("FB_levbtn608s.png",
                                                                 "FB_levbtn608s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev3->setTag(GameState::getInstance()->getCopyId() * 100 + 8 + LEVEL_BTN_TAG);
            pChosLev3->setAnchorPoint(ccp(0, 0));
            pChosLev3->setPosition(ccp(675, 335));
            CCMenu* menu3 = CCMenu::create(pChosLev3, NULL);
            menu3->setAnchorPoint(ccp(0 ,0));
            menu3->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu3, 1);
            CCMenuItemImage *pChosLevs4 = CCMenuItemImage::create("FB_levbtn608s1.png",
                                                                 "FB_levbtn608s1.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLevs4->setTag(GameState::getInstance()->getCopyId() * 100 + 8 + LEVEL_BTN_TAG);
            pChosLevs4->setAnchorPoint(ccp(0, 0));
            pChosLevs4->setPosition(ccp(675, 390));
            CCMenu* menus4 = CCMenu::create(pChosLevs4, NULL);
            menus4->setAnchorPoint(ccp(0 ,0));
            menus4->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus4, 1);
            CCMenuItemImage *pChosLev4 = CCMenuItemImage::create("FB_levbtn609s.png",
                                                                 "FB_levbtn609s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev4->setTag(GameState::getInstance()->getCopyId() * 100 + 9 + LEVEL_BTN_TAG);
            pChosLev4->setAnchorPoint(ccp(0, 0));
            pChosLev4->setPosition(ccp(910, 500));
            CCMenu* menu4 = CCMenu::create(pChosLev4, NULL);
            menu4->setAnchorPoint(ccp(0 ,0));
            menu4->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu4, 1);
            CCMenuItemImage *pChosLev5 = CCMenuItemImage::create("FB_levbtn611s.png",
                                                                 "FB_levbtn611s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev5->setTag(GameState::getInstance()->getCopyId() * 100 + 11 + LEVEL_BTN_TAG);
            pChosLev5->setAnchorPoint(ccp(0, 0));
            pChosLev5->setPosition(ccp(1280, 645));
            CCMenu* menu5 = CCMenu::create(pChosLev5, NULL);
            menu5->setAnchorPoint(ccp(0 ,0));
            menu5->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu5, 1);
            CCMenuItemImage *pChosLev6 = CCMenuItemImage::create("FB_levbtn611s1.png",
                                                                 "FB_levbtn611s1.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev6->setTag(GameState::getInstance()->getCopyId() * 100 + 11 + LEVEL_BTN_TAG);
            pChosLev6->setAnchorPoint(ccp(0, 0));
            pChosLev6->setPosition(ccp(1080, 510));
            CCMenu* menu6 = CCMenu::create(pChosLev6, NULL);
            menu6->setAnchorPoint(ccp(0 ,0));
            menu6->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu6, 1);
            CCMenuItemImage *pChosLev7 = CCMenuItemImage::create("FB_levbtn610s.png",
                                                                 "FB_levbtn610s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev7->setTag(GameState::getInstance()->getCopyId() * 100 + 10 + LEVEL_BTN_TAG);
            pChosLev7->setAnchorPoint(ccp(0, 0));
            pChosLev7->setPosition(ccp(795, 390));
            CCMenu* menu7 = CCMenu::create(pChosLev7, NULL);
            menu7->setAnchorPoint(ccp(0 ,0));
            menu7->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu7, 1);
            CCMenuItemImage *pChosLev8 = CCMenuItemImage::create("FB_levbtn612s.png",
                                                                 "FB_levbtn612s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev8->setTag(GameState::getInstance()->getCopyId() * 100 + 12 + LEVEL_BTN_TAG);
            pChosLev8->setAnchorPoint(ccp(0, 0));
            pChosLev8->setPosition(ccp(1155, 425));
            CCMenu* menu8 = CCMenu::create(pChosLev8, NULL);
            menu8->setAnchorPoint(ccp(0 ,0));
            menu8->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu8, 1);
            CCMenuItemImage *pChosLev9 = CCMenuItemImage::create("FB_levbtn614s.png",
                                                                 "FB_levbtn614s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev9->setTag(GameState::getInstance()->getCopyId() * 100 + 14 + LEVEL_BTN_TAG);
            pChosLev9->setAnchorPoint(ccp(0, 0));
            pChosLev9->setPosition(ccp(1380, 655));
            CCMenu* menu9 = CCMenu::create(pChosLev9, NULL);
            menu9->setAnchorPoint(ccp(0 ,0));
            menu9->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu9, 1);
            CCMenuItemImage *pChosLev10 = CCMenuItemImage::create("FB_levbtn614s1.png",
                                                                  "FB_levbtn614s1.png",
                                                                  this,
                                                                  menu_selector(ZhuLuLayer::chosLev));
            pChosLev10->setTag(GameState::getInstance()->getCopyId() * 100 + 14 + LEVEL_BTN_TAG);
            pChosLev10->setAnchorPoint(ccp(0, 0));
            pChosLev10->setPosition(ccp(1200, 525));
            CCMenu* menu10 = CCMenu::create(pChosLev10, NULL);
            menu10->setAnchorPoint(ccp(0 ,0));
            menu10->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu10, 1);
            CCMenuItemImage *pChosLev11 = CCMenuItemImage::create("FB_levbtn614s2.png",
                                                                  "FB_levbtn614s2.png",
                                                                  this,
                                                                  menu_selector(ZhuLuLayer::chosLev));
            pChosLev11->setTag(GameState::getInstance()->getCopyId() * 100 + 14 + LEVEL_BTN_TAG);
            pChosLev11->setAnchorPoint(ccp(0, 0));
            pChosLev11->setPosition(ccp(1440, 435));
            CCMenu* menu11 = CCMenu::create(pChosLev11, NULL);
            menu11->setAnchorPoint(ccp(0 ,0));
            menu11->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu11, 1);
            CCMenuItemImage *pChosLev12 = CCMenuItemImage::create("FB_levbtn615s.png",
                                                                  "FB_levbtn615s.png",
                                                                  this,
                                                                  menu_selector(ZhuLuLayer::chosLev));
            pChosLev12->setTag(GameState::getInstance()->getCopyId() * 100 + 15 + LEVEL_BTN_TAG);
            pChosLev12->setAnchorPoint(ccp(0, 0));
            pChosLev12->setPosition(ccp(1500, 450));
            CCMenu* menu12 = CCMenu::create(pChosLev12, NULL);
            menu12->setAnchorPoint(ccp(0 ,0));
            menu12->setPosition(ccp(0 ,0));

        }
        /************************************************/
        /* 袁术讨伐战抽出重复按钮 */
        if ( GameState::getInstance()->getCopyId() == 7 ) {
            CCMenuItemImage *pChosLevs = CCMenuItemImage::create("FB_levbtn701s.png",
                                                                "FB_levbtn701s.png",
                                                                this,
                                                                menu_selector(ZhuLuLayer::chosLev));
            pChosLevs->setTag(GameState::getInstance()->getCopyId() * 100 + 1 + LEVEL_BTN_TAG);
            pChosLevs->setAnchorPoint(ccp(0, 0));
            pChosLevs->setPosition(ccp(260, 460));
            CCMenu* menus = CCMenu::create(pChosLevs, NULL);
            menus->setAnchorPoint(ccp(0 ,0));
            menus->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus, 1);
            CCMenuItemImage *pChosLev = CCMenuItemImage::create("FB_levbtn703s.png",
                                                                "FB_levbtn703s.png",
                                                                this,
                                                                menu_selector(ZhuLuLayer::chosLev));
            pChosLev->setTag(GameState::getInstance()->getCopyId() * 100 + 3 + LEVEL_BTN_TAG);
            pChosLev->setAnchorPoint(ccp(0, 0));
            pChosLev->setPosition(ccp(295, 580));
            CCMenu* menu = CCMenu::create(pChosLev, NULL);
            menu->setAnchorPoint(ccp(0 ,0));
            menu->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu, 1);
            CCMenuItemImage *pChosLev1 = CCMenuItemImage::create("FB_levbtn705s1.png",
                                                                 "FB_levbtn705s1.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev1->setTag(GameState::getInstance()->getCopyId() * 100 + 5 + LEVEL_BTN_TAG);
            pChosLev1->setAnchorPoint(ccp(0, 0));
            pChosLev1->setPosition(ccp(520, 460));
            CCMenu* menu1 = CCMenu::create(pChosLev1, NULL);
            menu1->setAnchorPoint(ccp(0 ,0));
            menu1->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu1, 1);
            CCMenuItemImage *pChosLevs1 = CCMenuItemImage::create("FB_levbtn705s.png",
                                                                 "FB_levbtn705s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLevs1->setTag(GameState::getInstance()->getCopyId() * 100 + 5 + LEVEL_BTN_TAG);
            pChosLevs1->setAnchorPoint(ccp(0, 0));
            pChosLevs1->setPosition(ccp(435, 595));
            CCMenu* menus1 = CCMenu::create(pChosLevs1, NULL);
            menus1->setAnchorPoint(ccp(0 ,0));
            menus1->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus1, 1);
            CCMenuItemImage *pChosLev2 = CCMenuItemImage::create("FB_levbtn706s.png",
                                                                 "FB_levbtn706s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev2->setTag(GameState::getInstance()->getCopyId() * 100 + 6 + LEVEL_BTN_TAG);
            pChosLev2->setAnchorPoint(ccp(0, 0));
            pChosLev2->setPosition(ccp(605, 335));
            CCMenu* menu2 = CCMenu::create(pChosLev2, NULL);
            menu2->setAnchorPoint(ccp(0 ,0));
            menu2->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu2, 1);
            CCMenuItemImage *pChosLevs2 = CCMenuItemImage::create("FB_levbtn707s.png",
                                                                 "FB_levbtn707s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLevs2->setTag(GameState::getInstance()->getCopyId() * 100 + 7 + LEVEL_BTN_TAG);
            pChosLevs2->setAnchorPoint(ccp(0, 0));
            pChosLevs2->setPosition(ccp(705, 465));
            CCMenu* menus2 = CCMenu::create(pChosLevs2, NULL);
            menus2->setAnchorPoint(ccp(0 ,0));
            menus2->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus2, 1);
            CCMenuItemImage *pChosLev3 = CCMenuItemImage::create("FB_levbtn708s.png",
                                                                 "FB_levbtn708s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev3->setTag(GameState::getInstance()->getCopyId() * 100 + 8 + LEVEL_BTN_TAG);
            pChosLev3->setAnchorPoint(ccp(0, 0));
            pChosLev3->setPosition(ccp(980, 550));
            CCMenu* menu3 = CCMenu::create(pChosLev3, NULL);
            menu3->setAnchorPoint(ccp(0 ,0));
            menu3->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu3, 1);
            CCMenuItemImage *pChosLev4 = CCMenuItemImage::create("FB_levbtn708s1.png",
                                                                 "FB_levbtn708s1.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev4->setTag(GameState::getInstance()->getCopyId() * 100 + 8 + LEVEL_BTN_TAG);
            pChosLev4->setAnchorPoint(ccp(0, 0));
            pChosLev4->setPosition(ccp(825, 395));
            CCMenu* menu4 = CCMenu::create(pChosLev4, NULL);
            menu4->setAnchorPoint(ccp(0 ,0));
            menu4->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu4, 1);
            CCMenuItemImage *pChosLev5 = CCMenuItemImage::create("FB_levbtn709s.png",
                                                                 "FB_levbtn709s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev5->setTag(GameState::getInstance()->getCopyId() * 100 + 9 + LEVEL_BTN_TAG);
            pChosLev5->setAnchorPoint(ccp(0, 0));
            pChosLev5->setPosition(ccp(1060, 505));
            CCMenu* menu5 = CCMenu::create(pChosLev5, NULL);
            menu5->setAnchorPoint(ccp(0 ,0));
            menu5->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu5, 1);
            CCMenuItemImage *pChosLev6 = CCMenuItemImage::create("FB_levbtn709s1.png",
                                                                 "FB_levbtn709s1.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev6->setTag(GameState::getInstance()->getCopyId() * 100 + 9 + LEVEL_BTN_TAG);
            pChosLev6->setAnchorPoint(ccp(0, 0));
            pChosLev6->setPosition(ccp(1010, 485));
            CCMenu* menu6 = CCMenu::create(pChosLev6, NULL);
            menu6->setAnchorPoint(ccp(0 ,0));
            menu6->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu6, 1);
            CCMenuItemImage *pChosLevs4 = CCMenuItemImage::create("FB_levbtn710s.png",
                                                                 "FB_levbtn710s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLevs4->setTag(GameState::getInstance()->getCopyId() * 100 + 10 + LEVEL_BTN_TAG);
            pChosLevs4->setAnchorPoint(ccp(0, 0));
            pChosLevs4->setPosition(ccp(1135, 250));
            CCMenu* menus4 = CCMenu::create(pChosLevs4, NULL);
            menus4->setAnchorPoint(ccp(0 ,0));
            menus4->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus4, 1);
            CCMenuItemImage *pChosLevs5 = CCMenuItemImage::create("FB_levbtn711s.png",
                                                                 "FB_levbtn711s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLevs5->setTag(GameState::getInstance()->getCopyId() * 100 + 11 + LEVEL_BTN_TAG);
            pChosLevs5->setAnchorPoint(ccp(0, 0));
            pChosLevs5->setPosition(ccp(840, 315));
            CCMenu* menus5 = CCMenu::create(pChosLevs5, NULL);
            menus5->setAnchorPoint(ccp(0 ,0));
            menus5->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus5, 1);
            CCMenuItemImage *pChosLevs6 = CCMenuItemImage::create("FB_levbtn711s1.png",
                                                                 "FB_levbtn711s1.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLevs6->setTag(GameState::getInstance()->getCopyId() * 100 + 11 + LEVEL_BTN_TAG);
            pChosLevs6->setAnchorPoint(ccp(0, 0));
            pChosLevs6->setPosition(ccp(930, 385));
            CCMenu* menus6 = CCMenu::create(pChosLevs6, NULL);
            menus6->setAnchorPoint(ccp(0 ,0));
            menus6->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus6, 1);
            CCMenuItemImage *pChosLevs7 = CCMenuItemImage::create("FB_levbtn712s.png",
                                                                  "FB_levbtn712s.png",
                                                                  this,
                                                                  menu_selector(ZhuLuLayer::chosLev));
            pChosLevs7->setTag(GameState::getInstance()->getCopyId() * 100 + 12 + LEVEL_BTN_TAG);
            pChosLevs7->setAnchorPoint(ccp(0, 0));
            pChosLevs7->setPosition(ccp(1265, 435));
            CCMenu* menuss7 = CCMenu::create(pChosLevs7, NULL);
            menuss7->setAnchorPoint(ccp(0 ,0));
            menuss7->setPosition(ccp(0 ,0));
            litMapNode->addChild(menuss7, 1);
            CCMenuItemImage *pChosLev7 = CCMenuItemImage::create("FB_levbtn714s.png",
                                                                 "FB_levbtn714s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev7->setTag(GameState::getInstance()->getCopyId() * 100 + 14 + LEVEL_BTN_TAG);
            pChosLev7->setAnchorPoint(ccp(0, 0));
            pChosLev7->setPosition(ccp(1445, 500));
            CCMenu* menu7 = CCMenu::create(pChosLev7, NULL);
            menu7->setAnchorPoint(ccp(0 ,0));
            menu7->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu7, 1);
            CCMenuItemImage *pChosLevs8 = CCMenuItemImage::create("FB_levbtn715s.png",
                                                                 "FB_levbtn715s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLevs8->setTag(GameState::getInstance()->getCopyId() * 100 + 15 + LEVEL_BTN_TAG);
            pChosLevs8->setAnchorPoint(ccp(0, 0));
            pChosLevs8->setPosition(ccp(1420, 335));
            CCMenu* menus8 = CCMenu::create(pChosLevs8, NULL);
            menus8->setAnchorPoint(ccp(0 ,0));
            menus8->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus8, 1);
            
        }
        /**********************************ssss***/
        /* 袁术讨伐战抽出重复按钮 */
        if ( GameState::getInstance()->getCopyId() == 8 ) {
            CCMenuItemImage *pChosLevs = CCMenuItemImage::create("FB_levbtn801s.png",
                                                                "FB_levbtn801s.png",
                                                                this,
                                                                menu_selector(ZhuLuLayer::chosLev));
            pChosLevs->setTag(GameState::getInstance()->getCopyId() * 100 + 1 + LEVEL_BTN_TAG);
            pChosLevs->setAnchorPoint(ccp(0, 0));
            pChosLevs->setPosition(ccp(345, 585));
            CCMenu* menus = CCMenu::create(pChosLevs, NULL);
            menus->setAnchorPoint(ccp(0 ,0));
            menus->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus, 1);
            CCMenuItemImage *pChosLevs2 = CCMenuItemImage::create("FB_levbtn802s.png",
                                                                 "FB_levbtn802s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLevs2->setTag(GameState::getInstance()->getCopyId() * 100 + 2 + LEVEL_BTN_TAG);
            pChosLevs2->setAnchorPoint(ccp(0, 0));
            pChosLevs2->setPosition(ccp(275, 245));
            CCMenu* menus2 = CCMenu::create(pChosLevs2, NULL);
            menus2->setAnchorPoint(ccp(0 ,0));
            menus2->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus2, 1);
            CCMenuItemImage *pChosLev = CCMenuItemImage::create("FB_levbtn803s.png",
                                                                "FB_levbtn803s.png",
                                                                this,
                                                                menu_selector(ZhuLuLayer::chosLev));
            pChosLev->setTag(GameState::getInstance()->getCopyId() * 100 + 3 + LEVEL_BTN_TAG);
            pChosLev->setAnchorPoint(ccp(0, 0));
            pChosLev->setPosition(ccp(360, 490));
            CCMenu* menu = CCMenu::create(pChosLev, NULL);
            menu->setAnchorPoint(ccp(0 ,0));
            menu->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu, 1);
            CCMenuItemImage *pChosLevs3 = CCMenuItemImage::create("FB_levbtn803s1.png",
                                                                "FB_levbtn803s1.png",
                                                                this,
                                                                menu_selector(ZhuLuLayer::chosLev));
            pChosLevs3->setTag(GameState::getInstance()->getCopyId() * 100 + 3 + LEVEL_BTN_TAG);
            pChosLevs3->setAnchorPoint(ccp(0, 0));
            pChosLevs3->setPosition(ccp(285, 385));
            CCMenu* menus3 = CCMenu::create(pChosLevs3, NULL);
            menus3->setAnchorPoint(ccp(0 ,0));
            menus3->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus3, 1);
            CCMenuItemImage *pChosLev1 = CCMenuItemImage::create("FB_levbtn804s.png",
                                                                 "FB_levbtn804s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev1->setTag(GameState::getInstance()->getCopyId() * 100 + 4 + LEVEL_BTN_TAG);
            pChosLev1->setAnchorPoint(ccp(0, 0));
            pChosLev1->setPosition(ccp(575, 470));
            CCMenu* menu1 = CCMenu::create(pChosLev1, NULL);
            menu1->setAnchorPoint(ccp(0 ,0));
            menu1->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu1, 1);
            CCMenuItemImage *pChosLevs4 = CCMenuItemImage::create("FB_levbtn804s1.png",
                                                                 "FB_levbtn804s1.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLevs4->setTag(GameState::getInstance()->getCopyId() * 100 + 4 + LEVEL_BTN_TAG);
            pChosLevs4->setAnchorPoint(ccp(0, 0));
            pChosLevs4->setPosition(ccp(415, 650));
            CCMenu* menus4 = CCMenu::create(pChosLevs4, NULL);
            menus4->setAnchorPoint(ccp(0 ,0));
            menus4->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus4, 1);
            CCMenuItemImage *pChosLev2 = CCMenuItemImage::create("FB_levbtn805s.png",
                                                                 "FB_levbtn805s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev2->setTag(GameState::getInstance()->getCopyId() * 100 + 5 + LEVEL_BTN_TAG);
            pChosLev2->setAnchorPoint(ccp(0, 0));
            pChosLev2->setPosition(ccp(425, 255));
            CCMenu* menu2 = CCMenu::create(pChosLev2, NULL);
            menu2->setAnchorPoint(ccp(0 ,0));
            menu2->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu2, 1);
            CCMenuItemImage *pChosLevs5 = CCMenuItemImage::create("FB_levbtn805s1.png",
                                                                 "FB_levbtn805s1.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLevs5->setTag(GameState::getInstance()->getCopyId() * 100 + 5 + LEVEL_BTN_TAG);
            pChosLevs5->setAnchorPoint(ccp(0, 0));
            pChosLevs5->setPosition(ccp(395, 250));
            CCMenu* menus5 = CCMenu::create(pChosLevs5, NULL);
            menus5->setAnchorPoint(ccp(0 ,0));
            menus5->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus5, 1);
            CCMenuItemImage *pChosLev3 = CCMenuItemImage::create("FB_levbtn807s.png",
                                                                 "FB_levbtn807s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev3->setTag(GameState::getInstance()->getCopyId() * 100 + 7 + LEVEL_BTN_TAG);
            pChosLev3->setAnchorPoint(ccp(0, 0));
            pChosLev3->setPosition(ccp(565, 295));
            CCMenu* menu3 = CCMenu::create(pChosLev3, NULL);
            menu3->setAnchorPoint(ccp(0 ,0));
            menu3->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu3, 1);
            CCMenuItemImage *pChosLevs6 = CCMenuItemImage::create("FB_levbtn807s1.png",
                                                                 "FB_levbtn807s1.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLevs6->setTag(GameState::getInstance()->getCopyId() * 100 + 7 + LEVEL_BTN_TAG);
            pChosLevs6->setAnchorPoint(ccp(0, 0));
            pChosLevs6->setPosition(ccp(685, 355));
            CCMenu* menus6 = CCMenu::create(pChosLevs6, NULL);
            menus6->setAnchorPoint(ccp(0 ,0));
            CCMenuItemImage *pChosLev5 = CCMenuItemImage::create("FB_levbtn806s.png",
                                                                 "FB_levbtn806s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev5->setTag(GameState::getInstance()->getCopyId() * 100 + 6 + LEVEL_BTN_TAG);
            pChosLev5->setAnchorPoint(ccp(0, 0));
            pChosLev5->setPosition(ccp(800, 440));
            CCMenu* menu5 = CCMenu::create(pChosLev5, NULL);
            menu5->setAnchorPoint(ccp(0 ,0));
            menu5->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu5, 1);
            CCMenuItemImage *pChosLev6 = CCMenuItemImage::create("FB_levbtn810s1.png",
                                                                 "FB_levbtn810s1.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev6->setTag(GameState::getInstance()->getCopyId() * 100 + 10 + LEVEL_BTN_TAG);
            pChosLev6->setAnchorPoint(ccp(0, 0));
            pChosLev6->setPosition(ccp(970, 450));
            CCMenu* menu6 = CCMenu::create(pChosLev6, NULL);
            menu6->setAnchorPoint(ccp(0 ,0));
            menu6->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu6, 1);
            CCMenuItemImage *pChosLev7 = CCMenuItemImage::create("FB_levbtn810s.png",
                                                                 "FB_levbtn810s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev7->setTag(GameState::getInstance()->getCopyId() * 100 + 10 + LEVEL_BTN_TAG);
            pChosLev7->setAnchorPoint(ccp(0, 0));
            pChosLev7->setPosition(ccp(1090, 265));
            CCMenu* menu7 = CCMenu::create(pChosLev7, NULL);
            menu7->setAnchorPoint(ccp(0 ,0));
            menu7->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu7, 1);
            CCMenuItemImage *pChosLev8 = CCMenuItemImage::create("FB_levbtn809s.png",
                                                                 "FB_levbtn809s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev8->setTag(GameState::getInstance()->getCopyId() * 100 + 9 + LEVEL_BTN_TAG);
            pChosLev8->setAnchorPoint(ccp(0, 0));
            pChosLev8->setPosition(ccp(990, 565));
            CCMenu* menu8 = CCMenu::create(pChosLev8, NULL);
            menu8->setAnchorPoint(ccp(0 ,0));
            menu8->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu8, 1);
            CCMenuItemImage *pChosLevs8 = CCMenuItemImage::create("FB_levbtn809s1.png",
                                                                 "FB_levbtn809s1.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLevs8->setTag(GameState::getInstance()->getCopyId() * 100 + 9 + LEVEL_BTN_TAG);
            pChosLevs8->setAnchorPoint(ccp(0, 0));
            pChosLevs8->setPosition(ccp(1125, 485));
            CCMenu* menus8 = CCMenu::create(pChosLevs8, NULL);
            menus8->setAnchorPoint(ccp(0 ,0));
            menus8->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus8, 1);
            CCMenuItemImage *pChosLev9 = CCMenuItemImage::create("FB_levbtn811s.png",
                                                                 "FB_levbtn811s.png",
                                                                 this,
                                                                 menu_selector(ZhuLuLayer::chosLev));
            pChosLev9->setTag(GameState::getInstance()->getCopyId() * 100 + 11 + LEVEL_BTN_TAG);
            pChosLev9->setAnchorPoint(ccp(0, 0));
            pChosLev9->setPosition(ccp(1260, 450));
            CCMenu* menu9 = CCMenu::create(pChosLev9, NULL);
            menu9->setAnchorPoint(ccp(0 ,0));
            menu9->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu9, 1);
            CCMenuItemImage *pChosLev10 = CCMenuItemImage::create("FB_levbtn814s.png",
                                                                  "FB_levbtn814s.png",
                                                                  this,
                                                                  menu_selector(ZhuLuLayer::chosLev));
            pChosLev10->setTag(GameState::getInstance()->getCopyId() * 100 + 14 + LEVEL_BTN_TAG);
            pChosLev10->setAnchorPoint(ccp(0, 0));
            pChosLev10->setPosition(ccp(1525, 460));
            CCMenu* menu10 = CCMenu::create(pChosLev10, NULL);
            menu10->setAnchorPoint(ccp(0 ,0));
            menu10->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu10, 1);
            CCMenuItemImage *pChosLev11 = CCMenuItemImage::create("FB_levbtn815s.png",
                                                                  "FB_levbtn815s.png",
                                                                  this,
                                                                  menu_selector(ZhuLuLayer::chosLev));
            pChosLev11->setTag(GameState::getInstance()->getCopyId() * 100 + 15 + LEVEL_BTN_TAG);
            pChosLev11->setAnchorPoint(ccp(0, 0));
            pChosLev11->setPosition(ccp(1605, 320));
            CCMenu* menu11 = CCMenu::create(pChosLev11, NULL);
            menu11->setAnchorPoint(ccp(0 ,0));
            menu11->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu11, 1);
            CCMenuItemImage *pChosLev12 = CCMenuItemImage::create("FB_levbtn815s1.png",
                                                                  "FB_levbtn815s1.png",
                                                                  this,
                                                                  menu_selector(ZhuLuLayer::chosLev));
            pChosLev12->setTag(GameState::getInstance()->getCopyId() * 100 + 15 + LEVEL_BTN_TAG);
            pChosLev12->setAnchorPoint(ccp(0, 0));
            pChosLev12->setPosition(ccp(1610, 255));
            CCMenu* menu12 = CCMenu::create(pChosLev12, NULL);
            menu12->setAnchorPoint(ccp(0 ,0));
            menu12->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu12, 1);
            CCMenuItemImage *pChosLevss11 = CCMenuItemImage::create("FB_levbtn816s1.png",
                                                                   "FB_levbtn816s1.png",
                                                                   this,
                                                                   menu_selector(ZhuLuLayer::chosLev));
            pChosLevss11->setTag(GameState::getInstance()->getCopyId() * 100 + 16 + LEVEL_BTN_TAG);
            pChosLevss11->setAnchorPoint(ccp(0, 0));
            pChosLevss11->setPosition(ccp(1785, 585));
            CCMenu* menuss11 = CCMenu::create(pChosLevss11, NULL);
            menuss11->setAnchorPoint(ccp(0 ,0));
            menuss11->setPosition(ccp(0 ,0));
            litMapNode->addChild(menuss11, 1);
            CCMenuItemImage *pChosLevss1 = CCMenuItemImage::create("FB_levbtn816s2.png",
                                                                   "FB_levbtn816s2.png",
                                                                   this,
                                                                   menu_selector(ZhuLuLayer::chosLev));
            pChosLevss1->setTag(GameState::getInstance()->getCopyId() * 100 + 16 + LEVEL_BTN_TAG);
            pChosLevss1->setAnchorPoint(ccp(0, 0));
            pChosLevss1->setPosition(ccp(1655, 555));
            CCMenu* menuss1 = CCMenu::create(pChosLevss1, NULL);
            menuss1->setAnchorPoint(ccp(0 ,0));
            menuss1->setPosition(ccp(0 ,0));
            litMapNode->addChild(menuss1, 1);
            CCMenuItemImage *pChosLev13 = CCMenuItemImage::create("FB_levbtn818s.png",
                                                                  "FB_levbtn818s.png",
                                                                  this,
                                                                  menu_selector(ZhuLuLayer::chosLev));
            pChosLev13->setTag(GameState::getInstance()->getCopyId() * 100 + 18 + LEVEL_BTN_TAG);
            pChosLev13->setAnchorPoint(ccp(0, 0));
            pChosLev13->setPosition(ccp(1840, 530));
            CCMenu* menu13 = CCMenu::create(pChosLev13, NULL);
            menu13->setAnchorPoint(ccp(0 ,0));
            menu13->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu13, 1);
            CCMenuItemImage *pChosLev14 = CCMenuItemImage::create("FB_levbtn819s.png",
                                                                  "FB_levbtn819s.png",
                                                                  this,
                                                                  menu_selector(ZhuLuLayer::chosLev));
            pChosLev14->setTag(GameState::getInstance()->getCopyId() * 100 + 19 + LEVEL_BTN_TAG);
            pChosLev14->setAnchorPoint(ccp(0, 0));
            pChosLev14->setPosition(ccp(2025, 435));
            CCMenu* menu14 = CCMenu::create(pChosLev14, NULL);
            menu14->setAnchorPoint(ccp(0 ,0));
            menu14->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu14, 1);
            CCMenuItemImage *pChosLevs14 = CCMenuItemImage::create("FB_levbtn819s1.png",
                                                                  "FB_levbtn819s1.png",
                                                                  this,
                                                                  menu_selector(ZhuLuLayer::chosLev));
            pChosLevs14->setTag(GameState::getInstance()->getCopyId() * 100 + 19 + LEVEL_BTN_TAG);
            pChosLevs14->setAnchorPoint(ccp(0, 0));
            pChosLevs14->setPosition(ccp(1975, 355));
            CCMenu* menus14 = CCMenu::create(pChosLevs14, NULL);
            menus14->setAnchorPoint(ccp(0 ,0));
            menus14->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus14, 1);
            CCMenuItemImage *pChosLev15 = CCMenuItemImage::create("FB_levbtn817s.png",
                                                                  "FB_levbtn817s.png",
                                                                  this,
                                                                  menu_selector(ZhuLuLayer::chosLev));
            pChosLev15->setTag(GameState::getInstance()->getCopyId() * 100 + 17 + LEVEL_BTN_TAG);
            pChosLev15->setAnchorPoint(ccp(0, 0));
            pChosLev15->setPosition(ccp(1805, 250));
            CCMenu* menu15 = CCMenu::create(pChosLev15, NULL);
            menu15->setAnchorPoint(ccp(0 ,0));
            menu15->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu15, 1);
            CCMenuItemImage *pChosLevs15 = CCMenuItemImage::create("FB_levbtn817s1.png",
                                                                  "FB_levbtn817s1.png",
                                                                  this,
                                                                  menu_selector(ZhuLuLayer::chosLev));
            pChosLevs15->setTag(GameState::getInstance()->getCopyId() * 100 + 17 + LEVEL_BTN_TAG);
            pChosLevs15->setAnchorPoint(ccp(0, 0));
            pChosLevs15->setPosition(ccp(1825, 345));
            CCMenu* menus15 = CCMenu::create(pChosLevs15, NULL);
            menus15->setAnchorPoint(ccp(0 ,0));
            menus15->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus15, 1);
            CCMenuItemImage *pChosLev16 = CCMenuItemImage::create("FB_levbtn820s.png",
                                                                  "FB_levbtn820s.png",
                                                                  this,
                                                                  menu_selector(ZhuLuLayer::chosLev));
            pChosLev16->setTag(GameState::getInstance()->getCopyId() * 100 + 20 + LEVEL_BTN_TAG);
            pChosLev16->setAnchorPoint(ccp(0, 0));
            pChosLev16->setPosition(ccp(2100, 500));
            CCMenu* menu16 = CCMenu::create(pChosLev16, NULL);
            menu16->setAnchorPoint(ccp(0 ,0));
            menu16->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu16, 1);
            CCMenuItemImage *pChosLevs16 = CCMenuItemImage::create("FB_levbtn821s.png",
                                                                  "FB_levbtn821s.png",
                                                                  this,
                                                                  menu_selector(ZhuLuLayer::chosLev));
            pChosLevs16->setTag(GameState::getInstance()->getCopyId() * 100 + 21 + LEVEL_BTN_TAG);
            pChosLevs16->setAnchorPoint(ccp(0, 0));
            pChosLevs16->setPosition(ccp(2250, 605));
            CCMenu* menus16 = CCMenu::create(pChosLevs16, NULL);
            menus16->setAnchorPoint(ccp(0 ,0));
            menus16->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus16, 1);
            CCMenuItemImage *pChosLev17 = CCMenuItemImage::create("FB_levbtn824s.png",
                                                                  "FB_levbtn824s.png",
                                                                  this,
                                                                  menu_selector(ZhuLuLayer::chosLev));
            pChosLev17->setTag(GameState::getInstance()->getCopyId() * 100 + 24 + LEVEL_BTN_TAG);
            pChosLev17->setAnchorPoint(ccp(0, 0));
            pChosLev17->setPosition(ccp(2360, 410));
            CCMenu* menu17 = CCMenu::create(pChosLev17, NULL);
            menu17->setAnchorPoint(ccp(0 ,0));
            menu17->setPosition(ccp(0 ,0));
            litMapNode->addChild(menu17, 1);
            CCMenuItemImage *pChosLevs17 = CCMenuItemImage::create("FB_levbtn823s.png",
                                                                  "FB_levbtn823s.png",
                                                                  this,
                                                                  menu_selector(ZhuLuLayer::chosLev));
            pChosLevs17->setTag(GameState::getInstance()->getCopyId() * 100 + 23 + LEVEL_BTN_TAG);
            pChosLevs17->setAnchorPoint(ccp(0, 0));
            pChosLevs17->setPosition(ccp(2385, 265));
            CCMenu* menus17 = CCMenu::create(pChosLevs17, NULL);
            menus17->setAnchorPoint(ccp(0 ,0));
            menus17->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus17, 1);
            CCMenuItemImage *pChosLevs18 = CCMenuItemImage::create("FB_levbtn823s1.png",
                                                                  "FB_levbtn823s1.png",
                                                                  this,
                                                                  menu_selector(ZhuLuLayer::chosLev));
            pChosLevs18->setTag(GameState::getInstance()->getCopyId() * 100 + 23 + LEVEL_BTN_TAG);
            pChosLevs18->setAnchorPoint(ccp(0, 0));
            pChosLevs18->setPosition(ccp(2310, 265));
            CCMenu* menus18 = CCMenu::create(pChosLevs18, NULL);
            menus18->setAnchorPoint(ccp(0 ,0));
            menus18->setPosition(ccp(0 ,0));
            litMapNode->addChild(menus18, 1);
        }
        
        
        litMapNode->setPosition(ccp(20, 0));
        m_scrollView->getContainer()->addChild(litMapNode, 1, LITMAPNODE);
        /* 副本描述 */
        sprintf(buffer, "FB_des_copy%d.png", GameState::getInstance()->getCopyId());
        CCSprite* spDes = CCSprite::create(buffer);
        spDes->setAnchorPoint(ccp(0, 0));
        spDes->setPosition(ccp(7.0f, 260.0f + GameState::getInstance()->getBottomOffset()));
        pNode->addChild(spDes, 102);
        
        /* 当前所选副本的名字 */
        sprintf(buffer, "FB_font_tip%d.png", GameState::getInstance()->getCopyId());
        CCSprite* copyName = CCSprite::create(buffer);
        copyName->setAnchorPoint(ccp(0.5f, 0.0f));
        copyName->setPosition(ccp(320.0f, bottomOffset + 635.0f));
        pNode->addChild(copyName, 5);

        //        std::string test = "测试字体";
        //        CCStrokeLabel* testLabel = CCStrokeLabel::create(test.c_str(), "Arial", 25);
        //        testLabel->setAnchorPoint(ccp(0, 0));
        //        testLabel->setPosition(ccp(10, 400));
        //        pNode->addChild(testLabel, 1001);
        /*
         * pCon: 透明按钮
         * spr:  地图背景
         * pP:   体力信息
         * pT:   关卡名字
         */
        CCMenu* pCon1   = (CCMenu*)litMapNode->getChildByTag(LEVEL_BTN_TAG + 20000 + 1);
        CCMenu* pCon2   = (CCMenu*)litMapNode->getChildByTag(LEVEL_BTN_TAG + 20000 + 2);
        CCMenu* pCon3   = (CCMenu*)litMapNode->getChildByTag(LEVEL_BTN_TAG + 20000 + 3);
        CCMenu* pCon4   = (CCMenu*)litMapNode->getChildByTag(LEVEL_BTN_TAG + 20000 + 4);
        CCMenu* pCon5   = (CCMenu*)litMapNode->getChildByTag(LEVEL_BTN_TAG + 20000 + 5);
        CCMenu* pCon6   = (CCMenu*)litMapNode->getChildByTag(LEVEL_BTN_TAG + 20000 + 6);
        CCMenu* pCon7   = (CCMenu*)litMapNode->getChildByTag(LEVEL_BTN_TAG + 20000 + 7);
        CCMenu* pCon8   = (CCMenu*)litMapNode->getChildByTag(LEVEL_BTN_TAG + 20000 + 8);
        CCMenu* pCon9   = (CCMenu*)litMapNode->getChildByTag(LEVEL_BTN_TAG + 20000 + 9);
        CCMenu* pCon10  = (CCMenu*)litMapNode->getChildByTag(LEVEL_BTN_TAG + 20000 + 10);
        CCMenu* pCon11  = (CCMenu*)litMapNode->getChildByTag(LEVEL_BTN_TAG + 20000 + 11);
        CCMenu* pCon12  = (CCMenu*)litMapNode->getChildByTag(LEVEL_BTN_TAG + 20000 + 12);
        CCMenu* pCon13  = (CCMenu*)litMapNode->getChildByTag(LEVEL_BTN_TAG + 20000 + 13);
        CCMenu* pCon14  = (CCMenu*)litMapNode->getChildByTag(LEVEL_BTN_TAG + 20000 + 14);
        CCMenu* pCon15  = (CCMenu*)litMapNode->getChildByTag(LEVEL_BTN_TAG + 20000 + 15);
        CCMenu* pCon16  = (CCMenu*)litMapNode->getChildByTag(LEVEL_BTN_TAG + 20000 + 16);
        CCMenu* pCon17  = (CCMenu*)litMapNode->getChildByTag(LEVEL_BTN_TAG + 20000 + 17);
        CCMenu* pCon18  = (CCMenu*)litMapNode->getChildByTag(LEVEL_BTN_TAG + 20000 + 18);
        CCMenu* pCon19  = (CCMenu*)litMapNode->getChildByTag(LEVEL_BTN_TAG + 20000 + 19);
        CCMenu* pCon20  = (CCMenu*)litMapNode->getChildByTag(LEVEL_BTN_TAG + 20000 + 20);
        CCMenu* pCon21  = (CCMenu*)litMapNode->getChildByTag(LEVEL_BTN_TAG + 20000 + 21);
        CCMenu* pCon22  = (CCMenu*)litMapNode->getChildByTag(LEVEL_BTN_TAG + 20000 + 22);
        CCMenu* pCon23  = (CCMenu*)litMapNode->getChildByTag(LEVEL_BTN_TAG + 20000 + 23);
        CCMenu* pCon24  = (CCMenu*)litMapNode->getChildByTag(LEVEL_BTN_TAG + 20000 + 24);
        
        CCSprite* spr1  = (CCSprite*)litMapNode->getChildByTag(LEV_MAP_TAG+1);
        CCSprite* spr2  = (CCSprite*)litMapNode->getChildByTag(LEV_MAP_TAG+2);
        CCSprite* spr3  = (CCSprite*)litMapNode->getChildByTag(LEV_MAP_TAG+3);
        CCSprite* spr4  = (CCSprite*)litMapNode->getChildByTag(LEV_MAP_TAG+4);
        CCSprite* spr5  = (CCSprite*)litMapNode->getChildByTag(LEV_MAP_TAG+5);
        CCSprite* spr6  = (CCSprite*)litMapNode->getChildByTag(LEV_MAP_TAG+6);
        CCSprite* spr7  = (CCSprite*)litMapNode->getChildByTag(LEV_MAP_TAG+7);
        CCSprite* spr8  = (CCSprite*)litMapNode->getChildByTag(LEV_MAP_TAG+8);
        CCSprite* spr9  = (CCSprite*)litMapNode->getChildByTag(LEV_MAP_TAG+9);
        CCSprite* spr10 = (CCSprite*)litMapNode->getChildByTag(LEV_MAP_TAG+10);
        CCSprite* spr11 = (CCSprite*)litMapNode->getChildByTag(LEV_MAP_TAG+11);
        CCSprite* spr12 = (CCSprite*)litMapNode->getChildByTag(LEV_MAP_TAG+12);
        CCSprite* spr13 = (CCSprite*)litMapNode->getChildByTag(LEV_MAP_TAG+13);
        CCSprite* spr14 = (CCSprite*)litMapNode->getChildByTag(LEV_MAP_TAG+14);
        CCSprite* spr15 = (CCSprite*)litMapNode->getChildByTag(LEV_MAP_TAG+15);
        CCSprite* spr16 = (CCSprite*)litMapNode->getChildByTag(LEV_MAP_TAG+16);
        CCSprite* spr17 = (CCSprite*)litMapNode->getChildByTag(LEV_MAP_TAG+17);
        CCSprite* spr18 = (CCSprite*)litMapNode->getChildByTag(LEV_MAP_TAG+18);
        CCSprite* spr19 = (CCSprite*)litMapNode->getChildByTag(LEV_MAP_TAG+19);
        CCSprite* spr20 = (CCSprite*)litMapNode->getChildByTag(LEV_MAP_TAG+20);
        CCSprite* spr21 = (CCSprite*)litMapNode->getChildByTag(LEV_MAP_TAG+21);
        CCSprite* spr22 = (CCSprite*)litMapNode->getChildByTag(LEV_MAP_TAG+22);
        CCSprite* spr23 = (CCSprite*)litMapNode->getChildByTag(LEV_MAP_TAG+23);
        CCSprite* spr24 = (CCSprite*)litMapNode->getChildByTag(LEV_MAP_TAG+24);
        
        CCSprite* pT1   = (CCSprite*)litMapNode->getChildByTag(LEV_NAME_TAG+1);
        CCSprite* pT2   = (CCSprite*)litMapNode->getChildByTag(LEV_NAME_TAG+2);
        CCSprite* pT3   = (CCSprite*)litMapNode->getChildByTag(LEV_NAME_TAG+3);
        CCSprite* pT4   = (CCSprite*)litMapNode->getChildByTag(LEV_NAME_TAG+4);
        CCSprite* pT5   = (CCSprite*)litMapNode->getChildByTag(LEV_NAME_TAG+5);
        CCSprite* pT6   = (CCSprite*)litMapNode->getChildByTag(LEV_NAME_TAG+6);
        CCSprite* pT7   = (CCSprite*)litMapNode->getChildByTag(LEV_NAME_TAG+7);
        CCSprite* pT8   = (CCSprite*)litMapNode->getChildByTag(LEV_NAME_TAG+8);
        CCSprite* pT9   = (CCSprite*)litMapNode->getChildByTag(LEV_NAME_TAG+9);
        CCSprite* pT10  = (CCSprite*)litMapNode->getChildByTag(LEV_NAME_TAG+10);
        CCSprite* pT11  = (CCSprite*)litMapNode->getChildByTag(LEV_NAME_TAG+11);
        CCSprite* pT12  = (CCSprite*)litMapNode->getChildByTag(LEV_NAME_TAG+12);
        CCSprite* pT13  = (CCSprite*)litMapNode->getChildByTag(LEV_NAME_TAG+13);
        CCSprite* pT14  = (CCSprite*)litMapNode->getChildByTag(LEV_NAME_TAG+14);
        CCSprite* pT15  = (CCSprite*)litMapNode->getChildByTag(LEV_NAME_TAG+15);
        CCSprite* pT16  = (CCSprite*)litMapNode->getChildByTag(LEV_NAME_TAG+16);
        CCSprite* pT17  = (CCSprite*)litMapNode->getChildByTag(LEV_NAME_TAG+17);
        CCSprite* pT18  = (CCSprite*)litMapNode->getChildByTag(LEV_NAME_TAG+18);
        CCSprite* pT19  = (CCSprite*)litMapNode->getChildByTag(LEV_NAME_TAG+19);
        CCSprite* pT20  = (CCSprite*)litMapNode->getChildByTag(LEV_NAME_TAG+20);
        CCSprite* pT21  = (CCSprite*)litMapNode->getChildByTag(LEV_NAME_TAG+21);
        CCSprite* pT22  = (CCSprite*)litMapNode->getChildByTag(LEV_NAME_TAG+22);
        CCSprite* pT23  = (CCSprite*)litMapNode->getChildByTag(LEV_NAME_TAG+23);
        CCSprite* pT24  = (CCSprite*)litMapNode->getChildByTag(LEV_NAME_TAG+24);
        
        CCNode* pP1   = (CCNode*)litMapNode->getChildByTag(POWER_INFO_TAG + 1);
        CCNode* pP2   = (CCNode*)litMapNode->getChildByTag(POWER_INFO_TAG + 2);
        CCNode* pP3   = (CCNode*)litMapNode->getChildByTag(POWER_INFO_TAG + 3);
        CCNode* pP4   = (CCNode*)litMapNode->getChildByTag(POWER_INFO_TAG + 4);
        CCNode* pP5   = (CCNode*)litMapNode->getChildByTag(POWER_INFO_TAG + 5);
        CCNode* pP6   = (CCNode*)litMapNode->getChildByTag(POWER_INFO_TAG + 6);
        CCNode* pP7   = (CCNode*)litMapNode->getChildByTag(POWER_INFO_TAG + 7);
        CCNode* pP8   = (CCNode*)litMapNode->getChildByTag(POWER_INFO_TAG + 8);
        CCNode* pP9   = (CCNode*)litMapNode->getChildByTag(POWER_INFO_TAG + 9);
        CCNode* pP10  = (CCNode*)litMapNode->getChildByTag(POWER_INFO_TAG + 10);
        CCNode* pP11  = (CCNode*)litMapNode->getChildByTag(POWER_INFO_TAG + 11);
        CCNode* pP12  = (CCNode*)litMapNode->getChildByTag(POWER_INFO_TAG + 12);
        CCNode* pP13  = (CCNode*)litMapNode->getChildByTag(POWER_INFO_TAG + 13);
        CCNode* pP14  = (CCNode*)litMapNode->getChildByTag(POWER_INFO_TAG + 14);
        CCNode* pP15  = (CCNode*)litMapNode->getChildByTag(POWER_INFO_TAG + 15);
        CCNode* pP16  = (CCNode*)litMapNode->getChildByTag(POWER_INFO_TAG + 16);
        CCNode* pP17  = (CCNode*)litMapNode->getChildByTag(POWER_INFO_TAG + 17);
        CCNode* pP18  = (CCNode*)litMapNode->getChildByTag(POWER_INFO_TAG + 18);
        CCNode* pP19  = (CCNode*)litMapNode->getChildByTag(POWER_INFO_TAG + 19);
        CCNode* pP20  = (CCNode*)litMapNode->getChildByTag(POWER_INFO_TAG + 20);
        CCNode* pP21  = (CCNode*)litMapNode->getChildByTag(POWER_INFO_TAG + 21);
        CCNode* pP22  = (CCNode*)litMapNode->getChildByTag(POWER_INFO_TAG + 22);
        CCNode* pP23  = (CCNode*)litMapNode->getChildByTag(POWER_INFO_TAG + 23);
        CCNode* pP24  = (CCNode*)litMapNode->getChildByTag(POWER_INFO_TAG + 24);
        
        switch (GameState::getInstance()->getCopyId()) {
            case 1:
            {
                // 按第一个副本的关卡位置排列
                pCon1->setPosition(125, 505);
                pCon2->setPosition(130, 250);
                pCon3->setPosition(375, 520);
                pCon4->setPosition(440, 390);
                pCon5->setPosition(370, 245);
                pCon6->setPosition(680, 475);
                pCon7->setPosition(760, 350);
                pCon8->setPosition(830, 515);
                pCon9->setPosition(860, 255);
                
                spr1->setPosition(ccp(112, 360));
                spr2->setPosition(ccp(110, 240));
                spr3->setPosition(ccp(327, 512));
                spr4->setPosition(ccp(380, 273));
                spr5->setPosition(ccp(350, 235));
                spr6->setPosition(ccp(608, 443));
                spr7->setPosition(ccp(648, 240));
                spr8->setPosition(ccp(778, 473));
                spr9->setPosition(ccp(835, 250));
                
                pT1->setPosition(ccp(130, 590));
                pT2->setPosition(ccp(130, 365));
                pT3->setPosition(ccp(400, 590));
                pT4->setPosition(ccp(460, 445));
                pT5->setPosition(ccp(400, 300));
                pT6->setPosition(ccp(640, 595));
                pT7->setPosition(ccp(765, 395));
                pT8->setPosition(ccp(860, 590));
                pT9->setPosition(ccp(845, 300));
                
                pP1->setPosition(170, 510);
                pP2->setPosition(170, 280);
                pP3->setPosition(430, 515);
                pP4->setPosition(520, 380);
                pP5->setPosition(420, 240);
                pP6->setPosition(670, 515);
                pP7->setPosition(800, 325);
                pP8->setPosition(890, 525);
                pP9->setPosition(875, 240);
            }
                break;
            case 2:
            {
                // 按第二个副本的关卡位置排列
                pCon1->setPosition(115, 495);
                pCon2->setPosition(110, 250);
                pCon3->setPosition(355, 575);
                pCon4->setPosition(320, 380);
                pCon5->setPosition(266, 245);
                pCon6->setPosition(610, 530);
                pCon7->setPosition(665, 390);
                pCon8->setPosition(680, 245);
                pCon9->setPosition(885, 485);
                pCon10->setPosition(880, 380);
                pCon11->setPosition(1085, 625);
                pCon12->setPosition(1017, 510);
                pCon13->setPosition(1100, 260);
                pCon14->setPosition(1490, 445);
                pCon15->setPosition(1490, 255);
                
                
                spr1->setPosition(ccp(103, 461));
                spr2->setPosition(ccp(105, 239));
                spr3->setPosition(ccp(317, 452));
                spr4->setPosition(ccp(300, 278));
                spr5->setPosition(ccp(236, 235));
                spr6->setPosition(ccp(550, 515));
                spr7->setPosition(ccp(607, 365));
                spr8->setPosition(ccp(582, 239));
                spr9->setPosition(ccp(838, 470));
                spr10->setPosition(ccp(812, 240));
                spr11->setPosition(ccp(1053, 565));
                spr12->setPosition(ccp(1010, 398));
                spr13->setPosition(ccp(1052, 248));
                spr14->setPosition(ccp(1268, 430));
                spr15->setPosition(ccp(1318, 245));
                
                pT1->setPosition(ccp(140, 600));
                pT2->setPosition(ccp(140, 365));
                pT3->setPosition(ccp(372, 615));
                pT4->setPosition(ccp(430, 400));
                pT5->setPosition(ccp(325, 280));
                pT6->setPosition(ccp(650, 620));
                pT7->setPosition(ccp(662, 445));
                pT8->setPosition(ccp(622, 320));
                pT9->setPosition(ccp(898, 605));
                pT10->setPosition(ccp(912, 375));
                pT11->setPosition(ccp(1198, 660));
                pT12->setPosition(ccp(1045, 533));
                pT13->setPosition(ccp(1142, 315));
                pT14->setPosition(ccp(1368, 550));
                pT15->setPosition(ccp(1393, 265));
                
                pP1->setPosition(180, 530);
                pP2->setPosition(185, 295);
                pP3->setPosition(425, 550);
                pP4->setPosition(485, 335);
                pP5->setPosition(376, 220);
                pP6->setPosition(700, 555);
                pP7->setPosition(722, 385);
                pP8->setPosition(675, 255);
                pP9->setPosition(928, 535);
                pP10->setPosition(962, 300);
                pP11->setPosition(1233, 595);
                pP12->setPosition(1100, 473);
                pP13->setPosition(1177, 253);
                pP14->setPosition(1423, 480);
                pP15->setPosition(1413, 225);
            }
                break;
            case 3:
            {
                // 按第三个副本的关卡位置排列
                pCon1->setPosition(115, 495);
                pCon2->setPosition(120, 260);
                pCon3->setPosition(310, 505);
                pCon4->setPosition(360, 250);
                pCon5->setPosition(542, 520);
                pCon6->setPosition(695, 380);
                pCon7->setPosition(730, 545);
                pCon8->setPosition(675, 230);
                pCon9->setPosition(915, 505);
                pCon10->setPosition(960, 250);
                pCon11->setPosition(1215, 650);
                pCon12->setPosition(1210, 255);
                pCon13->setPosition(1315, 515);
                pCon14->setPosition(1495, 250);
                pCon15->setPosition(1515, 455);

                spr1->setPosition(ccp(105, 475));
                spr2->setPosition(ccp(103, 241));
                spr3->setPosition(ccp(270, 418));
                spr4->setPosition(ccp(264, 233));
                spr5->setPosition(ccp(464, 437));
                spr6->setPosition(ccp(546, 266));
                spr7->setPosition(ccp(704, 507));
                spr8->setPosition(ccp(630, 227));
                spr9->setPosition(ccp(904, 463));
                spr10->setPosition(ccp(911, 232));
                spr11->setPosition(ccp(1092, 499));
                spr12->setPosition(ccp(1138, 242));
                spr13->setPosition(ccp(1281, 450));
                spr14->setPosition(ccp(1382, 245));
                spr15->setPosition(ccp(1390, 439));
                
                
                pT1->setPosition(ccp(115, 605));
                pT2->setPosition(ccp(115, 370));
                pT3->setPosition(ccp(300, 605));
                pT4->setPosition(ccp(360, 370));
                pT5->setPosition(ccp(510, 560));
                pT6->setPosition(ccp(670, 450));
                pT7->setPosition(ccp(725, 630));
                pT8->setPosition(ccp(740, 305));
                pT9->setPosition(ccp(910, 598));
                pT10->setPosition(ccp(966, 352));
                pT11->setPosition(ccp(1130, 590));
                pT12->setPosition(ccp(1209, 388));
                pT13->setPosition(ccp(1308, 560));
                pT14->setPosition(ccp(1409, 355));
                pT15->setPosition(ccp(1403, 610));
                
                pP1->setPosition(160, 540);
                pP2->setPosition(160, 306);
                pP3->setPosition(345, 540);
                pP4->setPosition(410, 306);
                pP5->setPosition(566, 495);
                pP6->setPosition(715, 380);
                pP7->setPosition(783, 562);
                pP8->setPosition(783, 242);
                pP9->setPosition(960, 528);
                pP10->setPosition(1016, 272);
                pP11->setPosition(1170, 519);
                pP12->setPosition(1259, 312);
                pP13->setPosition(1352, 485);
                pP14->setPosition(1469, 285);
                pP15->setPosition(1488, 560);
            }
                break;
            case 4:
            {
                // 按第四个副本的关卡位置排列
                pCon1->setPosition(125, 460);
                pCon2->setPosition(130, 250);
                pCon3->setPosition(305, 520);
                pCon4->setPosition(320, 330);
                pCon5->setPosition(525, 485);
                pCon6->setPosition(580, 255);
                pCon7->setPosition(820, 510);
                pCon8->setPosition(860, 245);
                pCon9->setPosition(1115, 555);
                pCon10->setPosition(1120, 255);
                pCon11->setPosition(1415, 455);
                pCon12->setPosition(1410, 280);

                spr1->setPosition(ccp(105, 439));
                spr2->setPosition(ccp(105, 242));
                spr3->setPosition(ccp(275, 485));
                spr4->setPosition(ccp(281, 245));
                spr5->setPosition(ccp(472, 386));
                spr6->setPosition(ccp(546, 242));
                spr7->setPosition(ccp(783, 381));
                spr8->setPosition(ccp(812, 240));
                spr9->setPosition(ccp(1044, 435));
                spr10->setPosition(ccp(1068, 240));
                spr11->setPosition(ccp(1273, 390));
                spr12->setPosition(ccp(1369, 246));
                
                
                pT1->setPosition(ccp(105, 600));
                pT2->setPosition(ccp(125, 352));
                pT3->setPosition(ccp(315, 605));
                pT4->setPosition(ccp(351, 395));
                pT5->setPosition(ccp(582, 595));
                pT6->setPosition(ccp(611, 327));
                pT7->setPosition(ccp(853, 595));
                pT8->setPosition(ccp(867, 330));
                pT9->setPosition(ccp(1164, 600));
                pT10->setPosition(ccp(1138, 345));
                pT11->setPosition(ccp(1393, 530));
                pT12->setPosition(ccp(1379, 356));
                
                pP1->setPosition(160, 530);
                pP2->setPosition(170, 282);
                pP3->setPosition(370, 535);
                pP4->setPosition(401, 330);
                pP5->setPosition(635, 516);
                pP6->setPosition(660, 260);
                pP7->setPosition(908, 516);
                pP8->setPosition(922, 260);
                pP9->setPosition(1220, 520);
                pP10->setPosition(1193, 270);
                pP11->setPosition(1443, 455);
                pP12->setPosition(1429, 281);
            }
                break;
            case 5:
            {
                // 按第五个副本的关卡位置排列
                pCon1->setPosition(120, 465);
                pCon2->setPosition(120, 255);
                pCon3->setPosition(295, 625);
                pCon4->setPosition(350, 255);
                pCon5->setPosition(585, 480);
                pCon6->setPosition(765, 335);
                pCon7->setPosition(775, 620);
                pCon8->setPosition(1030, 432);
                pCon9->setPosition(965, 260);
                pCon10->setPosition(1165, 535);
                pCon11->setPosition(1300, 395);
                pCon12->setPosition(1390, 535);
                pCon13->setPosition(1395, 325);
                pCon14->setPosition(1625, 625);
                pCon15->setPosition(1650, 295);
                pCon16->setPosition(1770, 590);
                pCon17->setPosition(1900, 255);
                pCon18->setPosition(2030, 420);
                pCon19->setPosition(2180, 290);
                pCon20->setPosition(2325, 465);
                pCon21->setPosition(2385, 295);

                spr1->setPosition(ccp(110, 379));
                spr2->setPosition(ccp(110, 243));
                spr3->setPosition(ccp(240, 415));
                spr4->setPosition(ccp(295, 251));
                spr5->setPosition(ccp(516, 385));
                spr6->setPosition(ccp(580, 245));
                spr7->setPosition(ccp(732, 479));
                spr8->setPosition(ccp(953, 422));
                spr9->setPosition(ccp(918, 250));
                spr10->setPosition(ccp(1133, 410));
                spr11->setPosition(ccp(1137, 248));
                spr12->setPosition(ccp(1369, 390));
                spr13->setPosition(ccp(1375, 241));
                spr14->setPosition(ccp(1565, 440));
                spr15->setPosition(ccp(1574, 244));
                spr16->setPosition(ccp(1749, 445));
                spr17->setPosition(ccp(1849, 246));
                spr18->setPosition(ccp(2004, 417));
                spr19->setPosition(ccp(2154, 248));
                spr20->setPosition(ccp(2184, 425));
                spr21->setPosition(ccp(2351, 263));
                
                pT1->setPosition(ccp(120, 580));
                pT2->setPosition(ccp(100, 353));
                pT3->setPosition(ccp(360, 580));
                pT4->setPosition(ccp(405, 351));
                pT5->setPosition(ccp(576, 585));
                pT6->setPosition(ccp(705, 355));
                pT7->setPosition(ccp(787, 629));
                pT8->setPosition(ccp(978, 567));
                pT9->setPosition(ccp(968, 350));
                pT10->setPosition(ccp(1173, 615));
                pT11->setPosition(ccp(1182, 353));
                pT12->setPosition(ccp(1409, 615));
                pT13->setPosition(ccp(1410, 331));
                pT14->setPosition(ccp(1615, 535));
                pT15->setPosition(ccp(1669, 344));
                pT16->setPosition(ccp(1839, 600));
                pT17->setPosition(ccp(1944, 351));
                pT18->setPosition(ccp(2044, 562));
                pT19->setPosition(ccp(2170, 372));
                pT20->setPosition(ccp(2320, 600));
                pT21->setPosition(ccp(2365, 340));
                
                pP1->setPosition(165, 500);
                pP2->setPosition(155, 280);
                pP3->setPosition(410, 505);
                pP4->setPosition(455, 280);
                pP5->setPosition(626, 505);
                pP6->setPosition(750, 280);
                pP7->setPosition(842, 560);
                pP8->setPosition(1000, 490);
                pP9->setPosition(1023, 280);
                pP10->setPosition(1228, 520);
                pP11->setPosition(1232, 280);
                pP12->setPosition(1465, 540);
                pP13->setPosition(1460, 260);
                pP14->setPosition(1670, 465);
                pP15->setPosition(1730, 270);
                pP16->setPosition(1890, 530);
                pP17->setPosition(2000, 266);
                pP18->setPosition(2090, 475);
                pP19->setPosition(2220, 295);
                pP20->setPosition(2359, 525);
                pP21->setPosition(2421, 280);
            }
                break;
            case 6:
            {
                // 按第六个副本的关卡位置排列
                pCon1->setPosition(120, 585);
                pCon2->setPosition(120, 248);
                pCon3->setPosition(315, 250);
                pCon4->setPosition(330, 415);
                pCon5->setPosition(485, 520);
                pCon6->setPosition(470, 425);
                pCon7->setPosition(750, 465);
                pCon8->setPosition(650, 245);
                pCon9->setPosition(860, 625);
                pCon10->setPosition(900, 260);
                pCon11->setPosition(1030, 585);
                pCon12->setPosition(1125, 255);
                pCon13->setPosition(1365, 255);
                pCon14->setPosition(1300, 420);
                pCon15->setPosition(1480, 550);

                spr1->setPosition(ccp(108, 455));
                spr2->setPosition(ccp(110, 238));
                spr3->setPosition(ccp(247, 238));
                spr4->setPosition(ccp(278, 401));
                spr5->setPosition(ccp(469, 499));
                spr6->setPosition(ccp(447, 248));
                spr7->setPosition(ccp(610, 439));
                spr8->setPosition(ccp(614, 240));
                spr9->setPosition(ccp(838, 470));
                spr10->setPosition(ccp(764, 242));
                spr11->setPosition(ccp(996, 485));
                spr12->setPosition(ccp(1086, 255));
                spr13->setPosition(ccp(1328, 249));
                spr14->setPosition(ccp(1183, 371));
                spr15->setPosition(ccp(1431, 412));
                
                pT1->setPosition(ccp(115, 615));
                pT2->setPosition(ccp(115, 380));
                pT3->setPosition(ccp(305, 343));
                pT4->setPosition(ccp(290, 536));
                pT5->setPosition(ccp(475, 630));
                pT6->setPosition(ccp(460, 448));
                pT7->setPosition(ccp(670, 575));
                pT8->setPosition(ccp(680, 325));
                pT9->setPosition(ccp(883, 535));
                pT10->setPosition(ccp(864, 402));
                pT11->setPosition(ccp(1075, 640));
                pT12->setPosition(ccp(1130, 400));
                pT13->setPosition(ccp(1398, 349));
                pT14->setPosition(ccp(1245, 555));
                pT15->setPosition(ccp(1441, 597));
                
                pP1->setPosition(165, 545);
                pP2->setPosition(170, 308);
                pP3->setPosition(347, 268);
                pP4->setPosition(343, 461);
                pP5->setPosition(520, 560);
                pP6->setPosition(517, 368);
                pP7->setPosition(725, 490);
                pP8->setPosition(734, 250);
                pP9->setPosition(933, 480);
                pP10->setPosition(920, 322);
                pP11->setPosition(1116, 570);
                pP12->setPosition(1176, 340);
                pP13->setPosition(1443, 280);
                pP14->setPosition(1293, 471);
                pP15->setPosition(1490, 517);
            }
                break;
            case 7:
            {
                // 按第七个副本的关卡位置排列
                pCon1->setPosition(115, 460);
                pCon2->setPosition(125, 265);
                pCon3->setPosition(340, 440);
                pCon4->setPosition(370, 245);
                pCon5->setPosition(480, 550);
                pCon6->setPosition(660, 245);
                pCon7->setPosition(730, 585);
                pCon8->setPosition(855, 460);
                pCon9->setPosition(1000, 650);
                pCon10->setPosition(1100, 380);
                pCon11->setPosition(895, 245);
                pCon12->setPosition(1305, 600);
                pCon13->setPosition(1260, 242);
                pCon14->setPosition(1475, 530);
                pCon15->setPosition(1460, 275);

                spr1->setPosition(ccp(110, 419));
                spr2->setPosition(ccp(115, 239));
                spr3->setPosition(ccp(258, 409));
                spr4->setPosition(ccp(267, 236));
                spr5->setPosition(ccp(423, 442));
                spr6->setPosition(ccp(562, 238));
                spr7->setPosition(ccp(639, 428));
                spr8->setPosition(ccp(805, 385));
                spr9->setPosition(ccp(980, 452));
                spr10->setPosition(ccp(1051, 246));
                spr11->setPosition(ccp(815, 235));
                spr12->setPosition(ccp(1217, 405));
                spr13->setPosition(ccp(1230, 242));
                spr14->setPosition(ccp(1422, 500));
                spr15->setPosition(ccp(1398, 251));
                
                pT1->setPosition(ccp(110, 555));
                pT2->setPosition(ccp(115, 345));
                pT3->setPosition(ccp(275, 555));
                pT4->setPosition(ccp(387, 345));
                pT5->setPosition(ccp(478, 587));
                pT6->setPosition(ccp(607, 373));
                pT7->setPosition(ccp(685, 533));
                pT8->setPosition(ccp(840, 560));
                pT9->setPosition(ccp(1050, 602));
                pT10->setPosition(ccp(1065, 415));
                pT11->setPosition(ccp(890, 335));
                pT12->setPosition(ccp(1250, 560));
                pT13->setPosition(ccp(1245, 332));
                pT14->setPosition(ccp(1440, 600));
                pT15->setPosition(ccp(1430, 420));
                
                pP1->setPosition(165, 475);
                pP2->setPosition(165, 265);
                pP3->setPosition(358, 479);
                pP4->setPosition(442, 265);
                pP5->setPosition(528, 502);
                pP6->setPosition(657, 290);
                pP7->setPosition(730, 450);
                pP8->setPosition(885, 485);
                pP9->setPosition(1100, 517);
                pP10->setPosition(1116, 345);
                pP11->setPosition(930, 255);
                pP12->setPosition(1297, 485);
                pP13->setPosition(1305, 260);
                pP14->setPosition(1480, 530);
                pP15->setPosition(1480, 345);
            }
                break;
            case 8:
            {
                // 按第八个副本的关卡位置排列ssss
                pCon1->setPosition(125, 545);
                pCon2->setPosition(120, 250);
                pCon3->setPosition(320, 291);
                pCon4->setPosition(470, 525);
                pCon5->setPosition(445, 380);
                pCon6->setPosition(660, 555);
                pCon7->setPosition(565, 250);
                pCon8->setPosition(815, 245);
                pCon9->setPosition(905, 525);
                pCon10->setPosition(1025, 360);
                pCon11->setPosition(1230, 555);
                pCon12->setPosition(1240, 255);
                pCon13->setPosition(1420, 247);
                pCon14->setPosition(1435, 540);
                pCon15->setPosition(1730, 485);
                pCon16->setPosition(1595, 605);
                pCon17->setPosition(1840, 400);
                pCon18->setPosition(1815, 615);
                pCon19->setPosition(2030, 265);
                pCon20->setPosition(2058, 570);
                pCon21->setPosition(2250, 550);
                pCon22->setPosition(2220, 350);
                pCon23->setPosition(2460, 265);
                pCon24->setPosition(2440, 450);

                spr1->setPosition(ccp(107, 475));
                spr2->setPosition(ccp(105, 242));
                spr3->setPosition(ccp(273, 291));
                spr4->setPosition(ccp(395, 460));
                spr5->setPosition(ccp(386, 249));
                spr6->setPosition(ccp(599, 427));
                spr7->setPosition(ccp(547,237));
                spr8->setPosition(ccp(751, 235));
                spr9->setPosition(ccp(900, 445));
                spr10->setPosition(ccp(933, 252));
                spr11->setPosition(ccp(1161, 430));
                spr12->setPosition(ccp(1190, 249));
                spr13->setPosition(ccp(1377, 247));
                spr14->setPosition(ccp(1388, 423));
                spr15->setPosition(ccp(1595, 247));
                spr16->setPosition(ccp(1560, 525));
                spr17->setPosition(ccp(1762, 248));
                spr18->setPosition(ccp(1773, 468));
                spr19->setPosition(ccp(1944, 250));
                spr20->setPosition(ccp(1993, 486));
                spr21->setPosition(ccp(2217, 530));
                spr22->setPosition(ccp(2187, 286));
                spr23->setPosition(ccp(2275, 255));
                spr24->setPosition(ccp(2330, 400));
                
                pT1->setPosition(ccp(147, 630));
                pT2->setPosition(ccp(105, 402));
                pT3->setPosition(ccp(288, 426));
                pT4->setPosition(ccp(445, 610));
                pT5->setPosition(ccp(420, 405));
                pT6->setPosition(ccp(682, 590));
                pT7->setPosition(ccp(599, 377));
                pT8->setPosition(ccp(826, 347));
                pT9->setPosition(ccp(975, 620));
                pT10->setPosition(ccp(1038, 395));
                pT11->setPosition(ccp(1231, 590));
                pT12->setPosition(ccp(1225, 349));
                pT13->setPosition(ccp(1417, 355));
                pT14->setPosition(ccp(1410, 590));
                pT15->setPosition(ccp(1595, 400));
                pT16->setPosition(ccp(1575, 645));
                pT17->setPosition(ccp(1800, 428));
                pT18->setPosition(ccp(1813, 608));
                pT19->setPosition(ccp(2004, 380));
                pT20->setPosition(ccp(2053, 620));
                pT21->setPosition(ccp(2247, 630));
                pT22->setPosition(ccp(2190, 406));
                pT23->setPosition(ccp(2345, 330));
                pT24->setPosition(ccp(2385, 505));
                
                pP1->setPosition(207, 540);
                pP2->setPosition(165, 317);
                pP3->setPosition(323, 321);
                pP4->setPosition(495, 525);
                pP5->setPosition(450, 295);
                pP6->setPosition(650, 300);
                pP7->setPosition(745, 507);
                pP8->setPosition(881, 265);
                pP9->setPosition(1025, 545);
                pP10->setPosition(1090, 315);
                pP11->setPosition(1280, 505);
                pP12->setPosition(1285, 280);
                pP13->setPosition(1470, 280);
                pP14->setPosition(1470, 510);
                pP15->setPosition(1650, 317);
                pP16->setPosition(1625, 575);
                pP17->setPosition(1852, 353);
                pP18->setPosition(1880, 540);
                pP19->setPosition(2070, 285);
                pP20->setPosition(2108, 530);
                pP21->setPosition(2305, 555);
                pP22->setPosition(2247, 326);
                pP23->setPosition(2395, 255);
                pP24->setPosition(2435, 425);
            }
                break;
            default:
                break;
        }
    }
    this->getChildByTag(CONTAINER_TAG)->addChild(pNode, 1, MENU_TAG);
}

void ZhuLuLayer::chosCopy(CCObject* pSender) {
    CCMenuItemImage* pCCMenuItemImage = (CCMenuItemImage*)pSender;
    int copyId = pCCMenuItemImage->getTag() - COPY_BTN_TAG;
    int enterLev = CopyManager::getInstance()->getCopyData(copyId)->getEl();/* 进入等级 */
    int playerLev = GamePlayer::getInstance()->getLevel();/*玩家等级*/
    if (playerLev < enterLev || ((copyId > GameState::getInstance()->getNormalMaxCopyId())))
        return;
    
    if (!m_isMvoing) {
        CCFiniteTimeAction * seq= CCSequence::create(CCScaleTo::create(0.2, 1.05),CCScaleTo::create(0.1, 1),
                                                     CCCallFuncND::create(this, callfuncND_selector(ZhuLuLayer::copyCallback), (void*)copyId), NULL);
        CCSprite* sp = (CCSprite*)m_scrollView->getContainer()->getChildByTag(MAPNODE)->getChildByTag(COPY_NODE + copyId - 1)->
        getChildByTag(COPY_MAP_TAG + copyId - 1);
        if (sp) {
            sp->setZOrder(50);
            sp->runAction(seq);
        }
    }
}

void ZhuLuLayer::copyCallback(CCNode* pNode, void* pCopyId) {
    int copyId = reinterpret_cast<int>(pCopyId);
    GameState::getInstance()->setCopyId(copyId);
    update(1);
}

void ZhuLuLayer::chosLev(CCObject* pSender) {
    CCMenuItemImage* pCCMenuItemImage = (CCMenuItemImage*)pSender;
    int levId = pCCMenuItemImage->getTag() - LEVEL_BTN_TAG;
    int nextPve = 0;
    int maxPve = GameState::getInstance()->getNormalMaxLevId();
    LevelDataItem* pNextItem = CopyManager::getInstance()->getCopyData(GameState::getInstance()->getNormalMaxCopyId())
    ->getLevelDataById(maxPve);
    if (pNextItem) {
        nextPve = pNextItem->getNi();
        /* 0当前副本最大关卡 */
        if (nextPve == 0) {
        } else {
            if (levId >= nextPve) {
                return;
            }
        }
    }
    
    if (!m_isMvoing) {
        CCFiniteTimeAction * seq= CCSequence::create(CCScaleTo::create(0.2, 1.05),CCScaleTo::create(0.1, 1),
                                                     CCCallFuncND::create(this, callfuncND_selector(ZhuLuLayer::levCallback), (void*)levId), NULL);
        CCSprite* sp = (CCSprite*)m_scrollView->getContainer()->getChildByTag(LITMAPNODE)->getChildByTag(LEV_MAP_TAG + levId % 100);
        if (sp) {
            sp->setZOrder(50);
            sp->runAction(seq);
        }
    }
}

void ZhuLuLayer::levCallback(CCNode* pNode, void* pLevId) {
    int levId = reinterpret_cast<int>(pLevId);
    GameState* pGameState = GameState::getInstance();
    GameState::getInstance()->setLevId(levId);
    /*当前体力*/
    int curEnergy = GamePlayer::getInstance()->getEnergy();
    LevelDataItem* pItem = CopyManager::getInstance()->getCopyData(pGameState->getCopyId())
    ->getLevelDataById(pGameState->getLevId());
    /*进入体力*/
    int enterEnergy = pItem->getEn();
    if (curEnergy < enterEnergy) {
        char buffer[128];
        const char* title = "购买体力";
        sprintf(buffer, "你是否花费%d元宝购买体力                  还能购买%d次", GamePlayer::getInstance()->getEnergyPrice(),
                GamePlayer::getInstance()->getEnergyBuy());
        MessageBox::getInstance()->messageForBox(title , 30, buffer, 25, this, callfunc_selector(ZhuLuLayer::_buyEnergy));
        return;
    }
    CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_CHOSFORM_STRING);
}

// 关卡难度筛选, 默认普通难度
void ZhuLuLayer::filterCallback(CCObject* pSender) {
    CCSprite* sp = NULL;
    CCNode* p = this->getChildByTag(CONTAINER_TAG)->getChildByTag(MENU_TAG);
    sp = (CCSprite*)p->getChildByTag(FILTER_NORMAL);
    sp->setVisible(false);
    sp = (CCSprite*)p->getChildByTag(FILTER_DIFFIC);
    sp->setVisible(false);
    sp = (CCSprite*)p->getChildByTag(FILTER_HELL);
    sp->setVisible(false);
    sp = (CCSprite*)p->getChildByTag(FILTER_ACTIVI);
    sp->setVisible(false);
    
    CCMenuItemImage* pCCMenuItemImage = (CCMenuItemImage*)pSender;
    switch (pCCMenuItemImage->getTag()) {
        case NORMAL:
        {
            m_diffcult = 0;/*普通难度*/
            sp = (CCSprite*)p->getChildByTag(FILTER_NORMAL);
            sp->setVisible(true);
        }
            break;
        case DIFFIC:
        {
            m_diffcult = 1;/*精英难度*/
            sp = (CCSprite*)p->getChildByTag(FILTER_DIFFIC);
            sp->setVisible(true);
        }
            break;
        case HELL:
        {
            m_diffcult = 2;/*地狱难度*/
            sp = (CCSprite*)p->getChildByTag(FILTER_HELL);
            sp->setVisible(true);
        }
            break;
        case ACTIVI:
        {
            m_diffcult = 3;/*活动难度*/
            sp = (CCSprite*)p->getChildByTag(FILTER_ACTIVI);
            sp->setVisible(true);
        }
            break;
        default:
            CCAssert(false, "invalid filter");
            break;
    }
}

void ZhuLuLayer::initPosition() {
    CCNode* pNode = (CCNode*)this->getChildByTag(CONTAINER_TAG);
    pNode->setPosition(ccp(0, -LAYER_OFFSET));
}

/*
 *  将能响应 touch 事件的 ui 组件移动到屏幕外面
 */
void ZhuLuLayer::moveOutTouchUICompent() {
    /*移动tipMenu*/
    tipMenu(false);
    
    CCNode* pNode = (CCNode*)this->getChildByTag(CONTAINER_TAG);
    
    if (pNode->getActionByTag(RESTORE_ACTION_TAG)) {
        pNode->stopAllActions();
        pNode->setPosition(ccp(0, 0));
        
        CCSequence* pMoveOutAction = CCSequence::create(CCMoveTo::create(OPENNING_ANI_DURING, ccp(0, -LAYER_OFFSET)),
                                                        CCCallFunc::create(this, callfunc_selector(ZhuLuLayer::moveOutCallback)),NULL);
        pMoveOutAction->setTag(MOVEOUT_ACTION_TAG);
        pNode->runAction(pMoveOutAction);
        return;
    }
    
    if (!pNode->getActionByTag(MOVEOUT_ACTION_TAG)) {
        CCSequence* pMoveOutAction = CCSequence::create(CCMoveTo::create(OPENNING_ANI_DURING, ccp(0, -LAYER_OFFSET)),
                                                        CCCallFunc::create(this, callfunc_selector(ZhuLuLayer::moveOutCallback)),NULL);
        pMoveOutAction->setTag(MOVEOUT_ACTION_TAG);
        pNode->runAction(pMoveOutAction);
    }
    
    m_enableTouch = false;
    
}

void ZhuLuLayer::moveOutCallback() {
    if (m_isRemoveSelf) {
        this->removeFromParent();
        CCLog("ZhuLuLayer 从主场景移除");
    }
}

/*
 *  恢复能响应 touch 事件的 ui 组件的位置
 */
void ZhuLuLayer::restoreTouchUICompent() {
    /*恢复可见*/
    this->setVisible(true);
    m_enableTouch = true;
    
    if (m_isUpdateOne)
        tipMenu(true);
    
    CCNode* pNode = (CCNode*)this->getChildByTag(CONTAINER_TAG);
    
    if (pNode->getActionByTag(MOVEOUT_ACTION_TAG)) {
        pNode->stopAllActions();
        pNode->setPosition(ccp(0, -LAYER_OFFSET));
        CCMoveTo* pMoveTo = CCMoveTo::create(OPENNING_ANI_DURING, ccp(0, 0));
        pMoveTo->setTag(RESTORE_ACTION_TAG);
        pNode->runAction(pMoveTo);
        return;
    }
    
    if (!pNode->getActionByTag(RESTORE_ACTION_TAG)) {
        CCMoveTo* pMoveTo = CCMoveTo::create(OPENNING_ANI_DURING, ccp(0, 0));
        pMoveTo->setTag(RESTORE_ACTION_TAG);
        pNode->runAction(pMoveTo);
    }
}

void ZhuLuLayer::option() {
    update(0);
}

void ZhuLuLayer::optionQie() {
    update(0);
    if (m_diffcult == 0) {
        m_diffcult = 1;
    } else if (m_diffcult == 1) {
        m_diffcult = 0;
    }
}

/***************************************
 页面滚动效果, 这是整个层的滚动效果
 ***************************************/
void ZhuLuLayer::scrollViewDidScroll(CCScrollView* view) {
//    m_isMvoing = true;
}

void ZhuLuLayer::scrollViewDidZoom(CCScrollView* view) {
    
}

bool ZhuLuLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    if (!m_enableTouch) {
        return false;
    }
    
    float x = pTouch->getLocation().x;
    float y = pTouch->getLocation().y;
    /* 放大背景地图 */
    
    /* 如果内容小于640则不滑动 */
    if (m_scrollView->getContentSize().width <= 640) {
        return false;
    }
    if (y < 160.0f || y > 640.0f) {
        return false;
    }
    struct timeval tv;
    gettimeofday(&tv,NULL);
    long nowTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    m_lastTime = (float)(nowTime)/1000;
    m_scrollView->stopAllActions();
    m_lastPos = ccp(x, y);
    m_xOffset = m_scrollView->getPositionX();
    m_tempPos = x;
    return true;
}

void ZhuLuLayer::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    float x = pTouch->getLocation().x;
    if ((x - m_lastPos.x > 10) || (x - m_lastPos.x) < -10 ) {
        m_isMvoing = true;
    }

    float maxX = m_scrollView->getContentSize().width;
    m_xOffset += (x - m_tempPos);
    if (m_xOffset > SCROLLVIEW_OFFSETX + BOUNCE_LIMIT) {
       m_xOffset = SCROLLVIEW_OFFSETX + BOUNCE_LIMIT ;
    }
    if (m_xOffset < -maxX + 640 - BOUNCE_LIMIT) {
       m_xOffset = -maxX + 640 - BOUNCE_LIMIT;
    }
    m_scrollView->setPosition(ccp(m_xOffset, m_scrollView->getPositionY()));
    m_tempPos = x;
}

void ZhuLuLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    _inertiaScroll(pTouch, pEvent);
    m_tempPos = 0;
    m_lastTime = 0;
    //    m_lastPos = ccp(0, 0);
}

void ZhuLuLayer::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    _autoScroll();
    m_lastTime = 0;
}

void ZhuLuLayer::_inertiaScroll(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    float x = pTouch->getLocation().x;
    float maxX = m_scrollView->getContentSize().width;
    int value = 0.0f;
    struct timeval tv;
    gettimeofday(&tv,NULL);
    long tt = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    float nowTime = (float)(tt / 1000);
    if (fabs(nowTime - m_lastTime) < 0.8 && fabs(nowTime - m_lastTime) > 0.1) {
        value = (int)(((x - m_lastPos.x)/fabs(nowTime - m_lastTime)) / 5);
    }
    m_xOffset += value;
    if (m_xOffset > SCROLLVIEW_OFFSETX) {
        m_xOffset = SCROLLVIEW_OFFSETX;
        //        m_scrollView->setPosition(ccp(0, m_scrollView->getPositionY()));
    }else if (m_xOffset < -maxX + 640) {
        m_xOffset = -maxX + 640;
        //        m_scrollView->setPosition(ccp(- 975, m_scrollView->getPositionY()));
    }
    m_scrollView->runAction(CCSequence::create(CCEaseExponentialOut::create(CCMoveTo::create(0.3f, CCPointMake(m_xOffset, m_scrollView->getPositionY()))), CCCallFunc::create(this, callfunc_selector(ZhuLuLayer::_autoScroll)), NULL));
}

void ZhuLuLayer::_autoScroll() {
    m_isMvoing = false;
    float maxX = m_scrollView->getContentSize().width;
    if (m_xOffset > SCROLLVIEW_OFFSETX) {
        m_scrollView->setPosition(ccp(SCROLLVIEW_OFFSETX, m_scrollView->getPositionY()));
    } else if (m_xOffset < -maxX + 640) {
        m_scrollView->setPosition(ccp(-maxX+640, m_scrollView->getPositionY()));
    }
//    CCLog("_autoScroll");
}

void ZhuLuLayer::onEnter() {
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, false);
}

void ZhuLuLayer::onExit() {
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}


void ZhuLuLayer::tipMenu(const bool isMoveIn) {
    if (isMoveIn)/*移入*/ {
        CCNode* pL = (CCNode*)this->getChildByTag(LL_MENU_TAG);
        CCNode* pM = (CCNode*)this->getChildByTag(MID_MENU_TAG);
        CCNode* pR = (CCNode*)this->getChildByTag(RR_MENU_TAG);
        if (pL) {
            pL->setPosition(ccp(-150.0f, 0.0f));
            pL->runAction(CCMoveTo::create(OPENNING_ANI_DURING, ccp(0, 0)));
        }
        if (pM) {
            pM->setPosition(ccp(0.0f, 150.0f));
            pM->runAction(CCMoveTo::create(OPENNING_ANI_DURING, ccp(0, 0)));
        }
        if (pR) {
            pR->setPosition(ccp(150.0f, 0.0f));
            pR->runAction(CCMoveTo::create(OPENNING_ANI_DURING, ccp(0, 0)));
        }
    } else {
        CCNode* pL = (CCNode*)this->getChildByTag(LL_MENU_TAG);
        CCNode* pM = (CCNode*)this->getChildByTag(MID_MENU_TAG);
        CCNode* pR = (CCNode*)this->getChildByTag(RR_MENU_TAG);
        int dir;
        if (pL) {
            dir = 0;
            pL->setPosition(ccp(0, 0));
            pL->runAction(CCSequence::create(CCMoveTo::create(OPENNING_ANI_DURING, ccp(-150.0f, 0.0f)), CCCallFuncND::create(this, callfuncND_selector(ZhuLuLayer::_callbackForTipMenu), (void*)dir), NULL));
        }
        if (pM) {
            dir = 1;
            pM->setPosition(ccp(0, 0));
            pM->runAction(CCSequence::create(CCMoveTo::create(OPENNING_ANI_DURING, ccp(0.0f, 150.0f)), CCCallFuncND::create(this, callfuncND_selector(ZhuLuLayer::_callbackForTipMenu), (void*)dir), NULL));
        }
        if (pR) {
            dir = 2;
            pR->setPosition(ccp(0, 0));
            pR->runAction(CCSequence::create(CCMoveTo::create(OPENNING_ANI_DURING, ccp(150.0f, 0.0f)), CCCallFuncND::create(this, callfuncND_selector(ZhuLuLayer::_callbackForTipMenu), (void*)dir), NULL));
        }
    }
}

void ZhuLuLayer::_callbackForTipMenu(CCNode* pNode, void* pDir) {
    int dir = reinterpret_cast<int>(pDir);
    if (dir == 0) {
        if (this->getChildByTag(LL_MENU_TAG)) {
            this->removeChildByTag(LL_MENU_TAG);
        }
    } else if (dir == 1) {
        if (this->getChildByTag(MID_MENU_TAG)) {
            this->removeChildByTag(MID_MENU_TAG);
        }
    } else if (dir == 2) {
        if (this->getChildByTag(RR_MENU_TAG)) {
            this->removeChildByTag(RR_MENU_TAG);
        }
    }
}

void ZhuLuLayer::setVisible(bool visible) {
    CCLayer::setVisible(visible);
    if (visible && m_isUpdateOne) {
//        tipMenu(true);
        update(1);
    }
}

void ZhuLuLayer::_buyEnergy() {
    NetConnection* pNetConnection = NetConnection::getInstance();
    char szPostBuffer[MAX_POSTTO_SERVER_BUFFER_SIZE];
    memset(szPostBuffer, '\0', MAX_POSTTO_SERVER_BUFFER_SIZE);
    strcpy(szPostBuffer, "c=user&a=addEnergy");
    char szHttpTag[] = HTTP_REQUEST_BUYENERGY_TAG;
    strcat(szPostBuffer, "&uid=");
    strcat(szPostBuffer, GamePlayer::getInstance()->getUid().c_str());
    CCLog("post 购买 = %s", szPostBuffer);
    pNetConnection->commitPostRequestData(szPostBuffer, szHttpTag);
}