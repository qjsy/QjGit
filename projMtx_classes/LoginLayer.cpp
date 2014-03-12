//
//  LoginLayer.cpp
//  hero
//
// 登陆层
//
// 隶属于登陆场景
//
#include <stdlib.h>
#include "LoginLayer.h"
#include "MainScene.h"
#include "BattleScene.h"
#include "NetConnection.h"
#include "GamePlayer.h"
#include "ProcessData.h"
#include "common.h"
#include "GameState.h"
#include "LoginBox.h"
#include "customMessage.h"
#include "HeroManager.h"
#include "HeroOfPlayerItem.h"
#include "Battle.h"

#define HTTP_SERVER_URL             "http://1.moutx.sinaapp.com"        // 缺省服务器 URL
#define HTTP_SERVER_URL1            "http://192.168.11.45"              //

#define SCROLLVIEW_OFFSETX          165                                 // 滚动视图x方向偏移
#define SCROLLVIEW_OFFSETY          190                                 // 滚动视图y方向偏移
#define BOUNCE_LIMIT                0                                   // 最大可拖动偏移
#define VIEW_SIZE                        (CCSizeMake(310, 450))         // 滚动视图大小
#define SVRBG_WIDTH                 86                                  // 服务器选择 背景宽度
#define LOGIN_BG_WIDTH              2184                                // 登陆界面背景宽度
#define LOGIN_BG_TIME               70                                  // 背景移动时间

#define LAST_FLAG                   1000                                // 最近登陆旗帜
#define SVR_NAME                    2000                                // 服务器名称
#define RUN_STATE                   2500                                // 服务器拥挤精灵
#define SVRBG_NORMAL                3000                                // 服务器背景正常状态精灵
#define SVRBG_SELECT                3500                                // 服务器背景选中状态精灵
#define LOGIN_ID_TAG                3600                                // 输入用户ID

#define LOGIN_SERVER_KEY            "SERVER_KEY"

/*
 *  构造函数
 */
LoginLayer::LoginLayer()
:m_lastTime(0),
m_lastPos(CCPointZero),
m_tempPos(0),
m_xOffset(0),
m_isMoving(false),
m_numOfSvr(0),
m_pGameState(GameState::getInstance())
{}

/*
 *  析构函数
 */
LoginLayer::~LoginLayer() {
    CCTextureCache* pTextureCache = CCTextureCache::sharedTextureCache();
    
    pTextureCache->removeTextureForKey("GAME_NAME.png");
    pTextureCache->removeTextureForKey("bg_big.png");
    pTextureCache->removeTextureForKey("pre.png");
    pTextureCache->removeTextureForKey("def.png");
    pTextureCache->removeTextureForKey("start.png");
    pTextureCache->removeTextureForKey("LG_nor.png");
    pTextureCache->removeTextureForKey("LG_sel.png");
}

/*
 *  初始化
 */
bool LoginLayer::init() {
    if (!CCLayer::init()) {
        return false;
    }
    
    NetConnection* pNetConnection = NetConnection::getInstance();
    pNetConnection->setHandleNetDataDelegate(this);
    
    unsigned int server_id = CCUserDefault::sharedUserDefault()->getIntegerForKey(LOGIN_SERVER_KEY, 0);
    /* 更改服务器名 */
    if (0 == server_id) {
        pNetConnection->setUUU(HTTP_SERVER_URL); /* 默认新浪服务器 */
        m_pGameState->setServerId(0);
    } else {
        pNetConnection->setUUU(HTTP_SERVER_URL1); /* 研发服务器 */
        m_pGameState->setServerId(1);
    }
    /* 初始化UI组件 */
    _initComponent();
    
    return true;
}

