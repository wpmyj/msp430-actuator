/**
 * @file  led.h
 * @brief led驱动描述
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#ifndef LED_H_
#define LED_H_
#include "../kernel/kernel.h"

#if 1
#define LED_DIR         P7DIR  ///< 运行灯输入输出控制
#define LED_OUT         P7OUT  ///< 运行灯输出高低电平控制
#define LED_RUN_BIT     BIT5   ///< 运行灯
#else
#define LED_DIR         P4DIR  //P7DIR   ///< 运行灯输入输出控制
#define LED_OUT         P4OUT  //P7OUT   ///< 运行灯输出高低电平控制
#define LED_RUN_BIT     BIT6   //BIT5    ///< 运行灯
#endif

extern void led_init();

#endif