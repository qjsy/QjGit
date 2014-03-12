//
//  ChosConfirm.h
//  hero
//


#ifndef __hero__ChosConfirm__
#define __hero__ChosConfirm__
#include "CCHeroCommonLayer.h"
#include "cocos2d.h"

USING_NS_CC;

class GameState;
class ChosConfirm : public CCHeroCommonLayer {
public:
    ChosConfirm();
    virtual ~ChosConfirm();
    
public:
    virtual bool init();
    CREATE_FUNC(ChosConfirm);

public:
    /* 开场动画 */
    virtual void openningAni();
    /* 移出组件 */
    virtual void moveOutTouchUICompent();
    /* 恢复响应 */
    virtual void restoreTouchUICompent();
    /*初始化位置*/
    virtual void initPosition();
    
    /*隐藏回调*/
    void moveOutCallback();
    
    /* 选择确认 */
    void chosConfirmOk();
    
    /* 
     * 更新选择确认层ui显示
     * mode: 需要更新界面的类型
     * count: 选择的数量
     * value: 值
     */
    void updateUi(int count, int value);

private:
    GameState* m_pGameState;
};

#endif /* defined(__hero__ChosConfirm__) */