void LoginLayer::_initComponent() {
    CCSize size = CCDirector::sharedDirector()->getVisibleSize();
    
    /* 大游戏之名字 */
    CCSprite* nameOfGame = CCSprite::create("GAME_NAME.png");
    nameOfGame->setPosition(ccp(size.width / 2, size.height - 120));
    addChild(nameOfGame, 10);
    
    /* 可移动背景 */
    srand( (unsigned)time( NULL ) );
    int x = rand() % 2;
    int y = rand() % 1500;
    
    CCSprite* pLoginBg = CCSprite::create("bg_big.png");
    pLoginBg->setAnchorPoint(CCPointZero);
    pLoginBg->setPosition(ccp(-y/*(LOGIN_BG_WIDTH - size.width) / 2*/, 0));
    this->addChild(pLoginBg);
    if (x == 0) {
        CCActionInterval * move = CCMoveTo::create(LOGIN_BG_TIME, ccp(size.width - LOGIN_BG_WIDTH, 0));
        CCActionInterval * move1 = CCMoveTo::create(LOGIN_BG_TIME, ccp(0, 0));
        CCFiniteTimeAction * seq= CCSequence::create(move, move1, NULL);
        CCActionInterval * repeatForever =CCRepeatForever::create((CCActionInterval* )seq);
        pLoginBg->runAction(repeatForever);
    } else if (x == 1) {
        CCActionInterval * move = CCMoveTo::create(LOGIN_BG_TIME, ccp(0, 0));
        CCActionInterval * move1 = CCMoveTo::create(LOGIN_BG_TIME, ccp(size.width - LOGIN_BG_WIDTH, 0));
        CCFiniteTimeAction * seq= CCSequence::create(move, move1, NULL);
        CCActionInterval * repeatForever =CCRepeatForever::create((CCActionInterval* )seq);
        pLoginBg->runAction(repeatForever);
    }
    
    /* 效果火焰 */
    CCParticleSystemQuad* particle = CCParticleSystemQuad::create("dzt3.plist");
    particle->setAnchorPoint(CCPointZero);
    particle->setPosition(ccp(320, 0));
    particle->setScaleX(0.9);
    particle->setAutoRemoveOnFinish(true);
    
    addChild(particle, 11);
    
    /* 火星 */
    CCParticleSystemQuad* particlefire = CCParticleSystemQuad::create("firefly.plist");
    particlefire->setAnchorPoint(CCPointZero);
    particlefire->setPosition(ccp(0, 800));
    particlefire->setAutoRemoveOnFinish(true);
    addChild(particlefire, 11);
    
    /* 火星1 */
    CCParticleSystemQuad* particlefire1 = CCParticleSystemQuad::create("firefly2.plist");
    particlefire1->setAnchorPoint(CCPointZero);
    particlefire1->setPosition(ccp(0, 800));
    particlefire1->setAutoRemoveOnFinish(true);
    addChild(particlefire1, 11);
    
    /* 开始游戏按钮 */
    CCMenuItemImage *pLoginBtn = CCMenuItemImage::create( "pre.png",
                                                         "def.png",
                                                         this,
                                                         menu_selector(LoginLayer::gameInit));
    pLoginBtn->setAnchorPoint(ccp(0.5, 0));
    pLoginBtn->setPosition(ccp(size.width / 2, 70));
    CCMenu* pLoginMenu = CCMenu::create(pLoginBtn, NULL);
    pLoginMenu->setAnchorPoint(CCPointZero);
    pLoginMenu->setPosition(CCPointZero);
    this->addChild(pLoginMenu);
    CCSprite* startFont = CCSprite::create("start.png");
    startFont->setAnchorPoint(ccp(0.5, 0));
    startFont->setPosition(ccp(size.width / 2, 115));
    this->addChild(startFont);
    CCActionInterval * scale = CCScaleTo::create(1, 1.2);
    CCActionInterval * scale1 = CCScaleTo::create(1.5, 1);
    CCFiniteTimeAction * seq= CCSequence::create(scale,scale1,NULL);
    CCActionInterval * repeatForever =CCRepeatForever::create((CCActionInterval* )seq);
    startFont->runAction(repeatForever);
    
    /* 服务器列表滚动视图 */
    /*
     m_scrollView = CCScrollView::create(VIEW_SIZE);
     m_scrollView->setViewSize(VIEW_SIZE);
     m_scrollView->setContentSize(CCSizeMake(62*5, VIEW_SIZE.height));
     m_scrollView->setAnchorPoint(ccp(0, 0));
     m_scrollView->setPosition(SCROLLVIEW_OFFSETX, OFFSET_1136 + SCROLLVIEW_OFFSETY);
     m_scrollView->setDirection(kCCScrollViewDirectionHorizontal);
     m_scrollView->setBounceable(true);
     m_scrollView->setDelegate(this);
     this->addChild(m_scrollView);
     */
    
    for (int i = 0; i < 5; ++i) {
        CCMenuItemImage *pSvrBtn = CCMenuItemImage::create( "LG_nor.png",
                                                           "LG_nor.png",
                                                           this,
                                                           menu_selector(LoginLayer::option));
        pSvrBtn->setTag(SVRBG_NORMAL + i);
        pSvrBtn->setAnchorPoint(ccp(0, 0));
        pSvrBtn->setPosition(ccp(SCROLLVIEW_OFFSETX + 60 * i, m_pGameState->getBottomOffset() + SCROLLVIEW_OFFSETY));
        CCMenu* pMenu = CCMenu::create(pSvrBtn, NULL);
        pMenu->setAnchorPoint(ccp(0, 0));
        pMenu->setPosition(ccp(0, 0));
        this->addChild(pMenu);
        /* 服务器名字 */
        CCLabelTTF* svrName = CCLabelTTF::create("", "Arial", 30, CCSizeMake(35, 300), kCCTextAlignmentCenter);
        svrName->setAnchorPoint(ccp(0.5, 0));
        svrName->setColor(ccc3(255, 215, 0));
        svrName->setPosition(ccp(SCROLLVIEW_OFFSETX + 37.0f + 60.0f * i,
                                 m_pGameState->getBottomOffset() + SCROLLVIEW_OFFSETY + 75.0f));
        this->addChild(svrName, 2, SVR_NAME + i);
        
        /* 选中背景状态 */
        CCSprite* pSvrBtnBg = CCSprite::create("LG_sel.png");
        pSvrBtnBg->setAnchorPoint(ccp(0, 0));
        pSvrBtnBg->setPosition(ccp(SCROLLVIEW_OFFSETX + 60.0f * i,
                                   m_pGameState->getBottomOffset() + SCROLLVIEW_OFFSETY - 10.0f));
        pSvrBtnBg->setVisible(false);
        this->addChild(pSvrBtnBg, 1, SVRBG_SELECT + i);
    }
    
    unsigned int server_id = CCUserDefault::sharedUserDefault()->getIntegerForKey(LOGIN_SERVER_KEY, 0);
    /* 更改服务器名 */
    if (0 == server_id) {
        CCLabelTTF* svrName = (CCLabelTTF*)this->getChildByTag(SVR_NAME);
        svrName->setScale(1.1);
        svrName->setString("体验服务器");
        
        CCLabelTTF* svrName1 = (CCLabelTTF*)this->getChildByTag(SVR_NAME+1);
        svrName1->setString("研发服务器");
        
        m_serverid[0] = 0;
        m_serverid[1] = 1;
        
        CCSprite* sp = (CCSprite*)this->getChildByTag(SVRBG_SELECT); /*默认第一个选中*/
        sp->setVisible(true);
        
    } else {
        
        CCLabelTTF* svrName = (CCLabelTTF*)this->getChildByTag(SVR_NAME);
        svrName->setString("体验服务器");
        
        CCLabelTTF* svrName1 = (CCLabelTTF*)this->getChildByTag(SVR_NAME + 1);
        svrName1->setString("研发服务器");
        svrName1->setScale(1.1);
        
        m_serverid[0] = 0;
        m_serverid[1] = 1;
        
        CCSprite* sp1 = (CCSprite*)this->getChildByTag(SVRBG_SELECT + 1);
        sp1->setVisible(true);
    }
    
    /* 登陆框，待改善 */
    // 取出存储的用户 id
    std::string strUserId = CCUserDefault::sharedUserDefault()->getStringForKey(USER_NAME_KEY);
    LoginBox* login = LoginBox::create();
    login->setPosition(ccp(100, 200));
    login->setContent(strUserId);
    addChild(login, 100, LOGIN_ID_TAG);
}

