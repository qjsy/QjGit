//
//  MainCityLayer.cpp
//  hero
//
//  Created by yangjie on 2013/11/28.
//
// 主城层（主界面首页）
// 隶属于主场景
//

#include "MainCityLayer.h"
#include "customMessage.h"
#include "GameState.h"
#include "NetConnection.h"
#include "GamePlayer.h"
#include "Battle.h"

#define CONTAINER_TAG               1500

#define M_CITY_IMAGE_FILE           "mcity.pvr.ccz"
#define M_CITY_LIST_FILE            "mcity.plist"

#define MC_MENU_HIGH_LIGTH_OFFSET_H           0.0f
#define MC_MENU_HIGH_LIGTH_OFFSET_V           8.0f

/*
 *  构造函数
 */
MainCityLayer::MainCityLayer():
m_pGameState(GameState::getInstance())
,m_pBattle(Battle::getInstance())
,m_touchEnable(true)
{
    memset(m_menuButtonSprite, '\0', sizeof(m_menuButtonSprite));
    
    // 逐鹿中原
    m_menuButtonPosition[0][0] = ccp(460.0f, 0.0f);     // 按钮
    m_menuButtonPosition[0][1] = ccp(465.0f, 80.0f);    // 文字
    m_menuEnable[0] = true;
    // 铜雀台
    m_menuButtonPosition[1][0] = ccp(0.0f, 320.0f);     // 按钮
    m_menuButtonPosition[1][1] = ccp(50.0f, 290.0f);    // 文字
    m_menuEnable[1] = true;
    // 升阶
    m_menuButtonPosition[2][0] = ccp(175.0f, 190.0f);   // 按钮
    m_menuButtonPosition[2][1] = ccp(245.0f, 145.0f);   // 文字
    m_menuEnable[2] = true;
    // 驿站
    m_menuButtonPosition[3][0] = ccp(420.0f, 245.0f);   // 按钮
    m_menuButtonPosition[3][1] = ccp(505.0f, 230.0f);   // 文字
    m_menuEnable[3] = true;
    // 铁匠铺
    m_menuButtonPosition[4][0] = ccp(180.0f, 45.0f);    // 按钮
    m_menuButtonPosition[4][1] = ccp(190.0f, 20.0f);    // 文字
    m_menuEnable[4] = true;
    // 国战
    m_menuButtonPosition[5][0] = ccp(460.0f, 365.0f);   // 按钮
    m_menuButtonPosition[5][1] = ccp(485, 350.0f);      // 文字
    m_menuEnable[5] = true;
    // 左未开放
    m_menuButtonPosition[6][0] = ccp(0.0f, 105.0f);     // 按钮
    m_menuButtonPosition[6][1] = ccp(30.0f, 95.0f);     // 文字
    m_menuEnable[6] = false;
    // 监狱
    m_menuButtonPosition[7][0] = ccp(310, 325.0f);      // 按钮
    m_menuButtonPosition[7][1] = ccp(330.0f, 315.0f);   // 文字
    m_menuEnable[7] = true;
}

/*
 *  析构函数
 */
MainCityLayer::~MainCityLayer() {
    CCSpriteFrameCache* pSpriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCTextureCache* pTextureCache = CCTextureCache::sharedTextureCache();

    pSpriteFrameCache->removeSpriteFramesFromFile(M_CITY_LIST_FILE);
    pTextureCache->removeTextureForKey(M_CITY_IMAGE_FILE);
}

/*
 *  初始化
 */
