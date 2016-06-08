/**
 * @file  button_task.c
 * @brief 按钮任务实现
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#include "button_task.h"
#include "app.h"
Button button_list[BUTTON_MAX_NUM];  ///< 按钮列表

/**
 * @brief 主按键短按
 * @return 无返回值
 */
void button_main_press();
/**
 * @brief 主按键长按
 * @return 无返回值
 */
void button_main_long_press();
/**
 * @brief 左按键短按
 * @return 无返回值
 */
void button_left_press();
/**
 * @brief 左按键长按
 * @return 无返回值
 */
void button_left_long_press();
/**
 * @brief 右按键短按
 * @return 无返回值
 */
void button_right_press();
/**
 * @brief 右按键长按
 * @return 无返回值
 */
void button_right_long_press();
/**
 * @brief 按键检测
 * @return 无返回值
 */
void button_detect();
/**
 * @brief 上调节
 * @return 无返回值
 */
void button_adjust_up();
/**
 * @brief 下调节
 * @return 无返回值
 */
void button_adjust_down();
/**
 * @brief 串口参数调节
 * @return 无返回值
 */
void button_uart_adjust(u8 no);

/**
 * @brief 按键任务
 * @return 无返回值
 */
void button_task() {
  u16 event;
  button_detect();
  if(TRUE == task_event_get(TASK_BUTTON, &event))
  {
    switch (event)
    {
    case EV_READY:
      break;
    case EV_BUTTON_MAIN_PRESS:
      ir_button.state = STATE_BUTTONN_UP;
      //motor_run(MOTOR_ROTATION_DIR_FORWARD);
      button_main_press();
      break;
    case EV_BUTTON_MAIN_LONG_PRESS:
      ir_button.state = STATE_BUTTONN_UP;
      button_main_long_press();
      break;
    case EV_BUTTON_LEFT_PRESS:
      ir_button.state = STATE_BUTTONN_UP;
      button_left_press();
      break;
    case EV_BUTTON_LEFT_LONG_PRESS:
      ir_button.state = STATE_BUTTONN_UP;
      button_left_long_press();
      break;
    case EV_BUTTON_RIGHT_PRESS:
      ir_button.state = STATE_BUTTONN_UP;
      button_right_press();
      break;
    case EV_BUTTON_RIGHT_LONG_PRESS:
      ir_button.state = STATE_BUTTONN_UP;
      button_right_long_press();
      break;
    }
  }
}

/* 按键检测 */
void button_detect() {
  u8 i = 0;
  Button *btn = button_list;
  for(;i < BUTTON_MAX_NUM;++i,++btn) {
    if(!(btn->bit&BUTTON_IN)) {
      if(STATE_BUTTONN_UP == btn->state) {
        btn->time_a = task_timer.tick;
        btn->state = STATE_BUTTONN_DOWN;
      } else if(STATE_BUTTONN_DOWN == btn->state) {
        if(task_timer.tick - btn->time_a > BUTTON_PRESS_TIME){
          task_event_post(TASK_BUTTON, EV_BUTTON_MAIN_PRESS + i, IRDA_BUTTON_DELAY);
          btn->state = STATE_BUTTONN_PRESS;
        }
      } else if(STATE_BUTTONN_PRESS == btn->state) {
        if(task_timer.tick - btn->time_a > BUTTON_LONG_PRESS_TIME) {
          task_event_post(TASK_BUTTON, EV_BUTTON_MAIN_LONG_PRESS + i, IRDA_BUTTON_DELAY);
          btn->state = STATE_BUTTONN_LONG_PRESS;
        } else {
          task_event_post(TASK_BUTTON, EV_BUTTON_MAIN_PRESS + i, IRDA_BUTTON_DELAY);
        }
      }
    } else {
      btn->state = STATE_BUTTONN_UP;
      btn->time_a = task_timer.tick;
    }
  }
}