/*
 * 处理 web 服务器响应 get 请求回应数据
 */
void LoginLayer::handleWSResponseData(const char *szData, const unsigned int dataSize, char* szTag) {
    CCAssert(szData && dataSize > 0 && szTag, "invalid data");
    
    bool isSuccess = false;
    if (!strcmp(szTag, HTTP_REQUEST_LOGIN_TAG)) /*登录请求*/ {
        isSuccess = ProcessData::processLoginResponseFromServer(szData, dataSize);
        if (isSuccess) {
            // 登陆成功，请求玩家武将信息
            NetConnection* pNetConnection = NetConnection::getInstance();
            
            char szPostBuffer[MAX_POSTTO_SERVER_BUFFER_SIZE];
            memset(szPostBuffer, '\0', MAX_POSTTO_SERVER_BUFFER_SIZE);
            
            sprintf(szPostBuffer, "c=hero&a=hero_list&uid=%s", GamePlayer::getInstance()->getUid().c_str());
            char szTag[] = HTTP_REQUEST_GETHERO_TAG;
            pNetConnection->commitPostRequestData(szPostBuffer, szTag);
        }
    } else if (!strcmp(szTag, HTTP_REQUEST_GETHERO_TAG)) /*武将数据*/ {
        NetConnection* pNetConnection = NetConnection::getInstance();
        do {
            CC_BREAK_IF(!ProcessData::processHerosDataFromServer(szData, dataSize));
            // 还有 “未决” 请求
            CC_BREAK_IF(pNetConnection->isPending());
            
            // 武将管理
            HeroManager* pHeroManager = HeroManager::getInstance();
            // 恢复战队位置
            pHeroManager->restoreBattleTeamPosIndex();
            
            if (m_pGameState->hasEndlessFight()) /* 有未完成战斗 */ {
                Battle* pBattle = Battle::getInstance();
                
                // 恢复副本id
                m_pGameState->setCopyId(m_pGameState->getEndlessFightCopyId());
                // 恢复关卡id
                m_pGameState->setLevId(m_pGameState->getEndlessFightLevelId());
                // 恢复战斗波数
                pBattle->setRoundIndex(m_pGameState->getEndlessFightRoundIndex() - 1);
                
                // 往战场添加本方上阵武将
                unsigned int countOfGotoBattleHeros = pHeroManager->getGoIntoBattleHerosOfPlayerCount();
                for (unsigned int index = 0; index < countOfGotoBattleHeros; index++) {
                    HeroOfPlayerItem* pGotoBattleHeroItem = pHeroManager->getGoIntoBattleHeroDataByIndex(index);
                    CCAssert(pGotoBattleHeroItem, "invalid data");
                    unsigned int posIndexInBattle = m_pGameState->getUfTeamPos(pGotoBattleHeroItem->getUniId());
                    CCAssert(posIndexInBattle > 0, "invalid posIndexInBattle");
                    pGotoBattleHeroItem->setPosInBattle(posIndexInBattle);
                    pBattle->appendInBattleHero(pGotoBattleHeroItem);
                } /*for*/
                pHeroManager->saveBattleTeamPosIndex();
                
                // 提交战斗请求
                pBattle->commitPveFightRequest();
            } else {
                pNetConnection->setHandleNetDataDelegate(NULL);
                /* 设置login调用了主场景 */
                m_pGameState->setTagWhoCallMainScene(1);
                CCDirector::sharedDirector()->replaceScene(MainScene::create());
                CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_SHUTDOWN_LOADING);
            }
        } while (0);
    } else if (!strcmp(szTag, HTTP_REQUEST_FIGHT_PVE_TAG)) /*战斗请求*/{
        NetConnection* pNetConnection = NetConnection::getInstance();
        bool isSuccess = ProcessData::parseBufferFromSvr(szData, dataSize, szTag);
        if (isSuccess) {
            pNetConnection->setHandleNetDataDelegate(NULL);
            CCDirector::sharedDirector()->replaceScene(BattleScene::create());
        }
    }
}

