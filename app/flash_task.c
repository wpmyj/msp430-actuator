/**
 * @file  flash_task.c
 * @brief flash任务实现
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#include "flash_task.h"
#include "app.h"

Flash flash;    ///< flash属性
/**
 * @brief flash同步处理
 * @return 无返回值
 */
void flash_syn_handler();
/**
 * @brief 参数刷新(刷新到配置缓冲区)
 * @return 无返回值
 */
void flash_flush_config();
/**
 * @brief 从flash加载参数
 * @return 无返回值
 */
void flash_load_config();
/**
 * @brief 保存参数
 * @return 无返回值
 */
void flash_save_config();
/**
 * @brief 应用参数
 * @return 无返回值
 */
void flash_apply_config();

void flash_uart_apply(UartInfo *to, UartInfo *from);

void flash_pec_apply_config();

/**
 * @brief flash任务入口
 * @return 无返回值
 */
void flash_task() {
  u16 event;
  flash_syn_handler();
  if(TRUE == task_event_get(TASK_FLASH, &event))
  {
    switch (event)
    {
    case EV_READY:
      break;
    case EV_SAVE_CONFIG:
      flash.action = FLASH_CHANGED;
      break;
    case EV_APPLY_CONFIG:
      flash_flush_config();
      flash_apply_config();
    case EV_UPGRADE:
      upgrade_action();
      break;
    }
  }
}

void flash_syn_handler() {
  if(FLASH_CHANGED == flash.action 
     && task_timer.tick - flash.time_a > flash.itv_time) {
    flash_save_config();
    flash.saved = TRUE;
    flash.action = FLASH_NO_CHANGE;
    flash.time_a = task_timer.tick;
  }
}

void flash_flush_config() {
  config.sys_timer_tick = task_timer.sys_tick;          // 系统时间
  
  config.light_time = lcd_page.light_time;         // 背光
  config.flip_page_time = lcd_page.flip_page_time; // 翻页时间间隔
  
  /* 执行器RTU串口参数 */
  config.actuator_slave = actuator.slave;
  flash_uart_apply(&config.rtu_uart, &uart_list[RTU_UART].info);
  
  /* 执行器流量计串口参数 */
  config.flow_meter_slave = flow_meter.slave;
  flash_uart_apply(&config.flow_meter_uart, &uart_list[FLOW_METER_UART].info);
  
  config.flow_meter_type = flow_meter.type;            //仪表类型
  config.precision = flow_meter.precision;             //调节精度
  config.daily_injection = flow_meter.daily_injection; //日配注

  /* 电机相关参数 */
  config.angle_ad_max = actuator.angle_ad_max;
  config.angle_ad_min = actuator.angle_ad_min;
  config.open_angle = actuator.open_angle;
  
  config.lpm_switch = actuator.lpm_switch;
  config.motor_time = actuator.motor_time;
  config.motor_ratio = actuator.motor_ratio;
  config.motor_reverse = motor.reverse;
  
  config.flow_meter_req_time_0 = flow_meter.req_time_0;
  config.flow_meter_req_time_1 = flow_meter.req_time_1;
  
  config.weight_edge = flow_meter.weight_edge;
  config.precision_ratio = flow_meter.precision_ratio;
  config.flow_compro = flow_meter.compro;
  config.flow_commode = flow_meter.commode;
  
  config.rtu_compro = rtu.compro;
  config.rtu_commode = rtu.commode;
}

void flash_load_config() {
  memcpy(&config, (u8 *)FLASH_INFO_ADDR, sizeof(Config));
  flash_apply_config();
}

void flash_save_config() {
  flash_flush_config();
  flash_apply_config();
  FLASH_WRITE((u8 *)FLASH_INFO_ADDR, 0, (u8 *)&config, sizeof(Config), FLASH_INFO_SEGMENT_SIZE);
}

