/**
 * @file  motor.c
 * @brief 电机驱动代码(PWM)
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#include "motor.h"
#include "../app/actuator.h"

/**
 * @brief 获取角度AD值
 * @return 无返回值
 */
#define MOTOR_ANGLE_AD_GET()\
    motor.ad_tmp = ad_channels[AD_CHANNEL_ANGLE_R];
  
/**
 * @brief 设置电机转动方向为开方向
 * @return 无返回值
 */
#define MOTOR_BTN_DIR_OPEN()\
  if(TRUE == motor.reverse){\
    BTN_IN2_OUT &= ~BTN_IN2_BIT;\
    BTN_IN1_OUT |= BTN_IN1_BIT;\
    TA1CCTL2 = OUTMOD_6;\
  } else {\
    BTN_IN2_OUT |= BTN_IN2_BIT;\
    BTN_IN1_OUT &= ~BTN_IN1_BIT;\
    TA1CCTL2 = OUTMOD_3;\
  }

/**
 * @brief 设置电机转动方向为关方向
 * @return 无返回值
 */
#define MOTOR_BTN_DIR_SHUT()\
  if(TRUE == motor.reverse){\
    BTN_IN2_OUT |= BTN_IN2_BIT;\
    BTN_IN1_OUT &= ~BTN_IN1_BIT;\
    TA1CCTL2 = OUTMOD_3;\
  } else {\
    BTN_IN2_OUT &= ~BTN_IN2_BIT;\
    BTN_IN1_OUT |= BTN_IN1_BIT;\
    TA1CCTL2 = OUTMOD_6;\
  }

/**
 * @brief 打开电机电源
 * @return 无返回值
 */
#define MOTOR_POW_ON()\
  BTN_IN1_SEL |= BTN_IN1_BIT;\
  BTN_INH_OUT |= BTN_INH_BIT;

/**
 * @brief 关闭电机电源
 * @return 无返回值
 */
#define MOTOR_POW_OFF()\
  BTN_INH_OUT &= ~BTN_INH_BIT;\
  BTN_IN1_SEL &= ~BTN_IN1_BIT;\
  BTN_IN1_OUT &= ~BTN_IN1_BIT;\
  BTN_IN2_OUT &= ~BTN_IN2_BIT;

/**
 * @brief 占空比增长至设定值
 * @return 无返回值
 */
#define MOTOR_DUTY_RATIO_UP()\
  if(motor.duty_ratio_tmp < motor.duty_ratio) {\
    ++motor.duty_ratio_tmp;\
    TA1CCR2 = (u16)(motor.duty_ratio_tmp * MOTOR_WAVE_TICK_TIME-1);\
  }
  
Motor motor;  ///< 电机属性

/**
 * @brief 电机驱动初始化
 * @return 无返回值
 */
void motor_init() {
  ANGLE_PWR_SWITCH_SEL &= ~ANGLE_PWR_SWITCH_BIT;
  ANGLE_PWR_SWITCH_DIR |= ANGLE_PWR_SWITCH_BIT;
  //ANGLE_PWR_SWITCH_REN |= ANGLE_PWR_SWITCH_BIT;
  //ANGLE_PWR_SWITCH_OUT &= ~ANGLE_PWR_SWITCH_BIT; //关闭
  ANGLE_PWR_SWITCH_ON();    //开启
  
  ANGLE_INT_SEL &= ~ANGLE_INT_BIT; 
  ANGLE_INT_DIR &= ~ANGLE_INT_BIT; // 输入
  ANGLE_INT_REN |= ANGLE_INT_BIT;
  ANGLE_INT_OUT |= ANGLE_INT_BIT;  // 拉高
  ANGLE_INT_IES |= ANGLE_INT_BIT;
  ANGLE_INT_IFG &= ~ANGLE_INT_BIT;
  ANGLE_INT_IE |= ANGLE_INT_BIT;
  
  BTN_IN1_SEL &= ~BTN_IN1_BIT;
  BTN_IN1_DIR |= BTN_IN1_BIT;
  //BTN_IN1_REN |= BTN_IN1_BIT;
  //BTN_IN1_OUT |= BTN_IN1_BIT;
  
  BTN_IN2_SEL &= ~BTN_IN2_BIT;
  BTN_IN2_DIR |= BTN_IN2_BIT;
  //BTN_IN2_REN |= BTN_IN2_BIT;
  //BTN_IN2_OUT |= BTN_IN2_BIT;
  
  BTN_INH_SEL &= ~BTN_INH_BIT;
  BTN_INH_DIR |= BTN_INH_BIT;
  //BTN_INH_REN |= BTN_INH_BIT;
  //BTN_INH_OUT &= ~BTN_INH_BIT;
  //motor.switch_dir = MOTOR_ROTATION_DIR_NULL;
  //motor_run(MOTOR_ROTATION_DIR_FORWARD);
  MOTOR_POW_OFF();
  motor.edge_switch = EDGE_SWITCH_UP;
}

