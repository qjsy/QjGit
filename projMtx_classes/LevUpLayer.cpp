//
//  LevUpLayer.cpp
//  hero
//
// 商店层（主界面首页）
// 隶属于主场景
//

#include "LevUpLayer.h"
#include "customMessage.h"
#include "GameState.h"

#include "HeroManager.h"
#include "HeroItem.h"
#include "HeroAdditionItem.h"
#include "HeroOfPlayerItem.h"
#include "NetConnection.h"
#include "GamePlayer.h"
#include "CardBox.h"


#define OPENNING_ANI_DURING                 0.2
#define ANIMATION_DURING                    0.75

#define CONTAINER_TAG                       1300        //整个层的TAG
#define LL_MENU_TAG                         1301        //tipMenu左边容器tag值
#define MID_MENU_TAG                        1302        //..
#define RR_MENU_TAG                         1303        //..
#define RETURN_TAG                          1304

#define LEV_TAG                             1305
#define EVO_TAG                             1306

#define INFO_TAG                            1307        // 武将升级升阶信息

#define SCROLLVIEW                          2001
#define CONTAINER_VIEW                      2002
#define BOARD                               2003
#define CHOS_LEVUP                          2004
#define CHOS_UPDATE                         2005        // 下方需要更新的按钮和城池

#define NOARG                               2
#define NEXT                                1
#define PREVIOUS                            0

#define ISLEVUP_TAG1                         3601        // 被升级武将按钮
#define GREEN_PLUS                           4000        // 绿色提示
#define LEVUP_TIPS                           4100        // 提示选择需要升级的武将按钮
#define EVOUP_TIPS                           4200        // 不允许进化提示


#define TAG_TUN                              8000        // 吞噬获得经验
#define TAG_GOLD                             8001        // 吞噬所需铜钱
#define TAG_MAX                              8002        // 满级所需经验
#define TAG_SAVE                             8003        // 城池省下铜钱
#define TAG_EVO                              8004        // 升阶所需铜钱
#define TAG_EVO_SAVE                         8005        // 升阶城池省下经验

#define MINI_CARD                            10000          // 头像卡（被吞噬）
#define COM_CARD                             10100          // 整卡（升级）
#define EVO_CARD                             10200          // 整卡（进化）
#define EVOH_CARD                            10300          // 整卡（进化后）

#define INFO_LEVUP                           10400          // 升级武将信息
#define INFO_EVOUP                           10401          // 进化武将信息

#define ALREADYMAXLEV                        5000           // 已经达到最大等级
#define LEVUP_COPPER                         5001           // 升级所需铜币状态
#define EVOUP_COPPER                         5002           // 升阶所需铜币状态
#define MAXEVO                               5003           // 最大等阶



/*
 *  构造函数
 */
LevUpLayer::LevUpLayer()
:moving(false),
active(false),
lastPos(ccp(0, 0)),
tempPos(0.0f),
curIndex(1),
offset(0.0f),
m_foodHeroForLevupArray(NULL),
stateForBtn(false),
m_isEvo(false),
m_isMoving(false)
,m_enableTouch(false)
,m_pGameState(GameState::getInstance())
{
    memset(m_heroForLevUp, '\0', sizeof(m_heroForLevUp));
    memset(m_heroForEvoUp, '\0', sizeof(m_heroForEvoUp));
}

/*
 *  析构函数
 */
LevUpLayer::~LevUpLayer() {
    
    CCTextureCache* pTextureCache = CCTextureCache::sharedTextureCache();
    
    pTextureCache->removeTextureForKey("LU_21.png");
    pTextureCache->removeTextureForKey("LU_view.png");
    pTextureCache->removeTextureForKey("LU_btn.png");
    pTextureCache->removeTextureForKey("LU_board.png");
    pTextureCache->removeTextureForKey("LU_fang.png");
    pTextureCache->removeTextureForKey("LU_plus.png");
    pTextureCache->removeTextureForKey("LU_addHero.png");
    pTextureCache->removeTextureForKey("LU_chang.png");
    pTextureCache->removeTextureForKey("LU_tipsChos.png");
    pTextureCache->removeTextureForKey("LU_levBz.png");
    pTextureCache->removeTextureForKey("CM_bigBtn.png");
    pTextureCache->removeTextureForKey("LU_sel_lev.png");
    pTextureCache->removeTextureForKey("LU_tun.png");
    pTextureCache->removeTextureForKey("LU_font_levup.png");
    pTextureCache->removeTextureForKey("LU_zhancity.png");
    pTextureCache->removeTextureForKey("LU_sel_evo.png");
    pTextureCache->removeTextureForKey("CM_bigBtnh.png");
    pTextureCache->removeTextureForKey("LU_font_evoup.png");
    pTextureCache->removeTextureForKey("CM_left.png");
    pTextureCache->removeTextureForKey("CM_mid.png");
    pTextureCache->removeTextureForKey("LU_max_lev.png");
    pTextureCache->removeTextureForKey("LU_min_lev.png");
    pTextureCache->removeTextureForKey("LU_min_evo.png");
    pTextureCache->removeTextureForKey("LU_max_evo.png");
    pTextureCache->removeTextureForKey("CM_right.png");
    pTextureCache->removeTextureForKey("LU_return.png");
    pTextureCache->removeTextureForKey("LU_min_lev.png");
    pTextureCache->removeTextureForKey("LU_font_bg.png");
    pTextureCache->removeTextureForKey("LU_islevget.png");
    pTextureCache->removeTextureForKey("LU_islevcop.png");
    pTextureCache->removeTextureForKey("LU_maxexp.png");
    pTextureCache->removeTextureForKey("LU_save.png");
    pTextureCache->removeTextureForKey("LU_evogold.png");
}

/*
 *  初始化
 */
bool LevUpLayer::init() {
    // super init
    if (!CCLayer::init()) {
        return false;
    }
    
    /*********************************************
     添加容器,所有随层同步移入主场景的内容全部加入容器中
     *********************************************/
    CCNode* pContainer = CCNode::create();
    this->addChild(pContainer, 1, CONTAINER_TAG);
    // 升级背景图
    CCSprite* pBgSprite = CCSprite::create("LU_21.png");
    pBgSprite->setAnchorPoint(CCPointZero);
    pBgSprite->setPosition(ccp(0.0f, GameState::getInstance()->getBottomOffset() + 124.0f));
    pContainer->addChild(pBgSprite, 1);
    
    /*
     *  所有升级初始化的内容,需要随view移动的加入scrollview的container中
     */
    const float Btm = GameState::getInstance()->getBottomOffset();
    CCSprite* viewBg = CCSprite::create("LU_view.png");
    viewBg->setAnchorPoint(ccp(0, 0));
    viewBg->setPosition(ccp(0, 0));
    CCSize viewSize = viewBg->getContentSize();
    scrollView = CCScrollView::create(CCSizeMake(viewSize.width, viewSize.height));
    scrollView->setContentSize(viewSize);
    // 将背景加入容器
    scrollView->getContainer()->addChild(viewBg);
    scrollView->setAnchorPoint(ccp(0, 0));
    scrollView->setPosition(ccp(0,  Btm + 300.0f));
    pContainer->addChild(scrollView, 10);
    scrollView->setDirection(kCCScrollViewDirectionHorizontal);
    scrollView->setBounceable(false);
    scrollView->setDelegate(this);
    // 添加图标
    CCSprite* spr = CCSprite::create("LU_btn.png");
    spr->setAnchorPoint(ccp(0, 0));
    spr->setPosition(ccp(290, 225));
    scrollView->getContainer()->addChild(spr);
    // 添加移动挡板
    CCSprite* board = CCSprite::create("LU_board.png");
    board->setAnchorPoint(ccp(0, 0));
    board->setPosition(ccp(600, 65)); //board1->setPosition(ccp(400, 65));
    scrollView->getContainer()->addChild(board, 1, BOARD);
    // 添加选择被吞噬武将的按钮
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 3; ++j) {
            CCMenuItemImage *pChosIsLeved = CCMenuItemImage::create( "LU_fang.png",
                                    "LU_fang.png", this, menu_selector(LevUpLayer::onSelFoodForLevupHero));
            pChosIsLeved->setAnchorPoint(CCPointZero);
            pChosIsLeved->setPosition(ccp(38 + i * 132, 318 - j * 116));
            CCMenu* pMenu = CCMenu::create(pChosIsLeved, NULL);
            pMenu->setAnchorPoint(ccp(0, 0));
            pMenu->setPosition(ccp(0, 0));
            CCSprite* plus = CCSprite::create("LU_plus.png");
            plus->setAnchorPoint(ccp(0, 0));
            plus->setPosition(ccp(113 + i * 132, 393 - j * 116));
            scrollView->getContainer()->addChild(plus, 2);
            CCSprite* plusGre = CCSprite::create("LU_addHero.png");
            plusGre->setAnchorPoint(ccp(0, 0));
            plusGre->setPosition(ccp(113 + i * 132, 393 - j * 116));
            scrollView->getContainer()->addChild(plusGre, 3, GREEN_PLUS - 1 + 2*(j-1) + i);
            scrollView->getContainer()->addChild(pMenu, 1,ISLEVUP_TAG1 - 1 + 2*(j-1) + i);
        }
    }
    
    // 选择吞噬武将的武将按钮,按钮和以后的武将都放入容器pChosLevUpLayer
    CCNode* pChosLevUpLayer = CCNode::create();
    pChosLevUpLayer->setAnchorPoint(ccp(0, 0));
    pChosLevUpLayer->setPosition(ccp(375, 80));
    CCMenuItemImage *pChosLevUp = CCMenuItemImage::create( "LU_chang.png",
                                                           "LU_chang.png",
                                                            this,
                                                            menu_selector(LevUpLayer::onSelLevEvoUpHero));
    pChosLevUp->setAnchorPoint(CCPointZero);
    pChosLevUp->setPosition(ccp(0, 0));
    CCMenu* pMenu = CCMenu::create(pChosLevUp, NULL);
    pMenu->setAnchorPoint(ccp(0, 0));
    pMenu->setPosition(ccp(0, 0));
    CCSprite* plus = CCSprite::create("LU_addHero.png");
    plus->setAnchorPoint(ccp(0, 0));
    plus->setPosition(ccp(195, 305));
    
    CCActionInterval * fadein = CCFadeIn::create(ANIMATION_DURING);
    CCActionInterval * fadeout = CCFadeOut::create(ANIMATION_DURING);
    CCFiniteTimeAction* seq = CCSequence::create(fadein, fadeout, NULL);
    CCActionInterval * repeatForever =CCRepeatForever::create((CCActionInterval* )seq);
    plus->runAction(repeatForever);
    pChosLevUpLayer->addChild(plus, 2);
    pChosLevUpLayer->addChild(pMenu);
    scrollView->getContainer()->addChild(pChosLevUpLayer, 2, CHOS_LEVUP);
    CCSprite* chosTips = CCSprite::create("LU_tipsChos.png");
    chosTips->setAnchorPoint(ccp(0, 0));
    chosTips->setPosition(ccp(50, 145));
    CCActionInterval * fadein1 = CCFadeIn::create(ANIMATION_DURING);
    CCActionInterval * fadeout1 = CCFadeOut::create(ANIMATION_DURING);
    CCFiniteTimeAction* seq1 = CCSequence::create(fadein1, fadeout1, NULL);
    CCActionInterval * repeatForever1 =CCRepeatForever::create((CCActionInterval* )seq1);
    chosTips->runAction(repeatForever1);
    pChosLevUpLayer->addChild(chosTips, 2, LEVUP_TIPS);
    
    CCSprite* cannotEvoTips = CCSprite::create("LU_levBz.png");
    cannotEvoTips->setAnchorPoint(ccp(0, 0));
    cannotEvoTips->setPosition(ccp(460, 245));
    cannotEvoTips->setColor(ccc3(255, 0, 0));
    cannotEvoTips->setVisible(false);
    scrollView->getContainer()->addChild(cannotEvoTips, 50, EVOUP_TIPS);
    
    /*升级后已经满级*/
    CCSprite* alreadyMaxLev = CCSprite::create("LU_alMaxLev.png");
