
#include "HeroIntroduce.h"
#include "HeroManager.h"
#include "HeroItem.h"
#include "HeroOfPlayerItem.h"
#include "GameItemManager.h"
#include "customMessage.h"
#include "GameState.h"
#include "SkillManager.h"
#include "SkillItem.h"
#include "CCStrokeLabel.h"  
#include "NetConnection.h"
#include "GamePlayer.h"
#include "HerosLayer.h"
#include "JianyuLayer.h"

#define HEROS_LAYER_TAG                     112     // “武将列表” 层 tag
#define JIANYU_LAYER                        200     // 监狱层 Tag
//主菜单大图文件
#define HERO_BRO_IMAGE_FILE                 "heroBro.pvr.ccz"
#define HERO_BRO_LIST_FILE                  "heroBro.plist"
#define HERO_BRO_IMAGE_TAG                  10

//边框及背景图、龙纹、锁链
#define IMAGE_BOARD                         "DE_board.png"
#define IMAGE_LOCK                          "DE_lock.png"
//星级颜色图片
#define IMAGE_STAR_WHITE                    "DE_whitestar.png"
#define IMAGE_STAR_GREEN                    "DE_greenstar.png"
#define IMAGE_STAR_BLUE                     "DE_bluestar.png"
#define IMAGE_STAR_PURPLE                   "DE_purplestar.png"
#define IMAGE_STAR_ORANGE                   "DE_yellowstar.png"
//武将品级图片
#define IMAGE_WHITE                         "DE_white.png"
#define IMAGE_GREEN                         "DE_green.png"
#define IMAGE_BLUE                          "DE_blue.png"
#define IMAGE_PURPLE                        "DE_purple.png"
#define IMAGE_ORANGE                        "DE_orange.png"
//解锁武将、确定、及按钮图片
#define IMAGE_LOCKED                        "DE_locked.png"
#define IMAGE_UNLOCK                        "DE_unlock.png"
#define IMAGE_OK                            "DE_ok.png"
#define IMAGE_BTN                           "DE_BTNbox.png"
#define IMAGE_BTN_S                         "DE_BTNbox_s.png"
//分割线
#define IMAGE_LINE                          "DE_fengge.png"


//龙纹下边距
#define DRAGON_PADDING_DOWN                 468.0f
//武将品级
#define HERO_GRADE_PADDING_DOWN             683.0f
#define HERO_GRADE_PADDING_RIGHT            508.0f
//星星间隔
#define STAR_SPACE                          5.0f
#define STAR_WIDTH                          38.0f
#define STAR_PADDING_LEFT                   358.0f
#define STAR_PADDING_DOWN                   900.0f
//按钮下边距
#define BTN_PADDING_DOWN                    115.0f

#define TEXT_CONTAINER_TAG                  1000
#define BG_TAG                              1001

#define CONTAINER_TAG                       1500

#define LOCKED_TAG                          100
#define UNLOCK_TAG                          101
#define LOCK_TAG                            102     /*锁链*/
#define ON_MESSAGE_UNLOCK_HERO_STRING       "unLockHero"
#define ON_MESSAGE_LOCK_HERO_STRING         "lockHero"

#define TEXT_COLOR_WHITE                  (ccc3(255,255,255))

#define OFFSET_V                            (m_GameState->is1136() ? 0.0f : -90.0f)
/*
 *      构造函数
 */
HeroIntroduce::HeroIntroduce():
m_pSBN(NULL),
m_enableTouch(true),
m_pHeroManager(HeroManager::getInstance()),
m_GameState(GameState::getInstance())
{
    memset(m_menuButtonSprite, '\0', sizeof(m_menuButtonSprite));
    
    //解锁按钮
    m_menuButtonPosition[0][0] = ccp(46.0f,BTN_PADDING_DOWN+OFFSET_V);
    m_menuButtonPosition[0][1] = ccp(46.0f,BTN_PADDING_DOWN+OFFSET_V);
    m_menuButtonPosition[0][2] = ccp(46.0f,BTN_PADDING_DOWN+OFFSET_V);
 
    //确定按钮
    unsigned int tag = m_GameState->getTagWhoCallHeroSel();
    WhoCallHeroIntro tagLayer = m_GameState->getTagWhoCallHeroIntro();
    if ((tagLayer == HEROLIST_LAYER && tag != WCH_BROWSE)|| (tagLayer == JY_LAYER) ) {
       // CCSize s = CCDirector::sharedDirector()->getVisibleSize();
        float width = 202.0f;
        m_menuButtonPosition[1][0] = ccp(width,BTN_PADDING_DOWN+OFFSET_V);
        m_menuButtonPosition[1][1] = ccp(width,BTN_PADDING_DOWN+OFFSET_V);
        m_menuButtonPosition[1][2] = ccp(width,BTN_PADDING_DOWN+OFFSET_V);
        
    }else if(tagLayer == HEROLIST_LAYER && tag == WCH_BROWSE){
        m_menuButtonPosition[1][0] = ccp(358.0f,BTN_PADDING_DOWN+OFFSET_V);
        m_menuButtonPosition[1][1] = ccp(358.0f,BTN_PADDING_DOWN+OFFSET_V);
        m_menuButtonPosition[1][2] = ccp(358.0f,BTN_PADDING_DOWN+OFFSET_V);
        
    }
    
    m_uniId = m_GameState->getHeroIntro();
    m_id = m_GameState->getHeroIntroId();

    //获取武将锁定状态
    if (m_GameState->getTagWhoCallHeroIntro() == HEROLIST_LAYER) {
         m_isLocked = m_pHeroManager->getHeroDataByUniId(m_uniId)->getLock();
    }
}

/*
 *      析构函数
 */
