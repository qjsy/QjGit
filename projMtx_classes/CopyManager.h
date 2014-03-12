//
//  CopyManager.h
//  hero
//
//  Created by yangjie on 2013/12/23.
//
//  副本管理
//

#ifndef __hero__CopyManager__
#define __hero__CopyManager__

#include "cocos2d.h"

USING_NS_CC;

class CopyDataItem;
class LevelDataItem;
class CopyManager {
private:
    CopyManager();

public:
    virtual ~CopyManager();

public:
    // 单例模式，获取本类唯一实例对象
    static CopyManager* getInstance();
    
public:
    /* 清除所有副本数据 */
    void removeAllCopyData(const bool isRelease = false);
    
    /* 添加副本数据 */
    void appendCopyData(CopyDataItem*);
    
    /* 添加关卡数据 */
    void appendLevelData(const unsigned int, LevelDataItem*);
    
    /* 获取副本数据项 */
    CopyDataItem* getCopyData(const unsigned int);
    
    /* 获取副本的关卡数量 */
    unsigned int getCountOfLevels(const unsigned int) const;
    
    /* 获取关卡数据项(id) */
    LevelDataItem* getLevelDataById(const unsigned int, const unsigned int);

    /* 获取关卡数据项(index) */
    LevelDataItem* getLevelDataByIndex(const unsigned int, const unsigned int);
    
private:
    CCDictionary* m_copys;          // 所有副本
    
};

#endif /* defined(__hero__CopyManager__) */

