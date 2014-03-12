//
//  BgOfMainSceneLayer.cpp
//  hero
//
//  Created by yangjie on 2013/11/29.
//
//

#include "BgOfMainSceneLayer.h"
#include "GamePlayer.h"
#include "GameState.h"

// 玩家信息大图文件
#define PLAYER_INFO_IMAGE_FILE      "playerInfo.pvr.ccz"
#define PLAYER_INFO_LIST_FILE       "playerInfo.plist"

#define BACKGROUND_IMAGE            "M_1.png"

#define INFO_IMAGE_OFFSET           795.0f

#define BACKGROUND_TAG              1000
#define PLAYER_NAME_TAG             1001
#define PLAYER_LEV_TAG              1002
#define PLAYER_EXP_TAG              1003
#define PLAYER_GOLD_TAG             1004
#define PLAYER_COPPER_TAG           1005
#define PLAYER_HP_TAG               1006
#define PLAYER_TIME_TAG             1007
#define PLAYER_DANG_TAG             1009
#define PLAYER_MAX_TAG              1010
#define PLAYER_ENERGY_TAG           1011
#define PLAYER_WEI_TAG              1100
#define PLAYER_SHU_TAG              1101
#define PLAYER_WU_TAG               1102

#define FONT                        "Arial"
#define FONT_SZ                     25

#define COLOR_GOLD                  ccc3(255, 255, 0)
#define COLOR_GREEN                 ccc3(127, 255, 0)
#define COLOR_BLUE                  ccc3(135, 206, 235)

/*
 *  构造函数
 */
BgOfMainSceneLayer::BgOfMainSceneLayer():m_pGamePlayer(GamePlayer::getInstance())
{
    cur_power = m_pGamePlayer->getEnergy();
}

/*
 *  析构函数
 */
BgOfMainSceneLayer::~BgOfMainSceneLayer() {
    CCTextureCache* pTextureCache = CCTextureCache::sharedTextureCache();
    pTextureCache->removeTextureForKey("M_B_name.png");
    pTextureCache->removeTextureForKey("M_B_lev.png");
    pTextureCache->removeTextureForKey("M_L_lv.png");
    pTextureCache->removeTextureForKey("M_B_exp.png");
    pTextureCache->removeTextureForKey("M_L_exp.png");
    pTextureCache->removeTextureForKey("M_exp.png");
    pTextureCache->removeTextureForKey("M_B_gem.png");
    pTextureCache->removeTextureForKey("M_gem.png");
    pTextureCache->removeTextureForKey("M_cop.png");
    pTextureCache->removeTextureForKey("M_B_pow.png");
    pTextureCache->removeTextureForKey("M_L_pow.png");
    pTextureCache->removeTextureForKey("M_hp.png");
}

/*
 *  初始化
 */
bool BgOfMainSceneLayer::init() {
    // super init
    if (!CCLayer::init()) {
        return false;
    }
    
    // 初始化UI组件
    _initUI();
    // 更新需要改变的东西
    _initUpdate();
    // 实时刷新玩家显示
    schedule(schedule_selector(BgOfMainSceneLayer::_updatePlayerInfo), 0.5f);
    
    return true;
}

