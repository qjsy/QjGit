//
//  BattleTeam.cpp
//  hero
//
//  Created by yangjie on 2013/12/31.
//
//

#include "BattleTeam.h"
#include "BattleLayer.h"
#include "HeroItem.h"
#include "HeroOfPlayerItem.h"
#include "InBattleItem.h"
#include "MosterItem.h"
#include "MosterInBattleItem.h"
#include "Battle.h"
#include "HeroManager.h"
#include "SkillItem.h"
#include "UtilityFunc.h"
#include "GameState.h"

#define LEADER_PADDIN                           0.0f        // 武将离边界的间隙距离
#define HERO_HEAD_WIDTH                         160.0f      // 武将头像宽度
#define HERO_HEAD_HEIGHT                        70.0f       // 武将头像高度
#define SOLDIERS_COUNT_PER_ROW                  5           // 每行士兵的数量
#define SOLDIER_IMAGE_WIDTH                     26.0f       // 士兵图片宽度
#define SOLDIER_IMAGE_HEIGHT                    26.0f       // 士兵图片高度
#define SOLDIER_PADDING_HERO                    5.0f        // 士兵方阵离武将头像间隙距离
#define SOLDIERS_HORIZONTAL_SPACING             6.0f        // 士兵之间水平间隔
#define SOLDIER_OFFSET_LEFT                     14.0f       // 士兵位置左偏移量

#define SKILL_SPRITE_BATCH_TAG                  650         // 技能特效批处理精灵

// 士兵区域宽度
#define SOLDIER_AREA_WIDTH (SOLDIER_IMAGE_WIDTH * SOLDIERS_COUNT_PER_ROW)
// 士兵区域高度
#define SOLDIER_AREA_HEIGHT (SOLDIER_IMAGE_HEIGHT * 3)

// 武将、士兵区块高度
#define BLOCK_HEIGHT (SOLDIER_AREA_HEIGHT + SOLDIER_PADDING_HERO + HERO_HEAD_HEIGHT)

#define TEAM_DEFAULT_ZORDER                      5          // 战斗队伍精灵默认 zorder
#define TEAM_TOUCHMOVE_ZORDER                    60         // 战斗队伍精灵 touchMove zorder

#define SKILL_DAMAGE_ANI_OFFSETY                24.0f       // 技能伤害数字动画偏移量
#define SKILL_DAMAGE_ANI_DELAY                  0.2f        // 技能伤害数字动画延迟时间（在技能目标后）


/* 武将框偏移位置 */
#define HERO_BOX_OFFSETX                         20.0f
#define HERO_BOX_OFFSETY                         (-5.0f)
#define LEADER_AND_SOLDIER                       (15.0f)

/*
 *  构造函数
 */
BattleTeam::BattleTeam() {
    m_skillSprites = CCArray::createWithCapacity(6);
    m_skillSprites->retain();
    
    m_skillDamageSprites = CCArray::createWithCapacity(16);
    m_skillDamageSprites->retain();
    
    m_gotoActions = CCArray::createWithCapacity(16);
    m_gotoActions->retain();
    
    m_gotoBySoldierActions = CCArray::createWithCapacity(16);
    m_gotoBySoldierActions->retain();
}

/*
 *  析构函数
 */
BattleTeam::~BattleTeam() {
    CC_SAFE_RELEASE(m_skillSprites);
    CC_SAFE_RELEASE(m_skillDamageSprites);
    CC_SAFE_RELEASE(m_gotoActions);
    CC_SAFE_RELEASE(m_gotoBySoldierActions);
}

/*
 *  初始化
 */
bool BattleTeam::init() {
    m_action = 0;
    m_posIndex = 0;
    m_pLeadObject = NULL;
    m_pTeamLeaderSprite = NULL;
    m_hpInBattle = 0;
    for (int i = 0; i < MAX_IN_BATTLE_SOLDIERS; i++) {
        m_pSoldiersSprite[i] = NULL;
    } /*for*/
    
    m_isDead = false;
    m_isDamage = false;
    return true;
}

/*
 *  功能：
 *      设置关联武将（怪物）数据项
 *  参数：
 *      pItem           关联武将
 *      soldierState    士兵状态
 */
