//
//  LoginBox.h
//  hero
//
//  Created by 钱剑 on 14-1-12.
//
//

#ifndef __hero__LoginBox__
#define __hero__LoginBox__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
class LoginBox : public CCLayer, public CCEditBoxDelegate {
public:
    LoginBox();
    virtual ~LoginBox();
    virtual bool init();
    CREATE_FUNC(LoginBox);
    
public:
    virtual void editBoxEditingDidBegin(CCEditBox* editBox);
    virtual void editBoxEditingDidEnd(CCEditBox* editBox);
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    virtual void editBoxReturn(CCEditBox* editBox);
    
public:
    std::string getContent();
    void setContent(std::string);
    
private:
    CCEditBox* editBox;
};

#endif /* defined(__hero__LoginBox__) */