/*
 */
void LoginLayer::gameInit() {
    NetConnection* pNetConnection = NetConnection::getInstance();
    char szPostBuffer[MAX_POSTTO_SERVER_BUFFER_SIZE];
    memset(szPostBuffer, '\0', MAX_POSTTO_SERVER_BUFFER_SIZE);
    strcpy(szPostBuffer, "c=index&a=index&uid=");
    
    LoginBox* l = (LoginBox*)this->getChildByTag(LOGIN_ID_TAG);
    if (l->getContent() == "") {
        strcat(szPostBuffer, "1");
    } else {
        strcat(szPostBuffer, l->getContent().c_str());
        
        CCUserDefault::sharedUserDefault()->setStringForKey(USER_NAME_KEY, l->getContent().c_str());
        CCUserDefault::sharedUserDefault()->flush();
    }
    
    CCUserDefault::sharedUserDefault()->setIntegerForKey(LOGIN_SERVER_KEY, m_pGameState->getServerId());
    CCUserDefault::sharedUserDefault()->flush();
    
    char szTag[] = HTTP_REQUEST_LOGIN_TAG;
    pNetConnection->commitPostRequestData(szPostBuffer, szTag);
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_LOADING_NOW);
}

void LoginLayer::option(CCObject* pSender) {
    for (int i = 0; i < 5; ++i) {
        CCSprite* spBtnBg = (CCSprite*)this->getChildByTag(SVRBG_SELECT + i);
        spBtnBg->setVisible(false);
        CCLabelTTF* svrName = (CCLabelTTF*)this->getChildByTag(SVR_NAME + i);
        svrName->setColor(ccc3(227, 168, 105));
        svrName->setScale(1);
    }
    CCMenuItemImage* pItemImage = (CCMenuItemImage*)pSender;
    
    switch (pItemImage->getTag() - SVRBG_NORMAL) {
        case 0:
        {
            NetConnection* pNetConnection = NetConnection::getInstance();
            
            pNetConnection->setUUU(HTTP_SERVER_URL);
            CCUserDefault::sharedUserDefault()->setIntegerForKey(LOGIN_SERVER_KEY, 0);
            CCUserDefault::sharedUserDefault()->flush();
            
            m_pGameState->setServerId(0);
            
            CCSprite* spBtnBg =  (CCSprite*)this->getChildByTag(SVRBG_SELECT);
            spBtnBg->setVisible(true);
            CCLabelTTF* svrName = (CCLabelTTF*)this->getChildByTag(SVR_NAME);
            svrName->setColor(ccc3(255, 215, 0));
            svrName->setScale(1.1);
        }
            break;
        case 1:
        {
            NetConnection* pNetConnection = NetConnection::getInstance();
            pNetConnection->setUUU(HTTP_SERVER_URL1);
            CCUserDefault::sharedUserDefault()->setIntegerForKey(LOGIN_SERVER_KEY, 1);
            CCUserDefault::sharedUserDefault()->flush();
            
            m_pGameState->setServerId(1);
            
            CCSprite* spBtnBg =  (CCSprite*)this->getChildByTag(SVRBG_SELECT + 1);
            spBtnBg->setVisible(true);
            CCLabelTTF* svrName = (CCLabelTTF*)this->getChildByTag(SVR_NAME + 1);
            svrName->setColor(ccc3(255, 215, 0));
            svrName->setScale(1.1);
        }
            break;
        case 2:
        {
            CCSprite* spBtnBg =  (CCSprite*)this->getChildByTag(SVRBG_SELECT+2);
            spBtnBg->setVisible(true);
            CCLabelTTF* svrName = (CCLabelTTF*)this->getChildByTag(SVR_NAME+2);
            svrName->setColor(ccc3(255, 215, 0));
            svrName->setScale(1.1);
        }
            break;
        case 3:
        {
            CCSprite* spBtnBg =  (CCSprite*)this->getChildByTag(SVRBG_SELECT+3);
            spBtnBg->setVisible(true);
            CCLabelTTF* svrName = (CCLabelTTF*)this->getChildByTag(SVR_NAME+2);
            svrName->setColor(ccc3(255, 215, 0));
            svrName->setScale(1.1);
        }
            break;
        case 4:
        {
            CCSprite* spBtnBg =  (CCSprite*)this->getChildByTag(SVRBG_SELECT+4);
            spBtnBg->setVisible(true);
            CCLabelTTF* svrName = (CCLabelTTF*)this->getChildByTag(SVR_NAME+2);
            svrName->setColor(ccc3(255, 215, 0));
            svrName->setScale(1.1);
        }
            break;
        default:
            CCAssert(false, "invalid server");
            break;
    }
}