void BattleTeam::setLeaderObject(InBattleItem* pInBattleItem, const unsigned int soldierState) {
    if (NULL == pInBattleItem) {
        CCAssert(m_pLeadObject, "invalid data");
    } else {
        CCAssert(NULL == m_pLeadObject, "m_pLeadObject must null");
        m_pLeadObject = pInBattleItem;
    }
    
    if (m_isEnemy) {
        if (m_BL->m_pBattle->getBattlePkMode() == BPM_PVE) {
            MosterInBattleItem* pMosterInBattleItem = (MosterInBattleItem*)m_pLeadObject;
            m_rarity = pMosterInBattleItem->getMosterItem()->getRarity();
            m_job = pMosterInBattleItem->getMosterItem()->getJob();
            m_hpInBattle = pMosterInBattleItem->getHP();
            m_pTeamLeaderSprite = CCSprite::createWithSpriteFrame(m_BL->m_pSpriteFrameCache->spriteFrameByName(pMosterInBattleItem->getMosterItem()->getFi().c_str()));
        } else {
            if (m_pLeadObject->isMonster()) {
                MosterInBattleItem* pMosterInBattleItem = (MosterInBattleItem*)m_pLeadObject;
                m_rarity = pMosterInBattleItem->getMosterItem()->getRarity();
                m_job = pMosterInBattleItem->getMosterItem()->getJob();
                m_hpInBattle = pMosterInBattleItem->getHP();
                m_pTeamLeaderSprite = CCSprite::createWithSpriteFrame(m_BL->m_pSpriteFrameCache->spriteFrameByName(pMosterInBattleItem->getMosterItem()->getFi().c_str()));
            } else {
                HeroOfPlayerItem* pHeroOfPlayerItem = (HeroOfPlayerItem*)m_pLeadObject;
                m_rarity = pHeroOfPlayerItem->getHeroItem()->getRarity();
                m_job = pHeroOfPlayerItem->getHeroItem()->getJob();
                m_hpInBattle = pHeroOfPlayerItem->getPvpHp();
                m_pTeamLeaderSprite = CCSprite::createWithSpriteFrame(m_BL->m_pSpriteFrameCache->spriteFrameByName( pHeroOfPlayerItem->getHeroItem()->getFIcon().c_str()));
            }
        }
    } else {
        HeroOfPlayerItem* pHeroOfPlayerItem = (HeroOfPlayerItem*)m_pLeadObject;
        m_rarity = pHeroOfPlayerItem->getHeroItem()->getRarity();
        m_job = pHeroOfPlayerItem->getHeroItem()->getJob();
        m_hpInBattle = pHeroOfPlayerItem->getHP();
        m_pTeamLeaderSprite = CCSprite::createWithSpriteFrame(m_BL->m_pSpriteFrameCache->spriteFrameByName( pHeroOfPlayerItem->getHeroItem()->getFIcon().c_str()));
    }
    
    char buffer[32];/*缓冲区*/
    
    if (1 == m_rarity) {
        strcpy(buffer, "bt_whiBox.png");
    } else if (2 == m_rarity || 3 == m_rarity) {
        strcpy(buffer, "bt_greBox.png");
    } else if (4 == m_rarity || 5 == m_rarity) {
        strcpy(buffer, "bt_bluBox.png");
    } else if (6 == m_rarity || 7 == m_rarity || 8 == m_rarity) {
        strcpy(buffer, "bt_purBox.png");
    } else {
        strcpy(buffer, "bt_oraBox.png");
    }
    
    // 背景框
    if (m_isEnemy) {
        m_pBoxSprite = CCSprite::createWithSpriteFrame(m_BL->m_pSpriteFrameCache->spriteFrameByName("bt_mok.png"));
        m_pBoxForSoldierSprite = CCSprite::createWithSpriteFrame(m_BL->m_pSpriteFrameCache->spriteFrameByName("bt_mok.png"));
    } else {
        m_pBoxSprite = CCSprite::createWithSpriteFrame(m_BL->m_pSpriteFrameCache->spriteFrameByName(buffer));
        m_pBoxForSoldierSprite = CCSprite::createWithSpriteFrame(m_BL->m_pSpriteFrameCache->spriteFrameByName("bt_mok.png"));
    }
    m_pBoxForSoldierSprite->setOpacity(0);
    m_BL->m_teamCBN->addChild(m_pBoxSprite, TEAM_DEFAULT_ZORDER - 1);
    m_BL->m_teamCBN->addChild(m_pBoxForSoldierSprite, TEAM_DEFAULT_ZORDER);
    
    // 领头武将精灵
    m_pBoxSprite->addChild(m_pTeamLeaderSprite, TEAM_DEFAULT_ZORDER);
    m_pTeamLeaderSprite->setPosition(ccp(80.0f, 40.0f));
    
    // 稀有度
    if (1 == m_rarity) {
        strcpy(buffer, "bt_whi.png");
    } else if (2 == m_rarity || 3 == m_rarity) {
        strcpy(buffer, "bt_gre.png");
    } else if (4 == m_rarity || 5 == m_rarity) {
        strcpy(buffer, "bt_blu.png");
    } else if (6 == m_rarity || 7 == m_rarity || 8 == m_rarity) {
        strcpy(buffer, "bt_pur.png");
    } else {
        strcpy(buffer, "bt_ora.png");
    }
    m_pRaritySprite = CCSprite::createWithSpriteFrame(m_BL->m_pSpriteFrameCache->spriteFrameByName(buffer));
    m_pRaritySprite->setPosition(ccp(m_pTeamLeaderSprite->boundingBox().getMinX() + m_pRaritySprite->getContentSize().width / 2 + HERO_BOX_OFFSETX + 5.0f, m_pTeamLeaderSprite->boundingBox().getMinY() + m_pRaritySprite->getContentSize().height / 2 + 5.0f + HERO_BOX_OFFSETY));
    m_pBoxSprite->addChild(m_pRaritySprite, TEAM_DEFAULT_ZORDER - 2);
    
    if (1 == m_rarity) {
        strcpy(buffer, "bt_whiJiao.png");
    } else if (2 == m_rarity || 3 == m_rarity) {
        strcpy(buffer, "bt_greJiao.png");
    } else if (4 == m_rarity || 5 == m_rarity) {
        strcpy(buffer, "bt_bluJiao.png");
    } else if (6 == m_rarity || 7 == m_rarity || 8 == m_rarity) {
        strcpy(buffer, "bt_purJiao.png");
    } else {
        strcpy(buffer, "bt_oraJiao.png");
    }
    
    CCSpriteFrame* pJiaoSpriteFrame = m_BL->m_pSpriteFrameCache->spriteFrameByName(buffer);
    // 边角1
    m_pBoxSprite->addChild(m_pCornerSprite1 = CCSprite::createWithSpriteFrame(pJiaoSpriteFrame), TEAM_DEFAULT_ZORDER + 10);
    m_pCornerSprite1->setPosition(ccp(m_pTeamLeaderSprite->boundingBox().getMinX() + m_pCornerSprite1->getContentSize().width / 2 + HERO_BOX_OFFSETX + 5.0f, m_pTeamLeaderSprite->boundingBox().getMinY() + m_pCornerSprite1->getContentSize().height / 2 + 5.0f + HERO_BOX_OFFSETY));
    // 边角2
    m_pBoxSprite->addChild(m_pCornerSprite2 = CCSprite::createWithSpriteFrame(pJiaoSpriteFrame), TEAM_DEFAULT_ZORDER + 10);
    m_pCornerSprite2->setPosition(ccp(m_pTeamLeaderSprite->boundingBox().getMaxX() - m_pCornerSprite2->getContentSize().width / 2 + HERO_BOX_OFFSETX - 40.0f, m_pTeamLeaderSprite->boundingBox().getMinY() + m_pCornerSprite2->getContentSize().height / 2 + 5.0f + HERO_BOX_OFFSETY));
    m_pCornerSprite2->setFlipX(true);
    // 边角3
    m_pBoxSprite->addChild(m_pCornerSprite3 = CCSprite::createWithSpriteFrame(pJiaoSpriteFrame), TEAM_DEFAULT_ZORDER + 10);
    m_pCornerSprite3->setPosition(ccp(m_pTeamLeaderSprite->boundingBox().getMinX() + m_pCornerSprite3->getContentSize().width / 2 + HERO_BOX_OFFSETX + 5.0f, m_pTeamLeaderSprite->boundingBox().getMaxY() + m_pCornerSprite3->getContentSize().height / 2 - 38.0f + HERO_BOX_OFFSETY));
    m_pCornerSprite3->setRotation(90.0f);
    if ((m_rarity > 0 && m_rarity <= 2) || m_rarity == 4 || m_rarity == 6 || m_rarity == 9) {
        m_pCornerSprite1->setVisible(false);
        m_pCornerSprite2->setVisible(false);
        m_pCornerSprite3->setVisible(false);
    } else if (m_rarity == 3 || m_rarity == 5 || m_rarity == 8) {
        m_pCornerSprite1->setVisible(true);
        m_pCornerSprite2->setVisible(true);
        m_pCornerSprite3->setVisible(true);
    } else if (m_rarity == 7) {
        m_pCornerSprite1->setVisible(false);
        m_pCornerSprite2->setVisible(true);
        m_pCornerSprite3->setVisible(true);
    }
    
    // 职业背景框
    if (1 == m_rarity) {
        strcpy(buffer, "LST_whiJob.png");
    } else if (2 == m_rarity || 3 == m_rarity) {
        strcpy(buffer, "LST_greJob.png");
    } else if (4 == m_rarity || 5 == m_rarity) {
        strcpy(buffer, "LST_bluJob.png");
    } else if (6 == m_rarity || 7 == m_rarity || 8 == m_rarity) {
        strcpy(buffer, "LST_purJob.png");
    } else {
        strcpy(buffer, "LST_oraJob.png");
    }
    m_pBoxSprite->addChild(m_pJobBoxSprite = CCSprite::createWithSpriteFrame(m_BL->m_pSpriteFrameCache->spriteFrameByName(buffer)), TEAM_DEFAULT_ZORDER + 10);
    m_pJobBoxSprite->setPosition(ccp(m_pTeamLeaderSprite->boundingBox().getMaxX() - m_pJobBoxSprite->getContentSize().width / 2.0f - 40.0f + HERO_BOX_OFFSETX, m_pTeamLeaderSprite->boundingBox().getMaxY() - m_pJobBoxSprite->getContentSize().height / 2.0f + HERO_BOX_OFFSETY - 2.0f));
    // 职业
    CCString* jobStr = CCString::createWithFormat("btJob%d.png", m_job);
    m_pBoxSprite->addChild(m_pJobSprite = CCSprite::createWithSpriteFrame(m_BL->m_pSpriteFrameCache->spriteFrameByName(jobStr->getCString())), TEAM_DEFAULT_ZORDER + 10);
    m_pJobSprite->setPosition(ccp(m_pTeamLeaderSprite->boundingBox().getMaxX() - m_pJobSprite->getContentSize().width / 2.0f - 42.0f + HERO_BOX_OFFSETX, m_pTeamLeaderSprite->boundingBox().getMaxY() - m_pJobSprite->getContentSize().height / 2.0f + HERO_BOX_OFFSETY - 15.0f));
    
    // 获得士兵的数量
    m_countOfSoldiers = m_BL->m_pHeroManager->getCountOfSoldiers(m_rarity, m_job, m_hpInBattle);
    for (unsigned int i = 0; i < m_countOfSoldiers; i++) {
        CCAnimation* pAni = m_BL->getSoldierAni(m_job, soldierState);
        m_pSoldiersSprite[i] = CCSprite::createWithSpriteFrame(
            ((CCAnimationFrame*)pAni->getFrames()->objectAtIndex(0))->getSpriteFrame());
        m_pSoldiersSprite[i]->setRotation(180.0f);
        // 士兵动画
        m_pSoldiersSprite[i]->runAction(CCRepeatForever::create(CCAnimate::create(pAni)));
        m_pBoxForSoldierSprite->addChild(m_pSoldiersSprite[i], TEAM_DEFAULT_ZORDER);
    } /*for*/
    
    updateSoldierPosition();
    m_isDead = false;
}

/*
 *  功能：
 *      士兵位置
 */
void BattleTeam::updateSoldierPosition() {
    unsigned int col = 0, row = 0;
    float posX = 0.0f;
    for (unsigned int i = 0; i < MAX_IN_BATTLE_SOLDIERS; i++) {
        if (NULL == m_pSoldiersSprite[i])
            continue;
        
        col = i % SOLDIERS_COUNT_PER_ROW;
        if (0 == col) {
            posX = m_pTeamLeaderSprite->getPosition().x + LEADER_AND_SOLDIER;
        } else if (1 == col) {
            posX = m_pTeamLeaderSprite->getPosition().x - SOLDIER_IMAGE_WIDTH - SOLDIERS_HORIZONTAL_SPACING + LEADER_AND_SOLDIER;
        } else if (2 == col) {
            posX = m_pTeamLeaderSprite->getPosition().x + SOLDIER_IMAGE_WIDTH + SOLDIERS_HORIZONTAL_SPACING + LEADER_AND_SOLDIER;
        } else if (3 == col) {
            posX = m_pTeamLeaderSprite->getPosition().x - (SOLDIER_IMAGE_WIDTH + SOLDIERS_HORIZONTAL_SPACING) * 2.0f + LEADER_AND_SOLDIER;
        } else if (SOLDIERS_COUNT_PER_ROW - 1 == col) {
            posX = m_pTeamLeaderSprite->getPosition().x + (SOLDIER_IMAGE_WIDTH + SOLDIERS_HORIZONTAL_SPACING) * 2.0f + LEADER_AND_SOLDIER;
        }
        row = (i - col) / SOLDIERS_COUNT_PER_ROW;
        m_pSoldiersSprite[i]->setPosition(ccp(posX - SOLDIER_OFFSET_LEFT,
                                              31.0f - HERO_HEAD_HEIGHT / 2.0f + 9.0f - SOLDIER_PADDING_HERO -
                                              SOLDIER_IMAGE_HEIGHT / 2.0f - SOLDIER_IMAGE_HEIGHT * row));
    } /*for*/
}

/*
 *  功能：
 *      恢复队伍
 */