void BgOfMainSceneLayer::_updatePlayerInfo() {
    /*玩家金锭*/
    CCLabelTTF* play_gold = (CCLabelTTF*)this->getChildByTag(PLAYER_GOLD_TAG);
    int gold = m_pGamePlayer->getGold();
    char goldStr[16];
    if (gold > 99999) {
        gold = (int)(gold / 10000);
        sprintf(goldStr, "%d万", gold);
    }else {
        gold = gold;
        sprintf(goldStr, "%d", gold);
    }
    play_gold->setString(goldStr);
    
    /*玩家铜币*/
    CCLabelTTF* play_copper = (CCLabelTTF*)this->getChildByTag(PLAYER_COPPER_TAG);
    int copper = m_pGamePlayer->getMoney();
    char copperStr[16];
    if (copper >99999){
        copper = (int)(copper/10000);
        sprintf(copperStr, "%d万", copper);
    }else {
        sprintf(copperStr, "%d", copper);
    }
    play_copper->setString(copperStr);
    
    // 玩家等级
    CCLabelTTF* play_lev = (CCLabelTTF*)this->getChildByTag(PLAYER_LEV_TAG);
    int lev = m_pGamePlayer->getLevel();
    char strLev[16];
    sprintf(strLev, "%d", lev);
    play_lev->setString(strLev);
    
    // 玩家经验
    CCProgressTimer *play_exp = (CCProgressTimer *)this->getChildByTag(PLAYER_EXP_TAG);
    // 经验条的变化
    int gamePlayerExp = m_pGamePlayer->getExp();
    int gamePlayerLev = m_pGamePlayer->getLevel();
    float exp_progress = 0.0f;
    exp_progress = float(gamePlayerExp) / float(m_pGamePlayer->getExpByLev(gamePlayerLev));
    play_exp->setPercentage(exp_progress * 100);
}

/* 初始化刷新 */
void BgOfMainSceneLayer::_initUpdate(){
    CCLabelTTF* play_name = (CCLabelTTF*)this->getChildByTag(PLAYER_NAME_TAG);
    std::string strName = m_pGamePlayer->getName();
    play_name->setString(strName.c_str());
    
    //改变玩家等级
    CCLabelTTF* play_lev = (CCLabelTTF*)this->getChildByTag(PLAYER_LEV_TAG);
    int lev = m_pGamePlayer->getLevel();
    char strLev[16];
    sprintf(strLev, "%d", lev);
    play_lev->setString(strLev);
    
    //玩家金锭
    CCLabelTTF* play_gold = (CCLabelTTF*)this->getChildByTag(PLAYER_GOLD_TAG);
    int gold = m_pGamePlayer->getGold();
    char goldStr[16];
    if (gold > 99999) {
        gold = (int)(gold / 10000);
        sprintf(goldStr, "%d万", gold);
    }else {
        gold = gold;
        sprintf(goldStr, "%d", gold);
    }
    play_gold->setString(goldStr);
    
    // 玩家铜币
    CCLabelTTF* play_copper = (CCLabelTTF*)this->getChildByTag(PLAYER_COPPER_TAG);
    int copper = m_pGamePlayer->getMoney();
    char copperStr[16];
    if (copper > 99999){
        copper = (int)(copper / 10000);
        sprintf(copperStr, "%d万", copper);
    } else {
        sprintf(copperStr, "%d", copper);
    }
    play_copper->setString(copperStr);
    
    //玩家经验**************
    CCProgressTimer *play_exp = (CCProgressTimer *)this->getChildByTag(PLAYER_EXP_TAG);
    play_exp->setPercentage(70.0);
    //***********************************************************************************
    //经验条的变化
    int gamePlayerExp = m_pGamePlayer->getExp();
    int gamePlayerLev = m_pGamePlayer->getLevel();
    float exp_progress = 0.0f;
    exp_progress = float(gamePlayerExp) / float(m_pGamePlayer->getExpByLev(gamePlayerLev));
    play_exp->setPercentage(exp_progress * 100);
    
    _update();
    schedule(schedule_selector(BgOfMainSceneLayer::_update), 1.0f);
}

/*
 * 实时刷新,体力
 */