void flash_uart_apply(UartInfo *to, UartInfo *from) {
  /* 波特率 */
  if(from->baudrate < 300 || from->baudrate > 115200) {
    from->baudrate = UART_DEFAULT_BAUD;
  }
  to->baudrate = from->baudrate;
  /* 数据位 */
  if(from->bytesize < 8 || from->bytesize > 8) {
    from->bytesize = UART_DEFAULT_BYTE;
  }
  to->bytesize = from->bytesize;
  /* 奇偶校验位 */
  if(from->parity != 'N' 
     || from->parity != 'O'
     || from->parity != 'E') {
    from->parity = UART_DEFAULT_PARITY;
    if(1 == from->parity) {
      from->parity = 'O';
    } else if(2 == from->parity) {
      from->parity = 'E';
    }
  }
  to->parity = from->parity;
  /* 停止位 */
  if(from->stopbits < 1 || from->stopbits > 2) {
    from->stopbits = UART_DEFAULT_STOP_BITS;
  }
  to->stopbits = from->stopbits;
}

void flash_apply_config() {
  //sys_timer_tick = config.sys_timer_tick;
  /* 执行器与RTU通信串口参数 */
  /* 从机号 */
  if(config.actuator_slave < 0x01 || config.actuator_slave > 0x99) {
    config.actuator_slave = ACTUATOR_SLAVE_NO;
  }
  actuator.slave = config.actuator_slave;
  flash_uart_apply(&uart_list[RTU_UART].info, &config.rtu_uart);
  
  /* 执行器与流量计通信串口参数 */
  /* 从机号 */
  if(config.flow_meter_slave < 0x01 || config.flow_meter_slave > 0x99) {
    config.flow_meter_slave = FlOW_METER_SLAVE;
  }
  flow_meter.slave = config.flow_meter_slave;
  flash_uart_apply(&uart_list[FLOW_METER_UART].info, &config.flow_meter_uart);
  
  if(config.light_time > 120) {
    config.light_time = 5;
  }
  lcd_page.light_time = config.light_time;
  if(config.open_angle > 100) {
    config.open_angle = 0;
  }
  actuator.open_angle = config.open_angle;         // 开度
  //lcd_page.light_time = config.light_time;         // 背光时间
  if(config.precision > 100) {
    config.precision = 5;
  }
  flow_meter.precision = config.precision;         // 精度
  if(config.flip_page_time > 120) {
    config.flip_page_time = 0;
  }
  lcd_page.flip_page_time = config.flip_page_time; // 翻页间隔
  if(config.flow_meter_type > 0x99) {
    config.flow_meter_type = 0;
  }
  flow_meter.type = config.flow_meter_type;        // 仪表类型
  if(config.daily_injection > 9999000L) {
    config.daily_injection = 40000L;
  }
  flow_meter.daily_injection = config.daily_injection; //日配置注入
  
  if(config.angle_ad_max > 4095) { // 这个地方还得改
    config.angle_ad_max = 4095;
  }
  if(config.angle_ad_max < 2000) { // 这个地方还得改
    config.angle_ad_max = 2000;
  }
  if(config.angle_ad_min > 1300) {
    config.angle_ad_min = 1300;
  }
  if(config.angle_ad_min < 300) {
    config.angle_ad_min = 300;
  }
  actuator.angle_ad_max = config.angle_ad_max;
  actuator.angle_ad_min = config.angle_ad_min;
  actuator.angle_ad_mid = (config.angle_ad_min + config.angle_ad_max)/2;
  
  actuator.lpm_switch = TRUE == config.lpm_switch?TRUE:FALSE;
  if(config.motor_time > 10000) {
    config.motor_time = 2000;
  }
  actuator.motor_time = config.motor_time;
  if(config.motor_ratio > 100) {
    config.motor_ratio = 50;
  }
  actuator.motor_ratio = config.motor_ratio;
  
  motor.reverse = TRUE == config.motor_reverse?TRUE:FALSE;
  
  if(config.flow_meter_req_time_0 > FLOW_METER_REQ_TIME_MAX 
     || config.flow_meter_req_time_0 < FLOW_METER_REQ_TIME_MIN) {
     config.flow_meter_req_time_0 = FLOW_METER_REQ_TIME0;
  }
  flow_meter.req_time_0 = config.flow_meter_req_time_0;
  if(config.flow_meter_req_time_1 > FLOW_METER_REQ_TIME_MAX 
     || config.flow_meter_req_time_1 < FLOW_METER_REQ_TIME_MIN) {
     config.flow_meter_req_time_1 = FLOW_METER_REQ_TIME1;
  }
  flow_meter.req_time_1 = config.flow_meter_req_time_1;
  if(FLOW_METER_WEIGHT_EDGE_MAX < config.weight_edge || config.weight_edge < 1) {
    config.weight_edge = FLOW_METER_WEIGHT_EDGE_MIN;
  }
  flow_meter.weight_edge = config.weight_edge;
  if(config.precision_ratio > 100) {
    config.precision_ratio = FLOW_METER_PRECISION_RATIO;
  }
  flow_meter.precision_ratio = config.precision_ratio;
  /** 流量计通信协议 */
  if(config.flow_compro >= FLOW_PROTOCOL_MAX) {
    config.flow_compro = FLOW_PROTOCOL_AUTO;
  }
  flow_meter.compro = config.flow_compro;
  /** 流量计通信方式 */
  if(config.flow_commode > COM_MODE_MASTER) {
    config.flow_commode = COM_MODE_MASTER;
  }
  flow_meter.commode = config.flow_commode;
  /** RTU通信协议 */
  if(config.rtu_compro >= RTU_COM_PRO_MAX) {
    config.rtu_compro = RTU_COM_PRO_MAX - 1;
  }
  rtu.compro = config.rtu_compro;
  /** RTU通信方式 */
  if(config.rtu_commode > COM_MODE_MASTER) {
    config.rtu_commode = COM_MODE_SLAVE;
  }
  rtu.commode = config.rtu_commode;
  
  flash_pec_apply_config();
  
  UART_RE_INIT(RTU_UART);
  UART_RE_INIT(FLOW_METER_UART);
}

