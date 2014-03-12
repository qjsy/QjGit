//
//  MenuOfMainSceneLayer.cpp
//  hero
//
//  Created by yangjie on 2013/11/28.
//

#include "MenuOfMainSceneLayer.h"
#include "GameState.h"
#include "customMessage.h"

// 主菜单大图文件
#define MAIN_MENU_IMAGE_FILE        "mainMenuIcon.pvr.ccz"
#define MAIN_MENU_LIST_FILE         "mainMenuIcon.plist"
#define MAIN_MENU_IMAGE_TAG         10

#define IMAGE_TOP                   "top.png"
#define IMAGE_BOTTOM                "bottom.png"

// 菜单栏背景图片
#define MENUBAR_BACKGROUND_IMAGE    "menuIconlayerBackground.png"
// 菜单框图片
#define ICON_BOX_IMAGE              "iconbox.png"
// 菜单选中效果图片
#define MENU_SELECTED_IMAGE         "M_select.png"

// 主城菜单按钮、文字图片
#define MAIN_CITY_ICON_IMAGE        "mainCityIcon.png"
#define MAIN_CITY_FONT_IMAGE        "M_font_city.png"

// 英雄列表菜单按钮、文字图片
#define HEROS_ICON_IMAGE            "herosIcon.png"
#define HEROS_FONT_IMAGE            "M_font_hero.png"
// 商城菜单按钮、文字图片
#define SHOP_ICON_IMAGE             "shopIcon.png"
#define SHOP_FONT_IMAGE             "M_font_shop.png"
// 好友菜单按钮、文字图片
#define FRIENDS_ICON_IMAGE          "friendsIcon.png"
#define FRIENDS_FONT_IMAGE          "M_font_frid.png"
// 选项菜单按钮、文字图片
#define OPTION_ICON_IMAGE           "optionIcon.png"
#define OPTION_FONT_IMAGE           "M_font_chos.png"

// 菜单栏背景 tag
#define MENUBAR_BACKGROUND_TAG      100

// menubar 左边距
#define MENU_PADDING_LEFT           62.0f

// 菜单选项单元格宽度
#define MENUBAR_WIDTH               100.0f

// 菜单文字偏移
#define MENUFONT_HORIZONTAL         48.0f
#define MENUFONT_VERTICAL           24.0f

// 菜单选项单元格之间的间隔
#define MENUBAR_CELL_SPACING        28.0f

#define MOVEOUT_ACTION_TAG          1400
#define RESTORE_ACTION_TAG          1401

#define MOVE_ACTION_DRUATION        0.2f

/*
 *   构造函数
 */
MenuOfMainSceneLayer::MenuOfMainSceneLayer():m_pMainMenuCBN(NULL)
,m_enableTouch(true)
,m_menuSelected(NULL)
,m_pMenuBackgroundSprite(NULL)
,m_isInScreen(true)
{
    for (unsigned int i = 0; i < 5; i++) {
        m_pMenuBar[i] = m_pMenuBarBox[i] = NULL;
    } /*for*/
}

/*
 *  析构函数
 */
MenuOfMainSceneLayer::~MenuOfMainSceneLayer() {
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(MAIN_MENU_LIST_FILE);
    CCTextureCache::sharedTextureCache()->removeTextureForKey(MAIN_MENU_IMAGE_FILE);
}

/*
 *   初始化
 */
