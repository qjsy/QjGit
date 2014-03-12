//
//  ZhuLuLayer.h
//  这是出征沙场层
//

#ifndef __hero__ZhuLuLayer__
#define __hero__ZhuLuLayer__

#include "CCHeroCommonLayer.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class ZhuLuLayer : public CCHeroCommonLayer, public CCScrollViewDelegate {
public:
    ZhuLuLayer();
    virtual ~ZhuLuLayer();
    
public:
    virtual bool init();
    CREATE_FUNC(ZhuLuLayer);

public:
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    /* 开场动画 */
    virtual void initPosition();
    /* 将能响应 touch 事件的 ui 组件移动到屏幕外面 */
    virtual void moveOutTouchUICompent();
    /* 恢复能响应 touch 事件的 ui 组件的位置 */
    virtual void restoreTouchUICompent();
    
    virtual void setVisible(bool);
    
public:
    // 预留动作
    void option();
    void optionQie();
    
    void moveOutCallback();
    // 选择关卡
    void chosLev(CCObject*);
    // 地图难度筛选按钮
    void filterCallback(CCObject*);
    // 副本选择
    void chosCopy(CCObject*);
    // 更新函数
    void update(int);
    // 选择效果callback
    void copyCallback(CCNode*, void*);
    void levCallback(CCNode*, void*);
    
    
    /* tipmenu*/
    void tipMenu(const bool isMoveIn);
private:
    void _inertiaScroll(CCTouch *pTouch, CCEvent *pEvent);
    void _autoScroll();
    void _callbackForTipMenu(CCNode*, void*);
    void _buyEnergy();
private:
    int m_filterControl;
    CCScrollView* m_scrollView;
    float m_lastTime;
    CCPoint m_lastPos;
    float m_xOffset;
    float m_tempPos;
    bool m_isMvoing;
    int m_diffcult; /* 0为普通 1为精英 2为地狱 3为活动 用于记录选择的难度 */
    
    bool m_isUpdateOne;
    
    bool m_enableTouch;
};

#endif /* defined(__hero__ZhuLuLayer__) */
