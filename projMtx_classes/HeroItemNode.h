//
//  HeroItemNode.h
//  hero
//
//  Created by yangjie on 2013/12/13.
//
//  自定义武将显示单元，
//  用于在武将列表中浏览、选择武将
//

#ifndef __hero__HeroItemNode__
#define __hero__HeroItemNode__

#include "cocos2d.h"

USING_NS_CC;

class HeroOfPlayerItem;
class HerosLayer;
class GameState;
class HeroItemNode : public CCNode {
public:
    HeroItemNode();
    virtual ~HeroItemNode() {}
    
public:
    virtual bool init();

    CREATE_FUNC(HeroItemNode);
    
public:
    void updateUiComponent(HeroOfPlayerItem*, HerosLayer*);
    
    /* 设置模式 */
    void setMode(const unsigned int);
    
private:
    // 初始化 UI 组件
    void _initUiComponent();

private:
    GameState* m_pGameState;
};


#endif /* defined(__hero__HeroItemNode__) */
