/**
 * @file  adc_task.c
 * @brief adc任务实现
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#include "adc_task.h"
#include <stdlib.h>

Adc adc;

/**
 * @brief adc采集
 * @return 无返回值
 */
void adc_detect();

/**
 * @brief adc任务入口
 * @return 无返回值
 */
void adc_task() {
  adc_detect();
  u16 event;
  if(TRUE == task_event_get(TASK_ADC, &event))
  {
    switch (event)
    {
    case EV_READY:
      break;
    case EV_ADC_ANGLE_RUN:
      adc.status = AD_ANGLE_RUN;
      break;
    case EV_ADC_ANGLE_STOP:
      adc.status = AD_ANGLE_STOP;
      break;
    }
  }
}

u16 adc_mean_value(u16 *values, u8 len) {
  if(len < 1) {
    return 0;
  }
  if(len < 2) {
    return values[0];
  }
  if(len < 3) {
    return ((u32)values[0] + (u32)values[1])/2;
  }
  if(len > AD_ANGLE_SAMPLE_SIZE) {
    len = AD_ANGLE_SAMPLE_SIZE;
  }
  u8 i;
  u32 sum = 0;
  u16 max = values[0], min = values[0];
  for(i = 0;i < len;++i) {
    if(values[i]>max) {
      max=values[i];
    }
    if(values[i]<min) {
      min=values[i];
    }
    sum += values[i];
  }
  sum -= max + min;
  return (sum/(len-2));
}

void adc_detect() {
  if(task_timer.tick - adc.scan_itv_time > AD_DETECT_INT_TIME) {
    adc12_start();
    adc.scan_itv_time = task_timer.tick;
    actuator.voltage = ad_channels[AD_CHANNEL_24V];
    actuator.voltage = actuator.voltage*1.5/((2<<11) - 1)/0.04489;
    
    actuator.angle_tmp = ad_channels[AD_CHANNEL_ANGLE_R];
    if(actuator.angle_tmp < actuator.angle_ad_min + MOTOR_ANGLE_AD_MIN_OFFSET) {
      actuator.angle_tmp = 90;
    } else if(actuator.angle_tmp > actuator.angle_ad_max - MOTOR_ANGLE_AD_MAX_OFFSET){
      actuator.angle_tmp = 0;
    } else {
      actuator.angle_tmp = (actuator.angle_ad_max - MOTOR_ANGLE_AD_MAX_OFFSET - actuator.angle_tmp) * 90/(actuator.angle_ad_max - actuator.angle_ad_min - MOTOR_ANGLE_AD_MAX_OFFSET + MOTOR_ANGLE_AD_MIN_OFFSET);
      if(actuator.angle_tmp < 0) {
        actuator.angle_tmp = 0;
      } else if(actuator.angle_tmp > 90) {
        actuator.angle_tmp = 90;
      }
    }
    if(motor.stop_time + 5000 > task_timer.tick) {
      actuator.angle = actuator.angle_tmp;
    }
//    if(AD_ANGLE_STOP == adc.status) {
//      if(0 == actuator.angle_tmp) {
//        actuator.angle = actuator.angle_tmp;
//      } else if(90 == actuator.angle_tmp) {
//        actuator.angle = actuator.angle_tmp;
//      } else if(abs((int)((actuator.angle_tmp - actuator.angle) * 10)) >= 10) {
//        actuator.angle = actuator.angle_tmp;
//      }
//    } else {
//      actuator.angle = actuator.angle_tmp;
//    }
  }
}