//
//  BattleLayer.cpp
//  battle
//
//  战斗
//

#include "BattleLayer.h"
#include "BattleTeam.h"
#include "Battle.h"
#include "HeroItem.h"
#include "HeroOfPlayerItem.h"
#include "MosterItem.h"
#include "MosterInBattleItem.h"
#include "AttackDataItem.h"
#include "AttackDamageDataItem.h"
#include "BattleEvent.h"
#include "SkillItem.h"
#include "GameState.h"
#include "ProcessData.h"
#include "HeroManager.h"
#include "customMessage.h"
#include "GamePlayer.h"
#include "NetConnection.h"

#define OPENNING_ANI_DURING                 0.2
#define CONTAINER                           1300                //整个层的TAG
#define IMAGE_TOP                           "top_inbattle.png"
#define IMAGE_BOTTOM                        "bottom_inbattle.png"
#define TOP_IMAGE_OFFSET                    1035.f
#define INFOBOX_TAG                         1000                // 信息框的tag
#define STONE_TAG                           1001                // 宝石的tag
#define GOLD_TAG                            1002
#define EQUIP_TAG                           1003
#define CARD_TAG                            1004
#define BAT_ROUND_TAG                       1005
#define ROUND_TAG                           1006
#define VICTORY_TAG                         1008                // 胜利
#define DEFEAT_TAG                          1009                // 失败

#define ROUND_EFFECT_ACTION_TAG             1100                // 战斗开始前波数动画特效 action tag
#define ROUND_EFFECT_LEVG_TAG               1101                // 战斗开始前波数动画特效 分割线精灵图片 tag
#define ROUND_EFFECT_NUMBER_TAG             1102                // 战斗开始前波数动画特效 波数图片 tag
#define ROUND_EFFECT_TOTOL_TAG              1103                // 战斗开始前波数动画特效 总波数图片 tag
#define ROUND_EFFECT_BOSS_TAG               1104                // 战斗开始前波数动画特效 boss 精灵图片 tag

#define BLINKKUANG                          1105                // 1105 ~ 1120

#define BACKGROUND_IMAGE_TAG                1500                // 背景图 tag

#define SKILL_SPRITE_BATCH_TAG              650                 // 技能特效批处理精灵

#define BACKGROUND_IMAGE_SCROLL_ACTION_TAG  1600                // 背景滚动 action tag
#define BACKGROUND_IMAGE_SCROLL_DURATUION   2.0f                // 背景滚动时间(秒)

#define HERO_TAG_START                      2001                // 本方武将起始 tag 值
#define ENEMY_TAG_START                     3001                // 敌方武将（怪物）起始 tag 值

#define INIT_TEAM_Y_OFFSET                  360.0f

#define BATTLE_INIT_ANI_SPRITE_TAG          1700                // 战斗开始动画 sprite tag
#define BATTLE_INIT_ANI_ACTION_TAG          1701                // 战斗开始动画 action tag

#define BATTLE_EXIT_BTN_BG                  1705                // 退出按钮背景
#define BATTLE_EXIT_BTN_HL                  1706                // 退出按钮高亮背景

#define LEADER_PADDIN                       0.0f                // 武将离边界的间隙距离
#define HERO_HEAD_WIDTH                     160.0f              // 武将头像宽度
#define HERO_HEAD_HEIGHT                    70.0f               // 武将头像高度
#define SOLDIER_IMAGE_HEIGHT                26.0f               // 士兵图片高度
#define SOLDIER_PADDING_HERO                0.0f                // 士兵方阵离武将头像间隙距离

#define SCROLL_VERTICAL_OFFSERT             540.0f              //

// 掉落动画 action tag
#define DROP_ACTIOIN_TAG                    4000

#define TEAM_GOINTOBATTLE_DELAY             0.3f                // 队伍进场

// 士兵区域高度
#define SOLDIER_AREA_HEIGHT (SOLDIER_IMAGE_HEIGHT * 3)

// 武将、士兵区块高度
#define BLOCK_HEIGHT (SOLDIER_AREA_HEIGHT + SOLDIER_PADDING_HERO + HERO_HEAD_HEIGHT)

#define DEFAULT_ZORDER                      5                   // 默认 zorder

/*
 *  本方队伍位置索引，用于进场动画(可改变进场队伍的顺序)
 */
static unsigned int ownTeamPosIndex[6] = {21, 22, 23, 11, 12, 13};

/*
 *  构造函数
 */
BattleLayer::BattleLayer():
m_countOfWaitEvents(0)
,m_indexOfBattleEvents(0)
,m_enableTouch(false)
,m_dropItems(NULL)
,m_propDropAni(NULL)
,m_heroDropAni(NULL)
,m_equipDropAni(NULL)
,m_pBattle(Battle::getInstance())           // 关联的战场数据
,m_pGameState(GameState::getInstance())     // 游戏状态数据
,m_pSkillBatchNode(NULL)
,m_pSpriteFrameCache(CCSpriteFrameCache::sharedSpriteFrameCache())
,m_pHeroManager(HeroManager::getInstance())
,m_pDirector(CCDirector::sharedDirector())
,m_go(NULL)
,m_exit(NULL)
,m_pNotificationCenter(CCNotificationCenter::sharedNotificationCenter())
{
    m_BattleTeamsArray = CCArray::createWithCapacity(16);
    m_BattleTeamsArray->retain();
    
    m_BattleDeadTeamsArray = CCArray::createWithCapacity(8);
    m_BattleDeadTeamsArray->retain();
    
    m_battleEvents = CCArray::createWithCapacity(256);
    m_battleEvents->retain();
    
    m_dropItems = CCArray::createWithCapacity(16);
    m_dropItems->retain();
    
    // 读取战场速度
    m_pBattle->setBattleSpeed(CCUserDefault::sharedUserDefault()->getIntegerForKey(BATTLE_SPEED_KEY, 1));
    
    // 初始化战斗区块原点位置
    _initPosByPosIndex();
}

/*
 *  析构函数
 */
BattleLayer::~BattleLayer() {
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 3; j++) {
            CC_SAFE_RELEASE_NULL(m_soldierAni[i][j]);
        }
    }
    
    CC_SAFE_RELEASE_NULL(m_propDropAni);
    CC_SAFE_RELEASE_NULL(m_heroDropAni);
    CC_SAFE_RELEASE_NULL(m_equipDropAni);
    CC_SAFE_RELEASE_NULL(m_dropItems);
    
    CC_SAFE_RELEASE_NULL(m_BattleTeamsArray);
    CC_SAFE_RELEASE_NULL(m_BattleDeadTeamsArray);
    CC_SAFE_RELEASE_NULL(m_battleEvents);
    
    CCTextureCache* pTextureCache = CCTextureCache::sharedTextureCache();
    
    m_pSpriteFrameCache->removeSpriteFramesFromFile(IN_BATTLE_HERO_PLIST_FILE);
    m_pSpriteFrameCache->removeSpriteFramesFromFile(IN_BATTLE_PLIST_FILE);
    
    pTextureCache->removeTextureForKey("hjzl.pvr.ccz");
    pTextureCache->removeTextureForKey(IN_BATTLE_HERO_IMAGE_FILE);
    pTextureCache->removeTextureForKey(IN_BATTLE_IMAGE_FILE);
    
    // 保存战场速度
    CCUserDefault::sharedUserDefault()->setIntegerForKey(BATTLE_SPEED_KEY, m_pBattle->getBattleSpeed());
    CCUserDefault::sharedUserDefault()->flush();
}

/*
 *  初始化
 */
bool BattleLayer::init() {
    // super init
    if (!CCLayer::init()) {
        return false;
    }
    this->setTouchEnabled(true);
    
    // 初始化 ui 组件
    _initUiComponent();
    
    // 初始化 touch 区域
    _initTouchArea();
    
    // 加载地图背景
    loadMap();
    
    // 加载士兵动画
    _loadSoldierAni();
    
    // 加载掉落动画
    _loadDropAni();
    
    return true;
}

/*
 *  进入战斗
 */
void BattleLayer::onEnter() {
    /////////////////////////////
    // super first
    CCLayer::onEnter();
    
    NetConnection::getInstance()->setHandleNetDataDelegate(this);
    
    // 初始化本方战队
    _initOwnBattleTeam();
    
    // 初始化战场
    _initBattle();
}

/*
 *  离开战斗
 */
void BattleLayer::onExit() {
    CCLayer::onExit();
    
    NetConnection::getInstance()->setHandleNetDataDelegate(NULL);
}

//
void BattleLayer::registerWithTouchDispatcher(void) {
    m_pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, false);
}

/*
 *  调整阵型 touchBegan
 */
bool BattleLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    CCPoint touchPoint = this->convertTouchToNodeSpace(pTouch);
    /*
     *  调整战场速度
     */
    if (m_battleSpeedIcons[0]->boundingBox().containsPoint(touchPoint)) {
        _adjustBattleSpeed();
        return false;
    }
    
    /*
     *  国战退出
     */
    if (m_pBattle->isCWar() && m_exit->isVisible() && m_exit->boundingBox().containsPoint(touchPoint)) {
        CCSprite* pEBHL = (CCSprite*)m_battleCBN->getChildByTag(BATTLE_EXIT_BTN_HL);
        pEBHL->setVisible(true);
        return true;
    }
    
    if (!m_enableTouch)
        return false;
    
    if (m_go->isVisible() && m_go->boundingBox().containsPoint(touchPoint)) /* 玩家按了前进 go */ {
        // 下一拨战斗
        nextFight();
        return false;
    }
    
    CCSprite* victory = (CCSprite*)m_battleCBN->getChildByTag(VICTORY_TAG);
    if (victory->isVisible() && victory->boundingBox().containsPoint(touchPoint))/* 玩家按了胜利 victory*/ {
        victory->stopAllActions();
        victory->setVisible(false);
        m_enableTouch = false;
        
        // 战斗胜利
        victoryCallback();
        return false;
    }
    
    CCSprite* defeat = (CCSprite*)m_battleCBN->getChildByTag(DEFEAT_TAG);
    if (defeat->isVisible() && defeat->boundingBox().containsPoint(touchPoint))/* 玩家按了失败 defeat*/ {
        defeat->stopAllActions();
        defeat->setVisible(false);
        m_enableTouch = false;
        
        // 战斗失败
        defeatCallback();
        return false;
    }
    
    if (victory->isVisible() || defeat->isVisible())
        return false;
    
    for (int i = 0; i < MAX_IN_BATTLE_HEROS; i++) {
        if (m_touchArea[i].containsPoint(touchPoint)) {
            BattleTeam* pBattleTeam = _getBattleTeam(m_ownTeamPosIndexs[i]);
            if (pBattleTeam) {
                _touchBeganIndex = i;
                _touchBeganPoint = touchPoint;
                pBattleTeam->teamTouchMoveZorder();
                return true;
            }
        }
    } /*for*/
    return false;
}

/*
 *  调整阵型 touchMove
 */
void BattleLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {
    CCAssert(_touchBeganIndex < MAX_IN_BATTLE_HEROS, "invalid data");
    BattleTeam* pBattleTeam = _getBattleTeam(m_ownTeamPosIndexs[_touchBeganIndex]);
    CCAssert(pBattleTeam, "invalid battle team");
    
    CCPoint touchPoint = this->convertTouchToNodeSpace(pTouch);
    float offsetX = touchPoint.x - _touchBeganPoint.x;
    float offsetY = touchPoint.y - _touchBeganPoint.y;
    
    CCPoint areaPoint = _getPosByPosIndex(m_ownTeamPosIndexs[_touchBeganIndex]);
    pBattleTeam->updateTeamPosition(ccp(areaPoint.x + offsetX, areaPoint.y + offsetY), 1);
}

/*
 *  调整阵型 touchEnd
 */
void BattleLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {
    CCAssert(_touchBeganIndex < MAX_IN_BATTLE_HEROS, "invalid data");
    CCPoint touchPoint = this->convertTouchToNodeSpace(pTouch);
    
    /*
     *  国战退出
     */
    if (m_pBattle->isCWar()) {
        CCSprite* pEBHL = (CCSprite*)m_battleCBN->getChildByTag(BATTLE_EXIT_BTN_HL);
        pEBHL->setVisible(false);
        if (m_exit->isVisible() && m_exit->boundingBox().containsPoint(touchPoint)) {
            this->unscheduleAllSelectors();
            
            m_go->stopAllActions();
            m_go->setVisible(false);
            
            // 显示失败
            CCSprite* defeat = (CCSprite*)m_battleCBN->getChildByTag(DEFEAT_TAG);
            defeat->setScale(0.6);
            defeat->setVisible(true);
            CCActionInterval * scaleto = CCScaleTo ::create(1, 1);
            CCActionInterval * fadein = CCFadeIn::create(1);
            CCFiniteTimeAction * spawn =CCSpawn::create(scaleto,fadein,NULL);
            CCActionInterval* waves3D = CCWaves3D::create(2, CCSize(10, 10), 5, 5);
            CCFiniteTimeAction * seq = CCSequence::create(spawn,waves3D,CCCallFuncN::create(this, callfuncN_selector(BattleLayer::defeatCallback)),NULL);
            defeat->runAction(seq);
            return;
        }
    }
    
    BattleTeam* pBattleTeam = _getBattleTeam(m_ownTeamPosIndexs[_touchBeganIndex]);
    CCAssert(pBattleTeam, "invalid battle team");
    
    unsigned int touchEndedIndex = 0;       // 触摸结束位置所在阵型位置索引
    bool isNewPosIndex = false;             // 触摸结束位置是不是新的位置
    
    for (unsigned int i = 0; i < MAX_IN_BATTLE_HEROS; i++) {
        if (i != _touchBeganIndex && m_touchArea[i].containsPoint(touchPoint)) {
            isNewPosIndex = true;
            touchEndedIndex = i;
            break;
        }
    } /*for*/
    
    if (isNewPosIndex) {
        BattleTeam* pTouchEndBattleTeam = _getBattleTeam(m_ownTeamPosIndexs[touchEndedIndex]);
        if (pTouchEndBattleTeam) /* 目标位置有team */{
            pTouchEndBattleTeam->setPosIndex(m_ownTeamPosIndexs[_touchBeganIndex]);
            pTouchEndBattleTeam->updateTeamPosition(_getPosByPosIndex(m_ownTeamPosIndexs[_touchBeganIndex]), 1);
            
            HeroOfPlayerItem* pHeroOfPlayerItem = dynamic_cast<HeroOfPlayerItem*>(pTouchEndBattleTeam->getLeadObject());
            CCAssert(pHeroOfPlayerItem, "invalid team");
            pHeroOfPlayerItem->setPosInBattle(m_ownTeamPosIndexs[_touchBeganIndex]);
        }
        pBattleTeam->setPosIndex(m_ownTeamPosIndexs[touchEndedIndex]);
        pBattleTeam->updateTeamPosition(_getPosByPosIndex(m_ownTeamPosIndexs[touchEndedIndex]), 1);
        
        HeroOfPlayerItem* pHeroOfPlayerItem = dynamic_cast<HeroOfPlayerItem*>(pBattleTeam->getLeadObject());
        CCAssert(pHeroOfPlayerItem, "invalid team");
        pHeroOfPlayerItem->setPosInBattle(m_ownTeamPosIndexs[touchEndedIndex]);
        
        // 保存上阵武将战斗位置索引
        m_pHeroManager->saveBattleTeamPosIndex();
        
    } else /* 回到原来的地方 */{
        pBattleTeam->updateTeamPosition(_getPosByPosIndex(m_ownTeamPosIndexs[_touchBeganIndex]), 1);
    }
    pBattleTeam->teamDefaultZorder();
}

/*
 *  调整阵型 touchCancel
 */
void BattleLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {
    CCAssert(_touchBeganIndex < MAX_IN_BATTLE_HEROS, "invalid data");
    BattleTeam* pBattleTeam = _getBattleTeam(m_ownTeamPosIndexs[_touchBeganIndex]);
    CCAssert(pBattleTeam, "invalid battle team");
    
    // 回到原来的地方
    pBattleTeam->updateTeamPosition(_getPosByPosIndex(m_ownTeamPosIndexs[_touchBeganIndex]), 1);
    pBattleTeam->teamDefaultZorder();
}

/*
 *  自动前进调度
 */
void BattleLayer::auto_go_schedule(CCTime ccTime) {
    if (4 == m_pBattle->getBattleSpeed() && m_go->isVisible()/*防止重复提交战斗请求*/) {
        // 下一拨战斗
        nextFight();
    }
    this->unschedule(schedule_selector(BattleLayer::auto_go_schedule));
}

/*
 *  战场调度
 */
