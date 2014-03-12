
#include "ChosFormLayer.h"
#include "HeroManager.h"
#include "HeroOfPlayerItem.h"
#include "HeroItem.h"
#include "Battle.h"
#include "GameState.h"
#include "common.h"
#include "customMessage.h"
#include "GamePlayer.h"
#include "LevelDataItem.h"
#include "CopyManager.h"
#include "CopyDataItem.h"
#include "MessageBox.h"

#define OPENNING_ANI_DURING                     0.2f
#define CONTAINER                               1300                // 整个层的TAG

#define TOUCH_AREA_LEFT                         8.0f                //
#define TOUCH_AREA_TOP                          600.0f              //
#define TOUCH_AREA_HORIZONTAL_SPACING           5.0f                // 水平间距
#define TOUCH_AREA_VERTICAL_SPACING             8.0f                // 垂直间距
#define HERO_HEAD_WIDTH                         200.0f              // 武将头像宽度
#define HERO_HEAD_HEIGHT                        70.0f               // 武将头像高度
#define SOLDIERS_HORIZONTAL_SPACING             7.0f                // 士兵之间水平间隔
#define SOLDIERS_VERTICAL_SPACING               1.0f                // 士兵之间垂直间隔
#define SOLDIERS_COUNT_PER_ROW                  5                   // 每行士兵的数量
#define SOLDIER_IMAGE_WIDTH                     25.0f               // 士兵图片宽度
#define SOLDIER_IMAGE_HEIGHT                    35.0f               // 士兵图片高度
#define SOLDIER_PADDING_LEFT                    10.0f               // 士兵方阵左间隔
#define SOLDIER_PADDING_HERO                    3.0f                // 士兵方阵离武将间隔

#define BT_MOK_WIDTH                            164.0f              // 背景框宽度
#define BT_MOK_HEIGHT                           62.0f               // 背景框高度
#define BT_RARITY_WIDTH                         154.0f              // 稀有度背景框宽度
#define BT_RARITY_HEIGHT                        50.0f               // 稀有度背景框高度
#define BT_JOB_WIDTH                            26.0f               // 职业宽度
#define BT_JOB_HEIGHT                           26.0f               // 职业高度
#define BT_JOB_BOX_WIDTH                        42.0f               // 职业背景框宽度
#define BT_JOB_BOX_HEIGHT                       42.0f               // 职业背景框高度
#define BT_CORNER_WIDTH                         22.0f               // 边角宽度
#define BT_CORNER_BOX_HEIGHT                    20.0f               // 边角高度

/* 武将框偏移位置 */
#define HERO_BOX_OFFSETX                         20.0f
#define HERO_BOX_OFFSETY                         (-5.0f)

#define DEFAULT_ZORDER                          4                   // 默认 zorder 值

#define LL_MENU_TAG                             1301                // tipMenu 左边容器 tag 值
#define MID_MENU_TAG                            1302                // ..
#define RR_MENU_TAG                             1303                // ..

#define HERO_SPRITE_BATCH_TAG                   2001                // 用于渲染武将头像的批处理精灵
#define HERO_BOX_BATCH_TAG                      2002                //


#define CHOSFORM_BTN                            3000                // 阵型选择按钮
#define CHOSFORMSEL_BTN                         3500                // 被选中
#define CHOSBTN_WIDTH                           140                 // 按钮宽
#define CHOSBTN_FONT                            4000                // 按钮文字
#define CHOSBTN_MENU                            4100                // 菜单TAG

#define MOVEOUT_TAG                         5100
#define MOVEIN_TAG                          5101

/*
 *  构造函数
 */
