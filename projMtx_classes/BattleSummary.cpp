#include "BattleSummary.h"
#include "MainScene.h"
#include "customMessage.h"
#include "GameState.h"
#include "DropItemManager.h"
#include "DropItem.h"
#include "GamePlayer.h"
#include "HeroManager.h"
#include "HeroItem.h"
#include "CardBox.h"
#include "CopyManager.h"
#include "CopyDataItem.h"
#include "LevelDataItem.h"
#include "Battle.h"

// 上下款大图
#define MAIN_MENU_IMAGE_FILE        "mainMenuIcon.pvr.ccz"
#define MAIN_MENU_LIST_FILE         "mainMenuIcon.plist"

#define IMAGE_TOP                   "top.png"
#define IMAGE_BOTTOM                "bottom.png"

#define CONTAINER           100                     // 层容器
#define MENUOFBG            101                     // 背景层按钮tag
#define NODE_TAG            102                     // 更新界面容器

#define HEI_COLOR           ccc3(0, 0, 0)

#define PADDING_H           38.0f
#define PADDING_V           350.0f

#define ANI_TIME            0.5f
#define ANI_TIME_DETA       1.0f

#define EXP_ANI_TAG         201         
#define COP_ANI_TAG         202
#define LEV_ANI_TAG         203
#define PRO_ANI_TAG         204
#define CARD_TAG            205

#define SPOIL_FONT_TAG      250
#define PRISON_CARD_TAG     251


/*
 *  构造函数
 */
BattleSummary::BattleSummary()
:m_isSummary(true)
,m_cop(0)
,m_lev(GamePlayer::getInstance()->getLevel())
,m_heroIdx(0)
,m_pro(0.0f)
,m_curLev(0)
,m_showExp(0)
,m_curExp(0)
,m_oldExp(GamePlayer::getInstance()->getExp())
{
}

/*
 *  析构函数
 */
BattleSummary::~BattleSummary() {
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(MAIN_MENU_LIST_FILE);
    CCTextureCache::sharedTextureCache()->removeTextureForKey(MAIN_MENU_IMAGE_FILE);
}

/*
 *  初始化
 */
bool BattleSummary::init() {
    // super init
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
                                                      menu_selector(BattleSummary::updateUi));
    pNext->setAnchorPoint(ccp(0, 0));
    pNext->setPosition(ccp(0, -25));
    CCMenu* menu = CCMenu::create(pNext, NULL);
    menu->setEnabled(false);
    menu->setAnchorPoint(ccp(0, 0));
    menu->setPosition(ccp(0, 0));
    pContainer->addChild(menu, 1, MENUOFBG);
    
    _updateUiData();

    return true;
}

/* 等级 */
void BattleSummary::_updateUiData() {
    int level = 0;
    int getExp = GameState::getInstance()->getCopyForExp();
    int localExp = GamePlayer::getInstance()->getExpByLev(m_lev);
    int curExp = GamePlayer::getInstance()->getExp();
    m_pro = ((float)curExp / (float)localExp) * 100.0;
    while (getExp - (localExp - curExp) > 0) {
        getExp -= (localExp - curExp);
        curExp = 0;
        level++;
        localExp = GamePlayer::getInstance()->getExpByLev(m_lev + level);
        m_curExp = getExp;
    }
    m_oldLev = GamePlayer::getInstance()->getLevel();
    m_curLev = m_oldLev + level;
    
    /* 存储获得等级,已经当前等级的经验 */
    GamePlayer::getInstance()->setLevel(m_curLev);
    if (level == 0)/*没有升级*/ {
        m_curExp = curExp + getExp;
    }
    GamePlayer::getInstance()->setExp(m_curExp);
}