//    CCLabelTTF* alreadyMaxLev = CCLabelTTF::create("当前最大等级", "Arial", 21);
    alreadyMaxLev->setAnchorPoint(ccp(0, 0));
    alreadyMaxLev->setPosition(ccp(435, 255));
    alreadyMaxLev->setColor(ccc3(255, 0, 0));
    alreadyMaxLev->setVisible(false);
    scrollView->getContainer()->addChild(alreadyMaxLev, 50, ALREADYMAXLEV);
    
    CCLabelTTF* lessCopForLev = CCLabelTTF::create("铜币不足", "Arial", 19);
    lessCopForLev->setAnchorPoint(ccp(0, 0));
    lessCopForLev->setPosition(ccp(560, 43));
    lessCopForLev->setColor(ccc3(255, 0, 0));
    lessCopForLev->setVisible(false);
    scrollView->getContainer()->addChild(lessCopForLev, 50, LEVUP_COPPER);
    // 最大等阶
    CCLabelTTF* maxEvo = CCLabelTTF::create("最大等阶", "Arial", 19);
    maxEvo->setAnchorPoint(ccp(0, 0));
    maxEvo->setPosition(ccp(460, 245));
    maxEvo->setColor(ccc3(255, 0, 0));
    maxEvo->setVisible(false);
    scrollView->getContainer()->addChild(maxEvo, 50, MAXEVO);
    
    CCLabelTTF* lessCopForEvo = CCLabelTTF::create("铜币不足", "Arial", 19);
    lessCopForEvo->setAnchorPoint(ccp(0, 0));
    lessCopForEvo->setPosition(ccp(865, 35));
    lessCopForEvo->setColor(ccc3(255, 0, 0));
    lessCopForEvo->setVisible(false);
    scrollView->getContainer()->addChild(lessCopForEvo, 50, EVOUP_COPPER);

    // 进化后的武将
    CCSprite* pEvoUp = CCSprite::create("LU_chang.png");
    pEvoUp->setAnchorPoint(ccp(0, 0));
    pEvoUp->setPosition(ccp(800, 80));
    scrollView->getContainer()->addChild(pEvoUp);
    
    CCSprite* plusEvo = CCSprite::create("LU_plus.png");
    plusEvo->setAnchorPoint(ccp(0, 0));
    plusEvo->setPosition(ccp(990, 385));
    scrollView->getContainer()->addChild(plusEvo, 2);
    
    ////////首先更新在第一页///////////
    update(0);
    ////////首先更新在第一页///////////
    
    return true;
}

/*
 *
 */
void LevUpLayer::setVisible(bool visible) {
    ////////////////////////////////
    // super first
    CCLayer::setVisible(visible);
    //setTouchEnabled(visible);
    /*武将列表置空,不置空会出错*/
    m_pGameState->setTagWhoCallHeroSel(WCH_NONE);
    
    if (visible) {
        if (curIndex == 1) {
            update(0);
        } else if (2 == curIndex) {
            update(1);
        }
    } else {
        
    }
}

/* 
 * 更新武将相关 UI 组件 
 */
