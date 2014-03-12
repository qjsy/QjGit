//
//  GameItemManager.h
//  hero
//
//  Created by yangjie on 2014/01/12.
//
//  游戏物品管理
//

#ifndef __hero__GameItemManager__
#define __hero__GameItemManager__

#include "cocos2d.h"

USING_NS_CC;

class PropItem;
class EquipItem;
class GameItemManager {
private:    /*禁止外部 new*/
    GameItemManager();
    
public:
    virtual ~GameItemManager();
    
    /* 单例模式，获取 GameItemManager 类唯一实例对象 */
    static GameItemManager* getInstance();
    
public:
    /* 清除所有游戏物品数据 */
    void removeAllGameItemData(const bool isRelease = false);
    
    /* 增加道具 */
    void appendPropItem(PropItem*);
    /* 获取道具数据 */
    PropItem* getPropItem(const unsigned int);

    /* 增加装备 */
    void appendEquipItem(EquipItem*);
    /* 获取装备数据 */
    EquipItem* getEquipItem(const unsigned int);
    
private:
    CCDictionary *m_PropsByDict;                 // 所有道具(装备id 为 key)
    CCDictionary *m_EquipsByDict;                // 所有装备(装备id 为 key)
    
};


#endif /* defined(__hero__GameItemManager__) */