ChosFormLayer::ChosFormLayer():
m_enableTouch(false)
,m_pGameState(GameState::getInstance())
,m_pHeroManager(HeroManager::getInstance())
,m_pBattle(Battle::getInstance())
,m_pGamePlayer(GamePlayer::getInstance())
,m_pNotificationCenter(CCNotificationCenter::sharedNotificationCenter())
{
    _touchBeganIndex = 0;
    
    for (int i = 0; i < MAX_IN_BATTLE_HEROS; i++) {
        m_team[i] = NULL;
    } /*for*/
    
    /*
     * 设置所有位置
     */
    for (int i = 0; i < MAX_IN_BATTLE_HEROS; i++) {
        // 设置武将头像位置（锚点在中间）
        float heroPosX = TOUCH_AREA_LEFT + HERO_HEAD_WIDTH / 2.0f +
            (i % 3) * (HERO_HEAD_WIDTH + TOUCH_AREA_HORIZONTAL_SPACING);
        
        float heroPosY = TOUCH_AREA_TOP + m_pGameState->getBottomOffset() - HERO_HEAD_HEIGHT / 2.0f;
        heroPosY -= i > 2 ? TOUCH_AREA_VERTICAL_SPACING + SOLDIER_PADDING_HERO +
            SOLDIER_IMAGE_HEIGHT * 3 + SOLDIERS_VERTICAL_SPACING * 2 + HERO_HEAD_HEIGHT : 0.0f;
        m_AllPos[i * CFL_COUNT_OF_TEAM] = ccp(heroPosX, heroPosY);
        
        float heroLeft = heroPosX - HERO_HEAD_WIDTH / 2.0f;
        float herorRight = heroPosX + HERO_HEAD_WIDTH / 2.0f;
        float heroTop = heroPosY + HERO_HEAD_HEIGHT / 2.0f;
        float herorBottom = heroPosY - HERO_HEAD_HEIGHT / 2.0f;
        
        // 背景框
        m_AllPos[i * CFL_COUNT_OF_TEAM + 1] = ccp(heroLeft + BT_MOK_WIDTH / 2 + HERO_BOX_OFFSETX,
                                                  herorBottom + BT_MOK_HEIGHT / 2 + HERO_BOX_OFFSETY);
        // 稀有度背景
        m_AllPos[i * CFL_COUNT_OF_TEAM + 2] = ccp(heroLeft + BT_RARITY_WIDTH / 2 + HERO_BOX_OFFSETX + 5.0f,
                                                  herorBottom + BT_RARITY_HEIGHT / 2 + 6.0f + HERO_BOX_OFFSETY);
        // 职业
        m_AllPos[i * CFL_COUNT_OF_TEAM + 3] = ccp(herorRight - BT_JOB_WIDTH / 2.0f - 45.0f + HERO_BOX_OFFSETX,
                                                  heroTop - BT_JOB_HEIGHT / 2.0f + HERO_BOX_OFFSETY - 15.0f);
        // 职业框
        m_AllPos[i * CFL_COUNT_OF_TEAM + 4] = ccp(herorRight - BT_JOB_BOX_WIDTH / 2.0f - 37.0f + HERO_BOX_OFFSETX,
                                                  heroTop - BT_JOB_BOX_HEIGHT / 2.0f + HERO_BOX_OFFSETY - 8.0f);
        // 边角1
        m_AllPos[i * CFL_COUNT_OF_TEAM + 5] = ccp(heroLeft + BT_CORNER_WIDTH / 2 + HERO_BOX_OFFSETX + 6.0f,
                                                  herorBottom + BT_CORNER_BOX_HEIGHT / 2 + 5.0f + HERO_BOX_OFFSETY);
        // 边角2
        m_AllPos[i * CFL_COUNT_OF_TEAM + 6] = ccp(herorRight - BT_CORNER_WIDTH / 2 + HERO_BOX_OFFSETX - 43.0f,
                                                  herorBottom + BT_CORNER_BOX_HEIGHT / 2 + 5.0f + HERO_BOX_OFFSETY);
        // 边角3
        m_AllPos[i * CFL_COUNT_OF_TEAM + 7] = ccp(heroLeft + BT_CORNER_WIDTH / 2 + HERO_BOX_OFFSETX + 6.0f,
                                                  heroTop + BT_CORNER_BOX_HEIGHT / 2 - 35.0f + HERO_BOX_OFFSETY);
        
        // 设置士兵头像位置（锚点在中间）
        unsigned int col = 0;
        for (int j = 0; j < MAX_IN_BATTLE_SOLDIERS; j++) {
            col = j % SOLDIERS_COUNT_PER_ROW;
            
            float soldierPosX = TOUCH_AREA_LEFT + SOLDIER_PADDING_LEFT + SOLDIER_IMAGE_WIDTH / 2.0f + 15.0f;
            soldierPosX += (i % 3) * (SOLDIER_PADDING_LEFT + HERO_HEAD_WIDTH + TOUCH_AREA_HORIZONTAL_SPACING - 10.0f);
            if (0 == col)
                soldierPosX += 2 * (SOLDIER_IMAGE_WIDTH + SOLDIERS_HORIZONTAL_SPACING);
            else if (1 == col)
                soldierPosX += SOLDIER_IMAGE_WIDTH + SOLDIERS_HORIZONTAL_SPACING;
            else if (2 == col)
                soldierPosX += 3 * (SOLDIER_IMAGE_WIDTH + SOLDIERS_HORIZONTAL_SPACING);
            else if (3 == col)
                soldierPosX += 0;
            else if (4 == col)
                soldierPosX += 4 * (SOLDIER_IMAGE_WIDTH + SOLDIERS_HORIZONTAL_SPACING);
            
            float soldierPosY = TOUCH_AREA_TOP + m_pGameState->getBottomOffset()
                - HERO_HEAD_HEIGHT - SOLDIER_PADDING_HERO - SOLDIER_IMAGE_HEIGHT / 2.0f;
            soldierPosY -= i > 2 ? SOLDIER_PADDING_HERO + HERO_HEAD_HEIGHT +
                TOUCH_AREA_VERTICAL_SPACING + SOLDIER_IMAGE_HEIGHT * 3 + SOLDIERS_VERTICAL_SPACING * 2 : 0.0f;
            soldierPosY -= (j - j % 5) / 5  * (SOLDIER_IMAGE_HEIGHT + SOLDIERS_VERTICAL_SPACING);
            
            m_AllPos[i * CFL_COUNT_OF_TEAM + 8 + j] = ccp(soldierPosX, soldierPosY);
        } /*for*/
    } /*for*/
    
    /*
     *  设置触摸区域（每个要上阵的武将和兵士）
     */
    unsigned int touchHeight = HERO_HEAD_HEIGHT + SOLDIER_PADDING_HERO + SOLDIER_IMAGE_HEIGHT * 3 + SOLDIERS_VERTICAL_SPACING * 2;
    m_touchArea[0] = CCRectMake(TOUCH_AREA_LEFT,
                                TOUCH_AREA_TOP + m_pGameState->getBottomOffset() - touchHeight,
                                HERO_HEAD_WIDTH,
                                touchHeight);
    m_touchArea[1] = CCRectMake(m_touchArea[0].origin.x + HERO_HEAD_WIDTH + TOUCH_AREA_HORIZONTAL_SPACING,
                m_touchArea[0].origin.y,
                m_touchArea[0].size.width,
                m_touchArea[0].size.height);
    m_touchArea[2] = CCRectMake(m_touchArea[1].origin.x + HERO_HEAD_WIDTH + TOUCH_AREA_HORIZONTAL_SPACING,
                m_touchArea[0].origin.y,
                m_touchArea[0].size.width,
                m_touchArea[0].size.height);
    m_touchArea[3] = CCRectMake(m_touchArea[0].origin.x,
                                m_touchArea[0].origin.y - m_touchArea[0].size.height - TOUCH_AREA_VERTICAL_SPACING,
                                m_touchArea[0].size.width,
                                m_touchArea[0].size.height);
    m_touchArea[4] = CCRectMake(m_touchArea[1].origin.x,
                                m_touchArea[3].origin.y,
                                m_touchArea[0].size.width,
                                m_touchArea[0].size.height);
    m_touchArea[5] = CCRectMake(m_touchArea[2].origin.x,
                                m_touchArea[3].origin.y,
                                m_touchArea[0].size.width,
                                m_touchArea[0].size.height);
}

/*
 *  析构函数
 */
ChosFormLayer::~ChosFormLayer() {
    for (int i = 0; i < MAX_IN_BATTLE_HEROS; i++) {
        if (m_team[i]) {
            m_team[i]->removeAllObjects();
            m_team[i]->release();
        }
    } /*for*/
    
    // release 所有士兵动画
    for (int i = 0; i < 7; i++) {
        m_soldierAni[i]->release();
    } /*for*/
    
    /*
     *  释放纹理资源
     */
    CCTextureCache* pTextureCache = CCTextureCache::sharedTextureCache();
    
    char buffer[32];
    for (unsigned int i = 0; i < 4; ++i) {
        sprintf(buffer, "FB_form%d.png", i + 1);
        pTextureCache->removeTextureForKey(buffer);
        
        sprintf(buffer, "FB_form%dh.png", i+1);
        pTextureCache->removeTextureForKey(buffer);
    } /*for*/
    pTextureCache->removeTextureForKey("FB_bg.png");
    pTextureCache->removeTextureForKey("FB_bgx.png");
    pTextureCache->removeTextureForKey("FB_kuan.png");
    pTextureCache->removeTextureForKey("CM_left.png");
    pTextureCache->removeTextureForKey("LU_return.png");
    pTextureCache->removeTextureForKey("CM_mid.png");
    pTextureCache->removeTextureForKey("FB_chgForm.png");
    pTextureCache->removeTextureForKey("CM_right.png");
    pTextureCache->removeTextureForKey("CM_bigBtn.png");
    pTextureCache->removeTextureForKey("FB_chosFrid.png");
    pTextureCache->removeTextureForKey("CM_bigBtn.png");
    pTextureCache->removeTextureForKey("FB_fight.png");
    pTextureCache->removeTextureForKey("FB_ganz.png");
    pTextureCache->removeTextureForKey("FB_formnor.png");
    pTextureCache->removeTextureForKey("FB_forms.png");
    pTextureCache->removeTextureForKey("FB_21.png");
}

