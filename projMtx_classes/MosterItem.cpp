//
//  MosterItem.cpp
//  hero
//
//  Created by yangjie on 2013/12/24.
//
//

#include "MosterItem.h"

/*
 *  初始化
 */
bool MosterItem::init() {
    m_id = 0;
    m_name = "";
    m_rarity = 0;
    m_job = 0;
    m_fi = "";
    
    return true;
}

/* 获取和设置id */
unsigned int MosterItem::getId() {return m_id;}
void MosterItem::setId(unsigned int var) {m_id = var;}
/* 获取和设置怪物名称 */
std::string MosterItem::getName() {return m_name;}
void MosterItem::setName(std::string var) {m_name = var;}
/* 获取和设置稀有度 */
unsigned int MosterItem::getRarity() {return m_rarity;}
void MosterItem::setRarity(unsigned int var) {m_rarity = var;}
/* 获取和设置职业 */
unsigned int MosterItem::getJob() {return m_job;}
void MosterItem::setJob(unsigned int var) {m_job= var;}



std::string MosterItem::getLi() {return m_li;}
void MosterItem::setLi(std::string var) {m_li = var;}


/* 获取和设置怪物图片 */
std::string MosterItem::getFi() {return m_fi;}
void MosterItem::setFi(std::string var) {m_fi = var;}