/**
 * @brief 关闭电机定时器
 * @return 无返回值
 */
void motor_timer_stop() {
  TA1CCTL0 &= ~CCIE;
}

/**
 * @brief 停止电机
 * @return 无返回值
 */
void motor_stop() {
  motor_timer_stop();
  MOTOR_POW_OFF();
  task_event_post(TASK_ADC, EV_ADC_ANGLE_STOP, 1000);
}

void motor_timer_before_start() {
  switch(motor.mode) {
    case MOTOR_ANGLE_POWER:
      if(MOTOR_DIR_OPEN == motor.dir) {
        MOTOR_BTN_DIR_OPEN();
      } else {
        MOTOR_BTN_DIR_SHUT();
      }
      MOTOR_POW_ON();
      break;
    case MOTOR_ANGLE_POWER_BACK:
      MOTOR_ANGLE_AD_GET();
      if(motor.ad_tmp < actuator.angle_ad_mid) {
        MOTOR_BTN_DIR_SHUT();
      } else if(motor.ad_tmp > actuator.angle_ad_mid) {
        MOTOR_BTN_DIR_OPEN();
      } else {
        motor_stop();
        return;
      }
      MOTOR_POW_ON();
      break;
    case MOTOR_ANGLE_ADJ:
      if(MOTOR_DIR_OPEN == motor.dir) {
        MOTOR_BTN_DIR_OPEN();
      } else {
        MOTOR_BTN_DIR_SHUT();
      }
      MOTOR_POW_ON();
      break;
    case MOTOR_ANGLE_ADJ_BACK:
      MOTOR_ANGLE_AD_GET();
      if(motor.ad_tmp < actuator.angle_ad_mid) {
        MOTOR_BTN_DIR_SHUT();
      } else if(motor.ad_tmp > actuator.angle_ad_mid) {
        MOTOR_BTN_DIR_OPEN();
      } else {
        motor_stop();
        return;
      }
      MOTOR_POW_ON();
      break;
    case MOTOR_TEST:
      if(MOTOR_DIR_OPEN == motor.dir) {
        MOTOR_BTN_DIR_OPEN();
      } else {
        MOTOR_BTN_DIR_SHUT();
      }
      MOTOR_POW_ON();
      break;
    case MOTOR_TEST_BACK:
      MOTOR_ANGLE_AD_GET();
      if(motor.ad_tmp < actuator.angle_ad_mid) {
        MOTOR_BTN_DIR_SHUT();
      } else if(motor.ad_tmp > actuator.angle_ad_mid) {
        MOTOR_BTN_DIR_OPEN();
      } else {
        motor_stop();
        return;
      }
      MOTOR_POW_ON();
      break;
  }
}

/**
 * @brief 开启电机定时器
 * @return 无返回值
 */