/*
 *  初始化
 */
bool ChosFormLayer::init() {
    // super init
    if (!CCLayer::init()) {
        return false;
    }
    this->setTouchEnabled(true);
    
    // 初始化 ui 组件
    _initUiComponent();
    
    // 加载士兵动画
    _loadSoldierAni();
    
    return true;
}

//
void ChosFormLayer::registerWithTouchDispatcher(void) {
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, false);
}

//
bool ChosFormLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    if (!m_enableTouch)
        return false;
    
    CCPoint touchPoint = this->convertTouchToNodeSpace(pTouch);
    for (int i = 0; i < MAX_IN_BATTLE_HEROS; i++) {
        if (m_team[i] && m_touchArea[i].containsPoint(touchPoint)) /* 选中了要调整的 team */ {
            _touchBeganIndex = i;
            _touchBeganPoint = touchPoint;
            // 调整 zorder 值
            _updateTeamZorder(i, DEFAULT_ZORDER + 100);
            return true;
        }
    } /*for*/
    
    return false;
}

/*
 *
 */
void ChosFormLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {
    CCPoint touchPoint = this->convertTouchToNodeSpace(pTouch);
    
    if (_touchBeganIndex < MAX_IN_BATTLE_HEROS && m_team[_touchBeganIndex]) {
        float offsetX = touchPoint.x - _touchBeganPoint.x;
        float offsetY = touchPoint.y - _touchBeganPoint.y;

        CCObject* item = NULL;
        unsigned int index = 0;
        CCSprite* pItem = NULL;
        CCPoint point;
        CCARRAY_FOREACH(m_team[_touchBeganIndex], item) {
            pItem = (CCSprite*)item;
            point = m_AllPos[_touchBeganIndex * CFL_COUNT_OF_TEAM + index++];
            point.x += offsetX;
            point.y += offsetY;
            pItem->setPosition(point);
        } /*for*/
    }
}

/*
 *
 */
void ChosFormLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {
    CCPoint touchPoint = this->convertTouchToNodeSpace(pTouch);
    unsigned int touchEndedIndex = 0;       // 触摸结束位置所在阵型位置索引
    bool isSet = false;

    for (unsigned int i = 0; i < MAX_IN_BATTLE_HEROS; i++) {
        if (m_touchArea[i].containsPoint(touchPoint) ) {
            if (i != _touchBeganIndex) {
                isSet = true;
                touchEndedIndex = i;
                break;
            }
        }
    } /*for*/

    CCObject* item = NULL;
    unsigned int index = 0;
    CCSprite* pItem = NULL;
    if (isSet) {
        if (m_team[touchEndedIndex]) /* 目标位置已经有 team */{
            CCArray* pTempArray = m_team[touchEndedIndex];
            m_team[touchEndedIndex] = m_team[_touchBeganIndex];
            m_team[_touchBeganIndex] = pTempArray;
            
            index = 0;
            CCARRAY_FOREACH(m_team[_touchBeganIndex], item) {
                ((CCSprite*)item)->setPosition(m_AllPos[_touchBeganIndex * CFL_COUNT_OF_TEAM + index++]);
            } /*for*/
        } else /*目标位置没有武将*/ {
            m_team[touchEndedIndex] = m_team[_touchBeganIndex];
            m_team[_touchBeganIndex] = NULL;
        }
        
        index = 0;
        CCARRAY_FOREACH(m_team[touchEndedIndex], item) {
            ((CCSprite*)item)->setPosition(m_AllPos[touchEndedIndex * CFL_COUNT_OF_TEAM + index++]);
        } /*for*/
        
        // 调整 zorder 值
        _updateTeamZorder(touchEndedIndex, DEFAULT_ZORDER);
    } else {
        // 回到原来的位置
        index = 0;
        CCARRAY_FOREACH(m_team[_touchBeganIndex], item) {
            pItem = (CCSprite*)item;
            pItem->setPosition(m_AllPos[_touchBeganIndex * CFL_COUNT_OF_TEAM + index++]);
        } /*for*/
        // 调整 zorder 值
        _updateTeamZorder(_touchBeganIndex, DEFAULT_ZORDER);
    }
}

/*
 *
 */
void ChosFormLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {
    CCObject* item = NULL;
    unsigned int index = 0;
    CCSprite* pItem = NULL;
    // 回到原来的位置
    CCARRAY_FOREACH(m_team[_touchBeganIndex], item) {
        pItem = (CCSprite*)item;
        pItem->setPosition(m_AllPos[_touchBeganIndex * CFL_COUNT_OF_TEAM + index++]);
    } /*for*/
    // 调整 zorder 值
    _updateTeamZorder(_touchBeganIndex, DEFAULT_ZORDER);
}

/*
 *
 */