HeroIntroduce::~HeroIntroduce()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(HERO_BRO_LIST_FILE);
    CCTextureCache::sharedTextureCache()->removeTextureForKey(HERO_BRO_IMAGE_FILE);
}

/*
 *      初始化
 */
bool HeroIntroduce::init()
{
    //////////////////////////////
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    //允许接受touch事件
    this->setTouchEnabled(true);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(HERO_BRO_LIST_FILE, HERO_BRO_IMAGE_FILE);
    this->addChild(m_pSBN = CCSpriteBatchNode::create(HERO_BRO_IMAGE_FILE));
    
    CCSize size = CCDirector::sharedDirector()->getVisibleSize();
    
    //添加背景图片
    /*上背景*/
    CCSprite *pTopSprite =CCSprite::createWithSpriteFrameName("DE_bkboardup.png");
    pTopSprite->setAnchorPoint(CCPointZero);
    pTopSprite->setPosition(ccp(0, size.height - pTopSprite->getContentSize().height));
    m_pSBN->addChild(pTopSprite, 1,BG_TAG);
    /*下背景*/
    CCSprite *pBottomSprite =CCSprite::createWithSpriteFrameName("DE_bkboarddown.png");
    pBottomSprite->setAnchorPoint(CCPointZero);
    pBottomSprite->setPosition(ccp(0,0+OFFSET_V));
    m_pSBN->addChild(pBottomSprite, 2);
    /*龙纹背景*/
    CCSprite *pDragonSprite =CCSprite::createWithSpriteFrameName("DE_dragon.png");
    pDragonSprite->setPosition(ccp(size.width/2, DRAGON_PADDING_DOWN + OFFSET_V+pDragonSprite->getContentSize().height / 2.0f));
    m_pSBN->addChild(pDragonSprite, 3);
    /*外边框背景*/
    CCSprite *pBoardSprite = CCSprite::createWithSpriteFrameName(IMAGE_BOARD);
    pBoardSprite->setAnchorPoint(CCPointZero);
    pBoardSprite->setPosition(ccp(0,0+OFFSET_V));
    m_pSBN->addChild(pBoardSprite,4,CONTAINER_TAG);
    /*添加锁链*/
    CCSprite *pLockSprite = CCSprite::createWithSpriteFrameName(IMAGE_LOCK);
    pLockSprite->setAnchorPoint(CCPointZero);
    pLockSprite->setPosition(ccp(438.0f,932.0f+OFFSET_V));
    m_pSBN->addChild(pLockSprite,3,LOCK_TAG);
    pLockSprite->setVisible(m_isLocked);

    //武将品级
    memset(m_heroGrade, '\0', sizeof(m_heroGrade));
    m_heroGrade[0] = CCSprite::createWithSpriteFrameName(IMAGE_WHITE);
    m_heroGrade[1] = CCSprite::createWithSpriteFrameName(IMAGE_GREEN);
    m_heroGrade[2] = CCSprite::createWithSpriteFrameName(IMAGE_BLUE);
    m_heroGrade[3] = CCSprite::createWithSpriteFrameName(IMAGE_PURPLE);
    m_heroGrade[4] = CCSprite::createWithSpriteFrameName(IMAGE_ORANGE);
    //武将星级
    memset(m_heroStar, '\0', sizeof(m_heroStar));
    for (unsigned int i=0; i < 5 ; i++) {
            m_heroStar[0][i] =  CCSprite::createWithSpriteFrameName(IMAGE_STAR_WHITE);
            m_heroStar[1][i] =  CCSprite::createWithSpriteFrameName(IMAGE_STAR_GREEN);
            m_heroStar[2][i] =  CCSprite::createWithSpriteFrameName(IMAGE_STAR_BLUE);
            m_heroStar[3][i] =  CCSprite::createWithSpriteFrameName(IMAGE_STAR_PURPLE);
            m_heroStar[4][i] =  CCSprite::createWithSpriteFrameName(IMAGE_STAR_ORANGE);
    }
    
    //添加武将品级图片
    for (unsigned int i=0; i < HERO_GRADE; i++) {
        m_heroGrade[i]->setAnchorPoint(CCPointZero);
        m_heroGrade[i]->setPosition(ccp(size.width-HERO_GRADE_PADDING_RIGHT,HERO_GRADE_PADDING_DOWN+OFFSET_V));
        m_pSBN->addChild(m_heroGrade[i],1,200+i);
        m_heroGrade[i]->setVisible(false);
    }
    //添加星级图片
    for (unsigned int i=0; i < HERO_GRADE ; i++) {
        for (unsigned int j =0; j<5; j++) {
            m_heroStar[i][j]->setAnchorPoint(CCPointZero);
            m_heroStar[i][j]->setPosition(ccp(STAR_PADDING_LEFT+STAR_WIDTH*j+STAR_SPACE*j,STAR_PADDING_DOWN+OFFSET_V));
            m_pSBN->addChild(m_heroStar[i][j],2,300+i*5+j);
            m_heroStar[i][j]->setVisible(false);
        }
    }
    starShow();
    
    //添加 等级、hp、攻击力、及其小背景
    /*等级*/
    CCSprite* pLevelSprite = CCSprite::createWithSpriteFrameName("DE_lv.png");
    pLevelSprite->setAnchorPoint(CCPointZero);
    pLevelSprite->setPosition(ccp(STAR_PADDING_LEFT+7.0f,860.0f+OFFSET_V));
    m_pSBN->addChild(pLevelSprite,3);
    /*小背景*/
    CCSprite *pHpBkSmallSprite = CCSprite::createWithSpriteFrameName("DE_smallbk.png");
    pHpBkSmallSprite->setAnchorPoint(CCPointZero);
    pHpBkSmallSprite->setPosition(ccp(STAR_PADDING_LEFT,807.0f+OFFSET_V));
    m_pSBN->addChild(pHpBkSmallSprite,2);
    CCSprite *pAttBkSmallSprite = CCSprite::createWithSpriteFrameName("DE_smallbk.png");
    pAttBkSmallSprite->setAnchorPoint(CCPointZero);
    pAttBkSmallSprite->setPosition(ccp(STAR_PADDING_LEFT,755.0f+OFFSET_V));
    m_pSBN->addChild(pAttBkSmallSprite,2);
    /*hp*/
    CCSprite *pHpSprite = CCSprite::createWithSpriteFrameName("DE_hp.png");
    pHpSprite->setAnchorPoint(CCPointZero);
    pHpSprite->setPosition(ccp(STAR_PADDING_LEFT+7.0f,809.0f+OFFSET_V));
    m_pSBN->addChild(pHpSprite,3);
    /*攻击力*/
    CCSprite *pAttSprite = CCSprite::createWithSpriteFrameName("DE_ATT.png");
    pAttSprite->setAnchorPoint(CCPointZero);
    pAttSprite->setPosition(ccp(STAR_PADDING_LEFT+7.0f,757.0f+OFFSET_V));
    m_pSBN->addChild(pAttSprite,3);
    //添加中部的上中下边框栏
    /*上边框*/
    CCSprite *pUpBoardSprite = CCSprite::createWithSpriteFrameName("DE_topboard.png");
    pUpBoardSprite->setAnchorPoint(CCPointZero);
    pUpBoardSprite->setPosition(ccp(0,692.0f+OFFSET_V));
    m_pSBN->addChild(pUpBoardSprite,3);
    /*中边框*/
    CCSprite *pMidBoardSprite = CCSprite::createWithSpriteFrameName("DE_equip.png");
    pMidBoardSprite->setAnchorPoint(CCPointZero);
    pMidBoardSprite->setPosition(ccp(-50.0f,368.0f+OFFSET_V));
    m_pSBN->addChild(pMidBoardSprite,3);
    /*下边框*/
    CCSprite *pDownBoardSprite = CCSprite::createWithSpriteFrameName("DE_bottomboard.png");
    pDownBoardSprite->setAnchorPoint(CCPointZero);
    pDownBoardSprite->setPosition(ccp(0,276.0f+OFFSET_V));
    m_pSBN->addChild(pDownBoardSprite,3);
    /*下部边框*/
    CCSprite *pDownDownBoardSprite = CCSprite::createWithSpriteFrameName("DE_bottomboard.png");
    pDownDownBoardSprite->setAnchorPoint(CCPointZero);
    pDownDownBoardSprite->setPosition(ccp(0,0+OFFSET_V));
    m_pSBN->addChild(pDownDownBoardSprite,3);
    
    //添加兵种效果图片
    CCSprite *pArmEffectSprite = CCSprite::createWithSpriteFrameName("DE_armeffect.png");
    pArmEffectSprite->setAnchorPoint(CCPointZero);
    pArmEffectSprite->setPosition(ccp(46.0f,686.0f+OFFSET_V));
    m_pSBN->addChild(pArmEffectSprite,2);
    //添加分割线
    CCSprite *pLineSprite1 = CCSprite::createWithSpriteFrameName(IMAGE_LINE);
    pLineSprite1->setAnchorPoint(CCPointZero);
    pLineSprite1->setPosition(ccp(48.0f,638.0f+OFFSET_V));
    m_pSBN->addChild(pLineSprite1,3);
    
    //添加普通技能图片
    CCSprite *pNormaSkillSprite = CCSprite::createWithSpriteFrameName("DE_normalskill.png");
    pNormaSkillSprite->setAnchorPoint(CCPointZero);
    pNormaSkillSprite->setPosition(ccp(46.0f,602.0f+OFFSET_V));
    m_pSBN->addChild(pNormaSkillSprite,2);
    //添加分割线
    CCSprite *pLineSprite2 = CCSprite::createWithSpriteFrameName(IMAGE_LINE);
    pLineSprite2->setAnchorPoint(CCPointZero);
    pLineSprite2->setPosition(ccp(48.0f,558.0f+OFFSET_V));
    m_pSBN->addChild(pLineSprite2,3);
    //添加特殊技能图片
    CCSprite *pSpecialSkillSprite = CCSprite::createWithSpriteFrameName("DE_specialskill.png");
    pSpecialSkillSprite->setAnchorPoint(CCPointZero);
    pSpecialSkillSprite->setPosition(ccp(46.0f,521.0f+OFFSET_V));
    m_pSBN->addChild(pSpecialSkillSprite,2);
    
    //添加攻具
    CCSprite *pAttEquipSprite = CCSprite::createWithSpriteFrameName("DE_ATTequip.png");
    pAttEquipSprite->setAnchorPoint(CCPointZero);
    pAttEquipSprite->setPosition(ccp(208.0f,326.0f+OFFSET_V));
    m_pSBN->addChild(pAttEquipSprite,3);
    
    //添加防具
    CCSprite *pDefEquipSprite = CCSprite::createWithSpriteFrameName("DE_DEFequip.png");
    pDefEquipSprite->setAnchorPoint(CCPointZero);
    pDefEquipSprite->setPosition(ccp(328.0f,326.0f+OFFSET_V));
    m_pSBN->addChild(pDefEquipSprite,3);
    
    //添加武将简介
    CCSprite *pHeroIntroSprite = CCSprite::createWithSpriteFrameName("DE_herointrodu.png");
    pHeroIntroSprite->setAnchorPoint(CCPointZero);
    pHeroIntroSprite->setPosition(ccp(46.0f,232.0f+OFFSET_V));
    m_pSBN->addChild(pHeroIntroSprite,2);
    
    //添加分割线
    CCSprite *pLineSprite3 = CCSprite::createWithSpriteFrameName(IMAGE_LINE);
    pLineSprite3->setAnchorPoint(CCPointZero);
    pLineSprite3->setPosition(ccp(48.0f,176.0f+OFFSET_V));
    m_pSBN->addChild(pLineSprite3,3);
    
    //添加锁／解锁武将卡牌按钮的文字图片
    /*锁定武将卡牌按钮*/
     unsigned int tag = m_GameState->getTagWhoCallHeroSel();
     WhoCallHeroIntro tagLayer = m_GameState->getTagWhoCallHeroIntro();
    if (tagLayer == HEROLIST_LAYER && tag == WCH_BROWSE) {
        m_menuButtonSprite[0][0] = CCSprite::createWithSpriteFrameName(IMAGE_BTN);
        m_menuButtonSprite[0][0]->setAnchorPoint(CCPointZero);
        m_menuButtonSprite[0][1] = CCSprite::createWithSpriteFrameName(IMAGE_BTN_S);
        m_menuButtonSprite[0][1]->setAnchorPoint(CCPointZero);
        m_menuButtonSprite[0][2] = CCSprite::createWithSpriteFrameName(IMAGE_BTN);
        m_menuButtonSprite[0][2]->setAnchorPoint(CCPointZero);
        m_pSBN->addChild(m_menuButtonSprite[0][0],2);
        m_pSBN->addChild(m_menuButtonSprite[0][1],3);
        m_pSBN->addChild(m_menuButtonSprite[0][2],4);
        /*解锁武将卡牌文字*/
        CCSprite *pUnlockFontSprite = CCSprite::createWithSpriteFrameName(IMAGE_UNLOCK);
        pUnlockFontSprite->setAnchorPoint(CCPointZero);
        pUnlockFontSprite->setPosition(ccp(70.0f,BTN_PADDING_DOWN+11.0f+OFFSET_V));
        m_pSBN->addChild(pUnlockFontSprite,4,UNLOCK_TAG);
        pUnlockFontSprite->setVisible(false);
        /*锁定武将卡牌文字*/
        CCSprite *pLockedFontSprite = CCSprite::createWithSpriteFrameName(IMAGE_LOCKED);
        pLockedFontSprite->setAnchorPoint(CCPointZero);
        pLockedFontSprite->setPosition(ccp(70.0f,BTN_PADDING_DOWN+11.0f+OFFSET_V));
        m_pSBN->addChild(pLockedFontSprite,4,LOCKED_TAG);
        pLockedFontSprite->setVisible(false);
    }
    
    
    /*确定文字*/
    CCSprite *pOkFontSprite = CCSprite::createWithSpriteFrameName(IMAGE_OK);
    pOkFontSprite->setAnchorPoint(CCPointZero);
    m_pSBN->addChild(pOkFontSprite,5);
    /*确定按钮*/
    m_menuButtonSprite[1][0] = CCSprite::createWithSpriteFrameName(IMAGE_BTN);
    m_menuButtonSprite[1][0]->setAnchorPoint(CCPointZero);
    m_menuButtonSprite[1][1] = CCSprite::createWithSpriteFrameName(IMAGE_BTN_S);
    m_menuButtonSprite[1][1]->setAnchorPoint(CCPointZero);
    m_menuButtonSprite[1][2] = CCSprite::createWithSpriteFrameName(IMAGE_BTN);
    m_menuButtonSprite[1][2]->setAnchorPoint(CCPointZero);
    m_pSBN->addChild(m_menuButtonSprite[1][0],2);
    m_pSBN->addChild(m_menuButtonSprite[1][1],3);
    m_pSBN->addChild(m_menuButtonSprite[1][2],4);
    
    if ( (tagLayer == HEROLIST_LAYER && tag != WCH_BROWSE)|| (tagLayer == JY_LAYER)) {
        pOkFontSprite->setPosition(ccp(286.0f,BTN_PADDING_DOWN+11.0f+OFFSET_V));
    }else if( tagLayer == HEROLIST_LAYER && tag == WCH_BROWSE){

        pOkFontSprite->setPosition(ccp(446.0f,BTN_PADDING_DOWN+11.0f+OFFSET_V));
    }
    
   

    //初始化按钮
    _initMenuButton();
    //显示文字
    _updateHeroUI(m_uniId);
    _initLocked();
    return true;
}

