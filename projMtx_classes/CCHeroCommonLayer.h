//
//  CCHeroCommonLayer.h
//  hero
//
//  Created by yangjie on 2014/01/20.
//
//

#ifndef __hero__CCHeroCommonLayer__
#define __hero__CCHeroCommonLayer__

#include "cocos2d.h"

USING_NS_CC;

class CCHeroCommonLayer : public CCLayer {
public:
    CCHeroCommonLayer() {
        m_isRemoveSelf = false;
    }
public:
    /* 初始化位置 */
    virtual void initPosition();
    
    /* 开场动画 */
    virtual void openningAni();

    /* 将能响应 touch 事件的 ui 组件移动到屏幕外面 */
    virtual void moveOutTouchUICompent();

    /* 恢复能响应 touch 事件的 ui 组件的位置 */
    virtual void restoreTouchUICompent();

    /* 设置是否移出自己 */
    inline void setRemoveSelf(const bool isRemoveSelf) {m_isRemoveSelf = isRemoveSelf;}
    
protected:
    bool m_isRemoveSelf;
};

#endif /* defined(__hero__CCHeroCommonLayer__) */