void BattleLayer::battle_schedule(CCTime ccTime) {
    if (0 == m_countOfWaitEvents) {
        // 停止调度
        this->unschedule(schedule_selector(BattleLayer::battle_schedule));
        return;
    }
    
    unsigned int countOfComplete = 0;   // 保存已经完成的事件数量
    for (int i = 0; i < m_countOfWaitEvents; i++) {
        BattleEvent* pBattleEvent = (BattleEvent*)m_battleEvents->objectAtIndex(m_indexOfBattleEvents + i);
        if (pBattleEvent->getComplete()) /*已经完成*/ {
            countOfComplete++;
        } else /* 没有完成 */ {
            BattleTeam* pCompleteBattleTeam = pBattleEvent->getBattleTeam();
            
            unsigned int action = pBattleEvent->getAction();
            if (21 == action) /* 进场 */ {
                CCAssert(pCompleteBattleTeam != NULL, "Invalid battleTeam");
                
                if (pCompleteBattleTeam->goIntoBattleIsDone()) {
                    pCompleteBattleTeam->setTeamAction(0);
                    pCompleteBattleTeam->updateSoldierAni(1);
                    
                    pBattleEvent->setComplete(true);
                    countOfComplete++;
                }
            } else if (1 == action) /*移动*/ {
                CCAssert(pCompleteBattleTeam != NULL, "Invalid battleTeam");
                
                if (pCompleteBattleTeam->getTeamAction() == 0) {
                    continue;
                }
                
                if (pCompleteBattleTeam->moveIsDone()) {
                    pCompleteBattleTeam->setTeamAction(0);
                    pCompleteBattleTeam->updateSoldierAni(1);
                    pCompleteBattleTeam->teamDefaultZorder();
                    
                    pBattleEvent->setComplete(true);
                    countOfComplete++;
                    
                    if (pBattleEvent->getUpdatePosIndex()) /*需要改变战队位置*/ {
                        pCompleteBattleTeam->setPosIndex(pBattleEvent->getMoveToPosIndex());
                        if (pBattleEvent->getRestoreDir()) {
                            if (!pCompleteBattleTeam->isEnemy() )
                                pCompleteBattleTeam->updateTeamPosition(_getPosByPosIndex(pCompleteBattleTeam->getPosIndex()), 1);
                            else
                                pCompleteBattleTeam->updateTeamPosition(_getPosByPosIndex(pCompleteBattleTeam->getPosIndex()), 3);
                        }
                    }
                }
            } else if (27 == action) {
                CCAssert(pCompleteBattleTeam != NULL, "Invalid battleTeam");
                if (pCompleteBattleTeam->GoToIsDone()) {
                    pCompleteBattleTeam->setTeamAction(0);
                    pCompleteBattleTeam->setPosIndex(pBattleEvent->getMoveToPosIndex());
                    
                    if (pBattleEvent->getRestoreDir()) {
                        if (!pCompleteBattleTeam->isEnemy() )
                            pCompleteBattleTeam->updateTeamPosition(_getPosByPosIndex(pCompleteBattleTeam->getPosIndex()), 1);
                        else
                            pCompleteBattleTeam->updateTeamPosition(_getPosByPosIndex(pCompleteBattleTeam->getPosIndex()), 3);
                    } else {
                        pCompleteBattleTeam->updateTeamPosition(_getPosByPosIndex(pCompleteBattleTeam->getPosIndex()),pCompleteBattleTeam->getTeamDir());
                    }
                    
                    pBattleEvent->setComplete(true);
                    countOfComplete++;
                }
            } else if (2 == action) /*背景滚动*/ {
                CCSprite* mapBg = (CCSprite*)this->getChildByTag(BACKGROUND_IMAGE_TAG);
                CCAction* pAction = mapBg->getActionByTag(BACKGROUND_IMAGE_SCROLL_ACTION_TAG);
                if (NULL == pAction) {
                    CCObject* pItem = NULL;
                    CCARRAY_FOREACH(m_BattleTeamsArray, pItem) {
                        BattleTeam* pBattleTeam = (BattleTeam*)pItem;
                        pBattleTeam->setTeamAction(0);
                        CCSprite* pTeamLeaderSprite = pBattleTeam->getLeaderSprite();
                        pTeamLeaderSprite->stopAllActions();
                        pBattleTeam->updateSoldierAni(1);
                    } /*for*/
                    
                    pBattleEvent->setComplete(true);
                    countOfComplete++;
                }
            } else if (3 == action || 4 == action) {
                CCAssert(pCompleteBattleTeam != NULL, "Invalid battleTeam");
                if (pCompleteBattleTeam->skillIsDone() ) {
                    pCompleteBattleTeam->clearSkill(true);
                    pCompleteBattleTeam->setTeamAction(0);
                    pBattleEvent->setComplete(true);
                    
                    countOfComplete++;
                }
            } else if (5 == action) /* 技能目标、士兵死亡或增益 */  {
                CCAssert(pCompleteBattleTeam != NULL, "Invalid battleTeam");
                if (!pCompleteBattleTeam->skillIsDone() ) {
                    continue;
                }
                
                unsigned int count = 0;
                unsigned int countOfSkillTarget = pBattleEvent->getCountOfSkillTarget();
                
                for (int i = 0; i < countOfSkillTarget; i++) {
                    // 获取技能目标战队
                    BattleTeam* pSkillTargetTeamBattleTeam = _getBattleTeam(pBattleEvent->getSkillTargetPosIndex(i));
                    if (pSkillTargetTeamBattleTeam) {
                        if (pSkillTargetTeamBattleTeam->soldierDeadAniIsDone()) {
                            pSkillTargetTeamBattleTeam->setTeamAction(0);
                            count++;
                            
                            /*
                             *  死亡的话要检查是否有掉落物品
                             */
                            if (pSkillTargetTeamBattleTeam->isDead()) {
                                // 处理物品掉落
                                _processDrop(pSkillTargetTeamBattleTeam);
                                
                                if (!pSkillTargetTeamBattleTeam->isEnemy()) /* 是本方队伍死亡 */ {
                                    if (m_pBattle->isCWar()) {
                                        m_pBattle->removeInBattleHero((HeroOfPlayerItem*)pSkillTargetTeamBattleTeam->getLeadObject());
                                        // 更新本方战队数量
                                        m_countOfInBattleHero = m_pBattle->getCountOfInBattleHero();
                                    } else {
                                        // 不是国战且不是敌方战队的话加入死亡队伍组(用于复活)
                                        m_BattleDeadTeamsArray->addObject(pSkillTargetTeamBattleTeam);
                                    }
                                }
                                // 从战队集合中中移除该战队
                                m_BattleTeamsArray->removeObject(pSkillTargetTeamBattleTeam);
                            }
                        } else
                            break;
                    } else /*已经阵亡*/{
                        count++;
                    }
                } /*for*/
                
                if (count == countOfSkillTarget) /*完成*/ {
                    pCompleteBattleTeam->clearSkill(true);
                    m_pSkillBatchNode->removeFromParent();
                    m_pSkillBatchNode = NULL;
                    pCompleteBattleTeam->setTeamAction(0);
                    for (unsigned int i = 0; i < countOfSkillTarget; i++) {
                        // 获取技能目标战队
                        BattleTeam* pSkillTargetTeamBattleTeam = _getBattleTeam(pBattleEvent->getSkillTargetPosIndex(i));
                        if (pSkillTargetTeamBattleTeam) {
                            pSkillTargetTeamBattleTeam->setTeamAction(0);
                        }
                    } /*for*/
                    /*
                     *  释放技能资源
                     */
                    pBattleEvent->getSkillItem()->releaseUI();
                    pBattleEvent->setComplete(true);
                    countOfComplete++;
                }
            } else if (11 == action) {
                CCSprite* pBattleInitSprite = (CCSprite*)m_battleCBN->getChildByTag(BATTLE_INIT_ANI_SPRITE_TAG);
                CCAssert(pBattleInitSprite, "invalid data");
                CCAction* pBattleInitAction = pBattleInitSprite->getActionByTag(BATTLE_INIT_ANI_ACTION_TAG);
                if (NULL == pBattleInitAction) /* action 已经完成 */{
                    pBattleInitSprite->removeFromParent();
                    countOfComplete++;
                }
            } else if (12 == action) /* 战斗开始前动画 */ {
                if (m_pBattle->getRoundIndex() + 1 < MAX_BATTLE_ROUNDS) {
                    CCSprite* pLevgRound = (CCSprite*)m_battleCBN->getChildByTag(ROUND_EFFECT_LEVG_TAG);
                    CCAssert(pLevgRound, "invalid data");
                    CCAction* pLevgAction = pLevgRound->getActionByTag(ROUND_EFFECT_ACTION_TAG);
                    if (pLevgAction) {
                        continue;
                    }
                    CCSprite* pRoundNumber = (CCSprite*)m_battleCBN->getChildByTag(ROUND_EFFECT_NUMBER_TAG);
                    CCAssert(pRoundNumber, "invalid data");
                    CCAction* pRoundNumberAction = pRoundNumber->getActionByTag(ROUND_EFFECT_ACTION_TAG);
                    if (pRoundNumberAction) {
                        continue;
                    }
                    CCSprite* pRoundTotolNumber = (CCSprite*)m_battleCBN->getChildByTag(ROUND_EFFECT_TOTOL_TAG);
                    CCAssert(pRoundTotolNumber, "invalid data");
                    CCAction* pRoundTotalNumberAction = pRoundTotolNumber->getActionByTag(ROUND_EFFECT_ACTION_TAG);
                    if (pRoundTotalNumberAction) {
                        continue;
                    }
                    
                    pLevgRound->removeFromParent();
                    pRoundNumber->removeFromParent();
                    pRoundTotolNumber->removeFromParent();
                    countOfComplete++;
                } else {
                    CCSprite* pBossRound = (CCSprite*)m_battleCBN->getChildByTag(ROUND_EFFECT_BOSS_TAG);
                    CCAssert(pBossRound, "invalid data");
                    CCAction* pRoundAction = pBossRound->getActionByTag(ROUND_EFFECT_ACTION_TAG);
                    if (pRoundAction) {
                        continue;
                    }
                    pBossRound->removeFromParent();
                    countOfComplete++;
                }
            }
        }
    } /*for*/
    
    do {
        CC_BREAK_IF(countOfComplete < m_countOfWaitEvents); // 还有未完成的 action
        
        // 所有等待完成的 action 都已经完成
        m_countOfWaitEvents = 0;
        
        // 更新 battle 事件索引
        m_indexOfBattleEvents += countOfComplete;
        if (m_indexOfBattleEvents == m_battleEvents->count()) /*所有事件都已经完成，当前回合结束*/{
            // 停止调度
            this->unschedule(schedule_selector(BattleLayer::battle_schedule));
            
            // 战斗结束调度
            this->schedule(schedule_selector(BattleLayer::battleFinishSchedule), 0.5f, kCCRepeatForever, 0.0f);
            break;
        }
        
        /*
         * 当前回合还有事件要处理，取出下一条要处理的战场事件
         */
        BattleEvent* pBattleEvent = (BattleEvent*)m_battleEvents->objectAtIndex(m_indexOfBattleEvents);
        
        // 下次战场调度要等待的事件数量
        unsigned int action = pBattleEvent->getAction();
        m_countOfWaitEvents = pBattleEvent->getCountOfWaitEvents();
        
        if (11 == action) {
            battleInitEffect();
            break;
        }
        if (12 == action) {
            roundEffect(m_pBattle->getRoundIndex() + 1);
            break;
        }
        if (13 == action) {
            /* 显示战斗回合数 */
            CCLabelTTF* pLaBatRound = (CCLabelTTF*)this->getChildByTag(BAT_ROUND_TAG);
            CCLabelTTF* pLaRound = (CCLabelTTF*)this->getChildByTag(ROUND_TAG);
            pLaBatRound->setString("回合数");
            char szSeq[8];
            sprintf(szSeq, "%d/30", m_pBattle->getSequenceIndex() + 1);
            pLaRound->setString(szSeq);
            m_pBattle->setSequenceIndex(m_pBattle->getSequenceIndex() + 1);
            
            pBattleEvent->setComplete(true);
            break;
        }
        
        BattleTeam* pBattleTeam = pBattleEvent->getBattleTeam();
        if (NULL == pBattleTeam) {
            pBattleTeam = _getBattleTeam(pBattleEvent->getCurrPosIndex(),
                                         pBattleEvent->getEnemy(), pBattleEvent->getAttackTag());
            CCAssert(pBattleTeam, "invalid posIndex = %d");
            pBattleEvent->setBattleTeam(pBattleTeam);
        }
        
        if (27 == action) {
            pBattleTeam->updateSoldierAni(2);
            pBattleTeam->teamTouchMoveZorder();
            pBattleTeam->teamGoTo(pBattleEvent->getMoveToPosIndex());
        } else if (3 == action) /* 技能释放 */ {
            SkillItem* pSkillItem = pBattleEvent->getSkillItem();
            CCAssert(pSkillItem, "invalid SkillItem");
            pBattleTeam->clearSkill();
            
            /* 技能批处理精灵在这里应该为 NULL */
            CCAssert(m_pSkillBatchNode == NULL, "Error");
            
            // 获取大文件名
            std::string c = pSkillItem->getC();
            std::string pvrFile = c + ".pvr.ccz";
            std::string plistFile = c + ".plist";
            m_pSkillBatchNode = CCSpriteBatchNode::create(pvrFile.c_str());
            //            CCLog("技能 pvr.ccz 文件 = %s", pvrFile.c_str());
            this->addChild(m_pSkillBatchNode, 4, SKILL_SPRITE_BATCH_TAG);
            m_pSpriteFrameCache->addSpriteFramesWithFile(plistFile.c_str(), pvrFile.c_str());
            
            if (pSkillItem->getCz() > 0) /*技能释放*/ {
                // 获取攻击目标
                unsigned int countOfSkillTarget = pBattleEvent->getCountOfSkillTarget();
                if (pSkillItem->getIs() && pBattleEvent->getSkillTargetDamageValue(0) >= 0 )  /*全屏攻击并且是伤害（不是增益）*/ {
                    for (int i = 0; i < countOfSkillTarget; i++) {
                        pBattleTeam->appendSkill(1, pBattleEvent->getSkillTargetPosIndex(i),
                                                 pSkillItem, pBattleEvent->getRemoteAttack(), pBattleEvent->getSkillTargetDamageValue(i));
                    } /*for*/
                    
                } else {
                    if (pSkillItem->getIsco()) /*是近战攻击*/ {
                        // 是否有相邻目标
                        unsigned int hasNeighbor = false;
                        // 保存相邻
                        unsigned int nNeighborPos = 0;
                        // 技能方向
                        unsigned int nDir = 0;
                        unsigned int nIndex = 0;
                        for (unsigned int i = 0; i < countOfSkillTarget; i++) {
                            nNeighborPos = pBattleEvent->getSkillTargetPosIndex(i);
                            if (m_pBattle->isNeighborPosIndex(pBattleTeam->getPosIndex(), nNeighborPos, nDir)) {
                                hasNeighbor = true;
                                nIndex = i;
                                break;
                            }
                        } /**/
                        CCAssert(hasNeighbor, "invalid data");
                        pBattleTeam->updateTeamPosition(_getPosByPosIndex(pBattleTeam->getPosIndex()), nDir);
                        
                        pBattleTeam->appendSkill(1, nNeighborPos, pSkillItem, pBattleEvent->getRemoteAttack(),
                                                 pBattleEvent->getSkillTargetDamageValue(nIndex));
                    } else {
                        pBattleTeam->appendSkill(1, pBattleEvent->getSkillTargetPosIndex(0),
                                                 pSkillItem, pBattleEvent->getRemoteAttack(), pBattleEvent->getSkillTargetDamageValue(0));
                    }
                }
            } else /*没有技能释放*/ {
                pBattleEvent->setComplete(true);
            }
        } else if (4 == action) /* 技能弹道 */{
            SkillItem* pSkillItem = pBattleEvent->getSkillItem();
            CCAssert(pSkillItem, "invalid SkillItem");
            pBattleTeam->clearSkill();
            
            if (pSkillItem->getBz() > 0) /* 有弹道特效 */ {
                unsigned int countOfSkillTarget = pBattleEvent->getCountOfSkillTarget();
                for (int i = 0; i < countOfSkillTarget; i++) {
                    pBattleTeam->appendSkill(2, pBattleEvent->getSkillTargetPosIndex(i), pSkillItem);
                } /*for*/
            } else /* 没有弹道特效*/{
                pBattleEvent->setComplete(true);
            }
        } else if (5 == action) /* 技能目标 */{
            SkillItem* pSkillItem = pBattleEvent->getSkillItem();
            CCAssert(pSkillItem, "invalid SkillItem");
            pBattleTeam->clearSkill();
            
            unsigned int countOfSkillTarget = pBattleEvent->getCountOfSkillTarget();
            for (int i = 0; i < countOfSkillTarget; i++) {
                pBattleTeam->appendSkill(3, pBattleEvent->getSkillTargetPosIndex(i),
                                         pSkillItem, false, pBattleEvent->getSkillTargetDamageValue(i),
                                         pBattleEvent->getSkillTargetRemainHP(i));
            } /*for*/
        }
    } while (0);
}

/*
 * 战斗结束调度
 */
