
#include "CardBox.h"
#include "CCStrokeLabel.h"
#include "HeroManager.h"
#include "HeroOfPlayerItem.h"
#include "HeroItem.h"
#include "HeroAdditionItem.h"
#include "MosterItem.h"
#include "GamePlayer.h"

#define MODE_HEAD                       1               // 头像卡
#define MODE_LIST                       2               // 列表卡
#define MODE_COMPLETE                   3               // 整卡
#define MODE_BATTLE                     4               // 战斗卡

#define NODE_TAG                        100             // node容器
#define COMPELETPIC_HUI                 105             // 头像灰态

#define NODE_OPERATOR_TAG               200

#define HERO_ACTION_TAG                 150

#define PRISON_CARD_TAG                 200

#define TONGQUE_NODE_TAG                300
#define TONGQUE_NODE_PVP_TAG            301

#define TONGQUE_ACTION_TAG              350

/*
 *  构造函数
 */
CardBox::CardBox()
:m_oldAtt(0)
,m_curAtt(0)
,m_oldHp(0)
,m_curHp(0)
,m_expPer(0.0f)
,m_heroOfPlayerItem(NULL)
,m_curLev(0)
,m_oldLev(0)
,m_isAction(false)
{
}

/*
 *  析构函数
 */
CardBox::~CardBox() {
}

/*
 *  初始化
 */
bool CardBox::init() {
    // super init
    if (!CCLayer::init()) {
        return false;
    }

    /*初始化显示头像卡，id1001，等级1，稀有度1*/
    // setCard(1, "h_sunj3.png", 1, 1);
    
    
    return true;
}

/*
 * 根据参数更新ui显示
 * 参数：
 * mode         卡的类型
 * pItem        玩家拥有的武将卡
 * isShowLev    是否显示卡的等级,如果heroLev==0则表示不需要显示等级,默认为0
 * isShowInfo   是否显示详细信息
 * isShowEvo    是否显示进化的信息
 */
