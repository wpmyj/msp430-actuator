/**
 * @file  button.h
 * @brief 按键驱动描述
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#ifndef BUTTON_H_
#define BUTTON_H_
#include "../kernel/kernel.h"
#if 1
#define BUTTON_SEL          P2SEL   ///< 按键管脚定义
#define BUTTON_DIR          P2DIR
#define BUTTON_OUT          P2OUT
#define BUTTON_REN          P2REN
#define BUTTON_IN           P2IN
#define BUTTON_IES          P2IES
#define BUTTON_IE           P2IE
#define BUTTON_IFG          P2IFG
#define BUTTON_MAIN_BIT     BIT2
#define BUTTON_LEFT_BIT     BIT1
#define BUTTON_RIGHT_BIT    BIT0
#else
#define BUTTON_SEL          P4SEL
#define BUTTON_DIR          P4DIR
#define BUTTON_OUT          P4OUT
#define BUTTON_REN          P4REN
#define BUTTON_IN           P4IN
#define BUTTON_MAIN_BIT     BIT7
#define BUTTON_LEFT_BIT     BIT7
#define BUTTON_RIGHT_BIT    BIT7
#endif

/**
 * @brief 按键结构
 */
typedef struct Button {
  u8 state;    ///< 按键状态
  u8 bit;      ///< 按键索引
  u64 time_a;  ///< 计时器
}Button;

/**
 * @brief 红外按键结构
 */
typedef struct IRButton {
  u8 state;   ///< 按键状态
  u8 bit;     ///< 按键索引
  u64 time_a; ///< 计时器
}IRButton;

#define BUTTON_PRESS_TIME            10   //短按超时ms
#define BUTTON_LONG_PRESS_TIME       800  //长按超时ms

/**
 * @brief 按钮（按键）状态
 */
enum {
  STATE_BUTTONN_UP,         ///< 按键抬起
  STATE_BUTTONN_DOWN,       ///< 按下
  STATE_BUTTONN_PRESS,      ///< 短按
  STATE_BUTTONN_LONG_PRESS, ///< 长按
};

/**
 * @brief 按钮定义
 */
enum {
  BUTTON_MAIN,    ///< 主键
  BUTTON_LEFT,    ///< 左键
  BUTTON_RIGHT,   ///< 右键
  BUTTON_MAX_NUM, ///< 按键数量
};

/**
 * @brief 按键初始化
 * @param[in] NO 按键号
 * @param[in] BIT 按键索引
 * @return 无返回值
 */
#define BUTTON_CREATE(NO, BIT)\
  button_list[NO].state = STATE_BUTTONN_UP;\
  button_list[NO].bit = BIT;

extern void button_init();
#endif