/* 主键短按 */
void button_main_press() {
  if(lcd_page.page < PAGE_LOGIN) {           //显示界面
    lcd_page.page = PAGE_MAIN;
  } else if(PAGE_LOGIN == lcd_page.page) {  //登陆界面
    lcd_page.status += 1;
    if(lcd_page.status > ITEM_FOCUS_FOURTH) {
      lcd_page.status = ITEM_FOCUS_FIRST;
    }
  } else if(PAGE_SETTING == lcd_page.page) {//设置界面
    /**if(ITEM_PASSWD == lcd_page.hover_item) { //密码、串口设置界面
      lcd_page.status += 1;
      if(ITEM_FOCUS_ALL == lcd_page.status) {
        lcd_page.status += 1;
      } else if(lcd_page.status > ITEM_FOCUS_FOURTH) {
        lcd_page.status = ITEM_HOVER;
      }
    } else */if(ITEM_FLOW_UART == lcd_page.hover_item 
              || ITEM_RTU_UART == lcd_page.hover_item) {
      lcd_page.status += 1;
      if(ITEM_FOCUS_ALL == lcd_page.status) {
        lcd_page.status += 1;
      } else if(lcd_page.status > ITEM_FOCUS_SIXTH) {
        lcd_page.status = ITEM_HOVER;
      }
    } else {
      lcd_page.status += 1;
      if(lcd_page.status > ITEM_FOCUS_ALL) {
        lcd_page.status = ITEM_HOVER;
      }
    }
  }
  task_event_post(TASK_LCD, EV_FRAME_RECEIVED, 0);
}

/* 主键长按 */
void button_main_long_press() {
  if(lcd_page.page < PAGE_LOGIN) {
    lcd_page.status = ITEM_FOCUS_FIRST;
    lcd_page.page = PAGE_LOGIN;
  } else if(lcd_page.page == PAGE_LOGIN) {
    u16 pwd_tmp = ((u16)actuator.pwd_tmp[0]<<12) | ((u16)actuator.pwd_tmp[1]<<8) | ((u16)actuator.pwd_tmp[2]<<4) | ((u16)actuator.pwd_tmp[3]);
    if(USR_PWD_ROOT == pwd_tmp) {
      lcd_page.page = PAGE_SETTING;
      lcd_page.hover_item = ITEM_ADDR;//ITEM_PASSWD;
      lcd_page.status = ITEM_HOVER;
      lcd_page.max_item = ITEM_MAX_NUM;
    } else {
      lcd_page.page = PAGE_SETTING;
      lcd_page.hover_item = ITEM_ADDR;//ITEM_PASSWD;
      lcd_page.status = ITEM_HOVER;
      lcd_page.max_item = ITEM_MOTOR_MANUAL_ADJ + 1;
    }
  } else if(PAGE_SETTING == lcd_page.page) {
    lcd_page.page = PAGE_MAIN;
    //task_event_post(TASK_FLASH, EV_SAVE_CONFIG, 1000);
  } else {
    lcd_page.page = PAGE_MAIN;
  }
  task_event_post(TASK_LCD, EV_FRAME_RECEIVED, 0);
}
void button_left_press() {
  if(lcd_page.page < PAGE_LOGIN) {  //主界面
    if(lcd_page.page > PAGE_MAIN) {
      lcd_page.page -= 1;
    } else {
      lcd_page.page = PAGE_SUB1;
    }
  }else if(PAGE_LOGIN == lcd_page.page) {
    if(0 == actuator.pwd_tmp[lcd_page.status - ITEM_FOCUS_FIRST]) {
      actuator.pwd_tmp[lcd_page.status - ITEM_FOCUS_FIRST] = 0xF;
    } else {
      actuator.pwd_tmp[lcd_page.status - ITEM_FOCUS_FIRST] -= 1;
    }
  } else if(PAGE_SETTING == lcd_page.page) {
    if(ITEM_HOVER != lcd_page.status) {      //调节
      button_adjust_up();
    } else {
      if(lcd_page.hover_item < 1) {
        lcd_page.hover_item = lcd_page.max_item - 1;
      } else {
        lcd_page.hover_item -= 1;
      }
    }
  }
  task_event_post(TASK_LCD, EV_FRAME_RECEIVED, 0);
}
void button_left_long_press() {

}

void button_right_press() {
  if(lcd_page.page < PAGE_LOGIN) {
    if(lcd_page.page < PAGE_SUB1) {
      lcd_page.page += 1;
    } else {
      lcd_page.page = PAGE_MAIN;
    }
  }else if(PAGE_LOGIN == lcd_page.page) {
    actuator.pwd_tmp[lcd_page.status - ITEM_FOCUS_FIRST] += 1;
    if(actuator.pwd_tmp[lcd_page.status - ITEM_FOCUS_FIRST] > 0xF) {
      actuator.pwd_tmp[lcd_page.status - ITEM_FOCUS_FIRST] = 0;
    }
  }else if(PAGE_SETTING == lcd_page.page) {
    if(ITEM_HOVER != lcd_page.status) {  
      button_adjust_down();
    } else {
      if(lcd_page.hover_item >= lcd_page.max_item - 1) {
        lcd_page.hover_item = 0;
      } else {
        lcd_page.hover_item += 1;
      }
    }
  }
  task_event_post(TASK_LCD, EV_FRAME_RECEIVED, 0);
}

