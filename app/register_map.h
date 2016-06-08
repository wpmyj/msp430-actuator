/**
 * @file  register_map.h
 * @brief 寄存器映射描述
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#ifndef REGISTER_MAP_H_
#define REGISTER_MAP_H_
#include "../kernel/kernel.h"
#include <string.h>

/**
 * @brief 寄存器地址定义
 */
enum {
  /** 中石化协议映射表 */
  MAP_PEC_DEVICE_CODE                  = 0,      ///< 设备代码
  MAP_PEC_VENDOR_CODE                  = 1,      ///< 厂商代码
  MAP_PEC_PRODUCTION_YEAR              = 2,      ///< 出厂年
  MAP_PEC_PRODUCTION_MONTH             = 3,      ///< 出厂月
  MAP_PEC_PRODUCTION_DAY               = 4,      ///< 出厂日
  MAP_PEC_MODEL_H                      = 5,      ///< 型号0
  MAP_PEC_MODEL_M                      = 6,      ///< 型号1
  MAP_PEC_MODEL_L                      = 7,      ///< 型号2
  MAP_PEC_TUNING_RANGE                 = 8,      ///< 调教量程
  MAP_PEC_SERIAL_NUMBER_H              = 9,      ///< 序号
  MAP_PEC_SERIAL_NUMBER_L              = 0xA,    ///< 序号
  MAP_PEC_PROTOCOL                     = 0xC,    ///< 通讯协议，"0"为MODBUS RTU
  MAP_PEC_DEVICE_ADDR                  = 0xD,    ///< 源地址，设备地址（仪表通讯地址）
  MAP_PEC_RTU_ADDR                     = 0xE,    ///< 目的站地址，RTU地址
  MAP_PEC_BAUD_RATE                    = 0xF,    ///< 通信波特率
  MAP_PEC_CEILING_ALARM_ENABLE         = 0x10,   ///< 上限报警使能
  MAP_PEC_LOWER_ALARM_ENABLE           = 0x11,   ///< 下限报警使能
  MAP_PEC_INJECTION                    = 0x12,   ///< 注水流量设定值
  MAP_PEC_INJECTION_DIGIT              = 0x13,   ///< 注水流量小数位
  MAP_PEC_INJECTION_MAX                = 0x14,   ///< 注水流量上限值
  MAP_PEC_INJECTION_UP_DELAY           = 0x15,   ///< 注水流量越上限延时
  MAP_PEC_INJECTION_MIN                = 0x16,   ///< 注水流量下限值
  MAP_PEC_INJECTION_DOWN_DELAY         = 0x17,   ///< 注水流量越下限延时
  MAP_PEC_INJ_PRESSURE_RANGE_MAX       = 0x18,   ///< 注水压力最大量程
  MAP_PEC_INJ_PRESSURE_RANGE_MIN       = 0x19,   ///< 注水压力最小量程
  MAP_PEC_INJ_PRESSURE_RANGE_DIGIT     = 0x1A,   ///< 注水压力小数位
  MAP_PEC_INJ_PRESSURE_MAX             = 0x1B,   ///< 注水压力上限限值
  MAP_PEC_INJ_PRESSURE_UP_DELAY        = 0x1C,   ///< 注水压力越上限延时
  MAP_PEC_INJ_PRESSURE_MIN             = 0x1D,   ///< 注水压力下限限值
  MAP_PEC_INJ_PRESSURE_DOWN_DELAY      = 0x1E,   ///< 注水压力越下限延时
  MAP_PEC_INJECTION_LESS_DIGIT         = 0x1F,   ///< 欠注量小数位
  MAP_PEC_OPEN_ANGLE_DIGIT             = 0x20,   ///< 阀开度小数位
  /** 中石化协议信息数据 */
  MAP_PEC_INJECTION_UP_ALARM           = 0xFE,   ///< 上限报警状态
  MAP_PEC_INJECTION_DOWN_ALARM         = 0xFF,   ///< 下限报警状态
  MAP_PEC_RUN_STATE                    = 0x100,  ///< 运行状态
  MAP_PEC_RUN_TIME_H                   = 0x101,  ///< 设备运行时间（高）
  MAP_PEC_RUN_TIME_L                   = 0x102,  ///< 设备运行时间（低）
  MAP_PEC_INJECTION_LESS               = 0x103,  ///< 欠注量
  MAP_PEC_INJ_PRESSURE                 = 0x104,  ///< 注水压力
  MAP_PEC_OPEN_ANGLE                   = 0x105,  ///< 阀门开度
  MAP_PEC_INSTANT_FLOW                 = 0x106,  ///< 注水流量瞬时量
  MAP_PEC_CUMULA_FLOW_H                = 0x107,  ///< 注水流量累计量（高）
  MAP_PEC_CUMULA_FLOW_L                = 0x108,  ///< 注水流量累计量（低）
  /** 自定义映射表 */
  MAP_BATTERY_VOLTAGE                  = 300,    ///< 40001 电池电压
  MAP_AUTO_ADJ_NO                      = 301,    ///< 40002 自动调节次数
  MAP_RTU_COM_STATE                    = 302,    ///< 40003 RTU通信状态
  MAP_FLOW_METER_COM_STATE             = 303,    ///< 40004 流量计通信状态
  MAP_HOUR                             = 304,    ///< 40005 时
  MAP_MINUTE                           = 305,    ///< 40006 分
  MAP_SECOND                           = 306,    ///< 40007 秒
  MAP_ANGLE                            = 307,    ///< 40008 角度
  MAP_INSTANT_FLOW_H                   = 308,    ///< 40009 瞬时流速H
  MAP_INSTANT_FLOW_L                   = 309,    ///< 40010 瞬时流速L
  MAP_DAILY_INJECTION_H                = 310,    ///< 40011 日配注H
  MAP_DAILY_INJECTION_L                = 311,    ///< 40012 日配注L
  MAP_ACTUATOR_SLAVE                   = 312,    ///< 40013 执行器地址
  MAP_LCD_LIGHT_TIME                   = 313,    ///< 40014 背光时间
  MAP_OPENING_VALUE                    = 314,    ///< 40015 开度值
  MAP_PRECISION                        = 315,    ///< 40016 调节精度
  MAP_COM_QUALITY                      = 316,    ///< 40017 通信质量
  MAP_CUMULA_FLOW_H                    = 317,    ///< 40018 累计注入量(Float Inverse)H
  MAP_CUMULA_FLOW_L                    = 318,    ///< 40019 累计注入量(Float Inverse)L
  MAP_FLOW_METER_TYPE                  = 319,    ///< 40020 流量计类型
  MAP_FLOW_METER_VER                   = 320,    ///< 40021 流量计版本
  MAP_FLOW_METER_SLAVE                 = 321,    ///< 40022 流量计从机号
  MAP_FLOW_METER_UART_BAUD_H           = 322,    ///< 40023 流量计串口波特率H
  MAP_FLOW_METER_UART_BAUD_L           = 323,    ///< 40024 流量计串口波特率L
  MAP_FLOW_METER_UART_BYTE             = 324,    ///< 40025 流量计串口数据位
  MAP_FLOW_METER_UART_PARITY           = 325,    ///< 40026 流量计串口校验位
  MAP_FLOW_METER_UART_STOP_BITS        = 326,    ///< 40027 流量计串口停止位
  MAP_RTU_SLAVE                        = 327,    ///< 40028 RTU从机地址
  MAP_RTU_UART_BAUD_H                  = 328,    ///< 40029 RTU串口波特率H
  MAP_RTU_UART_BAUD_L                  = 329,    ///< 40030 RTU串口波特率L
  MAP_RTU_UART_BYTE                    = 330,    ///< 40031 RTU串口数据位
  MAP_RTU_UART_PARITY                  = 331,    ///< 40032 RTU串口校验位
  MAP_RTU_UART_STOP_BITS               = 332,    ///< 40033 RTU串口停止位
  MAP_AUTO_ADJ                         = 333,    ///< 40034 自动调节标志位
  MAP_SINGLE_ADJ_NO                    = 334,    ///< 40035 单向连续调节次数
  MAP_SCREEN_FLIP_TIME                 = 335,    ///< 40036 屏幕翻滚时间
  MAP_USR_NORMAL_PWD                   = 336,    ///< 40037 普通用户密码
  MAP_USR_TEST_PWD                     = 337,    ///< 40038 测试用户密码
  MAP_USR_SUP_PWD                      = 338,    ///< 40039 超级用户密码
  MAP_SAVE_CONF                        = 339,    ///< 40040 写入参数
  MAP_REBOOT                           = 340,    ///< 40041 重启
  MAP_MOTOR_REMOTE_CTL                 = 400,    ///< 40101 电机遥控
  MAP_MOTOR_ADJ                        = 401,    ///< 40102 电机角度校准
  MAP_MOTOR_TEST                       = 402,    ///< 40103 电机测试
  MAP_MOTOR_RATIO                      = 403,    ///< 40104 电机运行占空比
  MAP_MOTOR_TIME                       = 404,    ///< 40105 电机运行时间
  MAP_LPM_SWITCH                       = 405,    ///< 40106 电机测试
  MAP_MOTOR_REVERSE                    = 406,    ///< 40107 反向
  MAP_FLOW_POWER                       = 407,    ///< 40108 流量计电源
  MAP_FLOW_REQ_TIME0                   = 408,    ///< 40109 无调节请求间隔
  MAP_FLOW_REQ_TIME1                   = 409,    ///< 40110 有调节请求间隔
  MAP_FLOW_MOTOR_WEIGHT_EDGE           = 410,    ///< 40111 流量计调节权值
  MAP_PRECISION_RATIO                  = 411,    ///< 40112 流量计精度权值
  MAP_H_VERSION                        = 412,    ///< 40113 硬件版本
  MAP_S_VERSION                        = 413,    ///< 40114 软件版本
  MAP_FLOW_COMPRO                      = 414,    ///< 40115 流量计通信协议
  MAP_FLOW_COMMODE                     = 415,    ///< 40116 流量计通信方式
  MAP_RTU_COMPRO                       = 416,    ///< 40117 RTU通信协议
  MAP_RTU_COMMODE                      = 417,    ///< 40118 RTU通信方式
  MAP_UPGRADE_ADDR                     = 1000,   ///< 41001 升级
};

extern u8 read_registers(u8 slave, u16 addr, u16 len, u8 *out_buf);
extern u8 write_registers(u8 slave, u16 addr, u16 len, u8 *in_buf);

#endif