void LevUpLayer::_updateHeroUiComponent() {
    char buffer[64];
    ///////////////////////
    //    显示被吞武将     //
    ///////////////////////
    for (int i = 0; i < MAX_FOOD_HEROS_FOR_LEVUP; i++) {
        /*minCard:被吞噬武将头像卡*/
        CardBox* minCard = (CardBox*)scrollView->getContainer()->getChildByTag(MINI_CARD + i);
        if (minCard)
            minCard->setVisible(false);
    }
    
    // 显示已经选定的用于升级的被吞噬武将
    HeroManager* pHeroManager = HeroManager::getInstance();
    unsigned int index = 0;/*被吞英雄的数量计数*/
    int getExpForEat = 0;
    
    CCObject* pObject = NULL;
    CCARRAY_FOREACH(this->m_foodHeroForLevupArray, pObject) {
        HeroOfPlayerItem* pItem = (HeroOfPlayerItem*)pObject;
        if (pItem) {
            std::string pic = pItem->getHeroItem()->getHIcon();
            int level = pItem->getLevel();
            int raity  = pItem->getHeroItem()->getRarity();
            /*吞噬获得经验*/
            getExpForEat += pHeroManager->getAdditionHeroData(raity, level)->getExpByFood();
            /*排列方式*/
            unsigned int col = index % 2;
            unsigned int row = (index - col) / 2;
            /*武将卡显示*/
            CardBox* minCard = (CardBox*)scrollView->getContainer()->getChildByTag(MINI_CARD + index);
            if (NULL == minCard) {
                minCard = CardBox::create();
                minCard->setCard(1, pItem, true);
                minCard->setAnchorPoint(ccp(0, 0));
                minCard->setPosition(ccp(40 + col * 132, 315 - row * 116));
                scrollView->getContainer()->addChild(minCard, 10, MINI_CARD + index);
            } else {
                minCard->setCard(1, pItem, true);
                minCard->setVisible(true);
            }
         }
        index++;
    } /*for*/
    
    // 吞噬武将获得经验
    sprintf(buffer, "%d", getExpForEat);
    CCLabelTTF* pGetExp = (CCLabelTTF*)scrollView->getContainer()->getChildByTag(INFO_TAG)->getChildByTag(TAG_TUN);
    if (getExpForEat != 0) {
        if (pGetExp)
            pGetExp->setString(buffer);
    } else {
        if (pGetExp)
            pGetExp->setString("");
    }
    
    ///////////////////////
    //    显示升级武将     //
    ///////////////////////
    /*升级武将uniid*/
    HeroOfPlayerItem* pItem = HeroManager::getInstance()->getHeroDataByUniId(m_heroForLevUp);
    /*升级武将卡整卡*/
    CardBox* comCard = (CardBox*)scrollView->getContainer()->getChildByTag(CHOS_LEVUP)->getChildByTag(COM_CARD);
    /* 升级武将详细信息 */
//    CCSprite* levInfo = (CCSprite*)scrollView->getContainer()->getChildByTag(CHOS_LEVUP)->getChildByTag(INFO_LEVUP);
   
    if (pItem) {
        // 满级所需经验 //
        int needMaxExp   = 0;
        int max1 = 0;
        int max2 = 0;
        int level = pItem->getLevel();
        int raity = pItem->getHeroItem()->getRarity();
        for (int i = 0; i < level; ++i) {
            max1 += pHeroManager->getAdditionHeroData(raity, i+1)->getExpForLevUp();
        }
        GamePlayer* pGamePlayer = GamePlayer::getInstance();
        int playerLev = pGamePlayer->getLevel();
        for (int i = 0; i < playerLev; ++i) {
            max2 += pHeroManager->getAdditionHeroData(raity, i+1)->getExpForLevUp();
        }
        needMaxExp = max2 - max1;
        sprintf(buffer, "%d", needMaxExp);
        CCLabelTTF* maxExp = (CCLabelTTF*)scrollView->getContainer()->getChildByTag(INFO_TAG)
        ->getChildByTag(TAG_MAX);
        if (needMaxExp != 0) {
            if (maxExp)
                maxExp->setString(buffer);
        } else {
            if (maxExp)
                maxExp->setString("");
        }
        // 吞噬所需铜钱
        int getNeedGold  = 0;
        getNeedGold = (pHeroManager->getAdditionHeroData(raity, level)->getGoldByFood()) * index;
        // 吞噬所需铜币
        sprintf(buffer, "%d", getNeedGold);
        CCLabelTTF* needGold = (CCLabelTTF*)scrollView->getContainer()->getChildByTag(INFO_TAG)->getChildByTag(TAG_GOLD);
        if (getNeedGold != 0) {
            if (needGold) {
                needGold->setString(buffer);
                if (getNeedGold > GamePlayer::getInstance()->getMoney()) {
                    needGold->setColor(ccc3(255, 0, 0));
                } else {
                    needGold->setColor(ccc3(255, 255, 255));
                }
            }
            GameState::getInstance()->setTunNeedMoney(getNeedGold);
        } else {
            if (needGold)
                needGold->setString("");
        }
        
        /* 可以获得的等级数 */
        int getLev = 0;
        if (getExpForEat >= HeroManager::getInstance()->getAdditionHeroData(pItem->getHeroItem()->getRarity(), pItem->getLevel())->getExpForLevUp() - pItem->getExp()) {
            getExpForEat -= (HeroManager::getInstance()->getAdditionHeroData(pItem->getHeroItem()->getRarity(), pItem->getLevel())->getExpForLevUp() - pItem->getExp());
            getLev++;
            while (getExpForEat > 0) {
                getExpForEat -= HeroManager::getInstance()->getAdditionHeroData(pItem->getHeroItem()->getRarity(), pItem->getLevel() + getLev )->getExpForLevUp();
                if (getExpForEat > 0)
                    getLev++;
            }
        }
        if (comCard) {
            comCard->setCard(3, pItem, false, true, getLev);
            comCard->setVisible(true);
        } else {
            comCard = CardBox::create();
            comCard->setCard(3, pItem, false, true, getLev);
            comCard->setAnchorPoint(ccp(0, 0));
            comCard->setPosition(ccp(-3, 60));
            scrollView->getContainer()->getChildByTag(CHOS_LEVUP)->addChild(comCard, 11, COM_CARD);
        }
    } else {
        if (comCard)
            comCard->setVisible(false);
    }

    ///////////////////////
    //    显示进化武将     //
    ///////////////////////
    pItem = NULL;
    pItem = HeroManager::getInstance()->getHeroDataByUniId(m_heroForEvoUp);
    CardBox* evoCard = (CardBox*)scrollView->getContainer()->getChildByTag(EVO_CARD);
    if (pItem) {
        // 升阶所需铜币，有的不需要铜币
        int nedEvoGOld = 0;
        nedEvoGOld = pItem->getHeroItem()->getUg();
        sprintf(buffer, "%d", nedEvoGOld);
        CCLabelTTF* needGoldLabel = (CCLabelTTF*)scrollView->getContainer()->getChildByTag(INFO_TAG)->getChildByTag(TAG_EVO);
        if (nedEvoGOld != 0) {
            if (needGoldLabel)
                needGoldLabel->setString(buffer);
            GameState::getInstance()->setEvoNeedMoney(nedEvoGOld);
        } else {
            if (needGoldLabel)
                needGoldLabel->setString("");
        }
        
        /*进化武将卡*/
        if (evoCard) {
            evoCard->setCard(3, pItem, false, true, 0);
            evoCard->setVisible(true);
        } else {
            evoCard = CardBox::create();
            evoCard->setCard(3, pItem, false, true, 0);
            evoCard->setAnchorPoint(ccp(0, 0));
            evoCard->setPosition(ccp(413, 140));
            scrollView->getContainer()->addChild(evoCard, 11, EVO_CARD);
        }
    } else {
        if (evoCard){
             evoCard->setVisible(false);
        }
    }
    
    //////////////////////////
    //    显示进化获得武将     //
    //////////////////////////
    CardBox* evohCard = (CardBox*)scrollView->getContainer()->getChildByTag(EVOH_CARD);
    int idNext = 0;
    if (pItem)
        idNext = pItem->getHeroItem()->getUt();

        HeroItem* pItemNext = NULL;
        pItemNext = HeroManager::getInstance()->getHeroItem(idNext);
        if (pItemNext) {
            /*进化获得武将卡*/
            if (evohCard) {
                evohCard->setCard(3, pItem, false, true, -10, pItemNext, m_isEvo);
                evohCard->setVisible(true);
            } else {
                evohCard = CardBox::create();
                evohCard->setCard(3, pItem, false, true, -10, pItemNext, m_isEvo);
                evohCard->setAnchorPoint(ccp(0, 0));
                evohCard->setPosition(ccp(786, 142));
                scrollView->getContainer()->addChild(evohCard, 11, EVOH_CARD);
            }
        } else {
            if (evohCard)
                evohCard->setVisible(false);
        }

        pItem = NULL;
        pItem = HeroManager::getInstance()->getHeroDataByUniId(m_heroEvoUp);

        if (pItem) {
            /*进化获得武将卡*/
            if (evohCard) {
                evohCard->setCard(3, pItem, false, true, 0);
                evohCard->setVisible(true);
            } else {
                evohCard = CardBox::create();
                evohCard->setCard(3, pItem, false, true, 0);
                evohCard->setAnchorPoint(ccp(0, 0));
                evohCard->setPosition(ccp(786, 142));
                scrollView->getContainer()->addChild(evohCard, 11, EVOH_CARD);
            }
        } else {
//            if (evohCard)
//                evohCard->setVisible(false);
        }
}

/*
 * 页面更新,滑动结束后的回调函数
 */