void button_right_long_press() {
  
}

void button_adjust_up() {
  switch(lcd_page.hover_item) {
    /**case ITEM_PASSWD:
      if(lcd_page.status - ITEM_FOCUS_FIRST <= 0) {
        break;
      }
      if(0 == actuator.pwd_tmp[lcd_page.status - ITEM_FOCUS_FIRST]) {
        actuator.pwd_tmp[lcd_page.status - ITEM_FOCUS_FIRST] = 0xF;
      } else {
        actuator.pwd_tmp[lcd_page.status - ITEM_FOCUS_FIRST] -= 1;
      }
      break;*/
    case ITEM_ADDR:
      if(actuator.slave <= 1) {
        actuator.slave = 0x99;
      } else {
        --actuator.slave;
      }
      break;
    case ITEM_BACK_LIGHT:
      if(lcd_page.light_time <= 0) {
        lcd_page.light_time = 120;
      } else {
        --lcd_page.light_time;
      }
      break;
    /**case ITEM_OPEN_ANGLE:
      if(actuator.open_angle <= 0) {
        actuator.open_angle = 100;
      } else {
        --actuator.open_angle;
      }
      break;*/
    case ITEM_PRECISION:
      if(flow_meter.precision <= 1) {
        flow_meter.precision = 100;
      } else {
        --flow_meter.precision;
      }
      break;
    case ITEM_INJECTION:
      if(flow_meter.daily_injection/1000 <= 0) {
        flow_meter.daily_injection = FLOW_METER_DAILY_INJECTION_MAX;
      } else {
        flow_meter.daily_injection -= 1000;
      }
      break;
    /**case ITEM_FLIP_PAGE:
      if(lcd_page.flip_page_time <= 0) {
        lcd_page.flip_page_time = 100;
      } else {
        --lcd_page.flip_page_time;
      }
      break;*/
    case ITEM_FLOW_UART:break;
    case ITEM_RTU_UART:break;
    case ITEM_FLOW_TYPE:
      if(flow_meter.type <= 0) {
        flow_meter.type = 0x99;
      } else {
        --flow_meter.type;
      }
      break;
    case ITEM_FLOW_ADDR:
      if(flow_meter.slave <= 0) {
        flow_meter.slave = 0x99;
      } else {
        --flow_meter.slave;
      }
      break;
    case ITEM_MOTOR_REMOTE_CTL:
      motor_ctl(TRUE, MOTOR_DIR_SHUT, actuator.motor_ratio, actuator.motor_time, MOTOR_ANGLE_POWER);
      break;
    case ITEM_MOTOR_MANUAL_ADJ:
      //flow_meter.auto_adj = FLOW_METER_ADJ_MANUAL;
      if(flow_meter.auto_adj <= FLOW_METER_ADJ_AUTO) {
        flow_meter.auto_adj = FLOW_METER_ONLY_MANUAL;
      } else {
        --flow_meter.auto_adj;
      }
      break;
    case ITEM_MOTOR_ADJ:
      motor_ctl(TRUE, MOTOR_DIR_SHUT, MOTOR_WAVE_DUTY_RATIO_DEF, MOTOR_ADJ_COUNT_DEF, MOTOR_ANGLE_ADJ);
      break;
    case ITEM_MOTOR_TEST:
      motor_ctl(TRUE, MOTOR_DIR_SHUT, MOTOR_WAVE_DUTY_RATIO_DEF, MOTOR_ADJ_COUNT_DEF, MOTOR_TEST);
      break;
    case ITEM_MOTOR_RATIO:
      if(actuator.motor_ratio <= MOTOR_WAVE_DUTY_RATIO_MIN) {
        actuator.motor_ratio = MOTOR_WAVE_DUTY_RATIO_MAX;
      } else {
        --actuator.motor_ratio;
      }
      break;
    case ITEM_MOTOR_TIME:
      if(actuator.motor_time <= 0) {
        actuator.motor_time = 10000;
      } else {
        actuator.motor_time -= 100;
      }
      break;
    case ITEM_LPM_SWITCH:
      actuator.lpm_switch = TRUE;
      break;
    case ITEM_SAVE_CONF:
      flash.saved = FALSE;
      task_event_post(TASK_FLASH, EV_SAVE_CONFIG, 1000);
      break;
    case ITEM_REBOOT:
      PMMCTL0 = PMMPW + PMMSWBOR;//电源控制复位
      break;
    case ITEM_MOTOR_REVERSE:
      motor.reverse = TRUE;
      break;
    case ITEM_FLOW_REQ_TIME0:
      if(flow_meter.req_time_0 <= 2000) {
        flow_meter.req_time_0 = 60000;
      } else {
        flow_meter.req_time_0 -= 100;
      }
      break;
    case ITEM_FLOW_REQ_TIME1:
      if(flow_meter.req_time_1 <= 2000) {
        flow_meter.req_time_1 = 60000;
      } else {
        flow_meter.req_time_1 -= 100;
      }
      break;
    case ITEM_FLOW_MOTOR_WEIGHT_EDGE:
      if(flow_meter.weight_edge <= FLOW_METER_WEIGHT_EDGE_MIN) {
        flow_meter.weight_edge = FLOW_METER_WEIGHT_EDGE_MAX;
      } else {
        flow_meter.weight_edge -= 1;
      }
      break;
    case ITEM_PRECISION_RATIO:
      if(flow_meter.precision_ratio <= 0) {
        flow_meter.precision_ratio = 100;
      } else {
        flow_meter.precision_ratio -= 1;
      }
      break;
  }
}

