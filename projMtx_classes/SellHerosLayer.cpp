
#include "SellHerosLayer.h"
#include "GameState.h"
#include "HeroOfPlayerItem.h"
#include "NetConnection.h"
#include "HeroItem.h"
#include "HeroManager.h"
#include "GamePlayer.h"
#include "customMessage.h"
#include "CardBox.h"
#include "HeroAdditionItem.h"
#include "CCStrokeLabel.h"

#define OFFSET_1136                 (m_is1136 ? 72.0f : 0.0f)

#define CONTAINER                   100
#define SHOWHERO_TAG                101                     // 显示将要出售的武将，用于更新界面

#define SELHERO_TAG                 120                     //


#define LIST_PADDING                45                      // 列表偏移量
#define LIST_CELL_HEIGHT            110                     // 每个cell的高度
#define LIST_DISPLAY_ROWS           2                       // 列表显示cell的行数

/*
 *  构造函数
 */
SellHerosLayer::SellHerosLayer() {
    m_is1136 = CCDirector::sharedDirector()->getVisibleSize().height > 960;

    m_herosForSellArray = CCArray::createWithCapacity(MAX_ALLOW_SELECT_HEROS);
    m_herosForSellArray->retain();
}

/*
 *  析构函数
 */
SellHerosLayer::~SellHerosLayer() {
    CC_SAFE_RELEASE_NULL(m_herosForSellArray);
    /* 出售武将纹理资源释放 */
    CCTextureCache* pTextureCache = CCTextureCache::sharedTextureCache();
    pTextureCache->removeTextureForKey("HL_sellBg.png");
    pTextureCache->removeTextureForKey("CM_litBtn.png");
    pTextureCache->removeTextureForKey("CM_confirm.png");
    pTextureCache->removeTextureForKey("CM_cancel.png");
    pTextureCache->removeTextureForKey("HL_sellTip.png");
}

/*
 *  初始化
 */
bool SellHerosLayer::init() {
    // super init
    if (!CCLayer::init()) {
        return false;
    }
    
    
    
    // 出售显示层的背景
    CCNode* pContainer = CCNode::create();
    pContainer->setAnchorPoint(ccp(0, 0));
    pContainer->setPosition(ccp(0, OFFSET_1136 + 95.0f));
    addChild(pContainer, 1, CONTAINER);
    CCSprite* spBg = CCSprite::create("HL_sellBg.png");
    spBg->setAnchorPoint(ccp(0, 0));
    spBg->setPosition(ccp(0, 0));
    pContainer->addChild(spBg);
    // 出售确认按钮
    CCMenuItemImage *pConfirm = CCMenuItemImage::create("CM_litBtn.png",
                                                        "CM_litBtn.png",
                                                        this,
                                                        menu_selector(SellHerosLayer::
                                                                      onSellHero));
    pConfirm->setAnchorPoint(CCPointZero);
    pConfirm->setPosition(ccp(85, 230));
    CCMenu* pMenu = CCMenu::create(pConfirm, NULL);
    pMenu->setAnchorPoint(ccp(0, 0));
    pMenu->setPosition(ccp(0, 0));
    pContainer->addChild(pMenu, 2);
    CCSprite* spConfirm = CCSprite::create("CM_confirm.png");
    spConfirm->setAnchorPoint(ccp(0, 0));
    spConfirm->setPosition(ccp(85 + (pConfirm->getContentSize().width - spConfirm->getContentSize().width) / 2, 230 + (pConfirm->getContentSize().height - spConfirm->getContentSize().height) / 2));
    pContainer->addChild(spConfirm, 3);
    // 出售返回，返回到武将选择界面
    CCMenuItemImage *pCancel = CCMenuItemImage::create("CM_litBtn.png",
                                                         "CM_litBtn.png",
                                                         this,
                                                         menu_selector(SellHerosLayer::
                                                                       cancel));
    pCancel->setAnchorPoint(CCPointZero);
    pCancel->setPosition(ccp(415, 230));
    CCMenu* pMenu1 = CCMenu::create(pCancel, NULL);
    pMenu1->setAnchorPoint(ccp(0, 0));
    pMenu1->setPosition(ccp(0, 0));
    pContainer->addChild(pMenu1, 2);
    CCSprite* spCancel = CCSprite::create("CM_cancel.png");
    spCancel->setAnchorPoint(ccp(0, 0));
    spCancel->setPosition(ccp(415 + (pConfirm->getContentSize().width - spCancel->getContentSize().width) / 2, 230 + (pConfirm->getContentSize().height - spCancel->getContentSize().height) / 2));
    pContainer->addChild(spCancel, 3);
    
    // 出售标题
    CCSprite* tip = CCSprite::create("HL_sellTip.png");
    tip->setAnchorPoint(ccp(0.5, 0));
    tip->setPosition(ccp(320, 620));
    pContainer->addChild(tip, 2);
    
  
    return true;
}