void ChosFormLayer::setVisible(bool visible) {
    ////////////////////////////////
    // super first
    CCLayer::setVisible(visible);

    CCSpriteBatchNode* pIbh = (CCSpriteBatchNode*)this->getChildByTag(HERO_SPRITE_BATCH_TAG);
    pIbh->removeAllChildrenWithCleanup(true);

    CCSpriteBatchNode* pIch = (CCSpriteBatchNode*)this->getChildByTag(HERO_BOX_BATCH_TAG);
    pIch->removeAllChildrenWithCleanup(true);
    
    for (int i = 0; i < MAX_IN_BATTLE_HEROS; i++) {
        if (m_team[i]) {
            m_team[i]->removeAllObjects();
            m_team[i]->release();
            m_team[i] = NULL;
        }
    } /*for*/
    
    if (visible) {
        bool hasTeam[6];
        for (unsigned int i = 0; i < 6; i++) {
            hasTeam[i] = false;
        } /*for*/
        
        HeroOfPlayerItem* pHeroOfPlayerItem = NULL;
        for (unsigned int i = 0; i < MAX_GO_INTO_BATTLE_HEROS; i++) {
            // 获取上阵武将数据
            if (BPM_PVE == m_pBattle->getBattlePkMode()) {
                pHeroOfPlayerItem = m_pHeroManager->getGoIntoBattleHeroDataByIndex(i);
            } else {
                pHeroOfPlayerItem = m_pHeroManager->getPvpGoIntoBattleHeroDataByIndex(i);
            }
            if (NULL == pHeroOfPlayerItem)
                break;
            
            unsigned int teamIndex = 0;
            unsigned int posInBattle = pHeroOfPlayerItem->getPosInBattle();
            if (posInBattle > 0) {
                if (21 == posInBattle) {
                    teamIndex = 0;
                } else if (22 == posInBattle) {
                    teamIndex = 1;
                } else if (23 == posInBattle) {
                    teamIndex = 2;
                } else if (11 == posInBattle) {
                    teamIndex = 3;
                } else if (12 == posInBattle) {
                    teamIndex = 4;
                } else if (13 == posInBattle) {
                    teamIndex = 5;
                }
                if (!hasTeam[teamIndex]) {
                    hasTeam[teamIndex] = true;
                    _attachTeam(teamIndex, pHeroOfPlayerItem);
                } else /*位置有重复*/ {
                    pHeroOfPlayerItem->setPosInBattle(0);
                }
            }
        } /*for*/
        
        if (BPM_PVE == m_pBattle->getBattlePkMode()) {
            for (int i = 0; i < MAX_GO_INTO_BATTLE_HEROS; i++) {
                // 获取上阵武将数据
                pHeroOfPlayerItem = m_pHeroManager->getGoIntoBattleHeroDataByIndex(i);
                if (NULL == pHeroOfPlayerItem)
                    break;
                
                unsigned int posInBattle = pHeroOfPlayerItem->getPosInBattle();
                if (0 == posInBattle) {
                    if (!hasTeam[i]) /* 该位置没有战队 */ {
                        _attachTeam(i, pHeroOfPlayerItem);
                        hasTeam[i] = true;
                    } else {
                        unsigned int nTemp = i + 1;
                        while (true) {
                            if (!hasTeam[nTemp]) {
                                _attachTeam(nTemp, pHeroOfPlayerItem);
                                hasTeam[nTemp] = true;
                                break;
                            }
                            nTemp++;
                            nTemp = (nTemp == 6 ? 0 : nTemp);
                        } /*while*/
                    }
                }
            } /*for*/
        }
    
        for (int i = 0; i < 6; i++) {
            CCPoint boxPoint = m_AllPos[i * CFL_COUNT_OF_TEAM];
            boxPoint.x += 2.0f;
            boxPoint.y -= 92.0f;
            CCSprite* pBlink = CCSprite::createWithSpriteFrameName("BT_blinkK.png");
            pBlink->setPosition(boxPoint);
            pBlink->runAction(CCRepeatForever::create(CCSequence::create(CCFadeOut::create(1.0f),
                                                                         CCFadeIn::create(1.0f), NULL)));
            pIch->addChild(pBlink);
        } /*for*/
    }
}

/*
 *  上阵武将关联战队
 */
