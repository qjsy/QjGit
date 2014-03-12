//
//  PvpSummary.h
//  hero
//
//  Created by 钱剑 on 14-2-24.
//
//

#ifndef __hero__PvpSummary__
#define __hero__PvpSummary__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

class PvpSummary : public CCLayer {
public:
    PvpSummary();
    virtual ~PvpSummary();
    
public:
    virtual bool init();
    CREATE_FUNC(PvpSummary);
    
private:
    void _click();
    void _copAni();
    void _scheduleForCop();
    
private:
    bool m_touchEnable;
    CCLabelTTF* pCop;               /*铜币显示*/
    int m_cop;                      /**/
};

#endif /* defined(__hero__PvpSummary__) */
