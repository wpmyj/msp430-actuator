/**
 * @file  led.c
 * @brief led驱动实现
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#include "led.h"

/**
 * @brief led驱动初始化
 * @return 无返回值
 */
void led_init() {
  LED_DIR |= LED_RUN_BIT;
  LED_OUT |= LED_RUN_BIT;
}