bool MenuOfMainSceneLayer::init() {
    ///////////////////////////////
    // super first
    if (!CCLayer::init()) {
        return false;
    }
    
    // 允许接收 touch 事件
    this->setTouchEnabled(true);

    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(MAIN_MENU_LIST_FILE, MAIN_MENU_IMAGE_FILE);
    this->addChild(m_pMainMenuCBN = CCSpriteBatchNode::create(MAIN_MENU_IMAGE_FILE), 1, MAIN_MENU_IMAGE_TAG);
    
    if (GameState::getInstance()->is1136()) /*1136 分辨率*/ {
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
    
    // 添加菜单栏背景
    m_pMenuBackgroundSprite = CCSprite::createWithSpriteFrameName(MENUBAR_BACKGROUND_IMAGE);
    m_pMainMenuCBN->addChild(m_pMenuBackgroundSprite, 1, MENUBAR_BACKGROUND_TAG);
    
    // 添加菜单选中
    m_pMenuBackgroundSprite->addChild(m_menuSelected = CCSprite::createWithSpriteFrameName(MENU_SELECTED_IMAGE), 5);
    // 添加主城菜单按钮、文字
    m_pMenuBackgroundSprite->addChild(m_pMenuBar[0] = CCSprite::createWithSpriteFrameName(MAIN_CITY_ICON_IMAGE), 3);
    m_pMenuBar[0]->addChild(m_pMenuFont[0] = CCSprite::createWithSpriteFrameName(MAIN_CITY_FONT_IMAGE), 4);
    // 添加英雄列表菜单按钮、文字
    m_pMenuBackgroundSprite->addChild(m_pMenuBar[1] = CCSprite::createWithSpriteFrameName(HEROS_ICON_IMAGE), 3);
    m_pMenuBar[1]->addChild(m_pMenuFont[1] = CCSprite::createWithSpriteFrameName(HEROS_FONT_IMAGE), 4);
    // 添加商城菜单按钮、文字
    m_pMenuBackgroundSprite->addChild(m_pMenuBar[2] = CCSprite::createWithSpriteFrameName(SHOP_ICON_IMAGE), 3);
    m_pMenuBar[2]->addChild(m_pMenuFont[2] = CCSprite::createWithSpriteFrameName(SHOP_FONT_IMAGE), 4);
    // 添加好友菜单按钮、文字
    m_pMenuBackgroundSprite->addChild(m_pMenuBar[3] = CCSprite::createWithSpriteFrameName(FRIENDS_ICON_IMAGE), 3);
    m_pMenuBar[3]->addChild(m_pMenuFont[3] = CCSprite::createWithSpriteFrameName(FRIENDS_FONT_IMAGE), 4);
    // 添加好友菜单按钮、文字
    m_pMenuBackgroundSprite->addChild(m_pMenuBar[4] = CCSprite::createWithSpriteFrameName(OPTION_ICON_IMAGE), 3);
    m_pMenuBar[4]->addChild(m_pMenuFont[4] = CCSprite::createWithSpriteFrameName(OPTION_FONT_IMAGE), 4);
    
    // 菜单框
    for (unsigned int i = 0; i < 5; i++) {
        m_pMenuBackgroundSprite->addChild(m_pMenuBarBox[i] = CCSprite::createWithSpriteFrameName(ICON_BOX_IMAGE), 2);
    } /*for*/
    
    // 初始化菜单选项（位置）
    initPosition();
    
    m_isMoveOut = m_isRestore = false;

    return true;
}

/*
 *  功能：
 *      初始化选项菜单
 */
void MenuOfMainSceneLayer::initPosition() {
    m_pMenuBackgroundSprite->setVisible(true);
    m_pMenuBackgroundSprite->setAnchorPoint(CCPointZero);
    m_pMenuBackgroundSprite->setPosition(ccp(0.0f, -26.0f + GameState::getInstance()->getBottomOffset()));
    
    // 菜单选项
    const float menuIconY = 86.0f;
    
    for (unsigned int i = 0; i < 5; i++) {
        m_pMenuBar[i]->setVisible(true);
        m_pMenuBar[i]->setPosition(ccp(MENU_PADDING_LEFT + (MENUBAR_WIDTH + MENUBAR_CELL_SPACING) * i, menuIconY));
        if (i == 0) {
           m_pMenuFont[i]->setPosition(ccp(MENUFONT_HORIZONTAL - 4.0f, MENUFONT_VERTICAL));
        } else if (i == 1) {
           m_pMenuFont[i]->setPosition(ccp(MENUFONT_HORIZONTAL, MENUFONT_VERTICAL - 5.0f));
        } else if (i == 2){
            m_pMenuFont[i]->setPosition(ccp(MENUFONT_HORIZONTAL, MENUFONT_VERTICAL + 2.0f));
        } else if (i == 3) {
            m_pMenuFont[i]->setPosition(ccp(MENUFONT_HORIZONTAL, MENUFONT_VERTICAL + 2.0f));
        } else {
            m_pMenuFont[i]->setPosition(ccp(MENUFONT_HORIZONTAL, MENUFONT_VERTICAL));
        }
        
        m_pMenuBarBox[i]->setVisible(true);
        m_pMenuBarBox[i]->setPosition(ccp(MENU_PADDING_LEFT + (MENUBAR_WIDTH + MENUBAR_CELL_SPACING) * i, menuIconY));
    } /*for*/
    
    m_menuSelected->setVisible(false);
}

/*
 *
 */
void MenuOfMainSceneLayer::onEnter() {
    CCLayer::onEnter();
    
}

/*
 *
 */
void MenuOfMainSceneLayer::onExit() {
    
    
    CCLayer::onExit();
}

//
void MenuOfMainSceneLayer::registerWithTouchDispatcher(void) {
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, false);
}

