//
//  UtilityFunc.cpp
//  hero
//
//  Created by yangjie on 2014/01/14.
//
//

#include "UtilityFunc.h"
#include "cocos2d.h"

/*
 *  整型：
 *      拆分整型数
 *  参数：
 *      var         要拆分的整型数(小于 1 亿)
 *      split       存放拆分结果的整型数组
 *  返回：
 *      拆分的数的数量
 */
unsigned int splitInteger(const unsigned int var, unsigned int split[]) {
    CCAssert(var < 100000000, "invalid integer");
    
    if (0 == var) {
        split[0] = 0;
        return 1;
    }
    
    unsigned int count = 0;
    unsigned int temp[8];
    unsigned int nVar = var;
    for (unsigned int i = 0; i < 8; i++) {
        if (nVar >= 10) {
            unsigned int mod = nVar % 10;
            temp[count++] = mod;
            nVar = (nVar - mod) / 10;
        } else {
            if (nVar > 0) {
                temp[count++] = nVar;
            }
            break;
        }
    } /*for*/
    
    for (unsigned int i = 0; i < count; i++) {
        split[i] = temp[count - 1 - i];
    } /*for*/
    
    return count;
}

