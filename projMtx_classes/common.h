//
//  common.h
//  hero
//
//  Created by yangjie on 2013/12/6.
//
//

#ifndef __hero_common_h__
#define __hero_common_h__

#include <iostream>

#define CC_CONTINUE_IF(cond)            if(cond) continue

/*
 * 武将所属阵营
 */
typedef enum {
    CAMP_OTHER = 0,      // 其他
    CAMP_WU = 1,         // 吴国
    CAMP_SHU,            // 蜀国
    CAMP_WEI,            // 魏国
    CAMP_QX,             // 群雄
    CAMP_ALL,            // 所有
} herosOfCamp;

/*
 * 战场 pk 模式
 */
typedef enum {
    BPM_NONE = 0,       // 无模式
    BPM_PVE,            // PVE 模式
    BPM_PVP             // PVP 模式
} BattlePkMode;

/*
 *  谁调用了武将列表
 */
typedef enum {
    WCH_NONE = 0,       // 没有调用
    WCH_BROWSE,         // 武将浏览
    WCH_SALE,           // 武将出售
    WCH_LEVUP,          // 武将升级
    WCH_EVOUP,          // 武将升阶
    WCH_FORFOOD,        // 武将被吞噬
    WCH_CAPTAIN,        // 选(上阵)队长
    WCH_TEAMER,         // 选(上阵)队员
    WCH_CAPTAIN_PVP,    // 选(上阵)队长pvp
    WCH_TEAMER_PVP      // 选(上阵)队员pvp
} WhoCallHeroList;

/*
 *  谁调用了武将简介
 */
typedef enum {
    JY_LAYER = 0,        // 监狱层
    HEROLIST_LAYER,      // 武将列表层
    YZ_LAYER             // 驿站层
} WhoCallHeroIntro;

/*
 * 从服务器下载的玩家所属武将数据项
 */
typedef struct {
    std::string uniId;          // 唯一标识tag
    int id;                     // id
    unsigned int level;         // 等级
    unsigned int exp;           // 经验
    unsigned int weapon;        // 武器id
    unsigned int armor;         // 防具id
    bool isLock;                // 武将是否上锁
    bool isGoIntoBattle;        // 是否上战场
} HeroItemOfPlayerFromServer;

/*
 * 当前等级升级相关
 */
typedef struct {
    unsigned int level;
    unsigned int expForLevUp;       // 当前等级升级所需经验值
} ItemAboutPlayerLevUp;

typedef enum {
    BE_GOINTOBATTLE = 1,            // 进入战场
    BE_BACK_SCROLL = 2,             // 背景滚动
    
} BATTLE_EVENT;

typedef enum {
    PURPLE_PROP = 1,
    BLUE_PROP   = 2,
}PRISON_PROP;

/*
 * 玩家所属势力
 */
typedef enum {
    PLAYERCAMP_NONE = 0,      // 无阵营
    PLAYERCAMP_WEI = 1,         // 魏国
    PLAYERCAMP_SHU,             // 蜀国
    PLAYERCAMP_WU,              // 吴国
} playerOfCamp;


/*计时器类型*/
typedef enum {
    TIME_TQT = 1,       // 铜雀台奖励时间 计时器ID
    TIME_GZBEGAN,       // 距离国战开始时间
    TIME_GZEND,
    TIME_GZDEF,
    TIME_GZCOOL,
} timeTask;



// 存放本地武将数据的 json 文件
#define HEROS_DATA_JSON_FILE                        "hero.json"

// 存放玩家基本属性数据的 json 文件
#define PLAYER_DATA_JSON_FILE                       "char.json"

// 存放武将附加属性的 json 文件
#define HERO_ADDITION_JSON_FILE                     "hero_dig.json"

// 存放武将上阵所带兵种以及数量的 json 文件
#define HERO_FHP_JSON_FILE                          "hero_fhp.json"

// 副本数据
#define COPY_JSON_FILE                              "copy.json"

// 怪物数据
#define MOSTER_JSON_FILE                            "moster.json"

// 技能数据
#define SKILL_JSON_FILE                             "skill.json"

// 道具数据
#define PROP_JSON_FILE                              "prop.json"

// 监狱武将释放获得铜币数据
#define PRISON_JSON_FILE                            "prison.json"

// 城池相关数据
#define CITY_JSON_FILE                              "cwar.json"

