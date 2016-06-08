/**
 * @file  motor.h
 * @brief 电机驱动描述
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#ifndef MOTOR_H_
#define MOTOR_H_
#include "../kernel/kernel.h"
#include "adc.h"
#define ANGLE_INT_SEL              P1SEL  ///< 限位开关寄存器
#define ANGLE_INT_DIR              P1DIR
#define ANGLE_INT_REN              P1REN
#define ANGLE_INT_OUT              P1OUT
#define ANGLE_INT_IN               P1IN
#define ANGLE_INT_IES              P1IES
#define ANGLE_INT_IE               P1IE
#define ANGLE_INT_IFG              P1IFG
#define ANGLE_INT_BIT              BIT7

#define ANGLE_DIR_SEL              P1SEL  ///< 电机转动方向控制寄存器
#define ANGLE_DIR_DIR              P1DIR
#define ANGLE_DIR_REN              P1REN
#define ANGLE_DIR_OUT              P1OUT
#define ANGLE_DIR_BIT              BIT7

#define ANGLE_PWR_SWITCH_SEL       P6SEL ///< 限位开关电源控制寄存器
#define ANGLE_PWR_SWITCH_DIR       P6DIR
#define ANGLE_PWR_SWITCH_REN       P6REN
#define ANGLE_PWR_SWITCH_OUT       P6OUT
#define ANGLE_PWR_SWITCH_BIT       BIT4
#define ANGLE_PWR_SWITCH_ON()      ANGLE_PWR_SWITCH_OUT |= ANGLE_PWR_SWITCH_BIT   //开启
#define ANGLE_PWR_SWITCH_OFF()     ANGLE_PWR_SWITCH_OUT &= ~ANGLE_PWR_SWITCH_BIT

#define BTN_IN1_SEL                P2SEL  ///< 电机波形输出寄存器
#define BTN_IN1_DIR                P2DIR
#define BTN_IN1_REN                P2REN
#define BTN_IN1_OUT                P2OUT
#define BTN_IN1_BIT                BIT3

#define BTN_IN2_SEL                P2SEL  ///< 电机波形输出寄存器
#define BTN_IN2_DIR                P2DIR
#define BTN_IN2_REN                P2REN
#define BTN_IN2_OUT                P2OUT
#define BTN_IN2_BIT                BIT4

#define BTN_INH_SEL                P2SEL  ///< 电机电源寄存器
#define BTN_INH_DIR                P2DIR
#define BTN_INH_REN                P2REN
#define BTN_INH_OUT                P2OUT
#define BTN_INH_BIT                BIT5

#define MOTOR_EDGE_MID             (ANGLE_INT_IN & ANGLE_INT_BIT) ///< 未触发限位开关

/**
 * @brief 电机属性结构
 */
typedef struct Motor {
  u32 run_duration;  ///< 电机运行时间
  u64 time_a;        ///< 计时器
  u64 stop_time;     ///< 电机停止运行时间
  u8 duty_ratio;     ///< 占空比
  u8 duty_ratio_tmp; ///< 临时值
  u8 dir;            ///< 转动方向
  u8 switch_dir;     ///< 限位开关位置
  u8 status;         ///< 电机运行状态
  u8 edge_switch;    ///< 限位开关状态
  u8 mode;           ///< 电机运行模式
  u8 adj_count_left; ///< 左限位碰撞次数
  u8 adj_count_right;///< 右限位碰撞次数
  u16 ad_tmp;        ///< ad值
  u8 reverse;        ///< 反向
}Motor;

enum {
  MOTOR_DIR_SHUT,
  MOTOR_DIR_OPEN,
};

enum {
  MOTOR_STOP,
  MOTOR_RUN,
};

enum {
  MOTOR_ANGLE_MAN,
  MOTOR_ANGLE_MAN_BACK,
  MOTOR_ANGLE_POWER,
  MOTOR_ANGLE_POWER_BACK,
  MOTOR_ANGLE_ADJ,
  MOTOR_ANGLE_ADJ_BACK,
  MOTOR_TEST,
  MOTOR_TEST_BACK,
};

enum {
  EDGE_SWITCH_UP,
  EDGE_SWITCH_DOWN,
};

#define MOTOR_ANGLE_AD_MIN_OFFSET   30
#define MOTOR_ANGLE_AD_MAX_OFFSET   10
#define MOTOR_ADJ_COUNT_MAX         100
#define MOTOR_ADJ_COUNT_MIN         2
#define MOTOR_ADJ_COUNT_DEF         2
#define MOTOR_RUN_DURATION_TIME_MIN 300
#define MOTOR_RUN_DURATION_TIME_MAX 300000                       ///< 电机默认运行时间2s
#define MOTOR_WAVE_TICK_TIME        TASK_TIMER_CLOCK/10000       ///< PWM时间,忘记什么意思了,一会再看
#define MOTOR_WAVE_DUTY_RATIO       (100 * MOTOR_WAVE_TICK_TIME-1)  ///< 电机默认占空比50%
#define MOTOR_WAVE_CYCLE_TIME       (100 * MOTOR_WAVE_TICK_TIME-1) ///< PWM周期
#define MOTOR_WAVE_DUTY_RATIO_MIN   20   ///< 10%
#define MOTOR_WAVE_DUTY_RATIO_MAX   80   ///< 80%
#define MOTOR_WAVE_DUTY_RATIO_DEF   60   ///< 50%
#define MOTOR_STOP_DELAY            200  ///< 200ms
#define MOTOR_ADJ_STOP_DELAY        3000 ///< 3000ms
#define MOTOR_ADJ_DELAY             1000 ///< 100ms

extern Motor motor;
extern void motor_init();
extern u8 motor_ctl(u8 run, u8 dir, u8 ratio, u32 duration, u8 mode);
extern void motor_edge_ctl();
extern void motor_timer_start();
extern void motor_stop();

#endif