void button_adjust_down() {
  switch(lcd_page.hover_item) {
    /**case ITEM_PASSWD:
      if(lcd_page.status - ITEM_FOCUS_FIRST <= 0) {
        break;
      }
      if(0xF == actuator.pwd_tmp[lcd_page.status - ITEM_FOCUS_FIRST]) {
        actuator.pwd_tmp[lcd_page.status - ITEM_FOCUS_FIRST] = 0;
      } else {
        actuator.pwd_tmp[lcd_page.status - ITEM_FOCUS_FIRST] += 1;
      }
      break;*/
    case ITEM_ADDR:
      if(actuator.slave >= 0x99) {
        actuator.slave = 1;
      } else {
        ++actuator.slave;
      }
      break;
    case ITEM_BACK_LIGHT:
      if(lcd_page.light_time >= 120) {
        lcd_page.light_time = 0;
      } else {
        ++lcd_page.light_time;
      }
      break;
    /**case ITEM_OPEN_ANGLE:
      if(actuator.open_angle >= 100) {
        actuator.open_angle = 0;
      } else {
        ++actuator.open_angle;
      }
      break;*/
    case ITEM_PRECISION:
      if(flow_meter.precision >= 100) {
        flow_meter.precision = 1;
      } else {
        ++flow_meter.precision;
      }
      break;
    case ITEM_INJECTION:
      if(flow_meter.daily_injection/1000 >= FLOW_METER_DAILY_INJECTION_MAX/1000) {
        flow_meter.daily_injection = 0;
      } else {
        flow_meter.daily_injection += 1000;
      }
      break;
    /**case ITEM_FLIP_PAGE:
      if(lcd_page.flip_page_time >= 120) {
        lcd_page.flip_page_time = 0;
      } else {
        ++lcd_page.flip_page_time;
      }
      break;*/
    case ITEM_FLOW_UART:
      button_uart_adjust(FLOW_METER_UART);
      break;
    case ITEM_RTU_UART:
      button_uart_adjust(RTU_UART);
      break;
    case ITEM_FLOW_TYPE:
      if(flow_meter.type >= 0x99) {
        flow_meter.type = 0;
      } else {
        ++flow_meter.type;
      }
      break;
    case ITEM_FLOW_ADDR:
      if(flow_meter.slave >= 0x99) {
        flow_meter.slave = 0;
      } else {
        ++flow_meter.slave;
      }
      break;
    case ITEM_MOTOR_REMOTE_CTL:
      motor_ctl(TRUE, MOTOR_DIR_OPEN, actuator.motor_ratio, actuator.motor_time, MOTOR_ANGLE_POWER);
      break;
    case ITEM_MOTOR_MANUAL_ADJ:
      //flow_meter.auto_adj = FLOW_METER_ADJ_AUTO;
      if(flow_meter.auto_adj >= FLOW_METER_ONLY_MANUAL) {
        flow_meter.auto_adj = FLOW_METER_ADJ_AUTO;
      } else {
        ++flow_meter.auto_adj;
      }
      break;
    case ITEM_MOTOR_ADJ:
      motor_ctl(FALSE, MOTOR_DIR_OPEN, MOTOR_WAVE_DUTY_RATIO_DEF, MOTOR_ADJ_COUNT_DEF, MOTOR_ANGLE_ADJ);
      break;
    case ITEM_MOTOR_TEST:
      motor_ctl(FALSE, MOTOR_DIR_SHUT, MOTOR_WAVE_DUTY_RATIO_DEF, MOTOR_ADJ_COUNT_DEF, MOTOR_TEST);
      break;
    case ITEM_MOTOR_RATIO:
      if(actuator.motor_ratio >= 100) {
        actuator.motor_ratio = 0;
      } else {
        ++actuator.motor_ratio;
      }
      break;
    case ITEM_MOTOR_TIME:
      if(actuator.motor_time >= 10000) {
        actuator.motor_time = 100;
      } else {
        actuator.motor_time += 100;
      }
      break;
    case ITEM_LPM_SWITCH:
      actuator.lpm_switch = FALSE;
      break;
    case ITEM_MOTOR_REVERSE:
      motor.reverse = FALSE;
      break;
    case ITEM_FLOW_REQ_TIME0:
      if(flow_meter.req_time_0 >= 60000) {
        flow_meter.req_time_0 = 2000;
      } else {
        flow_meter.req_time_0 += 100;
      }
      break;
    case ITEM_FLOW_REQ_TIME1:
      if(flow_meter.req_time_1 >= 60000) {
        flow_meter.req_time_1 = 2000;
      } else {
        flow_meter.req_time_1 += 100;
      }
      break;
    case ITEM_FLOW_MOTOR_WEIGHT_EDGE:
      if(flow_meter.weight_edge >= FLOW_METER_WEIGHT_EDGE_MAX) {
        flow_meter.weight_edge = FLOW_METER_WEIGHT_EDGE_MIN;
      } else {
        flow_meter.weight_edge += 1;
      }
      break;
    case ITEM_PRECISION_RATIO:
      if(flow_meter.precision_ratio >= 100) {
        flow_meter.precision_ratio = 0;
      } else {
        flow_meter.precision_ratio += 1;
      }
      break;
  }
}

