/**
 * @file  lcd_task.h
 * @brief lcd任务描述
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#ifndef LCD_TASK_H_
#define LCD_TASK_H_
#include "../kernel/kernel.h"
#include "../drivers/drivers.h"
#include "lcd_font.h"
/**
 * @brief 当前显示屏(焦点)状态
 */
enum {
  ITEM_HOVER,         ///< 悬停状态(初始状态)
  ITEM_FOCUS_ALL,     ///< 全焦点状态
  ITEM_FOCUS_FIRST,   ///< 第一焦点
  ITEM_FOCUS_SECOND,  ///< 第二焦点
  ITEM_FOCUS_THIRD,   ///< 第三焦点
  ITEM_FOCUS_FOURTH,  ///< 第四焦点
  ITEM_FOCUS_FIFTH,   ///< 第五焦点
  ITEM_FOCUS_SIXTH,   ///< 第六焦点
};

/**
 * @brief 选项卡描述(设置)
 */
enum {
  //ITEM_PASSWD,                     ///< 密码项
  ITEM_ADDR,                       ///< 地址项 //////////第一页
  ITEM_BACK_LIGHT,                 ///< 背光项
  //ITEM_OPEN_ANGLE,                 ///< 开度项
  ITEM_PRECISION,                  ///< 精度项
  ITEM_INJECTION,                  ///< 配注项
  //ITEM_FLIP_PAGE,                ///< 翻页项
  ITEM_H_VERSION,                  ///< 硬件版本项
  ITEM_ACTUATOR_VERSION,           ///< 软件版本项
  ITEM_SAVE_CONF,                  ///< 参数保存
  ITEM_REBOOT,                     ///< 重启
  ITEM_FLOW_UART,                  ///< 流量计串口项 ////第二页
  ITEM_RTU_UART,                   ///< RTU串口项
  ITEM_FLOW_TYPE,                  ///< 流量计类型
  ITEM_FLOW_ADDR,                  ///< 流量计地址
  ITEM_MOTOR_REMOTE_CTL,           ///< 电机遥控    /////第三页
  ITEM_MOTOR_MANUAL_ADJ,           ///< 自动/手动
  ITEM_MOTOR_ADJ,                  ///< 电机角度校准/////第四页
  ITEM_MOTOR_TEST,                 ///< 电机测试
  ITEM_MOTOR_ANGLE_AD_MIN,         ///< 角度电阻AD最小值
  ITEM_MOTOR_ANGLE_AD_MID,         ///< 角度电阻AD中间值
  ITEM_MOTOR_ANGLE_AD_MAX,         ///< 角度电阻AD最大值
  ITEM_MOTOR_RATIO,                ///< 电机运行占空比
  ITEM_MOTOR_TIME,                 ///< 电机运行时间
  ITEM_LPM_SWITCH,                 ///< 低功耗
  ITEM_MOTOR_REVERSE,              ///< 反向
  ITEM_PRECISION_RATIO,            ///< 流量计精度权值
  ITEM_FLOW_MOTOR_WEIGHT_EDGE,     ///< 权值
  ITEM_FLOW_REQ_TIME0,             ///< 无调节请求间隔///第五页
  ITEM_FLOW_REQ_TIME1,             ///< 有调节请求间隔
  //ITEM_SUN_RISE_HOUR,
  ITEM_MAX_NUM,                    ///< 选项卡数量
};

/**
 * @brief 页面描述
 */
enum {
  PAGE_MAIN,    ///< 主页面
  PAGE_SUB0,    ///< 副0页面
  PAGE_SUB1,    ///< 副1页面
  PAGE_LOGIN,   ///< 登陆页面
  PAGE_SETTING, ///< 设置页面
  PAGE_MAX_NUM, ///< 页面数量
};

/**
 * @brief lcd页结构
 */
typedef struct LcdPage {
  u8 page;            ///< 当前页
  u8 hover_item;      ///< 当悬停项
  u8 status;          ///< lcd当前焦点状态
  u64 frame_time_a;   ///< 刷屏计时器
  u64 frame_time_b;   ///< 内容更新计时器
  u16 frame_itv_time; ///< 刷屏间隔
  u8 flip_page_time;  ///< 自动翻屏间隔
  u8 x;               ///< 横向偏移
  u8 y;               ///< 垂直偏移
  u8 light_time;      ///< 屏幕亮时间
  u64 light_time_a;   ///< 计时器
  u8 max_item;
}LcdPage;
extern LcdPage lcd_page;

#define LCD_FRAME_INT_TIME     100   ///< 屏幕刷新频率100ms
#define LCD_PAGE_OFFSET_X      0     ///< 横向偏移默认值
#define LCD_PAGE_OFFSET_Y      0     ///< 垂直偏移默认值

extern void lcd_task();

#endif