void CardBox::setCard(const unsigned int mode, HeroOfPlayerItem* pItem,
                      const bool isShowLev, const bool isShowInfo, const int upLevel,
                      HeroItem* pLocalItem, const bool isEvo) {
    char buffer[16];
    if (this->getChildByTag(NODE_TAG)) {
        this->removeChildByTag(NODE_TAG);
    }
    CCNode* pNode = CCNode::create();
    this->addChild(pNode, 1, NODE_TAG);
    int heroRarity = 0;
    int heroLev = pItem->getLevel();
    
    if (pItem) {
        if (upLevel < 0)/*升阶*/ {
            heroRarity = pLocalItem->getRarity();
        } else {
             heroRarity = pItem->getHeroItem()->getRarity();
        }
        
        if (MODE_HEAD == mode) {
            std::string heroPic = pItem->getHeroItem()->getHIcon();
            
            
            /* 武将框 */
            if (1 == heroRarity) {
                strcpy(buffer, "HP_whiBox.png");
            } else if (2 ==heroRarity || 3 ==heroRarity) {
                strcpy(buffer, "HP_greBox.png");
            } else if (4 == heroRarity || 5 == heroRarity) {
                strcpy(buffer, "HP_bluBox.png");
            } else if (6 ==heroRarity || 7 == heroRarity || 8 == heroRarity) {
                strcpy(buffer, "HP_purBox.png");
            } else {
                strcpy(buffer, "HP_oraBox.png");
            }
            CCSprite* box = CCSprite::create(buffer);
            box->setAnchorPoint(ccp(0, 0));
            box->setPosition(ccp(0, 0));
            pNode->addChild(box, 10);
            /* 角标显示规律 */
            if (1 == heroRarity) {
                strcpy(buffer, "HP_whiJiao.png");
            } else if (2 ==heroRarity || 3 ==heroRarity) {
                strcpy(buffer, "HP_greJiao.png");
            } else if (4 == heroRarity || 5 == heroRarity) {
                strcpy(buffer, "HP_bluJiao.png");
            } else if (6 ==heroRarity || 7 == heroRarity || 8 == heroRarity) {
                strcpy(buffer, "HP_purJiao.png");
            } else {
                strcpy(buffer, "HP_oraJiao.png");
            }
            
            CCSprite* jiao1 = CCSprite::create(buffer);
            jiao1->setAnchorPoint(ccp(0, 0));
            jiao1->setPosition(ccp(4, 5));
            pNode->addChild(jiao1, 9);
            CCSprite* jiao2 = CCSprite::create(buffer);
            jiao2->setAnchorPoint(ccp(0, 0));
            jiao2->setPosition(ccp(66, 5));
            jiao2->setFlipX(true);
            pNode->addChild(jiao2, 9);
            CCSprite* jiao3 = CCSprite::create(buffer);
            jiao3->setAnchorPoint(ccp(0, 0));
            jiao3->setPosition(ccp(66, 64));
            jiao3->setFlipX(true);
            jiao3->setFlipY(true);
            pNode->addChild(jiao3, 9);
            CCSprite* jiao4 = CCSprite::create(buffer);
            jiao4->setAnchorPoint(ccp(0, 0));
            jiao4->setPosition(ccp(4, 64));
            jiao4->setFlipY(true);
            pNode->addChild(jiao4, 9);
            /*稀有度*/
            if ((heroRarity > 0 && heroRarity <= 2) || heroRarity == 4 || heroRarity == 6 || heroRarity == 9) {
                jiao1->setVisible(false);
                jiao2->setVisible(false);
                jiao3->setVisible(false);
                jiao4->setVisible(false);
            } else if (heroRarity == 3 || heroRarity == 5 || heroRarity == 8) {
                jiao1->setVisible(true);
                jiao2->setVisible(true);
                jiao3->setVisible(true);
                jiao4->setVisible(true);
            } else if (heroRarity == 7) {
                jiao1->setVisible(false);
                jiao2->setVisible(true);
                jiao3->setVisible(false);
                jiao4->setVisible(true);
            }
            if (1 == heroRarity) {
                strcpy(buffer, "HP_whi.png");
            } else if (2 ==heroRarity || 3 ==heroRarity) {
                strcpy(buffer, "HP_gre.png");
            } else if (4 == heroRarity || 5 == heroRarity) {
                strcpy(buffer, "HP_blu.png");
            } else if (6 ==heroRarity || 7 == heroRarity || 8 == heroRarity) {
                strcpy(buffer, "HP_pur.png");
            } else {
                strcpy(buffer, "HP_ora.png");
            }
            CCSprite* rabg = CCSprite::create(buffer);
            rabg->setAnchorPoint(ccp(0, 0));
            rabg->setPosition(ccp(4, 3));
            pNode->addChild(rabg, 6);
            // 武将头像
            CCSprite* pic = CCSprite::create(heroPic.c_str());
            pic->setAnchorPoint(ccp(0, 0));
            pic->setPosition(ccp(5, 4));
            pNode->addChild(pic, 7);
            sprintf(buffer, "%d", heroLev);
            if (isShowLev) {
                CCSprite* levLabel = CCSprite::create("CM_lev.png");
                levLabel->setAnchorPoint(ccp(0, 0));
                levLabel->setPosition(ccp(15, 0));
                pNode->addChild(levLabel, 10);
                CCStrokeLabel* levFont = CCStrokeLabel::create(buffer, "Arial", 17);
                levFont->setAnchorPoint(ccp(0, 0));
                levFont->setPosition(ccp(45, 0));
                pNode->addChild(levFont, 11);
                
            }
        } else if (MODE_COMPLETE == mode)/* 整卡模式 */ {
            if (m_isAction) {
                unschedule(schedule_selector(CardBox::_updateData));
            }

            std::string heroPic;
            if (upLevel < 0)/*升阶*/ {
                heroPic = pLocalItem->getBIcon();
            } else {
                heroPic = pItem->getHeroItem()->getBIcon();
            }
            
            /* 武将框 */
            if (1 == heroRarity) {
                strcpy(buffer, "COM_whiBox.png");
            } else if (2 ==heroRarity || 3 ==heroRarity) {
                strcpy(buffer, "COM_greBox.png");
            } else if (4 == heroRarity || 5 == heroRarity) {
                strcpy(buffer, "COM_bluBox.png");
            } else if (6 ==heroRarity || 7 == heroRarity || 8 == heroRarity) {
                strcpy(buffer, "COM_purBox.png");
            } else {
                strcpy(buffer, "COM_oraBox.png");
            }
            CCSprite* box = CCSprite::create(buffer);
            box->setAnchorPoint(ccp(0, 0));
            box->setPosition(ccp(0, 0));
            pNode->addChild(box, 2);
            /* 角标显示规律 */
            if (1 == heroRarity) {
                strcpy(buffer, "COM_whiJiao.png");
            } else if (2 ==heroRarity || 3 ==heroRarity) {
                strcpy(buffer, "COM_greJiao.png");
            } else if (4 == heroRarity || 5 == heroRarity) {
                strcpy(buffer, "COM_bluJiao.png");
            } else if (6 ==heroRarity || 7 == heroRarity || 8 == heroRarity) {
                strcpy(buffer, "COM_purJiao.png");
            } else {
                strcpy(buffer, "COM_oraJiao.png");
            }
            CCSprite* jiao1 = CCSprite::create(buffer);
            jiao1->setAnchorPoint(ccp(0, 0));
            jiao1->setPosition(ccp(13, 12));
            pNode->addChild(jiao1, 2);
            CCSprite* jiao2 = CCSprite::create(buffer);
            jiao2->setAnchorPoint(ccp(0, 0));
            jiao2->setPosition(ccp(143, 12));
            jiao2->setFlipX(true);
            pNode->addChild(jiao2, 2);
            CCSprite* jiao4 = CCSprite::create(buffer);
            jiao4->setAnchorPoint(ccp(0, 0));
            jiao4->setPosition(ccp(16, 187));
            jiao4->setFlipY(true);
            pNode->addChild(jiao4, 2);
            
            if (1 == heroRarity) {
                strcpy(buffer, "COM_whiJob.png");
            } else if (2 ==heroRarity || 3 ==heroRarity) {
                strcpy(buffer, "COM_greJob.png");
            } else if (4 == heroRarity || 5 == heroRarity) {
                strcpy(buffer, "COM_bluJob.png");
            } else if (6 ==heroRarity || 7 == heroRarity || 8 == heroRarity) {
                strcpy(buffer, "COM_purJob.png");
            } else {
                strcpy(buffer, "COM_oraJob.png");
            }
            CCSprite* jiao3 = CCSprite::create(buffer);/*职业框*/
            jiao3->setAnchorPoint(ccp(0, 0));
            jiao3->setPosition(ccp(159, 205));
            pNode->addChild(jiao3, 10);
            sprintf(buffer, "sb%d.png", pItem->getHeroItem()->getJob());/*职业*/
            CCSprite* job = CCSprite::create(buffer);
            job->setAnchorPoint(ccp(0, 0));
            job->setPosition(ccp(170, 211));
            pNode->addChild(job, 10);
            
            if ((heroRarity > 0 && heroRarity <= 2) || heroRarity == 4 || heroRarity == 6 || heroRarity == 9) {
                jiao1->setVisible(false);
                jiao2->setVisible(false);
                jiao4->setVisible(false);
            } else if (heroRarity == 3 || heroRarity == 5 || heroRarity == 8) {
                jiao1->setVisible(true);
                jiao2->setVisible(true);
                jiao4->setVisible(true);
            } else if (heroRarity == 7) {
                jiao1->setVisible(false);
                jiao2->setVisible(true);
                jiao4->setVisible(true);
            }
            if (1 == heroRarity) {
                strcpy(buffer, "CM_whi.png");
            } else if (2 ==heroRarity || 3 ==heroRarity) {
                strcpy(buffer, "CM_gre.png");
            } else if (4 == heroRarity || 5 == heroRarity) {
                strcpy(buffer, "CM_blu.png");
            } else if (6 ==heroRarity || 7 == heroRarity || 8 == heroRarity) {
                strcpy(buffer, "CM_pur.png");
            } else {
                strcpy(buffer, "CM_ora.png");
            }
            CCSprite* rabg = CCSprite::create(buffer);
            rabg->setAnchorPoint(ccp(0, 0));
            rabg->setPosition(ccp(16, 10));
            pNode->addChild(rabg, 1);
            /* 武将头像 */
            CCSprite* pic = CCSprite::create(heroPic.c_str());
            pic->setAnchorPoint(ccp(0, 0));
            pic->setPosition(ccp(-62, 12));
            pNode->addChild(pic, 5, COMPELETPIC_HUI);
            
            /* 经验进度条 */
            CCSprite* pExpBox = CCSprite::create("LU_proLevBg.png");
            pExpBox->setAnchorPoint(ccp(0, 0));
            pExpBox->setPosition(ccp(70.0f, -12.0f));
            pNode->addChild(pExpBox, 20);
            
            m_expProgress = CCProgressTimer::create(CCSprite::create("LU_proLev.png"));
            m_expProgress->setType(kCCProgressTimerTypeBar);
            m_expProgress->setMidpoint(ccp(0,0));
            m_expProgress->setBarChangeRate(ccp(1, 0));
            m_expProgress->setAnchorPoint(ccp(0, 0));
            m_expProgress->setPosition(ccp(70.0f, -12.0f));
            pNode->addChild(m_expProgress, 20);
            /* 当前武将经验条数值百分比 */
            float expPer = 0.0f;
            HeroAdditionItem* pHeroAdditionItem = HeroManager::getInstance()->getAdditionHeroData(pItem->getHeroItem()->getRarity(), pItem->getLevel());
//            CCLog("exp = %d, curExp = %d", pHeroAdditionItem->getExpForLevUp(), pItem->getExp());
            expPer = ((float)pItem->getExp() / (float)pHeroAdditionItem->getExpForLevUp()) * 100.0;
            m_expProgress->setPercentage(expPer);
            
            m_heroOfPlayerItem = pItem;
            
            /* 其他信息显示 */
            if (isShowInfo) {
                CCSprite* infoBg = CCSprite::create("LU_infoHero.png");
                infoBg->setAnchorPoint(ccp(0, 0));
                infoBg->setPosition(ccp(5, -60));
                pNode->addChild(infoBg, 10);
                
                sprintf(buffer, "%d", pItem->getLevel());
                m_levLabel = CCLabelTTF::create(buffer, "Arial", 19);
                m_levLabel->setAnchorPoint(ccp(0, 0));
                m_levLabel->setPosition(ccp(40, -15));
                pNode->addChild(m_levLabel, 10);
                
                CCLabelTTF* l2 = CCLabelTTF::create("", "Arial", 19);
                l2->setAnchorPoint(ccp(0, 0));
                l2->setPosition(ccp(40, -15));
                pNode->addChild(l2, 10);
                
                int hpN = pItem->getHP();
                sprintf(buffer, "%d", hpN);
                m_hpLabel = CCLabelTTF::create(buffer, "Arial", 19);
                m_hpLabel->setAnchorPoint(ccp(0, 0));
                m_hpLabel->setPosition(ccp(65, -33));
                pNode->addChild(m_hpLabel, 10);
                
                int attN = pItem->getAttack();
                sprintf(buffer, "%d", attN);
                m_attLabel = CCLabelTTF::create(buffer, "Arial", 19);
                m_attLabel->setAnchorPoint(ccp(0, 0));
                m_attLabel->setPosition(ccp(65, -53));
                pNode->addChild(m_attLabel, 10);

                if (upLevel == 0)/*可能是升级*/ {
                    m_isEvo = false;
                    m_oldLev = (float)pItem->getLevel();
                } else if (upLevel >= 0)/*升级*/{
                    m_curLev = (float)(pItem->getLevel() + upLevel);
                    if (m_curLev >= GamePlayer::getInstance()->getLevel())/*控制武将等级小于等于玩家等级*/ {
                        m_curLev = GamePlayer::getInstance()->getLevel();
                    }
                    
                    m_oldHp = hpN;/*保存老的hp*/
                    /* 新的血量 */
                    int hpNe = 0;
                    hpNe = pItem->getHeroItem()->getBasicHp() + (m_curLev - 1)*pItem->getHeroItem()->getGrowthHp();
                    m_curHp = hpNe;/*保存当前hp*/
                    
                    m_isEvo = false;
                    
                    sprintf(buffer, "%d", hpNe);
                    CCLabelTTF* hpNext = CCLabelTTF::create(buffer, "Arial", 19);
                    hpNext->setAnchorPoint(ccp(0, 0));
                    hpNext->setPosition(ccp(145, -33));
                    hpNext->setColor(ccc3(0, 255, 0));
                    pNode->addChild(hpNext,20);
                    m_oldAtt = attN;
                    /* 新的攻击力 */
                    int attNe = 0;
                    attNe = pItem->getHeroItem()->getBasicProperty() + (m_curLev - 1)*pItem->getHeroItem()->getGrowthProperty();
                    m_curAtt = attNe;
                    sprintf(buffer, "%d", attNe);
                    CCLabelTTF* attNext = CCLabelTTF::create(buffer, "Arial", 19);
                    attNext->setAnchorPoint(ccp(0, 0));
                    attNext->setPosition(ccp(145, -53));
                    attNext->setColor(ccc3(0, 255, 0));
                    pNode->addChild(attNext,20);
                    
                    /* 旧的等级 */
                    sprintf(buffer, "%d", pItem->getLevel());
                    m_oldLev = (float)pItem->getLevel();
                    l2->setString(buffer);
                    l2->setOpacity(0);
                    CCFiniteTimeAction * seq1 = CCSequence::create(CCFadeIn::create(1),CCFadeOut::create(1),CCDelayTime::create(2),
                                                                   NULL);
                    CCActionInterval * repeatForever1 =CCRepeatForever::create((CCActionInterval* )seq1);
                    l2->runAction(repeatForever1);
                    /* 新的等级 */
                    sprintf(buffer, "%d", m_curLev);
                    m_levLabel->setString(buffer);
                    m_levLabel->setOpacity(0);
                    CCFiniteTimeAction * seq = CCSequence::create(CCDelayTime::create(2),CCFadeIn::create(1),CCFadeOut::create(1),
                                                                  NULL);
                    CCActionInterval * repeatForever =CCRepeatForever::create((CCActionInterval* )seq);
                    m_levLabel->runAction(repeatForever);
                    
                    CCSprite* spJian = CCSprite::create("LU_jiantou.png");
                    spJian->setAnchorPoint(ccp(0, 0));
                    spJian->setPosition(ccp(115, -30));
                    pNode->addChild(spJian,20);
                    
                    CCSprite* spJian1 = CCSprite::create("LU_jiantou.png");
                    spJian1->setAnchorPoint(ccp(0, 0));
                    spJian1->setPosition(ccp(115, -50));
                    pNode->addChild(spJian1,20);
                } else if (upLevel < 0)/*进化*/ {
                    if (isEvo) {
                        m_isEvo = isEvo;
                        
                        int qjLev = pItem->getLevel() + upLevel;
                        
                        m_oldLev = pItem->getLevel();
                        m_curLev = qjLev;
                        
                        CCLog("pitem = %d, uplevep = %d", pItem->getLevel(), upLevel);
                        if (qjLev <= 0) {
                            qjLev = 1;
                        }
                        m_oldHp = hpN;
                        /* 新的血量 */
                        int hpNe = 0;
                        hpNe = pLocalItem->getBasicHp() + (qjLev - 1)*pLocalItem->getGrowthHp();
                        m_curHp = hpNe;
                        sprintf(buffer, "%d", hpNe);
                        CCLabelTTF* hpNext = CCLabelTTF::create(buffer, "Arial", 19);
                        hpNext->setAnchorPoint(ccp(0, 0));
                        hpNext->setPosition(ccp(145, -33));
                        hpNext->setColor(ccc3(0, 255, 0));
                        pNode->addChild(hpNext,20);
                        
                        m_oldAtt = attN;
                        /* 新的攻击力 */
                        int attNe = 0;
                        attNe = pLocalItem->getBasicProperty() + (qjLev - 1)*pLocalItem->getGrowthProperty();
                        m_curAtt = attNe;
                        sprintf(buffer, "%d", attNe);
                        CCLabelTTF* attNext = CCLabelTTF::create(buffer, "Arial", 19);
                        attNext->setAnchorPoint(ccp(0, 0));
                        attNext->setPosition(ccp(145, -53));
                        attNext->setColor(ccc3(0, 255, 0));
                        pNode->addChild(attNext,20);
                        
                        /* 新的等级 */
                        sprintf(buffer, "%d", qjLev);
                        m_levLabel->setString(buffer);
                        CCActionInterval * fadein = CCFadeIn::create(0.5);
                        CCActionInterval * fadeout = CCFadeOut::create(0.5);
                        CCFiniteTimeAction * seq = CCSequence::create(fadein,fadeout,NULL);
                        CCActionInterval * repeatForever =CCRepeatForever::create((CCActionInterval* )seq);
                        m_levLabel->runAction(repeatForever);
                        
                        /* 箭头 */
                        CCSprite* spJian = CCSprite::create("LU_jiantou.png");
                        spJian->setAnchorPoint(ccp(0, 0));
                        spJian->setPosition(ccp(115, -30));
                        pNode->addChild(spJian,20);
                        
                        CCSprite* spJian1 = CCSprite::create("LU_jiantou.png");
                        spJian1->setAnchorPoint(ccp(0, 0));
                        spJian1->setPosition(ccp(115, -50));
                        pNode->addChild(spJian1,20);
                        
                        /* 进化前头像需要灰态 */
                        CCSprite* spPic = NULL;
                        if (pNode->getChildByTag(COMPELETPIC_HUI)) {
                            spPic = (CCSprite*)pNode->getChildByTag(COMPELETPIC_HUI);
                            spPic->setColor(ccc3(128, 128, 105));
                        }
                    } else /*不可进化*/{
                        /* 新的血量 */
                        CCLabelTTF* hpNext = CCLabelTTF::create("???", "Arial", 19);
                        hpNext->setAnchorPoint(ccp(0, 0));
                        hpNext->setPosition(ccp(145, -33));
                        hpNext->setColor(ccc3(0, 255, 0));
                        pNode->addChild(hpNext,20);
                        /* 新的攻击力 */
                        CCLabelTTF* attNext = CCLabelTTF::create("???", "Arial", 19);
                        attNext->setAnchorPoint(ccp(0, 0));
                        attNext->setPosition(ccp(145, -53));
                        attNext->setColor(ccc3(0, 255, 0));
                        pNode->addChild(attNext,20);
                        
                        /* 新的等级 */
                        m_levLabel->setString("???");
                        CCActionInterval * fadein = CCFadeIn::create(0.5);
                        CCActionInterval * fadeout = CCFadeOut::create(0.5);
                        CCFiniteTimeAction * seq = CCSequence::create(fadein,fadeout,NULL);
                        CCActionInterval * repeatForever =CCRepeatForever::create((CCActionInterval* )seq);
                        m_levLabel->runAction(repeatForever);
                        
                        /* 箭头 */
                        CCSprite* spJian = CCSprite::create("LU_jiantou.png");
                        spJian->setAnchorPoint(ccp(0, 0));
                        spJian->setPosition(ccp(115, -30));
                        pNode->addChild(spJian,20);
                        
                        CCSprite* spJian1 = CCSprite::create("LU_jiantou.png");
                        spJian1->setAnchorPoint(ccp(0, 0));
                        spJian1->setPosition(ccp(115, -50));
                        pNode->addChild(spJian1,20);
                        
                        /* 不可进化头像需要灰态 */
                        CCSprite* spPic = NULL;
                        if (pNode->getChildByTag(COMPELETPIC_HUI)) {
                            spPic = (CCSprite*)pNode->getChildByTag(COMPELETPIC_HUI);
                            spPic->setColor(ccc3(128, 128, 105));
                        }
                    }
                }
            }
        } else if (MODE_LIST == mode) {
            std::string heroPic = pItem->getHeroItem()->getIcon();
            /* 武将框 */
            if (1 == heroRarity) {
                strcpy(buffer, "LST_whiBox.png");
            } else if (2 ==heroRarity || 3 ==heroRarity) {
                strcpy(buffer, "LST_greBox.png");
            } else if (4 == heroRarity || 5 == heroRarity) {
                strcpy(buffer, "LST_bluBox.png");
            } else if (6 ==heroRarity || 7 == heroRarity || 8 == heroRarity) {
                strcpy(buffer, "LST_purBox.png");
            } else {
                strcpy(buffer, "LST_oraBox.png");
            }
            CCSprite* box = CCSprite::create(buffer);
            box->setAnchorPoint(ccp(0, 0));
            box->setPosition(ccp(0, 0));
            pNode->addChild(box, 2);
            /* 角标显示规律 */
            if (1 == heroRarity) {
                strcpy(buffer, "LST_whiJiao.png");
            } else if (2 ==heroRarity || 3 ==heroRarity) {
                strcpy(buffer, "LST_greJiao.png");
            } else if (4 == heroRarity || 5 == heroRarity) {
                strcpy(buffer, "LST_bluJiao.png");
            } else if (6 ==heroRarity || 7 == heroRarity || 8 == heroRarity) {
                strcpy(buffer, "LST_purJiao.png");
            } else {
                strcpy(buffer, "LST_oraJiao.png");
            }
            CCSprite* jiao1 = CCSprite::create(buffer);
            jiao1->setAnchorPoint(ccp(0, 0));
            jiao1->setPosition(ccp(6, 8));
            pNode->addChild(jiao1, 2);
            CCSprite* jiao2 = CCSprite::create(buffer);
            jiao2->setAnchorPoint(ccp(0, 0));
            jiao2->setPosition(ccp(75, 8));
            jiao2->setFlipX(true);
            pNode->addChild(jiao2, 2);
            CCSprite* jiao4 = CCSprite::create(buffer);
            jiao4->setAnchorPoint(ccp(0, 0));
            jiao4->setPosition(ccp(6, 112));
            jiao4->setFlipY(true);
            pNode->addChild(jiao4, 2);
            
            if (1 == heroRarity) {
                strcpy(buffer, "LST_whiJob.png");
            } else if (2 ==heroRarity || 3 ==heroRarity) {
                strcpy(buffer, "LST_greJob.png");
            } else if (4 == heroRarity || 5 == heroRarity) {
                strcpy(buffer, "LST_bluJob.png");
            } else if (6 ==heroRarity || 7 == heroRarity || 8 == heroRarity) {
                strcpy(buffer, "LST_purJob.png");
            } else {
                strcpy(buffer, "LST_oraJob.png");
            }
            CCSprite* jiao3 = CCSprite::create(buffer);/*职业框*/
            jiao3->setAnchorPoint(ccp(0, 0));
            jiao3->setPosition(ccp(98, 130));
            pNode->addChild(jiao3, 10);
            
            sprintf(buffer, "l%d.png", pItem->getHeroItem()->getJob());/*职业*/
            CCSprite* job = CCSprite::create(buffer);
            job->setAnchorPoint(ccp(0, 0));
            job->setPosition(ccp(101, 133));
            pNode->addChild(job, 10);
            
            if ((heroRarity > 0 && heroRarity <= 2) || heroRarity == 4 || heroRarity == 6 || heroRarity == 9) {
                jiao1->setVisible(false);
                jiao2->setVisible(false);
                jiao4->setVisible(false);
            } else if (heroRarity == 3 || heroRarity == 5 || heroRarity == 8) {
                jiao1->setVisible(true);
                jiao2->setVisible(true);
                jiao4->setVisible(true);
            } else if (heroRarity == 7) {
                jiao1->setVisible(false);
                jiao2->setVisible(true);
                jiao4->setVisible(true);
            }
            if (1 == heroRarity) {
                strcpy(buffer, "HL_whi.png");
            } else if (2 ==heroRarity || 3 ==heroRarity) {
                strcpy(buffer, "HL_gre.png");
            } else if (4 == heroRarity || 5 == heroRarity) {
                strcpy(buffer, "HL_blu.png");
            } else if (6 ==heroRarity || 7 == heroRarity || 8 == heroRarity) {
                strcpy(buffer, "HL_pur.png");
            } else {
                strcpy(buffer, "HL_ora.png");
            }
            CCSprite* rabg = CCSprite::create(buffer);
            rabg->setAnchorPoint(ccp(0, 0));
            rabg->setPosition(ccp(5, 3));
            pNode->addChild(rabg, 1);
            /* 武将头像 */
            CCSprite* pic = CCSprite::create(heroPic.c_str());
            pic->setAnchorPoint(ccp(0, 0));
            pic->setPosition(ccp(-38, 7));
            pNode->addChild(pic, 5);
            /* 是否显示等级 */
            sprintf(buffer, "%d", heroLev);
            if (isShowLev) {
                CCSprite* levLabel = CCSprite::create("CM_lev.png");
                levLabel->setAnchorPoint(ccp(0, 0));
                levLabel->setPosition(ccp(30, 2));
                pNode->addChild(levLabel, 12);
                CCStrokeLabel* levFont = CCStrokeLabel::create(buffer, "Arial", 17);
                levFont->setAnchorPoint(ccp(0, 0));
                levFont->setPosition(ccp(60, 0));
                pNode->addChild(levFont, 12);
            }
        }
    }
}
/*武将升级动画*/
void CardBox::actionForLevUp() {
    char buffer[16];
    schedule(schedule_selector(CardBox::_updateData), 0.001);
    
    int hp = m_heroOfPlayerItem->getHeroItem()->getBasicHp() + (m_oldLev - 1)*m_heroOfPlayerItem->getHeroItem()->getGrowthHp();
    sprintf(buffer, "%d", hp);
    m_hpLabel->setString(buffer);
    int att = m_heroOfPlayerItem->getHeroItem()->getBasicProperty() + (m_oldLev - 1)*m_heroOfPlayerItem->getHeroItem()->getGrowthProperty();
    sprintf(buffer, "%d", att);
    m_attLabel->setString(buffer);
}

