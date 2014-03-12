//
//  LevUpLayer.h
//  levup
//


#ifndef __hero__LevUpLayer__
#define __hero__LevUpLayer__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCHeroCommonLayer.h"
#include "common.h"

USING_NS_CC;
USING_NS_CC_EXT;

class GameState;
class HeroItem;
class LevUpLayer : public CCHeroCommonLayer, public CCScrollViewDelegate {
public:
    LevUpLayer();
    virtual ~LevUpLayer();
    
public:
    virtual bool init();
    CREATE_FUNC(LevUpLayer);
    
public:
    virtual void onEnter();
    virtual void onExit();
    virtual void setVisible(bool);
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    /*初始化位置*/
    virtual void initPosition();
    /*移出动作*/
    virtual void moveOutTouchUICompent();
    /*恢复响应动作*/
    virtual void restoreTouchUICompent();
    
public:
    // 预留动作
    void chosForUp(CCObject*);
    // tipMenu动画
    void tipMenu();
    /*callback*/
    void moveOutAndcallBack();
    
    /* 选择用于升级的被吞噬武将 */
    void onSelFoodForLevupHero();

    /* 选择需要升级或升阶的武将 */
    void onSelLevEvoUpHero();

    /* 武将升级 */
    void onHeroLevUp();

    /* 武将升阶 */
    void onHeroEvoUp();
    
    /* 更新界面的函数 */
    void update(int mode);

public:
    /* 获取升级武将 */
    char* getHeroForLevUp(char []);
    
    /* 设置升级武将 */
    void setHeroForLevUp(char []);
    
    /* 获取升阶武将 uniid*/
    char* getHeroForEvoUp(char []);
    
    /* 设置升阶武将 */
    void setHeroForEvoUp(char []);

    /* 设置升阶后武将 */
    void setHeroEvoUp(char []);
    
    // 获取已选择的升级被吞噬的武将
    unsigned int getFoodHerosForLevup(char [][HERO_UNIID_BUFFER_SIZE]);
    
    // 设置已选择的升级被吞噬的武将
    void setFoodHerosForLevup(const unsigned int, const char [][HERO_UNIID_BUFFER_SIZE]);
    
     /* 清除所有升级、升阶、被吞噬武将数据 */
    void clearHeroData();
    
    /* 播放升级动画 */
    void actionForLevUp();
    
    /*清除进化获得武将临时数据*/
    void clearEvoTempData();

    
private:
    // 当触摸事件结束时，校正 CCScrollView 的位置
    void _adjustScrollView(int dir);
    void _scrollX(CCTouch *pTouch, CCEvent *pEvent);
    //
    void _callback(CCNode*, void*);
private:
    /* 更新武将相关 UI 组件 */
    void _updateHeroUiComponent();
    
private:
    // 记录按钮状态
    bool stateForBtn;
    
    bool m_enableTouch;
    
    // 滑动页面的相关变量
    bool active;
    bool moving;
    CCPoint lastPos;
    float tempPos;
    int curIndex;
    float offset;
    CCScrollView* scrollView;
    // 是否可以进化
    bool m_isEvo;
    // 是否正在滑动
    bool m_isMoving;
    
    GameState* m_pGameState;
private:
    // 存放用于升级武将的被吞噬武将
    CCArray* m_foodHeroForLevupArray;
    // 升级武将 uniid
    char m_heroForLevUp[HERO_UNIID_BUFFER_SIZE];
    // 升阶武将 uniid
    char m_heroForEvoUp[HERO_UNIID_BUFFER_SIZE];
    // 进化后武将 uniid
    char m_heroEvoUp[HERO_UNIID_BUFFER_SIZE];
};

#endif /* defined(__hero__LevUpLayer__) */

