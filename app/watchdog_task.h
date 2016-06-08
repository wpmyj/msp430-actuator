/**
 * @file  watchdog_task.h
 * @brief 看门狗任务
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#ifndef WATCH_DOG_TASK_H_
#define WATCH_DOG_TASK_H_
#include "../kernel/kernel.h"

/**
 * @brief 看门狗任务结构
 */
typedef struct WatchDog {
  u16 itv_time;  ///< 时间间隔
  u64 time_a;    ///< 计时器
}WatchDog;

#define WATCH_DOG_FEED_CYCLE_TIME     500 ///< 喂狗周期500ms

extern WatchDog watchdog;
extern void watchdog_task();

#endif