void BgOfMainSceneLayer::_update() {
    char buffer[64];
    //*****从服务器取得时间 static 程序运行
    static int time = m_pGamePlayer->getEnergyTime();
    int max_power = m_pGamePlayer->getMaxEnergy();
    
    CCLabelTTF* pEn = (CCLabelTTF*)this->getChildByTag(PLAYER_ENERGY_TAG);
    CCLabelTTF* pTime = (CCLabelTTF*)this->getChildByTag(PLAYER_TIME_TAG);
    CCProgressTimer* player_hp = (CCProgressTimer*)this->getChildByTag(PLAYER_HP_TAG);
    
    if (cur_power >= max_power) {
        sprintf(buffer, "%d/%d",cur_power, max_power);
        pEn->setString(buffer);
        pTime->setVisible(false);
        player_hp->setPercentage(((float)cur_power / (float)max_power) * 100.0);
        unschedule(schedule_selector(BgOfMainSceneLayer::_update));
        return;
    }
    
    int min = 0;
    int sec = 0;
    min = (int)(time/60);
    sec = time%60;
    
    sprintf(buffer, "剩余时间: %02d:%02d",min, sec);
    pTime->setString(buffer);
    sprintf(buffer, "%d/%d", cur_power, max_power);
    pEn->setString(buffer);
    
    // 严格为浮点数，不然会转整型时变成0
    player_hp->setPercentage(((float)cur_power / (float)max_power) * 100.0);
    // 处理时间
    time--;
    if (time <= 0 && cur_power != max_power) {
        time = m_pGamePlayer->getEnergyCycle();
        cur_power++;
    }
}

/*
 * 初始化UI组件
 */