void HeroIntroduce::_initLocked()
{
     WhoCallHeroIntro tagLayer = m_GameState->getTagWhoCallHeroIntro();
    int tag = m_GameState->getTagWhoCallHeroSel();
    if (tagLayer == HEROLIST_LAYER && tag == WCH_BROWSE) {
        CCSprite *locked=(CCSprite*)m_pSBN->getChildByTag(LOCKED_TAG);
        CCSprite *unlock=(CCSprite*)m_pSBN->getChildByTag(UNLOCK_TAG);
        if (m_isLocked){
            locked->setVisible(false);
            unlock->setVisible(true);
        }else{
            locked->setVisible(true);
            unlock->setVisible(false);
        }
    }
}
/*
 *      初始化按钮
 */
void HeroIntroduce::_initMenuButton()
{
    unsigned int tag = m_GameState->getTagWhoCallHeroSel();
    WhoCallHeroIntro tagLayer = m_GameState->getTagWhoCallHeroIntro();
    
    if ( tagLayer == HEROLIST_LAYER && tag == WCH_BROWSE) {
        for (int i=0; i < MENU_NUMBER ; i++) {
            m_menuButtonSprite[i][0]->setVisible(true);
            m_menuButtonSprite[i][0]->setPosition(m_menuButtonPosition[i][0]);
            m_menuButtonSprite[i][1]->setVisible(false);
            m_menuButtonSprite[i][1]->setPosition(m_menuButtonPosition[i][1]);
            m_menuButtonSprite[i][2]->setVisible(false);
            m_menuButtonSprite[i][2]->setPosition(m_menuButtonPosition[i][2]);
        }   /*for*/
    }else if( (tagLayer == HEROLIST_LAYER && tag != WCH_BROWSE)|| (tagLayer == JY_LAYER)){
        m_menuButtonSprite[1][0]->setVisible(true);
        m_menuButtonSprite[1][0]->setPosition(m_menuButtonPosition[1][0]);
        m_menuButtonSprite[1][1]->setVisible(false);
        m_menuButtonSprite[1][1]->setPosition(m_menuButtonPosition[1][1]);
        m_menuButtonSprite[1][2]->setVisible(false);
        m_menuButtonSprite[1][2]->setPosition(m_menuButtonPosition[1][2]);
    }
}


