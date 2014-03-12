//
//  BattleReportItem.h
//  hero
//
//  Created by yangjie on 2014/02/24.
//
//  战斗报告
//

#ifndef __hero__BattleReportItem__
#define __hero__BattleReportItem__

#include "cocos2d.h"

USING_NS_CC;

class BattleReportItem : public CCObject {
public:
    BattleReportItem() {}
    virtual ~BattleReportItem() {}
    
public:
    virtual bool init();
    CREATE_FUNC(BattleReportItem);
    
public:
    /*
     *  属性定义
     */
    CC_PROPERTY(unsigned int, m_tp, Tp);                // 1:本方是发起人 2:别人打我
    CC_PROPERTY(std::string, m_Name, Name);             // 对方名称
    CC_PROPERTY(unsigned int, m_rank, Rank);            // 对方排名
    CC_PROPERTY(unsigned int, m_ud, Ud);                // 0:排名下降 1:排名上升 2:排名不变
    CC_PROPERTY(unsigned int, m_money, Money);          // 获得多少金钱
    CC_PROPERTY(unsigned int, m_tm, Tm);                // 多少秒之前发生的战斗
};

#endif /* defined(__hero__BattleReportItem__) */
