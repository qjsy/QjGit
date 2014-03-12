//
//  battleEvent.cpp
//  hero
//
//  Created by yangjie on 2014/01/03.
//
//

#include "BattleEvent.h"
#include "BattleTeam.h"
#include "SkillItem.h"

/*
 *  构造函数
 */
BattleEvent::BattleEvent() {}

/*
 *  析构函数
 */
BattleEvent::~BattleEvent() {}

/*
 *  初始化
 */
bool BattleEvent::init() {
    m_complete = false;
    m_action = 0;
    m_currPosIndex = 0;
    m_BattleTeam = NULL;
    m_countOfWaitEvents = 0;
    
    m_moveFromPosIndex = 0;
    m_moveToPosIndex = 0;
    m_moveDirection  = 0;
    m_moveToDirection = 0;
    m_updatePosIndex = false;
    m_restoreDir = false;
    
    m_damageTargetPosIndex = 0;
    m_damageTargetBattleTeam = NULL;
    m_remoteAttack = false;
    m_skillItem = NULL;
    
    m_countOfSkillTarget = 0;
    m_moveYOffset = 0;
    
    return true;
}

bool BattleEvent::getComplete() {return m_complete;}
void BattleEvent::setComplete(bool var) {m_complete = var;}
unsigned int BattleEvent::getAction() {return m_action;}
void BattleEvent::setAction(unsigned int var) {m_action = var;}
unsigned int BattleEvent::getCurrPosIndex() {return m_currPosIndex;}
void BattleEvent::setCurrPosIndex(unsigned int var) {m_currPosIndex = var;}
bool BattleEvent::getEnemy() {return m_isEnemy;}
void BattleEvent::setEnemy(bool var) {m_isEnemy = var;}
std::string BattleEvent::getAttackTag() {return m_attackTag;}
void BattleEvent::setAttackTag(std::string var) {m_attackTag = var;}
BattleTeam* BattleEvent::getBattleTeam() {return m_BattleTeam;}
void BattleEvent::setBattleTeam(BattleTeam* var) {m_BattleTeam = var;}
unsigned int BattleEvent::getCountOfWaitEvents() {return m_countOfWaitEvents;}
void BattleEvent::setCountOfWaitEvents(unsigned int var) {m_countOfWaitEvents = var;}
unsigned int BattleEvent::getMoveFromPosIndex() {return m_moveFromPosIndex;}
void BattleEvent::setMoveFromPosIndex(unsigned int var) {m_moveFromPosIndex = var;}
unsigned int BattleEvent::getMoveToPosIndex() {return m_moveToPosIndex;}
void BattleEvent::setMoveToPosIndex(unsigned int var) {m_moveToPosIndex = var;}
unsigned int BattleEvent::getMoveDirection() {return m_moveDirection;}
void BattleEvent::setMoveDirection(unsigned int var) {m_moveDirection = var;}
unsigned int BattleEvent::getMoveToDirection() {return m_moveToDirection;}
void BattleEvent::setMoveToDirection(unsigned int var) {m_moveToDirection = var;}
bool BattleEvent::getUpdatePosIndex() {return m_updatePosIndex;}
void BattleEvent::setUpdatePosIndex(bool var) {m_updatePosIndex = var;}
bool BattleEvent::getRestoreDir() {return m_restoreDir;}
void BattleEvent::setRestoreDir(bool var) {m_restoreDir = var;}
unsigned int BattleEvent::getDamageTargetPosIndex() {return m_damageTargetPosIndex;}
void BattleEvent::setDamageTargetPosIndex(unsigned int var) {m_damageTargetPosIndex = var;}
BattleTeam* BattleEvent::getDamageTargetBattleTeam() {return m_damageTargetBattleTeam;}
void BattleEvent::setDamageTargetBattleTeam(BattleTeam* var) {m_damageTargetBattleTeam = var;}
bool BattleEvent::getRemoteAttack() {return m_remoteAttack;}
void BattleEvent::setRemoteAttack(bool var) {m_remoteAttack = var;}
SkillItem* BattleEvent::getSkillItem() {return m_skillItem;}
void BattleEvent::setSkillItem(SkillItem *var) {m_skillItem = var;}

