//
//  SkillItem.cpp
//  hero
//
//  Created by yangjie on 2013/12/29.
//
//

#include "SkillItem.h"

/*
 *  初始化
 */
bool SkillItem::init() {
    m_id = 0;
    m_name = "";
    m_desc = "";
    m_cm = "";
    m_cz = 0;
    m_cs = 0;
    m_bm = "";
    m_bz = 0;
    m_bs = 0.0f;
    m_is = false;
    m_bp = 0;
    m_tm = "";
    m_tz = 0;
    m_ts = 0.0f;
    m_ty = 0;
    m_lv = 0;
    m_at = 0;
    m_ap = 0;
    m_tt = 0;
    m_tn = 0;
    m_sf = 0;
    m_sc = 0;
    m_rt = 0;
    
    return true;
}

/* 释放本技能相关 ui 资源 */
void SkillItem::releaseUI() {
    CCTextureCache* pTextureCache = CCTextureCache::sharedTextureCache();
    CCSpriteFrameCache* pSpriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    
    // 获取大文件名
    
    std::string c = getC();
    std::string pvrFile = c + ".pvr.ccz";
    std::string plistFile = c + ".plist";
    
    pTextureCache->removeTextureForKey(pvrFile.c_str());
    pSpriteFrameCache->removeSpriteFramesFromFile(plistFile.c_str());
}


/* 获取和设置技能id */
unsigned int SkillItem::getId() {return m_id;}
void SkillItem::setId(unsigned int var) {m_id = var;}
/* 获取和设置技能名称 */
std::string SkillItem::getName() {return m_name;}
void SkillItem::setName(std::string var) {m_name = var;}
/* 获取和设置技能详细描述 */
std::string SkillItem::getDesc() {return m_desc;}
void SkillItem::setDesc(std::string var) {m_desc = var;}
/* 获取和设置大文件名 */
std::string SkillItem::getC() {return m_c;}
void SkillItem::setC(std::string var) {m_c = var;}
/* 获取和设置技能释放特效帧动画(文件名) */
std::string SkillItem::getCm() {return m_cm;}
void SkillItem::setCm(std::string var) {m_cm = var;}
/* 获取和设置技能释放特效帧动画帧数量 */
unsigned int SkillItem::getCz() {return m_cz;}
void SkillItem::setCz(unsigned int var) {m_cz = var;}
/* 获取和设置技能释放特效速度 */
float SkillItem::getCs() {return m_cs;}
void SkillItem::setCs(float var) {m_cs = var;}
/* 获取和设置技能弹道特效帧动画(文件名) */
std::string SkillItem::getBm() {return m_bm;}
void SkillItem::setBm(std::string var) {m_bm = var;}
/* 获取和设置技能弹道特效帧动画帧数量 */
unsigned int SkillItem::getBz() {return m_bz;}
void SkillItem::setBz(unsigned int var) {m_bz = var;}
/* 获取和设置技能弹道特效速度 */
float SkillItem::getBs() {return m_bs;}
void SkillItem::setBs(float var) {m_bs = var;}
/* 获取和设置是否全屏（仅针对技能弹道特效） */
bool SkillItem::getIs() {return m_is;}
void SkillItem::setIs(bool var) {m_is = var;}
/* 获取和设置是否近战 */
bool SkillItem::getIsco() {return m_isco;}
void SkillItem::setIsco(bool var) {m_isco = var;}
/* 获取和设置技能特效全屏方向(0:反向 1: 正向)（仅针对技能弹道特效）*/
unsigned int SkillItem::getBp() {return m_bp;}
void SkillItem::setBp(unsigned int var) {m_bp = var;}
// 获取和设置技能释放特效距离调整值
int SkillItem::getCos() {return m_cos;}
void SkillItem::setCos(int var) {m_cos = var;}
// 获取和设置技能弹道特效距离调整值
int SkillItem::getOs() {return m_os;}
void SkillItem::setOs(int var) {m_os = var;}
/* 获取和设置技能目标特效帧动画(文件名) */
std::string SkillItem::getTm() {return m_tm;}
void SkillItem::setTm(std::string var) {m_tm = var;}
/* 获取和设置技能目标特效帧动画帧数量 */
unsigned int SkillItem::getTz() {return m_tz;}
void SkillItem::setTz(unsigned int var) {m_tz = var;}
/* 获取和设置技能目标特效速度 */
float SkillItem::getTs() {return m_ts;}
void SkillItem::setTs(float var) {m_ts = var;}
/* 获取和设置技能类型(普通技能，主动技能) */
unsigned int SkillItem::getTy() {return m_ty;}
void SkillItem::setTy(unsigned int var) {m_ty = var;}
/* 获取和设置技能等级 */
unsigned int SkillItem::getLv() {return m_lv;}
void SkillItem::setLv(unsigned int var) {m_lv = var;}
/* 获取和设置攻击类型 */
unsigned int SkillItem::getAt() {return m_at;}
void SkillItem::setAt(unsigned int var) {m_at = var;}
/* 获取和设置攻击属性 */
unsigned int SkillItem::getAp() {return m_ap;}
void SkillItem::setAp(unsigned int var) {m_ap = var;}
/* 获取和设置目标选择类型 */
unsigned int SkillItem::getTt() {return m_tt;}
void SkillItem::setTt(unsigned int var) {m_tt = var;}
/* 获取和设置目标数量 */
unsigned int SkillItem::getTn() {return m_tn;}
void SkillItem::setTn(unsigned int var) {m_tn = var;}
/* 获取和设置技能首次释放的回合 */
unsigned int SkillItem::getSf() {return m_sf;}
void SkillItem::setSf(unsigned int var) {m_sf = var;}
/* 获取和设置 */
unsigned int SkillItem::getSc() {return m_sc;}
void SkillItem::setSc(unsigned int var) {m_sc = var;}
/* 获取和设置伤害倍率 */
unsigned int SkillItem::getRt() {return m_rt;}
void SkillItem::setRt(unsigned int var) {m_rt = var;}

