//
//  HerosLayer.cpp
//  hero
//
//  Created by yangjie on 2013/11/28
//
//

#include "HerosLayer.h"
#include "HeroItem.h"
#include "HeroOfPlayerItem.h"
#include "HeroManager.h"
#include "GamePlayer.h"
#include "customMessage.h"
#include "HeroItemNode.h"
#include "ChosConfirm.h"
#include "GameState.h"
#include "HeroAdditionItem.h"
#include "Battle.h"


#define OPENNING_ANI_DURING                 0.3f
#define ANIMATION_DURING                    0.2f

// 筛选武将按钮背景图片离 midImage 偏移量
#define FILTER_HERO_BTN_BG_OFFSET           70.0f

// 筛选武将按钮离 midImage 偏移量
#define FILTER_HERO_BTN_OFFSET              60.0f
// 筛选武将按钮之间的间隔
#define FILTER_HERO_BTN_SPACEING            104.0f
// 筛选武将按钮背景图片和屏幕左边偏移量
#define FILTER_HERO_BACK_LEFT_OFFSET        8.0f
// 筛选武将按钮的偏移量
#define FILTER_HERO_BUTTON_TOP_OFFSET       35.0f

// 武将列表离顶部偏移量
#define HEROLIST_TOP_OFFSET                 340.0f
// 武将列表项右半部分位置偏移量
#define RIGHT_ITEM_OFFSET                   307.0f
#define LEFT_ITEM_OFFSET                    6

// 武将列表显示的行数
#define HEROLIST_DISPLAY_ROWS               3
// 武将列表每页最多显示行数
#define HEROSLIST_MAX_ROWS_PER_PAGE         5

// 武将列表 ui 组件离屏幕左右边界的间隔大小
#define HEROLIST_CELL_PADDING               14.0f
// 武将单元格高度
#define HEROLIST_CELL_HEIGHT                172
// 缺省武将头像
#define HERO_DEFAULT_HEAD_IMAGE             "default.png"

// 翻页动画持续时间
#define TURNPAGE_ANI_DURING                 0.3

// 点击选择左边武将的左边界
#define TOUCH_LEFT_HERO_BOUND_LEFT          170.0f
// 点击选择左边武将的右边界
#define TOUCH_LEFT_HERO_BOUND_RIGHT         310.0f
// 点击选择右边武将的左边界
#define TOUCH_RIGHT_HERO_BOUND_LEFT         470.0f
// 点击选择右边武将的右边界
#define TOUCH_RIGHT_HERO_BOUND_RIGHT        610.0f
// 武将图片宽度
#define HERO_CONTENT_X                      130.0f

//////////////////////////////////////////////
//                  tag                     //
//////////////////////////////////////////////

// 筛选武将按钮 tag
#define ALL_HERO_BTN_TAG                    1111        // 所有武将
#define WEI_HERO_BTN_TAG                    1112        // 魏国武将
#define SHU_HERO_BTN_TAG                    1113        // 蜀国武将
#define WU_HERO_BTN_TAG                     1115        // 吴国武将
#define QX_HERO_BTN_TAG                     1116        // 群雄
#define OTHER_HERO_BTN_TAG                  1117        // 其他

#define RETURN_MENU_TAG                     1201        // 返回菜单按钮
#define LL_IMAGE_TAG                        1202        //

#define L_TIPMENU_TAG                      1203        // tipMenu左菜单
#define M_TIPMENU_TAG                      1204        // tipMenu中间菜单
#define R_TIPMENU_TAG                      1205        // tipMenu右菜单

#define RETURNMENU_TAG                      1210        // 列表返回


#define HEROS_TABLEVIEW1_TAG                1301        // 显示武将列表1
#define HEROS_TABLEVIEW2_TAG                1302        // 显示武将列表2

#define FONT                                "Arial"     // 字体

#define LEFT_HERO_NODE_TAG                  1350        // 左边英雄节点
#define RIGHT_HERO_NODE_TAG                 1351        // 右边英雄节点

#define PAGE_TAG                            3001        // 当前页数
#define HERO_COUNT_TAG                      3002        // 拥有武将

#define LEFT_TIP_TAG                        4001        // 翻页左提示
#define RIGHT_TIP_TAG                       4002        // 翻页右提示

#define BTNALL_TAG                          5000        // 武将筛选按钮高亮--全部
#define BTNWEI_TAG                          5001
#define BTNSHU_TAG                          5002
#define BTNWU_TAG                           5003
#define BTNQUN_TAG                          5004
#define BTNOTHER_TAG                        5005
#define BTNALL_TAG_B                        5006        // 武将筛选按钮高亮--全部
#define BTNWEI_TAG_B                        5007
#define BTNSHU_TAG_B                        5008
#define BTNWU_TAG_B                         5009
#define BTNQUN_TAG_B                        5010
#define BTNOTHER_TAG_B                      5011

#define MOVEOUT_TAG                         5100
#define MOVEIN_TAG                          5101

#define CONTAINER_TAG                       10000

/*
 *   构造函数
 */
HerosLayer::HerosLayer():
m_isTurnPage(false)
,m_isSelectHeroMode(false)
,m_countOfSelectedHeros(0)
,m_isExceptLock(false)
,m_isExceptGoIntoBattle(false)
,m_pGameState(GameState::getInstance())
,m_pHeroManager(HeroManager::getInstance())
{}

/*
 *   析构函数
 */
HerosLayer::~HerosLayer() {}

/*
 *   “武将” 层初始化
 */
bool HerosLayer::init() {
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() ) {
        return false;
    }
    
    this->setTouchEnabled(true);
    
    m_pHeroManager->setCamp(CAMP_ALL, m_isExceptLock, m_isExceptGoIntoBattle);
    m_pHeroManager->updateCamp();
    
    /*
     *  选择武将模式相关数据
     */
    memset(m_szSelectedHeroUniId, '\0', sizeof(m_szSelectedHeroUniId));
    memset(m_szNotAllowedHeroUniId, '\0', sizeof(m_szNotAllowedHeroUniId));
    m_countOfSelectedHeros = 0;
    m_countOfNotAllowed = 0;
    
    /*层容器*/
    CCNode* pNode = CCNode::create();
    addChild(pNode, 1, CONTAINER_TAG);
    
    /*
     *  初始化 UI 组件
     */
    _initUiComponent();
    
    return true;
}

