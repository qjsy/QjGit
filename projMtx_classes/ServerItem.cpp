//
//  ServerItem.cpp
//  hero
//
//  Created by yangjie on 2014/02/14.
//
//

#include "ServerItem.h"

/*
 *  初始化
 */
bool ServerItem::init() {
    m_id = 0;
    m_name = "";
    m_url = "";
    m_state = 0;
    
    return true;
}

unsigned int ServerItem::getId() {return m_id;}
void ServerItem::setId(unsigned int var) {m_id = var;}
std::string ServerItem::getName() {return m_name;}
void ServerItem::setName(std::string var) {m_name = var;}
std::string ServerItem::getUrl() {return m_url;}
void ServerItem::setUrl(std::string var) {m_url = var;}
unsigned int ServerItem::getState() {return m_state;}
void ServerItem::setState(unsigned int var) {m_state = var;}