/***************************************
 页面滚动效果, 这是整个层的滚动效果
 ***************************************/
void LoginLayer::scrollViewDidScroll(CCScrollView* view) {
    m_isMoving = true;
}

void LoginLayer::scrollViewDidZoom(CCScrollView* view) {
    
}

bool LoginLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    //    float x = pTouch->getLocation().x;
    //    float y = pTouch->getLocation().y;
    //    /* 如果内容小于640则不滑动 */
    //    if (y < OFFSET_1136 + SCROLLVIEW_OFFSETY || y > 660) {
    //        CCLog("不在点击区");
    //        return false;
    //    }
    //    struct timeval tv;
    //    gettimeofday(&tv,NULL);
    //    long nowTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    //    m_lastTime = (float)(nowTime)/1000;
    //    m_scrollView->stopAllActions();
    //    m_lastPos = ccp(x, y);
    //    m_xOffset = m_scrollView->getContentOffset().x;
    //    m_tempPos = x;
    return true;
}

void LoginLayer::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    //    m_isMoving = true;
    //    float x = pTouch->getLocation().x;
    //    m_xOffset += (x - m_tempPos);
    ////    if (m_xOffset > 0) {
    ////        m_xOffset = 0;
    ////    }
    ////    if (m_xOffset < -340) {
    ////        m_xOffset = -340;
    ////    }
    //    m_scrollView->setContentOffset((ccp(m_xOffset, m_scrollView->getContentOffset().y)));
    //    m_tempPos = x;
}

void LoginLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    //    _inertiaScroll(pTouch, pEvent);
    //    m_tempPos = 0;
    //    m_lastTime = 0;
    //    m_lastPos = ccp(0, 0);
}

void LoginLayer::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    //    _autoScroll();
    //    m_lastTime = 0;
}

void LoginLayer::_inertiaScroll(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    //    float x = pTouch->getLocation().x;
    //    int value = 0.0f;
    //    struct timeval tv;
    //    gettimeofday(&tv,NULL);
    //    long tt = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    //    float nowTime = (float)(tt / 1000);
    //    if (fabs(nowTime - m_lastTime) < 0.8 && fabs(nowTime - m_lastTime) > 0.1) {
    //        value = (int)(((x - m_lastPos.x)/fabs(nowTime - m_lastTime)) / 5);
    //    }
    //    m_xOffset += value;
    //    if (m_xOffset > 0) {
    //        m_xOffset = 0;
    //
    //    }else if (m_xOffset < -340) {
    //        m_xOffset = -340;
    //    }
    //    CCArray* array = CCArray::create();
    //    array->addObject(CCEaseExponentialOut::create(CCMoveTo::create(0.3f, CCPointMake(m_xOffset, m_scrollView->getContentOffset().y))));
    //    array->addObject(CCCallFunc::create(this, callfunc_selector(LoginLayer::_autoScroll)));
    //    m_scrollView->getContainer()->runAction(CCSequence::create(array));
}

void LoginLayer::_autoScroll() {
    //    m_isMoving = false;
    //    if (m_xOffset > 0) {
    //        m_scrollView->setContentOffset(ccp(0, m_scrollView->getContentOffset().y));
    //    } else if (m_xOffset < -310) {
    //        m_scrollView->setContentOffset(ccp(-310, m_scrollView->getContentOffset().y));
    //    }
}

void LoginLayer::onEnter() {
    CCLayer::onEnter();
    //    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, false);
}

void LoginLayer::onExit() {
    //    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