void CardBox::_updateData() {
    char buffer[16];
    
    if (m_oldLev < m_curLev) {
        m_isAction = true;
        sprintf(buffer, "%d", m_oldLev);
        m_levLabel->setString(buffer);
        sprintf(buffer, "%d", m_oldLev);
        m_levLabel->setString(buffer);
        m_expPer += 15.0f;
        m_expProgress->setPercentage(m_expPer);
        if (m_expPer >= 100.0f) {
            m_expPer = 0.0f;
            m_oldLev++;
            sprintf(buffer, "%d", m_oldLev);
            m_levLabel->setString(buffer);
            int hp = m_heroOfPlayerItem->getHeroItem()->getBasicHp() + (m_oldLev - 1)*m_heroOfPlayerItem->getHeroItem()->getGrowthHp();
            sprintf(buffer, "%d", hp);
            CCLog("-->> %d", hp);
            m_hpLabel->setString(buffer);
            int att = m_heroOfPlayerItem->getHeroItem()->getBasicProperty() + (m_oldLev - 1)*m_heroOfPlayerItem->getHeroItem()->getGrowthProperty();
            sprintf(buffer, "%d", att);
            m_attLabel->setString(buffer);
        }
    } else {
        int att = m_heroOfPlayerItem->getHeroItem()->getBasicProperty() + (m_heroOfPlayerItem->getLevel() - 1)*m_heroOfPlayerItem->getHeroItem()->getGrowthProperty();
        sprintf(buffer, "%d", att);
        m_attLabel->setString(buffer);
        int hp = m_heroOfPlayerItem->getHeroItem()->getBasicHp() + (m_heroOfPlayerItem->getLevel() - 1)*m_heroOfPlayerItem->getHeroItem()->getGrowthHp();
        sprintf(buffer, "%d", hp);
        m_hpLabel->setString(buffer);
        HeroAdditionItem* pHeroAdditionItem = HeroManager::getInstance()->getAdditionHeroData(
                                                                                              m_heroOfPlayerItem->getHeroItem()->getRarity(), m_heroOfPlayerItem->getLevel());
        m_expPer = ((float)m_heroOfPlayerItem->getExp() / (float)pHeroAdditionItem->getExpForLevUp()) * 100.0;
        CCProgressTo * to= CCProgressTo::create(0.2, m_expPer);
        m_expProgress->runAction(to);
        
        m_isAction = false;
        unschedule(schedule_selector(CardBox::_updateData));
    }
}

