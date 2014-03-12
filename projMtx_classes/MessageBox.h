//
//  MessageBox.h
//  hero
//
//  Created by 钱剑 on 14-1-14.
//
//

#ifndef __hero__MessageBox__
#define __hero__MessageBox__

#include "cocos2d.h"
USING_NS_CC;

#define RED_COLOR                       ccc3(227, 23, 13)
#define GREEN_COLOR                     ccc3(0, 255, 0)
#define BLUE_COLOR                      ccc3(30, 144, 255)
#define PURPLE_COLOR                    ccc3(160, 32, 240)

class HeroItem;
class MessageBox : public CCLayer {
    
private:
    MessageBox();
    
public:
    ~MessageBox();
    
public:
    virtual bool init();
    CREATE_FUNC(MessageBox);
    
public:
    static MessageBox* getInstance();/*本类唯一实例对象*/
    static MessageBox* create(const char* bgImage);
    void messageTips(const char* text, const int colorType = 0, const float distance = 100.0f);
    void messageForBox(const char*, int, const char*, int, CCObject*, SEL_CallFunc, float scale = 1.0);
    void messageForTqt(HeroItem* pItem);
    void messageTipsX(const char* text, const int colorType = 0, const float distance = 100.0f);
    /*监狱消息弹框*/
private:
    void _callbackForRemoveSelf();
    void _callbackConfirm();
    void _callbackCancel();
private:
    CCScene* m_Scene;
    SEL_CallFunc m_func;
    CCObject* m_target;
};

#endif /* defined(__hero__MessageBox__) */