void LevUpLayer::update(int mode) {
    /*
     * 升级页面移动
     */
    CCNode* chosLevUpLayer = (CCNode*)scrollView->getContainer()->getChildByTag(CHOS_LEVUP);
    if (0 == mode) {
        chosLevUpLayer->runAction(CCMoveTo::create(OPENNING_ANI_DURING - 0.1, CCPointMake(375, 80)));
    } else if( 1 == mode) {
        chosLevUpLayer->runAction(CCMoveTo::create(OPENNING_ANI_DURING - 0.1, CCPointMake(410, 80)));
    }
    
    /* 
     * 更新滚动视图下方按钮
     * 在此新建按钮，并清除原来按钮
     */
    if (this->getChildByTag(CONTAINER_TAG)->getChildByTag(CHOS_UPDATE)) {
        this->getChildByTag(CONTAINER_TAG)->removeChildByTag(CHOS_UPDATE);
    }
    
    CCNode* node = CCNode::create();
    node->setAnchorPoint(ccp(0, 0));
    node->setPosition(ccp(0, GameState::getInstance()->getBottomOffset()));
    if ( 0 == mode) {
        // 选择被升级武将, 如果没有选升级武将则不能选被升级武将
        if (NULL == HeroManager::getInstance()->getHeroDataByUniId(m_heroForLevUp)) {
            CCMenuItemImage *pIsChosLevUp = CCMenuItemImage::create( "CM_bigBtn.png",
                                                                     "CM_bigBtn.png",
                                                                  this,
                                                                  menu_selector(LevUpLayer::onSelLevEvoUpHero));
            pIsChosLevUp->setAnchorPoint(CCPointZero);
            pIsChosLevUp->setPosition(ccp(60, 140));
            CCMenu* pIsMenu = CCMenu::create(pIsChosLevUp, NULL);
            pIsMenu->setAnchorPoint(ccp(0, 0));
            pIsMenu->setPosition(ccp(0, 0));
            CCSprite* fontIs = CCSprite::create("LU_sel_lev.png");
            fontIs->setAnchorPoint(ccp(0, 0));
            fontIs->setPosition(ccp(85, 150));
            node->addChild(fontIs, 11);
            node->addChild(pIsMenu, 10);
        } else {
            CCMenuItemImage *pIsChosLevUp = CCMenuItemImage::create( "CM_bigBtn.png",
                                                                     "CM_bigBtn.png",
                                                                  this,
                                                                  menu_selector(LevUpLayer::onSelFoodForLevupHero));
            pIsChosLevUp->setAnchorPoint(CCPointZero);
            pIsChosLevUp->setPosition(ccp(60, 140));
            CCMenu* pIsMenu = CCMenu::create(pIsChosLevUp, NULL);
            pIsMenu->setAnchorPoint(ccp(0, 0));
            pIsMenu->setPosition(ccp(0, 0));
            CCSprite* fontIs = CCSprite::create("LU_tun.png");
            fontIs->setAnchorPoint(ccp(0, 0));
            fontIs->setPosition(ccp(85, 149));
            node->addChild(fontIs, 11);
            node->addChild(pIsMenu, 10);
        }
        
        // 武将升级按钮
        CCMenuItemImage *pChosLevUp = CCMenuItemImage::create("CM_bigBtn.png",
                                                              "CM_bigBtn.png",
                                                              "CM_bigBtnh.png",
                                                              this,
                                                              menu_selector(LevUpLayer::onHeroLevUp));
        pChosLevUp->setAnchorPoint(CCPointZero);
        pChosLevUp->setPosition(ccp(350, 140));
        CCMenu* pMenu = CCMenu::create(pChosLevUp, NULL);
        pMenu->setAnchorPoint(ccp(0, 0));
        pMenu->setPosition(ccp(0, 0));
        CCSprite* font = CCSprite::create("LU_font_levup.png");
        font->setAnchorPoint(ccp(0, 0));
        font->setPosition(ccp(410, 150));
        node->addChild(font, 11);
        node->addChild(pMenu, 10);
        // 城池显示
        CCSprite* pFontCity = CCSprite::create("LU_zhancity.png");
        pFontCity->setAnchorPoint(ccp(0, 0));
        pFontCity->setPosition(ccp(60, 230));
        node->addChild(pFontCity);
        // 判断灰态,武将升级
        int rarity = 0;
        int level = 0;
        HeroOfPlayerItem* pItem = HeroManager::getInstance()->getHeroDataByUniId(m_heroForLevUp);
        if (pItem) {
            rarity = pItem->getHeroItem()->getRarity();
            level = pItem->getLevel();
        }
        CCLabelTTF* lessCopForLev = (CCLabelTTF*)scrollView->getContainer()->getChildByTag(LEVUP_COPPER);
        if (NULL != pItem && 0 != m_foodHeroForLevupArray->count())/*已选被吞噬武将计算铜币*/ {
            if (GamePlayer::getInstance()->getMoney() < HeroManager::getInstance()->getAdditionHeroData(rarity, level)->getGoldByFood() * m_foodHeroForLevupArray->count())/*铜币不足*/ {
                lessCopForLev->setVisible(true);
                CCFiniteTimeAction* seq = CCSequence::create(CCFadeOut::create(ANIMATION_DURING),
                                                             CCFadeIn::create(ANIMATION_DURING), NULL);
                lessCopForLev->runAction(CCRepeatForever::create((CCActionInterval* )seq));
                /*无法升级*/
                pChosLevUp->setEnabled(false);
                
            } else/*铜币足够*/ {
                pChosLevUp->setEnabled(true);
                lessCopForLev->setVisible(false);
            }
        } else {
            pChosLevUp->setEnabled(false);
            lessCopForLev->setVisible(false);
        }
        // 让6按钮不可点击
        CCSprite* alreadyMaxLev = (CCSprite*)scrollView->getContainer()->getChildByTag(ALREADYMAXLEV);
        if (NULL == pItem){
            for (int i = 0; i < 2; ++i) {
                for (int j = 0; j < 3; ++j) {
                    CCMenu* s = (CCMenu*) scrollView->getContainer()->getChildByTag(ISLEVUP_TAG1 - 1 + 2*(j-1) + i);
                    s->setEnabled(false);
                    // 绿色提示加号
                    CCSprite* sp = (CCSprite*)scrollView->getContainer()->getChildByTag(GREEN_PLUS - 1 + 2*(j-1) + i);
                    sp->setVisible(false);
                    sp->stopAllActions();
                }
            }
            alreadyMaxLev->setVisible(false);
        } else/*选择了要升级武将*/ {
            if (pItem->getLevel() < GamePlayer::getInstance()->getLevel())/*武将等级小于玩家等级*/ {
                for (int i = 0; i < 2; ++i) {
                    for (int j = 0; j < 3; ++j) {
                        CCMenu* s = (CCMenu*) scrollView->getContainer()->getChildByTag(ISLEVUP_TAG1 - 1 + 2*(j-1) + i);
                        s->setEnabled(true);
                        CCSprite* sp = (CCSprite*)scrollView->getContainer()->getChildByTag(GREEN_PLUS - 1 + 2*(j-1) + i);
                        sp->setVisible(true);
                        CCActionInterval * fadein = CCFadeIn::create(ANIMATION_DURING);
                        CCActionInterval * fadeout = CCFadeOut::create(ANIMATION_DURING);
                        CCFiniteTimeAction* seq = CCSequence::create(fadein, fadeout, NULL);
                        CCActionInterval * repeatForever =CCRepeatForever::create((CCActionInterval* )seq);
                        sp->runAction(repeatForever);
                    }
                }
                alreadyMaxLev->setVisible(false);
            } else/*>=*/ {
                for (int i = 0; i < 2; ++i) {
                    for (int j = 0; j < 3; ++j) {
                        CCMenu* s = (CCMenu*) scrollView->getContainer()->getChildByTag(ISLEVUP_TAG1 - 1 + 2*(j-1) + i);
                        s->setEnabled(false);
                        // 绿色提示加号
                        CCSprite* sp = (CCSprite*)scrollView->getContainer()->getChildByTag(GREEN_PLUS - 1 + 2*(j-1) + i);
                        sp->setVisible(false);
                        sp->stopAllActions();
                    }
                }
                pChosLevUp->setEnabled(false);
                alreadyMaxLev->setVisible(true);
                CCFiniteTimeAction* seq = CCSequence::create(CCFadeOut::create(ANIMATION_DURING),
                                                             CCFadeIn::create(ANIMATION_DURING), NULL);
                alreadyMaxLev->runAction(CCRepeatForever::create((CCActionInterval* )seq));
            }
        }
        /*升阶界面等级不足提示不显示*/
        CCSprite* cannotEvoTips = (CCSprite*)scrollView->getContainer()->getChildByTag(EVOUP_TIPS);
        cannotEvoTips->setVisible(false);
        /*升阶界面的铜币不足不显示*/
        CCLabelTTF* lessForEvo = (CCLabelTTF*)scrollView->getContainer()->getChildByTag(EVOUP_COPPER);
        lessForEvo->setVisible(false);
        /*升阶界面的最大等阶不显示*/
        CCLabelTTF* maxEvo = (CCLabelTTF*)scrollView->getContainer()->getChildByTag(MAXEVO);
        maxEvo->setVisible(false);
        
    } else if (1 == mode) {
        // 选择被进化武将
        CCMenuItemImage *pIsChosLevUp = CCMenuItemImage::create("CM_bigBtn.png",
                                                                "CM_bigBtn.png",
                                                                this,
                                                                menu_selector(LevUpLayer::onSelLevEvoUpHero));
        pIsChosLevUp->setAnchorPoint(CCPointZero);
        pIsChosLevUp->setPosition(ccp(60, 140));
        CCMenu* pIsMenu = CCMenu::create(pIsChosLevUp, NULL);
        pIsMenu->setAnchorPoint(ccp(0, 0));
        pIsMenu->setPosition(ccp(0, 0));
        CCSprite* fontIs = CCSprite::create("LU_sel_evo.png");
        fontIs->setAnchorPoint(ccp(0, 0));
        fontIs->setPosition(ccp(85, 150));
        node->addChild(fontIs, 11);
        node->addChild(pIsMenu, 10);
        // 武将进化按钮
        CCMenuItemImage *pChosLevUp = CCMenuItemImage::create("CM_bigBtn.png",
                                                              "CM_bigBtn.png",
                                                              "CM_bigBtnh.png",
                                                              this,
                                                              menu_selector(LevUpLayer::onHeroEvoUp));
        pChosLevUp->setAnchorPoint(CCPointZero);
        pChosLevUp->setPosition(ccp(350, 140));
        CCMenu* pMenu = CCMenu::create(pChosLevUp, NULL);
        pMenu->setAnchorPoint(ccp(0, 0));
        pMenu->setPosition(ccp(0, 0));
        CCSprite* font = CCSprite::create("LU_font_evoup.png");
        font->setAnchorPoint(ccp(0, 0));
        font->setPosition(ccp(410, 150));
        node->addChild(font, 11);
        node->addChild(pMenu, 10);
        
        /*判断是否允许进化*/
        HeroOfPlayerItem* pItem = NULL;
        pItem = HeroManager::getInstance()->getHeroDataByUniId(m_heroForEvoUp);
        CCSprite* cannotEvoTips = (CCSprite*)scrollView->getContainer()->getChildByTag(EVOUP_TIPS);
        CCLabelTTF* lessForEvo = (CCLabelTTF*)scrollView->getContainer()->getChildByTag(EVOUP_COPPER);
        CCLabelTTF* maxEvo = (CCLabelTTF*)scrollView->getContainer()->getChildByTag(MAXEVO);
        
        if (pItem == NULL) {
            cannotEvoTips->setVisible(false);
            pChosLevUp->setEnabled(false);
            maxEvo->setVisible(false);
            m_isEvo = false;
        } else/*选择了进化武将*/ {
            if (pItem->getHeroItem()->getUl() <= pItem->getLevel() && pItem->getHeroItem()->getUt()
                && pItem->getHeroItem()->getUg() <= GamePlayer::getInstance()->getMoney() /*&&进化道具*/)/*满足进化条件*/ {
                m_isEvo = true;/*可以进化*/
                pChosLevUp->setEnabled(true);
                cannotEvoTips->setVisible(false);
                lessForEvo->setVisible(false);
                maxEvo->setVisible(false);
            } else/*不可进化*/ {
                pChosLevUp->setEnabled(false);
                m_isEvo = false;
                
                if (pItem->getHeroItem()->getUl() > pItem->getLevel() && pItem->getHeroItem()->getUt())/*等级不足*/ {
                    cannotEvoTips->setVisible(true);/*显示等级不足提示*/
                    CCFiniteTimeAction* seq2 = CCSequence::create(CCFadeIn::create(ANIMATION_DURING), CCFadeOut::create(ANIMATION_DURING), NULL);
                    CCActionInterval * repeatForever2 =CCRepeatForever::create((CCActionInterval* )seq2);
                    cannotEvoTips->runAction(repeatForever2);
                } else {
                    cannotEvoTips->setVisible(false);
                }
                if (!pItem->getHeroItem()->getUt())/*已经到达最顶阶*/ {
                    maxEvo->setVisible(true);
                    CCFiniteTimeAction* seq2 = CCSequence::create(CCFadeIn::create(ANIMATION_DURING), CCFadeOut::create(ANIMATION_DURING), NULL);
                    CCActionInterval * repeatForever2 =CCRepeatForever::create((CCActionInterval* )seq2);
                    maxEvo->runAction(repeatForever2);
                } else {
                    maxEvo->setVisible(false);
                }
                if (pItem->getHeroItem()->getUg() > GamePlayer::getInstance()->getMoney())/*铜币不足*/ {
                    lessForEvo->setVisible(true);
                    CCFiniteTimeAction* seq = CCSequence::create(CCFadeOut::create(ANIMATION_DURING),
                                                                 CCFadeIn::create(ANIMATION_DURING), NULL);
                    lessForEvo->runAction(CCRepeatForever::create((CCActionInterval* )seq));
                } else {
                    lessForEvo->setVisible(false);
                }
                /*道具不足*/
            }
        }
        
        
        /*升阶页面已经满级不显示*/
        CCSprite* alreadyMaxLev = (CCSprite*)scrollView->getContainer()->getChildByTag(ALREADYMAXLEV);
        alreadyMaxLev->setVisible(false);
        /*升阶页面，升级的铜币不足不显示*/
        CCLabelTTF* lessCopForLev = (CCLabelTTF*)scrollView->getContainer()->getChildByTag(LEVUP_COPPER);
        lessCopForLev->setVisible(false);
        
        // 城池显示
        CCSprite* pFontCity = CCSprite::create("LU_zhancity.png");
        pFontCity->setAnchorPoint(ccp(0, 0));
        pFontCity->setPosition(ccp(60, 230));
        node->addChild(pFontCity);
    }
    this->getChildByTag(CONTAINER_TAG)->addChild(node, 10, CHOS_UPDATE);
    
    /*
     * 返回菜单tipMenu的更新
     */
    // 菜单按钮
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    if (this->getChildByTag(LL_MENU_TAG)) {
        this->removeChildByTag(LL_MENU_TAG);
    }
    if (this->getChildByTag(MID_MENU_TAG)) {
        this->removeChildByTag(MID_MENU_TAG);
    }
    if (this->getChildByTag(RR_MENU_TAG)) {
        this->removeChildByTag(RR_MENU_TAG);
    }
    CCNode* pLLContainer = CCNode::create();
    this->addChild(pLLContainer, 1, LL_MENU_TAG);
    CCNode* pMIDContainer = CCNode::create();
    this->addChild(pMIDContainer, 1, MID_MENU_TAG);
    CCNode* pRRContainer = CCNode::create();
    this->addChild(pRRContainer, 1, RR_MENU_TAG);
    if (0 == mode) {
        
        // 无返回按钮
        CCSprite* pLeftImage = CCSprite::create("CM_left.png");
        pLeftImage->setAnchorPoint(CCPointZero);
        pLeftImage->setPosition(ccp(0.0f, GameState::getInstance()->getBottomOffset() + 750.0f));
        pLLContainer->addChild(pLeftImage, 2);

        // 初始化升级文字
        CCSprite* pMiddleImage = CCSprite::create("CM_mid.png");
        pMiddleImage->setAnchorPoint(CCPointZero);
        pMiddleImage->setPosition(ccp(origin.x + visibleSize.width / 2 - pMiddleImage->getContentSize().width / 2, GameState::getInstance()->getBottomOffset() + 750.0f));
        pMIDContainer->addChild(pMiddleImage, 2);
        //文字
        CCSprite* pMyhImage = CCSprite::create("LU_max_lev.png");
        pMyhImage->setAnchorPoint(CCPointZero);
        pMyhImage->setPosition(ccp(5 + origin.x + visibleSize.width / 2 - pMyhImage->getContentSize().width / 2, GameState::getInstance()->getBottomOffset() + 733.0f));
        pMIDContainer->addChild(pMyhImage, 3);
        // 文字
        CCSprite* pRightImage = CCSprite::create("LU_min_evo.png");
        pRightImage->setAnchorPoint(CCPointZero);
        pRightImage->setPosition(ccp(535.0f, GameState::getInstance()->getBottomOffset() + 755.0f));
        pRRContainer->addChild(pRightImage, 3);
        //按钮
        CCMenuItemImage *pSellItem = CCMenuItemImage::create("CM_right.png",
                                                             "CM_right.png",
                                                             this,
                                                             menu_selector(LevUpLayer::
                                                                           chosForUp));
        pSellItem->setTag(LEV_TAG);
        CCMenu* pSellMenu = CCMenu::create(pSellItem, NULL);
        pSellItem->setAnchorPoint(ccp(0, 0));
        pSellMenu->setAnchorPoint(CCPointZero);
        pSellMenu->setPosition(ccp(485.0f,
                                   GameState::getInstance()->getBottomOffset() + 750.0f));
        pRRContainer->addChild(pSellMenu, 2);
    } else if (1 == mode) {
        // 有返回按钮
        CCMenuItemImage *pReturnItem = CCMenuItemImage::create("CM_left.png",
                                                               "CM_left.png",
                                                             this,
                                                             menu_selector(LevUpLayer::
                                                                           chosForUp));
        pReturnItem->setTag(RETURN_TAG);
        CCMenu* pMenu = CCMenu::create(pReturnItem, NULL);
        pMenu->setAnchorPoint(ccp(0, 0));
        pMenu->setPosition(ccp(0, 0));
        pReturnItem->setAnchorPoint(CCPointZero);
        pReturnItem->setPosition(ccp(0, GameState::getInstance()->getBottomOffset() + 750.0f));
        pLLContainer->addChild(pMenu, 2);
        // 返回文字
        CCSprite* returnBtn = CCSprite::create("LU_return.png");
        returnBtn->setAnchorPoint(ccp(0, 0));
        returnBtn->setPosition(ccp(20, GameState::getInstance()->getBottomOffset() + 755.0f));
        pLLContainer->addChild(returnBtn, 2);
        
        // 初始化升级文字
        CCSprite* pMiddleImage = CCSprite::create("CM_mid.png");
        pMiddleImage->setAnchorPoint(CCPointZero);
        pMiddleImage->setPosition(ccp(origin.x + visibleSize.width / 2 - pMiddleImage->getContentSize().width / 2, GameState::getInstance()->getBottomOffset() + 750.0f));
        pMIDContainer->addChild(pMiddleImage, 2);
        //文字
        CCSprite* pMyhImage = CCSprite::create("LU_max_evo.png");
        pMyhImage->setAnchorPoint(CCPointZero);
        pMyhImage->setPosition(ccp(origin.x + visibleSize.width / 2 - pMyhImage->getContentSize().width / 2, GameState::getInstance()->getBottomOffset() + 735.0f));
        pMIDContainer->addChild(pMyhImage, 3);
        
        // 文字
        CCSprite* pRightImage = CCSprite::create("LU_min_lev.png");
        pRightImage->setAnchorPoint(CCPointZero);
        pRightImage->setPosition(ccp(535.0f, GameState::getInstance()->getBottomOffset() + 755.0f));
        pRRContainer->addChild(pRightImage, 3);
        //按钮
        CCMenuItemImage *pSellItem = CCMenuItemImage::create("CM_right.png",
                                                             "CM_right.png",
                                                             this,
                                                             menu_selector(LevUpLayer::
                                                                           chosForUp));
        pSellItem->setTag(EVO_TAG);
        CCMenu* pSellMenu = CCMenu::create(pSellItem, NULL);
        pSellItem->setAnchorPoint(ccp(0, 0));
        pSellMenu->setAnchorPoint(CCPointZero);
        pSellMenu->setPosition(ccp(485.0f,
                                   GameState::getInstance()->getBottomOffset() + 750.0f));
        pRRContainer->addChild(pSellMenu, 2);
    }

    // 武将升级信息
    if (scrollView->getContainer()->getChildByTag(INFO_TAG)) {
        scrollView->getContainer()->removeChildByTag(INFO_TAG);
    }
    CCNode* info = CCNode::create();
    info->setAnchorPoint(ccp(0, 0));
    info->setPosition(ccp(0, 0));
    scrollView->getContainer()->addChild(info,10,INFO_TAG);
    if (0 == mode) {
        CCSprite* bg = CCSprite::create("LU_font_bg.png");
        bg->setAnchorPoint(ccp(0, 0));
        bg->setPosition(ccp(100, 20));
        info->addChild(bg);
        // 吞噬获得经验
        CCSprite* getExp = CCSprite::create("LU_islevget.png");
        getExp->setAnchorPoint(ccp(0, 0));
        getExp->setPosition(ccp(0, 20));
        bg->addChild(getExp);
        CCLabelTTF* getExpLabel = CCLabelTTF::create("", "Arial", 19);
        getExpLabel->setAnchorPoint(ccp(0, 0));
        getExpLabel->setPosition(ccp(240, 42));
//        getExpLabel->setColor(ccc3(255, 215, 0));
        info->addChild(getExpLabel, 1, TAG_TUN);
        // 吞噬所需铜钱
        CCSprite* needGold = CCSprite::create("LU_islevcop.png");
        needGold->setAnchorPoint(ccp(0, 0));
        needGold->setPosition(ccp(230, 20));
        bg->addChild(needGold);
        CCLabelTTF* needGoldLabel = CCLabelTTF::create("", "Arial", 19);
        needGoldLabel->setAnchorPoint(ccp(0, 0));
        needGoldLabel->setPosition(ccp(470, 42));
//        needGoldLabel->setColor(ccc3(255, 215, 0));
        info->addChild(needGoldLabel, 1, TAG_GOLD);
        // 满级所需经验
        CCSprite* maxExp = CCSprite::create("LU_maxexp.png");
        maxExp->setAnchorPoint(ccp(0, 0));
        maxExp->setPosition(ccp(0, -5));
        bg->addChild(maxExp);
        CCLabelTTF* needMaxExp = CCLabelTTF::create("", "Arial", 19);
        needMaxExp->setAnchorPoint(ccp(0, 0));
        needMaxExp->setPosition(ccp(240, 20));
//        needMaxExp->setColor(ccc3(255, 215, 0));
        info->addChild(needMaxExp, 1, TAG_MAX);
        // 城池省下铜钱
        CCSprite* saveGold = CCSprite::create("LU_save.png");
        saveGold->setAnchorPoint(ccp(0, 0));
        saveGold->setPosition(ccp(230, -5));
        bg->addChild(saveGold);
        CCLabelTTF* saveLabel = CCLabelTTF::create("", "Arial", 19);
        saveLabel->setAnchorPoint(ccp(0, 0));
        saveLabel->setPosition(ccp(470, 22));
//        saveLabel->setColor(ccc3(255, 215, 0));
        info->addChild(saveLabel, TAG_SAVE);

    } else if (1 == mode) {
        CCSprite* bg = CCSprite::create("LU_font_bg.png");
        bg->setAnchorPoint(ccp(0, 0));
        bg->setPosition(ccp(490, 20));
        info->addChild(bg);
        // 升阶所需铜钱
        CCSprite* needGold = CCSprite::create("LU_evogold.png");
        needGold->setAnchorPoint(ccp(0, 0));
        needGold->setPosition(ccp(0, 10));
        bg->addChild(needGold);
        CCLabelTTF* needGoldLabel = CCLabelTTF::create("", "Arial", 19);
        needGoldLabel->setAnchorPoint(ccp(0, 0));
        needGoldLabel->setPosition(ccp(635, 35));
//        needGoldLabel->setColor(ccc3(255, 215, 0));
        info->addChild(needGoldLabel,1, TAG_EVO);
        // 城池省下铜钱
        CCSprite* saveGold = CCSprite::create("LU_save.png");
        saveGold->setAnchorPoint(ccp(0, 0));
        saveGold->setPosition(ccp(230, 10));
        bg->addChild(saveGold);
        CCLabelTTF* saveLabel = CCLabelTTF::create("", "Arial", 19);
        saveLabel->setAnchorPoint(ccp(0, 0));
        saveLabel->setPosition(ccp(865, 35));
//        saveLabel->setColor(ccc3(255, 215, 0));
        info->addChild(saveLabel,1, TAG_EVO_SAVE);
    }
    
    
    // 更新武将显示
    if (0 == mode || 1 == mode) {
        _updateHeroUiComponent();
    }
    
}

