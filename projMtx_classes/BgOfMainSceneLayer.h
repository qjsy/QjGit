//
//  BgOfMainSceneLayer.h
//  hero
//
//  Created by yangjie on 2013/11/29.
//
//

#ifndef __hero__BgOfMainSceneLayer__
#define __hero__BgOfMainSceneLayer__

#include "CCHeroCommonLayer.h"
#include "common.h"

USING_NS_CC;

class GamePlayer;
class BgOfMainSceneLayer : public CCHeroCommonLayer {
public:
    BgOfMainSceneLayer();
    virtual ~BgOfMainSceneLayer();
    
public:
    virtual bool init();
    CREATE_FUNC(BgOfMainSceneLayer);
    
public:
    /* 移出动作 */
    virtual void moveOutTouchUICompent();
    /* 恢复响应动作 */
    virtual void restoreTouchUICompent();
    
public:
    /* 购买体力恢复 */
    void updateEnergy();
    /*购买元宝实时更新*/
    void updateGold();
    // 玩家选择阵营
    void confirmCamp();
private:
    // 实时刷新
    void _update();
    // 初始化UI组件
    void _initUI();
    // 调度器L:刷新玩家信息(同步更新)
    void _updatePlayerInfo();
    // 刷新玩家信息
    void _initUpdate();
private:
    GamePlayer* m_pGamePlayer;
    int cur_power;
};


#endif /* defined(__hero__BgOfMainSceneLayer__) */
