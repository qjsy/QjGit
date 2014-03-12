//
//  BattleReportItem.cpp
//  hero
//
//  Created by 杨杰 on 14-2-24.
//
//

#include "BattleReportItem.h"

/*
 *  初始化
 */
bool BattleReportItem::init() {
    m_tp = 0;
    m_rank = 0;
    m_Name = "";
    m_ud = 0;
    m_money = 0;
    m_tm = 0;
    
    return true;
}

unsigned int BattleReportItem::getTp() {return m_tp;}
void BattleReportItem::setTp(unsigned int var) {m_tp = var;}
unsigned int BattleReportItem::getRank() {return m_rank;}
void BattleReportItem::setRank(unsigned int var) {m_rank = var;}
std::string BattleReportItem::getName() {return m_Name;}
void BattleReportItem::setName(std::string var) {m_Name = var;}
unsigned int BattleReportItem::getUd() {return m_ud;}
void BattleReportItem::setUd(unsigned int var) {m_ud = var;}
unsigned int BattleReportItem::getMoney() {return m_money;}
void BattleReportItem::setMoney(unsigned int var) {m_money = var;}
unsigned int BattleReportItem::getTm() {return m_tm;}
void BattleReportItem::setTm(unsigned int var) {m_tm = var;}