/*
 * 武将升级
 */
void LevUpLayer::onHeroLevUp() {
    CCLOG("武将升级");
    
    do {
        // 升级武将是否已经选好
        CC_BREAK_IF(NULL == HeroManager::getInstance()->getHeroDataByUniId(m_heroForLevUp));
        
        // 被吞噬武将是否已经选好
        CC_BREAK_IF(0 == m_foodHeroForLevupArray->count());
        
        // http tag
        char szHttpTag[16];
        memset(szHttpTag, '\0', sizeof(szHttpTag));
        strcpy(szHttpTag, HTTP_REQUEST_LEVUPHERO_TAG);
        
        // 构造 post 字符串
        char szPostBuffer[MAX_POSTTO_SERVER_BUFFER_SIZE];
        memset(szPostBuffer, '\0', MAX_POSTTO_SERVER_BUFFER_SIZE);
        
        //
        strcpy(szPostBuffer, "c=hero&a=upgrade&target=");
        strcat(szPostBuffer, m_heroForLevUp);
        strcat(szPostBuffer, "&foods=");
        
        unsigned int countOfSelHeros = 0;
        CCObject* pObject = NULL;
        CCARRAY_FOREACH(m_foodHeroForLevupArray, pObject) {
            HeroOfPlayerItem* pHeroOfPlayerItem = (HeroOfPlayerItem*)pObject;
            if (countOfSelHeros > 0)
                strcat(szPostBuffer, "|");
            strcat(szPostBuffer, pHeroOfPlayerItem->getUniId().c_str());
            countOfSelHeros++;
        } /*for*/
        
        strcat(szPostBuffer, "&uid=");
        strcat(szPostBuffer, GamePlayer::getInstance()->getUid().c_str());
        CCLog("post buffer = %s", szPostBuffer);
        
        // 提交 post
        NetConnection::getInstance()->commitPostRequestData(szPostBuffer, szHttpTag);
        CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_LOADING_MAINSCENE);
    } while (0);
}