void BattleTeam::restoreTeam() {
    CCAssert(m_pLeadObject && dynamic_cast<HeroOfPlayerItem*>(m_pLeadObject), "invalid data");
    
    HeroOfPlayerItem* pHeroOfPlayerItem = (HeroOfPlayerItem*)m_pLeadObject;
    
    // 恢复生命值
    m_hpInBattle = pHeroOfPlayerItem->getHP();
    // 获得士兵的数量
    m_countOfSoldiers = m_BL->m_pHeroManager->getCountOfSoldiers(m_rarity, m_job, m_hpInBattle);
    
    // 恢复士兵
    for (unsigned int i = 0; i < MAX_IN_BATTLE_SOLDIERS; i++) {
        if (m_pSoldiersSprite[i]) {
            m_pSoldiersSprite[i]->stopAllActions();
            if (i >= m_countOfSoldiers) {
                m_pSoldiersSprite[i]->removeFromParent();
                m_pSoldiersSprite[i] = NULL;
            } else {
                CCAnimation* pAni = m_BL->getSoldierAni(m_job, 1);
                m_pSoldiersSprite[i]->runAction(CCRepeatForever::create(CCAnimate::create(pAni)));
            }
        } else {
            if (i < m_countOfSoldiers) {
                CCAnimation* pAni = m_BL->getSoldierAni(m_job, 1);
                m_pSoldiersSprite[i] = CCSprite::createWithSpriteFrame(((CCAnimationFrame*)pAni->getFrames()->objectAtIndex(0))->getSpriteFrame());
                m_pSoldiersSprite[i]->runAction(CCRepeatForever::create(CCAnimate::create(pAni)));
                m_pSoldiersSprite[i]->setRotation(180.0f);
                m_pBoxForSoldierSprite->addChild(m_pSoldiersSprite[i], TEAM_DEFAULT_ZORDER);
            }
        }
    } /*for*/
    
    updateSoldierPosition();
}

/*
 *  功能：
 *      更新整个队伍的位置和朝向
 *  参数：
 *      originPoint     (左下角坐标)位置
 *      dir             队伍朝向
 *                              1:上    2:右
 *                              3:下    4:左
 *      yoffset         y 轴方向偏移量(战斗入场)
 */
void BattleTeam::updateTeamPosition(const CCPoint originPoint, const unsigned int dir, const float yOffset) {
    float boxX = 0.0f, boxY = 0.0f, leaderSpriteX = 0.0f, leaderSpriteY = 0.0f;
    const float yAdjust = 8.0f;
    
    switch (m_dir = dir) {
        case 1:
        {
            boxX = originPoint.x + LEADER_PADDIN + HERO_HEAD_WIDTH / 2.0f - LEADER_AND_SOLDIER + 2.0f;
            boxY = originPoint.y + BLOCK_HEIGHT - HERO_HEAD_HEIGHT / 2.0f + yOffset - 9.0f;
            // 背景框
            m_pBoxSprite->setRotation(0.0f);
            m_pBoxSprite->setPosition(ccp(boxX, boxY));
            m_pBoxForSoldierSprite->setRotation(0.0f);
            m_pBoxForSoldierSprite->setPosition(ccp(boxX, boxY));
            
            // 领队武将（怪物）精灵
            m_pTeamLeaderSprite->setRotation(0.0f);
            m_pTeamLeaderSprite->setPosition(ccp(80.0f, 40.0f));
            
            leaderSpriteX = m_pTeamLeaderSprite->boundingBox().getMaxX();
            leaderSpriteY = m_pTeamLeaderSprite->boundingBox().getMaxY();
            
            // 职业背景框
            m_pJobBoxSprite->setRotation(0.0f);
            m_pJobBoxSprite->setPosition(ccp(leaderSpriteX - m_pJobBoxSprite->getContentSize().width / 2.0f - 39.0f + HERO_BOX_OFFSETX, leaderSpriteY - m_pJobBoxSprite->getContentSize().height / 2.0f + HERO_BOX_OFFSETY - 12.0f));
            // 职业
            m_pJobSprite->setRotation(0.0f);
            m_pJobSprite->setPosition(ccp(leaderSpriteX - m_pJobSprite->getContentSize().width / 2.0f - 42.0f + HERO_BOX_OFFSETX,
                                          leaderSpriteY - m_pJobSprite->getContentSize().height / 2.0f + HERO_BOX_OFFSETY - 15.0f));
        }
            break;
        case 2:
        {
            boxX = originPoint.x + BLOCK_HEIGHT - HERO_HEAD_HEIGHT / 2.0f - 9.0f;
            boxY = originPoint.y + LEADER_PADDIN + HERO_HEAD_WIDTH / 2.0f - yAdjust + 2.0f;
            // 背景框
            m_pBoxSprite->setRotation(90.0f);
            m_pBoxSprite->setPosition(ccp(boxX, boxY));
            m_pBoxForSoldierSprite->setRotation(90.0f);
            m_pBoxForSoldierSprite->setPosition(ccp(boxX, boxY));
            
            // 领队武将（怪物）精灵
            m_pTeamLeaderSprite->setRotation(0.0f);
            m_pTeamLeaderSprite->setPosition(ccp(80.0f, 40.0f));
            leaderSpriteX = m_pTeamLeaderSprite->boundingBox().getMaxX();
            leaderSpriteY = m_pTeamLeaderSprite->boundingBox().getMaxY();
            
            // 职业背景框
            m_pJobBoxSprite->setRotation(0.0f);
            m_pJobBoxSprite->setPosition(ccp(leaderSpriteX - m_pJobBoxSprite->getContentSize().width / 2.0f - 39.0f + HERO_BOX_OFFSETX, leaderSpriteY - m_pJobBoxSprite->getContentSize().height / 2.0f + HERO_BOX_OFFSETY - 12.0f));
            // 职业
            m_pJobSprite->setRotation(0.0f);
            m_pJobSprite->setPosition(ccp(leaderSpriteX - m_pJobSprite->getContentSize().width / 2.0f - 42.0f + HERO_BOX_OFFSETX,
                                          leaderSpriteY - m_pJobSprite->getContentSize().height / 2.0f + HERO_BOX_OFFSETY - 15.0f));
        }
            break;
        case 3:
        {
            // 领头武将（怪物）
            boxX = originPoint.x + LEADER_PADDIN + HERO_HEAD_WIDTH / 2.0f - LEADER_AND_SOLDIER + 2.0f;
            boxY = originPoint.y + HERO_HEAD_HEIGHT / 2.0f + yOffset - 9.0f;
            // 背景框
            m_pBoxSprite->setRotation(180.0f);
            m_pBoxSprite->setPosition(ccp(boxX, boxY));
            m_pBoxForSoldierSprite->setRotation(180.0f);
            m_pBoxForSoldierSprite->setPosition(ccp(boxX, boxY));
            
            // 领队武将（怪物）精灵
            m_pTeamLeaderSprite->setRotation(180.0f);
            m_pTeamLeaderSprite->setPosition(ccp(80.0f, 22.0f));
            leaderSpriteX = m_pTeamLeaderSprite->boundingBox().getMinX();
            leaderSpriteY = m_pTeamLeaderSprite->boundingBox().getMinY();
            
            // 职业背景框
            m_pJobBoxSprite->setRotation(180.0f);
            m_pJobBoxSprite->setPosition(ccp(leaderSpriteX + m_pJobBoxSprite->getContentSize().width / 2.0f + 43.0f - HERO_BOX_OFFSETX, leaderSpriteY + m_pJobBoxSprite->getContentSize().height / 2.0f - HERO_BOX_OFFSETY + 11.0f));
            // 职业
            m_pJobSprite->setRotation(180.0f);
            m_pJobSprite->setPosition(ccp(leaderSpriteX + m_pJobSprite->getContentSize().width / 2.0f + HERO_BOX_OFFSETX + 6.0f,
                                          leaderSpriteY + m_pJobSprite->getContentSize().height / 2.0f - HERO_BOX_OFFSETY + 14.0f));
        }
            break;
        case 4:
        {
            // 领头武将（怪物）
            boxX = originPoint.x + LEADER_PADDIN + HERO_HEAD_HEIGHT / 2.0f - 9.0f;
            boxY = originPoint.y + LEADER_PADDIN + HERO_HEAD_WIDTH / 2.0f - yAdjust + 2.0f;
            
            // 背景框
            m_pBoxSprite->setRotation(270.0f);
            m_pBoxSprite->setPosition(ccp(boxX, boxY));
            m_pBoxForSoldierSprite->setRotation(270.0f);
            m_pBoxForSoldierSprite->setPosition(ccp(boxX, boxY));
            
            // 领队武将（怪物）精灵
            m_pTeamLeaderSprite->setRotation(0.0f);
            m_pTeamLeaderSprite->setPosition(ccp(80.0f, 40.0f));
            leaderSpriteX = m_pTeamLeaderSprite->boundingBox().getMaxX();
            leaderSpriteY = m_pTeamLeaderSprite->boundingBox().getMaxY();
            
            // 职业背景框
            m_pJobBoxSprite->setRotation(0.0f);
            m_pJobBoxSprite->setPosition(ccp(leaderSpriteX - m_pJobBoxSprite->getContentSize().width / 2.0f - 39.0f + HERO_BOX_OFFSETX, leaderSpriteY - m_pJobBoxSprite->getContentSize().height / 2.0f + HERO_BOX_OFFSETY - 12.0f));
            // 职业
            m_pJobSprite->setRotation(0.0f);
            m_pJobSprite->setPosition(ccp(leaderSpriteX - m_pJobSprite->getContentSize().width / 2.0f - 42.0f + HERO_BOX_OFFSETX,
                                          leaderSpriteY - m_pJobSprite->getContentSize().height / 2.0f + HERO_BOX_OFFSETY - 15.0f));
        }
            break;
        default:
            CCAssert(false, "invalid data");
            break;
    } /*switch*/
}