void HeroIntroduce::registerWithTouchDispatcher(){
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority-2, true);
}
bool HeroIntroduce::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    if (!m_enableTouch) return false;
    _initMenuButton();
    
    m_touchBeganMenuIndex = -1;
    CCPoint ttt = m_pSBN->convertTouchToNodeSpace(pTouch);
    unsigned int tag = m_GameState->getTagWhoCallHeroSel();
    WhoCallHeroIntro tagLayer = m_GameState->getTagWhoCallHeroIntro();
    if ( tagLayer == HEROLIST_LAYER && tag == WCH_BROWSE) {
        for (unsigned int i = 0; i < MENU_NUMBER; i++) {
            if (m_menuButtonSprite[i][0]->boundingBox().containsPoint(ttt)){
                m_touchBeganMenuIndex = i;
                m_menuButtonSprite[i][0]->setVisible(false);
                m_menuButtonSprite[i][1]->setVisible(true);
                m_menuButtonSprite[i][2]->setVisible(false);
                break;
            }
        } /*for*/
    }else if( (tagLayer == HEROLIST_LAYER && tag != WCH_BROWSE)|| (tagLayer == JY_LAYER)){
        if (m_menuButtonSprite[1][0]->boundingBox().containsPoint(ttt)){
            m_touchBeganMenuIndex = 1;
            m_menuButtonSprite[1][0]->setVisible(false);
            m_menuButtonSprite[1][1]->setVisible(true);
            m_menuButtonSprite[1][2]->setVisible(false);
        }
    }
    return true;
}

