//
//  HeroItem.h
//  hero
//
//  Created by yangjie on 2013-12-06.
//
//

#ifndef __hero__HeroItem__
#define __hero__HeroItem__

#include "cocos2d.h"
#include "common.h"

USING_NS_CC;

/*
 *  武将数据项
 */
class HeroItem : public CCObject {
public:
    HeroItem() {}
    virtual ~HeroItem() {}
    
public:
    virtual bool init();
    
public:
    
////////////////////////
//  以下是武将属性定义   //
///////////////////////
    CC_PROPERTY(int, m_id, Id);                                     // id
    CC_PROPERTY(std::string, m_name, Name);                         // 姓名
    CC_PROPERTY(unsigned int, m_job, Job);                          // 职业
    CC_PROPERTY(herosOfCamp, m_herosOfCamp, Camp);                  // 所属阵营
    CC_PROPERTY(unsigned int, m_rarity, Rarity);                    // 稀有度
    CC_PROPERTY(float, m_if, If);                                   // 成长武力防御
    CC_PROPERTY(float, m_is, Is);                                   // 成长法术防御
    //**************************************************
    CC_PROPERTY(float, m_ba, Ba);                                   // 基础物理攻击
    CC_PROPERTY(float, m_bp, Bp);                                   // 基础法术攻击
    CC_PROPERTY(float, m_be, Be);                                   // 基础治疗
    CC_PROPERTY(float, m_bf, Bf);                                   // 基础物理防御
    CC_PROPERTY(float, m_bs, Bs);                                   // 基础法术防御
    CC_PROPERTY(float, m_bc, Bc);                                   // 基础暴击
    CC_PROPERTY(float, m_bm, Bm);                                   // 基础闪避
    CC_PROPERTY(float, m_bi, Bi);                                   // 基础命中
    CC_PROPERTY(float, m_ia, Ia);                                   // 成长物理攻击
    CC_PROPERTY(float, m_ip, Ip);                                   // 成长法术攻击
    CC_PROPERTY(float, m_ie, Ie);                                   // 成长治疗
    CC_PROPERTY(int, m_ui, Ui);                                     // 进化所需道具
    CC_PROPERTY(int, m_ug, Ug);                                     // 升级所需金币
    CC_PROPERTY(int, m_ut, Ut);                                     // 升级产物
    CC_PROPERTY(int, m_sn, Sn);                                     // 合成所需碎片
    CC_PROPERTY(int, m_sg, Sg);                                     // 碎片合成所需金币
    CC_PROPERTY(std::string, m_Jj, Jj);                             // 简介
    
    //***************************************************
    CC_PROPERTY(unsigned int, m_ns, Ns);                            // 普通技能
    CC_PROPERTY(unsigned int, m_as, As);                            // 主动技能
    CC_PROPERTY(unsigned int, m_ps, Ps);                            // 被动技能
    CC_PROPERTY(unsigned int, m_ul, Ul);                            // 进化所需等级
    CC_PROPERTY(float, m_basicHp, BasicHp);                  // 基础 hp
    CC_PROPERTY(float, m_basicProperty, BasicProperty);             // 基础属性（基础物理攻击、基础法术、基础治疗）
    CC_PROPERTY(float, m_growthHp, GrowthHp);                // 成长 hp
    CC_PROPERTY(float, m_growthProperty, GrowthProperty);           // 成长属性（成长物理攻击、成长法术、成长治疗）
    CC_PROPERTY(std::string, m_icon, Icon);                         // 列表头像
    CC_PROPERTY(std::string, m_hIcon, HIcon);                       // hi 武将头像
    CC_PROPERTY(std::string, m_bigIcon, BIcon);                     // 整卡的头像
    CC_PROPERTY(std::string, m_fightIcon, FIcon);                   // 武将战斗头像
};

#endif /* defined(__hero__HeroItem__) */