/*
 *  功能：
 *      更新士兵动画
 *  参数：
 *      state     1:普通 2:move 3:死亡
 */
void BattleTeam::updateSoldierAni(unsigned int state) {
    for (int i = 0; i < MAX_IN_BATTLE_SOLDIERS; i++) {
        if (m_pSoldiersSprite[i]) {
            m_pSoldiersSprite[i]->stopAllActions();
            m_pSoldiersSprite[i]->runAction(CCRepeatForever::create(CCAnimate::create(m_BL->getSoldierAni(m_job, state))));
        }
    } /*for*/
}

/*
 *  功能：
 *      进入战场
 *  参数：
 *      offset      偏移量
 */
void BattleTeam::teamGoIntoBattle(const float offset, const float delayTime) {
    CCAssert(m_action == 0, "invalid action");
    m_action = 21;
    
    const float x = 1;
    
    // 背景框
    m_pBoxSprite->setScale(0.7);
    CCFiniteTimeAction * spawn4 = CCSpawn::create(CCScaleTo::create(0.5f * x, 1.4f),
                                                  CCOrbitCamera::create(0.5*x, 1, 0, 0, 30, 0, 90),
                                                  CCMoveBy::create(0.5f * x, ccp(0.0f, offset - 50.0f)), NULL);
    
    CCFiniteTimeAction * spawn44 =CCSpawn::create(CCDelayTime::create(0.2*x),CCScaleTo ::create(0.2*x, 1.5),NULL);
    
    CCFiniteTimeAction * spawn444 =CCSpawn::create(CCScaleTo ::create(0.1*x, 1),CCMoveBy::create(0.1*x, ccp(0, 50.0f)),NULL);
    
    CCActionInterval * orbitcamera4 = CCOrbitCamera::create(0.0001*x, 1, 0, 0, 0, 0, 0);
    CCFiniteTimeAction * seq4 = CCSequence::create(CCRotateTo::create(0.15 * x, 3),
                                                   CCRotateTo::create(0.1*x, -3),
                                                   CCRotateTo::create(0.0001*x, 0), NULL);
    CCFiniteTimeAction * seq44 = CCSequence::create(CCDelayTime::create(delayTime),
                                                    spawn4, spawn44, spawn444,orbitcamera4,seq4, NULL);
    
    CCSpeed* plpedBox = CCSpeed::create(CCSpawn::create(seq44, NULL),
                        m_BL->m_pBattle->getBattleSpeed() >= 3 ? 3 : m_BL->m_pBattle->getBattleSpeed());
    plpedBox->setTag(TEAM_GOINTOBATTLE_TAG);
    m_pBoxSprite->runAction(plpedBox);
    
    // 士兵
    updateSoldierAni(2);
    plpedBox = CCSpeed::create(CCMoveBy::create(1.05f, ccp(0.0f, offset)),
                               m_BL->m_pBattle->getBattleSpeed() >= 3 ? 3 : m_BL->m_pBattle->getBattleSpeed());
    plpedBox->setTag(TEAM_GOINTOBATTLE_TAG);
    m_pBoxForSoldierSprite->runAction(plpedBox);
}

/*
 *  功能：
 *      队伍移动
 *  参数：
 *      dir         1: 水平方向 2: 垂直方向
 *      offset      偏移量
 *      isStay      是否原地
 *      setAction
 */
void BattleTeam::teamMove(const unsigned int dir, const float offset, const bool isStay, const bool setAction) {
    if (setAction) {
        if (m_action != 0) {
            CCLog("============= m_action = %d", m_action);
        }
        CCAssert(m_action == 0, "invalid action");
        m_action = 1;
    }
    
    float duration = JUMP_DURATION_PER_UNIT * abs(offset);
    CCPoint offsetPoint;
    if (isStay) {
        offsetPoint = ccp(0.0f, 0.0f);
    } else {
        offsetPoint = ccp(dir == 1 ? offset : 0.0f, dir == 1 ? 0.0f : offset);
    }
    unsigned int jumps = (abs(offset) - abs(offset) % JUMP_OFFSET) / JUMP_OFFSET;   // 跳跃次数
    
    // 背景框
    CCSpeed* plpedBox = CCSpeed::create(CCJumpBy::create(duration, offsetPoint, JUMP_HEIGHT, jumps),
            m_BL->m_pBattle->getBattleSpeed() >= 3 ? 3 : m_BL->m_pBattle->getBattleSpeed());
    plpedBox->setTag(TEAM_MOVE_TAG);
    m_pBoxSprite->runAction(plpedBox);
    
    // 士兵
    updateSoldierAni(2);
    plpedBox = CCSpeed::create(CCMoveBy::create(duration, offsetPoint),
                               m_BL->m_pBattle->getBattleSpeed() >= 3 ? 3 : m_BL->m_pBattle->getBattleSpeed());
    plpedBox->setTag(TEAM_MOVE_TAG);
    m_pBoxForSoldierSprite->runAction(plpedBox);
}

/*
 *  功能：
 *      判断队伍进入战场 action 是否完成
 *  返回：
 *      已经完成返回 true，否则返回 false
 */
bool BattleTeam::goIntoBattleIsDone() {
    CCAssert(m_action == 21, "invaild action");
    
    bool isgoIntoBattleDone = false;
    CCAction* pga = NULL;
    do {
        pga = m_pBoxSprite->getActionByTag(TEAM_GOINTOBATTLE_TAG);
        CC_BREAK_IF(pga != NULL);
        
        pga = m_pBoxForSoldierSprite->getActionByTag(TEAM_GOINTOBATTLE_TAG);
        CC_BREAK_IF(pga != NULL);
        
        isgoIntoBattleDone = true;
    } while (0);
    
    return isgoIntoBattleDone;
}

/*
 *  功能：
 *      判断队伍 move action 是否完成
 *  返回：
 *      已经完成返回 true，否则返回 false
 */
bool BattleTeam::moveIsDone() {
    CCAssert(m_action == 1, "invaild action");
    
    bool isMoveDone = false;
    CCAction* pla = NULL;
    do {
        pla = m_pBoxSprite->getActionByTag(TEAM_MOVE_TAG);
        CC_BREAK_IF(pla != NULL);
        
        pla = m_pBoxForSoldierSprite->getActionByTag(TEAM_MOVE_TAG);
        CC_BREAK_IF(pla != NULL);
        
        isMoveDone = true;
    } while (0);
    
    return isMoveDone;
}

/*
 * 队伍往目标位置前进途中的队伍转向回调方法
 */
void BattleTeam::goToRotateCallback(CCNode* node, void* dirAndPosIndex) {
    unsigned posIndex = (unsigned int)dirAndPosIndex % 100;
    unsigned int dir = ((unsigned int)dirAndPosIndex - posIndex) / 100;
    
    updateTeamPosition(m_BL->_getPosByPosIndex(posIndex), dir);
}

/*
 *  功能：
 *      队伍从当前所在位置移动到指定位置
 *  参数：
 *      targetPos       目标位置
 */
void BattleTeam::teamGoTo(const unsigned int targetPosIndex) {
    CCAssert(m_gotoActions->count() == 0 && m_gotoBySoldierActions->count() == 0, "error");
    CCAssert(m_posIndex != targetPosIndex, "invalid data");
    
    m_action = 27;
    
    _teamGoTo(m_posIndex, targetPosIndex);
    
    CCSpeed* plpedBox = CCSpeed::create(CCSequence::create(m_gotoActions),
            m_BL->m_pBattle->getBattleSpeed() >= 3 ? 3 : m_BL->m_pBattle->getBattleSpeed());
    plpedBox->setTag(TEAM_GOTO_TARGET_TAG);
    m_pBoxSprite->runAction(plpedBox);
    
    plpedBox = CCSpeed::create(CCSequence::create(m_gotoBySoldierActions),
            m_BL->m_pBattle->getBattleSpeed() >= 3 ? 3 : m_BL->m_pBattle->getBattleSpeed());
    plpedBox->setTag(TEAM_GOTO_TARGET_TAG);
    m_pBoxForSoldierSprite->runAction(plpedBox);
}

