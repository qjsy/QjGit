//
//  battleEvent.h
//  hero
//
//  Created by yangjie on 2014-01-03.
//
//  战场事件
//

#ifndef __hero__BattleEvent__
#define __hero__BattleEvent__

#include "cocos2d.h"

USING_NS_CC;

class BattleTeam;
class SkillItem;
class BattleEvent : public CCObject {
public:
    BattleEvent();
    virtual ~BattleEvent();
    
public:
    virtual bool init();
    CREATE_FUNC(BattleEvent);
    
    /*
     *  属性
     */
    CC_PROPERTY(bool, m_complete, Complete);                                        // 是否完成
    CC_PROPERTY(unsigned int, m_action, Action);                                    // 1:队伍移动 2:技能释放武将动画  3:技能释放
    CC_PROPERTY(unsigned int, m_currPosIndex, CurrPosIndex);                        // 战队当前所在的位置索引
    CC_PROPERTY(bool, m_isEnemy, Enemy);                                            // 是否敌方战队事件
    CC_PROPERTY(std::string, m_attackTag, AttackTag);                               // 进攻武将、怪物 tag 值
    CC_PROPERTY(BattleTeam*, m_BattleTeam, BattleTeam);                             // 战队
    CC_PROPERTY(unsigned int, m_countOfWaitEvents, CountOfWaitEvents);              // 从当前事件开始要等待的事件数量

    /* move 相关 */
    CC_PROPERTY(unsigned int, m_moveFromPosIndex, MoveFromPosIndex);                // 移动起始位置索引
    CC_PROPERTY(unsigned int, m_moveToPosIndex, MoveToPosIndex);                    // 移动目标位置索引
    CC_PROPERTY(unsigned int, m_moveDirection, MoveDirection);                      // 移动方向（队伍朝向） 1:上 2:右 3:下 4:左
    CC_PROPERTY(unsigned int, m_moveToDirection, MoveToDirection);                  // 移动到目标位置后的方向
    CC_PROPERTY(bool, m_updatePosIndex, UpdatePosIndex);                            // 是否需要改变战队位置索引
    CC_PROPERTY(bool, m_restoreDir, RestoreDir);                                    // 是否恢复原本的战队朝向
    
    /* skill 相关 */
    CC_PROPERTY(unsigned int, m_damageTargetPosIndex, DamageTargetPosIndex);        // 要伤害的目标战队的位置索引
    CC_PROPERTY(BattleTeam*, m_damageTargetBattleTeam, DamageTargetBattleTeam);     // 要伤害的目标战队
    CC_PROPERTY(bool, m_remoteAttack, RemoteAttack);                                // 是否远程攻击
    CC_PROPERTY(SkillItem*, m_skillItem, SkillItem);                                // 关联的技能数据

private:
    /*
     *   以下成员变量只在 action = 1 时有意义
     */
    int m_moveYOffset;
    
    /* 保存技能弹道和技能目标对象的数量 */
    unsigned int m_countOfSkillTarget;
    /* 保存技能目标对象的位置索引 */
    unsigned int m_skillTargetPosIndexs[6];
    /* 保存技能目标对象伤害值 */
    int m_skillTargetDamageValue[6];
    /* 保存技能目标剩余生命值 */
    int m_skillTargetRemainHp[6];
    
public:
    inline int getMoveYOffset() {return m_moveYOffset;}
    inline void setMoveYOffset(const int moveYOffset) {m_moveYOffset = moveYOffset;}
    
    inline unsigned int getCountOfSkillTarget() {return m_countOfSkillTarget;}
    inline void appendSkillTargetPosIndex(const unsigned int posIndex)
        {m_skillTargetPosIndexs[m_countOfSkillTarget++] = posIndex; }
    /*
     *  增加技能目标
     *  参数：
     *      posIndex        技能目标位置索引
     *      damageValue     伤害值
     *      remainHp        剩余生命值
     */
    inline void appendSkillTarget(const unsigned int posIndex, const int damageValue, const int remainHp) {
        m_skillTargetPosIndexs[m_countOfSkillTarget] = posIndex;
        m_skillTargetDamageValue[m_countOfSkillTarget] = damageValue;
        m_skillTargetRemainHp[m_countOfSkillTarget] = remainHp;
        m_countOfSkillTarget++;
    }
    inline unsigned int getSkillTargetPosIndex(const unsigned int index) {return m_skillTargetPosIndexs[index];}
    inline int getSkillTargetDamageValue(const unsigned int index) {return m_skillTargetDamageValue[index];}
    inline int getSkillTargetRemainHP(const unsigned int index) {return m_skillTargetRemainHp[index];}
};

#endif /* defined(__hero__BattleEvent__) */

