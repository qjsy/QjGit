//
//  MosterInBattleItem.cpp
//  hero
//
//  Created by yangjie on 2013/12/24.
//
//

#include "MosterInBattleItem.h"
#include "MosterItem.h"

/*
 *  初始化
 */
bool MosterInBattleItem::init() {
    m_id = 0;
    m_hpInBattle = 0;
    m_mosterItem = NULL;
    
    return true;
}

/* 获取和设置上阵怪物 id*/
unsigned int MosterInBattleItem::getId() {return m_id;}
void MosterInBattleItem::setId(unsigned int var) {m_id = var;}
/* 获取和设置关联的怪物数据 */
MosterItem* MosterInBattleItem::getMosterItem() {return m_mosterItem;}
void MosterInBattleItem::setMosterItem(MosterItem* var) {m_mosterItem = var;}
