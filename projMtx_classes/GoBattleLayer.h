//	武将上阵层
//
//  GoBattleLayer.h
//  hero
//

#ifndef __hero__GoBattleLayer__
#define __hero__GoBattleLayer__

#include "CCHeroCommonLayer.h"

USING_NS_CC;

class HeroManager;
class HeroOfPlayerItem;
class GameState;
class Battle;
class GoBattleLayer : public CCHeroCommonLayer {
public:
    GoBattleLayer();
    virtual ~GoBattleLayer();
    
public:
    virtual bool init();
    CREATE_FUNC(GoBattleLayer);
    
    virtual void setVisible(bool);
    virtual void registerWithTouchDispatcher(void);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
public:
    //
    void initPosition();
    /*移出*/
    virtual void moveOutTouchUICompent();
    /*移入*/
    virtual void restoreTouchUICompent();
 
    void moveOutCallback();
    
    /* 是否在场景内 */
    inline bool isInScreen() const {return m_isInScreen;}

    /* 更新上阵武将 ui 显示 */
    void updateHeroUi();
    
    /* 铜雀台指定动作 */
    void actionForPvp();

private:
    /* 选择队长 */
    void _selectCaptain();
    
    /* 选择队员 */
    void _selectTeamer();

    /* 回调函数 */
    void _callbackForAction();
    
    /* 生成对应背包武将的精灵 */
    CCSprite* _getHeroSpriteByHeroItem(HeroOfPlayerItem*);
    /* 更新对应背包武将的精灵 ui */
    CCSprite* _updateHeroSpriteByHeroItem(HeroOfPlayerItem*, CCSprite*);
    
private:
	// 是否在场景内
    bool m_isInScreen;
    
    CCSprite* m_heroSprite[5];              // 存放武将精灵
    CCSprite* m_heroBgSprite[5];            // 存放武将背景精灵
    CCSpriteBatchNode* m_pSBN;
    
    CCSpriteFrameCache* m_pSpriteFrameCache;
    CCSprite* m_pBgSprite;
    
    HeroManager* m_pHeroManager;
    GameState* m_pGameState;
    Battle* m_pBattle;
    
    unsigned int m_touchBlank;
};

#endif /* defined(__hero__GoBattleLayer__) */