/*
 *  moveOut 动画完成回调
 */
void MenuOfMainSceneLayer::moveOutIsDoneCallback() {
    m_isMoveOut = false;
}

/*
 *  restore 动画完成回调
 */
void MenuOfMainSceneLayer::restoreIsDoneCallback() {
    m_isRestore = false;
    m_enableTouch = true;
}

/* 将能响应 touch 事件的 ui 组件移动到屏幕外面 */
void MenuOfMainSceneLayer::moveOutTouchUICompent() {
    m_enableTouch = false;
    m_menuSelected->setVisible(false);
    if (m_isInScreen) {
        if (m_isRestore) /* restore 动画进行中 */{
            m_isRestore = m_isMoveOut = false;
            m_pMenuBackgroundSprite->stopAllActions();
            for (unsigned int i = 0; i < 5; i++) {
                m_pMenuBar[i]->stopAllActions();
                m_pMenuBarBox[i]->stopAllActions();
            } /**/
        }
        
        if (!m_isMoveOut) {
            CCPoint moveOffsetX = ccp(CCDirector::sharedDirector()->getVisibleSize().width, 0.0f);
            
            initPosition();
            m_pMenuBackgroundSprite->runAction(CCSequence::create(CCMoveBy::create(MOVE_ACTION_DRUATION, moveOffsetX),
                                                                  CCCallFuncN::create(this, callfuncN_selector(MenuOfMainSceneLayer::moveOutIsDoneCallback)), NULL));
            m_isMoveOut = true;
        }
    }
    
    m_isInScreen = false;
}

/* 恢复能响应 touch 事件的 ui 组件的位置 */
void MenuOfMainSceneLayer::restoreTouchUICompent() {
    m_menuSelected->setVisible(false);
    if (!m_isInScreen) {
        if (m_isMoveOut) {
            m_isMoveOut = m_isRestore = false;
            m_pMenuBackgroundSprite->stopAllActions();
            for (unsigned int i = 0; i < 5; i++) {
                m_pMenuBar[i]->stopAllActions();
                m_pMenuBarBox[i]->stopAllActions();
            } /**/
        }
        
        if (!m_isRestore) {
            initPosition();
            
            CCPoint moveOffsetX = ccp(-CCDirector::sharedDirector()->getVisibleSize().width, 0.0f);
            for (unsigned int i = 0; i < 5; i++) {
                CCPoint pos = m_pMenuBar[i]->getPosition();
                pos.x += CCDirector::sharedDirector()->getVisibleSize().width;
                m_pMenuBar[i]->setPosition(pos);
                pos = m_pMenuBarBox[i]->getPosition();
                pos.x += CCDirector::sharedDirector()->getVisibleSize().width;
                m_pMenuBarBox[i]->setPosition(pos);
                
                if (i > 0) {
                    m_pMenuBar[i]->runAction(CCEaseBounceOut::create(CCMoveBy::create(MOVE_ACTION_DRUATION, moveOffsetX)));
                }
                m_pMenuBarBox[i]->runAction(CCEaseBounceOut::create(CCMoveBy::create(MOVE_ACTION_DRUATION, moveOffsetX)));
            } /*for*/
            m_pMenuBar[0]->runAction(CCSequence::create(CCEaseBounceOut::create(CCMoveBy::create(MOVE_ACTION_DRUATION, moveOffsetX)),CCCallFuncN::create(this, callfuncN_selector(MenuOfMainSceneLayer::restoreIsDoneCallback)), NULL));
            m_isRestore = true;
        }

    }
    m_isInScreen = true;
}