/*
 *   初始化 UI 组件
*/
void HerosLayer::_initUiComponent() {

    CCNode* pParent = (CCNode*)this->getChildByTag(CONTAINER_TAG);
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    const float midImageTopOffset = m_pGameState->getBottomOffset() + 745.0f;
    float updownOffset = 0.0f;
    
    this->setTouchEnabled(true);
    setTouchPriority(-9999);
    
    //左右翻页提示 和 ***没有适配的 y 坐标
    CCSprite* tipleft = CCSprite::create("HL_tipl.png");
    tipleft->setAnchorPoint(CCPointZero);
    tipleft->setPosition(ccp(2, 400.0f + m_pGameState->getBottomOffset()));
    pParent->addChild(tipleft, 10, LEFT_TIP_TAG);
    CCSprite* tipright = CCSprite::create("HL_tipr.png");
    tipright->setAnchorPoint(CCPointZero);
    tipright->setPosition(ccp(618.0f, 400.0f + m_pGameState->getBottomOffset()));
    pParent->addChild(tipright, 10, RIGHT_TIP_TAG);
    if (m_pHeroManager->getCampHerosOfPlayerCount() < HEROSLIST_MAX_ROWS_PER_PAGE * 2) {
        tipleft->setVisible(false);
        tipright->setVisible(false);
    }
    //页数显示
    //显示当前页数
    char buffer[64];
    int allPages = (int)((m_pHeroManager->getCampHerosOfPlayerCount() -1 ) / (HEROSLIST_MAX_ROWS_PER_PAGE * 2)) + 1;
    sprintf(buffer, "1/%d", allPages);
    CCLabelTTF * cur_page = CCLabelTTF::create(buffer, FONT, 20);
    cur_page->setAnchorPoint(CCPointZero);
    cur_page->setPosition(ccp(35, 127.0f + m_pGameState->getBottomOffset()));
    pParent->addChild(cur_page, 10, PAGE_TAG);
    
    CCLabelTTF* had_heros = CCLabelTTF::create("", FONT, 19);
    had_heros->setAnchorPoint(CCPointZero);
    had_heros->setPosition(ccp(465.0f, 127.0f + m_pGameState->getBottomOffset()));
    pParent->addChild(had_heros, 10, HERO_COUNT_TAG);
    
    // 筛选武将按钮背景框
    CCSprite *pFilterHeroBtnBgFrame = CCSprite::create("kuan.png");
    pFilterHeroBtnBgFrame->setAnchorPoint(CCPointZero);
    updownOffset = midImageTopOffset - FILTER_HERO_BTN_BG_OFFSET;
    pFilterHeroBtnBgFrame->setPosition(ccp(0.0f, updownOffset));
    pParent->addChild(pFilterHeroBtnBgFrame, 2);
    
    // 底部框
    CCSprite *pBottomFrame = CCSprite::create("bottomOfHero.png");
    pBottomFrame->setAnchorPoint(CCPointZero);
    pBottomFrame->setPosition(ccp(0.0f, m_pGameState->getBottomOffset() + 123.0f));
    pParent->addChild(pBottomFrame, 3);
    
    // 背景
    CCSprite *pBg = CCSprite::create("bgOfHero.png");
    pBg->setAnchorPoint(CCPointZero);
    updownOffset = midImageTopOffset - 660.0f;
    pBg->setPosition(ccp(0.0f, updownOffset + 10));
    pParent->addChild(pBg, 1);
    
    // 筛选武将按钮背景图片
    updownOffset = midImageTopOffset - FILTER_HERO_BTN_OFFSET + 6.0f;
    CCSprite *pFilterHeroBtnBg1 = CCSprite::create("b1.png");
    pFilterHeroBtnBg1->setAnchorPoint(CCPointZero);
    pFilterHeroBtnBg1->setPosition(ccp(FILTER_HERO_BACK_LEFT_OFFSET,
                                       updownOffset));
    pParent->addChild(pFilterHeroBtnBg1, 3);
    CCSprite *pFilterHeroBtnBg2 = CCSprite::create("b1.png");
    pFilterHeroBtnBg2->setAnchorPoint(CCPointZero);
    pFilterHeroBtnBg2->setPosition(ccp(FILTER_HERO_BACK_LEFT_OFFSET + FILTER_HERO_BTN_SPACEING,
                                       updownOffset));
    pParent->addChild(pFilterHeroBtnBg2, 3);
    CCSprite *pFilterHeroBtnBg3 = CCSprite::create("b1.png");
    pFilterHeroBtnBg3->setAnchorPoint(CCPointZero);
    pFilterHeroBtnBg3->setPosition(ccp(FILTER_HERO_BACK_LEFT_OFFSET + FILTER_HERO_BTN_SPACEING * 2,
                                       updownOffset));
    pParent->addChild(pFilterHeroBtnBg3, 3);
    CCSprite *pFilterHeroBtnBg4 = CCSprite::create("b1.png");
    pFilterHeroBtnBg4->setAnchorPoint(CCPointZero);
    pFilterHeroBtnBg4->setPosition(ccp(FILTER_HERO_BACK_LEFT_OFFSET + FILTER_HERO_BTN_SPACEING * 3,
                                       updownOffset));
    pParent->addChild(pFilterHeroBtnBg4, 3);
    CCSprite *pFilterHeroBtnBg5 = CCSprite::create("b1.png");
    pFilterHeroBtnBg5->setAnchorPoint(CCPointZero);
    pFilterHeroBtnBg5->setPosition(ccp(FILTER_HERO_BACK_LEFT_OFFSET + FILTER_HERO_BTN_SPACEING * 4,
                                       updownOffset));
    pParent->addChild(pFilterHeroBtnBg5, 3);
    CCSprite *pFilterHeroBtnBg6 = CCSprite::create("b1.png");
    pFilterHeroBtnBg6->setAnchorPoint(CCPointZero);
    pFilterHeroBtnBg6->setPosition(ccp(FILTER_HERO_BACK_LEFT_OFFSET + FILTER_HERO_BTN_SPACEING * 5,
                                       updownOffset));
    pParent->addChild(pFilterHeroBtnBg6, 3);

    CCSprite *pFilterHeroBtnBg1_b = CCSprite::create("b2.png");
    pFilterHeroBtnBg1_b->setAnchorPoint(CCPointZero);
    pFilterHeroBtnBg1_b->setPosition(ccp(FILTER_HERO_BACK_LEFT_OFFSET,
                                       updownOffset));
    pParent->addChild(pFilterHeroBtnBg1_b, 3, BTNALL_TAG_B);
    pFilterHeroBtnBg1_b->setVisible(true);
    CCSprite *pFilterHeroBtnBg2_b = CCSprite::create("b2.png");
    pFilterHeroBtnBg2_b->setAnchorPoint(CCPointZero);
    pFilterHeroBtnBg2_b->setPosition(ccp(FILTER_HERO_BACK_LEFT_OFFSET + FILTER_HERO_BTN_SPACEING,
                                       updownOffset));
    pParent->addChild(pFilterHeroBtnBg2_b, 3, BTNWEI_TAG_B);
    pFilterHeroBtnBg2_b->setVisible(false);
    CCSprite *pFilterHeroBtnBg3_b = CCSprite::create("b2.png");
    pFilterHeroBtnBg3_b->setAnchorPoint(CCPointZero);
    pFilterHeroBtnBg3_b->setPosition(ccp(FILTER_HERO_BACK_LEFT_OFFSET + FILTER_HERO_BTN_SPACEING * 2,
                                       updownOffset));
    pParent->addChild(pFilterHeroBtnBg3_b, 3, BTNSHU_TAG_B);
    pFilterHeroBtnBg3_b->setVisible(false);
    CCSprite *pFilterHeroBtnBg4_b = CCSprite::create("b2.png");
    pFilterHeroBtnBg4_b->setAnchorPoint(CCPointZero);
    pFilterHeroBtnBg4_b->setPosition(ccp(FILTER_HERO_BACK_LEFT_OFFSET + FILTER_HERO_BTN_SPACEING * 3,
                                       updownOffset));
    pParent->addChild(pFilterHeroBtnBg4_b, 3, BTNWU_TAG_B);
    pFilterHeroBtnBg4_b->setVisible(false);
    CCSprite *pFilterHeroBtnBg5_b = CCSprite::create("b2.png");
    pFilterHeroBtnBg5_b->setAnchorPoint(CCPointZero);
    pFilterHeroBtnBg5_b->setPosition(ccp(FILTER_HERO_BACK_LEFT_OFFSET + FILTER_HERO_BTN_SPACEING * 4,
                                       updownOffset));
    pParent->addChild(pFilterHeroBtnBg5_b, 3, BTNQUN_TAG_B);
    pFilterHeroBtnBg5_b->setVisible(false);
    CCSprite *pFilterHeroBtnBg6_b = CCSprite::create("b2.png");
    pFilterHeroBtnBg6_b->setAnchorPoint(CCPointZero);
    pFilterHeroBtnBg6_b->setPosition(ccp(FILTER_HERO_BACK_LEFT_OFFSET + FILTER_HERO_BTN_SPACEING * 5,
                                       updownOffset));
    pParent->addChild(pFilterHeroBtnBg6_b, 3, BTNOTHER_TAG_B);
    pFilterHeroBtnBg6_b->setVisible(false);

    
    updownOffset = midImageTopOffset - FILTER_HERO_BUTTON_TOP_OFFSET;
    // 筛选全部武将按钮 (menuitem)
    CCMenuItemImage *pAllHeroFilterItem = CCMenuItemImage::create("HL_all.png",
                                                                  "HL_all.png",
                                                                  this,
                                                                  menu_selector(HerosLayer::filterHerosCallback));
    pAllHeroFilterItem->setTag(ALL_HERO_BTN_TAG);
    CCMenu* pAllFilterHeroMenu = CCMenu::create(pAllHeroFilterItem, NULL);
    pAllFilterHeroMenu->setPosition(ccp(FILTER_HERO_BACK_LEFT_OFFSET + 53.0f, updownOffset + 5.0f));
    pParent->addChild(pAllFilterHeroMenu, 4, ALL_HERO_BTN_TAG);
    
    // 筛选魏国武将按钮 (menuitem)
    CCMenuItemImage *pWeiHeroFilterItem = CCMenuItemImage::create("HL_wei.png",
                                                                  "HL_wei.png",
                                                                  "HL_weis.png",
                                                                  this,
                                                                  menu_selector(HerosLayer::filterHerosCallback));
    pWeiHeroFilterItem->setTag(WEI_HERO_BTN_TAG);
    CCMenu* pWeiFilterHeroMenu = CCMenu::create(pWeiHeroFilterItem, NULL);
    pWeiFilterHeroMenu->setPosition(ccp(FILTER_HERO_BACK_LEFT_OFFSET + 155.0f, updownOffset + 5.0f));
    pParent->addChild(pWeiFilterHeroMenu, 4, WEI_HERO_BTN_TAG);
    m_HeroMenuItems[CAMP_WEI] = pWeiHeroFilterItem;

    // 筛选蜀国武将按钮 (menuitem)
    CCMenuItemImage *pShuHeroFilterItem = CCMenuItemImage::create("HL_shu.png",
                                                                  "HL_shu.png",
                                                                  "HL_shus.png",
                                                                  this,
                                                                  menu_selector(HerosLayer::filterHerosCallback));
    pShuHeroFilterItem->setTag(SHU_HERO_BTN_TAG);
    CCMenu* pShuFilterHeroMenu = CCMenu::create(pShuHeroFilterItem, NULL);
    pShuFilterHeroMenu->setPosition(ccp(FILTER_HERO_BACK_LEFT_OFFSET + 258.0f, updownOffset + 5.0f));
    pParent->addChild(pShuFilterHeroMenu, 4, SHU_HERO_BTN_TAG);
    m_HeroMenuItems[CAMP_SHU] = pShuHeroFilterItem;

    // 筛选吴国武将按钮 (menuitem)
    CCMenuItemImage *pWuHeroFilterItem = CCMenuItemImage::create("HL_wu.png",
                                                                 "HL_wu.png",
                                                                 "HL_wus.png",
                                                                 this,
                                                                 menu_selector(HerosLayer::filterHerosCallback));
    pWuHeroFilterItem->setTag(WU_HERO_BTN_TAG);
    CCMenu* pWuFilterHeroMenu = CCMenu::create(pWuHeroFilterItem, NULL);
    pWuFilterHeroMenu->setPosition(ccp(FILTER_HERO_BACK_LEFT_OFFSET + 365.0f, updownOffset + 5.0f));
    pParent->addChild(pWuFilterHeroMenu, 4, WU_HERO_BTN_TAG);
    m_HeroMenuItems[CAMP_WU] = pWuHeroFilterItem;
    
    // 筛选群雄按钮 (menuitem)
    CCMenuItemImage *pQxHeroFilterItem = CCMenuItemImage::create("HL_qun.png",
                                                                 "HL_qun.png",
                                                                 "HL_quns.png",
                                                                 this,
                                                                 menu_selector(HerosLayer::filterHerosCallback));
    pQxHeroFilterItem->setTag(QX_HERO_BTN_TAG);
    CCMenu* pQxFilterHeroMenu = CCMenu::create(pQxHeroFilterItem, NULL);
    pQxFilterHeroMenu->setPosition(ccp(FILTER_HERO_BACK_LEFT_OFFSET + 472.0f, updownOffset + 5.0f));
    pParent->addChild(pQxFilterHeroMenu, 4, QX_HERO_BTN_TAG);
    m_HeroMenuItems[CAMP_QX] = pQxHeroFilterItem;
    
    // 筛选其他武将按钮 (menuitem)
    CCMenuItemImage *pOtherHeroFilterItem = CCMenuItemImage::create("HL_other.png",
                                                                    "HL_other.png",
                                                                    "HL_others.png",
                                                                    this,
                                                                    menu_selector(HerosLayer::filterHerosCallback));
    pOtherHeroFilterItem->setTag(OTHER_HERO_BTN_TAG);
    CCMenu* pOtherFilterHeroMenu = CCMenu::create(pOtherHeroFilterItem, NULL);
    pOtherFilterHeroMenu->setPosition(ccp(FILTER_HERO_BACK_LEFT_OFFSET + 576.0f,
                                          updownOffset + 5.0f));
    pParent->addChild(pOtherFilterHeroMenu, 4, OTHER_HERO_BTN_TAG);
    m_HeroMenuItems[CAMP_OTHER] = pOtherHeroFilterItem;
    
    // 武将列表大小
    CCSize heroTableViewSize = CCSizeMake(visibleSize.width - HEROLIST_CELL_PADDING * 2,
                                          HEROLIST_CELL_HEIGHT * HEROLIST_DISPLAY_ROWS);
    updownOffset = midImageTopOffset - (m_pGameState->is1136() ? 600.0f : 580.0f);
    
    // 武将列表相关
    m_HerosTablePageNumber = 0;       // 初始页号为 0
    m_isHeroTableView1Show = true;
    m_isHeroTableView2Show = false;
    HerosTableView *heros_tableView1 = HerosTableView::create(this, heroTableViewSize);
    heros_tableView1->setDirection(kCCScrollViewDirectionVertical);
    heros_tableView1->setAnchorPoint(CCPointZero);
    heros_tableView1->setPosition(ccp(HEROLIST_CELL_PADDING, m_pGameState->getBottomOffset() + 165.0f));
    heros_tableView1->setDelegate(this);
    heros_tableView1->setVerticalFillOrder(kCCTableViewFillTopDown);
    pParent->addChild(heros_tableView1, 4, HEROS_TABLEVIEW1_TAG);
    
    HerosTableView *heros_tableView2 = HerosTableView::create(this, heroTableViewSize);
    heros_tableView2->setDirection(kCCScrollViewDirectionVertical);
    heros_tableView2->setAnchorPoint(CCPointZero);
    heros_tableView2->setPosition(ccp(HEROLIST_CELL_PADDING + visibleSize.width, m_pGameState->getBottomOffset() + 165.0f));
    heros_tableView2->setDelegate(this);
    heros_tableView2->setVerticalFillOrder(kCCTableViewFillTopDown);
    pParent->addChild(heros_tableView2, 4, HEROS_TABLEVIEW2_TAG);
    // 武将列表2初始不显示
    heros_tableView2->setVisible(false);
}