void BattleLayer::battleFinishSchedule(CCTime ccTime) {
    /*
     *  检查是否还有物品掉落动画播放中
     */
    CCObject* pDropObject = NULL;
    CCARRAY_FOREACH(m_dropItems, pDropObject) {
        CCSprite* pDropSprite = (CCSprite*)pDropObject;
        CCAction* pAction = (CCAction*)pDropSprite->getActionByTag(DROP_ACTIOIN_TAG);
        if (pAction && !pAction->isDone()) {
            return;
        }
    } /*for*/
    
    /*
     *  如果是 pvp 模式需要保存敌方队长
     */
    if (m_pBattle->getBattlePkMode() == BPM_PVP) {
        //        void Battle::appendInBattleEnemyPvp(InBattleItem* pEnemyItem) {
        
    }
    
    // 清除战场事件
    m_battleEvents->removeAllObjects();
    // 清除所有攻击数据
    m_pBattle->clearAllAttackData();
    // 清除所有敌人数据
    m_pBattle->clearAllEnemyData();

    m_indexOfBattleEvents = 0;
    m_countOfWaitEvents = 0;
    
    m_enableTouch = true;
    restoreTouchUICompent();
    
    if (m_pBattle->getWin()) /* 战斗胜利 ✌️ */ {
        // 本方武将归位
        _resetTeamPos();
        
        if (BPM_PVE == m_pBattle->getBattlePkMode()) {
            if (m_pBattle->getRoundIndex() == MAX_BATTLE_ROUNDS - 1) {
                CCSprite* victory = (CCSprite*)m_battleCBN->getChildByTag(VICTORY_TAG);
                    
                victory->setScale(0.6f);
                victory->setVisible(true);
                CCActionInterval * scaleto = CCScaleTo ::create(1.0f, 1.0f);
                CCActionInterval * fadein = CCFadeIn::create(1);
                CCFiniteTimeAction * spawn =CCSpawn::create(scaleto,fadein,NULL);
                CCActionInterval* waves3D = CCWaves3D::create(3, CCSize(10, 10), 5, 5);
                CCFiniteTimeAction * seq = CCSequence::create(spawn, waves3D,
                    CCCallFuncN::create(this, callfuncN_selector(BattleLayer::victoryCallback)), NULL);
                victory->runAction(seq);
            } else /* 还有下一波进攻 */ {
                /* 显示战斗波数 */
                CCLabelTTF* pLaBatRound = (CCLabelTTF*)this->getChildByTag(BAT_ROUND_TAG);
                CCLabelTTF* pLaRound = (CCLabelTTF*)this->getChildByTag(ROUND_TAG);
                pLaBatRound->setString("波  数");
                char szRound[8];
                sprintf(szRound, "%d/3", m_pBattle->getRoundIndex() + 1);
                pLaRound->setString(szRound);
                
                // 递增战斗波数
                m_pBattle->setRoundIndex(m_pBattle->getRoundIndex() + 1);
                
                // 显示 go 前进 图标
                m_go->setVisible(true);
                m_go->runAction(CCRepeatForever::create(CCSequence::create(CCFadeOut::create(1.0f),
                                                                           CCFadeIn::create(1.0f), NULL)));
                
                /* 是否自动往前 */
                if (m_pBattle->getBattleSpeed() == 4) {
                    this->schedule(schedule_selector(BattleLayer::auto_go_schedule), 1.0f, kCCRepeatForever, AUTO_DELAY);
                }
                
                /* 等待下一波时可以调整阵型 */
                for (int i = 0; i < 2; ++i) {
                    for (int j = 0; j < 3; ++j) {
                        CCSprite* blinkK = (CCSprite*)m_battleCBN->getChildByTag(BLINKKUANG + 3 * i + j);
                        blinkK->setVisible(true);
                        blinkK->runAction(CCRepeatForever::create(CCSequence::create(CCFadeOut::create(1.0f),
                                                                                     CCFadeIn::create(1.0f), NULL)));
                    } /*for*/
                } /*for*/
            }
        } else /*pvp*/ {
            if (m_pBattle->isCWar()) /*国战*/ {
                /* 显示战斗波数 */
                CCLabelTTF* pLaBatRound = (CCLabelTTF*)this->getChildByTag(BAT_ROUND_TAG);
                CCLabelTTF* pLaRound = (CCLabelTTF*)this->getChildByTag(ROUND_TAG);
                pLaBatRound->setString("波  数");
                char szRound[8];
                sprintf(szRound, "%d/3", m_pBattle->getRoundIndex() + 1);
                pLaRound->setString(szRound);
                
                // 递增战斗波数
                m_pBattle->setRoundIndex(m_pBattle->getRoundIndex() + 1);
                
                // 显示 go 前进 图标
                m_go->setVisible(true);
                m_go->runAction(CCRepeatForever::create(CCSequence::create(CCFadeOut::create(1.0f),
                                                                           CCFadeIn::create(1.0f), NULL)));
                m_exit->setVisible(true);
                CCSprite* pEBG = (CCSprite*)m_battleCBN->getChildByTag(BATTLE_EXIT_BTN_BG);
                pEBG->setVisible(true);
                
                /* 是否自动往前 */
                if (m_pBattle->getBattleSpeed() == 4) {
                    this->schedule(schedule_selector(BattleLayer::auto_go_schedule), 1.0f, kCCRepeatForever, AUTO_DELAY);
                }
                
                /* 等待下一波时可以调整阵型 */
                for (int i = 0; i < 2; ++i) {
                    for (int j = 0; j < 3; ++j) {
                        CCSprite* blinkK = (CCSprite*)m_battleCBN->getChildByTag(BLINKKUANG + 3 * i + j);
                        blinkK->setVisible(true);
                        blinkK->runAction(CCRepeatForever::create(CCSequence::create(CCFadeOut::create(1.0f),
                                                                                     CCFadeIn::create(1.0f), NULL)));
                    } /*for*/
                } /*for*/
            } else /* 铜雀台 */ {
                /* 显示胜利 */
                CCSprite* victory = (CCSprite*)m_battleCBN->getChildByTag(VICTORY_TAG);
                victory->setScale(0.6f);
                victory->setVisible(true);
                CCActionInterval * scaleto = CCScaleTo ::create(1.0f, 1.0f);
                CCActionInterval * fadein = CCFadeIn::create(1);
                CCFiniteTimeAction * spawn =CCSpawn::create(scaleto,fadein,NULL);
                CCActionInterval* waves3D = CCWaves3D::create(3, CCSize(10, 10), 5, 5);
                CCFiniteTimeAction * seq = CCSequence::create(spawn, waves3D,
                    CCCallFuncN::create(this, callfuncN_selector(BattleLayer::victoryCallback)), NULL);
                victory->runAction(seq);
            }
        }
    } else /*战斗失败*/{
        CCSprite* defeat = (CCSprite*)m_battleCBN->getChildByTag(DEFEAT_TAG);
        defeat->setScale(0.6);
        defeat->setVisible(true);
        CCActionInterval * scaleto = CCScaleTo ::create(1, 1);
        CCActionInterval * fadein = CCFadeIn::create(1);
        CCFiniteTimeAction * spawn =CCSpawn::create(scaleto,fadein,NULL);
        CCActionInterval* waves3D = CCWaves3D::create(2, CCSize(10, 10), 5, 5);
        CCFiniteTimeAction * seq = CCSequence::create(spawn,waves3D,CCCallFuncN::create(this, callfuncN_selector(BattleLayer::defeatCallback)),NULL);
        defeat->runAction(seq);
    }
    this->unschedule(schedule_selector(BattleLayer::battleFinishSchedule));
}

/*
 *  处理物品掉落
 */
void BattleLayer::_processDrop(const BattleTeam* pSkillTargetTeamBattleTeam) {
    CCAssert(pSkillTargetTeamBattleTeam, "invalid team");
    
    std::string dropType = "";
    unsigned int countOfDrops = 0;
    do {
        CC_BREAK_IF(!pSkillTargetTeamBattleTeam->isDead());     // 没有死亡
        CC_BREAK_IF(!pSkillTargetTeamBattleTeam->isEnemy());    // 不是敌方战队

        InBattleItem* pInBattleItem = pSkillTargetTeamBattleTeam->getLeadObject();
        CC_BREAK_IF(!pInBattleItem->getHasDrop());              // 没有掉落物品

        dropType = pInBattleItem->getDropItemType();
        countOfDrops = pInBattleItem->getCountOfdropItem();
        
        /*播放掉落动画*/
        CCAnimate* pDropAnimate = NULL;
        CCSprite* pDropSprite = NULL;
        if (dropType.compare("hero") == 0) /*掉落武将*/ {
            pDropAnimate = CCAnimate::create(m_heroDropAni);
            pDropSprite = CCSprite::createWithSpriteFrame(((CCAnimationFrame*)m_heroDropAni->getFrames()->objectAtIndex(0))->getSpriteFrame());
            
            // 更新掉落武将数量
            char szString[16];
            m_pBattle->setCountOfDropHeroCards(m_pBattle->getCountOfDropHeroCards() + countOfDrops);
            sprintf(szString, "%02d", m_pBattle->getCountOfDropHeroCards());
            CCLabelTTF* cardLabel = (CCLabelTTF*)this->getChildByTag(CARD_TAG);
            cardLabel->setString(szString);
        } else if (dropType.compare("prop") == 0) /*掉落道具*/ {
            pDropAnimate = CCAnimate::create(m_propDropAni);
            pDropSprite = CCSprite::createWithSpriteFrame(((CCAnimationFrame*)m_propDropAni->getFrames()->objectAtIndex(0))->getSpriteFrame());
            
            // 更新掉落道具数量
            char szString[16];
            m_pBattle->setCountOfDropProps(m_pBattle->getCountOfDropProps() + countOfDrops);
            sprintf(szString, "%02d", m_pBattle->getCountOfDropProps());
            CCLabelTTF* stoneLabel = (CCLabelTTF*)this->getChildByTag(STONE_TAG);
            stoneLabel->setString(szString);
        } else /*掉落装备*/ {
            pDropAnimate = CCAnimate::create(m_equipDropAni);
            pDropSprite = CCSprite::createWithSpriteFrame(((CCAnimationFrame*)m_equipDropAni->getFrames()->objectAtIndex(0))->getSpriteFrame());
            
            // 更新掉落装备数量
            char szString[16];
            m_pBattle->setCountOfDropEquips(m_pBattle->getCountOfDropEquips() + countOfDrops);
            sprintf(szString, "%02d", m_pBattle->getCountOfDropEquips());
            CCLabelTTF* equipLabel = (CCLabelTTF*)this->getChildByTag(EQUIP_TAG);
            equipLabel->setString(szString);
        }
        
        CCPoint dropPosition = _getPosByPosIndex(pSkillTargetTeamBattleTeam->getPosIndex());
        dropPosition.x += LEADER_PADDIN + HERO_HEAD_WIDTH / 2.0f;
        dropPosition.y += BLOCK_HEIGHT / 2.0f;
        pDropSprite->setPosition(dropPosition);
        CCSequence* pDropSequence = CCSequence::create(pDropAnimate,
            CCCallFuncND::create(this, callfuncND_selector(BattleLayer::dropDoneCallback), (void*)pDropSprite), NULL);
        CCSpeed* pDropSpeed = CCSpeed::create(pDropSequence, m_pBattle->getBattleSpeed() >= 3 ? 3 : m_pBattle->getBattleSpeed());
        pDropSpeed->setTag(DROP_ACTIOIN_TAG);
        pDropSprite->runAction(pDropSpeed);
        m_battleCBN->addChild(pDropSprite, 102);
        
        // 加入掉落动画组
        m_dropItems->addObject(pDropSprite);
    } while (0);
}

/*
 *  功能：
 *      根据当前战场速度显示对应图像
 *  参数：
 *      battleSpeed     战场速度
 */
void  BattleLayer::_updateBattleSpeedIcon(const unsigned int nBattleSpeed) {
    CCAssert(nBattleSpeed > 0 && nBattleSpeed < 5, "invalid battleSpeed");
    for (unsigned int i = 0; i < 4; i++) {
        m_battleSpeedIcons[i]->setVisible(false);
    } /*for*/
    
    m_battleSpeedIcons[nBattleSpeed - 1]->setVisible(true);
}

/*
 *  调整当前战场速度
 */
void BattleLayer::_adjustBattleSpeed() {
    int nBattleSpeed = (4 == m_pBattle->getBattleSpeed() ? 1 : m_pBattle->getBattleSpeed() + 1);
    m_pBattle->setBattleSpeed(nBattleSpeed);
    _updateBattleSpeedIcon(nBattleSpeed);
    
    if (4 == nBattleSpeed && m_go->isVisible()) {
        // 下一拨战斗
        nextFight();
    }
}

/*
 *  功能：
 *      根据副本和 加载相应的背景图
 */
void BattleLayer::loadMap() {
    CCSprite* mapBg = CCSprite::create("hjzl.pvr.ccz");
    mapBg->setAnchorPoint(ccp(0, 0));
    mapBg->setPosition(ccp(0, m_pGameState->getBottomOffset() - 5.0f));
    addChild(mapBg, 1, BACKGROUND_IMAGE_TAG);
    
    //    CCSprite* mapBg1 = CCSprite::create("bg.png");
    //    mapBg1->setAnchorPoint(ccp(0, 0));
    //    mapBg1->setPosition(ccp(0, m_pGameState->getBottomOffset()));
    //    addChild(mapBg1, 2);
    
}

/*
 * 初始化 UI 组件
 */
