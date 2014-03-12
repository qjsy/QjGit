//
//  OptionLayer.cpp
//  hero
//
// 商店层（主界面首页）
// 隶属于主场景
//

#include "OptionLayer.h"

#define OPENNING_ANI_DURING                 0.2

#define MIDDLE_IMAGE_TOP                    744.0f      // midImage 离顶部偏移量
#define MIDDLE_IMAGE_1136_TOP               832.0f
#define MYH_IMAGE_OFFSET                    -10.0f      // “我的武将” 偏移量
#define RS_IMAGE_OFFSET                     -10.0f      // 返回 出售 按钮偏移量

#define CONTAINER                           1300
#define LL_MENU_TAG                         1301        //tipMenu左边容器tag值
#define MID_MENU_TAG                        1302        //..
#define RR_MENU_TAG                         1303        //..

/*
 *  构造函数
 */
OptionLayer::OptionLayer() {
    m_is1136 = CCDirector::sharedDirector()->getVisibleSize().height > 960;
}

/*
 *  析构函数
 */
OptionLayer::~OptionLayer() {
}

/*
 *  初始化
 */
bool OptionLayer::init() {
    // super init
    if (!CCLayer::init()) {
        return false;
    }
    
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    const float midImageTopOffset = m_is1136 ? MIDDLE_IMAGE_1136_TOP : MIDDLE_IMAGE_TOP;
    float updownOffset = 0.0f;
    
    //添加容器,所有随层同步移入主场景的内容全部加入容器中
    CCNode* pContainer = CCNode::create();
    this->addChild(pContainer, 1, CONTAINER);
    // 商店背景图
    CCSprite* pBgSprite = CCSprite::create("bgOfHero.png");
    pBgSprite->setAnchorPoint(CCPointZero);
    pBgSprite->setPosition(ccp(0.0f,
                                 m_is1136 ? 196.0f : 124.0f ));
    pContainer->addChild(pBgSprite, 1);
    //加入tipMenu,不是随层同步移动的，直接加入当前层
    //
    CCNode* pLLContainer = CCNode::create();
    this->addChild(pLLContainer, 1, LL_MENU_TAG);
    //
    CCSprite* pLeftImage = CCSprite::create("ll.png");
    pLeftImage->setAnchorPoint(CCPointZero);
    pLeftImage->setPosition(ccp(0.0f, midImageTopOffset + 8.0f));
    pLLContainer->addChild(pLeftImage, 2);
    //
    CCNode* pMIDContainer = CCNode::create();
    this->addChild(pMIDContainer, 1, MID_MENU_TAG);
    //"我的武将"
    CCSprite* pMiddleImage = CCSprite::create("mid.png");
    pMiddleImage->setAnchorPoint(CCPointZero);
    pMiddleImage->setPosition(ccp(origin.x + visibleSize.width / 2 - pMiddleImage->getContentSize().width / 2,
            midImageTopOffset + 8.0f));
    pMIDContainer->addChild(pMiddleImage, 2);

    CCSprite* pMyhImage = CCSprite::create("myh.png");
    pMyhImage->setAnchorPoint(CCPointZero);
    updownOffset = midImageTopOffset + 8.0f - MYH_IMAGE_OFFSET;
    pMyhImage->setPosition(ccp(origin.x + visibleSize.width / 2 - pMyhImage->getContentSize().width / 2,
                                 updownOffset));
    pMIDContainer->addChild(pMyhImage, 3);
    //
    CCNode* pRRContainer = CCNode::create();
    this->addChild(pRRContainer, 1, RR_MENU_TAG);
    //
    CCSprite* pRightImage = CCSprite::create("rr.png");
    pRightImage->setAnchorPoint(CCPointZero);
    pRightImage->setPosition(ccp(origin.x + visibleSize.width - pRightImage->getContentSize().width,
                            midImageTopOffset + 8.0f));
    pRRContainer->addChild(pRightImage, 2);
    //按钮
    CCMenuItemImage *pSellItem = CCMenuItemImage::create( "sell.png",
                                                          "sell.png",
                                                          this,
                                                          menu_selector(OptionLayer::
                                                            option));
    CCMenu* pSellMenu = CCMenu::create(pSellItem, NULL);
    pSellMenu->setAnchorPoint(CCPointZero);
    pSellMenu->setPosition(ccp(origin.x + visibleSize.width - 48.0f,
                            updownOffset - RS_IMAGE_OFFSET + 10.0f));
    pRRContainer->addChild(pSellMenu, 3);
    
    
    return true;
}

/*
 *  开场动画
 */
void OptionLayer::openningAni() {
    float oldY = 0.0f;
    float oldX = 0.0f;
    //左tipMenu
    CCNode* pLNode = (CCNode*)this->getChildByTag(LL_MENU_TAG);
    oldY = pLNode->getPosition().y;
    //CCLog("sssss%f",oldY);
    pLNode->setPosition(ccp(-123.0f, oldY));
    pLNode->runAction(CCMoveBy::create(OPENNING_ANI_DURING, CCPointMake(123.0f, 0.0f)));
    //中间
    CCNode* pMNode = (CCNode*)this->getChildByTag(MID_MENU_TAG);
    oldX = pMNode->getPosition().x;
    oldY = pMNode->getPosition().y;
    pMNode->setPosition(ccp(oldX, oldY + 150.0f));
    pMNode->runAction(CCMoveBy::create(OPENNING_ANI_DURING, CCPointMake(oldX, -150.0f)));
    //右边
    CCNode* pRNode = (CCNode*)this->getChildByTag(RR_MENU_TAG);
    oldY = pRNode->getPosition().y;
    oldX = pRNode->getPosition().x;
    pRNode->setPosition(ccp(oldX + 123.0f, oldY));
    pRNode->runAction(CCMoveBy::create(OPENNING_ANI_DURING, CCPointMake(-123.0f, 0.0f)));
    //层的移动
    CCNode* pLayer = (CCNode*)this->getChildByTag(CONTAINER);
    oldX = pLayer->getPosition().x;
    oldY = pLayer->getPosition().y;
    pLayer->setPosition(ccp(oldX, oldY - 835.0f));
    pLayer->runAction(CCMoveBy::create(OPENNING_ANI_DURING, CCPointMake(oldX, 835.0f)));
}

void OptionLayer::option() {
    return;
}