/**
 *   出售武将按钮回调
 *
 *   参数：
 *       pSender     出售武将按钮
 */
void HerosLayer::saleHerosCallback(CCObject* pSender) {
    m_pGameState->setCurrTagWhoCallHeroSel(WCH_SALE);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_HEROS_STRING);
}


/**
 *   筛选武将按钮回调
 *
 *   参数：
 *       pSender     筛选武将按钮
 */
void HerosLayer::filterHerosCallback(CCObject* pSender) {
    CCNode* pParent = (CCNode*)this->getChildByTag(CONTAINER_TAG);
    if (m_isTurnPage) return;
    CCSprite* sp = NULL;
    
    sp = (CCSprite*)pParent->getChildByTag(BTNWEI_TAG_B);
    sp->setVisible(false);
    sp = (CCSprite*)pParent->getChildByTag(BTNSHU_TAG_B);
    sp->setVisible(false);
    sp = (CCSprite*)pParent->getChildByTag(BTNWU_TAG_B);
    sp->setVisible(false);
    sp = (CCSprite*)pParent->getChildByTag(BTNQUN_TAG_B);
    sp->setVisible(false);
    sp = (CCSprite*)pParent->getChildByTag(BTNOTHER_TAG_B);
    sp->setVisible(false);
    sp = (CCSprite*)pParent->getChildByTag(BTNALL_TAG_B);
    sp->setVisible(false);
    
    CCMenuItemImage* pCCMenuItemImage = (CCMenuItemImage*)pSender;
    switch (pCCMenuItemImage->getTag()) {
    case ALL_HERO_BTN_TAG:
        {
            sp = (CCSprite*)pParent->getChildByTag(BTNALL_TAG_B);
            sp->setVisible(true);
            m_pHeroManager->setCamp(CAMP_ALL, m_isExceptLock, m_isExceptGoIntoBattle);
        }
        break;
    case WEI_HERO_BTN_TAG:
        {
            sp = (CCSprite*)pParent->getChildByTag(BTNWEI_TAG_B);
            sp->setVisible(true);
            m_pHeroManager->setCamp(CAMP_WEI, m_isExceptLock, m_isExceptGoIntoBattle);
        }
        break;
    case SHU_HERO_BTN_TAG:
        {
            sp = (CCSprite*)pParent->getChildByTag(BTNSHU_TAG_B);
            sp->setVisible(true);
            m_pHeroManager->setCamp(CAMP_SHU, m_isExceptLock, m_isExceptGoIntoBattle);
        }
        break;
    case WU_HERO_BTN_TAG:
        {
            sp = (CCSprite*)pParent->getChildByTag(BTNWU_TAG_B);
            sp->setVisible(true);

            m_pHeroManager->setCamp(CAMP_WU, m_isExceptLock, m_isExceptGoIntoBattle);
        }
        break;
    case QX_HERO_BTN_TAG:
        {
            sp = (CCSprite*)pParent->getChildByTag(BTNQUN_TAG_B);
            sp->setVisible(true);
            m_pHeroManager->setCamp(CAMP_QX, m_isExceptLock, m_isExceptGoIntoBattle);
        }
        break;
    case OTHER_HERO_BTN_TAG:
        {
            sp = (CCSprite*)pParent->getChildByTag(BTNOTHER_TAG_B);
            sp->setVisible(true);
            m_pHeroManager->setCamp(CAMP_OTHER, m_isExceptLock, m_isExceptGoIntoBattle);
        }
        break;
    default:
        CCAssert(false, "invaild camp");
        break;
    };
    
    m_HerosTablePageNumber = 0;
    HerosTableView* table1 = (HerosTableView*)pParent->getChildByTag(HEROS_TABLEVIEW1_TAG);
    HerosTableView* table2 = (HerosTableView*)pParent->getChildByTag(HEROS_TABLEVIEW2_TAG);
    table1->reloadData();
    table1->setPosition(ccp(HEROLIST_CELL_PADDING, 165.0f + m_pGameState->getBottomOffset()));
    table1->setVisible(true);
    table2->setVisible(false);
    
    _updatePages();
}

/*
 *
 */
void HerosLayer::registerWithTouchDispatcher() {
    //    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, false);
}

/*
 *   返回武将列表的单元格数量
 */
unsigned int HerosLayer::numberOfCellsInTableView(HerosTableView *table) {
    unsigned int numberOfCells = 0;
    
    // 获取当前阵营拥有的武将数量
    unsigned int herosOfPlayerCount = m_pHeroManager->getCampHerosOfPlayerCount();
    
    // 当前页要显示的武将数量
    int herosInCurrPage = 0;
    // 计算当前页要显示的武将数量
    herosInCurrPage = herosOfPlayerCount - HEROSLIST_MAX_ROWS_PER_PAGE * 2 * m_HerosTablePageNumber;
    if (herosInCurrPage > 0) {
        if (herosInCurrPage > HEROSLIST_MAX_ROWS_PER_PAGE * 2) {
            herosInCurrPage = HEROSLIST_MAX_ROWS_PER_PAGE * 2;
        }
    }
    
    if (herosInCurrPage > 0) {
        numberOfCells = static_cast<unsigned int>(herosInCurrPage / 2) + herosInCurrPage % 2;
    }
    return numberOfCells;
}

/*
 *  设置武将列表单元格的大小的回调方法
 */
CCSize HerosLayer::cellSizeForTable(HerosTableView *table)
{
    return CCSizeMake(table->getContentSize().width, HEROLIST_CELL_HEIGHT);
}

/*
 * 设置武将列表单元格内容的回调方法
 */
CCTableViewCell* HerosLayer::tableCellAtIndex(HerosTableView *table, unsigned int idx) {
    CCTableViewCell *cell = table->dequeueCell();
    
    // 获取武将数据管理器
    HeroItemNode* pLeftHeroItemNode = NULL;
    HeroItemNode* pRightHeroItemNode = NULL;
    if (!cell) {
        cell = new CCTableViewCell();
        cell->autorelease();
        
        pLeftHeroItemNode = HeroItemNode::create();
        pLeftHeroItemNode->setContentSize(CCSizeMake(table->getContentSize().width / 2.0f, HEROLIST_CELL_HEIGHT));
        pLeftHeroItemNode->setAnchorPoint(CCPointZero);
        pLeftHeroItemNode->setPosition(CCPointZero);
        cell->addChild(pLeftHeroItemNode, 1, LEFT_HERO_NODE_TAG);
        
        pRightHeroItemNode = HeroItemNode::create();
        pRightHeroItemNode->setContentSize(CCSizeMake(table->getContentSize().width / 2.0f, HEROLIST_CELL_HEIGHT));
        pRightHeroItemNode->setAnchorPoint(CCPointZero);
        
//        pRightHeroItemNode->setPosition(ccp(table->getContentSize().width / 2.0f, 0.0f));
        pRightHeroItemNode->setPosition(ccp(RIGHT_ITEM_OFFSET, 0.0f));
        cell->addChild(pRightHeroItemNode, 1, RIGHT_HERO_NODE_TAG);
    }
    if (!pLeftHeroItemNode) {
        pLeftHeroItemNode = (HeroItemNode*)cell->getChildByTag(LEFT_HERO_NODE_TAG);
    }
    if (!pRightHeroItemNode) {
        pRightHeroItemNode = (HeroItemNode*)cell->getChildByTag(RIGHT_HERO_NODE_TAG);
    }
    
    unsigned int leftHeroInHmIndex = m_HerosTablePageNumber * HEROSLIST_MAX_ROWS_PER_PAGE * 2 + idx * 2;
    HeroOfPlayerItem *pHeroItemLeft = m_pHeroManager->getCampHeroDataByIndex(leftHeroInHmIndex);
    HeroOfPlayerItem *pHeroItemRight = m_pHeroManager->getCampHeroDataByIndex(leftHeroInHmIndex + 1);
    
    pLeftHeroItemNode->setVisible(pHeroItemLeft != NULL);
    pRightHeroItemNode->setVisible(pHeroItemRight != NULL);
    if (pHeroItemLeft) {
        pLeftHeroItemNode->updateUiComponent(pHeroItemLeft, this);
    }
    if (pHeroItemRight) {
        pRightHeroItemNode->updateUiComponent(pHeroItemRight, this);
    }
    
    return cell;
}

void HerosLayer::tableCellHighlight(HerosTableView *table, CCTableViewCell *cell)
{
    //    CCTexture2D *aTexture=CCTextureCache::sharedTextureCache()->addImage("cell_selected.png");
    //    CCSprite *pSprite=(CCSprite *)cell->getChildByTag(789);
    //    pSprite->setTexture(aTexture);
}

void HerosLayer::tableCellUnhighlight(HerosTableView *table, CCTableViewCell *cell)
{
    //    CCTexture2D *aTexture=CCTextureCache::sharedTextureCache()->addImage("cell.png");
    //    CCSprite *pSprite=(CCSprite *)cell->getChildByTag(789);
    //    pSprite->setTexture(aTexture);
}

/*
 * 玩家 touch 单元格
 */