void BattleLayer::_initUiComponent() {
    // 所有武将、怪物、兵士
    m_teamCBN = CCSpriteBatchNode::create(IN_BATTLE_HERO_IMAGE_FILE, 200);
    this->addChild(m_teamCBN, 3);
    
    // 战场相关图片资源
    m_battleCBN = CCSpriteBatchNode::create(IN_BATTLE_IMAGE_FILE);
    this->addChild(m_battleCBN, 4);
    
    m_pSpriteFrameCache->addSpriteFramesWithFile(IN_BATTLE_HERO_PLIST_FILE, IN_BATTLE_HERO_IMAGE_FILE);
    m_pSpriteFrameCache->addSpriteFramesWithFile(IN_BATTLE_PLIST_FILE, IN_BATTLE_IMAGE_FILE);
    
    if (m_pGameState->is1136()) /*1136 分辨率*/ {
        // 显示在顶部的图片
        CCSprite *pTopSprite = CCSprite::createWithSpriteFrameName(IMAGE_TOP);
        pTopSprite->setAnchorPoint(CCPointZero);
        pTopSprite->setPosition(ccp(0.0f, 1136.0f - pTopSprite->getContentSize().height));
        m_battleCBN->addChild(pTopSprite, 10);
        
        // 显示在下边的图片
        CCSprite *pBottomSprite = CCSprite::createWithSpriteFrameName(IMAGE_BOTTOM);
        pBottomSprite->setAnchorPoint(CCPointZero);
        pBottomSprite->setPosition(CCPointZero);
        m_battleCBN->addChild(pBottomSprite, 10);
    }
    
    if (m_pBattle->isCWar() && m_pBattle->getBattlePkMode() == BPM_PVP) {
        m_exit = CCSprite::createWithSpriteFrameName("battleExit.png");
        m_exit->setPosition(ccp(m_pDirector->getVisibleSize().width - m_exit->getContentSize().width / 2.0f - 8.0f,
                                m_exit->getContentSize().height / 2.0f + 8.0f));
        m_battleCBN->addChild(m_exit, 13);
        m_exit->setVisible(false);

        CCSprite* pEBG = CCSprite::createWithSpriteFrameName("battleExitBg.png");
        pEBG->setPosition(ccp(m_pDirector->getVisibleSize().width - pEBG->getContentSize().width / 2.0f,
                                pEBG->getContentSize().height / 2.0f));
        m_battleCBN->addChild(pEBG, 11, BATTLE_EXIT_BTN_BG);
        pEBG->setVisible(false);
        CCSprite* pEBHL = CCSprite::createWithSpriteFrameName("battleExitHLBg.png");
        pEBHL->setPosition(ccp(m_pDirector->getVisibleSize().width - pEBHL->getContentSize().width / 2.0f,
                              pEBHL->getContentSize().height / 2.0f));
        m_battleCBN->addChild(pEBHL, 12, BATTLE_EXIT_BTN_HL);
        pEBHL->setVisible(false);
    }
    
    // go 前进
    m_go = CCSprite::createWithSpriteFrameName("BT_go.png");
    m_go->setPosition(ccp(m_pDirector->getVisibleSize().width / 2.0f,
                        m_pDirector->getVisibleSize().height / 2.0f + 35.0f));
    m_go->setVisible(false);
    m_battleCBN->addChild(m_go, 11);
    
    /* 调整阵型提示框 */
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 3; ++j) {
            CCSprite* blinkK = CCSprite::createWithSpriteFrameName("BT_blinkK.png");
            blinkK->setAnchorPoint(ccp(0, 0));
            blinkK->setPosition(ccp(37 + j * 200, 40 + i * 165 + m_pGameState->getBottomOffset()));
            blinkK->setVisible(false);
            m_battleCBN->addChild(blinkK, 12, BLINKKUANG + 3 * i + j);
        }
    }
    
    // 胜利
    CCSprite* victory = CCSprite::createWithSpriteFrameName("BT_victory.png");
    victory->setPosition(ccp(m_pDirector->getVisibleSize().width / 2.0f,
                             m_pDirector->getVisibleSize().height / 2.0f + 35.0f));
    victory->setOpacity(0);
    victory->setVisible(false);
    m_battleCBN->addChild(victory, 11, VICTORY_TAG);
    
    // 失败
    CCSprite* defeat = CCSprite::createWithSpriteFrameName("BT_defeat.png");
    defeat->setPosition(ccp(m_pDirector->getVisibleSize().width / 2.0f,
                            m_pDirector->getVisibleSize().height / 2.0f));
    defeat->setVisible(false);
    defeat->setOpacity(0);
    m_battleCBN->addChild(defeat, 11, DEFEAT_TAG);
    
    // 信息框
    const float infoBoxX = 0.0f, infoBoxY = m_pGameState->getBottomOffset() + 862.0f;
    CCSprite* infoBox = CCSprite::createWithSpriteFrameName("BT_info.png");
    infoBox->setAnchorPoint(ccp(0.0f, 0.0f));
    infoBox->setPosition(ccp(infoBoxX, infoBoxY));
    m_battleCBN->addChild(infoBox, 10, INFOBOX_TAG);
    
    // card 卡牌
    CCSprite* card = CCSprite::createWithSpriteFrameName("BT_card.png");
    card->setAnchorPoint(ccp(0.0f, 0.0f));
    card->setPosition(ccp(infoBoxX + 40.0f, infoBoxY + 25.0f));
    m_battleCBN->addChild(card, 11);
    CCLabelTTF* cardLabel = CCLabelTTF::create("00", "Arial", 25);
    cardLabel->setAnchorPoint(ccp(0.0f, 0.0f));
    cardLabel->setPosition(ccp(infoBoxX + 90.0f, infoBoxY + 30.0f));
    this->addChild(cardLabel, 5, CARD_TAG);
    
    // equip 装备
    CCSprite* equip = CCSprite::createWithSpriteFrameName("BT_equip.png");
    equip->setAnchorPoint(ccp(0.0f, 0.0f));
    equip->setPosition(ccp(infoBoxX + 140.0f, infoBoxY + 25.0f));
    m_battleCBN->addChild(equip, 11);
    CCLabelTTF* equipLabel = CCLabelTTF::create("00", "Arial", 25);
    equipLabel->setAnchorPoint(ccp(0.0f, 0.0f));
    equipLabel->setPosition(ccp(infoBoxX + 190.0f, infoBoxY + 30.0f));
    this->addChild(equipLabel, 5, EQUIP_TAG);
    
    // stone 宝石
    CCSprite* stone = CCSprite::createWithSpriteFrameName("BT_stone.png");
    stone->setAnchorPoint(ccp(0.0f, 0.0f));
    stone->setPosition(ccp(infoBoxX + 230.0f, infoBoxY + 25.0f));
    m_battleCBN->addChild(stone, 11);
    CCLabelTTF* stoneLabel = CCLabelTTF::create("00", "Arial", 25);
    stoneLabel->setAnchorPoint(ccp(0.0f, 0.0f));
    stoneLabel->setPosition(ccp(infoBoxX + 280.0f, infoBoxY + 30.0f));
    this->addChild(stoneLabel, 5, STONE_TAG);
    
    // gold 金钱
    CCSprite* gold = CCSprite::createWithSpriteFrameName("BT_gold.png");
    gold->setAnchorPoint(ccp(0.0f, 0.0f));
    gold->setPosition(ccp(infoBoxX + 320.0f, infoBoxY+ 30.0f));
    m_battleCBN->addChild(gold, 11);
    CCLabelTTF* goldLabel = CCLabelTTF::create("99999", "Arial", 25);
    goldLabel->setAnchorPoint(ccp(0.0f, 0.0f));
    goldLabel->setPosition(ccp(infoBoxX + 360.0f, infoBoxY + 30.0f));
    this->addChild(goldLabel, 5, GOLD_TAG);
    
    // 波数、回合数
    CCLabelTTF* batRound = CCLabelTTF::create("波  数", "Arial", 30);
    batRound->setAnchorPoint(ccp(0.0f, 0.0f));
    batRound->setPosition(ccp(infoBoxX + 450.0f, infoBoxY + 30.0f));
    this->addChild(batRound, 5, BAT_ROUND_TAG);
    CCLabelTTF* roundValue = CCLabelTTF::create("0/3", "Arial", 25);
    roundValue->setAnchorPoint(ccp(0.0f, 0.0f));
    roundValue->setPosition(ccp(infoBoxX + 550.0f, infoBoxY + 30.0f));
    this->addChild(roundValue, 5, ROUND_TAG);
    
    // 战场速度
    m_battleSpeedIcons[0] = CCSprite::createWithSpriteFrameName("BT_x1.png");
    m_battleSpeedIcons[1] = CCSprite::createWithSpriteFrameName("BT_x2.png");
    m_battleSpeedIcons[2] = CCSprite::createWithSpriteFrameName("BT_x3.png");
    m_battleSpeedIcons[3] = CCSprite::createWithSpriteFrameName("BT_auto.png");
    for (unsigned int i = 0; i < 4; i++) {
        m_battleSpeedIcons[i]->setAnchorPoint(CCPointZero);
        m_battleSpeedIcons[i]->setPosition(ccp(13.0f, 3.0f + m_pGameState->getBottomOffset()));
        m_battleCBN->addChild(m_battleSpeedIcons[i], 20);
    } /*for*/
    _updateBattleSpeedIcon(m_pBattle->getBattleSpeed());
}

/*
 * 初始化 Touch 区域
 */
void BattleLayer::_initTouchArea() {
    // 本方队伍位置索引
    m_ownTeamPosIndexs[0] = 11;
    m_ownTeamPosIndexs[1] = 12;
    m_ownTeamPosIndexs[2] = 13;
    m_ownTeamPosIndexs[3] = 21;
    m_ownTeamPosIndexs[4] = 22;
    m_ownTeamPosIndexs[5] = 23;
    
    CCPoint areaPoint;
    for (int i = 0; i < MAX_IN_BATTLE_HEROS; i++) {
        areaPoint = _getPosByPosIndex(m_ownTeamPosIndexs[i]);
        m_touchArea[i] = CCRectMake(areaPoint.x, areaPoint.y, LEADER_PADDIN * 2 + HERO_HEAD_WIDTH, BLOCK_HEIGHT);
    } /*for*/
}

/*
 *  功能：
 *      初始化本方战队
 */
void BattleLayer::_initOwnBattleTeam() {
    // 获取本方上阵武将数量
    m_countOfInBattleHero = m_pBattle->getCountOfInBattleHero();
    CCAssert(m_countOfInBattleHero > 0 && m_countOfInBattleHero <= MAX_IN_BATTLE_HEROS, "invalid data");
    
    HeroOfPlayerItem* pHeroOfPlayerItem = NULL;
    for (unsigned int i = 0; i < m_countOfInBattleHero; i++) {
        pHeroOfPlayerItem = m_pBattle->getHeroDataByIndex(i);
        
        /*
         *  生成本方战斗队伍
         */
        BattleTeam* pBattleTeam = BattleTeam::create();
        
        pBattleTeam->setBattleLayer(this);
        pBattleTeam->setEnemy(false);
        pBattleTeam->setPosIndex(pHeroOfPlayerItem->getPosInBattle());
        pBattleTeam->setLeaderObject(pHeroOfPlayerItem, 2);
        
        // 添加战队
        m_BattleTeamsArray->addObject((CCObject*)pBattleTeam);
    } /*for*/
}

/*
 *      初始化战场
 */