//
void BattleTeam::_teamGoTo(const unsigned int sourcePosIndex, const unsigned int targetPosIndex) {
    float offset = 0.0f;
    float duration = 0.0f;
    CCPoint offsetPoint = CCPointZero;
    unsigned int jumps = 0;     // 跳跃次数
    unsigned int dir = 0;
    
    const CCPoint sPos = m_BL->_getPosByPosIndex(sourcePosIndex);
    CCPoint tPos = m_BL->_getPosByPosIndex(targetPosIndex);
    
    // 是水平或垂直方向返回
    unsigned int tempDir = 0;
    if (m_BL->m_pBattle->isHorizontalPosIndex(sourcePosIndex, targetPosIndex, dir)) {
        tempDir = dir;
    }
    if (m_BL->m_pBattle->isVerticalPosIndex(sourcePosIndex, targetPosIndex, dir)) {
        tempDir = dir;
    }
    if (tempDir > 0) {
        if (1 == tempDir || 3 == tempDir) /* 垂直方向 */ {
            offset = tPos.y - sPos.y;
        } else {
            offset = tPos.x - sPos.x;
        }
        duration = JUMP_DURATION_PER_UNIT * abs(offset);
        offsetPoint = ccp(2 == tempDir || 4 == tempDir ? offset : 0.0f, 1 == tempDir || 3 == tempDir ? offset : 0.0f);
        jumps = (abs(offset) - abs(offset) % JUMP_OFFSET) / JUMP_OFFSET;   // 跳跃次数
        
        m_gotoActions->addObject(CCCallFuncND::create(this, callfuncND_selector(BattleTeam::goToRotateCallback),
                                                      (void*)(tempDir * 100 + sourcePosIndex)));
        m_gotoActions->addObject(CCJumpBy::create(duration, offsetPoint, JUMP_HEIGHT, jumps));
        m_gotoBySoldierActions->addObject(CCMoveBy::create(duration, offsetPoint));
        
        return;
    }
    
    // 获取下一个位置索引
    unsigned int nextPosIndex = m_BL->m_pBattle->getNextPosIndexHorizontal(sourcePosIndex, targetPosIndex, dir);
    BattleTeam* pBattleTeam = m_BL->_getBattleTeam(nextPosIndex);
    if (pBattleTeam) {
        nextPosIndex = m_BL->m_pBattle->getNextPosIndexVertical(sourcePosIndex, targetPosIndex, dir);
    }
    tPos = m_BL->_getPosByPosIndex(nextPosIndex);
    if (1 == dir || 3 == dir) /* 垂直方向 */ {
        offset = tPos.y - sPos.y;
    } else {
        offset = tPos.x - sPos.x;
    }
    duration = JUMP_DURATION_PER_UNIT * abs(offset);
    offsetPoint = ccp(2 == dir || 4 == dir ? offset : 0.0f, 1 == dir || 3 == dir ? offset : 0.0f);
    jumps = (abs(offset) - abs(offset) % JUMP_OFFSET) / JUMP_OFFSET;   // 跳跃次数
    
    m_gotoActions->addObject(CCCallFuncND::create(this, callfuncND_selector(BattleTeam::goToRotateCallback),
                                                  (void*)(dir * 100 + sourcePosIndex)));
    m_gotoActions->addObject(CCJumpBy::create(duration, offsetPoint, JUMP_HEIGHT, jumps));
    m_gotoBySoldierActions->addObject(CCMoveBy::create(duration, offsetPoint));
    
    _teamGoTo(nextPosIndex, targetPosIndex);
}

/*
 *  功能：
 *      队伍移动到指定位置是否已经完成
 *  返回：
 *      已经完成返回 true，否则返回 false
 */
bool BattleTeam::GoToIsDone() {
    CCAssert(m_action == 27, "invaild action");
    CCAction* pAction = m_pBoxSprite->getActionByTag(TEAM_GOTO_TARGET_TAG);
    CCAction* pAction1 = m_pBoxForSoldierSprite->getActionByTag(TEAM_GOTO_TARGET_TAG);
    
    if (NULL == pAction && NULL == pAction1) {
        m_gotoActions->removeAllObjects();
        m_gotoBySoldierActions->removeAllObjects();
        
        updateSoldierAni(1);
        teamDefaultZorder();
    }
    return (NULL == pAction);
}

/*
 *  功能：
 *      增加技能 action
 *  参数：
 *      skillSection
 *      skillTargetPosIndex                 技能释放对象位置索引
 *      pSkillItem                          技能数据
 *      isRemoteAttack                      是否远程攻击
 *      damageValue                         伤害值
 *      remainHp                            剩余生命值
 */