void HerosLayer::tableCellTouched(HerosTableView *table, CCTableViewCell *cell, const float touchX) {
    if (m_isTurnPage) return;
    
    CCLog("cell touched at index: %i, touchX = %f", cell->getIdx(), touchX);
    
    bool isSelectedUpdate = false;  // 武将选择状态是否有更新
    bool isAddSelectd = false;      // true 表示增加选择武将，false 表示去除已经选择的武将
    
    unsigned int leftHeroInHmIndex = m_HerosTablePageNumber * HEROSLIST_MAX_ROWS_PER_PAGE * 2 + cell->getIdx() * 2;
    HeroOfPlayerItem* pHeroOfPlayerItem = NULL;
    
    unsigned int tag = m_pGameState->getTagWhoCallHeroSel();
    if (touchX >= 0 && touchX <= TOUCH_LEFT_HERO_BOUND_LEFT) {
        //CCLog("touch left 武将头像");
        pHeroOfPlayerItem = m_pHeroManager->getCampHeroDataByIndex(leftHeroInHmIndex);
        if (pHeroOfPlayerItem && tag != WCH_NONE) {
            /*获取武将uniid*/
            m_pGameState->setHeroIntro(pHeroOfPlayerItem->getUniId());
            m_pGameState->setTagWhoCallHeroIntro(HEROLIST_LAYER) ;
            /*发送点击武将消息*/
            CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_HEROINTRO_STRING);
            return;
        }
    }
    if (touchX >= TOUCH_LEFT_HERO_BOUND_RIGHT && touchX <= TOUCH_RIGHT_HERO_BOUND_LEFT) /*touch 武将头像*/{
        //CCLog("touch right 武将头像");
        pHeroOfPlayerItem = m_pHeroManager->getCampHeroDataByIndex(leftHeroInHmIndex + 1);
        if (pHeroOfPlayerItem && tag != WCH_NONE) {
            /*获取武将uniid*/
            m_pGameState->setHeroIntro(pHeroOfPlayerItem->getUniId());
            m_pGameState->setTagWhoCallHeroIntro(HEROLIST_LAYER) ;
            /*发送点击武将消息*/
            CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_HEROINTRO_STRING);
            return;
        }
    }
    
    if (touchX >= TOUCH_LEFT_HERO_BOUND_LEFT && touchX <= TOUCH_LEFT_HERO_BOUND_RIGHT) /*touch 左边武将*/{
        pHeroOfPlayerItem = m_pHeroManager->getCampHeroDataByIndex(leftHeroInHmIndex);
        if (pHeroOfPlayerItem) {
            //CCLog("touch 左侧武将 name = %s", pHeroOfPlayerItem->getHeroItem()->getName().c_str());
            
            if (m_isSelectHeroMode) /*选择武将模式*/ {
                for (int i = 0; i < m_countOfNotAllowed; i++) {
                    if (!strcmp(pHeroOfPlayerItem->getUniId().c_str(), m_szNotAllowedHeroUniId[i])) /*该武将不能选*/ {
                        return;
                    }
                } /*for*/
            }
        }
    }
    
    if (touchX >= TOUCH_RIGHT_HERO_BOUND_LEFT && touchX <= TOUCH_RIGHT_HERO_BOUND_RIGHT) /*touch 右边武将*/{
        pHeroOfPlayerItem = m_pHeroManager->getCampHeroDataByIndex(leftHeroInHmIndex + 1);
        if (pHeroOfPlayerItem) {
            //CCLog("touch 右侧武将 name = %s", pHeroOfPlayerItem->getHeroItem()->getName().c_str());
            if (m_isSelectHeroMode) {
                for (int i = 0; i < m_countOfNotAllowed; i++) {
                    if (!strcmp(pHeroOfPlayerItem->getUniId().c_str(), m_szNotAllowedHeroUniId[i])) /*该武将不能选*/ {
                        return;
                    }
                } /*for*/
            }
        }
    }
    
    if (m_isSelectHeroMode && pHeroOfPlayerItem) {
        if (_heroIsSelected((char *)pHeroOfPlayerItem->getUniId().c_str())) /*该武将已经被选中*/  {
            _removeSelectedHeroUniId((char *)pHeroOfPlayerItem->getUniId().c_str());
            isAddSelectd = false;
        } else {
            if (!_isAlreadyMaxAllowSelectedHero()) /*还没到最大可选择武将数量*/{
                m_countOfSelectedHeros++;
                /*
                 * 添加已选武将
                 */
                memset(m_szSelectedHeroUniId[m_countOfSelectedHeros - 1], '\0', HERO_UNIID_BUFFER_SIZE);
                strcpy(m_szSelectedHeroUniId[m_countOfSelectedHeros - 1], pHeroOfPlayerItem->getUniId().c_str());
                if (m_isNoConfirm) /* 无需确认 */{
                    CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_CHOSCONFIRM_OK_STRING);
                    return;
                }
                isAddSelectd = true;
            }
        }
        isSelectedUpdate = true;
    }
    /* 获得已经选择的武将并显示再确认层 */
    ChosConfirm* confirmLayer = (ChosConfirm*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(119);
//    int attack = 0;
//    for (int i = 0; i < m_countOfSelectedHeros; ++i) {
//        HeroOfPlayerItem* pItem = HeroManager::getInstance()->getGoIntoBattleHeroDataByIndex(i);
//        if (pItem) {
//            attack += pItem->getAttack();
//        }
//    }
    if (isSelectedUpdate) /*武将选择状态有更新*/ {
        int sellForMoney = 0;
        int tunExp = 0;
        for (int i = 0; i < m_countOfSelectedHeros; i++) {
            HeroOfPlayerItem* pItem = m_pHeroManager->getHeroDataByUniId(m_szSelectedHeroUniId[i]);
            // kknd
            CCAssert(pItem, "invalid pItem");
            
            sellForMoney += m_pHeroManager->getAdditionHeroData(pItem->getHeroItem()->getRarity(), pItem->getLevel())->getGoldBySale();
            tunExp += m_pHeroManager->getAdditionHeroData(pItem->getHeroItem()->getRarity(), pItem->getLevel())->getExpByFood();
        }
        
        if (m_pGameState->getCurrTagWhoCallHeroSel() == WCH_SALE)/*出售武将列表*/{
            if (confirmLayer)
                confirmLayer->updateUi( m_countOfSelectedHeros, sellForMoney);
        } else if (m_pGameState->getCurrTagWhoCallHeroSel() == WCH_TEAMER)/*选择队员列表*/ {
            if (confirmLayer)
                confirmLayer->updateUi(m_countOfSelectedHeros, 0);
        } else if (m_pGameState->getCurrTagWhoCallHeroSel() == WCH_FORFOOD)/*被吞武将列表*/ {
            if (confirmLayer)
                confirmLayer->updateUi(m_countOfSelectedHeros, tunExp);
        }
        
        
        CCPoint oldContentOffset = table->getContentOffset();
        table->reloadData();
        table->setContentOffset(oldContentOffset);
        if (m_pGameState->getTagWhoCallHeroSel() == WCH_TEAMER || m_pGameState->getTagWhoCallHeroSel() == WCH_FORFOOD
            || m_pGameState->getTagWhoCallHeroSel() == WCH_TEAMER_PVP) {
            
        } else {
            if (isAddSelectd) {
                if (1 == m_countOfSelectedHeros)
                    CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_CHOSCONFIRM_SHOW_STRING);
            } else {
                if (0 == m_countOfSelectedHeros)
                {
                    CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_CHOSCONFIRM_HIDE_STRING);
                }
            }
        }
    }
}

/*
 *
 */
void HerosLayer::scrollViewDidZoom(CCScrollView* view) {}

/*
 *
 */
void HerosLayer::scrollViewDidScroll(CCScrollView* view) {}


/*
 * 功能：
 *      设置是否是选择武将模式
 * 参数:
 *      selectHeroMode      是否选择模式
 *      countOfIniHeros     初始加载的已选武将数量
 *      szIniHeros          初始加载的武将
 *      maxCountOfSelHeros  最大可选择武将数量
 *      szNotAllowHeros     不能选择的武将
 *      countOfNotAllowed   不能选择的武将数量
 *      isNoConfirm         是否不需要确认 true 不需要    false 需要
 *
 */
void HerosLayer::setSelectHeroMode(const bool selectHeroMode,
                                   const unsigned int countOfIniHeros,
                                   char szIniHeros[][HERO_UNIID_BUFFER_SIZE],
                                   unsigned int maxCountOfSelHeros,
                                   char szNotAllowHeros[][HERO_UNIID_BUFFER_SIZE],
                                   unsigned int countOfNotAllowed,
                                   const bool isNoConfirm)
{
    m_isSelectHeroMode = selectHeroMode;
    
    memset(m_szSelectedHeroUniId, '\0', sizeof(m_szSelectedHeroUniId));
    memset(m_szNotAllowedHeroUniId, '\0', sizeof(m_szNotAllowedHeroUniId));
    
    m_countOfSelectedHeros = 0;
    m_countOfNotAllowed = 0;
    m_countOfMaxAllowSelectHeros = 0;
    
    if (selectHeroMode) {
        m_countOfMaxAllowSelectHeros = maxCountOfSelHeros;
        m_isNoConfirm = isNoConfirm;
        
        if (countOfIniHeros > 0 && szIniHeros) /*有初始加载的已选武将*/ {
            m_countOfSelectedHeros = countOfIniHeros;
            for (int i = 0; i < countOfIniHeros; i++) {
                strcpy(m_szSelectedHeroUniId[i], szIniHeros[i]);
            } /*for*/
        }
        
        if (szNotAllowHeros && countOfNotAllowed > 0) /*有不能选择的武将*/ {
            m_countOfNotAllowed = countOfNotAllowed;
            for (int i = 0; i < m_countOfNotAllowed; i++) {
                strcpy(m_szNotAllowedHeroUniId[i], szNotAllowHeros[i]);
            } /*for*/
        }
    }
}

/*
 * 功能：
 *      获取已经选择的武将
 * 参数：
 *      szHeroUniid     接收已选武将 uniid 的缓冲区
 * 返回：
 *      已选武将数量
 */
unsigned int HerosLayer::getSelectedHeros(char szHeroUniid[][HERO_UNIID_BUFFER_SIZE]) {
    if (!szHeroUniid) return 0;
    
    for (int i = 0; i < m_countOfSelectedHeros; i++) {
        memset(szHeroUniid[i], '\0', sizeof(HERO_UNIID_BUFFER_SIZE));
        strcpy(szHeroUniid[i], m_szSelectedHeroUniId[i]);
    } /*for*/
    
    return m_countOfSelectedHeros;
}

/*
 * 功能：
 *      指定的武将是否已经被选中
 * 参数：
 *      heroUniId   ->  武将 uniId
 * 返回：
 *     已经被选中返回 true，否则返回 false
 */
bool HerosLayer::_heroIsSelected(char heroUniId[]) {
    if(!heroUniId) return false;
        
    bool isMatch = false;
    for (int i = 0; i < m_countOfSelectedHeros; i++) {
        if (!m_szSelectedHeroUniId[i])
            continue;
        if (!strcmp(heroUniId, m_szSelectedHeroUniId[i])) /*该武将被选中*/ {
            isMatch = true;
            break;
        }
    } /*for*/
    return isMatch;
}

/*
 * 功能：
 *      
 * 参数：
 *      heroUniId   ->  武将 uniId
 */
void HerosLayer::_removeSelectedHeroUniId(char heroUniId[]) {
    if(!heroUniId || m_countOfSelectedHeros == 0 )
        return;
    
    bool isMatch = false;
    int index = 0;
    for (int i = 0; i < m_countOfSelectedHeros; i++) {
        if (!strcmp(heroUniId, m_szSelectedHeroUniId[i])) /*该武将被选中*/ {
            isMatch = true;
            index = i;
            break;
        }
    } /*for*/
    
    if (isMatch) /*存在要 remove 的 uniid*/ {
        for (int i = index; i < m_countOfMaxAllowSelectHeros - 1; i++) {
            memset(m_szSelectedHeroUniId[i], '\0', 16);
            strcpy(m_szSelectedHeroUniId[i], m_szSelectedHeroUniId[i + 1]);
        } /*for*/
        m_countOfSelectedHeros--;
    }
}

/*
 *
 */
bool HerosLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    //获取触摸点初始化坐标
    CCPoint beginPoint = pTouch->getLocationInView();
    beginPoint = CCDirector::sharedDirector()->convertToGL(beginPoint);
    
//    CCLog("touchx = %f, touchy = %f\n", beginPoint.x, beginPoint.y);
    
    return true;
}

/*
 *
 */
void HerosLayer::ccTouchEnded(CCTouch*, CCEvent *) {}

