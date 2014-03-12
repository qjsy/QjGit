/*
 * 英雄卡片类
 * 根据等级和稀有度变化
 */


#ifndef __hero__CardBox__
#define __hero__CardBox__

#include "cocos2d.h"

USING_NS_CC;

class HeroOfPlayerItem;
class HeroItem;
class MosterItem;
class CardBox : public CCLayer {
public:
    CardBox();
    virtual ~CardBox();
    
public:
    virtual bool init();
    CREATE_FUNC(CardBox);

public:
	void setCard(const unsigned int mode, HeroOfPlayerItem* pItem,
                 const bool isShowLev = false, const bool isShowInfo = false,
                 const int upLevel = 0, HeroItem* pLocalItem = NULL, const bool isEvo = false);
    
    void setCard(HeroItem* pHeroItem);
    
    /* 监狱卡牌 */
    void setCardForPrison(HeroItem* pItem);
    
    void actionForLevUp();
    
    void runFadeInAction(float, float);
    
    void setOpa(int);
    
    void cardPrisonAcition(float, CCPoint);
    
    void cardOpa(int);
    
    void cardForTongQue(MosterItem* pMosterItem);
    void cardForTOngQue(HeroItem* pHeroItem);
    void tongQueAction(float dt, float delay);
    void tongQueOpa(int);
    
private:
    void _updateData();
    
private:
    int m_oldHp;
    int m_curHp;
    
    int m_oldAtt;
    int m_curAtt;
    
    int m_oldLev;
    int m_curLev;
    
    float m_expPer;
    
    int m_isEvo;
    
    bool m_isAction;
    
    HeroOfPlayerItem* m_heroOfPlayerItem;
    
    /*武将升级变化文字*/
    CCLabelTTF* m_hpLabel;
    CCLabelTTF* m_attLabel;
    CCLabelTTF* m_levLabel;
    CCProgressTimer* m_expProgress;
};

#endif /* defined(__hero__CardBox__) */