void button_uart_adjust(u8 no) {
  UartInfo *info = &uart_list[no].info;
  if(ITEM_FOCUS_FIRST == lcd_page.status) {
    if(115200 <= info->baudrate) {
      info->baudrate = uart_get_baud(0);
    } else {
      info->baudrate = uart_get_baud(info->baudrate + 1);
    }
  } else if(ITEM_FOCUS_SECOND == lcd_page.status) {
    if(info->bytesize >= 8) {
      info->bytesize = 7;
    } else {
      ++info->bytesize;
    }
  } else if(ITEM_FOCUS_THIRD == lcd_page.status) {
    if('O' <= info->parity) {
      info->parity = uart_get_parity(0);
    } else {
      info->parity = uart_get_parity(info->parity + 1);
    }
  } else if(ITEM_FOCUS_FOURTH == lcd_page.status) {
    if(info->stopbits >= 2) {
      info->stopbits  = 1;
    } else {
      ++info->stopbits ;
    }
  } else if(ITEM_FOCUS_FIFTH == lcd_page.status) {
    if(FLOW_METER_UART == no) {
      if(flow_meter.compro >= FLOW_PROTOCOL_MAX - 1) {
        flow_meter.compro = FLOW_PROTOCOL_AUTO;
      } else {
        ++flow_meter.compro;
      }
    } else if(RTU_UART == no) {
      if(rtu.compro >= RTU_COM_PRO_MAX - 1) {
        rtu.compro = RTU_COM_PRO_AUTO;
      } else {
        ++rtu.compro;
      }
    }
  } else if(ITEM_FOCUS_SIXTH == lcd_page.status) {
    if(FLOW_METER_UART == no) {
      if(flow_meter.commode >= COM_MODE_MASTER) {
        flow_meter.commode = COM_MODE_SLAVE;
      } else {
        ++flow_meter.commode;
      }
    } else if(RTU_UART == no) {
      if(rtu.commode >= COM_MODE_MASTER) {
        rtu.commode = COM_MODE_SLAVE;
      } else {
        ++rtu.commode;
      }
    }
  }
}