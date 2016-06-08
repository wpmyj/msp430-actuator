/**
 * @file  task.h
 * @brief 任务模块相关说明
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#ifndef TASK_H_
#define TASK_H_
#include "cpu.h"
#include "stdint.h"
#include "watchdog.h"

enum {
  TASK_STOP, ///< 任务运行
  TASK_RUN,  ///< 任务停止
};

/** 32768 */
#define TASK_TIMER_CLOCK              CRY_FREQ
/** 正常模式5ms */
#define TASK_TIMER_CYCLE_TIME         ((TASK_TIMER_CLOCK*5/(1000)) -1) ///<任务时间片5ms
/** 休眠模式模式1s */
#define TASK_TIMER_EMPTY_CYCLE_TIME   ((TASK_TIMER_CLOCK) -1)

#define TIME_ZONE                     8 //东八区

/**
 * @brief 任务结构体
 */
typedef struct Task {
  u8 run;                    ///< 程序运行标记：0-不运行，1运行
  u16 timer;                 ///< 计时器
  u16 itv_time;              ///< 任务运行间隔时间
  void (*task_hook)(void);   ///< 要运行的任务函数
  u8 evt_on;                 ///< 事件开启标志
  u16 evt;                   ///< 事件类型
  u64 evt_time_a;            ///< 事件初始时间
  u64 evt_time_b;            ///< 事件延时
} Task;

typedef struct TaskTimer {
  u8 status;
  u64 time_a;                   ///< 计时器
  u16 itv_time;                 ///< 任务运行间隔时间
  u64 tick;                     ///< 任务时钟滴答
  u64 sys_tick;                 ///< 系统时间
  void (*task_sleep)(void);    ///< 
  void (*task_wakeup)(void);   ///<
}TaskTimer;

extern TaskTimer task_timer;

/**
 * @brief 任务类型描述,若要添加新的任务类型需要在下面添加描述
 */
enum {
  TASK_RTU,                  ///< rtu任务
  TASK_FLOW_METER,           ///< 流量计任务
  TASK_LCD,                  ///< LCD任务
  TASK_BUTTON,               ///< 按键任务
  TASK_LED,                  ///< LED任务
  TASK_ADC,                  ///< AD采集任务
  TASK_FLASH,                ///< FLASH任务
  TASK_WATCH_DOG,            ///< 看门狗任务
  TASK_MOTOR,                ///< 电机任务
  TASK_MAX_NUM,
};

/**
 * @brief 事件类型描述,若要添加新的事件类型需要在下面添加描述
 */
enum {
  EV_NULL,                    ///< 空事件
  EV_READY,                   ///< 初始化完成
  EV_FRAME_RECEIVED,          ///< 帧接收完成
  EV_EXECUTE,                 ///< 执行事件
  EV_FRAME_SENT,              ///< 发送数据
  EV_TIMEOUT,                 ///< 超时事件
  EV_BUTTON_MAIN_PRESS,       ///< 主键短按
  EV_BUTTON_LEFT_PRESS,       ///< 左键短按
  EV_BUTTON_RIGHT_PRESS,      ///< 右键短按
  EV_BUTTON_MAIN_LONG_PRESS,  ///< 主键长按
  EV_BUTTON_LEFT_LONG_PRESS,  ///< 左键长按
  EV_BUTTON_RIGHT_LONG_PRESS, ///< 右键长按
  EV_APPLY_CONFIG,            ///< 应用配置
  EV_SAVE_CONFIG,             ///< 保存配置
  EV_MOTOR_START,             ///< 电机开启
  EV_MOTOR_RUN,               ///< 电机运行
  EV_MOTOR_ANGLE_AD_MIN,      ///< 电机角度AD最小值
  EV_MOTOR_ANGLE_AD_MAX,      ///< 电机角度AD最小值
  EV_MOTOR_STOP,              ///< 关闭电机
  EV_ADC_ANGLE_RUN,           ///< 开启角度变化
  EV_ADC_ANGLE_STOP,          ///< 关闭角度变化
  EV_UPGRADE,                 ///< 软件升级
};

enum {
  TIMER_STATE_TASK,
  TIMER_STATE_TASK_HANDLER,
  TIMER_STATE_EMPTY,
  TIMER_STATE_EMPTY_HANDLER,
};

enum {
  TIME_TIME,
  TIME_DAY,
  TIME_HOUR,
  TIME_MINUTE,
  TIME_SECOND,
};

extern Task task_list[];     ///< 任务列表

//#define TASK_EVENT_POST_DELAY(TASK, EVENT, DELAY)\
//  LOW_POWER_MODE_OFF();\
//  task_event_post_delay(TASK, EVENT, DELAY);

/**
 * @brief 任务创建(任务信息初始化)
 * @param[in] TASK 任务描述
 * @param[in] ITV 时间片大小
 * @param[in] TASK_HOOK 任务函数
 * @return 无返回值
 */
#define TASK_CREATE(TASK, ITV, TASK_HOOK)\
  task_list[TASK].timer = ITV;\
  task_list[TASK].itv_time = ITV;\
  task_list[TASK].task_hook = TASK_HOOK;\
  task_event_post(TASK, EV_READY, 0);\
  task_list[TASK].run = TASK_STOP;
  
#if 1
#define LOW_POWER_MODE_ON()\
  if(task_timer.status < TIMER_STATE_EMPTY) {\
    task_timer.status = TIMER_STATE_EMPTY;\
    task_timer.task_sleep();\
    WATCH_DOG_DISABLE();\
    LPM3;\
  }

#define LOW_POWER_MODE_OFF()\
  if(task_timer.status > TIMER_STATE_TASK_HANDLER) {\
    task_timer.status = TIMER_STATE_TASK;\
    task_timer.task_wakeup();\
    WATCH_DOG_ENABLE();\
    LPM3_EXIT;\
  }
#else
#define LOW_POWER_MODE_ON()
#define LOW_POWER_MODE_OFF()
#endif

extern void task_scheduling(void);
extern void task_timer_init(void);
extern u8 task_event_post(u8 task, u16 event, u64 delay);
extern u8 task_event_get(u8 task, u16 *event);
extern void task_setsystime(u64 tm, u8 opt);

#endif