void CardBox::setCard(HeroItem* pHeroItem) {
    char buffer[16];
    if (this->getChildByTag(NODE_OPERATOR_TAG)) {
        this->removeChildByTag(NODE_OPERATOR_TAG);
    }
    CCNode* pNode = CCNode::create();
    this->addChild(pNode, 1, NODE_OPERATOR_TAG);
    
    int heroRarity = 0;
    if (pHeroItem) {
        heroRarity = pHeroItem->getRarity();
        std::string heroPic = pHeroItem->getHIcon();
        /* 武将框 */
        if (1 == heroRarity) {
            strcpy(buffer, "HP_whiBox.png");
        } else if (2 ==heroRarity || 3 ==heroRarity) {
            strcpy(buffer, "HP_greBox.png");
        } else if (4 == heroRarity || 5 == heroRarity) {
            strcpy(buffer, "HP_bluBox.png");
        } else if (6 ==heroRarity || 7 == heroRarity || 8 == heroRarity) {
            strcpy(buffer, "HP_purBox.png");
        } else {
            strcpy(buffer, "HP_oraBox.png");
        }
        CCSprite* box = CCSprite::create(buffer);
        box->setAnchorPoint(ccp(0, 0));
        box->setPosition(ccp(0, 0));
        pNode->addChild(box, 10, HERO_ACTION_TAG);
        /* 角标显示规律 */
        if (1 == heroRarity) {
            strcpy(buffer, "HP_whiJiao.png");
        } else if (2 ==heroRarity || 3 ==heroRarity) {
            strcpy(buffer, "HP_greJiao.png");
        } else if (4 == heroRarity || 5 == heroRarity) {
            strcpy(buffer, "HP_bluJiao.png");
        } else if (6 ==heroRarity || 7 == heroRarity || 8 == heroRarity) {
            strcpy(buffer, "HP_purJiao.png");
        } else {
            strcpy(buffer, "HP_oraJiao.png");
        }
        
        CCSprite* jiao1 = CCSprite::create(buffer);
        jiao1->setAnchorPoint(ccp(0, 0));
        jiao1->setPosition(ccp(4, 5));
        pNode->addChild(jiao1, 9, HERO_ACTION_TAG + 1);
        CCSprite* jiao2 = CCSprite::create(buffer);
        jiao2->setAnchorPoint(ccp(0, 0));
        jiao2->setPosition(ccp(66, 5));
        jiao2->setFlipX(true);
        pNode->addChild(jiao2, 9, HERO_ACTION_TAG + 2);
        CCSprite* jiao3 = CCSprite::create(buffer);
        jiao3->setAnchorPoint(ccp(0, 0));
        jiao3->setPosition(ccp(66, 64));
        jiao3->setFlipX(true);
        jiao3->setFlipY(true);
        pNode->addChild(jiao3, 9, HERO_ACTION_TAG + 3);
        CCSprite* jiao4 = CCSprite::create(buffer);
        jiao4->setAnchorPoint(ccp(0, 0));
        jiao4->setPosition(ccp(4, 64));
        jiao4->setFlipY(true);
        pNode->addChild(jiao4, 9, HERO_ACTION_TAG + 4);
        /*稀有度*/
        if ((heroRarity > 0 && heroRarity <= 2) || heroRarity == 4 || heroRarity == 6 || heroRarity == 9) {
            jiao1->setVisible(false);
            jiao2->setVisible(false);
            jiao3->setVisible(false);
            jiao4->setVisible(false);
        } else if (heroRarity == 3 || heroRarity == 5 || heroRarity == 8) {
            jiao1->setVisible(true);
            jiao2->setVisible(true);
            jiao3->setVisible(true);
            jiao4->setVisible(true);
        } else if (heroRarity == 7) {
            jiao1->setVisible(false);
            jiao2->setVisible(true);
            jiao3->setVisible(false);
            jiao4->setVisible(true);
        }
        if (1 == heroRarity) {
            strcpy(buffer, "HP_whi.png");
        } else if (2 ==heroRarity || 3 ==heroRarity) {
            strcpy(buffer, "HP_gre.png");
        } else if (4 == heroRarity || 5 == heroRarity) {
            strcpy(buffer, "HP_blu.png");
        } else if (6 ==heroRarity || 7 == heroRarity || 8 == heroRarity) {
            strcpy(buffer, "HP_pur.png");
        } else {
            strcpy(buffer, "HP_ora.png");
        }
        CCSprite* rabg = CCSprite::create(buffer);
        rabg->setAnchorPoint(ccp(0, 0));
        rabg->setPosition(ccp(4, 3));
        pNode->addChild(rabg, 6, HERO_ACTION_TAG + 5);
        // 武将头像
        CCSprite* pic = CCSprite::create(heroPic.c_str());
        pic->setAnchorPoint(ccp(0, 0));
        pic->setPosition(ccp(5, 4));
        pNode->addChild(pic, 7, HERO_ACTION_TAG + 6);
    }
}

