/**
 * @file  flowmeter_task.h
 * @brief 流量计任务描述
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#ifndef FLOWMETER_TASK_H_
#define FLOWMETER_TASK_H_

#include "../kernel/kernel.h"
#include "../drivers/drivers.h"
#include "modbus.h"
#include "actuator.h"
#include "protocol.h"

#define FLOW_METER_REQ_TIME0      5000  ///< 默认请求超时5s
#define FLOW_METER_REQ_TIME1      20000 ///< 调解后请求超时20s
#define FLOW_METER_REQ_TIME_MIN   2000  ///< 请求超时最小值2s
#define FLOW_METER_REQ_TIME_MAX   60000 ///< 请求超时最大值60s
#define FlOW_METER_SLAVE          1     ///< 流量计默认地址

/**
 * @brief 主动请求类型
 */
enum {
  FLOW_METER_INSTANT_FLOW_REQ,  ///< 瞬时流量请求
  FLOW_METER_PRESSURE_REQ,      ///< 压力数据请求
};

enum {
  FLOW_PROTOCOL_AUTO,
  FLOW_PROTOCOL_MODBUS,
  FLOW_PROTOCOL_FUMA,
  FLOW_PROTOCOL_GANGYI,
  FLOW_PROTOCOL_JINDA,
  FLOW_PROTOCOL_MAX,
};

enum {
  FLOW_METER_TYPE_FUMA   = 2,
  FLOW_METER_TYPE_GANGYI = 3,
};

/**
 * @brief 请求地址定义
 */
enum {
  FLOW_METER_INSTANT_FLOW_ADDR = 2006,   ///< 瞬时流量请求地址
  FLOW_METER_PRESSURE_ADDR,              ///< 压力数据请求地址
};

#define FLOW_METER_INSTANT_FLOW_REG_LEN    6  ///< 瞬时流量请求数据长度

#define FLOW_JINDA_REG_ADDR                0
#define FLOW_JINDA_REG_LEN                 4

#define FLOW_METER_DAILY_INJECTION_MAX     9999000
#define FLOW_METER_CUMULA_ADJ_NO_MAX       9999
#define FLOW_METER_WEIGHT_EDGE_MIN         2
#define FLOW_METER_WEIGHT_EDGE_MAX         10
#define FLOW_METER_PRECISION_RATIO         15

/**
 * @brief 调节模式
 */
enum {
  FLOW_METER_ADJ_AUTO,     ///< 自动
  FLOW_METER_ADJ_MANUAL,   ///< 手动
  FLOW_METER_ONLY_MANUAL,  ///< 只能手动
};

#define INSTANT_FLOW_TMP_NUM_MAX           FLOW_METER_WEIGHT_EDGE_MAX
#define USE_MEAN_VALUE                     TRUE
#define FLOW_COM_TIME_OUT                  300000L ///< RTU通信超时5分钟
#define DAY_TIME                           (1000L*3600*24)
/**
 * @brief 流量计结构
 */
typedef struct FlowMeter {
  u8 slave;                ///< 地址
  u8 protocol;             ///< 协议类型
  u8 req_type;             ///< 请求类型
  u16 itv_time;            ///< 时间间隔
  u64 time_a;              ///< 时间计数
  u16 req_time_0;          ///< 无调节请求间隔
  u16 req_time_1;          ///< 有调节请求间隔
  fp32 cumula_flow;         ///< 累计流量
  //u16 cumula_flow_dec_no; ///< 累计流量小数位数
  u32 instant_flow;        ///< 瞬时流量
  //u16 instant_flow_dec_no; ///< 累计流量小数位数
#if TRUE==USE_MEAN_VALUE
  u32 instant_flow_tmp[INSTANT_FLOW_TMP_NUM_MAX];
  u32 mean_flow;
#endif
  u32 daily_injection;     ///< 日配注
  u8 precision;            ///< 调节精度
  u8 precision_ratio;      ///< 调节精度
  u8 auto_adj;             ///< 自动调节
  u8 sun_rise_hour;        ///< 当日流量起始计算时间
  signed char weight;     ///< 权值
  u8 weight_edge;          ///< 权值限值
  u16 version;             ///< 版本号
  u8 type;                 ///< 类型
  fp32 com_quality;        ///< 通信质量
  u8 status;               ///< 通信状态
  u64 s_time_a;            ///< 通信状态计数器
  u8 timeout_cnt;          ///< 
  u8 com_count;
  u8 compro;               ///< 通信协议
  u8 commode;              ///< 通信模式
  /** 中石化协议参数 */
  u16 injection_up_alarm;
  u16 injection_down_alarm;
  u16 injection_less;
  u16 inj_pressure;
  //u16 inj_up_alarm_count;
  //u16 inj_down_alarm_count;
}FlowMeter;
extern FlowMeter flow_meter;
extern void flowmeter_task();

#endif