void BgOfMainSceneLayer::_initUI() {
    CCSpriteBatchNode* pCBN = CCSpriteBatchNode::create(PLAYER_INFO_IMAGE_FILE);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(PLAYER_INFO_LIST_FILE, PLAYER_INFO_IMAGE_FILE);
    this->addChild(pCBN, 1);
    
    // 背景图
    CCSprite *pInfoSprite = CCSprite::createWithSpriteFrameName(BACKGROUND_IMAGE);
    pInfoSprite->setAnchorPoint(CCPointZero);
    pInfoSprite->setPosition(ccp(-6.0f, GameState::getInstance()->getBottomOffset() + INFO_IMAGE_OFFSET));
    pCBN->addChild(pInfoSprite, 1, BACKGROUND_TAG);
    CCPoint infoPoint = pInfoSprite->getPosition();
    
    // 姓名框
    CCSprite* pNameBox = CCSprite::createWithSpriteFrameName("M_B_name.png");
    pNameBox->setAnchorPoint(ccp(0.0f, 0.5f));
    pNameBox->setPosition(ccp(40.0f, 120.0f));
    pInfoSprite->addChild(pNameBox, 2);
    
    /* 玩家所属阵营 */
    CCSprite* campWu = CCSprite::create("M_campWu.png");
    campWu->setAnchorPoint(ccp(0.0f, 0.5f));
    campWu->setPosition(ccp(infoPoint.x + 22.0f, infoPoint.y + 120.0f));
    campWu->setScale(0.6);
    this->addChild(campWu, 2, PLAYER_WU_TAG);
    CCSprite* campShu = CCSprite::create("M_campShu.png");
    campShu->setAnchorPoint(ccp(0.0f, 0.5f));
    campShu->setPosition(ccp(infoPoint.x + 22.0f, infoPoint.y + 120.0f));
    campShu->setScale(0.6);
    this->addChild(campShu, 2, PLAYER_SHU_TAG);
    CCSprite* campWei = CCSprite::create("M_campWei.png");
    campWei->setAnchorPoint(ccp(0.0f, 0.5f));
    campWei->setPosition(ccp(infoPoint.x + 22.0f, infoPoint.y + 120.0f));
    campWei->setScale(0.6);
    this->addChild(campWei, 2, PLAYER_WEI_TAG);
    confirmCamp();/*确定玩家当前阵营*/
    
    //姓名**************************************************************
    CCLabelTTF* labelName = CCLabelTTF::create("我是柚子先生三号", FONT, FONT_SZ);
    labelName->setAnchorPoint(ccp(0, 0.5));
    labelName->setPosition(ccp(infoPoint.x + 70.0f, infoPoint.y + 120.0f));
    this->addChild(labelName, 2, PLAYER_NAME_TAG);
    
    // 等级框
    CCSprite* pLevBox = CCSprite::createWithSpriteFrameName("M_B_lev.png");
    pLevBox->setAnchorPoint(ccp(0.0f, 0.5f));
    pLevBox->setPosition(ccp(310.0f, 120.0f));
    pInfoSprite->addChild(pLevBox);
    
    CCSprite* pLabelLev = CCSprite::createWithSpriteFrameName("M_L_lv.png");
    pLabelLev->setAnchorPoint(ccp(0, 0.5));
    pLabelLev->setPosition(ccp(320.0f, 120.0f));
    pInfoSprite->addChild(pLabelLev);
    //等级**************************************************************
    CCLabelTTF* labelLev = CCLabelTTF::create("", FONT, FONT_SZ);
    labelLev->setColor(COLOR_BLUE);
    labelLev->setAnchorPoint(ccp(0, 0.5));
    labelLev->setPosition(ccp(infoPoint.x + 365.0f, infoPoint.y + 120.0f));
    this->addChild(labelLev, 2, PLAYER_LEV_TAG);
    
    //经验框
    CCSprite* pExpBox = CCSprite::createWithSpriteFrameName("M_B_exp.png");
    pExpBox->setAnchorPoint(ccp(0, 0.5));
    pExpBox->setPosition(ccp(480.0f, 120.0f));
    pInfoSprite->addChild(pExpBox);
    CCSprite* pLabelExp = CCSprite::createWithSpriteFrameName("M_L_exp.png");
    pLabelExp->setAnchorPoint(ccp(0.0f, 0.5f));
    pLabelExp->setPosition(ccp(438.0f, 120.0f));
    pInfoSprite->addChild(pLabelExp);
    // 经验条图片
    CCProgressTimer* expProgeress = CCProgressTimer::create(CCSprite::createWithSpriteFrameName("M_exp.png"));
    expProgeress->setType(kCCProgressTimerTypeBar);
    expProgeress->setMidpoint(ccp(0,0));
    expProgeress->setBarChangeRate(ccp(1, 0));
    expProgeress->setAnchorPoint(ccp(0, 0.5));
    expProgeress->setPosition(ccp(infoPoint.x + 480.0f, infoPoint.y + 120.0f));
    this->addChild(expProgeress, 2, PLAYER_EXP_TAG);
    
    // 金锭框
    CCSprite *pGemBox = CCSprite::createWithSpriteFrameName("M_B_gem.png");
    pGemBox->setAnchorPoint(CCPointZero);
    pGemBox->setPosition(ccp(40.0f, 55.0f));
    pInfoSprite->addChild(pGemBox);
    
    CCSprite* pLabelGem = CCSprite::createWithSpriteFrameName("M_gem.png");
    pLabelGem->setAnchorPoint(ccp(0, 0));
    pLabelGem->setPosition(ccp(50.0f, 55.0f));
    pInfoSprite->addChild(pLabelGem);
    //金锭*************************************************************
    CCLabelTTF* labelGold = CCLabelTTF::create("", FONT, FONT_SZ);
    labelGold->setColor(COLOR_GOLD);
    labelGold->setAnchorPoint(ccp(0, 0.5));
    labelGold->setPosition(ccp(infoPoint.x + 95.0f, infoPoint.y + 75.0f));
    this->addChild(labelGold, 2, PLAYER_GOLD_TAG);
    
    //铜币框
    CCSprite* pCopperBox = CCSprite::createWithSpriteFrameName("M_B_gem.png");
    pCopperBox->setAnchorPoint(ccp(0, 0));
    pCopperBox->setPosition(ccp(190.0f, 55.0f));
    pInfoSprite->addChild(pCopperBox);
    CCSprite* pLabelCopper = CCSprite::createWithSpriteFrameName("M_cop.png");
    pLabelCopper->setAnchorPoint(ccp(0, 0));
    pLabelCopper->setPosition(ccp(200.0f, 55.0f));
    pInfoSprite->addChild(pLabelCopper);
    //铜币*************************************************************
    CCLabelTTF* labelCop = CCLabelTTF::create("", FONT, FONT_SZ);
    labelCop->setColor(COLOR_GREEN);
    labelCop->setAnchorPoint(ccp(0.0f, 0.5f));
    labelCop->setPosition(ccp(infoPoint.x + 240.0f, infoPoint.y + 75.0f));
    this->addChild(labelCop, 2, PLAYER_COPPER_TAG);
    
    // 体力显示
    CCSprite* pPower = CCSprite::createWithSpriteFrameName("M_B_pow.png");
    pPower->setAnchorPoint(CCPointZero);
    pPower->setPosition(ccp(390.0f, 72.0f));
    pInfoSprite->addChild(pPower);
    CCSprite* pLabelPower = CCSprite::createWithSpriteFrameName("M_L_pow.png");
    pLabelPower->setAnchorPoint(CCPointZero);
    pLabelPower->setScale(1.4);
    pLabelPower->setPosition(ccp(346.0f, 50.0f));
    pInfoSprite->addChild(pLabelPower);
    
    CCProgressTimer* hpProgeress = CCProgressTimer::create(CCSprite::createWithSpriteFrameName("M_hp.png"));
    hpProgeress->setType(kCCProgressTimerTypeBar);
    hpProgeress->setMidpoint(ccp(0, 0));
    hpProgeress->setBarChangeRate(ccp(1, 0));
    hpProgeress->setPercentage(100.0);
    hpProgeress->setAnchorPoint(CCPointZero);
    hpProgeress->setPosition(ccp(infoPoint.x + 390.0f, infoPoint.y + 72.0f));
    this->addChild(hpProgeress, 2, PLAYER_HP_TAG);
    
    // 显示时间
    CCLabelTTF* font_time = CCLabelTTF::create("", FONT, FONT_SZ - 3);
    font_time->setAnchorPoint(CCPointZero);
    font_time->setPosition(ccp(infoPoint.x + 380.0f, infoPoint.y + 44.0f));
    this->addChild(font_time, 2, PLAYER_TIME_TAG);
    // 显示体力
    CCLabelTTF* font_en = CCLabelTTF::create("", FONT, FONT_SZ - 3);
    font_en->setAnchorPoint(CCPointZero);
    font_en->setPosition(ccp(infoPoint.x + 560.0f, infoPoint.y + 44.0f));
    this->addChild(font_en, 2, PLAYER_ENERGY_TAG);
}

