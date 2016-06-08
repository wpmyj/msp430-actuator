/**
 * @file  app.c
 * @brief 应用初始化
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#include "app.h"
Config config;
extern void app_sleep();
extern void app_wakeup();

void app_init() {
  task_timer.task_sleep = app_sleep;
  task_timer.task_wakeup = app_wakeup;
  rtu.status = FALSE;
  flow_meter.status = FALSE;
  lcd_page.page = PAGE_MAIN;
  lcd_page.hover_item = ITEM_ADDR;//ITEM_PASSWD;
  lcd_page.frame_itv_time = LCD_FRAME_INT_TIME;
  lcd_page.x = LCD_PAGE_OFFSET_X;
  lcd_page.y = LCD_PAGE_OFFSET_Y;
  lcd_page.max_item = ITEM_FLOW_ADDR + 1;
  
  leds[LED_RUN].state = STATE_LED_BLINK;
  leds[LED_RUN].time_a = task_timer.tick;
  
  flash.action = FLASH_NO_CHANGE;
  flash.itv_time = FLASH_SYN_INT_TIME;
  flash.time_a = task_timer.tick;
  
  adc.scan_itv_time = task_timer.tick;
  
  BUTTON_CREATE(BUTTON_MAIN, BUTTON_MAIN_BIT);
  BUTTON_CREATE(BUTTON_LEFT, BUTTON_LEFT_BIT);
  BUTTON_CREATE(BUTTON_RIGHT, BUTTON_RIGHT_BIT);
  
  flow_meter.time_a = task_timer.tick;
  flow_meter.itv_time = FLOW_METER_REQ_TIME0;
  flow_meter.req_time_0 = FLOW_METER_REQ_TIME0;
  flow_meter.req_time_1 = FLOW_METER_REQ_TIME1;
  flow_meter.req_type = FLOW_METER_INSTANT_FLOW_REQ;
  flow_meter.protocol = FLOW_PROTOCOL_MODBUS;
  //flow_meter.weight_edge = FLOW_METER_WEIGHT_EDGE_MIN;
  actuator.day_time = task_timer.tick;
  //flow_meter.precision_ratio = FLOW_METER_PRECISION_RATIO;
  flow_meter.com_count = 0;
  rtu.com_count = 0;
  
  flash_load_config();
}

void app_sleep() {
  FLOW_PWR_SWITCH_OFF();
}

void app_wakeup() {
  FLOW_PWR_SWITCH_ON();
}