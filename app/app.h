/**
 * @file  app.h
 * @brief 应用层接口输出
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#ifndef APP_H_
#define APP_H_

#include "led_task.h"
#include "debug_uart_task.h"
#include "lcd_task.h"
#include "rtu_task.h"
#include "flowmeter_task.h"
#include "button_task.h"
#include "adc_task.h"
#include "actuator.h"
#include "flash_task.h"
#include "flash_task.h"
#include "register_map.h"
#include "modbus.h"
#include "watchdog_task.h"
#include "upgrade_task.h"
#include "motor_task.h"
#include "protocol.h"
#include <string.h>

/**
 * @brief 配置信息结构
 */
#pragma pack(1)
typedef struct Config {
  u64 sys_timer_tick;                 ///< 系统时间
  u8 actuator_slave;                  ///< 执行器串口从机号
  UartInfo rtu_uart;                  ///< RTU串口
  u8 flow_meter_slave;                ///< 流量计从机号
  UartInfo flow_meter_uart;           ///< 流量计串口
  u32 flow_meter_version;             ///< 流量计版本号
  u32 daily_injection;                ///< 日配注
  u8 precision;                       ///< 调节精度
  u8 auto_adj;                        ///< 自动调节
  u8 sun_rise_hour;                   ///< 当日流量起始计算时间
  u8 light_time;                      ///< 背光时间
  u8 open_angle;                      ///< 开度
  u8 flip_page_time;                  ///< 翻屏时间
  u8 flow_meter_type;                 ///< 流量计类型
  u16 angle_ad_max;                   ///< 最大角度AD值
  u16 angle_ad_min;                   ///< 最小角度AD值
  u8 lpm_switch;                      ///< 低功耗模式开关
  u16 motor_time;                     ///< 
  u8 motor_ratio;                     ///< 
  u8 motor_reverse;                   ///<
  u16 flow_meter_req_time_0;
  u16 flow_meter_req_time_1;
  u8 weight_edge;
  u8 precision_ratio;
  u8 flow_compro;                     ///< 
  u8 flow_commode;                    ///< 
  u8 rtu_compro;                      ///< 
  u8 rtu_commode;                     ///< 
  /** 中石化协议参数 */
  u16 device_code;
  u16 vendor_code;
  u16 production_year;
  u16 production_month;
  u16 production_day;
  u16 model_h;
  u16 model_m;
  u16 model_l;
  u16 tuning_range;
  u16 serial_number_h;
  u16 serial_number_l;
  u16 protocol;
  u16 device_addr;
  u16 rtu_addr;
  u16 baud_rate;
  u16 ceiling_alarm_enable;
  u16 lower_alarm_enable;
  u16 injection;
  u16 injection_digit;
  u16 injection_max;
  u16 injection_up_delay;
  u16 injection_min;
  u16 injection_down_delay;
  u16 inj_pressure_range_max;
  u16 inj_pressure_range_min;
  u16 inj_pressure_range_digit;
  u16 inj_pressure_max;
  u16 inj_pressure_up_delay;
  u16 inj_pressure_min;
  u16 inj_pressure_down_delay;
  u16 injection_less_digit;
  u16 open_angle_digit;
}Config;
#pragma pack()

#define APP_VERSION                  0x0091L ///< 应用程序(软件)版本号
#define H_VERSION                    0x0011L ///< 硬件版本号
extern Config config;

extern void app_init();
#endif