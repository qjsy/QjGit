//
//  Demo.cpp
//  hero
//

#include "Demo.h"
#include "LoginLayer.h"
/*
 *  构造函数
 */
Demo::Demo() {}

/*
 *  析构函数
 */
Demo::~Demo() {
    CCTextureCache* pTextureCache = CCTextureCache::sharedTextureCache();
    pTextureCache->removeTextureForKey("des.png");
    pTextureCache->removeTextureForKey("desfont.png");
}

/*
 *  初始化
 */
bool Demo::init() {
    // super init
    if (!CCLayer::init()) {
        return false;
    }

    CCMenuItemImage *pSellItem = CCMenuItemImage::create( "des.png",
                                                          "des.png",
                                                          this,
                                                          menu_selector(Demo::
                                                            option));
    pSellItem->setAnchorPoint(ccp(0, 0));
    pSellItem->setPosition(ccp(0, 0));
    CCMenu* menu = CCMenu::create(pSellItem, NULL);
    menu->setAnchorPoint(ccp(0, 0));
    menu->setPosition(ccp(0, 0));
    addChild(menu);
    
    CCSprite* sp = CCSprite::create("desfont.png");
    sp->setAnchorPoint(ccp(0.5, 0));
    sp->setPosition(ccp(320, 50));
    addChild(sp);
    CCActionInterval * scale = CCScaleTo::create(1, 1.1);
    CCActionInterval * scale1 = CCScaleTo::create(1.5, 1);
    CCFiniteTimeAction * seq= CCSequence::create(scale, scale1, NULL);
    CCActionInterval * repeatForever =CCRepeatForever::create((CCActionInterval* )seq);
    sp->runAction(repeatForever);
    
    /**/
    LoginLayer* login = LoginLayer::create();
    login->setVisible(false);
    login->setTouchEnabled(false);
    this->addChild(login, 1, 100);

    return true;
}

/*
 *  开场动画
 */
void Demo::openningAni() {

}

void Demo::option() {
    CCLayer* l = (CCLayer*)this->getChildByTag(100);
    l->setTouchEnabled(true);
    l->setVisible(true);
}