void BattleTeam::appendSkill(const unsigned int skillSection,
                             const unsigned int skillTargetPosIndex, SkillItem* pSkillItem,
                             const bool isRemoteAttack,
                             const int damageValue, const int remainHp) {
    CCAssert(pSkillItem != NULL, "invalid skillitem");
    
    m_skillSection = skillSection;
    if (1 == skillSection) /* 技能释放 */{
        m_action = 3;
        
        BattleTeam* pSkillTargetBattleTeam = m_BL->_getBattleTeam(skillTargetPosIndex);
        CCAssert(pSkillTargetBattleTeam, "invalid battleTeam");
        
        if (pSkillItem->getIsco()) /*是近战攻击*/ {
            attackEffect(1);
        } else {
            attackEffect(2);
        }
        
        // 获取大文件名
        std::string c = pSkillItem->getC();
        std::string pvrFile = c + ".pvr.ccz";
        std::string plistFile = c + ".plist";
        // 技能释放
        std::string cm_prefix = pSkillItem->getCm();
        // 技能释放动画帧数
        unsigned int cz = pSkillItem->getCz();
        
        CCAnimation* pAnimation = CCAnimation::create();
        char szSkillRelease[64];
        for (unsigned int i = 1; i <= cz; i++) {
            sprintf(szSkillRelease, "%s%02d.png", cm_prefix.c_str(), i);
//            CCLog("%d:技能释放帧 = %s", i, szSkillRelease);
            pAnimation->addSpriteFrame(m_BL->m_pSpriteFrameCache->spriteFrameByName(szSkillRelease));
        } /*for*/
        pAnimation->setDelayPerUnit(SKILL_RELEASE_DURATION / cz);
        
        CCPoint startPos = CCPointZero;
        CCPoint targetPos = CCPointZero;
        
        if (damageValue >= 0) /* 伤害 */ {
            if (pSkillItem->getIs()) /*全屏攻击*/ {
                startPos = targetPos  = m_BL->_getPosByPosIndex(skillTargetPosIndex);
                startPos.x += HERO_HEAD_WIDTH / 2.0f;
                
                targetPos.x = startPos.x;
                targetPos.y += BLOCK_HEIGHT / 2.0f;
                
                if (pSkillTargetBattleTeam->isEnemy()) /* 我方攻击敌方 */ {
                    startPos.y = m_BL->m_pGameState->getBottomOffset() + FULLSCREEN_SKILL_SELF_YOFFSET;
                } else /* 敌方攻击我方 */ {
                    startPos.y = CCDirector::sharedDirector()->getVisibleSize().height -
                    m_BL->m_pGameState->getTopOffset() - FULLSCREEN_SKILL_ENEMY_YOFFSET;
                }
            } else {
                startPos = m_BL->_getPosByPosIndex(m_posIndex);
                startPos.x += LEADER_PADDIN + HERO_HEAD_WIDTH / 2.0f;
                startPos.y += BLOCK_HEIGHT / 2.0f;
                
                targetPos = m_BL->_getPosByPosIndex(skillTargetPosIndex);
                targetPos.x += LEADER_PADDIN + HERO_HEAD_WIDTH / 2.0f;
                targetPos.y += BLOCK_HEIGHT / 2.0f;
            }
            
            /*
             *  调整技能释放起始位置
             */
            if (pSkillItem->getCos() != 0) {
                CCPoint vec = ccpSub(targetPos, startPos);
                CCPoint normalizeVec = ccpNormalize(vec);
                CCPoint offsetPoint = ccpMult(normalizeVec, (float)pSkillItem->getCos());
                startPos = ccpAdd(startPos, offsetPoint);
            }
        } else /*增益*/ {
            startPos = m_BL->_getPosByPosIndex(m_posIndex);
            startPos.x += LEADER_PADDIN + HERO_HEAD_WIDTH / 2.0f;
            startPos.y += BLOCK_HEIGHT / 2.0f;
        }
        
        CCSprite* pSkillSprite = CCSprite::createWithSpriteFrame(((CCAnimationFrame*)pAnimation->getFrames()->objectAtIndex(0))->getSpriteFrame());
        CCAssert(pSkillSprite, "invalid aniFrame");
        //        if (NULL == pSkillSprite) {
        //            CCLog("技能释放 list = %s, pvr = %s", plistFile.c_str(), pvrFile.c_str());
        //            for (int i = 0; i < cz; i++) {
        //                str = CCString::createWithFormat("%s%02d.png", cm_prefix.c_str(), i + 1);
        //                CCLog("(%d)技能释放帧文件名 = %s", i + 1, str->getCString());
        //            } /*for*/
        //        }
        pSkillSprite->setPosition(startPos);
        CCAnimate* panimate = CCAnimate::create(pAnimation);
        CCSpeed* pspeed = CCSpeed::create(panimate,
            m_BL->m_pBattle->getBattleSpeed() >= 3 ? 3 : m_BL->m_pBattle->getBattleSpeed());
        pspeed->setTag(SKILL_RELEASE_TAG);
        
        if (damageValue >= 0) /* 技能伤害需要调整技能释放精灵方位 */{
            pSkillSprite->setRotation(m_BL->_getSkillAngle(startPos, targetPos));
        }
        pSkillSprite->runAction(pspeed);
        m_BL->m_pSkillBatchNode->addChild(pSkillSprite);
        
        /*
         *  加入技能精灵组
         */
        m_skillSprites->addObject(pSkillSprite);
    } else if (2 == skillSection) /* 技能弹道 */ {
        m_action = 4;
        
        BattleTeam* pSkillTargetBattleTeam = m_BL->_getBattleTeam(skillTargetPosIndex);
        CCAssert(pSkillTargetBattleTeam, "invalid battleTeam");
        
        CCAssert(m_BL->m_pSkillBatchNode, "invalid SkillBatchNode");
        
        // 技能弹道帧文件前缀
        std::string bm_prefix = pSkillItem->getBm();
        // 技能弹道帧数
        const unsigned int bz = pSkillItem->getBz();
        
        CCAnimation* pAnimation = CCAnimation::create();
        char szSkillBullet[64];
        for (unsigned int i = 1; i <= bz; i++) {
            sprintf(szSkillBullet, "%s%02d.png", bm_prefix.c_str(), i);
            pAnimation->addSpriteFrame(m_BL->m_pSpriteFrameCache->spriteFrameByName(szSkillBullet));
//                        CCLog("弹道帧文件 = %s", str->getCString());
        } /*for*/
        pAnimation->setDelayPerUnit(SKILL_RUNNING_DURATION / bz);
        
        /*
         *  技能弹道起始位置和终止位置
         */
        CCPoint startPos = CCPointZero;
        CCPoint targetPos = CCPointZero;
        if (pSkillItem->getIs() && damageValue >= 0) /* 全屏技能 */ {
            startPos = targetPos = m_BL->_getPosByPosIndex(skillTargetPosIndex);
            startPos.x += HERO_HEAD_WIDTH / 2.0f;
            targetPos.x = startPos.x;
            
            if (pSkillTargetBattleTeam->isEnemy()) /* 我方攻击敌方 */ {
                startPos.y = m_BL->m_pGameState->getBottomOffset() + FULLSCREEN_SKILL_SELF_YOFFSET;
            } else /* 敌方攻击我方 */ {
                startPos.y = CCDirector::sharedDirector()->getVisibleSize().height -
                m_BL->m_pGameState->getTopOffset() - FULLSCREEN_SKILL_ENEMY_YOFFSET;
            }
        } else /* 非全屏技能 */ {
            startPos = m_BL->_getPosByPosIndex(m_posIndex);
            targetPos = m_BL->_getPosByPosIndex(skillTargetPosIndex);
            
            startPos.x += LEADER_PADDIN + HERO_HEAD_WIDTH / 2.0f;
            startPos.y += BLOCK_HEIGHT / 2.0f;
            targetPos.x += LEADER_PADDIN + HERO_HEAD_WIDTH / 2.0f;
            targetPos.y += BLOCK_HEIGHT / 2.0f;
        }
        
        // 生成弹道技能精灵
        CCSprite* pSkillSprite = CCSprite::createWithSpriteFrame(((CCAnimationFrame*)pAnimation->getFrames()->objectAtIndex(0))->getSpriteFrame());
        
        // 调整弹道技能精灵方位
//        CCLog("技能文件:%s 起点=%f,%f 终点=%f,%f 角度=%f",pSkillItem->getC().c_str(), startPos.x, startPos.y,
//              targetPos.x, targetPos.y,
//              m_BattleLayer->_getSkillAngle(startPos, targetPos));
        pSkillSprite->setRotation(m_BL->_getSkillAngle(startPos, targetPos));
        
        // 调整技能弹道距离
        if (pSkillItem->getOs() != 0) {
            CCPoint vec = ccpSub(targetPos, startPos);
            CCPoint normalizeVec = ccpNormalize(vec);
            float distance = ccpDistance(targetPos, startPos) + (float)pSkillItem->getOs();
            CCPoint offsetPoint = ccpMult(normalizeVec, distance);
            targetPos = ccpAdd(startPos, offsetPoint);
        }
        pSkillSprite->setPosition(startPos);
        // 弹道帧动画
        pSkillSprite->runAction(CCRepeatForever::create(CCAnimate::create(pAnimation)));
        // 弹道移动
        CCMoveTo* pMoveTo = CCMoveTo::create(SKILL_RUNNING_DURATION, targetPos);
        
        CCSpeed* pspeed = CCSpeed::create(pMoveTo,
            m_BL->m_pBattle->getBattleSpeed() >= 3 ? 3 : m_BL->m_pBattle->getBattleSpeed());
        pspeed->setTag(SKILL_RUNNING_TAG);
        pSkillSprite->runAction(pspeed);
        
        m_BL->m_pSkillBatchNode->addChild(pSkillSprite);
        
        /*
         *  加入技能精灵组
         */
        m_skillSprites->addObject(pSkillSprite);
    } else if (3 == skillSection) /* 技能目标 */  {
        m_action = 5;
        
        CCAssert(m_BL->m_pSkillBatchNode, "invalid SkillBatchNode");
        
        // 技能目标动画帧前缀
        std::string tm_prefix = pSkillItem->getTm();
        // 技能目标动画帧数
        unsigned int tz = pSkillItem->getTz();
        
//        CCLog("技能纹理文件 = %s", pSkillItem->getC().c_str());
        
        CCAnimation* pAnimation = CCAnimation::create();
        char szSkillTarget[64];
        for (unsigned int i = 1; i <= tz; i++) {
            sprintf(szSkillTarget, "%s%02d.png", tm_prefix.c_str(), i);
//            CCLog("%d 技能目标帧 = %s", i, str->getCString());
            pAnimation->addSpriteFrame(m_BL->m_pSpriteFrameCache->spriteFrameByName(szSkillTarget));
        } /*for*/
        pAnimation->setDelayPerUnit(SKILL_TARGET_DURATION / tz);
        
        BattleTeam* pSkillTargetBattleTeam = m_BL->_getBattleTeam(skillTargetPosIndex);
        CCAssert(pSkillTargetBattleTeam, "invalid battleTeam");
        
        /*
         *
         */
        CCPoint startPos = CCPointZero, targetPos = CCPointZero;
        
        startPos = m_BL->_getPosByPosIndex(m_posIndex);
        startPos.x += HERO_HEAD_WIDTH / 2.0f;
        startPos.y += BLOCK_HEIGHT / 2.0f;
        
        targetPos = m_BL->_getPosByPosIndex(skillTargetPosIndex);
        targetPos.x += HERO_HEAD_WIDTH / 2.0f;
        targetPos.y += BLOCK_HEIGHT / 2.0f;
        
        CCSprite* pSkillTargetSprite = CCSprite::createWithSpriteFrame(
                                                                 ((CCAnimationFrame*)pAnimation->getFrames()->objectAtIndex(0))->getSpriteFrame());
//        if (NULL == pSkillTargetSprite) {
//            std::string c = pSkillItem->getC();
//            std::string pvrFile = c + ".pvr.ccz";
//            std::string plistFile = c + ".plist";
//
//            CCLog("技能目标 list = %s, pvr = %s", plistFile.c_str(), pvrFile.c_str());
//            CCLog("技能释放 list = %s, pvr = %s", plistFile.c_str(), pvrFile.c_str());
//            for (int i = 0; i < tz; i++) {
//                str = CCString::createWithFormat("%s%02d.png", tm_prefix.c_str(), i + 1);
//                CCLog("(%d)技能目标帧文件名 = %s", i + 1, str->getCString());
//            } /*for*/
//        }
        
        CCAssert(pSkillTargetSprite, "invalid skill");
        
        pSkillTargetSprite->setPosition(targetPos);
        CCSpeed* pspeed = CCSpeed::create(CCAnimate::create(pAnimation),
                m_BL->m_pBattle->getBattleSpeed() >= 3 ? 3 : m_BL->m_pBattle->getBattleSpeed());
        pspeed->setTag(SKILL_TARGET_TAG);
        pSkillTargetSprite->runAction(pspeed);
        
        m_BL->m_pSkillBatchNode->addChild(pSkillTargetSprite);
        /*
         *  加入技能精灵组
         */
        m_skillSprites->addObject(pSkillTargetSprite);
        
        /*
         *  伤害值
         */
        CCSprite* pSkillDamageSprite = NULL;
        if (0 == damageValue) /*miss*/ {
            pSkillTargetBattleTeam->attackEffect(4);
            
            pSkillDamageSprite = CCSprite::createWithSpriteFrame(m_BL->m_pSpriteFrameCache->spriteFrameByName("BT_miss.png"));
            pSkillDamageSprite->setPosition(targetPos);
            
            CCSequence* pSequence = CCSequence::create(CCDelayTime::create(SKILL_DAMAGE_ANI_DELAY),
                                                       CCMoveBy::create(SKILL_DAMAGE_DURATION, ccp(0.0f, SKILL_DAMAGE_ANI_OFFSETY)), NULL);
            CCSpeed* pDamageSpeed = CCSpeed::create(pSequence,
                    m_BL->m_pBattle->getBattleSpeed() >= 3 ? 3 : m_BL->m_pBattle->getBattleSpeed());
            pDamageSpeed->setTag(SKILL_DAMAGE_TAG);
            pSkillDamageSprite->runAction(pDamageSpeed);
            
            m_BL->m_battleCBN->addChild(pSkillDamageSprite, 10);
            m_skillDamageSprites->addObject((CCObject*)pSkillDamageSprite);
            
            pSkillTargetBattleTeam->setTeamAction(6);
        } else {
            char szImageName[32];
            char szImagePrefix[16];
            bool isRed = false;
            if (damageValue > 0) /*伤害*/ {
                strcpy(szImagePrefix, "BT_red");
                isRed = true;
                pSkillTargetBattleTeam->attackEffect(3);
            } else {               /*加血*/
                strcpy(szImagePrefix, "BT_gre");
                pSkillTargetBattleTeam->attackEffect(5);
            }
            
            unsigned int numbers[8];
            const unsigned int count = splitInteger(abs(damageValue), numbers);
            
            CCSprite* tempSprites[16];
            float firstWidth = 0.0f, totalWidth = 0.0f;
            CCPoint point = targetPos;
            for (unsigned int i = 0; i < count; i++) {
                sprintf(szImageName, "%s%d.png", szImagePrefix, numbers[i]);
                
                pSkillDamageSprite = CCSprite::createWithSpriteFrame(m_BL->m_pSpriteFrameCache->spriteFrameByName(szImageName));
                pSkillDamageSprite->setPosition(point);
                point.x += pSkillDamageSprite->getContentSize().width;
                
                tempSprites[i] = pSkillDamageSprite;
                if (0 == i) {
                    firstWidth = pSkillDamageSprite->getContentSize().width;
                }
                totalWidth += pSkillDamageSprite->getContentSize().width;
                
                CCSequence* pSequence = CCSequence::create(CCDelayTime::create(SKILL_DAMAGE_ANI_DELAY),
                                                           CCMoveBy::create(SKILL_DAMAGE_DURATION, ccp(0.0f, SKILL_DAMAGE_ANI_OFFSETY)), NULL);
                CCSpeed* pDamageSpeed = CCSpeed::create(pSequence,
                    m_BL->m_pBattle->getBattleSpeed() >= 3 ? 3 : m_BL->m_pBattle->getBattleSpeed());
                pDamageSpeed->setTag(SKILL_DAMAGE_TAG);
                pSkillDamageSprite->runAction(pDamageSpeed);
                
                m_BL->m_battleCBN->addChild(pSkillDamageSprite, 10);
                m_skillDamageSprites->addObject((CCObject*)pSkillDamageSprite);
            } /*for*/
            /* 调整位置到中间 */
            for (unsigned int i = 0; i < count; i++) {
                CCPoint tempPoint = tempSprites[i]->getPosition();
                tempPoint.x -= totalWidth / 2.0f - firstWidth /2.0f;
                tempSprites[i]->setPosition(tempPoint);
            } /*for*/
            
            /*
             * 同时播放士兵死亡动画
             */
            pSkillTargetBattleTeam->damageTeam(remainHp);
        }
    } else {
        CCAssert(false, "invalid skillSection");
    }
}

