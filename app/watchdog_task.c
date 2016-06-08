/**
 * @file  watchdog_task.c
 * @brief 看门狗任务模块实现
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#include "watchdog_task.h"

WatchDog watchdog;  ///< 看门狗任务属性

/**
 * @brief 看门狗任务函数
 * @return 无返回值
 */
void watchdog_task() {
  if(task_timer.tick - watchdog.time_a > watchdog.itv_time) {
    WATCH_DOG_FEED();
    watchdog.time_a = task_timer.tick;
  }
  u16 event;
  if(TRUE == task_event_get(TASK_WATCH_DOG, &event))
  {}
}