//
bool MenuOfMainSceneLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    if (!m_enableTouch) return false;

    CCPoint touchPoint = m_pMenuBackgroundSprite->convertTouchToNodeSpace(pTouch);
    
    bool isSelectedMenu = false;        // 是否选中了某个菜单
    for (unsigned int i = 0; i < 5; i++) {
        if (m_pMenuBar[i]->boundingBox().containsPoint(touchPoint)) {
            m_menuSelected->setVisible(isSelectedMenu = true);
            m_menuSelected->setPosition(m_pMenuBar[i]->getPosition());
            break;
        }
    } /*for*/
    return isSelectedMenu;
}

//
void MenuOfMainSceneLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {
    CCPoint touchPoint = m_pMenuBackgroundSprite->convertTouchToNodeSpace(pTouch);
    
    bool isSelectedMenu = false;        // 是否选中了某个菜单
    for (unsigned int i = 0; i < 5; i++) {
        if (m_pMenuBar[i]->boundingBox().containsPoint(touchPoint)) {
            isSelectedMenu = true;
            m_menuSelected->setPosition(m_pMenuBar[i]->getPosition());
            break;
        }
    } /*for*/
    
    m_menuSelected->setVisible(isSelectedMenu);
}

//
void MenuOfMainSceneLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {
    CCPoint touchPoint = m_pMenuBackgroundSprite->convertTouchToNodeSpace(pTouch);
    
    int selectedMenuIndex = 0;         // 玩家选择的菜单选项
    for (unsigned int i = 0; i < 5; i++) {
        if (m_pMenuBar[i]->boundingBox().containsPoint(touchPoint)) {
            selectedMenuIndex = i + 1;
            m_menuSelected->setPosition(m_pMenuBar[i]->getPosition());
            break;
        }
    } /*for*/
    m_menuSelected->setVisible(false);
    
    if (selectedMenuIndex > 0) /*玩家选择了某个菜单选项*/{
        _onMenuSelected(selectedMenuIndex);
    }
}

//
void MenuOfMainSceneLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {
    initPosition();
}

/*
 *  功能:
 *      响应玩家菜单选择
 *  参数:
 *      menuIndex           菜单选项索引号(基数为 1)
 */
void MenuOfMainSceneLayer::_onMenuSelected(const unsigned int menuIndex) {
    CCNotificationCenter* pNotificationCenter = CCNotificationCenter::sharedNotificationCenter();
    if (1 == menuIndex) /*主城*/ {
        pNotificationCenter->postNotification(ON_MESSAGE_MAINCITY_STRING);
    } else if (2 == menuIndex) /*武将列表*/ {
        GameState::getInstance()->setCurrTagWhoCallHeroSel(WCH_BROWSE);
        pNotificationCenter->postNotification(ON_MESSAGE_HEROS_STRING);
    } else if (3 == menuIndex) /*商城*/ {
        pNotificationCenter->postNotification(ON_MESSAGE_SHOP_STRING);
    } else if (4 == menuIndex) /*好友*/ {
        pNotificationCenter->postNotification(ON_MESSAGE_FRIENDS_STRING);
    } else if (5 == menuIndex) /*选项*/ {
        pNotificationCenter->postNotification(ON_MESSAGE_OPTIION_STRING);
    }
}