/* 购买体力更新 */
void BgOfMainSceneLayer::updateEnergy() {
    cur_power = m_pGamePlayer->getEnergy();
}

/*  */
void BgOfMainSceneLayer::updateGold() {
    m_pGamePlayer->setGold(m_pGamePlayer->getGold() - m_pGamePlayer->getEnergyPrice());
}

void BgOfMainSceneLayer::confirmCamp() {
    CCSprite* wei = (CCSprite*)this->getChildByTag(PLAYER_WEI_TAG);
    CCSprite* shu = (CCSprite*)this->getChildByTag(PLAYER_SHU_TAG);
    CCSprite* wu  = (CCSprite*)this->getChildByTag(PLAYER_WU_TAG);
    playerOfCamp camp = m_pGamePlayer->getPlayerOfCamp();
    if (PLAYERCAMP_WEI == camp) {
        wei->setVisible(true);
        shu->setVisible(false);
        wu->setVisible(false);
    } else if (PLAYERCAMP_SHU == camp) {
        wei->setVisible(false);
        shu->setVisible(true);
        wu->setVisible(false);
    } else if (PLAYERCAMP_WU == camp) {
        wei->setVisible(false);
        shu->setVisible(false);
        wu->setVisible(true);
    } else {
        wei->setVisible(false);
        shu->setVisible(false);
        wu->setVisible(false);
    }
}


//
void BgOfMainSceneLayer::restoreTouchUICompent() {}
void BgOfMainSceneLayer::moveOutTouchUICompent() {}
