//
//  BattleTeam.h
//  hero
//
//  Created by yangjie on 2013/12/31.
//
//  战斗队伍
//

#ifndef __hero__BattleTeam__
#define __hero__BattleTeam__

#include "cocos2d.h"
#include "common.h"

USING_NS_CC;

class BattleLayer;
class SkillItem;
class Battle;
class InBattleItem;
class BattleTeam : public CCObject {
public:
    BattleTeam();
    virtual ~BattleTeam();

public:
    virtual bool init();
    CREATE_FUNC(BattleTeam);

public:
    /* 设置关联武将（怪物）数据项 */
    void setLeaderObject(InBattleItem*, const unsigned int soldierState);
    /* 获取领队精灵 */
    inline CCSprite* getLeaderSprite() const {return m_pTeamLeaderSprite;}
    /* 获取和设置位置索引 */
    inline unsigned int getPosIndex(void) const {return m_posIndex;}
    inline void setPosIndex(const int posIndex) {m_posIndex = posIndex;}
    /* 获取关联的武将（怪物）数据 */
    inline InBattleItem* getLeadObject() const {return m_pLeadObject;}
    /* 设置关联的战斗 UI 层 */
    inline void setBattleLayer(BattleLayer* pBattleLayer) {m_BL = pBattleLayer;}
    /* 设置是否是敌方部队 */
    inline void setEnemy(const bool isEnemy) {m_isEnemy = isEnemy;}
    /* 获取本队伍是否是敌方部队 */
    inline bool isEnemy() const {return m_isEnemy;}
    /* 获取本队生命值 */
    inline int getHpInBattle() const {return m_hpInBattle;}

    /* 更新队伍的位置和朝向 */
    void updateTeamPosition(const CCPoint, const unsigned int, const float yoffset = 0.0f);
    
    /* 更新士兵动画 */
    void updateSoldierAni(unsigned int);
    
    /* 获取和设置 action */
    inline void setTeamAction(const unsigned int action) {m_action = action;}
    inline unsigned int getTeamAction() const {return m_action;}
    
    /* 进入战场 */
    void teamGoIntoBattle(const float, const float);

    /* 进入战场是否完成 */
    bool goIntoBattleIsDone();
    
    /* 队伍移动 */
    void teamMove(const unsigned int, const float, const bool isStay = false, const bool setAction = true);

    /* 队伍 move action 是否完成 */
    bool moveIsDone();
    
    /* 队伍从当前所在位置移动到指定位置 */
    void teamGoTo(const unsigned int);
    
    /* 队伍移动到指定位置是否已经完成 */
    bool GoToIsDone();
    
    /* 技能特效 */
    void appendSkill(const unsigned int, const unsigned int,
                     SkillItem*, const bool isRemoteAttack = false, const int damageValue = 0, const int remainHp = 0);
    
    /* 伤害或增益 */
    bool damageTeam(const int);
    
    /* 队伍 skill action 是否完成 */
    bool skillIsDone();
    
    /* 清除所有技能精灵 */
    void clearSkill(const bool clearUI = false);

    /* 士兵死亡动画是否完成 */
    bool soldierDeadAniIsDone();
   
    /* 是否阵亡 */
    bool isDead() const {return m_isDead;}
    
    /* touchMove 时调整 team zorder */
    void teamTouchMoveZorder();
    
    /*（touch end）时候调整为标准 team zorder */
    void teamDefaultZorder();
    
    /* 获取队伍朝向 */
    inline unsigned int getTeamDir() const {return m_dir;};
    
    /* 攻击特效 */
    void attackEffect(const unsigned int);
    
    /* 恢复士兵 */
    void restoreTeam();

    /* 设置士兵位置 */
    void updateSoldierPosition();
    
    /* 队伍到目标位置途中的队伍方位调整回调 */
    void goToRotateCallback(CCNode*, void*);
    
    /* 调节战斗速度 */
    void adjustBattleSpeed(const unsigned int);
    
private:
    void _teamGoTo(const unsigned int, const unsigned int);
    
private:
    unsigned int m_action;
    unsigned int m_posIndex;                                    // 所在位置索引
    unsigned int m_dir;                                         // 队伍朝向
    bool m_isEnemy;                                             // 是否敌方部队
    bool m_isDead;                                              // 是否死亡
    bool m_isDamage;                                            // true 伤害，false 增益
    int m_hpInBattle;                                           // 生命值
    
    InBattleItem* m_pLeadObject;                                // 关联武将（怪物）数据项
    CCSprite* m_pTeamLeaderSprite;                              // 带头武将精灵（怪物）
    CCSprite* m_pSoldiersSprite[MAX_IN_BATTLE_SOLDIERS];        // 士兵精灵
    bool m_soldierIsDead[MAX_IN_BATTLE_SOLDIERS];               // 保存死亡士兵索引号
    
    CCSprite* m_pBoxSprite;                                     // 背景框  (怪物武将不同)
    CCSprite* m_pBoxForSoldierSprite;                           // 背景框  (透明，用于包含士兵)
    CCSprite* m_pJobSprite;                                     // 职业
    CCSprite* m_pJobBoxSprite;                                  // 职业框(右上角 怪物武将不同)
    CCSprite* m_pCornerSprite1;                                 // 边角1  （怪物武将不同）
    CCSprite* m_pCornerSprite2;                                 // 边角2  （怪物武将不同）
    CCSprite* m_pCornerSprite3;                                 // 边角3  （怪物武将不同）
    CCSprite* m_pRaritySprite;                                  // 稀有度

    unsigned int m_countOfSoldiers;                             // 士兵数量
    CCSprite* m_pSkillSprite;                                   // 技能特效精灵
    
    unsigned int m_job;                                         // 职业
    unsigned int m_rarity;                                      // 稀有度
    
    CCArray* m_skillSprites;                                    // 存放所有技能精灵
    CCArray* m_skillDamageSprites;                              // 存放所有技能伤害精灵
    
    CCArray* m_gotoActions;                                     //
    CCArray* m_gotoBySoldierActions;                            //
    
    unsigned int m_skillSection;                                // 技能的不同阶段  1:释放    2:弹道    3:目标
    
    /* 关联的战斗 UI 层 */
    BattleLayer* m_BL;
};

#endif /* defined(__hero__BattleTeam__) */