void HeroIntroduce::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {
    _initMenuButton();
    CCPoint ttt = m_pSBN->convertTouchToNodeSpace(pTouch);
    unsigned int tag = m_GameState->getTagWhoCallHeroSel();
    WhoCallHeroIntro tagLayer = m_GameState->getTagWhoCallHeroIntro();
    if (tagLayer == HEROLIST_LAYER && tag == WCH_BROWSE) {
        for (unsigned int i = 0; i < MENU_NUMBER; i++) {
            if (m_menuButtonSprite[i][0]->boundingBox().containsPoint(ttt)){
                m_touchBeganMenuIndex = i;
                m_menuButtonSprite[i][0]->setVisible(false);
                m_menuButtonSprite[i][1]->setVisible(true);
                m_menuButtonSprite[i][2]->setVisible(false);
                break;
            }
        } /*for*/
    }else if( (tagLayer == HEROLIST_LAYER && tag != WCH_BROWSE)|| (tagLayer == JY_LAYER)){
        if (m_menuButtonSprite[1][0]->boundingBox().containsPoint(ttt)){
            m_touchBeganMenuIndex = 1;
            m_menuButtonSprite[1][0]->setVisible(false);
            m_menuButtonSprite[1][1]->setVisible(true);
            m_menuButtonSprite[1][2]->setVisible(false);
        }
    }
}

//
void HeroIntroduce::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {
    int selectedMenuIndex = -1;
    CCPoint ttt = m_pSBN->convertTouchToNodeSpace(pTouch);
    unsigned int tag = m_GameState->getTagWhoCallHeroSel();
    WhoCallHeroIntro tagLayer = m_GameState->getTagWhoCallHeroIntro();
    if ( tagLayer == HEROLIST_LAYER && tag == WCH_BROWSE) {
        for (unsigned int i = 0; i < MENU_NUMBER; i++) {
            if ( m_touchBeganMenuIndex == i && m_menuButtonSprite[i][0]->boundingBox().containsPoint(ttt)) {
                selectedMenuIndex = i + 1;
                break;
            }
        } /*for*/
    }else if( (tagLayer == HEROLIST_LAYER && tag != WCH_BROWSE)|| (tagLayer == JY_LAYER)){
        if ( m_touchBeganMenuIndex == 1 && m_menuButtonSprite[1][0]->boundingBox().containsPoint(ttt)) {
            selectedMenuIndex = 2;
        }
    }
    _initMenuButton();
    
    if (selectedMenuIndex > 0) /* 玩家选择了某个菜单选项 */ {
        _onMenuButtonSelected(selectedMenuIndex);
    }

}



/*
 *  将能响应 touch 事件的 ui 组件移动到屏幕外面
 */
