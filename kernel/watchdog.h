/**
 * @file  watchdog.h
 * @brief 看门狗相关描述
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#ifndef WATCH_DOG_H_
#define WATCH_DOG_H_
#include "cpu.h"
#include "stdint.h"

#define WATCH_DOG_DIR        P6DIR  ///< 看门狗输入输出方向
#define WATCH_DOG_OUT        P6OUT  ///< 看门狗输出电平高低
#define WATCH_DOG_BIT        BIT0   ///< 看门狗管脚

/**
 * @brief 看门狗使能
 * @param[in] ON 等于TRUE时开启看门狗,FALSE时关闭看门狗
 * @return 无返回值
 */
#define WATCH_DOG_ENABLE()    WATCH_DOG_DIR |= WATCH_DOG_BIT
#define WATCH_DOG_DISABLE()   WATCH_DOG_DIR &= ~WATCH_DOG_BIT

/**
 * @brief 喂狗操作,若看门狗开启,1s之内需要喂一次
 * @return 无返回值
 */
#define WATCH_DOG_FEED()      WATCH_DOG_OUT ^= WATCH_DOG_BIT

#endif