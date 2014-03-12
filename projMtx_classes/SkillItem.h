//
//  SkillItem.h
//  hero
//
//  Created by yangjie on 2013/12/29.
//
//

#ifndef __hero__SkillItem__
#define __hero__SkillItem__

#include "cocos2d.h"

USING_NS_CC;

class SkillItem : public CCObject {
public:
    SkillItem() {}
    virtual ~SkillItem() {}
    
public:
    virtual bool init();

public:
    /* 释放本技能相关 ui 资源 */
    void releaseUI();
    
    /*
     *  属性定义
     */
    CC_PROPERTY(unsigned int, m_id, Id);        // 技能 id
    CC_PROPERTY(std::string, m_name, Name);     // 技能名称
    CC_PROPERTY(std::string, m_desc, Desc);     // 技能详细描述
    CC_PROPERTY(std::string, m_c, C);           // 大文件名
    CC_PROPERTY(std::string, m_cm, Cm);         // 技能释放特效帧动画(文件名)
    CC_PROPERTY(unsigned int, m_cz, Cz);        // 技能释放特效帧动画帧数量
    CC_PROPERTY(float, m_cs, Cs);               // 技能释放特效帧动画速度
    CC_PROPERTY(std::string, m_bm, Bm);         // 技能弹道特效帧动画(文件名)
    CC_PROPERTY(unsigned int, m_bz, Bz);        // 技能弹道特效帧动画帧数量
    CC_PROPERTY(float, m_bs, Bs);               // 技能弹道特效帧动画速度
    CC_PROPERTY(bool, m_is, Is);                // 是否全屏（仅技能弹道特效）
    CC_PROPERTY(bool, m_isco, Isco);            // 是否近战
    CC_PROPERTY(unsigned int, m_bp, Bp);        // 技能特效全屏方向(0:反向 1: 正向)（仅技能弹道特效）
    CC_PROPERTY(int, m_cos, Cos);               // 技能释放特效距离调整值
    CC_PROPERTY(int, m_os, Os);                 // 技能弹道特效距离调整值（负数表示减少技能弹道长度，正数表示增加）
    CC_PROPERTY(std::string, m_tm, Tm);         // 技能目标特效帧动画(文件名)
    CC_PROPERTY(unsigned int, m_tz, Tz);        // 技能目标特效帧动画帧数量
    CC_PROPERTY(float, m_ts, Ts);               // 技能目标特效帧动画速度
    CC_PROPERTY(unsigned int, m_ty, Ty);        // 技能类型(普通技能，主动技能)
    CC_PROPERTY(unsigned int, m_lv, Lv);        // 技能等级
    CC_PROPERTY(unsigned int, m_at, At);        // 攻击类型 (攻击，增益)
    CC_PROPERTY(unsigned int, m_ap, Ap);        // 攻击属性（物理攻击、法术攻击）
    CC_PROPERTY(unsigned int, m_tt, Tt);        // 目标选择类型(1:横向 2:纵向 3:前至后 4:后至前 5:随机 6:全体 )
    CC_PROPERTY(unsigned int, m_tn, Tn);        // 目标数量
    CC_PROPERTY(unsigned int, m_sf, Sf);        // 技能首次释放的回合
    CC_PROPERTY(unsigned int, m_sc, Sc);        // 
    CC_PROPERTY(unsigned int, m_rt, Rt);        // 伤害倍率
};

#endif /* defined(__hero__SkillItem__) */

