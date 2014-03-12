//
//  SkillManager.cpp
//  hero
//
//  Created by yangjie on 2013/12/29.
//
//

#include "SkillManager.h"
#include "SkillItem.h"

/* 静态变量 */
static SkillManager* s_SkillManager = NULL;

/*
 * 单例模式，获取 SkillManager 类唯一实例对象
 */
SkillManager* SkillManager::getInstance() {
    if (NULL == s_SkillManager) {
        s_SkillManager = new SkillManager();
    }
    return s_SkillManager;
}

/*
 *  构造函数
 */
SkillManager::SkillManager() {
    m_SkillsByDict = CCDictionary::create();
    m_SkillsByDict->retain();
}

/*
 *  析构函数
 */
SkillManager::~SkillManager() {
    removeAllSkillData(true);
}

/*
 *  功能：
 *      清除所有技能数据 
 *  参数：
 *      isRelease   是否 release
 */
void SkillManager::removeAllSkillData(bool isRelease) {
    if (m_SkillsByDict) {
        m_SkillsByDict->removeAllObjects();
        if (isRelease) {
            CC_SAFE_RELEASE_NULL(m_SkillsByDict);
        }
    }
}

/* 
 *  功能：
 *      添加技能数据 
 *  参数：
 *      要添加的技能数据项
 */
void SkillManager::appendSkillItem(SkillItem* pSkillItem) {
    CCAssert(pSkillItem, "invalid skill item");
    m_SkillsByDict->setObject(static_cast<CCObject*>(pSkillItem), pSkillItem->getId());
}

/* 
 *  功能：
 *      获取技能数据 
 *  参数：
 *      skillId     技能 id
 *  返回：
 *      成功返回技能数据项，否则返回 NULL
 */
SkillItem* SkillManager::getSkillItem(const unsigned int skillId) {
    return  static_cast<SkillItem*>(m_SkillsByDict->objectForKey(skillId));
}