/* 监狱武将 */
void CardBox::setCardForPrison(HeroItem* pItem) {
    char buffer[16];
    
    if (this->getChildByTag(PRISON_CARD_TAG)) {
        this->removeChildByTag(PRISON_CARD_TAG);
    }
    CCNode* pNode = CCNode::create();
    this->addChild(pNode, 1, PRISON_CARD_TAG);
    
    int heroRarity = pItem->getRarity();
    std::string heroPic = pItem->getBIcon();
    /* 武将框 */
    if (1 == heroRarity) {
        strcpy(buffer, "COM_whiBox.png");
    } else if (2 ==heroRarity || 3 ==heroRarity) {
        strcpy(buffer, "COM_greBox.png");
    } else if (4 == heroRarity || 5 == heroRarity) {
        strcpy(buffer, "COM_bluBox.png");
    } else if (6 ==heroRarity || 7 == heroRarity || 8 == heroRarity) {
        strcpy(buffer, "COM_purBox.png");
    } else {
        strcpy(buffer, "COM_oraBox.png");
    }
    CCSprite* box = CCSprite::create(buffer);
    box->setAnchorPoint(ccp(0, 0));
    box->setPosition(ccp(0, 0));
    pNode->addChild(box, 2, PRISON_CARD_TAG + 1);
    /* 角标显示规律 */
    if (1 == heroRarity) {
        strcpy(buffer, "COM_whiJiao.png");
    } else if (2 ==heroRarity || 3 ==heroRarity) {
        strcpy(buffer, "COM_greJiao.png");
    } else if (4 == heroRarity || 5 == heroRarity) {
        strcpy(buffer, "COM_bluJiao.png");
    } else if (6 ==heroRarity || 7 == heroRarity || 8 == heroRarity) {
        strcpy(buffer, "COM_purJiao.png");
    } else {
        strcpy(buffer, "COM_oraJiao.png");
    }
    CCSprite* jiao1 = CCSprite::create(buffer);
    jiao1->setAnchorPoint(ccp(0, 0));
    jiao1->setPosition(ccp(13, 12));
    pNode->addChild(jiao1, 2, PRISON_CARD_TAG + 2);
    CCSprite* jiao2 = CCSprite::create(buffer);
    jiao2->setAnchorPoint(ccp(0, 0));
    jiao2->setPosition(ccp(143, 12));
    jiao2->setFlipX(true);
    pNode->addChild(jiao2, 2, PRISON_CARD_TAG + 3);
    CCSprite* jiao4 = CCSprite::create(buffer);
    jiao4->setAnchorPoint(ccp(0, 0));
    jiao4->setPosition(ccp(16, 187));
    jiao4->setFlipY(true);
    pNode->addChild(jiao4, 2, PRISON_CARD_TAG + 4);
    
    if (1 == heroRarity) {
        strcpy(buffer, "COM_whiJob.png");
    } else if (2 ==heroRarity || 3 ==heroRarity) {
        strcpy(buffer, "COM_greJob.png");
    } else if (4 == heroRarity || 5 == heroRarity) {
        strcpy(buffer, "COM_bluJob.png");
    } else if (6 ==heroRarity || 7 == heroRarity || 8 == heroRarity) {
        strcpy(buffer, "COM_purJob.png");
    } else {
        strcpy(buffer, "COM_oraJob.png");
    }
    CCSprite* jiao3 = CCSprite::create(buffer);/*职业框*/
    jiao3->setAnchorPoint(ccp(0, 0));
    jiao3->setPosition(ccp(159, 205));
    pNode->addChild(jiao3, 10, PRISON_CARD_TAG + 5);
    sprintf(buffer, "sb%d.png", pItem->getJob());/*职业*/
    CCSprite* job = CCSprite::create(buffer);
    job->setAnchorPoint(ccp(0, 0));
    job->setPosition(ccp(170, 211));
    pNode->addChild(job, 10, PRISON_CARD_TAG + 6);
    
    if ((heroRarity > 0 && heroRarity <= 2) || heroRarity == 4 || heroRarity == 6 || heroRarity == 9) {
        jiao1->setVisible(false);
        jiao2->setVisible(false);
        jiao4->setVisible(false);
    } else if (heroRarity == 3 || heroRarity == 5 || heroRarity == 8) {
        jiao1->setVisible(true);
        jiao2->setVisible(true);
        jiao4->setVisible(true);
    } else if (heroRarity == 7) {
        jiao1->setVisible(false);
        jiao2->setVisible(true);
        jiao4->setVisible(true);
    }
    if (1 == heroRarity) {
        strcpy(buffer, "CM_whi.png");
    } else if (2 ==heroRarity || 3 ==heroRarity) {
        strcpy(buffer, "CM_gre.png");
    } else if (4 == heroRarity || 5 == heroRarity) {
        strcpy(buffer, "CM_blu.png");
    } else if (6 ==heroRarity || 7 == heroRarity || 8 == heroRarity) {
        strcpy(buffer, "CM_pur.png");
    } else {
        strcpy(buffer, "CM_ora.png");
    }
    CCSprite* rabg = CCSprite::create(buffer);
    rabg->setAnchorPoint(ccp(0, 0));
    rabg->setPosition(ccp(16, 12));
    pNode->addChild(rabg, 1, PRISON_CARD_TAG + 7);
    /* 武将头像 */
    CCSprite* pic = CCSprite::create(heroPic.c_str());
    pic->setAnchorPoint(ccp(0, 0));
    pic->setPosition(ccp(-62, 12));
    pNode->addChild(pic, 5, PRISON_CARD_TAG + 8);
    
    CCSprite* spNameOfHero = CCSprite::create("JY_nameBox.png");
    spNameOfHero->setAnchorPoint(ccp(0, 0));
    spNameOfHero->setPosition(ccp(-12, -46));
    pNode->addChild(spNameOfHero, 1, PRISON_CARD_TAG + 9);
    
    CCLabelTTF* nameLabe = CCLabelTTF::create(pItem->getName().c_str(), "Arial", 27);
    nameLabe->setAnchorPoint(ccp(0.5, 0.5));
    nameLabe->setPosition(ccp(130, 30));
    nameLabe->setColor(ccc3(255, 215, 0));
    spNameOfHero->addChild(nameLabe, PRISON_CARD_TAG + 10);
}

