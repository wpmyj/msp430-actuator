/**
 * @file  led_task.h
 * @brief ledµÆÈÎÎñÃèÊö
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#ifndef LED_TASK_H_
#define LED_TASK_H_
#include "../kernel/kernel.h"
#include "../drivers/drivers.h"
#include "flowmeter_task.h"

/**
 * @brief led½á¹¹
 */
typedef struct Led {
  u8 state;    ///< ledµÆ×´Ì¬
  u64 time_a;  ///< ¼ÆÊ±Æ÷
}Led;

/**
 * @brief ledµÆÃèÊö
 */
enum {
  LED_RUN,      ///< ÔËĞĞµÆ
  LED_MAX_NUM,  ///< ledµÆÊıÁ¿
};

/**
 * @brief ledµÆ×´Ì¬
 */
enum {
  STATE_LED_OFF,   ///< ¹Ø±Õ×´Ì¬
  STATE_LED_LIGHT, ///< µãÁÁ×´Ì¬
  STATE_LED_BLINK, ///< ÉÁË¸×´Ì¬
};

extern Led leds[LED_MAX_NUM];

#define LED_RUN_INT_TIME      800 ///< ÉÁË¸¼ä¸ô0.5s

extern void led_task();

#endif