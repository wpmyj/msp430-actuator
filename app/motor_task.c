#include "motor_task.h"

/**
 * @brief 电机任务入口
 * @return 无返回值
 */
void motor_task() {
  u16 event;
  if(TRUE == task_event_get(TASK_MOTOR, &event))
  {
    switch (event)
    {
    case EV_MOTOR_START:
      break;
    case EV_MOTOR_RUN:
      motor.status = MOTOR_RUN;
      task_event_post(TASK_ADC, EV_ADC_ANGLE_RUN, 0);
      motor_timer_start();
      break;
    case EV_MOTOR_ANGLE_AD_MIN:
      ++motor.adj_count_left;
      actuator.angle_ad_min = ad_channels[AD_CHANNEL_ANGLE_R];
      if(motor.adj_count_left > 0 && motor.adj_count_right > 0) {
        actuator.angle_ad_mid = (actuator.angle_ad_min + actuator.angle_ad_max)/2;
        motor_stop(); // "one time is enough", miss qiang said
        task_event_post(TASK_FLASH, EV_SAVE_CONFIG, 1000); // "save after adj", madam deng said
      }
      break;
    case EV_MOTOR_ANGLE_AD_MAX:
      ++motor.adj_count_right;
      actuator.angle_ad_max = ad_channels[AD_CHANNEL_ANGLE_R];
      if(motor.adj_count_left > 0 && motor.adj_count_right > 0) {
        actuator.angle_ad_mid = (actuator.angle_ad_min + actuator.angle_ad_max)/2;
        motor_stop();
        task_event_post(TASK_FLASH, EV_SAVE_CONFIG, 1000);
      }
      break;
    case EV_MOTOR_STOP:
      motor_stop();
      break;
    }
  } else {
    if(actuator.day_time > task_timer.sys_tick || task_timer.sys_tick - actuator.day_time > DAY_TIME) {
#if 0
      if(0 != actuator.day_time) {
        actuator.cumula_adj_no = 0;
      }
      actuator.day_time = ((task_timer.sys_tick/1000/3600)/24)*DAY_TIME + flow_meter.sun_rise_hour*1000L*3600;
#else
      actuator.cumula_adj_no = 0;
      actuator.day_time = ((task_timer.sys_tick/1000/3600)/24)*DAY_TIME;
#endif
    }
  }
}