/* 
 *  功能：
 *      调节战斗速度
 *  参数：
 *      nAction
 */
void BattleTeam::adjustBattleSpeed(const unsigned int nAction) {
    switch (nAction) {
    case 1:     // 普通移动
        {
            CCSpeed* pSpeed = (CCSpeed*)m_pBoxSprite->getActionByTag(TEAM_MOVE_TAG);
            if (pSpeed) {
                pSpeed->setSpeed(m_BL->m_pBattle->getBattleSpeed() >= 3 ? 3 : m_BL->m_pBattle->getBattleSpeed());
            }
            pSpeed = (CCSpeed*)m_pBoxForSoldierSprite->getActionByTag(TEAM_MOVE_TAG);
            if (pSpeed) {
                pSpeed->setSpeed(m_BL->m_pBattle->getBattleSpeed() >= 3 ? 3 : m_BL->m_pBattle->getBattleSpeed());
            }
        }
        break;
    case 4:         // 技能弹道
        {
            
        }
        break;
    case 6:         // 小兵死亡动画
        {
            CCSpeed* pSpeed = NULL;
            for (int i = 0; i < MAX_IN_BATTLE_SOLDIERS; i++) {
                if (m_pSoldiersSprite[i]) {
                    pSpeed = (CCSpeed*)m_pSoldiersSprite[i]->getActionByTag(DEAD_ANIMATE_TAG);
                    if (pSpeed) {
                        pSpeed->setSpeed(m_BL->m_pBattle->getBattleSpeed() >= 3 ? 3 : m_BL->m_pBattle->getBattleSpeed());
                    }
                }
            } /*for*/
        }
        break;
    case 21:    // 本方队伍入场
        {
            CCSpeed* pSpeed = (CCSpeed*)m_pBoxSprite->getActionByTag(TEAM_GOINTOBATTLE_TAG);
            if (pSpeed) {
                pSpeed->setSpeed(m_BL->m_pBattle->getBattleSpeed() >= 3 ? 3 : m_BL->m_pBattle->getBattleSpeed());
            }
            pSpeed = (CCSpeed*)m_pBoxForSoldierSprite->getActionByTag(TEAM_GOINTOBATTLE_TAG);
            if (pSpeed) {
                pSpeed->setSpeed(m_BL->m_pBattle->getBattleSpeed() >= 3 ? 3 : m_BL->m_pBattle->getBattleSpeed());
            }
        }
        break;
    default:
        break;
    }
}

/*
 *  功能：
 *      伤害或增益
 *  参数：
 *      remainHP      本队伍剩余生命值
 *  返回：
 *      true    表示是伤害，需要播放士兵死亡动画
 *      false   是增益，不需要播放士兵死亡动画
 */
bool BattleTeam::damageTeam(const int remainHP) {
    m_action = 6;
    const int oldHpInBattle = m_hpInBattle;     // 保存原来的生命值
    
    /*
     *  更新为新的生命值
     */
    m_hpInBattle = remainHP;
    
    if (oldHpInBattle == remainHP) /*missing*/ {
        m_isDamage = false;
        return false;
    }
    
    if (remainHP < oldHpInBattle) /*伤血*/{
        for (unsigned int i = 0; i < MAX_IN_BATTLE_SOLDIERS; i++) /* 清空士兵死亡标记 */ {
            m_soldierIsDead[i] = false;
        }
        if (remainHP <= 0) /*阵亡*/{
            for (unsigned int i = 0; i < MAX_IN_BATTLE_SOLDIERS; i++) {
                if (m_pSoldiersSprite[i]) {
                    // 播死亡动画
                    CCSpeed* pSpeed = CCSpeed::create(CCAnimate::create(m_BL->getSoldierAni(m_job, 3)),
                            m_BL->m_pBattle->getBattleSpeed() >= 3 ? 3 : m_BL->m_pBattle->getBattleSpeed());
                    pSpeed->setTag(DEAD_ANIMATE_TAG);
                    m_pSoldiersSprite[i]->stopAllActions();
                    m_pSoldiersSprite[i]->runAction(pSpeed);
                    m_soldierIsDead[i] = true;
                }
            }
            m_countOfSoldiers = 0;
        } else /* 还活着 */ {
            // 保存所有活着的士兵的索引号
            unsigned int soliderDeadIndex[MAX_IN_BATTLE_SOLDIERS];
            unsigned int nTempIndex = 0;
            for (unsigned int i = 0; i < MAX_IN_BATTLE_SOLDIERS; i++) {
                if (m_pSoldiersSprite[i]) {
                    soliderDeadIndex[nTempIndex++] = i;
                }
            }
            
            // 获取剩下的士兵数量
            const unsigned int countOfRemainSoldiers = m_BL->m_pHeroManager->getCountOfSoldiers(m_rarity, m_job, remainHP);
            // 计算本次死亡的士兵数量
            const unsigned int countOfDeadSoldiers = m_countOfSoldiers - countOfRemainSoldiers;
            
            if (countOfDeadSoldiers > 0) {
                // 随机挑选 deadCountOfSoldiers 个死亡士兵
                srand((unsigned)time(0));
                
                unsigned int countOfDeadActions = 0, index = 0;
                while (countOfDeadActions < countOfDeadSoldiers) {
                    index = rand() % m_countOfSoldiers;
                    
                    if (m_soldierIsDead[soliderDeadIndex[index]]) // 已经选过该士兵
                        continue;
                    
                    // 播死亡动画
                    CCSpeed* pSpeed = CCSpeed::create(CCAnimate::create(m_BL->getSoldierAni(m_job, 3)),
                                m_BL->m_pBattle->getBattleSpeed() >= 3 ? 3 : m_BL->m_pBattle->getBattleSpeed());
                    pSpeed->setTag(DEAD_ANIMATE_TAG);
                    m_pSoldiersSprite[soliderDeadIndex[index]]->stopAllActions();
                    m_pSoldiersSprite[soliderDeadIndex[index]]->runAction(pSpeed);
                    m_soldierIsDead[soliderDeadIndex[index]] = true;
                    
                    countOfDeadActions++;
                } /*while*/
            }
            // 更新士兵数量
            m_countOfSoldiers = countOfRemainSoldiers;
        }
    } else /*补血*/ {
        // 计算补血后可以拥有的士兵数量
        unsigned int countOfSoldiers = m_BL->m_pHeroManager->getCountOfSoldiers(m_rarity, m_job, remainHP);
        // 计算增加的士兵数量
        unsigned int appendSoldiers = countOfSoldiers - m_countOfSoldiers;
        if (appendSoldiers > 0) {
            // 更新士兵数量
            m_countOfSoldiers = countOfSoldiers;
            for (int i = 0; i < MAX_IN_BATTLE_SOLDIERS; i++) {
                if (NULL == m_pSoldiersSprite[i]) {
                    CCAnimation* pAni = m_BL->getSoldierAni(m_job, 1);
                    m_pSoldiersSprite[i] = CCSprite::createWithSpriteFrame(((CCAnimationFrame*)pAni->getFrames()->objectAtIndex(0))->getSpriteFrame());
                    // 士兵动画
                    m_pSoldiersSprite[i]->runAction(CCRepeatForever::create(CCAnimate::create(pAni)));
                    m_pSoldiersSprite[i]->setRotation(180.0f);
                    m_pBoxForSoldierSprite->addChild(m_pSoldiersSprite[i], TEAM_DEFAULT_ZORDER);
                    if (--appendSoldiers == 0) /*补充完毕*/{
                        break;
                    }
                }
            } /*for*/
            
            // 更新所有士兵位置
            updateSoldierPosition();
            // 更新队伍位置朝向
            updateTeamPosition(m_BL->_getPosByPosIndex(m_posIndex), m_dir);
        }
    }
    return m_isDamage = remainHP < oldHpInBattle;
}