/*
 *
 */
void HerosLayer::setVisible(bool visible) {
    ////////////////////////////////
    // super first
    CCLayer::setVisible(visible);
    setTouchEnabled(visible);
    
    CCNode* pParent = (CCNode*)this->getChildByTag(CONTAINER_TAG);
    HerosTableView* table1 = (HerosTableView*)pParent->getChildByTag(HEROS_TABLEVIEW1_TAG);
    HerosTableView* table2 = (HerosTableView*)pParent->getChildByTag(HEROS_TABLEVIEW2_TAG);

    if (visible) {
        for (int i = 0; i < 6; i++) {
            ((CCSprite*)pParent->getChildByTag(BTNALL_TAG_B + i))->setVisible(false);
        } /*for*/
        ((CCSprite*)pParent->getChildByTag(BTNALL_TAG_B))->setVisible(true);
        
        /* 初始筛选 */
        _initFilter();
        
        m_HerosTablePageNumber = 0;
        table1->reloadData();
        table1->setPosition(ccp(HEROLIST_CELL_PADDING, GameState::getInstance()->getBottomOffset() + 165.0f));
        _updatePages();
        _updateHeroMenu();

        table1->setVisible(true);
        table2->setVisible(false);
        
        if (m_isSelectHeroMode && !m_isNoConfirm && m_countOfSelectedHeros > 0) {
            CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_CHOSCONFIRM_SHOW_STRING);
        }
    } else {
        table1->setPosition(ccp(-640.0f, 0.0f));
        table2->setPosition(ccp(-640.0f, 0.0f));
    }
}

/*
 *  武将列表初始筛选
 */
void HerosLayer::_initFilter() {
    switch (m_pGameState->getCurrTagWhoCallHeroSel()) {
        case WCH_BROWSE:
            {
                m_isExceptLock = false;
                m_isExceptGoIntoBattle = false;
                setSelectHeroMode(false);
                setTipMenu();
            }
            break;
        case WCH_SALE:
            {
                m_isExceptLock = true;
                m_isExceptGoIntoBattle = true;
                setTipMenu(2);
                
                WhoCallHeroList oldWch = m_pGameState->getTagWhoCallHeroSel();
                if (WCH_BROWSE == oldWch)/*武将列表调用出售*/ {
                    setSelectHeroMode(true, 0, NULL, MAX_ALLOW_SELECT_HEROS, NULL, 0, false);
                } else if (WCH_SALE == oldWch) {
                    if (m_pGameState->getHeroForSaleSuccess())/*出售确认*/ {
                        setSelectHeroMode(true, 0, NULL, MAX_ALLOW_SELECT_HEROS, NULL, 0, false);
                    } else/*出售取消*/ {
                        /*数据不处理*/
                        CCLog("");
                    }
                }
            }
            break;
        case WCH_LEVUP:
            {
                m_isExceptLock = false;
                m_isExceptGoIntoBattle = false;
                
                unsigned int notAllowSelected = 0;
                char szNotAllowSelected[MAX_FOOD_HEROS_FOR_LEVUP + 1][HERO_UNIID_BUFFER_SIZE];
                memset(szNotAllowSelected, '\0', sizeof(szNotAllowSelected));

                std::string levupHeroUUid = m_pGameState->getLevupHeroUUidForHeroList();
                if (!levupHeroUUid.empty()) {
                    notAllowSelected++;
                    strcpy(szNotAllowSelected[0], levupHeroUUid.c_str());
                }
                
                // 获取已选择的升级被吞噬的武将
                unsigned int countOfFoodHeros = m_pGameState->getFoodHersForLevUp(szNotAllowSelected + notAllowSelected);
                notAllowSelected += countOfFoodHeros;
                
                if (notAllowSelected > 0) {
                    setSelectHeroMode(true, 0, NULL, 1, szNotAllowSelected, notAllowSelected, true);
                } else {
                    setSelectHeroMode(true, 0, NULL, 1, NULL, 0, true);
                }
                setTipMenu(5);
            }
            break;
        case WCH_EVOUP:
            {
                m_isExceptLock = true;
                m_isExceptGoIntoBattle = false;
                
                std::string evoupHeroUUid = m_pGameState->getEvoupHeroUUidForHeroList();
                if (!evoupHeroUUid.empty()) {
                    char szHeroEvoUp[1][HERO_UNIID_BUFFER_SIZE];
                    strcpy(szHeroEvoUp[0], evoupHeroUUid.c_str());
                    setSelectHeroMode(true, 0, NULL, 1, szHeroEvoUp, 1, true);
                } else {
                    setSelectHeroMode(true, 0, NULL, 1, NULL, 0, true);
                }
                setTipMenu(7);
            }
            break;
        case WCH_FORFOOD:
            {
                m_isExceptLock = true;
                m_isExceptGoIntoBattle = true;
                
                unsigned int notAllowSelected = 0;
                char szNotAllowSelected[1][HERO_UNIID_BUFFER_SIZE];
                std::string levupHeroUUid = m_pGameState->getLevupHeroUUidForHeroList();
                if (!levupHeroUUid.empty()) {
                    notAllowSelected = 1;
                    strcpy(szNotAllowSelected[0], levupHeroUUid.c_str());
                }
                
                // 获取已选择的升级被吞噬的武将
                char szFoodHersForLevUp[MAX_FOOD_HEROS_FOR_LEVUP][HERO_UNIID_BUFFER_SIZE];
                memset(szFoodHersForLevUp, '\0', sizeof(szFoodHersForLevUp));
                unsigned int countOfFoodHeros = m_pGameState->getFoodHersForLevUp(szFoodHersForLevUp);
                setSelectHeroMode(true,
                                  countOfFoodHeros,
                                  countOfFoodHeros > 0 ? szFoodHersForLevUp : NULL,
                                  MAX_FOOD_HEROS_FOR_LEVUP,
                                  notAllowSelected > 0 ? szNotAllowSelected : NULL,
                                  notAllowSelected,
                                  false);
                setTipMenu(6);
                CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_CHOSCONFIRM_SHOW_STRING);
            }
            break;
        case WCH_CAPTAIN:
            {
                m_isExceptLock = false;
                m_isExceptGoIntoBattle = false;
                
                char m_szNotAllowedHeroUniId[1][HERO_UNIID_BUFFER_SIZE];
                memset(m_szNotAllowedHeroUniId, '\0', sizeof(m_szNotAllowedHeroUniId));
                strcpy(m_szNotAllowedHeroUniId[0],
                       m_pHeroManager->getGoIntoBattleHeroDataByIndex(0)->getUniId().c_str());
                setSelectHeroMode(true, 0, NULL, 1, m_szNotAllowedHeroUniId, 1, true);
                setTipMenu(3);
            }
            break;
        case WCH_CAPTAIN_PVP:
            {
                m_isExceptLock = false;
                m_isExceptGoIntoBattle = false;
                
                char m_szNotAllowedHeroUniId[1][HERO_UNIID_BUFFER_SIZE];
                memset(m_szNotAllowedHeroUniId, '\0', sizeof(m_szNotAllowedHeroUniId));
                strcpy(m_szNotAllowedHeroUniId[0],
                       m_pHeroManager->getPvpGoIntoBattleHeroDataByIndex(0)->getUniId().c_str());
                setSelectHeroMode(true, 0, NULL, 1, m_szNotAllowedHeroUniId, 1, true);
                setTipMenu(8);
            }
            break;
        case WCH_TEAMER:
            {
                m_isExceptLock = false;
                m_isExceptGoIntoBattle = false;
                
                // 获取上阵武将队员数量
                const unsigned int countOfTeamers = m_pHeroManager->getGoIntoBattleHerosOfPlayerCount() - 1;
                
                // 存放所有被选中的武将的 uniid
                char szSelectedHeroUniId[MAX_GO_INTO_BATTLE_HEROS - 1][HERO_UNIID_BUFFER_SIZE];
                memset(szSelectedHeroUniId, '\0', sizeof(szSelectedHeroUniId));
                for (int i = 0; i < countOfTeamers; i++) {
                    strcpy(szSelectedHeroUniId[i],
                           m_pHeroManager->getGoIntoBattleHeroDataByIndex(i + 1)->getUniId().c_str());
                }
                char m_szNotAllowedHeroUniId[1][HERO_UNIID_BUFFER_SIZE];
                memset(m_szNotAllowedHeroUniId, '\0', sizeof(m_szNotAllowedHeroUniId));
                strcpy(m_szNotAllowedHeroUniId[0],
                       m_pHeroManager->getGoIntoBattleHeroDataByIndex(0)->getUniId().c_str());
                setSelectHeroMode(true, countOfTeamers, szSelectedHeroUniId,
                                    MAX_GO_INTO_BATTLE_HEROS - 1, m_szNotAllowedHeroUniId,
                                  1, false);
                setTipMenu(4);
                
                CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_CHOSCONFIRM_SHOW_STRING);
                /*确认框数据更新*/
                ChosConfirm* confirmLayer = (ChosConfirm*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(119);
                if (confirmLayer)
                    confirmLayer->updateUi(m_countOfSelectedHeros, 0);
            }
            break;
        case WCH_TEAMER_PVP:
            {
                m_isExceptLock = false;
                m_isExceptGoIntoBattle = false;
                
                // 获取 pvp 上阵武将队员数量
                const unsigned int countOfTeamers = m_pHeroManager->getPvpGoIntoBattleHerosOfPlayerCount() - 1;
                
                // 存放所有被选中的武将的 uniid
                char szSelectedHeroUniId[MAX_GO_INTO_BATTLE_HEROS - 1][HERO_UNIID_BUFFER_SIZE];
                memset(szSelectedHeroUniId, '\0', sizeof(szSelectedHeroUniId));
                for (int i = 0; i < countOfTeamers; i++) {
                    strcpy(szSelectedHeroUniId[i],
                           m_pHeroManager->getPvpGoIntoBattleHeroDataByIndex(i + 1)->getUniId().c_str());
                }
                char m_szNotAllowedHeroUniId[1][HERO_UNIID_BUFFER_SIZE];
                memset(m_szNotAllowedHeroUniId, '\0', sizeof(m_szNotAllowedHeroUniId));
                strcpy(m_szNotAllowedHeroUniId[0],
                       m_pHeroManager->getPvpGoIntoBattleHeroDataByIndex(0)->getUniId().c_str());
                setSelectHeroMode(true, countOfTeamers, szSelectedHeroUniId,
                                  MAX_GO_INTO_BATTLE_HEROS - 1, m_szNotAllowedHeroUniId,
                                  1, false);
                setTipMenu(9);
                
                CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_CHOSCONFIRM_SHOW_STRING);
                /*确认框数据更新*/
                ChosConfirm* confirmLayer = (ChosConfirm*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(119);
                if (confirmLayer)
                    confirmLayer->updateUi(m_countOfSelectedHeros, 0);
            }
            break;
        default:
            break;
    }
    
    m_pHeroManager->setCamp(CAMP_ALL, m_isExceptLock, m_isExceptGoIntoBattle);
    m_pHeroManager->updateCamp();
}

