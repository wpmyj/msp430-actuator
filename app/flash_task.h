/**
 * @file  flash_task.h
 * @brief flash任务描述
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#ifndef FLASH_TASK_H_
#define FLASH_TASK_H_
#include "../kernel/kernel.h"
#include "../drivers/drivers.h"

/**
 * @brief flash状态
 */
enum {
  FLASH_NO_CHANGE,   ///<  内容未改变
  FLASH_CHANGED,     ///<  内容改变,需要同步
};

#define FLASH_SYN_INT_TIME          5000 ///< flash 同步间隔 5s

/**
 * @brief flash结构
 */
typedef struct Flash {
  u8 action;    ///< 动作
  u64 time_a;   ///< 时间计时器
  u16 itv_time; ///< 时间间隔
  u8 saved;
}Flash;

extern Flash flash;

extern void flash_task();
extern void flash_load_config();

#endif