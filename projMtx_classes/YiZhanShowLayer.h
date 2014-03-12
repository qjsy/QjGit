#ifndef __hero__YiZhanShowLayer__
#define __hero__YiZhanShowLayer__

#include "cocos-ext.h"
#include "CCHeroCommonLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;

class GameState;
class GamePlayer;
class HeroManager;
class YiZhanShowLayer : public CCHeroCommonLayer {
public:
    YiZhanShowLayer();
    virtual ~YiZhanShowLayer();
    
public:
    virtual bool init();
    CREATE_FUNC(YiZhanShowLayer);
    
public:
    virtual void setVisible(bool);
    virtual void registerWithTouchDispatcher(void);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

public:
    // 免费再次抽取
    void again(CCObject*);
    // 返回函数
    void returnNow();
    //
    void update(const int);
    
    // 服务器武将数据存储函数
    inline void setHeroDataFromSvr(const char* szData)  {
        m_heroFromSvr = szData;
    }
    
    /*移出动作*/
    virtual void moveOutTouchUICompent();
    /*恢复响应动作*/
    virtual void restoreTouchUICompent();
    /*武将列表移出隐藏回调方法*/
    void moveOutCallback();

private:
    GameState* m_pGameState;
    GamePlayer* m_pGamePlayer;
    HeroManager* m_pHeroManager;
    
    // 存储服务器获得的武将数据
    const char* m_heroFromSvr;
};

#endif /* defined(__hero__YiZhanShowLayer__) */