bool MainCityLayer::init() {
    // super init
    if (!CCLayer::init()) {
        return false;
    }
    this->setTouchEnabled(true);
    
    /*
     * 初始化
     */
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(M_CITY_LIST_FILE, M_CITY_IMAGE_FILE);
    this->addChild(m_pSBN = CCSpriteBatchNode::create(M_CITY_IMAGE_FILE));
    
    // 主城背景图
    CCSprite* pBgSprite = CCSprite::createWithSpriteFrameName("MC_cityBg.png");
    pBgSprite->setAnchorPoint(CCPointZero);
    pBgSprite->setPosition(ccp(0.0f, m_pGameState->getBottomOffset() + 122.0f));
    m_pSBN->addChild(pBgSprite, 1, CONTAINER_TAG);
    
    /* 
     *   按钮
     */
    // 逐鹿中原
    pBgSprite->addChild(m_menuButtonSprite[0][0] = CCSprite::createWithSpriteFrameName("MC_6.png"), 9);
    m_menuButtonSprite[0][0]->setAnchorPoint(CCPointZero);
    pBgSprite->addChild(m_menuButtonSprite[0][1] = CCSprite::createWithSpriteFrameName("MC_F_6.png"), 11);
    m_menuButtonSprite[0][1]->setAnchorPoint(CCPointZero);
    pBgSprite->addChild(m_menuButtonSprite[0][2] = CCSprite::createWithSpriteFrameName("MC_6s.png"), 10);
    m_menuButtonSprite[0][2]->setAnchorPoint(CCPointZero);
    
    // 铜雀台
    pBgSprite->addChild(m_menuButtonSprite[1][0] = CCSprite::createWithSpriteFrameName("MC_4.png"), 9);
    m_menuButtonSprite[1][0]->setAnchorPoint(CCPointZero);
    pBgSprite->addChild(m_menuButtonSprite[1][1] = CCSprite::createWithSpriteFrameName("MC_F_4.png"), 11);
    m_menuButtonSprite[1][1]->setAnchorPoint(CCPointZero);
    pBgSprite->addChild(m_menuButtonSprite[1][2] = CCSprite::createWithSpriteFrameName("MC_4s.png"), 10);
    m_menuButtonSprite[1][2]->setAnchorPoint(CCPointZero);
    
    // 升阶
    pBgSprite->addChild(m_menuButtonSprite[2][0] = CCSprite::createWithSpriteFrameName("MC_2.png"), 9);
    m_menuButtonSprite[2][0]->setAnchorPoint(CCPointZero);
    pBgSprite->addChild(m_menuButtonSprite[2][1] = CCSprite::createWithSpriteFrameName("MC_F_2.png"), 11);
    m_menuButtonSprite[2][1]->setAnchorPoint(CCPointZero);
    pBgSprite->addChild(m_menuButtonSprite[2][2] = CCSprite::createWithSpriteFrameName("MC_2s.png"), 10);
    m_menuButtonSprite[2][2]->setAnchorPoint(CCPointZero);
    
    // 驿站
    pBgSprite->addChild(m_menuButtonSprite[3][0] = CCSprite::createWithSpriteFrameName("MC_5.png"), 9);
    m_menuButtonSprite[3][0]->setAnchorPoint(CCPointZero);
    pBgSprite->addChild(m_menuButtonSprite[3][1] = CCSprite::createWithSpriteFrameName("MC_F_5.png"), 11);
    m_menuButtonSprite[3][1]->setAnchorPoint(CCPointZero);
    pBgSprite->addChild(m_menuButtonSprite[3][2] = CCSprite::createWithSpriteFrameName("MC_5s.png"), 10);
    m_menuButtonSprite[3][2]->setAnchorPoint(CCPointZero);
    
    // 铁匠铺
    pBgSprite->addChild(m_menuButtonSprite[4][0] = CCSprite::createWithSpriteFrameName("MC_3.png"), 9);
    m_menuButtonSprite[4][0]->setAnchorPoint(CCPointZero);
    pBgSprite->addChild(m_menuButtonSprite[4][1] = CCSprite::createWithSpriteFrameName("MC_F_3.png"), 11);
    m_menuButtonSprite[4][1]->setAnchorPoint(CCPointZero);
    pBgSprite->addChild(m_menuButtonSprite[4][2] = CCSprite::createWithSpriteFrameName("MC_3s.png"), 10);
    m_menuButtonSprite[4][2]->setAnchorPoint(CCPointZero);

    // 国战
    pBgSprite->addChild(m_menuButtonSprite[5][0] = CCSprite::createWithSpriteFrameName("MC_1.png"), 9);
    m_menuButtonSprite[5][0]->setAnchorPoint(CCPointZero);
    pBgSprite->addChild(m_menuButtonSprite[5][1] = CCSprite::createWithSpriteFrameName("MC_F_1.png"), 11);
    m_menuButtonSprite[5][1]->setAnchorPoint(CCPointZero);
    pBgSprite->addChild(m_menuButtonSprite[5][2] = CCSprite::createWithSpriteFrameName("MC_1s.png"), 10);
    m_menuButtonSprite[5][2]->setAnchorPoint(CCPointZero);
    
    // 左未开放
    pBgSprite->addChild(m_menuButtonSprite[6][0] = CCSprite::createWithSpriteFrameName("MC_zuo.png"), 9);
    m_menuButtonSprite[6][0]->setAnchorPoint(CCPointZero);
    pBgSprite->addChild(m_menuButtonSprite[6][1] = CCSprite::createWithSpriteFrameName("wkfs.png"), 11);
    m_menuButtonSprite[6][1]->setAnchorPoint(CCPointZero);
    pBgSprite->addChild(m_menuButtonSprite[6][2] = CCSprite::createWithSpriteFrameName("MC_zuo.png"), 10);
    m_menuButtonSprite[6][2]->setAnchorPoint(CCPointZero);
    
    // 监狱
    pBgSprite->addChild(m_menuButtonSprite[7][0] = CCSprite::createWithSpriteFrameName("MC_7.png"), 12);
    m_menuButtonSprite[7][0]->setAnchorPoint(CCPointZero);
    pBgSprite->addChild(m_menuButtonSprite[7][1] = CCSprite::createWithSpriteFrameName("MC_F_7.png"), 14);
    m_menuButtonSprite[7][1]->setAnchorPoint(CCPointZero);
    pBgSprite->addChild(m_menuButtonSprite[7][2] = CCSprite::createWithSpriteFrameName("MC_7s.png"), 13);
    m_menuButtonSprite[7][2]->setAnchorPoint(CCPointZero);
    
    _initMenuButton();
    return true;
}

