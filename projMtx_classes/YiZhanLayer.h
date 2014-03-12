#ifndef __hero__YiZhanLayer__
#define __hero__YiZhanLayer__

#include "CCHeroCommonLayer.h"

USING_NS_CC;

class GamePlayer;
class GameState;
class YiZhanLayer : public CCHeroCommonLayer {
public:
    YiZhanLayer();
    virtual ~YiZhanLayer();
    
public:
    virtual bool init();
    CREATE_FUNC(YiZhanLayer);

public:
    /*移出*/
    virtual void moveOutTouchUICompent();
    /*移入*/
    virtual void restoreTouchUICompent();
    virtual void initPosition();
    void moveOutCallback();
    
    /*选择抽取类型*/
    void chosForLucky(CCObject*);
    
    // 选择武将搜寻，宝物，国战寻宝按钮
    void chosTypeForSearch(CCObject*);
    /* 需要更新的ui*/
    void updateUi(unsigned int);

private:
    GameState* m_pGameState;
    GamePlayer* m_pGamePlayer;
};

#endif /* defined(__hero__YiZhanLayer__) */