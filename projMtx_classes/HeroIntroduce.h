#ifndef __HERO_INTRODUCE_H__
#define __HERO_INTRODUCE_H__

#include "CCHeroCommonLayer.h"

USING_NS_CC;

#define     MENU_NUMBER          2           // 菜单按钮数量
#define     HERO_GRADE           5           // 武将品级

class HeroManager;
class GameState;
class HeroIntroduce : public CCHeroCommonLayer
{
public:
    HeroIntroduce();
    virtual ~HeroIntroduce();
    
    virtual bool init();
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void setVisible(bool visible);
    
    CREATE_FUNC(HeroIntroduce);
    
public:
    /*移出动作*/
    virtual void moveOutTouchUICompent();
    void moveOutDidFinish();
    /*恢复响应动作*/
    virtual void restoreTouchUICompent();
    void restoreDidFinish();
public:
    /*武将星星的显示*/
    void starShow();
    /*武将是否解锁的显示*/
    void isLocked(bool);
    void setLockSuccess(bool);
private:
    /* 响应玩家按钮选择 */
    void _onMenuButtonSelected(const unsigned int);             // 响应玩家菜单选择
    void _initMenuButton();
    void _initLocked();
    
    /*重置ui组件位置*/
    void _resetUICompentPos();
    /*更新指定武将的ui显示*/
    void _updateHeroUI(std::string);
    
private:
    HeroManager* m_pHeroManager;
    
    CCSpriteBatchNode* m_pSBN;
    CCSprite *m_menuButtonSprite[MENU_NUMBER][3];        // 菜单项按钮
    CCPoint m_menuButtonPosition[MENU_NUMBER][3];        // 菜单项按钮位置
    CCSprite * m_heroGrade[HERO_GRADE];                  // 武将品级
    CCSprite * m_heroStar[HERO_GRADE][5];                   // 武将星级
    bool m_enableTouch;                                     // 是否响应 touch
    
    bool m_isLocked;                                        // 是否锁定武将
    bool m_isOk;                                            // 是否确定
    
    int  m_touchBeganMenuIndex;

    GameState* m_GameState;
    std::string m_uniId;                                //  武将uniid
    unsigned int m_id;                                  //  武将id
};

#endif // __HERO_INTRODUCE_H__