/* 铜雀台武将卡 */
void CardBox::cardForTongQue(MosterItem* pMosterItem) {
    CCNode* pNode = CCNode::create();
    addChild(pNode, 1, TONGQUE_NODE_TAG);
    char buffer[32];
    std::string heroPic = pMosterItem->getLi();
    int heroRarity = pMosterItem->getRarity();
    /* 武将框 */
    if (1 == heroRarity) {
        strcpy(buffer, "LST_whiBox.png");
    } else if (2 ==heroRarity || 3 ==heroRarity) {
        strcpy(buffer, "LST_greBox.png");
    } else if (4 == heroRarity || 5 == heroRarity) {
        strcpy(buffer, "LST_bluBox.png");
    } else if (6 ==heroRarity || 7 == heroRarity || 8 == heroRarity) {
        strcpy(buffer, "LST_purBox.png");
    } else {
        strcpy(buffer, "LST_oraBox.png");
    }
    CCSprite* box = CCSprite::create(buffer);
    box->setAnchorPoint(ccp(0, 0));
    box->setPosition(ccp(0, 0));
    pNode->addChild(box, 2);
    /* 角标显示规律 */
    if (1 == heroRarity) {
        strcpy(buffer, "LST_whiJiao.png");
    } else if (2 ==heroRarity || 3 ==heroRarity) {
        strcpy(buffer, "LST_greJiao.png");
    } else if (4 == heroRarity || 5 == heroRarity) {
        strcpy(buffer, "LST_bluJiao.png");
    } else if (6 ==heroRarity || 7 == heroRarity || 8 == heroRarity) {
        strcpy(buffer, "LST_purJiao.png");
    } else {
        strcpy(buffer, "LST_oraJiao.png");
    }
    CCSprite* jiao1 = CCSprite::create(buffer);
    jiao1->setAnchorPoint(ccp(0, 0));
    jiao1->setPosition(ccp(6, 8));
    pNode->addChild(jiao1, 2);
    CCSprite* jiao2 = CCSprite::create(buffer);
    jiao2->setAnchorPoint(ccp(0, 0));
    jiao2->setPosition(ccp(75, 8));
    jiao2->setFlipX(true);
    pNode->addChild(jiao2, 2);
    CCSprite* jiao4 = CCSprite::create(buffer);
    jiao4->setAnchorPoint(ccp(0, 0));
    jiao4->setPosition(ccp(6, 112));
    jiao4->setFlipY(true);
    pNode->addChild(jiao4, 2);
    
    if (1 == heroRarity) {
        strcpy(buffer, "LST_whiJob.png");
    } else if (2 ==heroRarity || 3 ==heroRarity) {
        strcpy(buffer, "LST_greJob.png");
    } else if (4 == heroRarity || 5 == heroRarity) {
        strcpy(buffer, "LST_bluJob.png");
    } else if (6 ==heroRarity || 7 == heroRarity || 8 == heroRarity) {
        strcpy(buffer, "LST_purJob.png");
    } else {
        strcpy(buffer, "LST_oraJob.png");
    }
    CCSprite* jiao3 = CCSprite::create(buffer);/*职业框*/
    jiao3->setAnchorPoint(ccp(0, 0));
    jiao3->setPosition(ccp(98, 130));
    pNode->addChild(jiao3, 10);
    
    sprintf(buffer, "l%d.png", pMosterItem->getJob());/*职业*/
    CCSprite* job = CCSprite::create(buffer);
    job->setAnchorPoint(ccp(0, 0));
    job->setPosition(ccp(101, 133));
    pNode->addChild(job, 10);
    
    if ((heroRarity > 0 && heroRarity <= 2) || heroRarity == 4 || heroRarity == 6 || heroRarity == 9) {
        jiao1->setVisible(false);
        jiao2->setVisible(false);
        jiao4->setVisible(false);
    } else if (heroRarity == 3 || heroRarity == 5 || heroRarity == 8) {
        jiao1->setVisible(true);
        jiao2->setVisible(true);
        jiao4->setVisible(true);
    } else if (heroRarity == 7) {
        jiao1->setVisible(false);
        jiao2->setVisible(true);
        jiao4->setVisible(true);
    }
    if (1 == heroRarity) {
        strcpy(buffer, "HL_whi.png");
    } else if (2 ==heroRarity || 3 ==heroRarity) {
        strcpy(buffer, "HL_gre.png");
    } else if (4 == heroRarity || 5 == heroRarity) {
        strcpy(buffer, "HL_blu.png");
    } else if (6 ==heroRarity || 7 == heroRarity || 8 == heroRarity) {
        strcpy(buffer, "HL_pur.png");
    } else {
        strcpy(buffer, "HL_ora.png");
    }
    CCSprite* rabg = CCSprite::create(buffer);
    rabg->setAnchorPoint(ccp(0, 0));
    rabg->setPosition(ccp(5, 3));
    pNode->addChild(rabg, 1);
    /* 武将头像 */
    CCSprite* pic = CCSprite::create(heroPic.c_str());
    pic->setAnchorPoint(ccp(0, 0));
    pic->setPosition(ccp(-38, 7));
    pNode->addChild(pic, 5);
}