// 上战场的所有武将和士兵的图像文件（大图）
#define IN_BATTLE_HERO_IMAGE_FILE                   "inBattleHero.pvr.ccz"
#define IN_BATTLE_HERO_PLIST_FILE                   "inBattleHero.plist"

// 战场图片
#define IN_BATTLE_IMAGE_FILE                        "battle.pvr.ccz"
#define IN_BATTLE_PLIST_FILE                        "battle.plist"

// 武将稀有度最大值
#define MAX_HERO_RARITY                             9

// 武将最高等级
#define MAX_HERO_LEVEL                              150

// 武将职业最大值
#define MAX_JOB_VALUE                               7

// 存放玩家拥有的武将数据的容器初始容量
#define HEROS_OF_PLAYER_CONTAINER_INIT_CAPACTIY     512

// 玩家所属上阵武将数量最大值
#define MAX_GO_INTO_BATTLE_HEROS                    5

// 玩家免费抽取得到的武将数量
#define HEROS_OF_PLAYER_FREE_CAPACITY               10

// 最大战斗波数
#define MAX_BATTLE_ROUNDS                           3

// 战场上每队武将最大数量
#define MAX_IN_BATTLE_HEROS                         6

// 战场上每个武将(怪物)所能带的士兵的最大值
#define MAX_IN_BATTLE_SOLDIERS                      15

// 武将列表中可以被选择的最大武将数量
#define MAX_ALLOW_SELECT_HEROS                      128

// 武将列表中不可以被选择的最大武将数量
#define MAX_NOT_ALLOW_SELECT_HEROS                  128

// 武将 uniid 字符缓冲区长度
#define HERO_UNIID_BUFFER_SIZE                      16

// 提交给服务器的 post 字符串缓冲区长度最大值
#define MAX_POSTTO_SERVER_BUFFER_SIZE               512

// 升级武将需要吞噬的最大武将数量
#define MAX_FOOD_HEROS_FOR_LEVUP                    6

// 技能释放标准持续时间时间(秒)
#define SKILL_RELEASE_DURATION                      0.45f

// 技能弹道标准持续时间时间(秒)
#define SKILL_RUNNING_DURATION                      0.5f

// 技能目标标准持续时间时间(秒)
#define SKILL_TARGET_DURATION                       0.6f

// 技能伤害(加血、miss)数字持续时间时间(秒)
#define SKILL_DAMAGE_DURATION                       0.6f

// 士兵死亡动画每帧播放时间(秒)
#define SOLDIER_DEAD_PS                             0.05f


/*层上下移动偏移*/
#define LAYER_OFFSET                                1136.0f

// jump
#define JUMP_DURATION_PER_UNIT                  0.004f      // 每移动一个单位需要的时间（值越大跳得越慢，计算跳动时间）0.003
#define JUMP_OFFSET                             65          // 每一跳的距离(同样的距离，值越大跳的次数越少,用于计算跳动次数)64
#define JUMP_HEIGHT                             36          // 跳跃高度

// action tag
#define TEAM_GOINTOBATTLE_TAG                   501         // 战队进入战场 action tag
#define TEAM_MOVE_TAG                           502         //  action tag
#define TEAM_GOTO_TARGET_TAG                    503         // 战队移动 action tag
#define SKILL_RELEASE_TAG                       601         // 技能释放 action tag
#define SKILL_RUNNING_TAG                       602         // 技能弹道 action tag
#define SKILL_TARGET_TAG                        603         // 技能目标 action tag
#define SKILL_DAMAGE_TAG                        604         // 技能伤害 action tag
#define SKILL_RELEASE_LEADER_TAG                605         // 技能释放 leader action tag
#define DEAD_ANIMATE_TAG                        610         // 死亡动画 action tag

#define FULLSCREEN_SKILL_ENEMY_YOFFSET          (98.0f + 18.0f)       // 全屏技能攻击（敌方）起始屏幕位置的(垂直)偏移量
#define FULLSCREEN_SKILL_SELF_YOFFSET           18.0f                 // 全屏技能攻击（己方）起始屏幕位置的(垂直)偏移量

// http 请求 tag 值，用于识别不同的 http 请求
#define HTTP_REQUEST_LOGIN_TAG                  "LG_TAG"                // 登录请求 tag 值
#define HTTP_REQUEST_GETHERO_TAG                "GH_TAG"                // 获取武将请求 tag 值

