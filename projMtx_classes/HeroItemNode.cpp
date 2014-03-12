//
//  HeroItemNode.cpp
//  hero
//
//  Created by yangjie on 2013/12/13.
//
//

#include "HeroItemNode.h"

#include "HeroManager.h"
#include "HeroItem.h"
#include "HeroAdditionItem.h"
#include "HeroOfPlayerItem.h"
#include "HerosLayer.h"
#include "CardBox.h"
#include "GameState.h"
#include "GamePlayer.h"
#include "CCStrokeLabel.h"


#define LEFT_ITEM_OFFSET                    5

#define HERO_NAME_TAG                       1402        // 武将姓名
#define HERO_FLAG_TAG                       1405        // 武将阵营
#define HERO_STAR_TAG                       1406        // 武将星形
#define HERO_EQUI_TAG                       1407        // 武将防具
#define HERO_WEPON_TAG                      1408        // 武将武器
#define HERO_HP_TAG                         1409        // 武将血量
#define HERO_ATT_TAG                        1410        // 武将攻击力
#define HERO_LOCK_TAG                       1411        // 是否上锁
#define HERO_BOXBG_TAG                      1412        // 单元格背景
#define CARD                                1413        // 头像框
#define HERO_NAMEBOX_TAG                    1414        // 姓名框
#define HERO_ICONHP_TAG                     1416        // 血量
#define HERO_ICONATT_TAG                    1417        // 攻击力框
#define HERO_CONFIRMBG_TAG                  1418        // 确认背景
#define HERO_CONFIRM_TAG                    1419        // 确认
#define HERO_SELL_COP                       1500        // 出售获得金钱
#define HERO_SELL_ICON                      1501        // 出售获得金币icon
#define ISLEADER                            1502        // 是不是队长
#define HERO_TUN_ICON                       1503        // 被吞武将可产生经验
#define HERO_TUN_EXP                        1504        // exp
#define LEVUP_ISMAXLEV                      1505        // 已经满级
#define EVOUP_ISNOTALLOWN                   1506        // 等级不足
#define EVOUP_ISMAXEVO                      1507        // 最大等阶

/*
 * 列表模式
 * 根据调用层的tag值
 * 0                    武将浏览模式
 * 112                  出售列表模式
 * 116                  上阵列表模式
 * 117                  升级模式
 * 118                  升级被吞武将模式
 * 119                  升阶模式
 */


#define FONT                                "Arial"     //字体


/*
 * 构造函数
 */
HeroItemNode::HeroItemNode()
:m_pGameState(GameState::getInstance())
{
}

/*
 *  初始化
 */
bool HeroItemNode::init() {
    //////////////////////////////
    // 1. super init first
    if (!CCNode::init()) {
        return false;
    }
    
    /*
     *  初始化 UI 组件
     */
    _initUiComponent();
    
    return true;
}

/*
 *  初始化 UI 组件
 */