void ChosFormLayer::_attachTeam(const unsigned teamIndex, HeroOfPlayerItem* pItem) {
    CCAssert(pItem, "invalid data");
    CCSpriteBatchNode* pIbh = (CCSpriteBatchNode*)this->getChildByTag(HERO_SPRITE_BATCH_TAG);
    
    // 添加上阵武将
    CCSprite* pSprite = CCSprite::createWithSpriteFrameName(pItem->getHeroItem()->getFIcon().c_str());
    pSprite->setPosition(m_AllPos[teamIndex * CFL_COUNT_OF_TEAM]);
    // 附加武将数据（很重要）
    pSprite->setUserData((void*)pItem);
    pIbh->addChild(pSprite, DEFAULT_ZORDER);
    
    m_team[teamIndex] = CCArray::createWithCapacity(CFL_COUNT_OF_TEAM);
    m_team[teamIndex]->retain();
    m_team[teamIndex]->addObject(pSprite);
    
    // 获取职业
    unsigned int job = pItem->getHeroItem()->getJob();
    // 获取稀有度
    unsigned int rarity = pItem->getHeroItem()->getRarity();
    
    char buffer[32];
    
    // 背景框
    if (1 == rarity) {
        strcpy(buffer, "bt_whiBox.png");
    } else if (2 == rarity || 3 == rarity) {
        strcpy(buffer, "bt_greBox.png");
    } else if (4 == rarity || 5 == rarity) {
        strcpy(buffer, "bt_bluBox.png");
    } else if (6 == rarity || 7 == rarity || 8 == rarity) {
        strcpy(buffer, "bt_purBox.png");
    } else {
        strcpy(buffer, "bt_oraBox.png");
    }
    pSprite = CCSprite::createWithSpriteFrameName(buffer);
    pSprite->setPosition(m_AllPos[teamIndex * CFL_COUNT_OF_TEAM + 1]);
    pIbh->addChild(pSprite, DEFAULT_ZORDER - 1);
    m_team[teamIndex]->addObject(pSprite);
    
    // 稀有度
    if (1 == rarity) {
        strcpy(buffer, "bt_whi.png");
    } else if (2 == rarity || 3 == rarity) {
        strcpy(buffer, "bt_gre.png");
    } else if (4 == rarity || 5 == rarity) {
        strcpy(buffer, "bt_blu.png");
    } else if (6 == rarity || 7 == rarity || 8 == rarity) {
        strcpy(buffer, "bt_pur.png");
    } else {
        strcpy(buffer, "bt_ora.png");
    }
    pSprite = CCSprite::createWithSpriteFrameName(buffer);
    pSprite->setPosition(m_AllPos[teamIndex * CFL_COUNT_OF_TEAM + 2]);
    pIbh->addChild(pSprite, DEFAULT_ZORDER - 2);
    m_team[teamIndex]->addObject(pSprite);
    
    // 职业
    CCString* jobStr = CCString::createWithFormat("btJob%d.png", job);
    pSprite = CCSprite::createWithSpriteFrameName(jobStr->getCString());
    pSprite->setPosition(m_AllPos[teamIndex * CFL_COUNT_OF_TEAM + 3]);
    pIbh->addChild(pSprite, DEFAULT_ZORDER + 10);
    m_team[teamIndex]->addObject(pSprite);
    
    // 职业框
    if (1 == rarity) {
        strcpy(buffer, "LST_whiJob.png");
    } else if (2 == rarity || 3 == rarity) {
        strcpy(buffer, "LST_greJob.png");
    } else if (4 == rarity || 5 == rarity) {
        strcpy(buffer, "LST_bluJob.png");
    } else if (6 == rarity || 7 == rarity || 8 == rarity) {
        strcpy(buffer, "LST_purJob.png");
    } else {
        strcpy(buffer, "LST_oraJob.png");
    }
    
    pSprite = CCSprite::createWithSpriteFrameName(buffer);
    pSprite->setPosition(m_AllPos[teamIndex * CFL_COUNT_OF_TEAM + 4]);
    pIbh->addChild(pSprite, DEFAULT_ZORDER + 10);
    m_team[teamIndex]->addObject(pSprite);
    
    CCSprite* pCornerSprite1 = NULL, *pCornerSprite2 = NULL, *pCornerSprite3 = NULL;
    if (1 == rarity) {
        strcpy(buffer, "bt_whiJiao.png");
    } else if (2 == rarity || 3 == rarity) {
        strcpy(buffer, "bt_greJiao.png");
    } else if (4 == rarity || 5 == rarity) {
        strcpy(buffer, "bt_bluJiao.png");
    } else if (6 == rarity || 7 == rarity || 8 == rarity) {
        strcpy(buffer, "bt_purJiao.png");
    } else {
        strcpy(buffer, "bt_oraJiao.png");
    }
    
    // 边角1
    pCornerSprite1 = CCSprite::createWithSpriteFrameName(buffer);
    pCornerSprite1->setPosition(m_AllPos[teamIndex * CFL_COUNT_OF_TEAM + 5]);
    pIbh->addChild(pCornerSprite1, DEFAULT_ZORDER + 10);
    m_team[teamIndex]->addObject(pCornerSprite1);
    
    // 边角2
    pCornerSprite2 = CCSprite::createWithSpriteFrameName(buffer);
    pCornerSprite2->setFlipX(true);
    pCornerSprite2->setPosition(m_AllPos[teamIndex * CFL_COUNT_OF_TEAM + 6]);
    pIbh->addChild(pCornerSprite2, DEFAULT_ZORDER + 10);
    m_team[teamIndex]->addObject(pCornerSprite2);
    
    // 边角3
    pCornerSprite3 = CCSprite::createWithSpriteFrameName(buffer);
    pCornerSprite3->setFlipY(true);
    pCornerSprite3->setPosition(m_AllPos[teamIndex * CFL_COUNT_OF_TEAM + 7]);
    pIbh->addChild(pCornerSprite3, DEFAULT_ZORDER + 10);
    m_team[teamIndex]->addObject(pCornerSprite3);
    
    if ((rarity > 0 && rarity <= 2) || rarity == 4 || rarity == 6 || rarity == 9) {
        pCornerSprite1->setVisible(false);
        pCornerSprite2->setVisible(false);
        pCornerSprite3->setVisible(false);
    } else if (rarity == 3 || rarity == 5 || rarity == 8) {
        pCornerSprite1->setVisible(true);
        pCornerSprite2->setVisible(true);
        pCornerSprite3->setVisible(true);
    } else if (rarity == 7) {
        pCornerSprite1->setVisible(false);
        pCornerSprite2->setVisible(true);
        pCornerSprite3->setVisible(true);
    }
    
    // 添加兵士
    unsigned int countOfSoldier = m_pHeroManager->getCountOfSoldiers(rarity, job, pItem->getHP());
    for (int j = 0; j < countOfSoldier; j++) {
        CCAnimationFrame* paf = (CCAnimationFrame*)m_soldierAni[job - 1]->getFrames()->objectAtIndex(0);
        pSprite = CCSprite::createWithSpriteFrame(paf->getSpriteFrame());
        pSprite->setPosition(m_AllPos[teamIndex * CFL_COUNT_OF_TEAM + 8 + j]);
        pSprite->setRotation(180.0f);
        pSprite->runAction(CCRepeatForever::create(CCAnimate::create(m_soldierAni[job - 1])));
        pIbh->addChild(pSprite, DEFAULT_ZORDER);
        m_team[teamIndex]->addObject(pSprite);
    } /*for*/
}

/*
 * 初始化 UI 组件
 */