/*
 * 武将升阶
 */
void LevUpLayer::onHeroEvoUp() {
    CCLOG("武将升阶");
    
    CCLog("m_heroForEvoUp = %s", m_heroForEvoUp);
    
    do {
        
        HeroOfPlayerItem* pHeroOfPlayerItem = HeroManager::getInstance()->getHeroDataByUniId(m_heroForEvoUp);
        // 升级武将是否已经选好
        CC_BREAK_IF(NULL == pHeroOfPlayerItem);
        if (pHeroOfPlayerItem->getLevel() < pHeroOfPlayerItem->getHeroItem()->getUl()) {
            //
            CCLog("不够升阶的等级 可升阶等级需要 %d 武将当前等级为 %d",
                  pHeroOfPlayerItem->getHeroItem()->getUl(), pHeroOfPlayerItem->getLevel());
            break;
        }
        
        // http tag
        char szHttpTag[16];
        memset(szHttpTag, '\0', sizeof(szHttpTag));
        strcpy(szHttpTag, HTTP_REQUEST_EVOUPHERO_TAG);
        
        // 构造 post 字符串
        char szPostBuffer[MAX_POSTTO_SERVER_BUFFER_SIZE];
        memset(szPostBuffer, '\0', MAX_POSTTO_SERVER_BUFFER_SIZE);
        
        //
        strcpy(szPostBuffer, "c=hero&a=quality&target=");
        strcat(szPostBuffer, m_heroForEvoUp);
        strcat(szPostBuffer, "&uid=");
        strcat(szPostBuffer, GamePlayer::getInstance()->getUid().c_str());
        CCLog("post buffer = %s", szPostBuffer);
        
        // 提交 post
        NetConnection::getInstance()->commitPostRequestData(szPostBuffer, szHttpTag);
        CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_LOADING_MAINSCENE);
    } while (0);
}


/*
 *
 */
void LevUpLayer::chosForUp(CCObject* pSeneder) {
    CCMenuItemImage* p = (CCMenuItemImage*)pSeneder;
    switch (p->getTag()) {
        case LEV_TAG:
        {
            _adjustScrollView(NEXT);
            tipMenu();
        }
            break;
        case EVO_TAG:
        {
            _adjustScrollView(PREVIOUS);
            tipMenu();
        }
            break;
        case RETURN_TAG:
        {
            _adjustScrollView(PREVIOUS);
            tipMenu();
        }
            break;
        default:
            break;
    }
}

/*
 *  选择需要升级或升阶的武将
 */
