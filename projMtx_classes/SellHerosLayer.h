/* 出售武将 */

#ifndef __hero__SellHerosLayer__
#define __hero__SellHerosLayer__

#include "cocos2d.h"
#include "cocos-ext.h"

#include "CCHeroCommonLayer.h"
#include "common.h"

USING_NS_CC;
USING_NS_CC_EXT;

class SellHerosLayer : public CCHeroCommonLayer, public CCTableViewDelegate, CCTableViewDataSource {
public:
    SellHerosLayer();
    virtual ~SellHerosLayer();
    
public:
    virtual bool init();
    CREATE_FUNC(SellHerosLayer);

public:
    virtual void setVisible(bool);
    
    //CCTableViewDelegate继承自CCScrollViewDelegate
    virtual void scrollViewDidScroll(CCScrollView* view) {}
    virtual void scrollViewDidZoom(CCScrollView* view) {}
    //点击哪个cell
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell) {}

    //每个cell的size
    virtual cocos2d::CCSize cellSizeForTable(cocos2d::extension::CCTableView *table);
    //生成cell
    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    //cell的数量
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    
    //按下去的时候，就是高亮显示，这里可以设置高亮状态
    virtual void tableCellHighlight(CCTableView* table, CCTableViewCell* cell) {}
    //松开的时候，取消高亮状态
    virtual void tableCellUnhighlight(CCTableView* table, CCTableViewCell* cell) {}

public:
    /*移出动作*/
    virtual void moveOutTouchUICompent();
    /*恢复响应动作*/
    virtual void restoreTouchUICompent();
    //出售取消
    void cancel();
    // 设置选择的要出售的武将
    void setSellHeros(const unsigned int,const char[][HERO_UNIID_BUFFER_SIZE]);
    // 获取要出售的武将
    unsigned int getSellHeros(char[][HERO_UNIID_BUFFER_SIZE]);
    // 出售post字符串构造
    void onSellHero();
    // 更新ui界面
    void updateUi();
private:
    bool m_is1136;
    // 用于存放被选择的出售武将
    CCArray* m_herosForSellArray;
};

#endif /* defined(__hero__SellHerosLayer__) */