void ChosFormLayer::_initUiComponent() {
    /*
     * 初始化战场武将区域
     */
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("battle.plist", "battle.pvr.ccz");
    CCSpriteBatchNode* pIch = CCSpriteBatchNode::create("battle.pvr.ccz");
    this->addChild(pIch, 3, HERO_BOX_BATCH_TAG);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("inBattleHero.plist", "inBattleHero.pvr.ccz");
    CCSpriteBatchNode* pIbh = CCSpriteBatchNode::create("inBattleHero.pvr.ccz", 120);
    this->addChild(pIbh, 3, HERO_SPRITE_BATCH_TAG);

    // 添加背景图片
    CCSprite* bg = CCSprite::create("FB_bg.png");
    bg->setAnchorPoint(ccp(0, 0));
    bg->setPosition(ccp(-8, m_pGameState->getBottomOffset() + 210.0f));
    this->addChild(bg, 2);
    
    // 下部背景图片
    CCSprite* bg_x = CCSprite::create("FB_bgx.png");
    bg_x->setAnchorPoint(ccp(0, 0));
    bg_x->setPosition(ccp(0, m_pGameState->getBottomOffset() + 120.0f));
    this->addChild(bg_x);
    
    // 上部背景图片
    CCSprite* bg_s = CCSprite::create("FB_kuan.png");
    bg_s->setAnchorPoint(ccp(0, 0));
    bg_s->setPosition(ccp(0, m_pGameState->getBottomOffset() + 655.0f));
    this->addChild(bg_s, 2);
    
    // 添加返回菜单
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCNode* pLLContainer = CCNode::create();
    this->addChild(pLLContainer, 1, LL_MENU_TAG);
    CCNode* pMIDContainer = CCNode::create();
    this->addChild(pMIDContainer, 1, MID_MENU_TAG);
    CCNode* pRRContainer = CCNode::create();
    this->addChild(pRRContainer, 1, RR_MENU_TAG);
    
    // 返回按钮
    CCMenuItemImage *pReturnItem = CCMenuItemImage::create("CM_left.png",
                                                           "CM_left.png",
                                                           this,
                                                           menu_selector(ChosFormLayer::exitChosForm));
    
    CCMenu* pMenu = CCMenu::create(pReturnItem, NULL);
    pMenu->setAnchorPoint(ccp(0, 0));
    pMenu->setPosition(ccp(0, 0));
    pReturnItem->setAnchorPoint(CCPointZero);
    pReturnItem->setPosition(ccp(0, m_pGameState->getBottomOffset() + 755.0f));
    pLLContainer->addChild(pMenu, 2);
    // 返回文字
    CCSprite* returnBtn = CCSprite::create("LU_return.png");
    returnBtn->setAnchorPoint(ccp(0, 0));
    returnBtn->setPosition(ccp(15, m_pGameState->getBottomOffset() + 758.0f));
    pLLContainer->addChild(returnBtn, 2);
    
    // 初始化升级文字
    CCSprite* pMiddleImage = CCSprite::create("CM_mid.png");
    pMiddleImage->setAnchorPoint(CCPointZero);
    pMiddleImage->setPosition(ccp(origin.x + visibleSize.width / 2 - pMiddleImage->getContentSize().width / 2,
                                  m_pGameState->getBottomOffset() + 755.0f));
    pMIDContainer->addChild(pMiddleImage, 2);
    //文字
    CCSprite* pMyhImage = CCSprite::create("FB_chgForm.png");
    pMyhImage->setAnchorPoint(CCPointZero);
    pMyhImage->setPosition(ccp(origin.x + visibleSize.width / 2 - pMyhImage->getContentSize().width / 2,
                               m_pGameState->getBottomOffset() + 740.0f));
    pMIDContainer->addChild(pMyhImage, 3);
    
    // 按钮
    CCMenuItemImage *pSellItem = CCMenuItemImage::create("CM_right.png",
                                                         "CM_right.png",
                                                         this,
                                                         menu_selector(ChosFormLayer::openningAni));
    CCMenu* pSellMenu = CCMenu::create(pSellItem, NULL);
    pSellItem->setAnchorPoint(ccp(0, 0));
    pSellMenu->setAnchorPoint(CCPointZero);
    pSellMenu->setPosition(ccp(485.0f, m_pGameState->getBottomOffset() + 755.0f));
    pRRContainer->addChild(pSellMenu, 2);
    
    // 下部按钮
    CCMenuItemImage *pChosFrid = CCMenuItemImage::create( "CM_bigBtn.png",
                                                         "CM_bigBtn.png",
                                                         this,
                                                         menu_selector(ChosFormLayer::chosFrid));
    pChosFrid->setAnchorPoint(CCPointZero);
    pChosFrid->setPosition(ccp(50, 140));
    CCSprite* fontIs = CCSprite::create("FB_chosFrid.png");
    fontIs->setAnchorPoint(ccp(0, 0));
    fontIs->setPosition(ccp(75, 150 + m_pGameState->getBottomOffset()));
    this->addChild(fontIs, 10);
    
    CCMenuItemImage *pFight = CCMenuItemImage::create( "CM_bigBtn.png",
                                                      "CM_bigBtn.png",
                                                      this,
                                                      menu_selector(ChosFormLayer::goToFight));
    pFight->setAnchorPoint(CCPointZero);
    pFight->setPosition(ccp(350, 140));
    CCSprite* fontFi = CCSprite::create("FB_fight.png");
    fontFi->setAnchorPoint(ccp(0, 0));
    fontFi->setPosition(ccp(435, 150 + m_pGameState->getBottomOffset()));
    this->addChild(fontFi, 10);
    
    CCMenu* pXMenu = CCMenu::create(pChosFrid, pFight, NULL);
    pXMenu->setAnchorPoint(ccp(0, 0));
    pXMenu->setPosition(ccp(0, m_pGameState->getBottomOffset()));
    this->addChild(pXMenu);
    
    /* 阵型选择按钮 */
    CCSprite* pGan = CCSprite::create("FB_ganz.png");
    pGan->setAnchorPoint(ccp(0 ,0));
    pGan->setPosition(ccp(3, 555 + m_pGameState->getBottomOffset()));
    this->addChild(pGan, 10);
    char buffer[16];
    for (int i = 0; i < 4; ++i) {
        CCMenuItemImage *pChosForm = CCMenuItemImage::create( "FB_formnor.png",
                                                              "FB_formnor.png",
                                                           this,
                                                           menu_selector(ChosFormLayer::chosForm));
        pChosForm->setTag(CHOSFORM_BTN + i);
        pChosForm->setAnchorPoint(CCPointZero);
        pChosForm->setPosition(ccp(38 + CHOSBTN_WIDTH * i, 655.0f + m_pGameState->getBottomOffset()));
        CCMenu* pMenu = CCMenu::create(pChosForm, NULL);
        pMenu->setAnchorPoint(CCPointZero);
        pMenu->setPosition(CCPointZero);
        pMenu->setTouchEnabled(false);
        this->addChild(pMenu, 8, CHOSBTN_MENU+i);
        CCSprite* pSel = CCSprite::create("FB_forms.png");
        pSel->setAnchorPoint(CCPointZero);
        pSel->setPosition(ccp(38.0f + CHOSBTN_WIDTH * i, 640.0f + m_pGameState->getBottomOffset()));
        pSel->setVisible(false);
        addChild(pSel, 9, CHOSFORMSEL_BTN + i);
        
        sprintf(buffer, "FB_form%d.png", i + 1);
        CCSprite* spFont = CCSprite::create(buffer);
        spFont->setAnchorPoint(CCPointZero);
        spFont->setPosition(ccp(60 + CHOSBTN_WIDTH * i, 695 + m_pGameState->getBottomOffset()));
        addChild(spFont, 12);
        
        sprintf(buffer, "FB_form%dh.png", i+1);
        CCSprite* spFonth = CCSprite::create(buffer);
        spFonth->setAnchorPoint(CCPointZero);
        spFonth->setPosition(ccp(60 + CHOSBTN_WIDTH*i, 695 + m_pGameState->getBottomOffset()));
        addChild(spFonth, 12, CHOSBTN_FONT + i);
    }
    CCMenu* pMe = (CCMenu*)this->getChildByTag(CHOSBTN_MENU);
    pMe->setTouchEnabled(true);
    CCSprite* sp = (CCSprite*)this->getChildByTag(CHOSFORMSEL_BTN);
    sp->setVisible(true);/*默认第一个阵型*/
    CCSprite* btn_font = (CCSprite*)this->getChildByTag(CHOSBTN_FONT);
    btn_font->setVisible(false);
    /* 阵型效果显示 */
    CCSprite* pEffect = CCSprite::create("FB_21.png");
    pEffect->setAnchorPoint(ccp(0, 0));
    pEffect->setPosition(ccp(170, 618 + m_pGameState->getBottomOffset()));
    addChild(pEffect, 10);
}

/*
 *  开场动画
 */