void BattleLayer::_initBattle() {
    moveOutTouchUICompent();
    
    // 是否第一波
    bool isFirstRound = (m_pBattle->getRoundIndex() == 0);
    CCObject* pForEachItem = NULL;
    
    /* 显示战斗波数 */
    CCLabelTTF* pLaBatRound = (CCLabelTTF*)this->getChildByTag(BAT_ROUND_TAG);
    CCLabelTTF* pLaRound = (CCLabelTTF*)this->getChildByTag(ROUND_TAG);
    pLaBatRound->setString("波  数");
    char szRound[8];
    sprintf(szRound, "%d/3", m_pBattle->getRoundIndex() + 1);
    pLaRound->setString(szRound);
    
    /*
     *  初始化敌方战队
     */
    if (m_pBattle->getBattlePkMode() == BPM_PVE) {
        // 获取敌方怪物数量 (pve)
        m_countOfInBattleEnemy = m_pBattle->getCountOfInBattleEnemyMoster();
    }
    else {
        // 获取敌方战队数量 (pvp)
        m_countOfInBattleEnemy = m_pBattle->getCountOfInBattleEnemyHero();
    }
    CCAssert(m_countOfInBattleEnemy > 0 && m_countOfInBattleEnemy <= MAX_IN_BATTLE_HEROS, "invalid data");
    for (unsigned int i = 0; i < m_countOfInBattleEnemy; i++) {
        InBattleItem* pInBattleItem = NULL;
        if (m_pBattle->getBattlePkMode() == BPM_PVE) {
            pInBattleItem = m_pBattle->getEnemyMosterDataByIndex(i);
        } else {
            pInBattleItem = m_pBattle->getEnemyDataByIndex(i);
        }
        
        // 生成敌方战斗队伍
        BattleTeam* pBattleTeam = BattleTeam::create();
        pBattleTeam->setBattleLayer(this);
        pBattleTeam->setEnemy(true);
        pBattleTeam->setPosIndex(pInBattleItem->getPosInBattle());
        pBattleTeam->setLeaderObject(pInBattleItem, 2);
        
        // 添加战队
        m_BattleTeamsArray->addObject((CCObject*)pBattleTeam);
    } /*for*/
    
    if (isFirstRound) {
        /*
         * 本方队伍进场动画
         */
        unsigned int countOfAlreadyGoIntoBattleTeam = 0;
        unsigned int indexOfGoIntoBattlePosIndex = 0;
        while (countOfAlreadyGoIntoBattleTeam < m_countOfInBattleHero) {
            BattleTeam* pGoIntoBattleTeam = NULL;
            for (unsigned int i = indexOfGoIntoBattlePosIndex; i < 6; i++) {
                pGoIntoBattleTeam = _getBattleTeam(ownTeamPosIndex[i]);
                if (pGoIntoBattleTeam) {
                    indexOfGoIntoBattlePosIndex = i + 1;
                    break;
                }
            } /**/
            CCAssert(pGoIntoBattleTeam, "invalid data");
            HeroOfPlayerItem* pHeroOfPlayerItem = (HeroOfPlayerItem*)pGoIntoBattleTeam->getLeadObject();
            pGoIntoBattleTeam->updateTeamPosition(_getPosByPosIndex(pHeroOfPlayerItem->getPosInBattle()), 1, -INIT_TEAM_Y_OFFSET);
            pGoIntoBattleTeam->teamGoIntoBattle(INIT_TEAM_Y_OFFSET, countOfAlreadyGoIntoBattleTeam * TEAM_GOINTOBATTLE_DELAY);
            
            BattleEvent* pGoIntoBattleBattleEvent = BattleEvent::create();
            pGoIntoBattleBattleEvent->setAction(21);
            pGoIntoBattleBattleEvent->setBattleTeam(pGoIntoBattleTeam);
            // 添加战场事件
            m_battleEvents->addObject((CCObject*)pGoIntoBattleBattleEvent);
            
            countOfAlreadyGoIntoBattleTeam++;
        } /*while*/
    } else {
        BattleEvent* pBattleEvent = BattleEvent::create();
        pBattleEvent->setAction(2);
        // 背景图滚动
        m_battleEvents->addObject((CCObject*)pBattleEvent);
        
        float duration = JUMP_DURATION_PER_UNIT * SCROLL_VERTICAL_OFFSERT;
        CCSprite* mapBg = (CCSprite*)this->getChildByTag(BACKGROUND_IMAGE_TAG);
        CCMoveBy* pMoveBy = CCMoveBy::create(duration, ccp(0.0f, -SCROLL_VERTICAL_OFFSERT));
        CCSpeed* pSpeed = CCSpeed::create(pMoveBy, m_pBattle->getBattleSpeed() >= 3 ? 3 : m_pBattle->getBattleSpeed());
        pSpeed->setTag(BACKGROUND_IMAGE_SCROLL_ACTION_TAG);
        mapBg->runAction(pSpeed);
        
        pForEachItem = NULL;
        CCARRAY_FOREACH(m_BattleTeamsArray, pForEachItem) {
            BattleTeam* pBattleTeam = (BattleTeam*)pForEachItem;
            if (!pBattleTeam->isEnemy()) {
                pBattleTeam->setTeamAction(1);
                pBattleTeam->updateTeamPosition(_getPosByPosIndex(pBattleTeam->getPosIndex()), 1);
                pBattleTeam->teamMove(2, SCROLL_VERTICAL_OFFSERT, true, false);
            }
        } /*for*/
    }
    
    pForEachItem = NULL;
    CCARRAY_FOREACH(m_BattleTeamsArray, pForEachItem) {
        BattleTeam* pBattleTeam = (BattleTeam*)pForEachItem;
        if (pBattleTeam->isEnemy()) {
            if (isFirstRound) /* 第一波 */ {
                pBattleTeam->updateTeamPosition(_getPosByPosIndex(pBattleTeam->getPosIndex()), 3, INIT_TEAM_Y_OFFSET);
                pBattleTeam->teamMove(2, -INIT_TEAM_Y_OFFSET);
                
                BattleEvent* pBattleEvent = BattleEvent::create();
                pBattleEvent->setAction(1);
                pBattleTeam->setTeamAction(1);
                pBattleEvent->setBattleTeam(pBattleTeam);
                pBattleEvent->setCurrPosIndex(pBattleTeam->getPosIndex());
                pBattleEvent->setMoveDirection(3);
                pBattleEvent->setMoveToPosIndex(pBattleTeam->getPosIndex());
                // 添加战场(move)事件
                m_battleEvents->addObject((CCObject*)pBattleEvent);
            } else {
                pBattleTeam->updateTeamPosition(_getPosByPosIndex(pBattleTeam->getPosIndex()), 3, SCROLL_VERTICAL_OFFSERT);
                pBattleTeam->teamMove(2, -SCROLL_VERTICAL_OFFSERT);
            }
        }
    } /*for*/
    
    if (isFirstRound) {
        m_countOfWaitEvents = m_countOfInBattleHero + m_countOfInBattleEnemy;
    } else {
        m_countOfWaitEvents = 1;
    }
    
    /* 动画 */
    if (isFirstRound) {
        BattleEvent* pBattleEvent = BattleEvent::create();
        pBattleEvent->setAction(11);
        pBattleEvent->setCountOfWaitEvents(1);
        m_battleEvents->addObject((CCObject*)pBattleEvent);
    }
    
    /*
     *  战斗开始前动画
     */
    if (m_pBattle->getBattlePkMode() == BPM_PVE) {
        BattleEvent* pBeforeBattleEvent = BattleEvent::create();
        pBeforeBattleEvent->setAction(12);
        pBeforeBattleEvent->setCountOfWaitEvents(1);
        m_battleEvents->addObject((CCObject*)pBeforeBattleEvent);
    }
    
    /*
     *  根据下载的战斗回合数据生成 battle event
     */
    int round = -1;
    // 获取攻击数据的数量
    unsigned int countOfAttackDataItem  = m_pBattle->getCountOfAttackDataItem();
    for (unsigned int i = 0; i < countOfAttackDataItem; i++) {
        AttackDataItem* pAttackDataItem = m_pBattle->getAttackDataItem(i);
        
        if (pAttackDataItem->getRound() != round) {
            BattleEvent* pBattleEvent = BattleEvent::create();
            pBattleEvent->setAction(13);
            pBattleEvent->setCountOfWaitEvents(1);
            m_battleEvents->addObject((CCObject*)pBattleEvent);
            
            round = pAttackDataItem->getRound();
        }
        
        // 攻击者身份 0:我方 1:敌方
        const unsigned int attacker = pAttackDataItem->getAttacker();
        // 攻击者 tag
        std::string attackerTag = pAttackDataItem->getAttackerTag();
        // 获取攻击者当前所在位置
        const unsigned int currTeamPosIndex = pAttackDataItem->getAttackerNowPos();
        // 获取施放技能所在位置
        const unsigned int skillPos = pAttackDataItem->getSkillPos();
        // 获取施放技能完成后所在位置
        const unsigned int completePos = pAttackDataItem->getCompletePos();
        
        /*
         *  首先移动到施放技能位置
         */
        if (currTeamPosIndex != skillPos) {
            // 生成战场事件
            BattleEvent* pBattleEvent = BattleEvent::create();
            pBattleEvent->setAction(27);
            pBattleEvent->setCurrPosIndex(currTeamPosIndex);
            pBattleEvent->setEnemy(1 == attacker);
            pBattleEvent->setAttackTag(attackerTag);
            pBattleEvent->setCountOfWaitEvents(1);
            pBattleEvent->setMoveToPosIndex(skillPos);
            // 添加战场(goto)事件
            m_battleEvents->addObject((CCObject*)pBattleEvent);
        }
        
        /*
         *  技能
         */
        // 获取技能数据
        SkillItem* pSkillItem = pAttackDataItem->getSkillItem();
        CCAssert(pSkillItem, "invalid skillitem");
        
        // 获取伤害、增益对象数量
        unsigned int countOfAttackDamageData = pAttackDataItem->getCountOfAttackDamageData();
        
        /* 技能释放 */
        BattleEvent* pSkillReleaseBattleEvent = BattleEvent::create();
        pSkillReleaseBattleEvent->setAction(3);
        pSkillReleaseBattleEvent->setCurrPosIndex(skillPos);
        pSkillReleaseBattleEvent->setEnemy(1 == attacker);
        pSkillReleaseBattleEvent->setAttackTag(attackerTag);
        pSkillReleaseBattleEvent->setCountOfWaitEvents(1);
        pSkillReleaseBattleEvent->setRemoteAttack(currTeamPosIndex == skillPos);
        pSkillReleaseBattleEvent->setSkillItem(pSkillItem);
        
        /* 技能弹道 */
        BattleEvent* pSkillRunBattleEvent = BattleEvent::create();
        pSkillRunBattleEvent->setAction(4);
        pSkillRunBattleEvent->setCurrPosIndex(skillPos);
        pSkillRunBattleEvent->setEnemy(1 == attacker);
        pSkillRunBattleEvent->setAttackTag(attackerTag);
        pSkillRunBattleEvent->setCountOfWaitEvents(1);
        pSkillRunBattleEvent->setSkillItem(pSkillItem);
        
        /* 技能目标 */
        BattleEvent* pSkillTargetBattleEvent = BattleEvent::create();
        pSkillTargetBattleEvent->setAction(5);
        pSkillTargetBattleEvent->setCurrPosIndex(skillPos);
        pSkillTargetBattleEvent->setEnemy(1 == attacker);
        pSkillTargetBattleEvent->setAttackTag(attackerTag);
        pSkillTargetBattleEvent->setCountOfWaitEvents(1);
        pSkillTargetBattleEvent->setSkillItem(pSkillItem);
        
        for (unsigned int j = 0; j < countOfAttackDamageData; j++) {
            // 获取攻击伤害数据
            AttackDamageDataItem* pAttackDamageDataItem = pAttackDataItem->getAttackDamageData(j);
            
            pSkillReleaseBattleEvent->appendSkillTarget(pAttackDamageDataItem->getPosIndexInBattle(),
                                                        pAttackDamageDataItem->getDamageValue(), pAttackDamageDataItem->getRemainHp());
            pSkillRunBattleEvent->appendSkillTargetPosIndex(
                                                            pAttackDamageDataItem->getPosIndexInBattle());
            pSkillTargetBattleEvent->appendSkillTarget(pAttackDamageDataItem->getPosIndexInBattle(),
                                                       pAttackDamageDataItem->getDamageValue(), pAttackDamageDataItem->getRemainHp());
        } /*for*/
        
        // 添加技能释放事件
        m_battleEvents->addObject((CCObject*)pSkillReleaseBattleEvent);
        // 添加技能弹道事件
        m_battleEvents->addObject((CCObject*)pSkillRunBattleEvent);
        // 添加技能目标事件
        m_battleEvents->addObject((CCObject*)pSkillTargetBattleEvent);
        
        /* 施放技能结束移动 */
        if (completePos > 0) {
            if (skillPos != completePos) {
                // 生成战场事件
                BattleEvent* pBattleEvent = BattleEvent::create();
                pBattleEvent->setAction(27);
                pBattleEvent->setCurrPosIndex(skillPos);
                pBattleEvent->setEnemy(1 == attacker);
                pBattleEvent->setAttackTag(attackerTag);
                pBattleEvent->setCountOfWaitEvents(1);
                pBattleEvent->setMoveToPosIndex(completePos);
                pBattleEvent->setRestoreDir(true);
                
                // 添加战场(goto)事件
                m_battleEvents->addObject((CCObject*)pBattleEvent);
            }
        } else /*回到原来位置*/ {
            if (currTeamPosIndex != skillPos) {
                // 生成战场事件
                BattleEvent* pBattleEvent = BattleEvent::create();
                pBattleEvent->setAction(27);
                pBattleEvent->setCurrPosIndex(skillPos);
                pBattleEvent->setEnemy(1 == attacker);
                pBattleEvent->setAttackTag(attackerTag);
                pBattleEvent->setCountOfWaitEvents(1);
                pBattleEvent->setMoveToPosIndex(currTeamPosIndex);
                pBattleEvent->setRestoreDir(true);
                
                // 添加战场(goto)事件
                m_battleEvents->addObject((CCObject*)pBattleEvent);
            }
        }
    } /*for*/
    
    // 战场调度
    this->schedule(schedule_selector(BattleLayer::battle_schedule), 0.1f, kCCRepeatForever, 0.0f);
}

/*
 *  功能：
 *      加载士兵动画
 */
void BattleLayer::_loadSoldierAni() {
    CCAnimation* animation = NULL;
    CCString* str = NULL;
    
    char szState[8];            // 状态
    char szJob[16];             // 士兵职业
    unsigned int frames = 0;    // 不同状态士兵动画帧数
    
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 3; j++) {
            animation = CCAnimation::create();
            
            if (0 == i) /* 职业:步兵 */{
                strcpy(szJob, "bub");
            } else if (1 == i) /* 职业:枪兵 */ {
                strcpy(szJob, "qiangb");
            } else if (2 == i) {
                strcpy(szJob, "gongb");
            } else if (3 == i) {
                strcpy(szJob, "yis");
            } else if (4 == i) {
                strcpy(szJob, "mous");
            } else if (5 == i) {
                strcpy(szJob, "ces");
            } else if (6 == i) {
                strcpy(szJob, "duw");
            }
            if (1 == j + 1) /*常规*/ {
                strcpy(szState, "dj");frames = SOLDIER_DJ_STATE_ANI_FRAMES;
            } else if (2 == j + 1)/*运动*/ {
                strcpy(szState, "yd");frames = SOLDIER_YD_STATE_ANI_FRAMES;
            } else if (3 == j + 1)/*死亡*/ {
                strcpy(szState, "sw");frames = SOLDIER_SW_STATE_ANI_FRAMES;
            }
            for (unsigned int k = 1; k <= frames; k++) {
                str = CCString::createWithFormat("%s_%s%02d.png", szState, szJob, k);
                animation->addSpriteFrame(m_pSpriteFrameCache->spriteFrameByName(str->getCString()));
            } /*for*/
            
            animation->setDelayPerUnit(SOLDIER_DEAD_PS);
            animation->retain();
            m_soldierAni[i][j] = animation;
        } /*for*/
    } /*for*/
}

