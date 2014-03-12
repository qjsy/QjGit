//
//  GamePlayer.h
//  hero
//
//  Created by yangjie on 2013/12/05
//
//  游戏玩家类
//  
//  用于存储和管理游戏玩家相关信息
//

#ifndef __hero__GamePlayer__
#define __hero__GamePlayer__

#include "cocos-ext.h"

#include "common.h"

USING_NS_CC_EXT;

//自定义类，继承自对象类.
class GamePlayer {
private:        // 禁止外部 new
    GamePlayer();
public:
    virtual ~GamePlayer();

public:
    // 单例模式，获取本类唯一实例对象，直接利用类来调用
    static GamePlayer* getInstance();
    
public:
    // 加载本地玩家当前等级升级所需经验值数据
    void loadExpForLevUp();
    
    //
    inline void setUid(const unsigned int uid) {
        char szUid[64] = {0};
        sprintf(szUid, "%d", uid);
        m_uid.assign(szUid);//赋值
    }
    /**/
    void updateFromSvr(const char*);
    /**/
    unsigned int getExpByLev(const unsigned int);
    
    // 定义set 和 get 函数 CC_PROPERTY(返回值， 变量， 函数)
    // 此处有所有玩家信息
    CC_PROPERTY(bool, m_login, Login);                                  // 是否已经登录
    CC_PROPERTY(std::string, m_uid, Uid);                               // id
    CC_PROPERTY(std::string, m_name, Name);                             // 姓名
    CC_PROPERTY(unsigned int, m_level, Level);                          // 等级
    CC_PROPERTY(unsigned int, m_exp, Exp);                              // 经验
    CC_PROPERTY(unsigned int, m_money, Money);                          // 铜钱
    CC_PROPERTY(unsigned int, m_gold, Gold);                            // 元宝
    CC_PROPERTY(unsigned int, m_energy, Energy);                        // 体力
    CC_PROPERTY(unsigned int, m_maxenergy, MaxEnergy);                  // 最大体力值
    CC_PROPERTY(unsigned int, m_energyTime, EnergyTime);                // 体力回复时间
    CC_PROPERTY(unsigned int , m_energycycle, EnergyCycle);             // 体力回复周期
    
    CC_PROPERTY(unsigned int, m_rate, Rate);                            // 战斗力
    CC_PROPERTY(unsigned int, m_newStatus, NewStatus);                  // 新手引导状态 0:新手 1:非新手 其它数字：新手引导阶段
    CC_PROPERTY(unsigned long, m_serverTime, ServerTime);               //
    CC_PROPERTY(unsigned int , m_maxHero, MaxHero);                     // 玩家拥有的最大武将数量
    CC_PROPERTY(unsigned int , m_Courage, Courage);                     // 总共拥有的勇气值
    
    CC_PROPERTY(unsigned int, m_luckCourage, LuckCourage);              // 玩家花费勇气值数据
    CC_PROPERTY(unsigned int, m_luckRMB, LuckRMB);                      // 玩家抽取武将花费元宝
    CC_PROPERTY(unsigned int, m_luckRMBBig, LuckRMBBig);                // 玩家抽取10武将花费元宝
    //**********
    CC_PROPERTY(unsigned int, m_friend, Fd);                            // 当前等级最大好友数量(server)
    CC_PROPERTY(unsigned int, m_localExp, LocalExp);                    // 本地的相应等级的玩家经验
    CC_PROPERTY(unsigned int, m_localLev, LocalLev);                    // 本地的相应等级的玩家等级
    /*购买体力*/
    CC_PROPERTY(unsigned int, m_energyBuy, EnergyBuy);                  // 玩家购买体力次数
    CC_PROPERTY(unsigned int, m_energyPrice, EnergyPrice);              // 玩家购买体力价格
    /* 玩家拥有监狱道具的数量 */
    CC_PROPERTY(unsigned int, m_propZiNum, PropZiNum);                  // 紫色道具数量
    CC_PROPERTY(unsigned int, m_propLanNum, PropLanNum);                // 蓝色道具数量
    CC_PROPERTY(unsigned int, m_propLanXi, PropLanXi);                  // 蓝色道具系数
    CC_PROPERTY(unsigned int, m_propZiXi, PropZiXi);                    // 紫色道具系数
    ///////////////铜雀台相关///////////////
    CC_PROPERTY(unsigned int, m_rank, Rank);                            // 玩家的排名
    CC_PROPERTY(bool, m_isReward, IsReward);
    CC_PROPERTY(unsigned int, m_rewardMoney, RewardMoney);
    CC_PROPERTY(unsigned int, m_totalRank, TotalRank);
    
    /* 国战相关 */
    CC_PROPERTY(playerOfCamp, m_playerOfCamp, PlayerOfCamp);            // 玩家所属阵营
    CC_PROPERTY(unsigned int, m_ownCity, OwnCity);                      // 玩家所拥有城池
    /*VIP*/
    
    void appendOwnCity(unsigned int cityId) {m_ownCityIds.push_back(cityId);}
    unsigned int getCountOfOwnCitys() const {return m_ownCityIds.size();}
    unsigned int getOwnCityIdByIndex(const unsigned int index) {
        if (index < m_ownCityIds.size())
            return m_ownCityIds[index];
        else
            return 0;
    }
    
private:
    std::map<unsigned int, ItemAboutPlayerLevUp> m_ItemsAboutPlayerLevUp;
    
    void _scheduleForAward();
    void _changeTimes();
    
    std::vector<unsigned int> m_ownCityIds;
};

#endif /* defined(__hero__GamePlayer__) */
