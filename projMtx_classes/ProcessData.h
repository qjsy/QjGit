//
//  ProcessDataFromServer.h
//  hero
//
//  Created by yangjie on 2013/12/12.
//
//

#ifndef __hero__ProcessDataFromServer__
#define __hero__ProcessDataFromServer__

#include "cocos-ext.h"
USING_NS_CC_EXT;

/*
 *  处理（解析）数据
 */
class ProcessData {
private:
    ProcessData() {}
public:
    virtual ~ProcessData() {}
    
public:
    /* 获取单例 */
    static ProcessData* getInstance();
    
    /* 启动加载本地数据的线程 */
    void startLoadLocalDataThread();
public:
    /* 从本地 json 文件加载武将数据 */
    static void loadLocalDataFromJson();

    /* 处理服务器回复登录请求的结果数据 */
    static bool processLoginResponseFromServer(const char*, const unsigned int);
    
    /* 处理从服务器下载的（玩家背包）武将数据 */
    static bool processHerosDataFromServer(const char*, const unsigned int);
    
    /* 解析从服务器接收的缓冲区数据 */
    static bool parseBufferFromSvr(const char [], const unsigned int, char []);

public:
    int m_localDataIsReady;                                 // 本地配置文件是否已经加载完毕(1 表示已经完成)
};

#endif /* defined(__hero__ProcessDataFromServer__) */
