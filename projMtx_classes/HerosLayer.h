//
//  HerosLayer.h
//  hero
//
//  Created by yangjie on 2013/11/28.
//
//  武将列表层
//      隶属于主场景 MainScene
//
//  用于游戏玩家浏览武将信息、选择武将。
//

#ifndef __hero__HerosLayer__
#define __hero__HerosLayer__

#include "cocos2d.h"
#include "cocos-ext.h"

#include "CCHeroCommonLayer.h"
#include "common.h"

USING_NS_CC;
USING_NS_CC_EXT;

class HeroOfPlayerItem;
class GameState;
class HeroManager;
class HerosLayer : public CCHeroCommonLayer, public HerosTableViewDataSource, public HerosTableViewDelegate {
public:
    HerosLayer();
    virtual ~HerosLayer();
    
    friend class HeroItemNode;

public:
    virtual bool init();
    CREATE_FUNC(HerosLayer);

public:
    virtual void scrollViewDidScroll(CCScrollView*);
    virtual void scrollViewDidZoom(CCScrollView*);
    // 点击哪个 cell
    virtual void tableCellTouched(HerosTableView*, CCTableViewCell*, const float);
    // 每个 cell 的 size
    virtual CCSize cellSizeForTable(HerosTableView*);
    // 生成 cell
    virtual CCTableViewCell* tableCellAtIndex(HerosTableView*, unsigned int);
    // cell的数量
    virtual unsigned int numberOfCellsInTableView(HerosTableView*);
    // 武将列表水平方向翻页回调
    virtual void tableViewTrunPageCallback(HerosTableView* table, HerosTableViewTurnPageDirection);
    // 按下去的时候，就是高亮显示，这里可以设置高亮状态
    virtual void tableCellHighlight(HerosTableView*, CCTableViewCell*);
    // 松开的时候，取消高亮状态
    virtual void tableCellUnhighlight(HerosTableView*, CCTableViewCell*);
    
    /*初始化位置*/
    virtual void initPosition();
    /*移出动作*/
    virtual void moveOutTouchUICompent();
    /*恢复响应动作*/
    virtual void restoreTouchUICompent();

public:
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch*, CCEvent*);
    virtual void ccTouchEnded(CCTouch*, CCEvent*);
    virtual void setVisible(bool);

public:
    // 出售武将回调方法
    void saleHerosCallback(CCObject*);
    // 筛选武将回调方法
    void filterHerosCallback(CCObject*);
    /*武将列表移出隐藏回调方法*/
    void moveOutCallback();
    /*设置tipMenu*/
    void setTipMenu(const int type = 1);
    /*武将列表返回按钮*/
    void returnFromList(CCObject*);
    /**/
    void herosTableView1MovedDoneCallback();
    /**/
    void herosTableView2MovedDoneCallback();

    /* 设置选择武将模式 */
    void setSelectHeroMode(const bool selectHeroMode, const unsigned int countOfIniHeros = 0,
                                       char szIniHeros[][HERO_UNIID_BUFFER_SIZE] = NULL,
                                       unsigned int maxCountOfSelHeros = MAX_ALLOW_SELECT_HEROS,
                                       char szNotAllowHeros[][HERO_UNIID_BUFFER_SIZE] = NULL,
                           unsigned int countOfNotAllowed = 0,
                           const bool isNoConfirm = false);
    /* 获取已经选的武将 */
    unsigned int getSelectedHeros(char szHeroUniid[][HERO_UNIID_BUFFER_SIZE]);
    
    /* 设置是否排除锁定的武将 */
    inline void setExceptLock(const bool isExceptLock) { m_isExceptLock = isExceptLock;}

    /* 设置是否排除上阵武将 */
    inline void setExceptGoIntoBattle(const bool isExceptGoIntoBattle) { m_isExceptGoIntoBattle = isExceptGoIntoBattle;}

    /* 是否不需要确认 */
    inline bool isNoConfirm() {return m_isNoConfirm;}
    
private:
    /* 初始化 UI 组件 */
    void _initUiComponent();

    /* 更新页数页数 */
    void _updatePages();
    
    // 指定的武将是否被选中
    bool _heroIsSelected(char []);
    
    // 在已选英雄 uniid 数组中删除指定的 uniid
    void _removeSelectedHeroUniId(char heroUniId[]);

    /* 是否已经到了可选武将的上限 */
    inline bool _isAlreadyMaxAllowSelectedHero() {return m_countOfMaxAllowSelectHeros == m_countOfSelectedHeros;}

    // 更新筛选菜单显示
    void _updateHeroMenu();

    /* 武将列表初始筛选 */
    void _initFilter();
    
    /**/
    void clearSelectedHero();

private:
    unsigned int m_HerosTablePageNumber;                            // 武将列表当前页
    bool m_isHeroTableView1Show;                                    // 是否显示武将列表1
    bool m_isHeroTableView2Show;                                    // 是否显示武将列表2
    bool m_isTurnPage;                                              // 是否处于滑动翻页中
    
    /*
     *  选择武将模式
     */
    bool m_isSelectHeroMode;                                        // 是否是选择武将模式(缺省是浏览武将模式)
    bool m_isNoConfirm;                                             // 不需要确认
    unsigned int m_countOfMaxAllowSelectHeros;                      // 允许被选择的最大武将数量
    unsigned int m_countOfSelectedHeros;                            // 目前已经选中的武将的数量
    
    // 存放所有被选中的武将的 uniid
    char m_szSelectedHeroUniId[MAX_ALLOW_SELECT_HEROS][HERO_UNIID_BUFFER_SIZE];
    
    // 存放所有不可选武将 uniid
    char m_szNotAllowedHeroUniId[MAX_NOT_ALLOW_SELECT_HEROS][HERO_UNIID_BUFFER_SIZE];
    unsigned int m_countOfNotAllowed;                               // 不可选武将数量
    
    bool m_isExceptLock;                                            // 是否排除锁定的武将
    bool m_isExceptGoIntoBattle;                                    // 是否排除上阵武将
    
    CCMenuItem* m_HeroMenuItems[CAMP_ALL];                          // 存放筛选英雄的菜单项
    
    GameState* m_pGameState;
    HeroManager* m_pHeroManager;
};

#endif /* defined(__hero__HerosLayer__) */
