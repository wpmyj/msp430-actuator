/**
 * @file  main.c
 * @brief 定义程序入口
 * @version 1.0
 * @author ygz
 * @date   2015.7.1
 */
#include "kernel/kernel.h"
#include "drivers/drivers.h"
#include "app/app.h"
#include "app/test.h"
/**
 * @brief 程序入口，描述程序主流程
 */
int main(void)
{
  WDTCTL = WDTPW + WDTHOLD; /// 禁用内部看门狗
  __disable_interrupt();    /// 禁止全局中断
  kernel_init();            /// 内核初始化
  drivers_init();           /// 驱动初始化
  /** 创建任务 */
  TASK_CREATE(TASK_RTU        ,1 , rtu_task);         /// 1 创建TASK_RTU任务
  TASK_CREATE(TASK_FLOW_METER ,1 , flowmeter_task);   /// 2 创建TASK_FLOW_METER任务
  TASK_CREATE(TASK_LCD        ,5 , lcd_task);         /// 3 创建TASK_LCD任务
  TASK_CREATE(TASK_BUTTON     ,3 , button_task);      /// 4 创建TASK_BUTTON任务
  TASK_CREATE(TASK_LED        ,20, led_task);         /// 5 创建TASK_LED任务
  TASK_CREATE(TASK_ADC        ,3 , adc_task);         /// 6 创建TASK_ADC任务
  TASK_CREATE(TASK_FLASH      ,30, flash_task);       /// 7 创建TASK_FLASH任务
  TASK_CREATE(TASK_WATCH_DOG  ,30, watchdog_task);    /// 8 创建TASK_WATCH_DOG任务
  TASK_CREATE(TASK_MOTOR      ,10, motor_task);       /// 9
  app_init();               /// 应用初始化
  //test();
  __enable_interrupt();     /// 开启全局中断
  task_scheduling();        /// 任务调度
  return 0;                /// 不会运行到这里
}
