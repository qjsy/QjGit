//
//  LoginBox.cpp
//  hero
//
//  Created by 钱剑 on 14-1-12.
//
//

#include "LoginBox.h"

#define USERID_TAG                      100


LoginBox::LoginBox() {
    
}

LoginBox::~LoginBox() {
    CCTextureCache* pTextureCache = CCTextureCache::sharedTextureCache();
    pTextureCache->removeTextureForKey("box_bg_loginTest.png");
    pTextureCache->removeTextureForKey("input_bgTest.png");
}

bool LoginBox::init() {
    if (!CCLayer::init()) {
        return false;
    }
    
    /*输入框背景*/
//    CCSprite* spBg = CCSprite::create("test_loginBa.jpg");
//    spBg->setAnchorPoint(ccp(0, 0));
//    spBg->setPosition(ccp(0, 0));
//    spBg->setScaleY(0.4);
//    this->addChild(spBg);
    CCSprite* sp = CCSprite::create("box_bg_loginTest.png");
    sp->setAnchorPoint(ccp(0, 0));
    sp->setPosition(ccp(0, 10));
    addChild(sp);
    
    CCSize editSize = CCSizeMake(235, 31);
    //第一个size参数表示输入编辑框的大小，第二个参数九宫格是用于输入编辑框的背景
    editBox = CCEditBox::create(editSize, CCScale9Sprite::create("input_bgTest.png"));
    editBox->setAnchorPoint(ccp(0, 0));
    editBox->cocos2d::CCNode::setPosition(150, 75);
    
    //以setFont开头的有几个方法是 用于设置输入文字的字体，大小，颜色
    editBox->setFont("Arial", 20);
    
    //editBox->setFontColor(ccRED);

    //设置输入编辑框在还没有输入的时候默认的提示文字
    editBox->setPlaceHolder("  请输入您的ID...");
    
    editBox->setTouchPriority(-129);
    //同样的，也有几个对应的方法的是用于设置这些提示文字的，都是以setPlaceHolder开头的
    editBox->setPlaceholderFontColor(ccWHITE);
    
    //设置输入编辑文字的长度，一个字符为一个长度
    editBox->setMaxLength(8);
    
    //设置键盘中return键显示的字符
    editBox->setReturnType(kKeyboardReturnTypeDone);
    //包括这些选项
    //      kKeyboardReturnTypeDefault:  默认使用键盘return 类型
    //      kKeyboardReturnTypeDone:     默认使用键盘return类型为“Done”字样
    //      kKeyboardReturnTypeSend:     默认使用键盘return类型为“Send”字样
    //      kKeyboardReturnTypeSearch:   默认使用键盘return类型为“Search”字样
    //      kKeyboardReturnTypeGo:       默认使用键盘return类型为“Go”字样
    
    //设置输入编辑框的编辑类型
    editBox->setInputMode(kEditBoxInputModeNumeric);
    //包括这些选项
    //      kEditBoxInputModeAny:         开启任何文本的输入键盘,包括换行
    //      kEditBoxInputModeEmailAddr:   开启 邮件地址 输入类型键盘
    //      kEditBoxInputModeNumeric:     开启 数字符号 输入类型键盘
    //      kEditBoxInputModePhoneNumber: 开启 电话号码 输入类型键盘
    //      kEditBoxInputModeUrl:         开启 URL 输入类型键盘
    //      kEditBoxInputModeDecimal:     开启 数字 输入类型键盘，允许小数点
    //      kEditBoxInputModeSingleLine:  开启任何文本的输入键盘,不包括换行
    
    //设置该属性输入密码时为替代符
    //    editBox->setInputFlag(kEditBoxInputFlagPassword);  //如果只是简单输入字符，则不用这个设置
    //包括这些选项
    //    kEditBoxInputFlagPassword,
    //    kEditBoxInputFlagSensitive,
    //    kEditBoxInputFlagInitialCapsWord,
    //    kEditBoxInputFlagInitialCapsSentence,
    //    kEditBoxInputFlagInitialCapsAllCharacters
    
    //设置委托代理对象为当前类
    editBox->setDelegate(this);
    this->addChild(editBox, 50, USERID_TAG);
    
    
    return true;
}

void LoginBox::editBoxEditingDidBegin(CCEditBox* editBox) {
    
}

void LoginBox::editBoxEditingDidEnd(CCEditBox* editBox) {
    
}
void LoginBox::editBoxTextChanged(CCEditBox* editBox, const std::string& text) {
    
}
void LoginBox::editBoxReturn(CCEditBox* editBox) {
    
}

std::string LoginBox::getContent() {
    CCEditBox* e = (CCEditBox*)this->getChildByTag(USERID_TAG);
    return e->getText();
}

void LoginBox::setContent(std::string uid) {
    CCEditBox* e = (CCEditBox*)this->getChildByTag(USERID_TAG);
    e->setText(uid.c_str());
}