void LevUpLayer::onSelLevEvoUpHero() {
    // 状态
    stateForBtn = true;
    if (!m_isMoving) {
        if (1 == curIndex) {
            m_pGameState->setCurrTagWhoCallHeroSel(WCH_LEVUP);
            if (HeroManager::getInstance()->getHeroDataByUniId(m_heroForLevUp)) {
                m_pGameState->setLevupHeroUUidForHeroList(m_heroForLevUp);
                char foodHersForLevUp[MAX_FOOD_HEROS_FOR_LEVUP][HERO_UNIID_BUFFER_SIZE];
                const unsigned int countOfFoodHeros = getFoodHerosForLevup(foodHersForLevUp);
                m_pGameState->setFoodHersForLevUp(countOfFoodHeros, foodHersForLevUp);
            } else {
                m_pGameState->setLevupHeroUUidForHeroList("");
            }
            CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_HEROS_STRING);
        } else if (2 == curIndex) {
            /*武将升阶清除，之前升级获得武将数据*/
            memset(m_heroEvoUp, '\0', sizeof(m_heroEvoUp));
            
            m_pGameState->setCurrTagWhoCallHeroSel(WCH_EVOUP);
            if (HeroManager::getInstance()->getHeroDataByUniId(m_heroForEvoUp)) {
                m_pGameState->setEvoupHeroUUidForHeroList(m_heroForEvoUp);
            } else {
                m_pGameState->setEvoupHeroUUidForHeroList("");
            }
            CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_HEROS_STRING);
        }
    }
}

/*
 *  选择用于升级的被吞噬武将
 */
void LevUpLayer::onSelFoodForLevupHero() {
    if (!m_isMoving) {
        char foodHersForLevUp[MAX_FOOD_HEROS_FOR_LEVUP][HERO_UNIID_BUFFER_SIZE];
        const unsigned int countOfFoodHeros = getFoodHerosForLevup(foodHersForLevUp);
        m_pGameState->setFoodHersForLevUp(countOfFoodHeros, foodHersForLevUp);
        if (HeroManager::getInstance()->getHeroDataByUniId(m_heroForLevUp)) {
            m_pGameState->setLevupHeroUUidForHeroList(m_heroForLevUp);
        }
        m_pGameState->setCurrTagWhoCallHeroSel(WCH_FORFOOD);
        CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_HEROS_STRING);
    }
}

void LevUpLayer::scrollViewDidScroll(CCScrollView* view) {
//    m_isMoving = true;
}

void LevUpLayer::scrollViewDidZoom(CCScrollView* view) {
    
}

/*
 *  进入当前层
 */
void LevUpLayer::onEnter() {
    CCLayer::onEnter();
    
    // 这里的第三个参数一定要设置成false，
    // true 即HelloWorld层吞噬掉触摸事件
    // false 即HelloWorld层与CCScrollView对象先后处理触摸事件
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, false);
    
    // 初始化用于存放用于升级武将的被吞噬武将
    m_foodHeroForLevupArray = CCArray::createWithCapacity(MAX_FOOD_HEROS_FOR_LEVUP);
    m_foodHeroForLevupArray->retain();
}

/*
 * 离开当前层
 */
void LevUpLayer::onExit() {
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);

    m_foodHeroForLevupArray->removeAllObjects();
    m_foodHeroForLevupArray->release();
    m_foodHeroForLevupArray = NULL;
    
    CCLayer::onExit();
}

bool LevUpLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!m_enableTouch)
        return false;
    
    float x = pTouch->getLocation().x;
    float y = pTouch->getLocation().y;
    if (y < ((GameState::getInstance()->getBottomOffset()) + 300.0f) || y > ((GameState::getInstance()->getBottomOffset()) + 300.0f) + 452.0f ) {
        return false;
    }
    // 刚开始触摸就移走挡板
    CCSprite* board = (CCSprite*)scrollView->getContainer()->getChildByTag(BOARD);
    board->setPosition(ccp(1500, 65));
    if ( moving ) {
        return false;
    }
    active = true;
    scrollView->stopAllActions();
    lastPos = ccp(x, y);
    tempPos = x;
    
    return true;
}

void LevUpLayer::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_isMoving = true;
    float xOffset = 0.0f;
    float x = pTouch->getLocation().x;
    if (active) {
        xOffset = x - tempPos;
        if (curIndex < 2) {
            if (xOffset < 0 && xOffset > -415.0f){
                
                scrollView->setPosition(ccp(0+xOffset, scrollView->getPositionY()));
            }
        } else if (curIndex > 1) {
            if (xOffset > 0 && xOffset < 415.0f) {
                scrollView->setPosition(ccp(-410+xOffset, scrollView->getPositionY()));
            }
        }
    }
}

void LevUpLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    // 触摸结束后，判断是否换页并移动挡板
    CCSprite* board = (CCSprite*)scrollView->getContainer()->getChildByTag(BOARD);
    //float x = pTouch->getLocation().x;&& (x - lastPos.x) != 0/*判断是否是点击*/
    if (active) {
        if (curIndex == 1) {
            offset = 0;
        }
        moving = true;
        _scrollX(pTouch,pEvent);
        active = false;
        tempPos = 0;
        lastPos = ccp(0, 0);
    }
    if (curIndex == 1) {
        board->stopAllActions();
        board->setPosition(ccp(600 + 500, 65));
        board->runAction(CCMoveBy::create(OPENNING_ANI_DURING, CCPointMake(-500, 0)));
    } else if (curIndex == 2) {
        board->stopAllActions();
        board->setPosition(ccp(325 - 500, 65));
        board->runAction(CCMoveBy::create(OPENNING_ANI_DURING, CCPointMake(500, 0)));
    }
}

void LevUpLayer::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (active) {
        active = false;
        moving = true;
        _scrollX(pTouch, pEvent);
    }
}

void LevUpLayer::_scrollX(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    float x = pTouch->getLocation().x;
    // && (lastPos != ccp(0, 0))
    if (abs(x - lastPos.x)>40) {
        if (x > lastPos.x) {
            if (curIndex > 1) {
                _adjustScrollView(PREVIOUS);
            } else {
                _adjustScrollView(NOARG);
            }
        } else {
            if (curIndex < 2) {
                _adjustScrollView(NEXT);
            }else {
                _adjustScrollView(NOARG);
            }
        }
    }else {
        _adjustScrollView(NOARG);
    }
    
}

void LevUpLayer::_adjustScrollView(int dir)
{
    CCSequence* autoMove;
    float time = 0.08f;
    if (dir != NOARG) {
        if (dir == NEXT) {
            if (curIndex < 2) {
                tipMenu();
                curIndex++;
                offset = -410.0f;
            }
        } else if (dir == PREVIOUS) {
            if (curIndex > 1) {
                tipMenu();
                curIndex--;
                offset = 0;
            }
        }
    }
    
    autoMove = CCSequence::create(CCMoveTo::create(time, ccp(offset, scrollView->getPositionY())), CCCallFuncND::create(this, callfuncND_selector(LevUpLayer::_callback), (void*)dir), NULL);
    scrollView->runAction(autoMove);
}

//kknd
//
void LevUpLayer::_callback(CCNode* pNode, void* pDir) {
    // 按钮移动翻页，移动挡板位置
    CCSprite* board = (CCSprite*)scrollView->getContainer()->getChildByTag(BOARD);
    if (curIndex == 1) {
        board->stopAllActions();
        board->setPosition(ccp(600 + 500, 65));
        board->runAction(CCMoveBy::create(OPENNING_ANI_DURING, CCPointMake(-500, 0)));
    } else if (curIndex == 2) {
        board->stopAllActions();
        board->setPosition(ccp(325 - 500, 65));
        board->runAction(CCMoveBy::create(OPENNING_ANI_DURING, CCPointMake(500, 0)));
    }
    
    moving = false;
    m_isMoving = false;
    
    int dir = reinterpret_cast<int>(pDir);
    if (offset == -410.0f) {
        if (dir != NOARG) {
            // 2
            memset(m_heroForEvoUp, '\0', sizeof(m_heroForEvoUp));
            memset(m_heroEvoUp, '\0', sizeof(m_heroEvoUp));
            if (HeroManager::getInstance()->getHeroDataByUniId(m_heroForLevUp)) {
                strcpy(m_heroForEvoUp, m_heroForLevUp);
            }
            memset(m_heroForLevUp, '\0', sizeof(m_heroForLevUp));
            update(NEXT);
        }
    } else {
        if (dir != NOARG) {
            // 1
            memset(m_heroForLevUp, '\0', sizeof(m_heroForLevUp));
            if (HeroManager::getInstance()->getHeroDataByUniId(m_heroForEvoUp)) {
                strcpy(m_heroForLevUp, m_heroForEvoUp);
            }
            if (HeroManager::getInstance()->getHeroDataByUniId(m_heroEvoUp)) {
                strcpy(m_heroForLevUp, m_heroEvoUp);
            }
            memset(m_heroForEvoUp, '\0', sizeof(m_heroForEvoUp));
            memset(m_heroEvoUp, '\0', sizeof(m_heroEvoUp));
            update(PREVIOUS);
        }
    }
}

