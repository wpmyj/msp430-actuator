/**
 * @file  infra-red.c
 * @brief 红外驱动实现
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#include "infra-red.h"

#define RED_R        (IRDA_BIT&IRDA_IN)     ///< 管脚状态

#define KEY_MAIN       0x05                 ///< 三个键的遥控器 键F(主键)
#define KEY_LEFT       0x0D                 ///< 键上
#define KEY_RIGHT      0x11                 ///< 键下

#define KEY_MAIN1      0x00                 ///< 六个键的遥控器 键F(主键)
#define KEY_LEFT1      0x02                 ///< 键上
#define KEY_RIGHT1     0x04                 ///< 键下
#define KEY_M1         0x01                 ///< 键M
#define KEY_S1         0x03                 ///< 键S
#define KEY_PLUSH_ONE1 0x05                 ///< 键+1

IRButton ir_button; ///< 红外按钮

/**
 * @brief 红外驱动初始化
 * @return 无返回值
 */
void infra_red_init() {
  IRDA_SEL &= ~IRDA_BIT;
  IRDA_DIR &= ~IRDA_BIT;
  IRDA_REN |= IRDA_BIT;
  IRDA_OUT |= IRDA_BIT;
  IRDA_IES |= IRDA_BIT;
  IRDA_IE |= IRDA_BIT;
  IRDA_IFG &= ~IRDA_BIT;
  
  ir_button.state = STATE_BUTTONN_UP;
  ir_button.bit = 0;
}

/**
 * @brief 按键解码
 * @param[in] code 数据
 * @param[in] repeat 重发标记
 * @return 无返回值
 */
void key_icode(u8 code, u8 repeat)
{
  ir_button.bit = code;
  u8 i = 0;
  if(KEY_MAIN == code || KEY_MAIN1 == code) {
    i = BUTTON_MAIN;
  } else if(KEY_LEFT == code || KEY_LEFT1 == code) {
    i = BUTTON_LEFT;
  } else if(KEY_RIGHT == code || KEY_RIGHT1 == code) {
    i = BUTTON_RIGHT;
  } else {
    return;
  }
  if(STATE_BUTTONN_UP == ir_button.state) {
    if(FALSE == repeat) {
      task_event_post(TASK_BUTTON, EV_BUTTON_MAIN_PRESS + i, IRDA_BUTTON_DELAY);
      ir_button.state = STATE_BUTTONN_PRESS;
      ir_button.time_a = task_timer.tick;
    }
  } else if(STATE_BUTTONN_PRESS == ir_button.state) {
    if(task_timer.tick - ir_button.time_a > BUTTON_LONG_PRESS_TIME) {
      task_event_post(TASK_BUTTON, EV_BUTTON_MAIN_LONG_PRESS + i, 0);
      ir_button.state = STATE_BUTTONN_LONG_PRESS;
    } else {
      task_event_post(TASK_BUTTON, EV_BUTTON_MAIN_PRESS + i, IRDA_BUTTON_DELAY);
    }
  }
}

/**
 * @brief 红外处理
 * @return 无返回值
 */
u8 irda_isr_handler() {
  IRDA_IFG &= ~IRDA_BIT;
  u8 i, j, k = 0;
  u8 code_value=0;
  u8 key_code[4];
  for(i = 0;i < 19;++i)
  {
    DELAY_US(400);              //延时400us
    if(RED_R)                   //9ms内有高电平，则判断为干扰，退出处理程序
    {
      return FALSE;
    }
  }
  while(!RED_R);
  for(i=0;i<5;++i)              //是否连发码
  {
    DELAY_US(500);
    if(!RED_R)
    {
      key_icode(ir_button.bit, TRUE);
      return TRUE;
    }
  }
  while(RED_R);                //等待4.5ms高电平过去
  for(i = 0;i < 4;++i)         //接收4个字节的数据
  {
    for(j = 0;j < 8;++j)       //每个字节的数据8位
    {
      
      while(!RED_R);           //等待变高电平
      while(RED_R)             //计算高电平时间
      {
        DELAY_US(100);
        k++;
        if(k > 22)              //高电平时间过长，则退出处理程序
        {
          return FALSE;        
        }
        
      }
      code_value>>=1;          //接受一位数据
      if(k >= 7)
      {
        code_value|=0x80;      //高电平时间大于0.56，则为数据1
      }
      k = 0;                   //计时清零
    }
    key_code[i]=code_value;    //四个字节的数据
  }
  if((u8)key_code[2] != (u8)~key_code[3]) {
    return FALSE;
  }
  key_icode(key_code[2], FALSE);      //调用赋值函数
  return TRUE;
}

/**
 * @brief 红外中断
 * @return 无返回值
 */
#pragma vector=PORT1_VECTOR
__interrupt void irda_isr(void) {
switch(__even_in_range(P1IV, 16))
  {
    case 0:
      break;
    case 2:
      break;
    case 4:
      if(TRUE == irda_isr_handler()) { //P1.1
        LOW_POWER_MODE_OFF();
      }
      break;
    case 6:
      break;
    case 8:
      break;
    case 10:
      break;
    case 12:
      break;
    case 14:
      break;
    case 16:
      motor_edge_ctl();
      break;
  }
}