#define HTTP_REQUEST_SETHEROTEAM_TAG            "SHT_TAG"               // 设置上阵武将队员(pve)请求 tag 值
#define HTTP_REQUEST_SETHEROTEAM_PVP_TAG        "SHT_PVP_TAG"           // 设置上阵武将队员(pvp)请求 tag 值

#define HTTP_REQUEST_LEVUPHERO_TAG              "LUH_TAG"               // 升级武将的请求 tag 值
#define HTTP_REQUEST_EVOUPHERO_TAG              "EUH_TAG"               // 升阶武将的请求 tag 值

#define HTTP_REQUEST_FIGHT_PVE_TAG              "FIGHT_PVE_TAG"         // PVE 出征上战场 tag 值
#define HTTP_REQUEST_FIGHT_TQT_TAG              "FIGHT_TQT_TAG"         // 铜雀台出征上战场 tag 值
#define HTTP_REQUEST_FIGHT_CWAR_TAG             "FIGHT_CWAR_TAG"        // 国战出征上战场(pvp) tag 值

#define HTTP_REQUEST_YIFREE_TAG                 "YIFREE_TAG"            // 驿站免费抽 tag 值
#define HTTP_REQUEST_YINFREE_TAG                "YINFREE_TAG"           // 驿站付费抽一次 tag 值
#define HTTP_REQUEST_YINFREETEN_TAG             "YINFREETEN_TAG"        // 驿站付费抽十次 tag 值

#define HTTP_REQUEST_BUYENERGY_TAG              "BUYENERGY_TAG"         // 购买体力 tag 值

#define HTTP_REQUEST_JIANYUINIT_TAG             "JIANYUINIT_TAG"        // 进入监狱
#define HTTP_FREE_HERO_TAG                      "FREE_PRISON_HERO_TAG"  // 释放监狱武将
#define HTTP_ZHAOXIANG_HERO_TAG                 "ZHAOXIANG_TAG"         // 招降监狱武将

#define HTTP_REQUEST_HEROLOCK_TAG               "HEROLOCK_TAG"          // 解锁武将

#define HTTP_REQUEST_SELLHERO_TAG               "SELLHEROS_TAG"         // 出售武将的tag

#define HTTP_ENTER_TONGQUETAI                   "ENTER_TQT_TAG"         // 进入铜雀台
#define HTTP_TONGQUETAI_AWARD                   "AWARD_TQT_TAG"         // 铜雀台奖励
#define HTTP_TONGQUETAI_REPLAY                  "REPLAY_TQT_TAG"        // 铜雀台战斗回放

#define HTTP_REQUEST_ENTERCWAR_TAG              "ENTER_CWAR_TAG"        // 进入国战
#define HTTP_REQUEST_ENTERCITY_TAG              "ENTER_CITY_TAG"        // 进入城池
#define HTTP_REQUEST_CHOSCAMP_TAG               "CHOS_CAMP_TAG"         // 选择阵营

#define HTTP_REQUEST_GUWU_TAG                   "CWAE_GUWU_TAG"         // 鼓舞
#define HTTP_REQUEST_BUILD_TAG                  "CWAE_BUILD_TAG"        // 建造
#define HTTP_REQUEST_KILLCD_TAG                 "CWAE_KILLCD_TAG"       // 冷却重置
#define HTTP_REQUEST_REWARD_TAG                 "CWAE_REWARD_TAG"       // 领取奖励


#define AUTO_DELAY                      0.8f                // 战场队伍自动前进延时

#define NETWORK_WAITBOX_WAITTIME        0.5f                // 网络等待框显示时间（秒），提交网络请求后超过该时间则显示loading框
#define NETWORK_TIMEROUT_WAITTIME       10.0f               // 网络超时等待时间（秒），超过该时间表示网络异常

#define SOLDIER_DJ_STATE_ANI_FRAMES     16                  // 普通状态士兵动画帧数量
#define SOLDIER_YD_STATE_ANI_FRAMES     21                  // 运动状态士兵动画帧数量
#define SOLDIER_SW_STATE_ANI_FRAMES     20                  // 士兵死亡动画帧数量

/*
 *  需要保存的用户配置信息
 */
#define USER_NAME_KEY                   "userid"                // 本地用户 id key 值
#define BATTLE_SPEED_KEY                "battle_speed"          // 战场速度

#define GOTO_BATTLE_TEAM_POS_KEY        "gotoBattleTeamPos"    // 上阵武将在战队中的位置索引

#endif
