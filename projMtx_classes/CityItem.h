//
//  CityItem.h
//  hero
//
//  Created by yangjie on 2014/02/28.
//
//  城池数据项
//

#ifndef __hero__CityItem__
#define __hero__CityItem__

#include "cocos2d.h"

USING_NS_CC;

class CityFunctionItem;
class CityItem : public CCObject {
public:
    CityItem();
    virtual ~CityItem();

public:
    /* 添加城池 func*/
    void appendCityFunc(CityFunctionItem*);
    /* 获取城池func 数量*/
    inline unsigned int getNumberOfCityFuncs() {return m_cityFuncs.size();}
    /* 根据 index 获取城池func */
    CityFunctionItem* getCityFuncItemByIndex(const unsigned int);
    
    /*
     *  属性定义
     */
    CC_PROPERTY(unsigned int, m_id, Id);                        // 城池id
    CC_PROPERTY(std::string , m_name, Name);                    // 城池名称
    
    CC_PROPERTY(unsigned int , m_guardNum, GuardNum);           // 城池守军数量
    CC_PROPERTY(unsigned int , m_holder, Holder);               // 城池拥有者
    /*添加城池分数*/
    void appendCityScore(unsigned int score) {m_cityScore.push_back(score);}
    unsigned int getCountOfCityScore() const {return m_cityScore.size();}
    unsigned int getCityScoreByIndex(const unsigned int index) {
        if (index < m_cityScore.size())
            return m_cityScore[index];
        else
            return 0;
    }
    
private:
    std::vector<CityFunctionItem*> m_cityFuncs;
    std::vector<unsigned int> m_cityScore;
};

#endif /* defined(__hero__CityItem__) */