void BattleSummary::updateUi() {
    CCNode* pParent = (CCNode*)this->getChildByTag(CONTAINER);
    if (pParent->getChildByTag(NODE_TAG)) {
        pParent->removeChildByTag(NODE_TAG);
    }
    CCNode* pNode = CCNode::create();
    pParent->addChild(pNode, 2, NODE_TAG);
    
    CCSprite* spfontBg = CCSprite::create("SM_fontbg.png");
    spfontBg->setAnchorPoint(ccp(0.5, 0));
    spfontBg->setPosition(ccp(320, 720.0f));
    pNode->addChild(spfontBg, 1);
    if (m_isSummary)/*物品结算*/ {
        char buffer[16];
        
        CCSprite* spfont = CCSprite::create("SM_battle.png");
        spfont->setAnchorPoint(ccp(0, 0));
        spfont->setPosition(ccp((spfontBg->getContentSize().width - spfont->getContentSize().width) / 2, (
                                spfontBg->getContentSize().height - spfont->getContentSize().height) / 2));
        spfontBg->addChild(spfont);
        
        /* 副本名 */
        sprintf(buffer, "FB_mapFont%d.png",GameState::getInstance()->getCopyId());
        CCLog("---->>> %s", buffer);
        CCSprite* pName = CCSprite::create(buffer);
        pName->setScale(0.8);
//        CopyDataItem* pCopyDataItem = CopyManager::getInstance()->getCopyData(GameState::getInstance()->getCopyId());
//        CCLabelTTF* pName = CCLabelTTF::create(pCopyDataItem->getName().c_str(), "Arial", 23);
        pName->setAnchorPoint(ccp(0.5, 0.5));
        pName->setPosition(ccp(320.0f, 710.0f));
        pNode->addChild(pName, 10);
        /* 关卡名 */
        sprintf(buffer, "FB_hj%d.png", GameState::getInstance()->getLevId());
        CCSprite* nameLabel = CCSprite::create(buffer);
        nameLabel->setScale(0.7);
//        LevelDataItem* pLevelDataItem = CopyManager::getInstance()->getLevelDataById(GameState::getInstance()->getCopyId(), GameState::getInstance()->getLevId());
//        CCLabelTTF* nameLabel = CCLabelTTF::create(pLevelDataItem->getName().c_str(), "Arial", 23);
        nameLabel->setAnchorPoint(ccp(0.5, 1));
        nameLabel->setPosition(ccp(320.0f, 690.0f));
        pNode->addChild(nameLabel, 10);
        
//        CCSprite* spNameBox = CCSprite::create("SM_fbName.png");
//        spNameBox->setAnchorPoint(ccp(0.5, 0.5));
//        spNameBox->setPosition(ccp(320, 700));
//        pNode->addChild(spNameBox, 10);
        
        // 铜钱获得
        CCSprite* labelCop = CCSprite::create("SM_copper.png");
        labelCop->setAnchorPoint(ccp(0, 0));
        labelCop->setPosition(ccp(80, 600));
        pNode->addChild(labelCop);
        
//        unsigned int money = GameState::getInstance()->getCopyForMoney();
//        sprintf(buffer, "%d", money);
        CCLabelTTF* moneyValue = CCLabelTTF::create("0", "Arial", 27);
        moneyValue->setAnchorPoint(ccp(0, 0));
        moneyValue->setPosition(ccp(230, 597));
        moneyValue->setColor(HEI_COLOR);
        pNode->addChild(moneyValue, 1, COP_ANI_TAG);
        
        // 获得经验
        CCSprite* labelExp = CCSprite::create("SM_exp.png");
        labelExp->setAnchorPoint(ccp(0, 0));
        labelExp->setPosition(ccp(80, 560));
        pNode->addChild(labelExp);
        
//        unsigned int exp = GameState::getInstance()->getCopyForExp();
//        sprintf(buffer, "%d", exp);
        CCLabelTTF* expValue = CCLabelTTF::create("0", "Arial", 27);
        expValue->setAnchorPoint(ccp(0, 0));
        expValue->setPosition(ccp(230, 557));
        expValue->setColor(HEI_COLOR);
        pNode->addChild(expValue, 1, EXP_ANI_TAG);
        
        // 等级显示
        sprintf(buffer, "%d", m_oldLev);
        CCLabelTTF* levValue = CCLabelTTF::create(buffer, "Arial", 27);
        levValue->setAnchorPoint(ccp(0, 0));
        levValue->setPosition(ccp(125, 516));
        levValue->setColor(HEI_COLOR);
        pNode->addChild(levValue, 1, LEV_ANI_TAG);
        CCSprite* labelLev = CCSprite::create("SM_lev.png");
        labelLev->setAnchorPoint(ccp(0, 0));
        labelLev->setPosition(ccp(80, 520));
        pNode->addChild(labelLev);
        // 等级增加动画
        CCSprite* expBox = CCSprite::create("SM_levprobg.png");
        expBox->setAnchorPoint(ccp(0, 0));
        expBox->setPosition(ccp(175, 516));
        pNode->addChild(expBox);
        float localExp = (float)GamePlayer::getInstance()->getExpByLev(m_lev);
        float percent = (m_oldExp / localExp) * 100.0f;
        CCProgressTimer* expPro = CCProgressTimer::create(CCSprite::create("SM_levpro.png"));
        expPro->setType(kCCProgressTimerTypeBar);
        expPro->setMidpoint(ccp(0, 0));
        expPro->setBarChangeRate(ccp(1, 0));
        expPro->setPercentage(percent);
        expPro->setAnchorPoint(ccp(0, 0));
        expPro->setPosition(ccp(175, 516));
        pNode->addChild(expPro, 1, PRO_ANI_TAG);
        // 获得战利品
        CCSprite* spoils = CCSprite::create("SM_spoils.png");
        spoils->setAnchorPoint(ccp(0.5, 0));
        spoils->setPosition(ccp(320, 470));
        spoils->setOpacity(0);
        pNode->addChild(spoils, 2, SPOIL_FONT_TAG);
        
        DropItemManager* pDropItemManager = DropItemManager::getInstance();
        HeroManager* pHeroManager = HeroManager::getInstance();
        
        int dropsNum = pDropItemManager->getCountOfDropItems();
        if (dropsNum <= 15) {
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 5; ++j) {
                    DropItem* pDropItem = pDropItemManager->getDropItem(5 * i + j);
                    if (pDropItem) {
                        if (1 == pDropItem->getType())/*武将*/ {
                            int heroId = pDropItem->getId();
                            HeroItem* pHeroItem = pHeroManager->getHeroItem(heroId);
                            CardBox* card = CardBox::create();
                            card->setCard(pHeroItem);
                            card->setAnchorPoint(ccp(0, 0));
                            card->setOpa(0);
                            card->setPosition(ccp(PADDING_H + j * 115, PADDING_V - i * 115));
                            pNode->addChild(card, 2, CARD_TAG + 5 * i + j);
                        }
                    }
                }
            }
        } else {
            for (int i = 0; i < 3; ++i) {
                DropItem* pDropItem = pDropItemManager->getDropItem(15 + i);
                if (pDropItem) {
                    if (1 == pDropItem->getType())/*武将*/ {
                        int heroId = pDropItem->getId();
                        HeroItem* pHeroItem = pHeroManager->getHeroItem(heroId);
                        CardBox* card = CardBox::create();
                        card->setCard(pHeroItem);
                        card->setOpa(0);
                        card->setAnchorPoint(ccp(0, 0));
                        card->setPosition(ccp(PADDING_H + 51.0f + i * 115, PADDING_V - 51.0f));
                        pNode->addChild(card, 3, CARD_TAG + 15 + i);
                    }
                }
            }
        }
        
        CardBox* pPrisonCard = CardBox::create();
        int id = GameState::getInstance()->getGetPrisonHero();
        CCLog("监狱获得武将ID = %d", id);
        if (id != 0) {
            HeroItem* pHeroItem = HeroManager::getInstance()->getHeroItem(id);
            if (pHeroItem) {
                pPrisonCard->cardForTOngQue(pHeroItem);
                pPrisonCard->tongQueOpa(0);
                pPrisonCard->setAnchorPoint(ccp(0, 0));
                pPrisonCard->setPosition(ccp(480.0f, 560.0f));
                pNode->addChild(pPrisonCard, 3, PRISON_CARD_TAG);
            }
        }
        
        _summaryAni();
    } else/*友情结算*/ {
        /*将友情结算界面设置为不可触摸*/
        CCMenu* pMenu = (CCMenu*)this->getChildByTag(CONTAINER)->getChildByTag(MENUOFBG);
        pMenu->setEnabled(false);
        
        CCSprite* spfont = CCSprite::create("SM_frid.png");
        spfont->setAnchorPoint(ccp(0, 0));
        spfont->setPosition(ccp((spfontBg->getContentSize().width - spfont->getContentSize().width) / 2, (spfontBg->getContentSize().height - spfont->getContentSize().height) / 2));
        spfontBg->addChild(spfont);
        // 好友姓名框
        CCSprite* nameBox = CCSprite::create("SM_fridName.png");
        nameBox->setAnchorPoint(ccp(0.5, 0));
        nameBox->setPosition(ccp(320, 635));
        pNode->addChild(nameBox);
        // 获得友情
        CCSprite* labelFrid = CCSprite::create("SM_getFrid.png");
        labelFrid->setAnchorPoint(ccp(0, 0));
        labelFrid->setPosition(ccp(100, 250));
        pNode->addChild(labelFrid);
        // 当前友情
        CCSprite* labelCurFrid = CCSprite::create("SM_curFrid.png");
        labelCurFrid->setAnchorPoint(ccp(0, 0));
        labelCurFrid->setPosition(ccp(100, 210));
        pNode->addChild(labelCurFrid);
        CCMenuItemImage *pConfirm = CCMenuItemImage::create( "CM_litBtn.png",
                                                         "CM_litBtn.png",
                                                         this,
                                                         menu_selector(BattleSummary::
                                                                       btnConfirm));
        pConfirm->setAnchorPoint(ccp(0.5, 0));
        pConfirm->setPosition(ccp(320, 100));
        CCMenu* menu = CCMenu::create(pConfirm, NULL);
        menu->setAnchorPoint(ccp(0, 0));
        menu->setPosition(ccp(0, 0));
        pNode->addChild(menu);
        CCSprite* btnfont = CCSprite::create("CM_confirm.png");
        btnfont->setAnchorPoint(ccp(0, 0));
        btnfont->setPosition(ccp((pConfirm->getContentSize().width - btnfont->getContentSize().width)/2, (pConfirm->getContentSize().height
                                                                                                          - btnfont->getContentSize().height)/2));
        pConfirm->addChild(btnfont);
    }
}