void motor_timer_start() {
  motor_timer_before_start();
  motor.time_a = task_timer.tick;
  motor.duty_ratio_tmp = MOTOR_WAVE_DUTY_RATIO_MIN;
  TA1CCR0 = MOTOR_WAVE_CYCLE_TIME;
  TA1CCR2 = (u16)(motor.duty_ratio_tmp * MOTOR_WAVE_TICK_TIME-1);//(u16)(motor.duty_ratio * MOTOR_WAVE_TICK_TIME-1);
  TA1CTL = MC__UP | TASSEL__ACLK | TACLR;
  TA1CCTL0 |= CCIE;
}

u8 motor_ctl(u8 run, u8 dir, u8 ratio, u32 duration, u8 mode) {
  switch(mode) {
    case MOTOR_ANGLE_POWER:
      if(!MOTOR_EDGE_MID) {
        return FALSE;
      }
      if(FALSE == run) {
        motor_stop();
        motor.status = MOTOR_STOP;
        return TRUE;
      }
      if(dir != motor.dir) {
        motor_stop();
      }
      MOTOR_ANGLE_AD_GET();
      if(MOTOR_DIR_OPEN == dir) {
        if(motor.ad_tmp <= actuator.angle_ad_min + MOTOR_ANGLE_AD_MIN_OFFSET) {
          return FALSE;
        }
      } else {
        if(motor.ad_tmp >= actuator.angle_ad_max - MOTOR_ANGLE_AD_MAX_OFFSET) {
          return FALSE;
        }
      }
      duration = duration<MOTOR_RUN_DURATION_TIME_MIN?MOTOR_RUN_DURATION_TIME_MIN:duration;
      duration = duration>MOTOR_RUN_DURATION_TIME_MAX?MOTOR_RUN_DURATION_TIME_MAX:duration;
      break;
    case MOTOR_ANGLE_ADJ://break;
      //motor.adj_count = 0;
      motor.adj_count_left = 0;
      motor.adj_count_right = 0;
    case MOTOR_TEST:
      if(FALSE == run) {
        motor.mode = MOTOR_ANGLE_MAN;
        motor_stop();
        motor.status = MOTOR_STOP;
        return TRUE;
      }
//      if(mode == motor.mode) {
//        return FALSE;
//      }
      MOTOR_ANGLE_AD_GET();
      if(motor.ad_tmp < actuator.angle_ad_mid) { //shut
        dir = MOTOR_DIR_SHUT;
      } else if(motor.ad_tmp > actuator.angle_ad_mid) { //open
        dir = MOTOR_DIR_OPEN;
      }
      duration = duration<MOTOR_ADJ_COUNT_MIN?MOTOR_ADJ_COUNT_MIN:duration;
      duration = duration>MOTOR_ADJ_COUNT_MAX?MOTOR_ADJ_COUNT_MAX:duration;
      break;
  }
  
  motor.mode = mode;
  //motor.status = MOTOR_START;
  ratio = ratio<MOTOR_WAVE_DUTY_RATIO_MIN?MOTOR_WAVE_DUTY_RATIO_MIN:ratio;
  ratio = ratio>MOTOR_WAVE_DUTY_RATIO_MAX?MOTOR_WAVE_DUTY_RATIO_MAX:ratio;
  motor.duty_ratio = ratio;
  motor.run_duration = duration;
  motor.dir = dir;
  task_event_post(TASK_MOTOR, EV_MOTOR_RUN, MOTOR_STOP_DELAY);
  return TRUE;
}