//每个cell的size
CCSize SellHerosLayer::cellSizeForTable(cocos2d::extension::CCTableView *table) {
    return CCSizeMake(CCDirector::sharedDirector()->getWinSize().width - LIST_PADDING * 2, LIST_CELL_HEIGHT);
}

//生成cell
CCTableViewCell* SellHerosLayer::tableCellAtIndex(CCTableView *table, unsigned int idx) {
    CCTableViewCell *cell = table->dequeueCell();
    unsigned int countOfSelHeros = m_herosForSellArray->count();
    
    if (!cell) {
        cell = new CCTableViewCell();
        cell->autorelease();
        
        for (unsigned int col = 0; col < 5; col++) {
            unsigned int index = idx * 5 + col;
            if (index < countOfSelHeros) {
                HeroOfPlayerItem* pHeroOfPlayerItem = (HeroOfPlayerItem*)m_herosForSellArray->objectAtIndex(index);
                
                CardBox* card = CardBox::create();
                card->setCard(1, pHeroOfPlayerItem, true);
                card->setAnchorPoint(ccp(0, 0));
                card->setPosition(ccp(col * 110, 0.0f));
                cell->addChild(card, 1, SELHERO_TAG + col);
            }
        } /*for*/
    } else {
        for (unsigned int col = 0; col < 5; col++) {
            unsigned int index = idx * 5 + col;
            CardBox* card = (CardBox*)cell->getChildByTag(SELHERO_TAG + col);
            if (index < countOfSelHeros) {
                HeroOfPlayerItem* pHeroOfPlayerItem = (HeroOfPlayerItem*)m_herosForSellArray->objectAtIndex(index);
                if (!pHeroOfPlayerItem) {
                    if (card)
                        cell->removeChildByTag(SELHERO_TAG + col);
                } else {
                    if (!card) {
                        card = CardBox::create();
                        cell->addChild(card, 1, SELHERO_TAG + col);
                    }
                    card->setCard(1, pHeroOfPlayerItem, true);
                    card->setAnchorPoint(ccp(0, 0));
                    card->setPosition(ccp(col * 110, 0.0f));
                }
            } else {
                if (card)
                    cell->removeChildByTag(SELHERO_TAG + col);
            }
        } /*for*/
    }
    
    
    return cell;
}

// cell的数量
unsigned int SellHerosLayer::numberOfCellsInTableView(CCTableView *table) {
    unsigned int mod = m_herosForSellArray->count() % 5;
    return (m_herosForSellArray->count() - mod) / 5 + (mod > 0 ? 1 : 0);
}

void SellHerosLayer::setVisible(bool isVisible) {
    CCLayer::setVisible(isVisible);
    if (isVisible) {
        updateUi();
    }
}