//
void MainCityLayer::_initMenuButton(void) {
    ((CCSprite*)m_pSBN->getChildByTag(CONTAINER_TAG))->setVisible(true);
    for (unsigned int i = 0; i < MC_MENU_NUMBER; i++) {
        m_menuButtonSprite[i][0]->setVisible(true);
        m_menuButtonSprite[i][0]->setPosition(m_menuButtonPosition[i][0]);
        m_menuButtonSprite[i][1]->setVisible(true);
        m_menuButtonSprite[i][1]->setPosition(m_menuButtonPosition[i][1]);
        m_menuButtonSprite[i][2]->setVisible(false);
    } /*for*/
}

//
void MainCityLayer::registerWithTouchDispatcher(void) {
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, false);
}

//
bool MainCityLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    if (!m_touchEnable) return false;
    
    _initMenuButton();
    
    m_touchBeganMenuIndex = -1;
    CCSprite* pBg = static_cast<CCSprite*>(m_pSBN->getChildByTag(CONTAINER_TAG));
    CCPoint touchPoint = pBg->convertTouchToNodeSpace(pTouch);
    
    for (unsigned int i = 0; i < MC_MENU_NUMBER; i++) {
        if (m_menuEnable[i] && m_menuButtonSprite[i][0]->boundingBox().containsPoint(touchPoint)) {
            m_touchBeganMenuIndex = i;
            
            m_menuButtonSprite[i][0]->setVisible(false);
            m_menuButtonSprite[i][2]->setVisible(true);
            
            CCPoint menuPos = m_menuButtonPosition[i][0];
            CCPoint textPos = m_menuButtonPosition[i][1];
            
            menuPos.x += MC_MENU_HIGH_LIGTH_OFFSET_H;
            menuPos.y += MC_MENU_HIGH_LIGTH_OFFSET_V;
            textPos.x += MC_MENU_HIGH_LIGTH_OFFSET_H;
            textPos.y += MC_MENU_HIGH_LIGTH_OFFSET_V;
            
            m_menuButtonSprite[i][1]->setPosition(textPos);
            m_menuButtonSprite[i][2]->setPosition(menuPos);
            
            break;
        }
    } /*for*/
    
    return true;
}

//
void MainCityLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {
    _initMenuButton();
    
    CCSprite* pBg = static_cast<CCSprite*>(m_pSBN->getChildByTag(CONTAINER_TAG));
    CCPoint touchPoint = pBg->convertTouchToNodeSpace(pTouch);
    
    for (unsigned int i = 0; i < MC_MENU_NUMBER; i++) {
        if (m_menuEnable[i] && m_menuButtonSprite[i][0]->boundingBox().containsPoint(touchPoint)) {
            
            m_menuButtonSprite[i][0]->setVisible(false);
            m_menuButtonSprite[i][2]->setVisible(true);
            
            CCPoint menuPos = m_menuButtonPosition[i][0];
            CCPoint textPos = m_menuButtonPosition[i][1];
            
            menuPos.x += MC_MENU_HIGH_LIGTH_OFFSET_H;
            menuPos.y += MC_MENU_HIGH_LIGTH_OFFSET_V;
            textPos.x += MC_MENU_HIGH_LIGTH_OFFSET_H;
            textPos.y += MC_MENU_HIGH_LIGTH_OFFSET_V;
            
            m_menuButtonSprite[i][1]->setPosition(textPos);
            m_menuButtonSprite[i][2]->setPosition(menuPos);
            
            break;
        }
    } /*for*/
}

