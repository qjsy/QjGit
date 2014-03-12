//
//  MainCityLayer.h
//  hero
//
//  Created by yangjie on 2013/11/28.
//  
//  层次结构如下:
//  MainCityLayer
//      LevUpLayer 升级层
//      GoBatLayer 国战层
//      YiZhLayer  驿站层
//      ZhuLuLayer 副本层
//      TieJiLayer 铁匠层
//      TongQLayer 铜雀台层
//

#ifndef __hero__MainCityLayer__
#define __hero__MainCityLayer__

#include "CCHeroCommonLayer.h"

USING_NS_CC;

#define MC_MENU_NUMBER          8           // 菜单按钮数量

class GameState;
class Battle;
class MainCityLayer : public CCHeroCommonLayer {
public:
    MainCityLayer();
    virtual ~MainCityLayer();

public:
    virtual bool init();
    virtual void registerWithTouchDispatcher(void);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    virtual void setVisible(bool visible);
    
    CREATE_FUNC(MainCityLayer);

public:
    /* 移出动作 */
    virtual void moveOutTouchUICompent();
    /* 恢复响应动作 */
    virtual void restoreTouchUICompent();
    
private:
    /* 初始化所有按钮（位置、是否显示）*/
    void _initMenuButton(void);
    
    /* 响应菜单选择 */
    void onMcMenuSelect(const unsigned int);
    
private:
    GameState* m_pGameState;
    Battle* m_pBattle;
    bool m_touchEnable;
    
    CCSpriteBatchNode* m_pSBN;
    CCSprite* m_menuButtonSprite[MC_MENU_NUMBER][3];
    CCPoint m_menuButtonPosition[MC_MENU_NUMBER][2];
    bool m_menuEnable[MC_MENU_NUMBER];
    int m_touchBeganMenuIndex;
};

#endif /* defined(__hero__MainCityLayer__) */