void motor_ctl_handler() {
  switch(motor.mode) {
    case MOTOR_ANGLE_POWER:
      MOTOR_ANGLE_AD_GET();
      if(MOTOR_DIR_OPEN == motor.dir) {
        if(motor.ad_tmp <= actuator.angle_ad_min + MOTOR_ANGLE_AD_MIN_OFFSET) {
          motor_stop();
          motor.status = MOTOR_STOP;
        }
      } else {
        if(motor.ad_tmp >= actuator.angle_ad_max - MOTOR_ANGLE_AD_MAX_OFFSET) {
          motor_stop();
          motor.status = MOTOR_STOP;
        }
      }
      if(task_timer.tick - motor.time_a >= motor.run_duration) {
        motor_stop();
        motor.status = MOTOR_STOP;
      }
      break;
    case MOTOR_ANGLE_POWER_BACK:
      if(MOTOR_EDGE_MID) {
        MOTOR_ANGLE_AD_GET();
        if(MOTOR_DIR_OPEN == motor.dir) {
          if(motor.ad_tmp >= actuator.angle_ad_min + MOTOR_ANGLE_AD_MIN_OFFSET) {
            motor_stop();
            motor.status = MOTOR_STOP;
          }
        } else {
          if(motor.ad_tmp <= actuator.angle_ad_max - MOTOR_ANGLE_AD_MAX_OFFSET) {
            motor_stop();
            motor.status = MOTOR_STOP;
          }
        }
      }
      break;
    case MOTOR_ANGLE_ADJ:
      //MOTOR_ANGLE_AD_GET();
      //if(motor.run_duration <= motor.adj_count_left) {
        //motor.mode = MOTOR_ANGLE_MAN;
        //motor.status = MOTOR_STOP;
        //motor_stop();
        //task_event_post(TASK_MOTOR, EV_MOTOR_STOP, MOTOR_ADJ_STOP_DELAY);
      //}
      break;
    case MOTOR_ANGLE_ADJ_BACK:
      if(MOTOR_EDGE_MID) {
        //++motor.adj_count;
        motor.mode = MOTOR_ANGLE_ADJ;
        MOTOR_ANGLE_AD_GET();
        if(motor.ad_tmp < actuator.angle_ad_mid) {
          //actuator.angle_ad_min = motor.ad_tmp;
          task_event_post(TASK_MOTOR, EV_MOTOR_ANGLE_AD_MIN, 1000);
        } else if(motor.ad_tmp > actuator.angle_ad_mid) {
          //actuator.angle_ad_max = motor.ad_tmp;
          task_event_post(TASK_MOTOR, EV_MOTOR_ANGLE_AD_MAX, 1000);
        }
      }
      break;
    case MOTOR_TEST:break;
    case MOTOR_TEST_BACK:
      if(MOTOR_EDGE_MID) {
        motor.mode = MOTOR_TEST;
      }
      break;
  }
  MOTOR_DUTY_RATIO_UP();
  motor.stop_time = task_timer.tick;
}

void motor_edge_ctl() {
  ANGLE_INT_IFG &= ~ANGLE_INT_BIT;
  if(!MOTOR_EDGE_MID) {
    switch(motor.mode) {
      case MOTOR_ANGLE_MAN:break;
      case MOTOR_ANGLE_MAN_BACK:break;
      case MOTOR_ANGLE_POWER:
        motor.mode = MOTOR_ANGLE_POWER_BACK;
        motor_stop();
        task_event_post(TASK_MOTOR, EV_MOTOR_RUN, MOTOR_STOP_DELAY);
        break;
      case MOTOR_ANGLE_POWER_BACK: break;
      case MOTOR_ANGLE_ADJ:
        motor.mode = MOTOR_ANGLE_ADJ_BACK;
        motor_stop();
        task_event_post(TASK_MOTOR, EV_MOTOR_RUN, MOTOR_STOP_DELAY);
        break;
      case MOTOR_ANGLE_ADJ_BACK:break;
      case MOTOR_TEST:
        motor.mode = MOTOR_TEST_BACK;
        motor_stop();
        task_event_post(TASK_MOTOR, EV_MOTOR_RUN, MOTOR_STOP_DELAY);
        break;
      case MOTOR_TEST_BACK:break;
    }
  }
}

/**
 * @brief 任务定时器中断
 * @return 无返回值
 */
#pragma vector = TIMER1_A0_VECTOR
__interrupt void motor_timer_isr(void) 
{
  motor_ctl_handler();
}