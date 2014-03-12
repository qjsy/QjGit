//
//  SkillManager.h
//  hero
//
//  Created by yangjie on 2013/12/29.
//
//

#ifndef __hero__SkillManager__
#define __hero__SkillManager__

#include "cocos2d.h"

USING_NS_CC;

class SkillItem;
class SkillManager {
private:    /*禁止外部 new*/
    SkillManager();
    
public:
    virtual ~SkillManager();

public:
    // 单例模式，获取本类唯一实例对象
    static SkillManager* getInstance();
    
public:
    /* 清除所有技能数据 */
    void removeAllSkillData(bool isRelease = false);
    /* 添加技能数据 */
    void appendSkillItem(SkillItem*);
    /* 获取技能数据 */
    SkillItem* getSkillItem(const unsigned int);
    
private:
    CCDictionary* m_SkillsByDict;                  // 存放所有技能
};

#endif /* defined(__hero__SkillManager__) */