void HeroIntroduce::moveOutTouchUICompent()
{
    m_enableTouch = false;
    CCSequence* seq = CCSequence::create(CCCallFunc::create(this,callfunc_selector(HeroIntroduce::moveOutDidFinish)),NULL);
    this->runAction(seq);
    
    
}
void HeroIntroduce::moveOutDidFinish()
{
    if (m_GameState->getTagWhoCallHeroIntro() == HEROLIST_LAYER) {
        HerosLayer* pHerosLayer = (HerosLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(HEROS_LAYER_TAG);
        m_GameState->setTagWhoCallHeroIntro(HEROLIST_LAYER) ;
        pHerosLayer->setVisible(true);
        this->removeFromParent();
    }else if(m_GameState->getTagWhoCallHeroIntro() == JY_LAYER){
        HerosLayer* pHerosLayer = (HerosLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(JIANYU_LAYER);
        m_GameState->setTagWhoCallHeroIntro(JY_LAYER) ;
        pHerosLayer->setVisible(true);
        this->removeFromParent();
    }
}
/*
 *  恢复响应动作
 */
void HeroIntroduce::restoreTouchUICompent()
{
    m_enableTouch = true;
    CCSequence* seq = CCSequence::create(CCCallFunc::create(this,callfunc_selector(HeroIntroduce::restoreDidFinish)),NULL);
    this->runAction(seq);
    
}
void HeroIntroduce::restoreDidFinish()
{
    if (m_GameState->getTagWhoCallHeroIntro() == HEROLIST_LAYER) {
        HerosLayer* pHerosLayer = (HerosLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(HEROS_LAYER_TAG);
        pHerosLayer->setVisible(false);
    }else if(m_GameState->getTagWhoCallHeroIntro() == JY_LAYER){
        HerosLayer* pHerosLayer = (HerosLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(JIANYU_LAYER);
        pHerosLayer->setVisible(false);
    }
}

/*
 *  设置不可见的不可触摸
 */
void HeroIntroduce::setVisible(bool visible) {
    CCLayer::setVisible(visible);
    this->setTouchEnabled(visible);
    m_enableTouch = visible;
    
    if (visible)
        _initMenuButton();
}

/*
 *  功能：
 *      更新指定武将的ui显示
 *  参数：
 *      uniid       武将 uniid
 */
void HeroIntroduce::_updateHeroUI(std::string uniid) {
    
    //添加需要提取的文字的节点
    CCNode* FontNode = CCNode::create();
    this->addChild(FontNode, 1, TEXT_CONTAINER_TAG);
    char buffer[256];/*用来装文字*/
    HeroOfPlayerItem* pHeroOfPlayerItem = m_pHeroManager->getHeroDataByUniId(uniid);
    HeroItem* pHeroItem = NULL;
    
    if(m_GameState->getTagWhoCallHeroIntro() == JY_LAYER){
        pHeroItem = m_pHeroManager->getHeroItem(m_id);
    }else {
            pHeroItem = pHeroOfPlayerItem->getHeroItem();
    }
    
    //添加武将头像
    strcpy(buffer, pHeroItem->getBIcon().c_str());
    CCSprite* HeroSprite = CCSprite::create(buffer);
    HeroSprite->setAnchorPoint(CCPointZero);
    HeroSprite->setPosition(ccp(15,736+OFFSET_V));//15,103
    FontNode->addChild(HeroSprite,1);
    
    //添加武将名字文字
    strcpy(buffer, pHeroItem->getName().c_str());
    CCStrokeLabel* HeroNameLabel = CCStrokeLabel::create(buffer, "Arial", 32);
    HeroNameLabel->setAnchorPoint(CCPointZero);
    HeroNameLabel->setPosition(ccp(STAR_PADDING_LEFT,941.0f+OFFSET_V));
    FontNode->addChild(HeroNameLabel,1);
    HeroNameLabel->setColor(TEXT_COLOR_WHITE);
    
    //添加等级文字
    if(m_GameState->getTagWhoCallHeroIntro() == JY_LAYER){
        sprintf(buffer,"%d",1);
    }else if(m_GameState->getTagWhoCallHeroIntro() == HEROLIST_LAYER){
        sprintf(buffer,"%d",pHeroOfPlayerItem->getLevel());
    }
    CCStrokeLabel* LevelLabel = CCStrokeLabel::create(buffer, "Arial", 28);
    LevelLabel->setAnchorPoint(CCPointZero);
    LevelLabel->setPosition(ccp(425.0f,856.0f+OFFSET_V));
    FontNode->addChild(LevelLabel,1);
    LevelLabel->setColor(TEXT_COLOR_WHITE);
    
    //添加hp文字
    if(m_GameState->getTagWhoCallHeroIntro() == JY_LAYER){
        sprintf(buffer,"%d",(int)pHeroItem->getBasicHp());
    }else{
        sprintf(buffer,"%d",pHeroOfPlayerItem->getHP());
    }
    CCStrokeLabel* HpLabel = CCStrokeLabel::create(buffer, "Arial", 28);
    HpLabel->setAnchorPoint(CCPointZero);
    HpLabel->setPosition(ccp(425.0f,809.0f+OFFSET_V));
    FontNode->addChild(HpLabel,1);
    HpLabel->setColor(TEXT_COLOR_WHITE);
    //添加攻击力文字
    if(m_GameState->getTagWhoCallHeroIntro() == JY_LAYER){
        sprintf(buffer,"%d",(int)pHeroItem->getBasicProperty());
        CCLog("监狱攻击力 = %s",buffer);
    }else{
        sprintf(buffer,"%d",pHeroOfPlayerItem->getAttack());
    }
    CCStrokeLabel* AttLabel = CCStrokeLabel::create(buffer, "Arial", 28);
    AttLabel->setAnchorPoint(CCPointZero);
    AttLabel->setPosition(ccp(425.0f,757.0f+OFFSET_V));
    FontNode->addChild(AttLabel,1);
    AttLabel->setColor(TEXT_COLOR_WHITE);
    
    //添加兵种效果图标
    int njob = pHeroItem->getJob();
    sprintf(buffer, "l%d.png", njob);
    CCSprite* ArmEffectSprite = CCSprite::create(buffer);
    ArmEffectSprite->setAnchorPoint(CCPointZero);
    ArmEffectSprite->setPosition(ccp(197.0f,689.0f+OFFSET_V));
    ArmEffectSprite->setScale(1.1f);
    FontNode->addChild(ArmEffectSprite,10);
    switch (njob) {
        case 1:
            strcpy(buffer, "步兵");
            break;
        case 2:
            strcpy(buffer, "骑兵");
            break;
        case 3:
            strcpy(buffer, "弓兵");
            break;
        case 4:
            strcpy(buffer, "医师");
            break;
        case 5:
            strcpy(buffer, "谋士");
            break;
        case 6:
            strcpy(buffer, "策士");
            break;
        case 7:
            strcpy(buffer, "都尉");
            break;
          default:
            break;
    }
    CCStrokeLabel* ArmEffectLabel = CCStrokeLabel::create(buffer, "Arial", 28);
    ArmEffectLabel->setAnchorPoint(CCPointZero);
    ArmEffectLabel->setPosition(ccp(198.0f+ArmEffectSprite->getContentSize().width,686.0f+OFFSET_V));
    ArmEffectLabel->setColor(TEXT_COLOR_WHITE);
    FontNode->addChild(ArmEffectLabel,10);
    
    SkillManager* pSkillManager = SkillManager::getInstance();
    
    SkillItem* pSkillItemP = pSkillManager->getSkillItem(pHeroItem->getPs());
    if (pSkillItemP) {
            //添加兵种效果描述
            strcpy(buffer,pSkillItemP->getDesc().c_str());
            CCStrokeLabel* ArmEffectLabelDes = CCStrokeLabel::create(buffer, "Arial", 20);
            ArmEffectLabelDes->setAnchorPoint(CCPointZero);
            ArmEffectLabelDes->setPosition(ccp(46.0f,653.0f+OFFSET_V));
            FontNode->addChild(ArmEffectLabelDes,10);
            ArmEffectLabelDes->setColor(TEXT_COLOR_WHITE);
    }
    
    
    //添加普通技能文字
    SkillItem* pSkillItemN = pSkillManager->getSkillItem(pHeroItem->getNs());
    strcpy(buffer,pSkillItemN->getName().c_str());
    CCStrokeLabel* NormalSkillLabel = CCStrokeLabel::create(buffer, "Arial", 28);
    NormalSkillLabel->setAnchorPoint(CCPointZero);
    NormalSkillLabel->setPosition(ccp(197.0f,602.0f+OFFSET_V));
    FontNode->addChild(NormalSkillLabel,1);
    NormalSkillLabel->setColor(TEXT_COLOR_WHITE);
    //添加普通技能描述 Desc
    strcpy(buffer,pSkillItemN->getDesc().c_str());
    CCStrokeLabel* NormalSkillDesLabel = CCStrokeLabel::create(buffer, "Arial", 20);
    NormalSkillDesLabel->setAnchorPoint(CCPointZero);
    NormalSkillDesLabel->setPosition(ccp(46.0f,570.0f+OFFSET_V));
    FontNode->addChild(NormalSkillDesLabel,1);
    NormalSkillDesLabel->setColor(TEXT_COLOR_WHITE);

    //添加特殊技能文字
    SkillItem* pSkillItemA= pSkillManager->getSkillItem(pHeroItem->getAs());
    
    strcpy(buffer,pSkillItemA->getName().c_str());
    CCStrokeLabel* SpecialSkillLabel = CCStrokeLabel::create(buffer, "Arial", 28);
    SpecialSkillLabel->setAnchorPoint(CCPointZero);
    SpecialSkillLabel->setPosition(ccp(197.0f,521.0f+OFFSET_V));
    FontNode->addChild(SpecialSkillLabel,1);
    SpecialSkillLabel->setColor(TEXT_COLOR_WHITE);
    //添加特殊技能描述
    strcpy(buffer,pSkillItemA->getDesc().c_str());
    CCStrokeLabel* SpecialSkillDesLabel = CCStrokeLabel::create(buffer, "Arial", 20);
    SpecialSkillDesLabel->setAnchorPoint(CCPointZero);
    SpecialSkillDesLabel->setPosition(ccp(46.0f,487.0f+OFFSET_V));
    FontNode->addChild(SpecialSkillDesLabel,1);
    SpecialSkillDesLabel->setColor(TEXT_COLOR_WHITE);
    
    if(m_GameState->getTagWhoCallHeroIntro() != JY_LAYER){
      if (pHeroOfPlayerItem->getWeapon()) {
        //添加攻击装名字
        
        CCStrokeLabel* AttEquipNameLabel = CCStrokeLabel::create(buffer, "Arial", 20);
        AttEquipNameLabel->setAnchorPoint(CCPointZero);
        AttEquipNameLabel->setPosition(ccp(210.f,298.0f+OFFSET_V));
        FontNode->addChild(AttEquipNameLabel,1);
        AttEquipNameLabel->setColor(TEXT_COLOR_WHITE);
   
        //添加装备效果描述1，2，3，4
        /*左边文字*/
        // buffer------>装备效果未完成
    
        CCLabelTTF* EquipEffect_LeftLabel = CCLabelTTF::create(buffer, "Arial", 20,CCSizeMake(183.0f, 108.0f), kCCTextAlignmentLeft);
        EquipEffect_LeftLabel->setAnchorPoint(CCPointZero);
        EquipEffect_LeftLabel->setPosition(ccp(17.0f,310.0f+OFFSET_V));
        FontNode->addChild(EquipEffect_LeftLabel,1);
        EquipEffect_LeftLabel->setColor(TEXT_COLOR_WHITE);
     }
    
     if (pHeroOfPlayerItem->getArmor()) {
        //添加防具名字
        CCStrokeLabel* DefEquipNameLabel = CCStrokeLabel::create(buffer, "Arial", 20);
        DefEquipNameLabel->setAnchorPoint(CCPointZero);
        DefEquipNameLabel->setPosition(ccp(332.0f,298.0f+OFFSET_V));
        FontNode->addChild(DefEquipNameLabel,1);
        DefEquipNameLabel->setColor(TEXT_COLOR_WHITE);
           /*右边文字*/
        // buffer------>装备效果未完成
    
        CCLabelTTF* EquipEffect_RightLabel = CCLabelTTF::create(buffer, "Arial", 20,CCSizeMake(183.0f, 108.0f), kCCTextAlignmentLeft);
        EquipEffect_RightLabel->setAnchorPoint(CCPointZero);
        EquipEffect_RightLabel->setPosition(ccp(429.0f,310.0f+OFFSET_V));
        FontNode->addChild(EquipEffect_RightLabel);
        EquipEffect_RightLabel->setColor(TEXT_COLOR_WHITE);
       }
    }
    //添加武将简介
    strcpy(buffer, pHeroItem->getJj().c_str());
    CCLabelTTF* HeroIntroDesLabel = CCLabelTTF::create(buffer, "Arial", 18,
                                    CCSizeMake(558.0f, 50.0f), kCCTextAlignmentLeft);
    HeroIntroDesLabel->setAnchorPoint(CCPointZero);
    HeroIntroDesLabel->setPosition(ccp(46.0f,180.0f+OFFSET_V));
    FontNode->addChild(HeroIntroDesLabel, 1);
    HeroIntroDesLabel->setColor(TEXT_COLOR_WHITE);
    
}

/*
 *  响应菜单选择
 */
void HeroIntroduce::_onMenuButtonSelected(const unsigned menuIndex)
{
    switch (menuIndex) {
        case 1:
        {
            if (m_isLocked) {
                CCLog("解锁武将");
                m_isLocked = false;
                isLocked(m_isLocked);           //锁链是否显示
                m_isLocked = false;
                _initLocked();
                
            }else {
                CCLog("锁定武将");
                m_isLocked = true;
                isLocked(m_isLocked);           //锁链是否显示
                 _initLocked();
            }
            ///////////////////////////
            //////传给服务器解锁信息//////
            ///////////////////////////
          
             NetConnection* pNetConnection = NetConnection::getInstance();
             HeroOfPlayerItem* pHeroOfPlayerItem = m_pHeroManager->getHeroDataByUniId(m_uniId);
             if (pHeroOfPlayerItem) {
                char szPostBuffer[MAX_POSTTO_SERVER_BUFFER_SIZE];
                memset(szPostBuffer, '\0', MAX_POSTTO_SERVER_BUFFER_SIZE);
                strcpy(szPostBuffer, "c=hero&a=lock&type=");
                char szHttpTag[] = HTTP_REQUEST_HEROLOCK_TAG;
                if (m_isLocked) {
                    strcat(szPostBuffer, "lock&hero=");
                }else{
                    strcat(szPostBuffer, "unlock&hero=");
                }
                strcat(szPostBuffer, m_uniId.c_str());
                 strcat(szPostBuffer, "&uid=");
                 strcat(szPostBuffer, GamePlayer::getInstance()->getUid().c_str());
                CCLog("post = %s", szPostBuffer);
                pNetConnection->commitPostRequestData(szPostBuffer, szHttpTag);
             }
           
        }
            break;
        case 2:
            CCNotificationCenter::sharedNotificationCenter()->postNotification(ON_MESSAGE_HEROINTRO_OK_STRING);
            break;
        default:
            break;
    }
    
}


/*武将品级的显示*/
void HeroIntroduce::starShow()
    {
        int lev =0,rare = 0;
        if (m_GameState->getTagWhoCallHeroIntro() == JY_LAYER) {
            lev = 1;
            rare = m_pHeroManager->getHeroItem(m_id)->getRarity();
        }else{
            lev = m_pHeroManager->getHeroDataByUniId(m_uniId)->getLevel();
            rare = m_pHeroManager->getHeroDataByUniId(m_uniId)->getHeroItem()->getRarity();
        }
    int levi = (int)((lev - 1) / 30) + 1;
        
    if (1 == rare) {
        m_pSBN->getChildByTag(200)->setVisible(true);
        for (unsigned int i = 0; i < levi; i++) {
            m_pSBN->getChildByTag(300+i)->setVisible(true);
        }

    } else if(2 == rare || 3 == rare) {
        m_pSBN->getChildByTag(201)->setVisible(true);
        for (unsigned int i = 0; i < levi; i++) {
            m_pSBN->getChildByTag(300+i+5)->setVisible(true);
        }
    } else if(4 == rare || 5 == rare) {
        m_pSBN->getChildByTag(202)->setVisible(true);
        for (unsigned int i = 0; i < levi; i++) {
            m_pSBN->getChildByTag(300+i+10)->setVisible(true);
        }
    } else if(6 ==rare || 7 == rare || 8 == rare) {
        m_pSBN->getChildByTag(203)->setVisible(true);
        for (unsigned int i = 0; i < levi; i++) {
            m_pSBN->getChildByTag(300+i+15)->setVisible(true);
        }
    } else {
        m_pSBN->getChildByTag(204)->setVisible(true);
        for (unsigned int i = 0; i < levi; i++) {
            m_pSBN->getChildByTag(300+i+20)->setVisible(true);
        }
    }

}

/*武将是否解锁的显示*/
void HeroIntroduce::isLocked(bool locked)
{
    if (locked) {
        CCSprite * lock = (CCSprite*)m_pSBN->getChildByTag(LOCK_TAG);
        lock->setVisible(locked);        
    }else{
        CCSprite * lock = (CCSprite*)m_pSBN->getChildByTag(LOCK_TAG);
        lock->setVisible(locked);
    }
}
/*武将上锁*/
void HeroIntroduce::setLockSuccess(bool isSuccess){
    if(isSuccess){
        m_pHeroManager->getHeroDataByUniId(m_uniId)->setLock(m_isLocked);
    }
}