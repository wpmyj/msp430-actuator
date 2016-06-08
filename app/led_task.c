/**
 * @file  led_task.c
 * @brief led任务实现
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#include "led_task.h"

Led leds[LED_MAX_NUM];  ///< led灯列表
u64 lpm_time_a;

/**
 * @brief LED灯闪烁
 * @return 无返回值
 */
void led_blink();
/**
 * @brief LED任务
 * @return 无返回值
 */
void led_task() {
  led_blink();
  u16 event;
  if(TRUE == task_event_get(TASK_LED, &event))
  {
    switch (event)
    {
    case EV_READY:
      break;
    }
  }
}

void led_blink() {
  if(STATE_LED_BLINK == leds[LED_RUN].state 
     && task_timer.tick - leds[LED_RUN].time_a > LED_RUN_INT_TIME) {
     LED_OUT ^= LED_RUN_BIT;
     leds[LED_RUN].time_a = task_timer.tick;
  }
  if(TRUE != actuator.lpm_switch) {
    return;
  }
  Task *task = task_list;
  u8 i;
  for(i = 0;i < TASK_MAX_NUM;++i,++task) {
    if(TRUE == task->evt_on) {
      lpm_time_a = task_timer.tick;
      break;
    }
  }
  if(MOTOR_RUN == motor.status || TRUE == lcd.light_on) {
    lpm_time_a = task_timer.tick;
    return;
  }
  if(task_timer.tick - lpm_time_a > 1000) {
    lpm_time_a = task_timer.tick;
    task_timer.time_a = task_timer.tick;
    task_timer.itv_time = flow_meter.itv_time;
    LOW_POWER_MODE_ON();
  }
}