void LevUpLayer::restoreTouchUICompent() {
    this->setVisible(true);
    m_enableTouch = true;
    
    float oldY = 0.0f;
    float oldX = 0.0f;
    //左tipMenu
    CCNode* pLNode = (CCNode*)this->getChildByTag(LL_MENU_TAG);
    oldY = pLNode->getPosition().y;
    pLNode->setPosition(ccp(-160.0f, oldY));
    pLNode->runAction(CCMoveTo::create(OPENNING_ANI_DURING, CCPointMake(0.0f, oldY)));
    //中间
    CCNode* pMNode = (CCNode*)this->getChildByTag(MID_MENU_TAG);
    oldX = pMNode->getPosition().x;
    pMNode->setPosition(ccp(oldX, 150.0f));
    pMNode->runAction(CCMoveTo::create(OPENNING_ANI_DURING, CCPointMake(oldX, 0)));
    //右边
    CCNode* pRNode = (CCNode*)this->getChildByTag(RR_MENU_TAG);
    oldY = pRNode->getPosition().y;
    pRNode->setPosition(ccp(160.0f, oldY));
    pRNode->runAction(CCMoveTo::create(OPENNING_ANI_DURING, CCPointMake(0.0f, oldY)));
    //层的移动
    CCNode* pLayer = (CCNode*)this->getChildByTag(CONTAINER_TAG);
    pLayer->runAction(CCMoveTo::create(OPENNING_ANI_DURING, CCPointMake(0, 0)));
}

void LevUpLayer::initPosition() {
    CCNode* pNode = (CCNode*)this->getChildByTag(CONTAINER_TAG);
    pNode->setPosition(0, -LAYER_OFFSET);
}

void LevUpLayer::moveOutTouchUICompent() {
    float oldY = 0.0f;
    float oldX = 0.0f;
    //左tipMenu
    CCNode* pLNode = (CCNode*)this->getChildByTag(LL_MENU_TAG);
    oldY = pLNode->getPosition().y;
//    pLNode->setPosition(ccp(-160.0f, oldY));
    pLNode->runAction(CCMoveTo::create(OPENNING_ANI_DURING, CCPointMake(-160.0f, oldY)));
    //中间
    CCNode* pMNode = (CCNode*)this->getChildByTag(MID_MENU_TAG);
    oldX = pMNode->getPosition().x;
//    pMNode->setPosition(ccp(oldX, -150.0f));
    pMNode->runAction(CCMoveTo::create(OPENNING_ANI_DURING, CCPointMake(oldX, 150.0f)));
    //右边
    CCNode* pRNode = (CCNode*)this->getChildByTag(RR_MENU_TAG);
    oldY = pRNode->getPosition().y;
//    pRNode->setPosition(ccp(160.0f, oldY));
    pRNode->runAction(CCMoveTo::create(OPENNING_ANI_DURING, CCPointMake(160.0f, oldY)));
    
    CCNode* pNode = (CCNode*)this->getChildByTag(CONTAINER_TAG);
    pNode->runAction(CCSequence::create(CCMoveTo::create(OPENNING_ANI_DURING, ccp(0, -LAYER_OFFSET)), CCCallFunc::create(this, callfunc_selector(LevUpLayer::moveOutAndcallBack)), NULL));
    
    m_enableTouch = false;
}

void LevUpLayer::moveOutAndcallBack() {
//    this->setVisible(false);
    if (m_isRemoveSelf) {
        this->removeFromParent();
        CCLog("LevUpLayer 从主场景移除");
    }
}


/*
 * 移动 tipMenu
 */
void LevUpLayer::tipMenu() {
    float oldY = 0.0f;
    float oldX = 0.0f;
    //左tipMenu
    CCNode* pLNode = (CCNode*)this->getChildByTag(LL_MENU_TAG);
    oldY = pLNode->getPosition().y;
    pLNode->setPosition(ccp(-160.0f, oldY));
    pLNode->runAction(CCMoveTo::create(OPENNING_ANI_DURING, CCPointMake(0.0f, oldY)));
    //中间
    CCNode* pMNode = (CCNode*)this->getChildByTag(MID_MENU_TAG);
    oldX = pMNode->getPosition().x;
    pMNode->setPosition(ccp(oldX, 150.0f));
    pMNode->runAction(CCMoveTo::create(OPENNING_ANI_DURING, CCPointMake(oldX, 0)));
    //右边
    CCNode* pRNode = (CCNode*)this->getChildByTag(RR_MENU_TAG);
    oldY = pRNode->getPosition().y;
    pRNode->setPosition(ccp(160.0f, oldY));
    pRNode->runAction(CCMoveTo::create(OPENNING_ANI_DURING, CCPointMake(0.0f, oldY)));
}

/*
 *  功能：
 *      获取升级武将 uniid
 *  参数：
 *      foodHersForLevUp    保存升级武将 uniid
 *  返回：
 *      获取成功返回指向 uniid 的字符串缓冲区的指针
 */
char* LevUpLayer::getHeroForLevUp(char hersForLevUp[]) {
    if (NULL == hersForLevUp)
        return NULL;
    
    if (HeroManager::getInstance()->getHeroDataByUniId(m_heroForLevUp)) {
        strcpy(hersForLevUp, m_heroForLevUp);
        return hersForLevUp;
    }
    return NULL;
}

/*
 *  功能：
 *      设置升级武将 uniid
 *  参数：
 *      levUpheroUnid    升级武将 uniid
 */
void LevUpLayer::setHeroForLevUp(char levUpheroUnid[]) {
    memset(m_heroForLevUp, '\0', sizeof(m_heroForLevUp));
    if (levUpheroUnid)
        strcpy(m_heroForLevUp, levUpheroUnid);
};

/*
 *  功能：
 *      获取升阶武将 uniid
 *  参数：
 *      foodHersForLevUp    保存升阶武将 uniid
 *  返回：
 *      获取成功返回指向 uniid 的字符串缓冲区的指针
 */
char* LevUpLayer::getHeroForEvoUp(char hersForEvoUp[]) {
    if (NULL == hersForEvoUp)
        return NULL;
    
    if (HeroManager::getInstance()->getHeroDataByUniId(m_heroForEvoUp)) {
        strcpy(hersForEvoUp, m_heroForEvoUp);
        return hersForEvoUp;
    }
    return NULL;
}

/*
 *  功能：
 *      设置升阶武将 uniid
 *  参数：
 *      evoUpheroUnid    升阶武将 uniid
 */
void LevUpLayer::setHeroForEvoUp(char evoUpheroUnid[]) {
    memset(m_heroForEvoUp, '\0', sizeof(m_heroForEvoUp));
    if (evoUpheroUnid)
        strcpy(m_heroForEvoUp, evoUpheroUnid);
};

/*
 *  功能：
 *      设置升阶武将 uniid
 *  参数：
 *      evoUpheroUnid    升阶武将 uniid
 */
/* 设置升阶后武将 */
void LevUpLayer::setHeroEvoUp(char evoUpHeroUnid[]) {
    memset(m_heroForEvoUp, '\0', sizeof(m_heroForEvoUp));
    memset(m_heroEvoUp, '\0', sizeof(m_heroEvoUp));
    
    CCAssert(evoUpHeroUnid, "invalid data");
    
    CCLog("evoUpHeroUnid = %s", evoUpHeroUnid);
    
//    if (evoUpHeroUnid) {
        strcpy(m_heroEvoUp, evoUpHeroUnid);
   // }
  
}


/*
 *  功能：
 *      获取已选升级被吞噬武将
 *  参数：
 *      foodHersForLevUp    保存已选升级被吞噬武将 uniid
 *  返回：
 *      已选的升级被吞噬武将数量
 */
unsigned int LevUpLayer::getFoodHerosForLevup(char foodHersForLevUp[][HERO_UNIID_BUFFER_SIZE]) {
    unsigned int i = 0;
    CCObject *pObject = NULL;
    CCARRAY_FOREACH(m_foodHeroForLevupArray, pObject) {
        HeroOfPlayerItem* pItem = (HeroOfPlayerItem*)pObject;
        strcpy(foodHersForLevUp[i++], pItem->getUniId().c_str());
    }
    return m_foodHeroForLevupArray->count();
}

/*
 *  功能：
 *      设置已选择的升级被吞噬武将
 *  参数：
 *      countOfFoodHeros        升级被吞噬武将数量
 *      foodHersForLevUp        升级被吞噬武将 uniid 字符串数组
 */
void LevUpLayer::setFoodHerosForLevup(const unsigned int countOfFoodHeros, const char foodHersForLevUp[][HERO_UNIID_BUFFER_SIZE]) {
    do {
//        CC_BREAK_IF(0 == countOfFoodHeros || NULL == foodHersForLevUp);
        
        m_foodHeroForLevupArray->removeAllObjects();
        for (int i = 0; i < countOfFoodHeros; i++) {
            HeroOfPlayerItem* pItem = HeroManager::getInstance()->getHeroDataByUniId(foodHersForLevUp[i]);
            if (pItem)
                m_foodHeroForLevupArray->addObject(pItem);
        } /*for*/
        
    } while (0);
}

/*
 *  功能：
 *      清除所有升级、升阶、被吞噬武将数据
 */
void LevUpLayer::clearHeroData() {
//    memset(m_heroForLevUp, '\0', sizeof(m_heroForLevUp));
//    memset(m_heroForEvoUp, '\0', sizeof(m_heroForEvoUp));
    if (m_foodHeroForLevupArray)
        m_foodHeroForLevupArray->removeAllObjects();
}

void LevUpLayer::actionForLevUp() {
    CardBox* comCard = (CardBox*)scrollView->getContainer()->getChildByTag(CHOS_LEVUP)->getChildByTag(COM_CARD);
    if (comCard)
        comCard->actionForLevUp();
    
    CardBox* evohCard = (CardBox*)scrollView->getContainer()->getChildByTag(EVOH_CARD);
    if (evohCard)
        evohCard->actionForLevUp();
}