/**
 * @brief 中石化协议参数应用
 * @return 无返回值
 */
void flash_pec_apply_config() {
  config.device_code = '1';
  config.vendor_code = '6';
  config.production_year = '1'<<8 | '5';
  config.production_month = '9';
  config.production_day = '1';
  config.model_h = 0;
  config.model_m = 0;
  config.model_l = 6002;
  config.tuning_range = 90;
  config.serial_number_h = 0;
  config.serial_number_l = 1;
  config.protocol = (1 == rtu.compro?0:1);
  config.device_addr = config.actuator_slave;
  config.rtu_addr = config.rtu_addr;
  config.baud_rate = config.rtu_uart.baudrate/1200;
  //config.ceiling_alarm_enable;
  //config.lower_alarm_enable;
  //config.injection;
  if(config.injection_digit > 5) {
    config.injection_digit = 2;
  }
  if(config.injection_max <= config.injection_min) {
    config.injection_min = 0;
    config.injection_max = 0xFFFF;
  }
  //config.injection_max;
  //config.injection_up_delay;
  //config.injection_min;
  //config.injection_down_delay;
  if(config.inj_pressure_range_max <= config.inj_pressure_range_min) {
    config.inj_pressure_range_min = 0;
    config.inj_pressure_range_max = 0xFFFF;
  }
  //config.inj_pressure_range_max;
  //config.inj_pressure_range_min;
  if(config.inj_pressure_range_digit > 5) {
    config.inj_pressure_range_digit = 2;
  }
  //config.inj_pressure_max;
  //config.inj_pressure_up_delay;
  //config.inj_pressure_min;
  //config.inj_pressure_down_delay;
  if(config.inj_pressure_max <= config.inj_pressure_min) {
    config.inj_pressure_min = 0;
    config.inj_pressure_max = 0xFFFF;
  }
  
  if(config.injection_less_digit > 5) {
    config.injection_less_digit = 2;
  }
  if(config.open_angle_digit > 5) {
    config.open_angle_digit = 2;
  }
}