/*
 *  功能：
 *      队伍 skill action 是否完成
 */
bool BattleTeam::skillIsDone() {
    CCAssert(m_action == 3 || m_action == 4 || m_action == 5 || m_action == 6 , "invaild action");
    
    bool isSkillDone = true;
    
    CCObject* pItem = NULL;
    CCARRAY_FOREACH(m_skillSprites, pItem) {
        CCSprite* pSprite = (CCSprite*)pItem;
        CCAction* pAction = NULL;
        if (3 == m_action) {
            pAction = pSprite->getActionByTag(SKILL_RELEASE_TAG);
            if (pAction) {
                isSkillDone = false;
                break;
            }
        } else if (4 == m_action) {
            pAction = pSprite->getActionByTag(SKILL_RUNNING_TAG);
            if (pAction) {
                isSkillDone = false;
                break;
            }
        } else if (5 == m_action) {
            pAction = pSprite->getActionByTag(SKILL_TARGET_TAG);
            if (pAction) {
                isSkillDone = false;
                break;
            }
        }
        
        pSprite->stopAllActions();
    } /*for*/
    
    return isSkillDone;
}

/*
 *  功能：
 *      清除所有技能精灵
 *  参数：
 *     clearUI     是否清除相关 UI 资源
 */
void BattleTeam::clearSkill(const bool clearUI) {
    CCObject* pObjectItem = NULL;
    CCARRAY_FOREACH(m_skillDamageSprites, pObjectItem) {
        ((CCNode*)pObjectItem)->removeFromParent();
    } /*for*/
    
    if (clearUI) {
        CCAssert(m_BL->m_pSkillBatchNode != NULL, "error");
        m_BL->m_pSkillBatchNode->removeAllChildren();
    }
    
    m_skillDamageSprites->removeAllObjects();
    m_skillSprites->removeAllObjects();
    m_skillSection = 0;
    m_action = 0;
}

/*
 *  功能：
 *      士兵死亡动画是否完成
 */
bool BattleTeam::soldierDeadAniIsDone() {
    if (!m_isDamage) /*如果是增益直接退出*/ {
        return true;
    }
    
    bool soldierDeadAniIsDone = true;
    for (int i = 0; i < MAX_IN_BATTLE_SOLDIERS; i++) {
        if (m_soldierIsDead[i]) {
            CCAction* pAction = m_pSoldiersSprite[i]->getActionByTag(DEAD_ANIMATE_TAG);
            if (pAction) {
                soldierDeadAniIsDone = false;
                break;
            }
        }
    } /*for*/
    
    if (soldierDeadAniIsDone) /*所有士兵死亡动画都播放完毕*/ {
        for (int i = 0; i < MAX_IN_BATTLE_SOLDIERS; i++) {
            if (m_soldierIsDead[i]) {
                m_pSoldiersSprite[i]->removeFromParent();
                m_pSoldiersSprite[i] = NULL;
                m_soldierIsDead[i] = false;
            }
        }
        
        if (0 == m_countOfSoldiers) /*阵亡*/ {
            m_isDead = true;
            
            m_pTeamLeaderSprite = NULL;
            m_pJobSprite = NULL;
            m_pCornerSprite1 = NULL;
            m_pCornerSprite2 = NULL;
            m_pCornerSprite3 = NULL;
            m_pJobBoxSprite = NULL;
            m_pRaritySprite = NULL;
            
            m_pBoxSprite->removeFromParent();
            m_pBoxForSoldierSprite->removeFromParent();
            m_pBoxSprite = m_pBoxForSoldierSprite = NULL;
        }
    }
    
    return soldierDeadAniIsDone;
}

/*
 *  功能：
 *      touchMove 时调整 team zorder
 */
void BattleTeam::teamTouchMoveZorder() {
    // 背景框
    m_BL->m_teamCBN->reorderChild(m_pBoxSprite, TEAM_TOUCHMOVE_ZORDER - 1);
    m_BL->m_teamCBN->reorderChild(m_pBoxForSoldierSprite, TEAM_TOUCHMOVE_ZORDER);
}

/*
 *  功能：
 *      （touch end）时候调整为标准 team zorder
 */
void BattleTeam::teamDefaultZorder() {
    // 背景框
    m_BL->m_teamCBN->reorderChild(m_pBoxSprite, TEAM_DEFAULT_ZORDER - 1);
    m_BL->m_teamCBN->reorderChild(m_pBoxForSoldierSprite, TEAM_DEFAULT_ZORDER);
}

/*
 *  功能：
 *      播放战队攻击（被攻击）特效
 *  参数：
 *      type            类型
 *                      1:近战            2:远程
 *                      3:受击            4:miss
 *                      5:补血
 */
void BattleTeam::attackEffect(const unsigned int type) {
    switch (type) {
    case 1:
        {
            CCPoint moveTo = CCPointZero, moveRestore = CCPointZero;
            if (1 == m_dir) {
                moveTo = ccp(0.0f, 65.0f);
                moveRestore = ccp(0.0f, -65.0f);
            } else if (2 == m_dir) {
                moveTo = ccp(65.0f, 0.0f);
                moveRestore = ccp(-65.0f, 0.0f);
            } else if (3 == m_dir) {
                moveTo = ccp(0, -65.0f);
                moveRestore = ccp(0.0f, 65.0f);
            } else if (4 == m_dir) {
                moveTo = ccp(-65.0f, 0.0f);
                moveRestore = ccp(65.0f, 0.0f);
            }
            const float scaleAndMove = 0.2f;
            
            teamTouchMoveZorder();
            
            CCFiniteTimeAction *spawn4 = CCSpawn::create(CCScaleTo::create(scaleAndMove, 1.4f), CCMoveBy::create(scaleAndMove, moveTo), NULL);
            CCFiniteTimeAction * spawn5 =CCSpawn::create(CCFadeOut::create(0.05f), CCMoveBy::create(0.05, moveRestore), NULL);
            m_pBoxSprite->runAction(CCSequence::create(spawn4, CCScaleTo ::create(0.00075f, 1.0f),
                                                       CCDelayTime::create(0.075f),
                                                       spawn5, CCFadeIn::create(0.075f),
                                                       CCCallFuncN::create(this, callfuncN_selector(BattleTeam::teamDefaultZorder)),
                                                       NULL));
        }
        break;
    case 2:
        {
            m_pBoxSprite->runAction(CCSequence::create(CCJumpBy::create(0.15, ccp(0, 10), 10, 1), CCJumpBy::create(0.15, ccp(0, -10), -10, 1), NULL));
        }
        break;
    case 3:
        {
            CCPoint moveTo = CCPointZero, moveRestore = CCPointZero;
            if (1 == m_dir) {
                moveTo = ccp(0.0f, -20.0f);
                moveRestore = ccp(0.0f, 20.0f);
            } else if (2 == m_dir) {
                moveTo = ccp(-20.0f, 0);
                moveRestore = ccp(20.0f, 0);
            } else if (3 == m_dir) {
                moveTo = ccp(0.0f, 20.0f);
                moveRestore = ccp(0.0f, -20.0f);
            } else if (4 == m_dir) {
                moveTo = ccp(20.0f, 0.0f);
                moveRestore = ccp(-20.0f, 0);
            }
            m_pBoxSprite->runAction(CCSequence::create(CCEaseBounceInOut::create(CCMoveBy::create(0.15f, moveTo)),
                                                       CCMoveBy::create(0.15f, moveRestore),NULL));
        }
        break;
    case 4:     // miss
        {
            CCPoint moveTo1, moveTo2, moveRestore;
            if (1 == m_dir) {
                moveTo1 = ccp(-10.0f, 0.0f); moveTo2 = ccp(20.0f, 0.0f); moveRestore = ccp(-10.0f, 0.0f);
            } else if (2 == m_dir) {
                moveTo1 = ccp(0.0f, 10.0f); moveTo2 = ccp(0.0f, -20.0f); moveRestore = ccp(0.0f, 10.0f);
            } else if (3 == m_dir) {
                moveTo1 = ccp(10.0f, 0.0f); moveTo2 = ccp(-20.0f, 0.0f); moveRestore = ccp(10.0f, 0.0f);
            } else if (4 == m_dir) {
                moveTo1 = ccp(0.0f, -10.0f); moveTo2 = ccp(0.0f, 20.0f); moveRestore = ccp(0.0f, -10.0f);
            }
            m_pBoxSprite->runAction(CCSequence::create(CCMoveBy::create(0.1f, moveTo1), CCMoveBy::create(0.1f, moveTo2),
                                                       CCMoveBy::create(0.1f, moveRestore), NULL));
        }
        break;
    case 5:     //补血
        {
            
        }
        break;
    default:
        CCAssert(false, "invalid type");
        break;
    } /*switch*/
    
}