/*
 *  功能：
 *      武将列表水平方向翻页回调方法
 *  参数：
 *      table               翻页 tableview
 *      turnPageDirection   翻页方向
 */
void HerosLayer::tableViewTrunPageCallback(HerosTableView* table, HerosTableViewTurnPageDirection turnPageDirection) {
    unsigned int campHerosOfPlayerCount = m_pHeroManager->getCampHerosOfPlayerCount();
    if (campHerosOfPlayerCount <= HEROSLIST_MAX_ROWS_PER_PAGE * 2 ) {
        /* 武将数量不超一页 */
        return;
    }
    CCNode* pParent = (CCNode*)this->getChildByTag(CONTAINER_TAG);
    
    // 余数
    unsigned int pageMod = campHerosOfPlayerCount % (HEROSLIST_MAX_ROWS_PER_PAGE * 2);
    // 计算总页数
    unsigned int totalPages = (campHerosOfPlayerCount - pageMod) / (HEROSLIST_MAX_ROWS_PER_PAGE * 2) + (pageMod > 0 ? 1 : 0);
    
    HerosTableView* table1 = (HerosTableView*)pParent->getChildByTag(HEROS_TABLEVIEW1_TAG);
    HerosTableView* table2 = (HerosTableView*)pParent->getChildByTag(HEROS_TABLEVIEW2_TAG);
    
    if (table->getTag() == HEROS_TABLEVIEW1_TAG) {
        if (kHerosTableViewTrunPageFromLToR == turnPageDirection) /*从左向右(右翻)*/{
            // 首先判断是否还有数据
            if (campHerosOfPlayerCount <= (m_HerosTablePageNumber + 1) * HEROSLIST_MAX_ROWS_PER_PAGE * 2) {
                /* 右边已经没有数据 */
                m_HerosTablePageNumber = 0;
            } else {
               // 页号递增
                m_HerosTablePageNumber++;
            }
            
            // 重新加载数据
            table2->reloadData();
            table2->setPosition(ccp(HEROLIST_CELL_PADDING + 640.0f, GameState::getInstance()->getBottomOffset() + 165.0f));
            table2->setVisible(true);
            table1->runAction(CCSequence::create(CCMoveBy::create(TURNPAGE_ANI_DURING, CCPointMake(-640.0f, 0.0f)),
                                                 CCCallFuncN::create(this, callfuncN_selector(HerosLayer::herosTableView1MovedDoneCallback)), NULL));
            table2->runAction(CCMoveBy::create(TURNPAGE_ANI_DURING, CCPointMake(-640.0f, 0.0f)));
        } else /*从右向左(左翻)*/ {
            if (0 == m_HerosTablePageNumber) /*已经最左*/{
                m_HerosTablePageNumber = totalPages - 1;
            }  else {
                // 页号递减
                m_HerosTablePageNumber--;
            }
            
            table2->reloadData();
            table2->setPosition(ccp(HEROLIST_CELL_PADDING - 640.0f, GameState::getInstance()->getBottomOffset() + 165.0f));
            table2->setVisible(true);
            table1->runAction(CCSequence::create(CCMoveBy::create(TURNPAGE_ANI_DURING, CCPointMake(640.0f, 0.0f)),
                                                 CCCallFuncN::create(this, callfuncN_selector(HerosLayer::herosTableView1MovedDoneCallback)), NULL));
            table2->runAction(CCMoveBy::create(TURNPAGE_ANI_DURING, CCPointMake(640.0f, 0.0f)));
        }
    } else {
        if (kHerosTableViewTrunPageFromLToR == turnPageDirection) /*从左向右*/{
            // 首先判断是否还有数据
            if (campHerosOfPlayerCount <= (m_HerosTablePageNumber + 1) * HEROSLIST_MAX_ROWS_PER_PAGE * 2) {
                /*右边已经没有数据*/
                m_HerosTablePageNumber = 0;
            } else {
                // 页号递增
                m_HerosTablePageNumber++;
            }
            // 重新加载数据
            table1->reloadData();
            table1->setPosition(ccp(HEROLIST_CELL_PADDING + 640.0f, GameState::getInstance()->getBottomOffset() + 165.0f));
            table1->setVisible(true);
            table2->runAction(CCSequence::create(CCMoveBy::create(OPENNING_ANI_DURING, CCPointMake(-640.0f, 0.0f)),
                                                 CCCallFuncN::create(this, callfuncN_selector(HerosLayer::herosTableView2MovedDoneCallback)), NULL));
            table1->runAction(CCMoveBy::create(OPENNING_ANI_DURING, CCPointMake(-640.0f, 0.0f)));
        } else /*从右向左*/ {
            if (0 == m_HerosTablePageNumber) /*已经最左*/{
                m_HerosTablePageNumber = totalPages - 1;
            } else {
                // 页号递减
                m_HerosTablePageNumber--;
            }
            
            table1->reloadData();
            table1->setPosition(ccp(HEROLIST_CELL_PADDING - 640.0f, GameState::getInstance()->getBottomOffset() + 165.0f));
            table1->setVisible(true);
            table2->runAction(CCSequence::create(CCMoveBy::create(OPENNING_ANI_DURING, CCPointMake(640.0f, 0.0f)),
                                                 CCCallFuncN::create(this, callfuncN_selector(HerosLayer::herosTableView2MovedDoneCallback)), NULL));
            table1->runAction(CCMoveBy::create(OPENNING_ANI_DURING, CCPointMake(640.0f, 0.0f)));
        }
    }
    table1->setTouchEnabled(false);
    table2->setTouchEnabled(false);
    m_isTurnPage = true;
    
    // 显示当前页数
    _updatePages();
}

/*
 *  武将列表一（滑动）动画完成
 */
void HerosLayer::herosTableView1MovedDoneCallback() {
    CCNode* pParent = (CCNode*)this->getChildByTag(CONTAINER_TAG);
    HerosTableView* table1 = (HerosTableView*)pParent->getChildByTag(HEROS_TABLEVIEW1_TAG);
    HerosTableView* table2 = (HerosTableView*)pParent->getChildByTag(HEROS_TABLEVIEW2_TAG);

    table1->setVisible(false);
    table1->setTouchEnabled(true);
    table2->setTouchEnabled(true);

    m_isTurnPage = false;
}

/*
 *  武将列表二（滑动）动画完成
 */
void HerosLayer::herosTableView2MovedDoneCallback() {
    CCNode* pParent = (CCNode*)this->getChildByTag(CONTAINER_TAG);
    HerosTableView* table1 = (HerosTableView*)pParent->getChildByTag(HEROS_TABLEVIEW1_TAG);
    HerosTableView* table2 = (HerosTableView*)pParent->getChildByTag(HEROS_TABLEVIEW2_TAG);

    table2->setVisible(false);
    
    table1->setTouchEnabled(true);
    table2->setTouchEnabled(true);
    
    m_isTurnPage = false;
}

/*
 *  更新页数显示 和 武将数量显示
 */
void HerosLayer::_updatePages() {
    CCNode* pParent = (CCNode*)this->getChildByTag(CONTAINER_TAG);
    char page[16];
    char buffer[64];
    
    CCLabelTTF * cur_page = (CCLabelTTF*)pParent->getChildByTag(PAGE_TAG);
    
    int allPage = 0;
    unsigned int countOfCampHeros = m_pHeroManager->getCampHerosOfPlayerCount();
    if (countOfCampHeros > 0) {
        unsigned int mod = countOfCampHeros % (HEROSLIST_MAX_ROWS_PER_PAGE * 2);
        allPage = (countOfCampHeros - mod) / (HEROSLIST_MAX_ROWS_PER_PAGE * 2) + (mod > 0 ? 1 : 0);
    } else {
        allPage = 0;
    }
    
    if (allPage > 0) {
        sprintf(page, "%d/%d",m_HerosTablePageNumber + 1, allPage);
        cur_page->setString(page);
        cur_page->setVisible(true);
    } else {
        cur_page->setVisible(false);
    }
        
    CCSprite* tipright = (CCSprite*)pParent->getChildByTag(RIGHT_TIP_TAG);
    CCSprite* tipleft = (CCSprite*)pParent->getChildByTag(LEFT_TIP_TAG);
    if (allPage <= 1) {
        tipright->setVisible(false);
        tipleft->setVisible(false);
    } else {
        tipright->setVisible(true);
        tipleft->setVisible(true);
    }
    CCLabelTTF* cur_heros = (CCLabelTTF*)pParent->getChildByTag(HERO_COUNT_TAG);
    // 显示武将数据
    sprintf(buffer, "拥有武将 %d/%d", m_pHeroManager->getHerosOfPlayerCount(),
            GamePlayer::getInstance()->getMaxHero());
    cur_heros->setString(buffer);
}

/*
 *  更新筛选武将按钮
 */
void HerosLayer::_updateHeroMenu() {
    for (int i = 0; i < CAMP_ALL; i++) {
        CCMenuItem* pCCMenuItem = m_HeroMenuItems[i];
        if (pCCMenuItem) {
            pCCMenuItem->setEnabled(m_pHeroManager->hasHerosOfCamp((herosOfCamp)i));
        }
    } /*for*/
}

void HerosLayer::initPosition() {
    CCNode* pParent = (CCNode*)this->getChildByTag(CONTAINER_TAG);
    pParent->setPosition(ccp(0, -LAYER_OFFSET));
}


void HerosLayer::restoreTouchUICompent() {
    /*恢复可见*/
    this->setVisible(true);
    CCNode* pParent = (CCNode*)this->getChildByTag(CONTAINER_TAG);
    
    if (m_pGameState->getTagWhoCallHeroSel() == WCH_BROWSE || m_pGameState->getTagWhoCallHeroSel() == WCH_SALE
        || (!m_pGameState->getHeroForSaleSuccess()/*出售取消*/)) {
        pParent->setPosition(ccp(0, 0));
    } else {
        if (pParent->getActionByTag(MOVEOUT_TAG)) {
            pParent->stopAllActions();
            pParent->setPosition(ccp(0, -LAYER_OFFSET));
            CCMoveTo* pMoveTo = CCMoveTo::create(ANIMATION_DURING, ccp(0, 0));
            pMoveTo->setTag(MOVEIN_TAG);
            pParent->runAction(pMoveTo);
            return;
        }
        
        if (!pParent->getActionByTag(MOVEIN_TAG)) {
            CCMoveTo* pMoveTo = CCMoveTo::create(ANIMATION_DURING, ccp(0, 0));
            pMoveTo->setTag(MOVEIN_TAG);
            pParent->runAction(pMoveTo);
        }
    }
    
    CCNode* pL = (CCNode*)this->getChildByTag(L_TIPMENU_TAG);
    CCNode* pM = (CCNode*)this->getChildByTag(M_TIPMENU_TAG);
    CCNode* pR = (CCNode*)this->getChildByTag(R_TIPMENU_TAG);
    
    pL->setPosition(ccp(-160.0f, 0.0f));
    pM->setPosition(ccp(0.0f, 150.0f));
    pR->setPosition(ccp(160.0f, 0.0f));
    pL->runAction(CCMoveTo::create(OPENNING_ANI_DURING, ccp(0.0f, 0.0f)));
    pM->runAction(CCMoveTo::create(OPENNING_ANI_DURING, ccp(0.0f, 0.0f)));
    pR->runAction(CCMoveTo::create(OPENNING_ANI_DURING, ccp(0.0f, 0.0f)));
}