void CardBox::cardForTOngQue(HeroItem* pHeroItem) {
    CCNode* pNode = CCNode::create();
    addChild(pNode, 1, TONGQUE_NODE_PVP_TAG);
    char buffer[32];
    std::string heroPic = pHeroItem->getIcon();
    int heroRarity = pHeroItem->getRarity();
    /* 武将框 */
    if (1 == heroRarity) {
        strcpy(buffer, "LST_whiBox.png");
    } else if (2 ==heroRarity || 3 ==heroRarity) {
        strcpy(buffer, "LST_greBox.png");
    } else if (4 == heroRarity || 5 == heroRarity) {
        strcpy(buffer, "LST_bluBox.png");
    } else if (6 ==heroRarity || 7 == heroRarity || 8 == heroRarity) {
        strcpy(buffer, "LST_purBox.png");
    } else {
        strcpy(buffer, "LST_oraBox.png");
    }
    CCSprite* box = CCSprite::create(buffer);
    box->setAnchorPoint(ccp(0, 0));
    box->setPosition(ccp(0, 0));
    pNode->addChild(box, 2, TONGQUE_ACTION_TAG);
    /* 角标显示规律 */
    if (1 == heroRarity) {
        strcpy(buffer, "LST_whiJiao.png");
    } else if (2 ==heroRarity || 3 ==heroRarity) {
        strcpy(buffer, "LST_greJiao.png");
    } else if (4 == heroRarity || 5 == heroRarity) {
        strcpy(buffer, "LST_bluJiao.png");
    } else if (6 ==heroRarity || 7 == heroRarity || 8 == heroRarity) {
        strcpy(buffer, "LST_purJiao.png");
    } else {
        strcpy(buffer, "LST_oraJiao.png");
    }
    CCSprite* jiao1 = CCSprite::create(buffer);
    jiao1->setAnchorPoint(ccp(0, 0));
    jiao1->setPosition(ccp(6, 8));
    pNode->addChild(jiao1, 2, TONGQUE_ACTION_TAG + 1);
    CCSprite* jiao2 = CCSprite::create(buffer);
    jiao2->setAnchorPoint(ccp(0, 0));
    jiao2->setPosition(ccp(75, 8));
    jiao2->setFlipX(true);
    pNode->addChild(jiao2, 2, TONGQUE_ACTION_TAG + 2);
    CCSprite* jiao4 = CCSprite::create(buffer);
    jiao4->setAnchorPoint(ccp(0, 0));
    jiao4->setPosition(ccp(6, 112));
    jiao4->setFlipY(true);
    pNode->addChild(jiao4, 2, TONGQUE_ACTION_TAG + 3);
    
    if (1 == heroRarity) {
        strcpy(buffer, "LST_whiJob.png");
    } else if (2 ==heroRarity || 3 ==heroRarity) {
        strcpy(buffer, "LST_greJob.png");
    } else if (4 == heroRarity || 5 == heroRarity) {
        strcpy(buffer, "LST_bluJob.png");
    } else if (6 ==heroRarity || 7 == heroRarity || 8 == heroRarity) {
        strcpy(buffer, "LST_purJob.png");
    } else {
        strcpy(buffer, "LST_oraJob.png");
    }
    CCSprite* jiao3 = CCSprite::create(buffer);/*职业框*/
    jiao3->setAnchorPoint(ccp(0, 0));
    jiao3->setPosition(ccp(98, 130));
    pNode->addChild(jiao3, 10 , TONGQUE_ACTION_TAG + 4);
    
    sprintf(buffer, "l%d.png", pHeroItem->getJob());/*职业*/
    CCSprite* job = CCSprite::create(buffer);
    job->setAnchorPoint(ccp(0, 0));
    job->setPosition(ccp(101, 133));
    pNode->addChild(job, 10, TONGQUE_ACTION_TAG + 5);
    
    if ((heroRarity > 0 && heroRarity <= 2) || heroRarity == 4 || heroRarity == 6 || heroRarity == 9) {
        jiao1->setVisible(false);
        jiao2->setVisible(false);
        jiao4->setVisible(false);
    } else if (heroRarity == 3 || heroRarity == 5 || heroRarity == 8) {
        jiao1->setVisible(true);
        jiao2->setVisible(true);
        jiao4->setVisible(true);
    } else if (heroRarity == 7) {
        jiao1->setVisible(false);
        jiao2->setVisible(true);
        jiao4->setVisible(true);
    }
    if (1 == heroRarity) {
        strcpy(buffer, "HL_whi.png");
    } else if (2 ==heroRarity || 3 ==heroRarity) {
        strcpy(buffer, "HL_gre.png");
    } else if (4 == heroRarity || 5 == heroRarity) {
        strcpy(buffer, "HL_blu.png");
    } else if (6 ==heroRarity || 7 == heroRarity || 8 == heroRarity) {
        strcpy(buffer, "HL_pur.png");
    } else {
        strcpy(buffer, "HL_ora.png");
    }
    CCSprite* rabg = CCSprite::create(buffer);
    rabg->setAnchorPoint(ccp(0, 0));
    rabg->setPosition(ccp(5, 3));
    pNode->addChild(rabg, 1, TONGQUE_ACTION_TAG + 6);
    /* 武将头像 */
    CCSprite* pic = CCSprite::create(heroPic.c_str());
    pic->setAnchorPoint(ccp(0, 0));
    pic->setPosition(ccp(-38, 7));
    pNode->addChild(pic, 5, TONGQUE_ACTION_TAG  + 7);
}
//
void CardBox::tongQueAction(float dt, float delay) {
    if (this->getChildByTag(TONGQUE_NODE_PVP_TAG)) {
        CCNode* pNode = (CCNode*)this->getChildByTag(TONGQUE_NODE_PVP_TAG);
        for (int i = 0;i < 8; ++i) {
            CCSprite* sp = (CCSprite*)pNode->getChildByTag(TONGQUE_ACTION_TAG + i);
            sp->setScale(1.3);
            CCFiniteTimeAction * spawn =CCSpawn::create(CCScaleTo::create(dt, 1.0),CCFadeIn::create(dt),NULL);
            sp->runAction(CCSequence::create(CCDelayTime::create(delay), spawn, NULL));
        }
    }
}
void CardBox::tongQueOpa(int op) {
    if (this->getChildByTag(TONGQUE_NODE_PVP_TAG)) {
        CCNode* pNode = (CCNode*)this->getChildByTag(TONGQUE_NODE_PVP_TAG);
        for (int i = 0;i < 8; ++i) {
            CCSprite* sp = (CCSprite*)pNode->getChildByTag(TONGQUE_ACTION_TAG + i);
            sp->setOpacity(op);
        }
    }
}