/*
 *  功能：
 *      获取士兵动画
 *  参数：
 *      job     职业
 *      state   状态
 *  返回：
 *      士兵动画
 */
CCAnimation* BattleLayer::getSoldierAni(const unsigned int job, const unsigned int state) {
    CCAssert(job >= 1 && job <= 7 && state >= 1 && state <= 3, "invalid data");
    return m_soldierAni[job - 1][state - 1];
}

/*
 *  功能：
 *      加载掉落动画
 */
void BattleLayer::_loadDropAni() {
    char szFrameName[32];
    
    m_propDropAni = CCAnimation::create();
    m_heroDropAni = CCAnimation::create();
    m_equipDropAni = CCAnimation::create();
    
    for (unsigned int i = 0; i < 23; i++) {
        // 道具掉落
        sprintf(szFrameName, "btde%02d.png", i + 1);
        m_propDropAni->addSpriteFrame(m_pSpriteFrameCache->spriteFrameByName(szFrameName));
        m_propDropAni->setDelayPerUnit(0.1f);
        
        // 武将掉落
        sprintf(szFrameName, "btca%02d.png", i + 1);
        m_heroDropAni->addSpriteFrame(m_pSpriteFrameCache->spriteFrameByName(szFrameName));
        m_heroDropAni->setDelayPerUnit(0.1f);
        
        // 装备掉落
        sprintf(szFrameName, "btcl%02d.png", i + 1);
        m_equipDropAni->addSpriteFrame(m_pSpriteFrameCache->spriteFrameByName(szFrameName));
        m_equipDropAni->setDelayPerUnit(0.1f);
    } /*for*/
    
    m_propDropAni->retain();
    m_heroDropAni->retain();
    m_equipDropAni->retain();
}

/*
 *  功能：
 *      初始化位置索引对应的区块左下角坐标点位置
 */
void BattleLayer::_initPosByPosIndex() {
    CCPoint point = CCPointZero;
    
    const float horizontalSpaceBetweenTeam = 160.0f + 40.0f;    // 队伍之间的水平方向间距
    const float verticalSpaceBetweenTeam = 160.0f + 6.0f;       // 队伍之间的垂直方向间距
    
    float axisY[5];
    axisY[0] = 48.0f;
    axisY[1] = axisY[0] + verticalSpaceBetweenTeam;
    axisY[2] = axisY[0] + verticalSpaceBetweenTeam * 2.0f;
    axisY[3] = axisY[0] + verticalSpaceBetweenTeam * 3.0f;
    axisY[4] = axisY[0] + verticalSpaceBetweenTeam * 4.0f;
    
    const float left = 54.0f, middle = left + horizontalSpaceBetweenTeam, right = left + horizontalSpaceBetweenTeam * 2;
    
    m_allOriginCoordinate[0] = ccp(left, axisY[0] + m_pGameState->getBottomOffset());       // 11
    m_allOriginCoordinate[1] = ccp(middle, axisY[0] + m_pGameState->getBottomOffset());     // 12
    m_allOriginCoordinate[2] = ccp(right, axisY[0] + m_pGameState->getBottomOffset());      // 13
    m_allOriginCoordinate[3] = ccp(left, axisY[1] + m_pGameState->getBottomOffset());       // 21
    m_allOriginCoordinate[4] = ccp(middle, axisY[1] + m_pGameState->getBottomOffset());     // 22
    m_allOriginCoordinate[5] = ccp(right, axisY[1] + m_pGameState->getBottomOffset());      // 23
    m_allOriginCoordinate[6] = ccp(left, axisY[2] + m_pGameState->getBottomOffset());       // 31
    m_allOriginCoordinate[7] = ccp(middle, axisY[2] + m_pGameState->getBottomOffset());     // 32
    m_allOriginCoordinate[8] = ccp(right, axisY[2] + m_pGameState->getBottomOffset());      // 33
    m_allOriginCoordinate[9] = ccp(left, axisY[3] + m_pGameState->getBottomOffset());       // 41
    m_allOriginCoordinate[10] = ccp(middle, axisY[3] + m_pGameState->getBottomOffset());    // 42
    m_allOriginCoordinate[11] = ccp(right, axisY[3] + m_pGameState->getBottomOffset());     // 43
    m_allOriginCoordinate[12] = ccp(left, axisY[4] + m_pGameState->getBottomOffset());      // 51
    m_allOriginCoordinate[13] = ccp(middle, axisY[4] + m_pGameState->getBottomOffset());    // 52
    m_allOriginCoordinate[14] = ccp(right, axisY[4] + m_pGameState->getBottomOffset());     // 53
}

/*
 *  功能：
 *      根据位置索引获得该区块左下角坐标点位置
 *  参数：
 *      posIndex    位置索引
 *  返回：
 *      对应位置索引的区块左下角坐标点位置
 */
CCPoint BattleLayer::_getPosByPosIndex(const unsigned int posIndex) {
    unsigned int retIndex = 0;
    if (11 == posIndex) {
        retIndex = 0;
    } else if (12 == posIndex) {
        retIndex = 1;
    } else if (13 == posIndex) {
        retIndex = 2;
    } else if (21 == posIndex) {
        retIndex = 3;
    } else if (22 == posIndex) {
        retIndex = 4;
    } else if (23 == posIndex) {
        retIndex = 5;
    } else if (31 == posIndex) {
        retIndex = 6;
    } else if (32 == posIndex) {
        retIndex = 7;
    } else if (33 == posIndex) {
        retIndex = 8;
    } else if (41 == posIndex) {
        retIndex = 9;
    } else if (42 == posIndex) {
        retIndex = 10;
    } else if (43 == posIndex) {
        retIndex = 11;
    } else if (51 == posIndex) {
        retIndex = 12;
    } else if (52 == posIndex) {
        retIndex = 13;
    } else if (53 == posIndex) {
        retIndex = 14;
    } else {
        CCAssert(false, "invalid posIndex");
    }
    return m_allOriginCoordinate[retIndex];
}

/*
 *  功能：
 *      根据位置索引获得对应战队
 *  参数：
 *      posIndex    位置索引
 *  返回：
 *      成功返回该位置索引的战队，否则返回 NULL
 */
BattleTeam* BattleLayer::_getBattleTeam(const unsigned int posIndex) {
    CCObject* pItem = NULL;
    BattleTeam* pRetBattleTeam = NULL;
    
    CCARRAY_FOREACH(m_BattleTeamsArray, pItem) {
        BattleTeam* pBattleTeam = (BattleTeam*)pItem;
        if (pBattleTeam->getPosIndex() == posIndex) {
            pRetBattleTeam = pBattleTeam;
            break;
        }
    } /*for*/
    
    return pRetBattleTeam;
}

/*
 *  功能：
 *      根据参数获得对应战队
 *  参数：
 *      posIndex    位置索引
 *      isEnemy     是否敌方战队
 *      attackTag   攻击者 tag
 *
 *  返回：
 *      成功返回该位置索引的战队，否则返回 NULL
 */
BattleTeam* BattleLayer::_getBattleTeam(const unsigned int posIndex, const bool isEnemy, std::string attackTag) {
    CCObject* pItem = NULL;
    BattleTeam* pRetBattleTeam = NULL;
    
    CCARRAY_FOREACH(m_BattleTeamsArray, pItem) {
        BattleTeam* pBattleTeam = (BattleTeam*)pItem;
        if (pBattleTeam->getPosIndex() == posIndex && pBattleTeam->isEnemy() == isEnemy) {
            if (m_pBattle->getBattlePkMode() == BPM_PVE && isEnemy) {
                MosterInBattleItem* pMosterItem = static_cast<MosterInBattleItem*>(pBattleTeam->getLeadObject());
                if (pMosterItem->getId() != atoi(attackTag.c_str())) {
                    continue;
                }
            } else {
                InBattleItem* pInBattleItem = pBattleTeam->getLeadObject();
                if (pInBattleItem->isMonster()) {
                    MosterInBattleItem* pMosterItem = static_cast<MosterInBattleItem*>(pInBattleItem);
                    if (pMosterItem->getId() != atoi(attackTag.c_str())) {
                        continue;
                    }
                } else {
                    HeroOfPlayerItem* pHeroOfPlayerItem = static_cast<HeroOfPlayerItem*>(pInBattleItem);
                    if (pHeroOfPlayerItem->getUniId().compare(attackTag.c_str()) != 0) {
                        continue;
                    }
                }
            }
            pRetBattleTeam = pBattleTeam;
            break;
        }
    } /*for*/
    
    return pRetBattleTeam;
}

/*
 *  功能：
 *      重置本方队伍的位置（到它的原始位置）
 */
void BattleLayer::_resetTeamPos() {
    CCObject* pItem = NULL;
    BattleTeam* pBattleTeam = NULL;
    
    CCARRAY_FOREACH(m_BattleTeamsArray, pItem) {
        pBattleTeam = (BattleTeam*)pItem;
        CCAssert(!pBattleTeam->isEnemy(), "invalid team");
        pBattleTeam->restoreTeam();
        
    } /*for*/
    
    if (m_pBattle->getBattlePkMode() == BPM_PVE) {
        // 如果本方队伍
        CCARRAY_FOREACH(m_BattleDeadTeamsArray, pItem) {
            pBattleTeam = (BattleTeam*)pItem;
            pBattleTeam->setLeaderObject(NULL, 1);
            m_BattleTeamsArray->addObject(pItem);
        } /*for*/
    }
    m_BattleDeadTeamsArray->removeAllObjects();
    
    CCARRAY_FOREACH(m_BattleTeamsArray, pItem) {
        pBattleTeam = (BattleTeam*)pItem;
        CCAssert(!pBattleTeam->isEnemy(), "invalid team");
        
        HeroOfPlayerItem* pAttachHeroOfPlayerItem = dynamic_cast<HeroOfPlayerItem*>(pBattleTeam->getLeadObject());
        CCAssert(pAttachHeroOfPlayerItem, "invalid attachItem");
        
        pBattleTeam->setPosIndex(pAttachHeroOfPlayerItem->getPosInBattle());
        pBattleTeam->updateTeamPosition(_getPosByPosIndex(pAttachHeroOfPlayerItem->getPosInBattle()), 1);
    } /*for*/
}

/*
 *  功能：
 *      获取技能精灵转动角度
 *  参数：
 *      startPoint  起点
 *      endPoint    终点
 *  返回：
 *      根据起点和终点计算的角度
 */
float BattleLayer::_getSkillAngle(const CCPoint startPoint, const CCPoint endPoint) const {
    float angle = 0;
    
    const float offsetX = endPoint.x - startPoint.x;
    const float offsetY = endPoint.y - startPoint.y;
    
    do {
        if (offsetX == 0.0f) {
            if (offsetY > 0.0) {
                angle = 180.0f;
            } else {
                angle = 0.0f;
            }
            break;
        }
        if (offsetY == 0.0f) {
            if (offsetX > 0.0f) {
                angle = 270.0f;
            } else {
                angle = 90.0f;
            }
            break;
        }
        
        angle = 270.0f - atan2f(offsetY, offsetX) * 180.0f / M_PI;
    } while (0);
    
    return angle;
}

/*
 *  提交本方阵型数据给服务器，获取下一回合战斗数据
 */