void HerosLayer::moveOutTouchUICompent() {
    CCNode* pParent = (CCNode*)this->getChildByTag(CONTAINER_TAG);
    
    if (m_pGameState->getTagWhoCallHeroSel() == WCH_BROWSE || m_pGameState->getTagWhoCallHeroSel() == WCH_SALE
        || (!m_pGameState->getHeroForSaleSuccess()/*出售取消*/)) {
        pParent->setPosition(ccp(0, -LAYER_OFFSET));
        this->setVisible(false);
    } else {
        if (pParent->getActionByTag(MOVEIN_TAG)) {
            pParent->stopAllActions();
            pParent->setPosition(ccp(0, 0));
            CCSequence* pMoveOUtAction = CCSequence::create(CCMoveTo::create(ANIMATION_DURING, ccp(0, -LAYER_OFFSET)), CCCallFunc::create(this, callfunc_selector(HerosLayer::moveOutCallback)), NULL);
            pMoveOUtAction->setTag(MOVEOUT_TAG);
            pParent->runAction(pMoveOUtAction);
            return;
        }
        if (!pParent->getActionByTag(MOVEOUT_TAG)) {
            CCSequence* pMoveOUtAction = CCSequence::create(CCMoveTo::create(ANIMATION_DURING, ccp(0, -LAYER_OFFSET)), CCCallFunc::create(this, callfunc_selector(HerosLayer::moveOutCallback)), NULL);
            pMoveOUtAction->setTag(MOVEOUT_TAG);
            pParent->runAction(pMoveOUtAction);
        }
    }
    
    CCNode* pL = (CCNode*)this->getChildByTag(L_TIPMENU_TAG);
    CCNode* pM = (CCNode*)this->getChildByTag(M_TIPMENU_TAG);
    CCNode* pR = (CCNode*)this->getChildByTag(R_TIPMENU_TAG);
   if (pL&&pM&&pR) {
    pL->setPosition(ccp(0.0f, 0.0f));
    pM->setPosition(ccp(0.0f, 0.0f));
    pR->setPosition(ccp(0.0f, 0.0f));
   
        pL->runAction(CCMoveTo::create(OPENNING_ANI_DURING, ccp(-160.0f, 0.0f)));
        pM->runAction(CCMoveTo::create(OPENNING_ANI_DURING, ccp(0.0f, 150.0f)));
        pR->runAction(CCMoveTo::create(OPENNING_ANI_DURING, ccp(160.0f, 0.0f)));

    }
  }

void HerosLayer::moveOutCallback() {
    this->setVisible(false);
    /* 刷新调用武将列表状态,为初始状态 */
    m_pGameState->setTagWhoCallHeroSel(WCH_NONE);
}

/*
 * type:
 *      1, 武将列表
 */

/*返回文字*/
//CCSprite* returnFont = CCSprite::create("HL_return.png");
//returnFont->setAnchorPoint(ccp(0, 0));
//returnFont->setPosition(ccp(0, GameState::getInstance()->getBottomOffset() + 750.0f));
//pL->addChild(returnFont, 2);
void HerosLayer::setTipMenu(const int type) {
    if (this->getChildByTag(L_TIPMENU_TAG)) {
        this->removeChildByTag(L_TIPMENU_TAG);
    }
    if (this->getChildByTag(M_TIPMENU_TAG)) {
        this->removeChildByTag(M_TIPMENU_TAG);
    }
    if (this->getChildByTag(R_TIPMENU_TAG)) {
        this->removeChildByTag(R_TIPMENU_TAG);
    }
    CCNode* pL = CCNode::create();
    addChild(pL, 1, L_TIPMENU_TAG);
    CCNode* pM = CCNode::create();
    addChild(pM, 1, M_TIPMENU_TAG);
    CCNode* pR = CCNode::create();
    addChild(pR, 1, R_TIPMENU_TAG);
    
    /*显示当前列表主题*/
    CCSprite* spM = CCSprite::create("CM_mid.png");
    spM->setAnchorPoint(ccp(0, 0));
    spM->setPosition(ccp(165.0f, GameState::getInstance()->getBottomOffset() + 745.0f));
    pM->addChild(spM, 1);
    
    if (type == 1)/*武将浏览列表*/ {
        /*无返回*/
        CCSprite* spL = CCSprite::create("CM_left.png");
        spL->setAnchorPoint(ccp(0, 0));
        spL->setPosition(ccp(0, GameState::getInstance()->getBottomOffset() + 745.0f));
        pL->addChild(spL, 1);
        /*显示当前列表主题*/
        CCSprite* spTitle = CCSprite::create("HL_mylist.png");
        spTitle->setAnchorPoint(ccp(0, 0));
        spTitle->setPosition(ccp(222.0f, GameState::getInstance()->getBottomOffset() + 730.0f));
        pM->addChild(spTitle, 2);
        /*右功能（出售）*/
        CCMenuItemImage *spR = CCMenuItemImage::create("CM_right.png",
                                                       "CM_right.png",
                                                       this,
                                                       menu_selector(HerosLayer::saleHerosCallback));
        spR->setAnchorPoint(ccp(0, 0));
        spR->setPosition(ccp(482.0f, GameState::getInstance()->getBottomOffset() + 745.0f));
        CCMenu* pSpRMenu = CCMenu::create(spR, NULL);
        pSpRMenu->setAnchorPoint(CCPointZero);
        pSpRMenu->setPosition(CCPointZero);
        pR->addChild(pSpRMenu, 1);
        CCSprite* pSell = CCSprite::create("HL_sell.png");
        pSell->setAnchorPoint(ccp(0, 0));
        pSell->setPosition(ccp(532.0f, GameState::getInstance()->getBottomOffset() + 750.0f));
        pR->addChild(pSell, 2);
    } else if (2 == type)/*出售列表*/ {
        /*有返回*/
        CCMenuItemImage *spL = CCMenuItemImage::create("CM_left.png",
                                                       "CM_left.png",
                                                       this,
                                                       menu_selector(HerosLayer::returnFromList));
        spL->setAnchorPoint(ccp(0, 0));
        spL->setTag(RETURNMENU_TAG);
        spL->setPosition(ccp(0.0f, GameState::getInstance()->getBottomOffset() + 745.0f));
        CCMenu* pSpLMenu = CCMenu::create(spL, NULL);
        pSpLMenu->setAnchorPoint(CCPointZero);
        pSpLMenu->setPosition(CCPointZero);
        pL->addChild(pSpLMenu, 1);
        CCSprite* spLFont = CCSprite::create("HL_return.png");
        spLFont->setAnchorPoint(ccp(0, 0));
        spLFont->setPosition(ccp(12, GameState::getInstance()->getBottomOffset() + 750.0f));
        pL->addChild(spLFont, 2);
        /*显示当前列表标题*/
        CCSprite* spTitle = CCSprite::create("HL_sellHeros.png");
        spTitle->setAnchorPoint(ccp(0, 0));
        spTitle->setPosition(ccp(222.0f, GameState::getInstance()->getBottomOffset() + 730.0f));
        pM->addChild(spTitle, 2);
        /*右*/
        CCSprite* spR = CCSprite::create("CM_right.png");
        spR->setAnchorPoint(ccp(0, 0));
        spR->setPosition(ccp(482.0f, GameState::getInstance()->getBottomOffset() + 745.0f));
        pR->addChild(spR, 1);

    } else if (3 == type)/*选择队长列表*/ {
        /*有返回*/
        CCMenuItemImage *spL = CCMenuItemImage::create("CM_left.png",
                                                       "CM_left.png",
                                                       this,
                                                       menu_selector(HerosLayer::returnFromList));
        spL->setAnchorPoint(ccp(0, 0));
        spL->setTag(RETURNMENU_TAG + 1);
        spL->setPosition(ccp(0.0f, GameState::getInstance()->getBottomOffset() + 745.0f));
        CCMenu* pSpLMenu = CCMenu::create(spL, NULL);
        pSpLMenu->setAnchorPoint(CCPointZero);
        pSpLMenu->setPosition(CCPointZero);
        pL->addChild(pSpLMenu, 1);
        CCSprite* spLFont = CCSprite::create("HL_return.png");
        spLFont->setAnchorPoint(ccp(0, 0));
        spLFont->setPosition(ccp(12, GameState::getInstance()->getBottomOffset() + 750.0f));
        pL->addChild(spLFont, 2);
        /*显示当前列表标题*/
        CCSprite* spTitle = CCSprite::create("CM_chosCape.png");
        spTitle->setAnchorPoint(ccp(0, 0));
        spTitle->setPosition(ccp(222.0f, GameState::getInstance()->getBottomOffset() + 730.0f));
        pM->addChild(spTitle, 2);
        /*右*/
        CCSprite* spR = CCSprite::create("CM_right.png");
        spR->setAnchorPoint(ccp(0, 0));
        spR->setPosition(ccp(482.0f, GameState::getInstance()->getBottomOffset() + 745.0f));
        pR->addChild(spR, 1);
        
    } else if (4 == type)/*选择队员列表*/ {
        /*有返回*/
        CCMenuItemImage *spL = CCMenuItemImage::create("CM_left.png",
                                                       "CM_left.png",
                                                       this,
                                                       menu_selector(HerosLayer::returnFromList));
        spL->setAnchorPoint(ccp(0, 0));
        spL->setTag(RETURNMENU_TAG + 2);
        spL->setPosition(ccp(0.0f, GameState::getInstance()->getBottomOffset() + 745.0f));
        CCMenu* pSpLMenu = CCMenu::create(spL, NULL);
        pSpLMenu->setAnchorPoint(CCPointZero);
        pSpLMenu->setPosition(CCPointZero);
        pL->addChild(pSpLMenu, 1);
        CCSprite* spLFont = CCSprite::create("HL_return.png");
        spLFont->setAnchorPoint(ccp(0, 0));
        spLFont->setPosition(ccp(12, GameState::getInstance()->getBottomOffset() + 750.0f));
        pL->addChild(spLFont, 2);
        /*显示当前列表标题*/
        CCSprite* spTitle = CCSprite::create("CM_chosTeam.png");
        spTitle->setAnchorPoint(ccp(0, 0));
        spTitle->setPosition(ccp(222.0f, GameState::getInstance()->getBottomOffset() + 730.0f));
        pM->addChild(spTitle, 2);
        /*右*/
        CCSprite* spR = CCSprite::create("CM_right.png");
        spR->setAnchorPoint(ccp(0, 0));
        spR->setPosition(ccp(482.0f, GameState::getInstance()->getBottomOffset() + 745.0f));
        pR->addChild(spR, 1);
    } else if (5 == type)/*升级列表*/ {
        /*有返回*/
        CCMenuItemImage *spL = CCMenuItemImage::create("CM_left.png",
                                                       "CM_left.png",
                                                       this,
                                                       menu_selector(HerosLayer::returnFromList));
        spL->setAnchorPoint(ccp(0, 0));
        spL->setTag(RETURNMENU_TAG + 3);
        spL->setPosition(ccp(0.0f, GameState::getInstance()->getBottomOffset() + 745.0f));
        CCMenu* pSpLMenu = CCMenu::create(spL, NULL);
        pSpLMenu->setAnchorPoint(CCPointZero);
        pSpLMenu->setPosition(CCPointZero);
        pL->addChild(pSpLMenu, 1);
        CCSprite* spLFont = CCSprite::create("HL_return.png");
        spLFont->setAnchorPoint(ccp(0, 0));
        spLFont->setPosition(ccp(12, GameState::getInstance()->getBottomOffset() + 750.0f));
        pL->addChild(spLFont, 2);
        /*显示当前列表标题*/
        CCSprite* spTitle = CCSprite::create("CM_chosHero.png");
        spTitle->setAnchorPoint(ccp(0, 0));
        spTitle->setPosition(ccp(222.0f, GameState::getInstance()->getBottomOffset() + 730.0f));
        pM->addChild(spTitle, 2);
        /*右*/
        CCSprite* spR = CCSprite::create("CM_right.png");
        spR->setAnchorPoint(ccp(0, 0));
        spR->setPosition(ccp(482.0f, GameState::getInstance()->getBottomOffset() + 745.0f));
        pR->addChild(spR, 1);
    } else if (6 == type)/*升级被吞列表*/ {
        /*有返回*/
        CCMenuItemImage *spL = CCMenuItemImage::create("CM_left.png",
                                                       "CM_left.png",
                                                       this,
                                                       menu_selector(HerosLayer::returnFromList));
        spL->setAnchorPoint(ccp(0, 0));
        spL->setTag(RETURNMENU_TAG + 4);
        spL->setPosition(ccp(0.0f, GameState::getInstance()->getBottomOffset() + 745.0f));
        CCMenu* pSpLMenu = CCMenu::create(spL, NULL);
        pSpLMenu->setAnchorPoint(CCPointZero);
        pSpLMenu->setPosition(CCPointZero);
        pL->addChild(pSpLMenu, 1);
        CCSprite* spLFont = CCSprite::create("HL_return.png");
        spLFont->setAnchorPoint(ccp(0, 0));
        spLFont->setPosition(ccp(12, GameState::getInstance()->getBottomOffset() + 750.0f));
        pL->addChild(spLFont, 2);
        /*显示当前列表标题*/
        CCSprite* spTitle = CCSprite::create("CM_chosHero.png");
        spTitle->setAnchorPoint(ccp(0, 0));
        spTitle->setPosition(ccp(222.0f, GameState::getInstance()->getBottomOffset() + 730.0f));
        pM->addChild(spTitle, 2);
        /*右*/
        CCSprite* spR = CCSprite::create("CM_right.png");
        spR->setAnchorPoint(ccp(0, 0));
        spR->setPosition(ccp(482.0f, GameState::getInstance()->getBottomOffset() + 745.0f));
        pR->addChild(spR, 1);
    } else if (7 == type)/*升阶列表*/ {
        /*有返回*/
        CCMenuItemImage *spL = CCMenuItemImage::create("CM_left.png",
                                                       "CM_left.png",
                                                       this,
                                                       menu_selector(HerosLayer::returnFromList));
        spL->setAnchorPoint(ccp(0, 0));
        spL->setTag(RETURNMENU_TAG + 5);
        spL->setPosition(ccp(0.0f, GameState::getInstance()->getBottomOffset() + 745.0f));
        CCMenu* pSpLMenu = CCMenu::create(spL, NULL);
        pSpLMenu->setAnchorPoint(CCPointZero);
        pSpLMenu->setPosition(CCPointZero);
        pL->addChild(pSpLMenu, 1);
        CCSprite* spLFont = CCSprite::create("HL_return.png");
        spLFont->setAnchorPoint(ccp(0, 0));
        spLFont->setPosition(ccp(12, GameState::getInstance()->getBottomOffset() + 750.0f));
        pL->addChild(spLFont, 2);
        /*显示当前列表标题*/
        CCSprite* spTitle = CCSprite::create("CM_chosHero.png");
        spTitle->setAnchorPoint(ccp(0, 0));
        spTitle->setPosition(ccp(222.0f, GameState::getInstance()->getBottomOffset() + 730.0f));
        pM->addChild(spTitle, 2);
        /*右*/
        CCSprite* spR = CCSprite::create("CM_right.png");
        spR->setAnchorPoint(ccp(0, 0));
        spR->setPosition(ccp(482.0f, GameState::getInstance()->getBottomOffset() + 745.0f));
        pR->addChild(spR, 1);
    } else if (8 == type)/*pvp队长返回*/ {
        /*有返回*/
        CCMenuItemImage *spL = CCMenuItemImage::create("CM_left.png",
                                                       "CM_left.png",
                                                       this,
                                                       menu_selector(HerosLayer::returnFromList));
        spL->setAnchorPoint(ccp(0, 0));
        spL->setTag(RETURNMENU_TAG + 6);
        spL->setPosition(ccp(0.0f, GameState::getInstance()->getBottomOffset() + 745.0f));
        CCMenu* pSpLMenu = CCMenu::create(spL, NULL);
        pSpLMenu->setAnchorPoint(CCPointZero);
        pSpLMenu->setPosition(CCPointZero);
        pL->addChild(pSpLMenu, 1);
        CCSprite* spLFont = CCSprite::create("HL_return.png");
        spLFont->setAnchorPoint(ccp(0, 0));
        spLFont->setPosition(ccp(12, GameState::getInstance()->getBottomOffset() + 750.0f));
        pL->addChild(spLFont, 2);
        /*显示当前列表标题*/
        CCSprite* spTitle = CCSprite::create("CM_chosCape.png");
        spTitle->setAnchorPoint(ccp(0, 0));
        spTitle->setPosition(ccp(222.0f, GameState::getInstance()->getBottomOffset() + 730.0f));
        pM->addChild(spTitle, 2);
        /*右*/
        CCSprite* spR = CCSprite::create("CM_right.png");
        spR->setAnchorPoint(ccp(0, 0));
        spR->setPosition(ccp(482.0f, GameState::getInstance()->getBottomOffset() + 745.0f));
        pR->addChild(spR, 1);
    } else if (9 == type)/*pvp队员返回*/ {
        /*有返回*/
        CCMenuItemImage *spL = CCMenuItemImage::create("CM_left.png",
                                                       "CM_left.png",
                                                       this,
                                                       menu_selector(HerosLayer::returnFromList));
        spL->setAnchorPoint(ccp(0, 0));
        spL->setTag(RETURNMENU_TAG + 7);
        spL->setPosition(ccp(0.0f, GameState::getInstance()->getBottomOffset() + 745.0f));
        CCMenu* pSpLMenu = CCMenu::create(spL, NULL);
        pSpLMenu->setAnchorPoint(CCPointZero);
        pSpLMenu->setPosition(CCPointZero);
        pL->addChild(pSpLMenu, 1);
        CCSprite* spLFont = CCSprite::create("HL_return.png");
        spLFont->setAnchorPoint(ccp(0, 0));
        spLFont->setPosition(ccp(12, GameState::getInstance()->getBottomOffset() + 750.0f));
        pL->addChild(spLFont, 2);
        /*显示当前列表标题*/
        CCSprite* spTitle = CCSprite::create("CM_chosTeam.png");
        spTitle->setAnchorPoint(ccp(0, 0));
        spTitle->setPosition(ccp(222.0f, GameState::getInstance()->getBottomOffset() + 730.0f));
        pM->addChild(spTitle, 2);
        /*右*/
        CCSprite* spR = CCSprite::create("CM_right.png");
        spR->setAnchorPoint(ccp(0, 0));
        spR->setPosition(ccp(482.0f, GameState::getInstance()->getBottomOffset() + 745.0f));
        pR->addChild(spR, 1);
    }
}