//
void MainCityLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {
    CCSprite* pBg = static_cast<CCSprite*>(m_pSBN->getChildByTag(CONTAINER_TAG));
    CCPoint touchPoint = pBg->convertTouchToNodeSpace(pTouch);
    
    int selectedMenuIndex = -1;
    for (unsigned int i = 0; i < MC_MENU_NUMBER; i++) {
        if (m_menuEnable[i] && m_touchBeganMenuIndex == i && m_menuButtonSprite[i][0]->boundingBox().containsPoint(touchPoint)) {
            selectedMenuIndex = i + 1;
            break;
        }
    } /*for*/
    
    _initMenuButton();
    
    if (selectedMenuIndex > 0) /* 玩家选择了某个菜单选项 */ {
        onMcMenuSelect(selectedMenuIndex);
    }
}

//
void MainCityLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {
    _initMenuButton();
}

/*
 *  响应菜单选择
 */
void MainCityLayer::onMcMenuSelect(const unsigned int menuIndex) {
    switch (menuIndex) {
    case 1:     // 逐鹿中原
        {
            m_pBattle->setCWar(false);
            m_pBattle->setBattlePkMode(BPM_PVE);
            CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_ZHULU_STRING);
        }
        break;
    case 2:     // 铜雀台
        {
            NetConnection* pNetConnection = NetConnection::getInstance();
            char szPostBuffer[MAX_POSTTO_SERVER_BUFFER_SIZE];
            memset(szPostBuffer, '\0', MAX_POSTTO_SERVER_BUFFER_SIZE);
            
            // 构造 post
            sprintf(szPostBuffer, "c=contest&a=enter&uid=%s", GamePlayer::getInstance()->getUid().c_str());
            
            char szHttpTag[] = HTTP_ENTER_TONGQUETAI;
            pNetConnection->commitPostRequestData(szPostBuffer, szHttpTag);
        }
            break;
        case 3:     // 升级升阶
            CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_LEVUP_STRING);
            break;
        case 4:     // 驿站
            CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_YIZHAN_STRING);
            break;
        case 5:     // 铁匠铺
            CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_TIEJIANG_STRING);
            break;
        case 6:     // 国战
            {
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
            break;
        case 7:     // 左未开放
            break;
        case 8:     // 监狱
            //
        {
            NetConnection* pNetConnection = NetConnection::getInstance();
            char szPostBuffer[MAX_POSTTO_SERVER_BUFFER_SIZE];
            memset(szPostBuffer, '\0', MAX_POSTTO_SERVER_BUFFER_SIZE);
            strcpy(szPostBuffer, "c=prison&a=enter");
            char szHttpTag[] = HTTP_REQUEST_JIANYUINIT_TAG;
            strcat(szPostBuffer, "&uid=");
            strcat(szPostBuffer, GamePlayer::getInstance()->getUid().c_str());
            CCLog("post 购买 = %s", szPostBuffer);
            pNetConnection->commitPostRequestData(szPostBuffer, szHttpTag);
        }
            break;
        default:
            break;
    }
    
}

/*
 *  设置不可见的不可触摸
*/
void MainCityLayer::setVisible(bool visible) {
    CCLayer::setVisible(visible);
    this->setTouchEnabled(visible);
    m_touchEnable = visible;
    
    if (visible)
        _initMenuButton();
}

void MainCityLayer::restoreTouchUICompent() {
    m_touchEnable = true;
    _initMenuButton();
}

void MainCityLayer::moveOutTouchUICompent() {
    m_touchEnable = false;
    for (unsigned int i = 0; i < MC_MENU_NUMBER; i++) {
        m_menuButtonSprite[i][0]->setVisible(false);
        m_menuButtonSprite[i][1]->setVisible(false);
        m_menuButtonSprite[i][2]->setVisible(false);
    } /*for*/
}

