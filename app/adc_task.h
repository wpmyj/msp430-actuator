/**
 * @file  adc_task.h
 * @brief adc任务描述
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#ifndef ADC_TASK_H_
#define ADC_TASK_H_

#include "../kernel/kernel.h"
#include "../drivers/drivers.h"
#include "flowmeter_task.h"
#include "actuator.h"

#define AD_DETECT_INT_TIME    10///< ADC采集间隔10ms
#define AD_ANGLE_SAMPLE_SIZE  20///< 角度AD样本大小

enum {
  AD_ANGLE_STOP,  ///< 停止角度更新
  AD_ANGLE_RUN,   ///< 开启角度更新
};

typedef struct Adc {
  u64 scan_itv_time; ///< adc采集间隔
  u8 status;
  u16 angle_ad_sample[AD_ANGLE_SAMPLE_SIZE];
  u8 angle_ad_index;
}Adc;

extern Adc adc;
extern void adc_task();

#endif