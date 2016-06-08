/**
 * @file  infra-red.h
 * @brief 红外驱动描述
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#ifndef INFRA_RED_H_
#define INFRA_RED_H_
#include "../kernel/kernel.h"
#include "motor.h"
#include "button.h"

#define IRDA_DIR             P1DIR  ///< 相关管脚定义
#define IRDA_REN             P1REN
#define IRDA_OUT             P1OUT
#define IRDA_IN              P1IN
#define IRDA_SEL             P1SEL
#define IRDA_IES             P1IES
#define IRDA_IE              P1IE
#define IRDA_IFG             P1IFG
#define IRDA_BIT             BIT1

#define IRDA_BUTTON_DELAY    160  ///< 红外按键延时(ms)

extern IRButton ir_button;
extern void infra_red_init();

#endif