void HeroItemNode::_initUiComponent() {
    // 静态背景图
    CCSprite* pBg = CCSprite::create("HL_Bg.png");
    pBg->setAnchorPoint(CCPointZero);
    pBg->setPosition(ccp(LEFT_ITEM_OFFSET - 2.0f, 0.0f));
    this->addChild(pBg, 1, HERO_BOXBG_TAG);
    
    // cardBox
    CardBox* card = CardBox::create();
    card->setAnchorPoint(ccp(0, 0));
    card->setPosition(ccp(LEFT_ITEM_OFFSET, 0));
    this->addChild(card, 4, CARD);
    
    // 姓名框
    CCSprite* pNameBox = CCSprite::create("name_box.png");
    pNameBox->setAnchorPoint(CCPointZero);
    pNameBox->setPosition(ccp(LEFT_ITEM_OFFSET + 143.0f, 134.0f));
    this->addChild(pNameBox, 1, HERO_NAMEBOX_TAG);
    
    
    // 血量icon
    CCSprite* pHpIcon = CCSprite::create("phicon.png");
    pHpIcon->setAnchorPoint(CCPointZero);
    pHpIcon->setPosition(ccp(LEFT_ITEM_OFFSET + 165.0f, 30.0f));
    this->addChild(pHpIcon, 1, HERO_ICONHP_TAG);
    // 获取血量
    CCLabelTTF* pHp = CCLabelTTF::create("", FONT, 18);
    pHp->setAnchorPoint(CCPointZero);
    pHp->setPosition(ccp(LEFT_ITEM_OFFSET + 195.0f, 30.0f));
    this->addChild(pHp, 1, HERO_HP_TAG);
    
    // 攻击icon
    CCSprite* pAttIcon = CCSprite::create("atticon.png");
    pAttIcon->setAnchorPoint(CCPointZero);
    pAttIcon->setPosition(ccp(LEFT_ITEM_OFFSET + 165.0f, 8.0f));
    this->addChild(pAttIcon, 1, HERO_ICONATT_TAG);
    // 获取攻击力
    CCLabelTTF* pAtt = CCLabelTTF::create("", FONT, 18);
    pAtt->setAnchorPoint(CCPointZero);
    pAtt->setPosition(ccp(LEFT_ITEM_OFFSET +195.0f, 7.0f));
    this->addChild(pAtt, 1, HERO_ATT_TAG);

    //铜币icon
    CCSprite* copperIcon = CCSprite::create("copper.png");
    copperIcon->setAnchorPoint(CCPointZero);
    copperIcon->setPosition(ccp(LEFT_ITEM_OFFSET + 150.0f, 12.0f));
    this->addChild(copperIcon, 1, HERO_SELL_ICON);
    // 获取出售获得铜币
    CCLabelTTF* pCop = CCLabelTTF::create("", FONT, 18);
    pCop->setAnchorPoint(CCPointZero);
    pCop->setPosition(ccp(LEFT_ITEM_OFFSET +185.0f, 18.0f));
    this->addChild(pCop, 1, HERO_SELL_COP);
    
    // expicon
    CCSprite* expIcon = CCSprite::create("label_exp.png");
    expIcon->setAnchorPoint(CCPointZero);
    expIcon->setPosition(ccp(LEFT_ITEM_OFFSET + 160.0f, 20.0f));
    this->addChild(expIcon, 1, HERO_TUN_ICON);
    CCLabelTTF* pExp = CCLabelTTF::create("", FONT, 18);
    pExp->setAnchorPoint(CCPointZero);
    pExp->setPosition(ccp(LEFT_ITEM_OFFSET +205.0f, 21.0f));
    this->addChild(pExp, 1, HERO_TUN_EXP);
    
    // 是不是队长
    CCLabelTTF* pLeader = CCLabelTTF::create("队长", "Arial", 21);
    pLeader->setAnchorPoint(ccp(0, 0));
    pLeader->setColor(ccc3(0,255,0));
    pLeader->setPosition(ccp(255, 50));
    pLeader->setVisible(false);
    this->addChild(pLeader, 1, ISLEADER);
    
    // 武将已经达到当前玩家等级
    CCLabelTTF* pIsMaxLev = CCLabelTTF::create("已满级", "Arial", 18);
//    CCStrokeLabel* pIsMaxLev = CCStrokeLabel::create("已满级", "Arial", 18);
    pIsMaxLev->setAnchorPoint(ccp(0, 0));
    pIsMaxLev->setPosition(ccp(237, 50));
    pIsMaxLev->setColor(ccc3(255, 0, 0));
    pIsMaxLev->setVisible(false);
    this->addChild(pIsMaxLev, 1, LEVUP_ISMAXLEV);
    
    // 升阶武将等级不够
    CCLabelTTF* pIsNotAllown = CCLabelTTF::create("等级不足", "Arial", 18);
//    CCStrokeLabel* pIsNotAllown = CCStrokeLabel::create("等级不足", "Arial", 18);
    pIsNotAllown->setAnchorPoint(ccp(0, 0));
    pIsNotAllown->setPosition(ccp(220, 50));
    pIsNotAllown->setColor(ccc3(255, 0, 0));
    pIsNotAllown->setVisible(false);
    this->addChild(pIsNotAllown, 1, EVOUP_ISNOTALLOWN);
    
    // 升阶武将已经最大等阶
    CCLabelTTF* pIsMaxEvo = CCLabelTTF::create("最大等阶", "Arial", 18);
    //    CCStrokeLabel* pIsNotAllown = CCStrokeLabel::create("等级不足", "Arial", 18);
    pIsMaxEvo->setAnchorPoint(ccp(0, 0));
    pIsMaxEvo->setPosition(ccp(220, 50));
    pIsMaxEvo->setColor(ccc3(255, 0, 0));
    pIsMaxEvo->setVisible(false);
    this->addChild(pIsMaxEvo, 1, EVOUP_ISMAXEVO);
    
    // 姓名
    CCLabelTTF* pName = CCLabelTTF::create("", FONT, 21);
    pName->setAnchorPoint(ccp(0.5, 0));
    pName->setPosition(ccp(LEFT_ITEM_OFFSET + 220.0f, 135.0f));
    this->addChild(pName, 1, HERO_NAME_TAG);
    
    // 英雄阵营
    CCSprite* pFlag = CCSprite::create("f4.png");
    pFlag->setAnchorPoint(CCPointZero);
    pFlag->setPosition(ccp(LEFT_ITEM_OFFSET, 138.0f));
    this->addChild(pFlag, 10, HERO_FLAG_TAG);
    
    // 英雄星形
    for (int i = 1; i <= 5; ++i) {
        CCSprite* pStar = CCSprite::create("wh_star.png");
        pStar->setAnchorPoint(CCPointZero);
        pStar->setPosition(ccp(LEFT_ITEM_OFFSET + 155.0f + (i - 1) * 25.0f, 105.0f));
        this->addChild(pStar, 2, HERO_STAR_TAG + 1000 + i);
    }
    
    // 英雄防具
//    CCSprite* pFang = CCSprite::create("cjb.png");
//    pFang->setAnchorPoint(CCPointZero);
//    pFang->setPosition(ccp(LEFT_ITEM_OFFSET + 23.0f, 42.0f));
//    cell->addChild(pFang, 1, HERO_EQUI_TAG);
    
    // 英雄武器
//    CCSprite* pWu = CCSprite::create("cjb.png");
//    pWu->setAnchorPoint(CCPointZero);
//    pWu->setPosition(ccp(LEFT_ITEM_OFFSET+23, 42));
//    cell->addChild(pWu, 1, HERO_WEPON_TAG);
    
    CCSprite* pLock = CCSprite::create("lock.png");
    pLock->setAnchorPoint(CCPointZero);
    pLock->setPosition(ccp(LEFT_ITEM_OFFSET + 245.0f, 0.0f));
    pLock->setVisible(false);
    this->addChild(pLock, 1, HERO_LOCK_TAG);
    
    // 勾选确认
    CCSprite* pConfirmBg = CCSprite::create("HL_conBg.png");
    pConfirmBg->setAnchorPoint(CCPointZero);
    pConfirmBg->setPosition(ccp(LEFT_ITEM_OFFSET + 242.0f, 0.0f));
    this->addChild(pConfirmBg, 1, HERO_CONFIRMBG_TAG);
    pConfirmBg->setVisible(false);
    CCSprite* pConfirm = CCSprite::create("HL_confirm.png");
    pConfirm->setAnchorPoint(CCPointZero);
    pConfirm->setPosition(ccp(LEFT_ITEM_OFFSET + 255.0f, 2.0f));
    this->addChild(pConfirm, 3, HERO_CONFIRM_TAG);
    pConfirm->setVisible(false);
}