void SellHerosLayer::updateUi() {
    char buffer[256];
    int getMoney = 0;
    
    /* 清除显示数据 */
    if (this->getChildByTag(CONTAINER)->getChildByTag(SHOWHERO_TAG)) {
        this->getChildByTag(CONTAINER)->removeChildByTag(SHOWHERO_TAG);
    }
    CCNode* pNode = CCNode::create();
    this->getChildByTag(CONTAINER)->addChild(pNode,1,SHOWHERO_TAG);
    CCObject* pObj = NULL;
    int index = 0;
    CCARRAY_FOREACH(m_herosForSellArray, pObj) {
        HeroOfPlayerItem* pHeroOfPlayerItem = (HeroOfPlayerItem*)pObj;
        int i, j;
        i = index % 5;
        j = (index - i) / 5;
        index++;
        getMoney += HeroManager::getInstance()->getAdditionHeroData(pHeroOfPlayerItem->getHeroItem()->getRarity(), pHeroOfPlayerItem->getLevel())->getGoldBySale();
    }/*for*/
    
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    /*列表大小*/
    CCSize heroSellSize = CCSizeMake(winSize.width - LIST_PADDING * 2, LIST_CELL_HEIGHT * LIST_DISPLAY_ROWS);
    /*出售列表*/
    CCTableView* heros_sell = CCTableView::create(this, heroSellSize);
    heros_sell->setDirection(kCCScrollViewDirectionVertical);
    heros_sell->setAnchorPoint(CCPointZero);
    heros_sell->setPosition(ccp(LIST_PADDING, 385.0f));
    heros_sell->setDelegate(this);
    heros_sell->setVerticalFillOrder(kCCTableViewFillTopDown);
    pNode->addChild(heros_sell, 2);
    
    // 出售文字提示
    sprintf(buffer, "总共出售 %d 个武将，获得 %d", index, getMoney);
    CCStrokeLabel* label = CCStrokeLabel::create(buffer, "Arial", 23);
    label->setAnchorPoint(ccp(0, 0));
    label->setPosition(ccp(80, 305));
    pNode->addChild(label, 1);
    CCSprite* spCop = CCSprite::create("copper.png");
    spCop->setAnchorPoint(ccp(0, 0));
    spCop->setPosition(ccp(480, 303));
    pNode->addChild(spCop);
}

void SellHerosLayer::moveOutTouchUICompent() {
    this->setVisible(false);
    this->removeFromParent();
    CCLog("出售武将层 从主场景移出");
}

void SellHerosLayer::restoreTouchUICompent() {
    this->setVisible(true);
}


/* 武将出售取消 */
void SellHerosLayer::cancel() {
    GameState::getInstance()->setHeroForSaleSuccess(false);
    GameState::getInstance()->setCurrTagWhoCallHeroSel(WCH_SALE);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_HEROS_STRING);
}

/* 武将出售确认 */
void SellHerosLayer::onSellHero() {
    /* 构造出售字符串 */
    do {
        CC_BREAK_IF(0 == m_herosForSellArray->count());
        
        char szHttpTag[16];
        memset(szHttpTag,'\0',sizeof(szHttpTag));
        strcpy(szHttpTag, HTTP_REQUEST_SELLHERO_TAG);
        
        char szPostBuffer[MAX_POSTTO_SERVER_BUFFER_SIZE];
        memset(szPostBuffer, '\0', MAX_POSTTO_SERVER_BUFFER_SIZE);
        
        strcpy(szPostBuffer, "c=hero&a=sell&list=");
        unsigned int countOfSelHeros = 0;
        CCObject* pObj = NULL;
        CCARRAY_FOREACH(m_herosForSellArray, pObj) {
            HeroOfPlayerItem* pHeroOfPlayerItem = (HeroOfPlayerItem*)pObj;
            if (countOfSelHeros > 0)
                strcat(szPostBuffer, "|");
            strcat(szPostBuffer, pHeroOfPlayerItem->getUniId().c_str());
            countOfSelHeros++;
        }/*for*/
        strcat(szPostBuffer, "&uid=");
        strcat(szPostBuffer, GamePlayer::getInstance()->getUid().c_str());
        CCLog("sell postbuffer = %s", szPostBuffer);
        NetConnection::getInstance()->commitPostRequestData(szPostBuffer, szHttpTag);
        
        CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_LOADING_MAINSCENE);
    } while (0);
}

void SellHerosLayer::setSellHeros(const unsigned int countOfSell, const char herosForSell[][HERO_UNIID_BUFFER_SIZE]) {
    do {
        m_herosForSellArray->removeAllObjects();
        for(int i = 0; i< countOfSell;++i) {
            HeroOfPlayerItem* pItem = HeroManager::getInstance()->getHeroDataByUniId(herosForSell[i]);
            if (pItem)
                m_herosForSellArray->addObject(pItem);
        }/*for*/
    } while (0);
}

unsigned int SellHerosLayer::getSellHeros(char herosForSell[][HERO_UNIID_BUFFER_SIZE]) {
    unsigned int i = 0;
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(m_herosForSellArray, pObj) {
        HeroOfPlayerItem* pItem = (HeroOfPlayerItem*)pObj;
        strcpy(herosForSell[i++],pItem->getUniId().c_str());
    }
    return m_herosForSellArray->count();
}