void ChosFormLayer::openningAni() {
    float oldY = 0.0f;
    float oldX = 0.0f;
    // 左tipMenu
    CCNode* pLNode = (CCNode*)this->getChildByTag(LL_MENU_TAG);
    oldY = pLNode->getPosition().y;
    pLNode->setPosition(ccp(-160.0f, oldY));
    pLNode->runAction(CCMoveBy::create(OPENNING_ANI_DURING, CCPointMake(160.0f, 0.0f)));
    // 中间
    CCNode* pMNode = (CCNode*)this->getChildByTag(MID_MENU_TAG);
    oldX = pMNode->getPosition().x;
    oldY = pMNode->getPosition().y;
    pMNode->setPosition(ccp(oldX, oldY + 150.0f));
    pMNode->runAction(CCMoveBy::create(OPENNING_ANI_DURING, CCPointMake(oldX, -150.0f)));
    // 右边
    CCNode* pRNode = (CCNode*)this->getChildByTag(RR_MENU_TAG);
    oldY = pRNode->getPosition().y;
    oldX = pRNode->getPosition().x;
    pRNode->setPosition(ccp(oldX + 160.0f, oldY));
    pRNode->runAction(CCMoveBy::create(OPENNING_ANI_DURING, CCPointMake(-160.0f, 0.0f)));
}


void ChosFormLayer::moveOutTouchUICompent() {
    m_enableTouch = false;
    this->setVisible(false);
    this->removeFromParent();
    CCLog("ChosForm 从主场景移除");
}

void ChosFormLayer::restoreTouchUICompent() {
    m_enableTouch = true;
    this->setVisible(true);
}

//
void ChosFormLayer::exitChosForm() {
    if (m_pBattle->getBattlePkMode() == BPM_PVE) {
        m_pNotificationCenter->postNotification(ON_MESSAGE_ZHULU_STRING);
    } else {
        if (m_pBattle->isCWar()) {
            m_pNotificationCenter->postNotification(ON_MESSAGE_CWARRETURN_STRING);
        } else {
            m_pNotificationCenter->postNotification(ON_MESSAGE_TONGQUETAI_STRING);
        }
    }
    this->removeFromParent();
}

void ChosFormLayer::chosForm(CCObject* pSender) {
    CCMenuItemImage* pItemImage = (CCMenuItemImage*)pSender;
    for (int i = 0; i < 4; ++i) {
        CCSprite* sp = (CCSprite*)this->getChildByTag(CHOSFORMSEL_BTN+i);
        sp->setVisible(false);
    }
    switch (pItemImage->getTag() - CHOSFORM_BTN) {
        case 0:
        {
            CCSprite* sp = (CCSprite*)this->getChildByTag(CHOSFORMSEL_BTN);
            sp->setVisible(true);
        }
            break;
        case 1:
        {
            CCSprite* sp = (CCSprite*)this->getChildByTag(CHOSFORMSEL_BTN+1);
            sp->setVisible(true);
        }
            break;
        case 2:
        {
            CCSprite* sp = (CCSprite*)this->getChildByTag(CHOSFORMSEL_BTN+2);
            sp->setVisible(true);
        }
            break;
        case 3:
        {
            CCSprite* sp = (CCSprite*)this->getChildByTag(CHOSFORMSEL_BTN+3);
            sp->setVisible(true);
        }
            break;
        default:
            break;
    }
}

/*
 *  选择好友
 */
void ChosFormLayer::chosFrid() {
    CCLog("选择好友");
}

// 出征
void ChosFormLayer::goToFight() {
    if (BPM_PVE == m_pBattle->getBattlePkMode()) {
        goToFightPve();
    } else {
        if (m_pBattle->isCWar()) {
            goToFightCwar();
        } else /*铜雀台*/{
            goToFightTqt();
        }
    }
}

/*
 *  出征 pve
 */
void ChosFormLayer::goToFightPve() {
    CCLog("pve 出征");
    
    /* 清除战场所有数据 */
    m_pBattle->clearAllBattleData(true);
    m_pBattle->clearAllHerosData();
    
    unsigned int numberOfGoIntoBattleHeros = 0;                  // 本方上阵武将的数量
    unsigned int heroIndex[MAX_IN_BATTLE_HEROS];    //
    unsigned int posInBattle[] = {21,22,23,11,12,13};
    
    for (unsigned int i = 0; i < MAX_IN_BATTLE_HEROS; i++) {
        if (m_team[i]) {
            heroIndex[numberOfGoIntoBattleHeros++] = i;
        }
    } /*for*/
    CCAssert(numberOfGoIntoBattleHeros == m_pHeroManager->getGoIntoBattleHerosOfPlayerCount(), "invalid data");
    
    for (unsigned int i = 0; i < numberOfGoIntoBattleHeros; i++) {
        HeroOfPlayerItem* pHeroItem = (HeroOfPlayerItem*)((CCSprite*)m_team[heroIndex[i]]->objectAtIndex(0))->getUserData();
        CCAssert(pHeroItem, "invalid hero data");
        
        // 在战场的位置
        pHeroItem->setPosInBattle(posInBattle[heroIndex[i]]);
        //往战场添加本方上阵武将
        m_pBattle->appendInBattleHero(pHeroItem);
    }/*for*/
    // 保存战队位置
    m_pHeroManager->saveBattleTeamPosIndex();
    
    // 向服务器提交 pve 战斗请求
    m_pBattle->commitPveFightRequest();
    m_pNotificationCenter->postNotification(ON_MESSAGE_LOADING_MAINSCENE);
}

/*
 *  出征铜雀台(pvp)
 */
void ChosFormLayer::goToFightTqt() {
    CCLog("出征铜雀台");
    
    /* 清除战场所有数据 */
    m_pBattle->clearAllBattleData(true);
    m_pBattle->clearAllHerosData();
    
    unsigned int numberOfPvpGoIntoBattleHeros = 0;                  // 本方 pvp 上阵武将的数量
    unsigned int heroIndex[MAX_IN_BATTLE_HEROS];    //
    unsigned int posInBattle[] = {21,22,23,11,12,13};
    for (unsigned int i = 0; i < MAX_IN_BATTLE_HEROS; i++) {
        if (m_team[i]) {
            heroIndex[numberOfPvpGoIntoBattleHeros++] = i;
        }
    } /*for*/
    CCAssert(numberOfPvpGoIntoBattleHeros == m_pHeroManager->getPvpGoIntoBattleHerosOfPlayerCount(), "invalid data");
    
    for (unsigned int i = 0; i < numberOfPvpGoIntoBattleHeros; i++) {
        HeroOfPlayerItem* pHeroItem = (HeroOfPlayerItem*)((CCSprite*)m_team[heroIndex[i]]->objectAtIndex(0))->getUserData();
        CCAssert(pHeroItem, "invalid hero data");
        
        // 更新上阵武将在战场的位置
        pHeroItem->setPosInBattle(posInBattle[heroIndex[i]]);
        
        // 添加本方上阵武将
        m_pBattle->appendInBattleHero(pHeroItem);
    }/*for*/
    
    // 向服务器提交战斗请求
    m_pBattle->commitTqtFightRequest();
    m_pNotificationCenter->postNotification(ON_MESSAGE_LOADING_MAINSCENE);
}