/*
 *  功能：
 *      更新 UI 组件
 *  参数：
 *      pHeroOfPlayerItem   ->  玩家所属武将数据项
 */
void HeroItemNode::updateUiComponent(HeroOfPlayerItem* pHeroOfPlayerItem, HerosLayer* pHerosLayer) {
    do {
        CC_BREAK_IF(!pHeroOfPlayerItem || !pHerosLayer);
        
        char buffer[64];
        
        CCLabelTTF* pHp = (CCLabelTTF*)this->getChildByTag(HERO_HP_TAG);
        sprintf(buffer, "%d", pHeroOfPlayerItem->getHP());
        pHp->setString(buffer);
        
        CCLabelTTF* pAtt = (CCLabelTTF*)this->getChildByTag(HERO_ATT_TAG);
        sprintf(buffer, "%d", pHeroOfPlayerItem->getAttack());
        pAtt->setString(buffer);
        
        CCLabelTTF* pCop = (CCLabelTTF*)this->getChildByTag(HERO_SELL_COP);
        sprintf(buffer, "%d", HeroManager::getInstance()->getAdditionHeroData(pHeroOfPlayerItem->getHeroItem()->getRarity(), pHeroOfPlayerItem->getLevel())->getGoldBySale());
        pCop->setString(buffer);
        
        CCLabelTTF* pExp = (CCLabelTTF*)this->getChildByTag(HERO_TUN_EXP);
        sprintf(buffer, "%d", HeroManager::getInstance()->getAdditionHeroData(pHeroOfPlayerItem->getHeroItem()->getRarity(), pHeroOfPlayerItem->getLevel())->getExpByFood());
        pExp->setString(buffer);
        
        CCSprite* spIcon1 = (CCSprite*)this->getChildByTag(HERO_ICONHP_TAG);
        CCSprite* spIcon2 = (CCSprite*)this->getChildByTag(HERO_ICONATT_TAG);
        CCSprite* spIcon3 = (CCSprite*)this->getChildByTag(HERO_SELL_ICON);
        CCSprite* spIcon4 = (CCSprite*)this->getChildByTag(HERO_TUN_ICON);
        
        CCLabelTTF* pLeader = (CCLabelTTF*)this->getChildByTag(ISLEADER);
        CCLabelTTF* pIsMaxLev = (CCLabelTTF*)this->getChildByTag(LEVUP_ISMAXLEV);
//        CCStrokeLabel* pIsMaxLev = (CCStrokeLabel*)this->getChildByTag(LEVUP_ISMAXLEV);
        CCLabelTTF* pIsNotAllown = (CCLabelTTF*)this->getChildByTag(EVOUP_ISNOTALLOWN);
//        CCStrokeLabel* pIsNotAllown = (CCStrokeLabel*)this->getChildByTag(EVOUP_ISNOTALLOWN);
        CCLabelTTF* pIsMaxEvo = (CCLabelTTF*)this->getChildByTag(EVOUP_ISMAXEVO);
        
        WhoCallHeroList whoCallHeroList = m_pGameState->getCurrTagWhoCallHeroSel();
        if (WCH_BROWSE == whoCallHeroList) /*浏览列表*/ {
            spIcon1->setVisible(true);
            pHp->setVisible(true);
            spIcon2->setVisible(true);
            pAtt->setVisible(true);
            spIcon3->setVisible(false);
            pCop->setVisible(false);
            spIcon4->setVisible(false);
            pExp->setVisible(false);
            
            pLeader->setVisible(false);
            pIsMaxLev->setVisible(false);
            pIsNotAllown->setVisible(false);
            pIsMaxEvo->setVisible(false);
        } else if (WCH_SALE == whoCallHeroList) /*出售武将列表*/ {
            spIcon1->setVisible(false);
            pHp->setVisible(false);
            spIcon2->setVisible(false);
            pAtt->setVisible(false);
            spIcon3->setVisible(true);
            pCop->setVisible(true);
            spIcon4->setVisible(false);
            pExp->setVisible(false);
            
            pLeader->setVisible(false);
            pIsMaxLev->setVisible(false);
            pIsNotAllown->setVisible(false);
            pIsMaxEvo->setVisible(false);
        } else if (WCH_TEAMER == whoCallHeroList || WCH_CAPTAIN == whoCallHeroList)
            /*上阵列表*/ {
            spIcon1->setVisible(true);
            pHp->setVisible(true);
            spIcon2->setVisible(true);
            pAtt->setVisible(true);
            spIcon3->setVisible(false);
            pCop->setVisible(false);
            spIcon4->setVisible(false);
            pExp->setVisible(false);
            
            if (HeroManager::getInstance()->isLeaderHero(pHeroOfPlayerItem->getUniId()))
                pLeader->setVisible(true);
            else
                pLeader->setVisible(false);
            
            pIsMaxLev->setVisible(false);
            pIsNotAllown->setVisible(false);
            pIsMaxEvo->setVisible(false);
        } else if (WCH_TEAMER_PVP == whoCallHeroList || WCH_CAPTAIN_PVP == whoCallHeroList) {
            /*上阵列表*/
                spIcon1->setVisible(true);
                pHp->setVisible(true);
                spIcon2->setVisible(true);
                pAtt->setVisible(true);
                spIcon3->setVisible(false);
                pCop->setVisible(false);
                spIcon4->setVisible(false);
                pExp->setVisible(false);
                
                if (HeroManager::getInstance()->isPvpLeaderHero(pHeroOfPlayerItem->getUniId()))
                    pLeader->setVisible(true);
                else
                    pLeader->setVisible(false);
                
                pIsMaxLev->setVisible(false);
                pIsNotAllown->setVisible(false);
                pIsMaxEvo->setVisible(false);
        } else if (WCH_LEVUP == whoCallHeroList) /*升级列表*/ {
            spIcon1->setVisible(true);
            pHp->setVisible(true);
            spIcon2->setVisible(true);
            pAtt->setVisible(true);
            spIcon3->setVisible(false);
            pCop->setVisible(false);
            spIcon4->setVisible(false);
            pExp->setVisible(false);
            
            pLeader->setVisible(false);
            if (pHeroOfPlayerItem->getLevel() >= GamePlayer::getInstance()->getLevel()) {
                pIsMaxLev->setVisible(true);
            } else {
                pIsMaxLev->setVisible(false);
            }
            pIsNotAllown->setVisible(false);
            pIsMaxEvo->setVisible(false);
        } else if (WCH_FORFOOD == whoCallHeroList) /*升级被吞列表*/ {
            spIcon1->setVisible(false);
            pHp->setVisible(false);
            spIcon2->setVisible(false);
            pAtt->setVisible(false);
            spIcon3->setVisible(false);
            pCop->setVisible(false);
            spIcon4->setVisible(true);
            pExp->setVisible(true);
            
            pLeader->setVisible(false);
            pIsMaxLev->setVisible(false);
            pIsNotAllown->setVisible(false);
            pIsMaxEvo->setVisible(false);
        } else if (WCH_EVOUP == whoCallHeroList) /*升阶列表*/ {
            spIcon1->setVisible(true);
            pHp->setVisible(true);
            spIcon2->setVisible(true);
            pAtt->setVisible(true);
            spIcon3->setVisible(false);
            pCop->setVisible(false);
            spIcon4->setVisible(false);
            pExp->setVisible(false);
            
            pLeader->setVisible(false);
            pIsMaxLev->setVisible(false);
            if (pHeroOfPlayerItem->getHeroItem()->getUl() > pHeroOfPlayerItem->getLevel()) {
                pIsNotAllown->setVisible(true);
            } else {
                pIsNotAllown->setVisible(false);
            }
            if (!pHeroOfPlayerItem->getHeroItem()->getUt())
                pIsMaxEvo->setVisible(true);
            else
                pIsMaxEvo->setVisible(false);
        }
        
        CCLabelTTF* pName = (CCLabelTTF*)this->getChildByTag(HERO_NAME_TAG);
        pName->setString((pHeroOfPlayerItem->getHeroItem()->getName()).c_str());
        
        // cardBox 的更新
        CardBox* card = (CardBox*)this->getChildByTag(CARD);
        card->setCard(2, pHeroOfPlayerItem, true);
        
        //************阵营
        sprintf(buffer, "f%d.png", pHeroOfPlayerItem->getHeroItem()->getCamp());
        CCSprite *apFlag = (CCSprite *)this->getChildByTag(HERO_FLAG_TAG);
        apFlag->setTexture(CCTextureCache::sharedTextureCache()->addImage(buffer));
        
        //*************星形
        CCTexture2D *aStar = NULL;
        int lev = pHeroOfPlayerItem->getLevel();
        int levi = (int)((lev - 1) / 30) + 1;
        int rare = pHeroOfPlayerItem->getHeroItem()->getRarity();
        if (1 == rare) {
            strcpy(buffer, "wh_star.png");
        } else if(2 == rare || 3 == rare) {
            strcpy(buffer, "gr_star.png");
        } else if(4 == rare || 5 == rare) {
            strcpy(buffer, "bl_star.png");
        } else if(6 ==rare || 7 == rare || 8 == rare) {
            strcpy(buffer, "pu_star.png");
        } else {
            strcpy(buffer, "or_star.png");
        }
        for (int i = 1; i <= levi; i++) {
            CCSprite* sp = (CCSprite*)this->getChildByTag(HERO_STAR_TAG + 1000 + i);
            aStar = CCTextureCache::sharedTextureCache()->addImage(buffer);
            sp->setTexture(aStar);
            sp->setVisible(true);
        }
        for (int i = levi + 1; i <= 5; i++) {
            CCSprite* sp = (CCSprite*)this->getChildByTag(HERO_STAR_TAG + 1000 + i);
            sp->setVisible(false);
        }
        
        //**************防具
        //CCTexture2D *aFang=CCTextureCache::sharedTextureCache()->addImage("cjb.png");
        //CCSprite *apFang=(CCSprite *)cell->getChildByTag(HERO_EQUI_TAG);
        //apFang->setTexture(aFang);
        
        //**************武器
        //CCTexture2D *aWu=CCTextureCache::sharedTextureCache()->addImage("cjb.png");
        //CCSprite *apWu=(CCSprite *)cell->getChildByTag(HERO_WEPON_TAG);
        //apWu->setTexture(aWu);
        
        //**************是否上锁
        CCSprite *apLock = (CCSprite *)this->getChildByTag(HERO_LOCK_TAG);
        apLock->setVisible(pHeroOfPlayerItem->getLock());

        /* 武将列表不同模式下的显示内容 */
        CCSprite* pConfirmBg = (CCSprite*)this->getChildByTag(HERO_CONFIRMBG_TAG);
        CCSprite* pConfirm = (CCSprite*)this->getChildByTag(HERO_CONFIRM_TAG);
        CCSprite* pBg = (CCSprite*)this->getChildByTag(HERO_BOXBG_TAG);
        if (pHerosLayer->m_isSelectHeroMode) /*选择武将模式*/ {
            pConfirmBg->setVisible(true);
            if (pHerosLayer->m_countOfSelectedHeros > 0) /*已经有选中的武将*/{
                bool isSelectedAll = (pHerosLayer->_isAlreadyMaxAllowSelectedHero());    // 是否已经到了可选武将的上限
                
                bool isAllow = true;        // 是否允许被选
                for (int i = 0; i < pHerosLayer->m_countOfNotAllowed; i++) {
                    if (!strcmp(pHeroOfPlayerItem->getUniId().c_str(), pHerosLayer->m_szNotAllowedHeroUniId[i])) /*该武将不能选*/ {
                        isAllow = false;
                        break;
                    }
                } /*for*/
                    
                bool isSelected = false;    // 是否被选中
                for (int i = 0; i < pHerosLayer->m_countOfSelectedHeros; i++) {
                    if (!strcmp(pHeroOfPlayerItem->getUniId().c_str(), pHerosLayer->m_szSelectedHeroUniId[i])) /*该武将被选中*/ {
                        isSelected = true;
                        break;
                    }
                } /*for*/
                    
                if (isAllow) {
                    if (isSelectedAll && !isSelected) {
//                        pConfirmBg->setTexture(CCTextureCache::sharedTextureCache()->addImage("HL_confirmBg_d.png"));
                        pConfirmBg->setVisible(false);
                        pBg->setTexture(CCTextureCache::sharedTextureCache()->addImage("HL_BGH.png"));
                    } else {
                        pConfirmBg->setVisible(true);
//                        pConfirmBg->setTexture(CCTextureCache::sharedTextureCache()->addImage("HL_conBg.png"));
                        pBg->setTexture(CCTextureCache::sharedTextureCache()->addImage("HL_Bg.png"));
                    }
                    pConfirm->setVisible(isSelected);
                } else {
//                    pConfirmBg->setTexture(CCTextureCache::sharedTextureCache()->addImage("HL_confirmBg_d.png"));
                    pConfirmBg->setVisible(false);
                    pBg->setTexture(CCTextureCache::sharedTextureCache()->addImage("HL_BGH.png"));
                    pConfirm->setVisible(false);
                }
            } else /*没有选中任何武将*/ {
                pConfirm->setVisible(false);
                
                bool isAllow = true;        // 是否允许被选
                for (int i = 0; i < pHerosLayer->m_countOfNotAllowed; i++) {
                    if (!strcmp(pHeroOfPlayerItem->getUniId().c_str(), pHerosLayer->m_szNotAllowedHeroUniId[i])) /*该武将不能选*/ {
                        isAllow = false;
                        break;
                    }
                } /*for*/
                if (isAllow) {
                    pConfirmBg->setTexture(CCTextureCache::sharedTextureCache()->addImage("HL_conBg.png"));
                    pBg->setTexture(CCTextureCache::sharedTextureCache()->addImage("HL_Bg.png"));
                } else {
                    //pConfirmBg->setTexture(CCTextureCache::sharedTextureCache()->addImage("HL_confirmBg_d.png"));
                    pConfirmBg->setVisible(false);
                    pBg->setTexture(CCTextureCache::sharedTextureCache()->addImage("HL_BGH.png"));
                }
            }
            
        } else /*浏览武将模式*/ {
            pConfirmBg->setVisible(false);
            pConfirm->setVisible(false);
            pBg->setTexture(CCTextureCache::sharedTextureCache()->addImage("HL_Bg.png"));
            pBg->setVisible(true);
        }
        
        
        //
        // CCLog("--> %d 调用了列表！", GameState::getInstance()->getTagWhoCallHeroSel());
        
        
    } while (0);
}