void HerosLayer::returnFromList(CCObject* pSender) {
    CCMenuItemImage* pItemImage = (CCMenuItemImage*)pSender;
    CCNotificationCenter* pNotificationCenter = CCNotificationCenter::sharedNotificationCenter();
    switch (pItemImage->getTag()) {
        case RETURNMENU_TAG:
        {
            if (m_isSelectHeroMode ) {
                m_countOfSelectedHeros = 0;
                memset(m_szSelectedHeroUniId, '\0', sizeof(m_szSelectedHeroUniId));
                CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_CHOSCONFIRM_HIDE_STRING);
            }
            
            // 出售返回到武将列表
            m_pGameState->setCurrTagWhoCallHeroSel(WCH_BROWSE);
            pNotificationCenter->postNotification(ON_MESSAGE_HEROS_STRING);
        }
            break;
        case RETURNMENU_TAG + 1:
        {
            // 上阵选择队长
            pNotificationCenter->postNotification(ON_MESSAGE_MAINCITY_STRING);
        }
            break;
        case RETURNMENU_TAG + 2:
        {
            // 上阵选择队友
//            GameState::getInstance()->setTagWhoCallHeroSel(1000);
            /*如果返回*/
            CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_CHOSCONFIRM_HIDE_STRING);
            pNotificationCenter->postNotification(ON_MESSAGE_MAINCITY_STRING);
        }
            break;
        case RETURNMENU_TAG + 3:
        {
            // 升级界面
            pNotificationCenter->postNotification(ON_MESSAGE_LEVUP_STRING);
        }
            break;
        case RETURNMENU_TAG + 4:
        {
            // 升级界面
            CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_CHOSCONFIRM_HIDE_STRING);
            pNotificationCenter->postNotification(ON_MESSAGE_LEVUP_STRING);
        }
            break;
        case RETURNMENU_TAG + 5:
        {
            // 升阶界面
            pNotificationCenter->postNotification(ON_MESSAGE_LEVUP_STRING);
        }
            break;
        case RETURNMENU_TAG + 6:
        {
            // PVP上阵选择队长
            Battle::getInstance()->setBattlePkMode(BPM_PVP);
            if (Battle::getInstance()->isCWar()) {
                pNotificationCenter->postNotification(ON_MESSAGE_GUOZHAN_STRING);
            } else {
                pNotificationCenter->postNotification(ON_MESSAGE_TONGQUETAI_STRING);
            }
            
        }
            break;
        case RETURNMENU_TAG + 7:
        {
            // PVP上阵选择队友
            Battle::getInstance()->setBattlePkMode(BPM_PVP);
            CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_CHOSCONFIRM_HIDE_STRING);
            if (Battle::getInstance()->isCWar()) {
                pNotificationCenter->postNotification(ON_MESSAGE_GUOZHAN_STRING);
            } else {
                pNotificationCenter->postNotification(ON_MESSAGE_TONGQUETAI_STRING);
            }
        }
            break;
        default:
            break;
    }
}

void HerosLayer::clearSelectedHero() {
    m_countOfSelectedHeros = 0;
    memset(m_szSelectedHeroUniId, '\0', sizeof(m_szSelectedHeroUniId));
}