/*
 *  出征国战(pvp)
 */
void ChosFormLayer::goToFightCwar() {
    CCLog("出征国战");
    
    /* 清除战场所有数据 */
    m_pBattle->clearAllBattleData(true);
    m_pBattle->clearAllHerosData();
    
    unsigned int numberOfPvpGoIntoBattleHeros = 0;                  // 本方 pvp 上阵武将的数量
    unsigned int heroIndex[MAX_IN_BATTLE_HEROS];    //
    unsigned int posInBattle[] = {21,22,23,11,12,13};
    for (unsigned int i = 0; i < MAX_IN_BATTLE_HEROS; i++) {
        if (m_team[i]) {
            heroIndex[numberOfPvpGoIntoBattleHeros++] = i;
        }
    } /*for*/
    CCAssert(numberOfPvpGoIntoBattleHeros == m_pHeroManager->getPvpGoIntoBattleHerosOfPlayerCount(), "invalid data");
    for (unsigned int i = 0; i < numberOfPvpGoIntoBattleHeros; i++) {
        HeroOfPlayerItem* pHeroItem = (HeroOfPlayerItem*)((CCSprite*)m_team[heroIndex[i]]->objectAtIndex(0))->getUserData();
        CCAssert(pHeroItem, "invalid hero data");
        
        // 更新上阵武将在战场的位置
        pHeroItem->setPosInBattle(posInBattle[heroIndex[i]]);
        
        // 添加本方上阵武将
        m_pBattle->appendInBattleHero(pHeroItem);
    }/*for*/
    
    // 清除国战获得的金钱数
    m_pBattle->setCwarAddMoney(0);
    // 向服务器提交国战战斗请求
    m_pBattle->commitCWarFightRequest();
    m_pNotificationCenter->postNotification(ON_MESSAGE_LOADING_MAINSCENE);
}

/* 
 *  加载士兵动画 
 */
void ChosFormLayer::_loadSoldierAni() {
    CCAnimation* animation = NULL;
    CCString* str = NULL;
    
    CCSpriteFrameCache* pSpriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    for (int i = 0; i < 7; i++) {
        animation = CCAnimation::create();
        if (1 == (i + 1)) /*职业1*/{
            for (int k = 1; k < 16; k++) {
                str = CCString::createWithFormat("dj_bub%02d.png", k);
                animation->addSpriteFrame(pSpriteFrameCache->spriteFrameByName(str->getCString()));
            } /*for*/
        } else if (2 == (i + 1)) {
            for (int k = 1; k <= 16; k++) {
                str = CCString::createWithFormat("dj_qiangb%02d.png", k);
                animation->addSpriteFrame(pSpriteFrameCache->spriteFrameByName(str->getCString()));
            } /*for*/
        } else if (3 == (i + 1)) {
            for (int k = 1; k <= 16; k++) {
                str = CCString::createWithFormat("dj_gongb%02d.png", k);
                animation->addSpriteFrame(pSpriteFrameCache->spriteFrameByName(str->getCString()));
            } /*for*/
        } else if (4 == (i + 1)) {
            for (int k = 1; k <= 16; k++) {
                str = CCString::createWithFormat("dj_yis%02d.png", k);
                animation->addSpriteFrame(pSpriteFrameCache->spriteFrameByName(str->getCString()));
            } /*for*/
        } else if (5 == (i + 1)) {
            for (int k = 1; k <= 16; k++) {
                str = CCString::createWithFormat("dj_mous%02d.png", k);
                animation->addSpriteFrame(pSpriteFrameCache->spriteFrameByName(str->getCString()));
            } /*for*/
        } else if (6 == (i + 1)) {
            for (int k = 1; k <= 16; k++) {
                str = CCString::createWithFormat("dj_ces%02d.png", k);
                animation->addSpriteFrame(pSpriteFrameCache->spriteFrameByName(str->getCString()));
            } /*for*/
        } else if (7 == (i + 1)) {
            for (int k = 1; k <= 16; k++) {
                str = CCString::createWithFormat("dj_duw%02d.png", k);
                animation->addSpriteFrame(pSpriteFrameCache->spriteFrameByName(str->getCString()));
            } /*for*/
        }
        
        animation->setDelayPerUnit(0.1f);
        animation->retain();
        m_soldierAni[i] = animation;
    } /*for*/
}

/*
 *  功能：
 *      调整 team zorder
 *  参数：
 *      teamIndex   队伍索引号
 *      zOrder
 */
void ChosFormLayer::_updateTeamZorder(const unsigned int teamIndex, const unsigned int zOrder) {
    CCSpriteBatchNode* pIbh = (CCSpriteBatchNode*)this->getChildByTag(HERO_SPRITE_BATCH_TAG);
    CCAssert(pIbh, "invalid data");
    CCAssert(m_team[teamIndex], "invalid team");
    
    // 武将（怪物）
    pIbh->reorderChild((CCSprite*)m_team[teamIndex]->objectAtIndex(0), zOrder);
    // 背景框
    pIbh->reorderChild((CCSprite*)m_team[teamIndex]->objectAtIndex(1), zOrder - 1);
    // 稀有度
    pIbh->reorderChild((CCSprite*)m_team[teamIndex]->objectAtIndex(2), zOrder - 2);
    // 职业
    pIbh->reorderChild((CCSprite*)m_team[teamIndex]->objectAtIndex(3), zOrder + 10);
    // 职业框
    pIbh->reorderChild((CCSprite*)m_team[teamIndex]->objectAtIndex(4), zOrder + 10);
    // 边角1
    pIbh->reorderChild((CCSprite*)m_team[teamIndex]->objectAtIndex(5), zOrder + 10);
    // 边角2
    pIbh->reorderChild((CCSprite*)m_team[teamIndex]->objectAtIndex(6), zOrder + 10);
    // 边角3
    pIbh->reorderChild((CCSprite*)m_team[teamIndex]->objectAtIndex(7), zOrder + 10);

    // 士兵
    unsigned int count = m_team[teamIndex]->count();
    for (int i = 8; i < count; i++) {
        pIbh->reorderChild((CCSprite*)m_team[teamIndex]->objectAtIndex(i), zOrder);
    } /*for*/
}