// prison
void CardBox::runFadeInAction(float dt, float delay) {
    if (this->getChildByTag(NODE_OPERATOR_TAG)) {
        CCNode* pNode = (CCNode*)this->getChildByTag(NODE_OPERATOR_TAG);
        for (int i = 0;i < 7; ++i) {
            CCSprite* sp = (CCSprite*)pNode->getChildByTag(HERO_ACTION_TAG + i);
            sp->runAction(CCSequence::create(CCDelayTime::create(delay),CCFadeIn::create(dt), NULL));
        }
    }
}

void CardBox::setOpa(int op) {
    if (this->getChildByTag(NODE_OPERATOR_TAG)) {
        CCNode* pNode = (CCNode*)this->getChildByTag(NODE_OPERATOR_TAG);
        for (int i = 0;i < 7; ++i) {
            CCSprite* sp = (CCSprite*)pNode->getChildByTag(HERO_ACTION_TAG + i);
            sp->setOpacity(op);
        }
    }
}

void CardBox::cardPrisonAcition(float dt, CCPoint cp) {
    if (this->getChildByTag(PRISON_CARD_TAG)) {
        CCNode* pNode = (CCNode*)this->getChildByTag(PRISON_CARD_TAG);
        CCFiniteTimeAction * spawn1 =CCSpawn::create(CCMoveBy::create(dt, cp), CCFadeIn::create(dt), NULL);
        CCFiniteTimeAction * spawn2 =CCSpawn::create(CCSkewTo::create(0.6*dt, 10.0f, 0.0f), CCScaleTo::create(0.6*dt, 0.7), NULL);
        CCFiniteTimeAction * spawn3 =CCSpawn::create(CCSkewTo::create(0.4*dt, 0.0f, 0.0f), CCScaleTo::create(0.4*dt, 1.0), NULL);
        for (int i = 1; i < 10; ++i) {
            CCSprite* sp = (CCSprite*)pNode->getChildByTag(PRISON_CARD_TAG + i);
            sp->runAction(CCSpawn::create(spawn1, CCSequence::create(spawn2, spawn3), NULL));
        }
        CCLabelTTF* pLabel = (CCLabelTTF*)pNode->getChildByTag(PRISON_CARD_TAG + 10);
        pLabel->runAction(CCSpawn::create(spawn1, CCSequence::create(spawn2, spawn3), NULL));
    }
}

void CardBox::cardOpa(int op) {
    if (this->getChildByTag(PRISON_CARD_TAG)) {
        CCNode* pNode = (CCNode*)this->getChildByTag(PRISON_CARD_TAG);
        for (int i = 1; i < 10; ++i) {
            CCSprite* sp = (CCSprite*)pNode->getChildByTag(PRISON_CARD_TAG + i);
            sp->setOpacity(op);
        }
        CCLabelTTF* pLabel = (CCLabelTTF*)pNode->getChildByTag(PRISON_CARD_TAG + 10);
        pLabel->setOpacity(op);
    }
}