/*
 *  开场动画
 */
void BattleSummary::openningAni() {

}

void BattleSummary::btnConfirm() {
    // 进入主场景的关卡副本
    GameState::getInstance()->setTagWhoCallMainScene(2);/*设置战斗调用返回主场景*/
    CCDirector::sharedDirector()->replaceScene(MainScene::create());
    int copyId = GameState::getInstance()->getCopyId();
    int levId = GameState::getInstance()->getLevId();
    if (copyId == GameState::getInstance()->getNormalMaxCopyId()) {
        
    }
    if (Battle::getInstance()->getWin()) {
        if (levId == GameState::getInstance()->getNormalMaxLevId()) {
            if ((CopyManager::getInstance()->getCopyData(copyId))->getCountOfLevel() ==
                (GameState::getInstance()->getNormalMaxLevId() % 100)) {
                copyId++;
                levId = copyId * 100 + 1;
                if (copyId > 8) {
                    copyId = 8;
                    levId = CopyManager::getInstance()->getCountOfLevels(copyId) + copyId * 100;
                }
                GameState::getInstance()->setNormalMax(copyId, levId);
            } else {
                levId++;
                GameState::getInstance()->setNormalMax(copyId, levId);
            }
        }
        CCLog("copyid = %d, levid = %d",copyId, levId);
    }
    
    /*移除磁层*/
    this->removeFromParent();
}