void BattleLayer::nextFight() {
    m_go->stopAllActions();
    m_go->setVisible(false);
    m_enableTouch = false;
    
    // 隐藏阵型调整提示框
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 3; ++j) {
            CCSprite* blinkK = (CCSprite*)m_battleCBN->getChildByTag(BLINKKUANG + 3 * i + j);
            blinkK->setVisible(false);
        }
    }
    
    /* 清除掉落动画 */
    _clearDropAni();
    
    if (m_pBattle->isCWar()) {
        m_exit->setVisible(false);
        CCSprite* pEBG = (CCSprite*)m_battleCBN->getChildByTag(BATTLE_EXIT_BTN_BG);
        pEBG->setVisible(false);
        CCSprite* pEBHL = (CCSprite*)m_battleCBN->getChildByTag(BATTLE_EXIT_BTN_HL);
        pEBHL->setVisible(false);
        
        // 提交 国战 战斗请求
        m_pBattle->commitCWarFightRequest();
    } else /*pve*/{
        /* 清除所有 pve 战斗回合数据 */
        m_pBattle->clearAllBattleData(false, false);
        
        // 提交 PVE 战斗请求
        m_pBattle->commitPveFightRequest();
    }
    
    m_pNotificationCenter->postNotification(ON_MESSAGE_LOADING_BATTLE);
}

/*
 * 功能：
 *      处理服务器回应数据
 * 功能：
 *      szData  服务器回应数据缓冲区
 */
void BattleLayer::handleWSResponseData(const char *szData, const unsigned int dataSize, char* szTag) {
    bool isSuccess = ProcessData::parseBufferFromSvr(szData, dataSize, szTag);
    m_pNotificationCenter->postNotification(ON_MESSAGE_SHUTDOWN_BATTLE);
    
    if (!strcmp(szTag, HTTP_REQUEST_FIGHT_PVE_TAG)) {
        if (isSuccess) {
            // 初始化战场
            _initBattle();
        }
    } else if (!strcmp(szTag, HTTP_REQUEST_FIGHT_CWAR_TAG)) /*国战*/ {
        if (isSuccess) {
            if (m_pBattle->isInList()) {
                defeatCallback();
            } else {
                // 初始化战场
                _initBattle();
            }
        } else {
            defeatCallback();
        }
    }
}

/*
 *  游戏胜利回调方法
 */
void BattleLayer::victoryCallback() {
    //    CCLog("胜利了,显示战斗结算界面 !");
    // 清除掉落动画
    _clearDropAni();
    if (m_pBattle->getBattlePkMode() == BPM_PVE) {
        m_pNotificationCenter->postNotification(ON_MESSAGE_SUMMARY_STRING);
    } else {
        m_pNotificationCenter->postNotification(ON_MESSAGE_PVP_SUMMARY_STRING);
    }
    
}

/*
 *  游戏失败回调方法
 */
void BattleLayer::defeatCallback() {
    // 清除掉落动画
    _clearDropAni();
    if (m_pBattle->getBattlePkMode() == BPM_PVE) {
        m_pNotificationCenter->postNotification(ON_MESSAGE_SUMMARY_STRING);
    } else {
        m_pNotificationCenter->postNotification(ON_MESSAGE_PVP_SUMMARY_STRING);
    }
}

/*
 *  功能:
 *      掉落动画完成回调
 */
void BattleLayer::dropDoneCallback(CCNode* pNode, void* pDropSprite) {
    CCAssert(pDropSprite, "invalid");
    
    CCSprite* pDropItem = (CCSprite*)pDropSprite;
    pDropItem->removeFromParent();
}

/*
 *  功能:
 *      清除掉落动画
 */
void BattleLayer::_clearDropAni() {
    CCObject* pObject = NULL;
    CCARRAY_FOREACH(m_dropItems, pObject) {
        CCSprite* pDropItem = (CCSprite*)pObject;
        
        pDropItem->stopAllActions();
        pDropItem->removeFromParent();
        
    } /*for*/
    m_dropItems->removeAllObjects();
}

/*
 *  功能:
 *      波数波数动画特效
 *  参数：
 *      round   波数(1为第一波)
 */
void BattleLayer::roundEffect(const unsigned int round) {
    float centerY = m_pDirector->getVisibleSize().height / 2.0f;
    const float speedCtrl = 0.5f;
    if (round < MAX_BATTLE_ROUNDS) {
        /* 分割线 */
        CCSprite* xieg = CCSprite::createWithSpriteFrameName("BT_levg.png");
        xieg->setPosition(ccp(320.0f, centerY));
        xieg->setOpacity(0);
        m_battleCBN->addChild(xieg, 2, ROUND_EFFECT_LEVG_TAG);
        CCSpeed* plevgSpeed = CCSpeed::create(CCSequence::create(CCFadeIn::create(0.25f*speedCtrl),
                                                                 CCDelayTime::create(0.5f*speedCtrl), CCFadeOut::create(0.1f*speedCtrl), NULL),
                                              m_pBattle->getBattleSpeed() >= 3 ? 3 : m_pBattle->getBattleSpeed());
        plevgSpeed->setTag(ROUND_EFFECT_ACTION_TAG);
        xieg->runAction(plevgSpeed);
        
        /* 波数 */
        CCString* pString = CCString::createWithFormat("BT_lev%d.png", round);
        CCSprite* curRound = CCSprite::createWithSpriteFrameName(pString->getCString());
        curRound->setOpacity(0);
        curRound->setPosition(ccp(-curRound->getContentSize().width / 2 + 100.0f, centerY + 50.0f));
        m_battleCBN->addChild(curRound, 2, ROUND_EFFECT_NUMBER_TAG);
        
        CCActionInterval* m2 = CCMoveTo::create(0.2f*speedCtrl, ccp(260.0f, curRound->getPositionY()));
        CCFiniteTimeAction * spawn1 = CCSpawn::create(CCEaseExponentialOut::create(m2), CCFadeIn::create(0.15f*speedCtrl), NULL);
        CCActionInterval* out2 = CCMoveTo::create(0.2f*speedCtrl, ccp(-curRound->getContentSize().width / 2 + 100.0f, curRound->getPositionY()));
        CCFiniteTimeAction * seq22 = CCSequence::create(CCDelayTime::create(0.05f*speedCtrl), CCFadeOut::create(0.15f*speedCtrl), NULL);
        CCFiniteTimeAction * spawn2 = CCSpawn::create(CCEaseExponentialIn::create(out2), seq22, NULL);
        CCSpeed* pRnSpeed = CCSpeed::create(CCSequence::create(CCDelayTime::create(0.25f*speedCtrl), spawn1, CCDelayTime::create(0.25f*speedCtrl), spawn2, NULL), m_pBattle->getBattleSpeed() >= 3 ? 3 : m_pBattle->getBattleSpeed());
        pRnSpeed->setTag(ROUND_EFFECT_ACTION_TAG);
        curRound->runAction(pRnSpeed);
        
        /* 总波数 */
        CCSprite* roundThree = CCSprite::createWithSpriteFrameName("BT_lev3.png");
        roundThree->setPosition(ccp(640.0f + roundThree->getContentSize().width / 2.0f, centerY -  50.0f));
        roundThree->setOpacity(0);
        m_battleCBN->addChild(roundThree, 2, ROUND_EFFECT_TOTOL_TAG);
        
        CCActionInterval *tan33 = CCEaseExponentialOut::create(CCMoveTo::create(0.15f*speedCtrl, ccp(370.0f, roundThree->getPositionY())));
        CCFiniteTimeAction * spawn3 = CCSpawn::create(tan33, CCFadeIn::create(0.15f*speedCtrl), NULL);
        CCActionInterval *tan333 = CCEaseExponentialIn::create(CCMoveTo::create(0.2f*speedCtrl,
                                                                                ccp(640.0f + roundThree->getContentSize().width / 2, roundThree->getPositionY())));
        CCFiniteTimeAction * seq232 = CCSequence::create(CCDelayTime::create(0.05f*speedCtrl), CCFadeOut::create(0.15f*speedCtrl), NULL);
        CCFiniteTimeAction * spawn21 =CCSpawn::create(tan333, seq232, NULL);
        
        CCSpeed* pR3Speed = CCSpeed::create(CCSequence::create(CCDelayTime::create(0.35f*speedCtrl), spawn3, CCDelayTime::create(0.25f*speedCtrl), spawn21, NULL), m_pBattle->getBattleSpeed() >= 3 ? 3 : m_pBattle->getBattleSpeed());
        pR3Speed->setTag(ROUND_EFFECT_ACTION_TAG);
        roundThree->runAction(pR3Speed);
    } else {
        /*BOSS*/
        CCSprite* spBoss = CCSprite::createWithSpriteFrameName("BT_boss.png");
        spBoss->setOpacity(0);
        spBoss->setPosition(ccp(655.0f, centerY));
        m_battleCBN->addChild(spBoss, 2, ROUND_EFFECT_BOSS_TAG);
        
        CCActionInterval *easeBackOut = CCEaseBackOut::create(CCMoveTo::create(0.4f*speedCtrl, ccp(405, spBoss->getPositionY())));
        CCFiniteTimeAction * spawnin = CCSpawn::create(easeBackOut, CCFadeIn::create(0.3*speedCtrl), NULL);
        
        CCActionInterval *easeBackIn = CCEaseBackIn::create(CCMoveTo::create(0.4f*speedCtrl, ccp(100, spBoss->getPositionY())));
        CCFiniteTimeAction * spawn = CCSequence::create(CCDelayTime::create(0.1f*speedCtrl), CCFadeOut::create(0.3*speedCtrl), NULL);
        
        CCSequence * seq = CCSequence::create(spawnin, CCDelayTime::create(0.5f*speedCtrl), CCSpawn::create(easeBackIn, spawn, NULL), NULL);
        CCSpeed* pSpeed = CCSpeed::create(seq, m_pBattle->getBattleSpeed() >= 3 ? 3 : m_pBattle->getBattleSpeed());
        pSpeed->setTag(ROUND_EFFECT_ACTION_TAG);
        spBoss->runAction(pSpeed);
    }
}

/*
 *  功能：
 *      战斗初始动画特效
 */
void BattleLayer::battleInitEffect() {
    const float speedCtrl = 0.5f;
    CCSprite* spBat = CCSprite::createWithSpriteFrameName("BT_batfont.png");
    spBat->setOpacity(0);
    spBat->setPosition(ccp(655.0f, m_pDirector->getWinSize().height / 2));
    m_battleCBN->addChild(spBat, 2, BATTLE_INIT_ANI_SPRITE_TAG);
    
    CCActionInterval *easeBackOut = CCEaseBackOut::create(CCMoveTo::create(0.4f*speedCtrl, ccp(405, spBat->getPositionY())));
    CCFiniteTimeAction * spawnin = CCSpawn::create(easeBackOut, CCFadeIn::create(0.3*speedCtrl), NULL);
    
    CCActionInterval *easeBackIn = CCEaseBackIn::create(CCMoveTo::create(0.4f*speedCtrl, ccp(100, spBat->getPositionY())));
    CCFiniteTimeAction * spawn = CCSequence::create(CCDelayTime::create(0.1f*speedCtrl), CCFadeOut::create(0.3*speedCtrl), NULL);
    
    CCSequence * seq = CCSequence::create(spawnin, CCDelayTime::create(0.5f*speedCtrl), CCSpawn::create(easeBackIn, spawn, NULL), NULL);
    CCSpeed* pSpeed = CCSpeed::create(seq, m_pBattle->getBattleSpeed() >= 3 ? 3 : m_pBattle->getBattleSpeed());
    pSpeed->setTag(BATTLE_INIT_ANI_ACTION_TAG);
    spBat->runAction(pSpeed);
}

//
void BattleLayer::moveOutTouchUICompent() {
    CCSprite* victory = (CCSprite*)m_battleCBN->getChildByTag(VICTORY_TAG);
    if (victory) {
        victory->setPosition(ccp(-960.0f, -1136.0f));
    }
    CCSprite* defeat = (CCSprite*)m_battleCBN->getChildByTag(DEFEAT_TAG);
    if (defeat) {
        defeat->setPosition(ccp(-960.0f, -1136.0f));
    }
    m_go->setPosition(ccp(-960.0f, -1136.0f));
}

//
void BattleLayer::restoreTouchUICompent() {
    CCSprite* victory = (CCSprite*)m_battleCBN->getChildByTag(VICTORY_TAG);
    if (victory) {
        victory->setPosition(ccp(m_pDirector->getVisibleSize().width / 2.0f,
                                 m_pDirector->getVisibleSize().height / 2.0f + 35.0f));
    }
    CCSprite* defeat = (CCSprite*)m_battleCBN->getChildByTag(DEFEAT_TAG);
    if (defeat) {
        defeat->setPosition(ccp(m_pDirector->getVisibleSize().width / 2.0f,
                                m_pDirector->getVisibleSize().height / 2.0f));
    }
    m_go->setPosition(ccp(m_pDirector->getVisibleSize().width / 2.0f,
                            m_pDirector->getVisibleSize().height / 2.0f + 35.0f));
}

