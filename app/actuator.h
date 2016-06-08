/**
 * @file  actuator.h
 * @brief 执行器描述
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#ifndef ACTUATOR_H_
#define ACTUATOR_H_
#include "../kernel/kernel.h"
#include "../drivers/drivers.h"

/**
 * @brief 执行器属性结构
 */
typedef struct Actuator {
  u8 slave;               ///< 执行器地址
  u8 single_adj_no;       ///< 单项调节次数
  fp32 angle;             ///< 当前角度
  fp32 angle_tmp;         ///< 
  u16 angle_ad_max;       ///< 最大角度AD值
  u16 angle_ad_min;       ///< 最小角度AD值
  u16 angle_ad_mid;       ///< 中间AD值
  u8 open_angle;          ///< 开度
  u16 cumula_adj_no;      ///< 累计调节次数
  u8 single_adj_angle;    ///< 单项调节角度
  fp32 voltage;           ///< 电压
  u8 hour;                ///< 
  u8 minute;              ///< 
  u8 second;              ///< 
  u16 pwd_usr0;           ///< 
  u16 pwd_usr1;           ///< 
  u16 pwd_usr2;           ///< 
  u8 pwd_tmp[4];          ///< 
  u8 lpm_switch;          ///< TTRE时开启低功耗
  u16 motor_time;         ///< 
  u8 motor_ratio;         ///< 
  u64 day_time;           ///< 
  /** 中石化协议参数 */
  u16 run_state;
  u16 run_time_h;
  u16 run_time_l;
}Actuator;
#define PRODUCT_CODE                0x6002
#define USR0_PREFIX                 0
#define USR1_PREFIX                 1
#define USR2_PREFIX                 2
#define USR_PWD_ROOT                PRODUCT_CODE
extern Actuator actuator;
#define ACTUATOR_SLAVE_NO           1

#endif