void BattleSummary::option() {
	CCMenu* pMenu = (CCMenu*)this->getChildByTag(CONTAINER)->getChildByTag(MENUOFBG);
    pMenu->setEnabled(true);
    m_isSummary = false;
}

void BattleSummary::setVisible(bool visible) {
    CCLayer::setVisible(visible);
    if (visible) {
        m_isSummary = true;
        updateUi();
    }
}

/*结算动画*/
void BattleSummary::_summaryAni() {
    /*播放铜钱*/
    CCNode* nodeOne = CCNode::create();
    addChild(nodeOne);
    nodeOne->runAction(CCSequence::create(CCDelayTime::create(ANI_TIME),CCCallFunc::create(this, callfunc_selector(BattleSummary::_copAni)) ,NULL));
    CCNode* nodeTwo = CCNode::create();
    addChild(nodeTwo);
    nodeTwo->runAction(CCSequence::create(CCDelayTime::create(ANI_TIME + ANI_TIME_DETA*0.5),CCCallFunc::create(this, callfunc_selector(BattleSummary::_expAni)) ,NULL));
    
    CCNode* nodeFour = CCNode::create();
    addChild(nodeFour);
    nodeFour->runAction(CCSequence::create(CCDelayTime::create(ANI_TIME + ANI_TIME_DETA*1),CCCallFunc::create(this, callfunc_selector(BattleSummary::_levAni)) ,NULL));
    
    CCNode* nodeThree = CCNode::create();
    addChild(nodeThree);
    nodeThree->runAction(CCSequence::create(CCDelayTime::create(ANI_TIME + ANI_TIME_DETA*1.5),CCCallFunc::create(this, callfunc_selector(BattleSummary::_heroAni)), NULL));
}



