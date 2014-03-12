//
//  HeroItem.cpp
//  hero
//
//  Created by yangjie on 2013/12/06.
//
//

#include "HeroItem.h"

/*
 * 武将数据项目初始化
 */
bool HeroItem::init() {
    m_id = 0;
    
    m_herosOfCamp = CAMP_OTHER;
    m_rarity = 0;
    
    
    return true;
}


/* 获取武将 id 号 */
int HeroItem::getId() {return m_id;}
/* 设置武将id */
void HeroItem::setId(int var) {m_id = var;}
/* 获取武将名称 */
std::string HeroItem::getName() {return m_name;}
/* 设置武将名称 */
void HeroItem::setName(std::string var) {m_name = var;}
/* 获取武将职业 */
unsigned int HeroItem::getJob() {return m_job;}
/* 设置武将职业 */
void HeroItem::setJob(unsigned int var) {m_job = var;}
/* 获取武将所属阵营 */
herosOfCamp HeroItem::getCamp() {return m_herosOfCamp;}
/* 设置武将所属阵营 */
void HeroItem::setCamp(herosOfCamp var) {m_herosOfCamp = var;}
/* 获取武将稀有度 */
unsigned int HeroItem::getRarity() {return m_rarity;}
/* 设置武将稀有度 */
void HeroItem::setRarity(unsigned int var) {m_rarity = var;}
/* 获取武将成长武力防御 */
float HeroItem::getIf() {return m_if;}
/* 设置武将成长武力防御 */
void HeroItem::setIf(float var) {m_if = var;}
/* 获取武将成长法术防御 */
float HeroItem::getIs() {return m_is;}
/* 设置武将成长法术防御 */
void HeroItem::setIs(float var) {m_is = var;}
/* 获取武将普通技能 */
unsigned int HeroItem::getNs() {return m_ns;}
/* 设置武将普通技能 */
void HeroItem::setNs(unsigned int var) {m_ns = var;}
/* 获取武将主动技能 */
unsigned int HeroItem::getAs() {return m_as;}
/* 设置武将主动技能 */
void HeroItem::setAs(unsigned int var) {m_as = var;}
/* 获取武将被动技能 */
unsigned int HeroItem::getPs() {return m_ps;}
/* 设置武将被动技能 */
void HeroItem::setPs(unsigned int var) {m_ps = var;}
/* 获取武将进化所需等级 */
unsigned int HeroItem::getUl() {return m_ul;}
/* 设置武将进化所需等级 */
void HeroItem::setUl(unsigned int var) {m_ul = var;}

/* 获取基础 hp */
float HeroItem::getBasicHp() {return m_basicHp;}
/* 设置基础 hp */
void HeroItem::setBasicHp(float var) {m_basicHp = var;}
/* 获取基础属性 */
float HeroItem::getBasicProperty() {return m_basicProperty;}
/* 设置基础属性 */
void HeroItem::setBasicProperty(float var) {m_basicProperty = var;}
/* 返回成长 hp */
float HeroItem::getGrowthHp() {return m_growthHp;}
/* 设置成长 hp */
void HeroItem::setGrowthHp(float var) {m_growthHp = var;}
/* 获取成长属性 */
float HeroItem::getGrowthProperty() {return m_growthProperty;}
/* 设置成长属性 */
void HeroItem::setGrowthProperty(float var) {m_growthProperty = var;}
/* 返回武将图片 */
std::string HeroItem::getIcon() {return m_icon;}
/* 设置武将图片 */
void HeroItem::setIcon(std::string var) {m_icon = var;}
// 获取基础物理攻击
float HeroItem::getBa() {return m_ba;}
void HeroItem::setBa(float var) {m_ba = var;}
// 获取基础法术攻击
float HeroItem::getBp() {return m_bp;}
void HeroItem::setBp(float var) {m_bp = var;}
// 获取基础治疗
float HeroItem::getBe() {return m_be;}
void HeroItem::setBe(float var) {m_be = var;}
// 获取基础物理防御
float HeroItem::getBf() {return m_bf;}
void HeroItem::setBf(float var) {m_bf = var;}
// 获取基础法术防御
float HeroItem::getBs() {return m_bs;}
void HeroItem::setBs(float var) {m_bs = var;}
// 获取基础暴击
float HeroItem::getBc() {return m_bc;}
void HeroItem::setBc(float var) {m_bc = var;}
// 获取基础闪避
float HeroItem::getBm() {return m_bm;}
void HeroItem::setBm(float var) {m_bm = var;}
// 获取基础命中
float HeroItem::getBi() {return m_bi;}
void HeroItem::setBi(float var) {m_bi = var;}
// 获取成长物理攻击
float HeroItem::getIa() {return m_ia;}
void HeroItem::setIa(float var) {m_ia = var;}
// 获取成长法术攻击
float HeroItem::getIp() {return m_ip;}
void HeroItem::setIp(float var) {m_ip = var;}
// 获取成长治疗
float HeroItem::getIe() {return m_ie;}
void HeroItem::setIe(float var) {m_ie = var;}
// 获取进化所需道具
int HeroItem::getUi() {return m_ui;}
void HeroItem::setUi(int var) {m_ui = var;}
// 获取升级所需金币
int HeroItem::getUg() {return m_ug;}
void HeroItem::setUg(int var) {m_ug = var;}
// 获取升级产物
int HeroItem::getUt() {return m_ut;}
void HeroItem::setUt(int var) {m_ut = var;}
// 获取合成所需碎片
int HeroItem::getSn() {return m_sn;}
void HeroItem::setSn(int var) {m_sn = var;}
// 获取碎片合成所需金币
int HeroItem::getSg() {return m_sg;}
void HeroItem::setSg(int var) {m_sg = var;}
// 获取简介
std::string HeroItem::getJj() {return m_Jj;}
void HeroItem::setJj(std::string var) {m_Jj = var;}
//获取武将小头像
std::string HeroItem::getHIcon(){return m_hIcon;}
//设置武将小头像
void HeroItem::setHIcon(std::string var){m_hIcon = var;}
// 获取整卡的图片
std::string HeroItem::getBIcon() {return m_bigIcon;}
// 设置整卡的图片
void HeroItem::setBIcon(std::string var) {m_bigIcon = var;}
// 获取武将战斗头像
std::string HeroItem::getFIcon() {return m_fightIcon;}
// 设置武将战斗头像
void HeroItem::setFIcon(std::string var) {m_fightIcon = var;}