void BattleSummary::_expAni() {
    schedule(schedule_selector(BattleSummary::_scheduleForExp), 0.001);
}

void BattleSummary::_scheduleForExp() {
    char buffer[16];
    CCLabelTTF* l = (CCLabelTTF*)this->getChildByTag(CONTAINER)->getChildByTag(NODE_TAG)->getChildByTag(EXP_ANI_TAG);
    l->setVisible(true);
    int getExp = GameState::getInstance()->getCopyForExp();
    if (m_showExp < getExp) {
        m_showExp += getExp / 30;
        sprintf(buffer, "%d", m_showExp);
        l->setString(buffer);
    } else {
        sprintf(buffer, "%d", getExp);
        l->setString(buffer);
        unschedule(schedule_selector(BattleSummary::_scheduleForExp));
    }
}

void BattleSummary::_copAni() {
    schedule(schedule_selector(BattleSummary::_scheduleForCop), 0.001);
}

void BattleSummary::_scheduleForCop() {
    char buffer[16];
    CCLabelTTF* l = (CCLabelTTF*)this->getChildByTag(CONTAINER)->getChildByTag(NODE_TAG)->getChildByTag(COP_ANI_TAG);
    l->setVisible(true);
    int getCop = GameState::getInstance()->getCopyForMoney();
    if (m_cop < getCop) {
        m_cop += getCop / 50;
        sprintf(buffer, "%d", m_cop);
        l->setString(buffer);
    } else {
        sprintf(buffer, "%d", getCop);
        l->setString(buffer);
        unschedule(schedule_selector(BattleSummary::_scheduleForCop));
    }
}

void BattleSummary::_levAni() {
    schedule(schedule_selector(BattleSummary::_scheduleForLev), 0.01);
}

void BattleSummary::_scheduleForLev() {
    char buffer[16];
    CCLabelTTF* l = (CCLabelTTF*)this->getChildByTag(CONTAINER)->getChildByTag(NODE_TAG)->getChildByTag(LEV_ANI_TAG);
    CCProgressTimer *expProgeress = (CCProgressTimer *)this->getChildByTag(CONTAINER)->getChildByTag(NODE_TAG)->getChildByTag(PRO_ANI_TAG);
    if (m_lev < m_curLev) {
        m_pro += 10.0f;
        expProgeress->setPercentage(m_pro);
        if (m_pro >= 100.0f) {
            m_pro = 0.0f;
            m_lev++;
            sprintf(buffer, "%d", m_lev);
            l->setString(buffer);
        }
    } else {
        m_pro = ((float)m_curExp / (float)GamePlayer::getInstance()->getExpByLev(m_curLev)) * 100.0;
        CCProgressTo * to= CCProgressTo::create(0.2, m_pro);
        expProgeress->runAction(to);
        unschedule(schedule_selector(BattleSummary::_scheduleForLev));
    }
}

void BattleSummary::_heroAni() {
    schedule(schedule_selector(BattleSummary::_scheduleForHero), 0.4);
}

void BattleSummary::_scheduleForHero() {
    CardBox* card = (CardBox*)this->getChildByTag(CONTAINER)->getChildByTag(NODE_TAG)->getChildByTag(CARD_TAG + m_heroIdx);
    CCSprite* spoilFont = (CCSprite*)this->getChildByTag(CONTAINER)->getChildByTag(NODE_TAG)->getChildByTag(SPOIL_FONT_TAG);
    if (spoilFont)
        if (m_heroIdx == 0) {
            spoilFont->runAction(CCFadeIn::create(0.3));
        }
    if (card) {
        card->runFadeInAction(0.5, 0.3);
    } else {
        unschedule(schedule_selector(BattleSummary::_scheduleForHero));
        /*回调可触摸*/
        /*播放动画,动画完回调按钮触摸*/
        CCNode* nodeNull = CCNode::create();
        addChild(nodeNull);
        nodeNull->runAction(CCSequence::create(CCCallFunc::create(this, callfunc_selector(BattleSummary::_prisonHeroAni)),NULL));
    }
    m_heroIdx++;
}

void BattleSummary::_prisonHeroAni() {
    CardBox* card = (CardBox*)this->getChildByTag(CONTAINER)->getChildByTag(NODE_TAG)->getChildByTag(PRISON_CARD_TAG);
    if (card)
        card->tongQueAction(0.5, 0.001);
    option();
}