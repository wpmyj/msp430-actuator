/**
 * @file  register_map.c
 * @brief 寄存器映射实现
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#include "register_map.h"
#include "app.h"

//#define MAX_REGISTER_NUM  100

/**
 * @brief 读映射寄存器
 * @param[in] slave 从机地址
 * @param[in] addr 数据地址
 * @param[out] out_buf 寄存器输出
 * @retval TRUE 成功
 * @retval FALSE 失败
 */
u8 map_read_registers(u8 slave, u16 addr, u8 *out_buf);

/**
 * @brief 写映射寄存器
 * @param[in] slave 从机地址
 * @param[in] addr 数据地址
 * @param[in] in_buf 寄存器输入
 * @retval TRUE 成功
 * @retval FALSE 失败
 */
u8 map_write_registers(u8 slave, u16 addr, u8 *in_buf);
   
/**
 * @brief 读寄存器
 * @param[in] slave 从机地址
 * @param[in] addr 数据地址
 * @param[in] len 寄存器长度
 * @param[out] out_buf 寄存器输出
 * @retval TRUE 成功
 * @retval FALSE 失败
 */
u8 read_registers(u8 slave, u16 addr, u16 len, u8 *out_buf) {
  u16 i = 0;
  for(;i < len;++i) {
    map_read_registers(slave, i + addr, out_buf + 2 * i);
  }
  return TRUE;
}

/**
 * @brief 写寄存器
 * @param[in] slave 从机地址
 * @param[in] addr 数据地址
 * @param[in] len 寄存器长度
 * @param[in] in_buf 寄存器输入
 * @retval TRUE 成功
 * @retval FALSE 失败
 */
u8 write_registers(u8 slave, u16 addr, u16 len, u8 *in_buf) {
  if(MAP_UPGRADE_ADDR == addr) {
    return upgrade_map_write_registers(slave, in_buf, len);
  }
  u16 i = 0;
  u8 save = FALSE;
  for(;i < len;++i) {
    map_write_registers(slave, i + addr, in_buf + 2 * i);
    if(i + addr >= 0x000C && i + addr <= 0x0024) {
      save = TRUE;
    }
  }
  if(TRUE == save) {
    task_event_post(TASK_FLASH, EV_SAVE_CONFIG, 1000);
  }
  return TRUE;
}

u8 map_read_registers(u8 slave, u16 addr, u8 *out_buf) {
  switch(addr) {
    /** 中石化协议映射表 */
    case MAP_PEC_DEVICE_CODE:
      u16_to_u8_big(config.device_code, out_buf);
      break;
    case MAP_PEC_VENDOR_CODE:
      u16_to_u8_big(config.vendor_code, out_buf);
      break;
    case MAP_PEC_PRODUCTION_YEAR:
      u16_to_u8_big(config.production_year, out_buf);
      break;
    case MAP_PEC_PRODUCTION_MONTH:
      u16_to_u8_big(config.production_month, out_buf);
      break;
    case MAP_PEC_PRODUCTION_DAY:
      u16_to_u8_big(config.production_day, out_buf);
      break;
    case MAP_PEC_MODEL_H:
      u16_to_u8_big(config.model_h, out_buf);
      break;
    case MAP_PEC_MODEL_M:
      u16_to_u8_big(config.model_m, out_buf);
      break;
    case MAP_PEC_MODEL_L:
      u16_to_u8_big(config.model_l, out_buf);
      break;
    case MAP_PEC_TUNING_RANGE:
      u16_to_u8_big(config.tuning_range, out_buf);
      break;
    case MAP_PEC_SERIAL_NUMBER_H:
      u16_to_u8_big(config.serial_number_h, out_buf);
      break;
    case MAP_PEC_SERIAL_NUMBER_L:
      u16_to_u8_big(config.serial_number_l, out_buf);
      break;
    case MAP_PEC_PROTOCOL:///< 
      config.protocol = (1 == rtu.compro?0:1);
      u16_to_u8_big(config.protocol, out_buf);
      break;
    case MAP_PEC_DEVICE_ADDR:
      u16_to_u8_big(actuator.slave, out_buf);
      break;
    case MAP_PEC_RTU_ADDR:
      u16_to_u8_big(rtu.slave, out_buf);
      break;
    case MAP_PEC_BAUD_RATE:
      u16_to_u8_big(uart_list[RTU_UART].info.baudrate/1200, out_buf);
      break;
    case MAP_PEC_CEILING_ALARM_ENABLE:
      u16_to_u8_big(config.ceiling_alarm_enable, out_buf);
      break;
    case MAP_PEC_LOWER_ALARM_ENABLE:
      u16_to_u8_big(config.lower_alarm_enable, out_buf);
      break;
    case MAP_PEC_INJECTION:
      config.injection = fp32_2_u16_l((fp32)flow_meter.daily_injection/1000, config.injection_digit);
      u16_to_u8_big(config.injection, out_buf);
      break;
    case MAP_PEC_INJECTION_DIGIT:
      u16_to_u8_big(config.injection_digit, out_buf);
      break;
    case MAP_PEC_INJECTION_MAX:
      u16_to_u8_big(config.injection_max, out_buf);
      break;
    case MAP_PEC_INJECTION_UP_DELAY:
      u16_to_u8_big(config.injection_up_delay, out_buf);
      break;
    case MAP_PEC_INJECTION_MIN:
      u16_to_u8_big(config.injection_min, out_buf);
      break;
    case MAP_PEC_INJECTION_DOWN_DELAY:
      u16_to_u8_big(config.injection_down_delay, out_buf);
      break;
    case MAP_PEC_INJ_PRESSURE_RANGE_MAX:
      u16_to_u8_big(config.inj_pressure_range_max, out_buf);
      break;
    case MAP_PEC_INJ_PRESSURE_RANGE_MIN:
      u16_to_u8_big(config.inj_pressure_range_min, out_buf);
      break;
    case MAP_PEC_INJ_PRESSURE_RANGE_DIGIT:
      u16_to_u8_big(config.inj_pressure_range_digit, out_buf);
      break;
    case MAP_PEC_INJ_PRESSURE_MAX:
      u16_to_u8_big(config.inj_pressure_max, out_buf);
      break;
    case MAP_PEC_INJ_PRESSURE_UP_DELAY:
      u16_to_u8_big(config.inj_pressure_up_delay, out_buf);
      break;
    case MAP_PEC_INJ_PRESSURE_MIN:
      u16_to_u8_big(config.inj_pressure_min, out_buf);
      break;
    case MAP_PEC_INJ_PRESSURE_DOWN_DELAY:
      u16_to_u8_big(config.inj_pressure_down_delay, out_buf);
      break;
    case MAP_PEC_INJECTION_LESS_DIGIT:
      u16_to_u8_big(config.injection_less_digit, out_buf);
      break;
    case MAP_PEC_OPEN_ANGLE_DIGIT:
      u16_to_u8_big(config.open_angle_digit, out_buf);
      break;
    case MAP_PEC_INJECTION_UP_ALARM:
      u16_to_u8_big(flow_meter.injection_up_alarm, out_buf);
      break;
    case MAP_PEC_INJECTION_DOWN_ALARM:
      u16_to_u8_big(flow_meter.injection_down_alarm, out_buf);
      break;
    case MAP_PEC_RUN_STATE:
      actuator.run_state = 1;
      if(actuator.angle > 89.0) {
        actuator.run_state |= 1<<1;
      } else if(actuator.angle < 1.0) {
        actuator.run_state |= 1<<2;
      }
      u16_to_u8_big(actuator.run_state, out_buf);
      break;
    case MAP_PEC_RUN_TIME_H:
      actuator.run_time_h = (task_timer.sys_tick/1000/3600)>>16&0xFFFF;
      u16_to_u8_big(actuator.run_time_h, out_buf);
      break;
    case MAP_PEC_RUN_TIME_L:
      actuator.run_time_l = (task_timer.sys_tick/1000/3600)&0xFFFF;
      u16_to_u8_big(actuator.run_time_l, out_buf);
      break;
    case MAP_PEC_INJECTION_LESS:
      u16_to_u8_big(flow_meter.injection_less, out_buf);
      break;
    case MAP_PEC_INJ_PRESSURE:
      u16_to_u8_big(flow_meter.inj_pressure, out_buf);
      break;
    case MAP_PEC_OPEN_ANGLE:
      actuator.open_angle = fp32_2_u8(actuator.angle/90, config.open_angle_digit);
      u16_to_u8_big(actuator.open_angle, out_buf);
      break;
    case MAP_PEC_INSTANT_FLOW:
      u16_to_u8_big(flow_meter.instant_flow/10, out_buf);
      break;
    case MAP_PEC_CUMULA_FLOW_H:
      u16_to_u8_big(((u32)(flow_meter.cumula_flow*100))>>16, out_buf);
      break;
    case MAP_PEC_CUMULA_FLOW_L:
      u16_to_u8_big(((u32)(flow_meter.cumula_flow*100))&0xFFFF, out_buf);
      break;
    /** END 中石化协议映射表 */
    case MAP_BATTERY_VOLTAGE:
      u16_to_u8_big((u16)(actuator.voltage * 100), out_buf);
      break;
    case MAP_AUTO_ADJ_NO:
      u16_to_u8_big(actuator.cumula_adj_no, out_buf);
      break;
    case MAP_RTU_COM_STATE:
      u16_to_u8_big(rtu.status, out_buf);
      break;
    case MAP_FLOW_METER_COM_STATE:
      u16_to_u8_big(flow_meter.status, out_buf);
      break;
    case MAP_HOUR:
      *out_buf = 0;
      *(out_buf+1) = u8_to_bcd((task_timer.sys_tick/1000/3600%24)&0xFF);
      break;
    case MAP_MINUTE:
      *out_buf = 0;
      *(out_buf+1) = u8_to_bcd((task_timer.sys_tick/1000/60%60)&0xFF);
      break;
    case MAP_SECOND:
      *out_buf = 0;
      *(out_buf+1) = u8_to_bcd((task_timer.sys_tick/1000%60)&0xFF);
      break;
    case MAP_ANGLE:
      u16_to_u8_big((u16)actuator.angle, out_buf);
      break;
    case MAP_INSTANT_FLOW_H:
      u16_to_u8_big(flow_meter.instant_flow>>16, out_buf);
      break;
    case MAP_INSTANT_FLOW_L:
      u16_to_u8_big(flow_meter.instant_flow&0xFFFF, out_buf);
      break;
    case MAP_DAILY_INJECTION_H:
      u16_to_u8_big(flow_meter.daily_injection>>16, out_buf);
      break;
    case MAP_DAILY_INJECTION_L:
      u16_to_u8_big(flow_meter.daily_injection&0xFFFF, out_buf);
      break;
    case MAP_ACTUATOR_SLAVE:
      u16_to_u8_big(actuator.slave, out_buf);
      break;
    case MAP_LCD_LIGHT_TIME:
      u16_to_u8_big(lcd_page.light_time, out_buf);
      break;
    case MAP_OPENING_VALUE:
      u16_to_u8_big(actuator.open_angle, out_buf);
      break;
    case MAP_PRECISION:
      u16_to_u8_big(flow_meter.precision, out_buf);
      break;
    case MAP_COM_QUALITY:
      u16_to_u8_big((u16)flow_meter.com_quality, out_buf);
      break;
    case MAP_CUMULA_FLOW_H:
      fp32_to_u8_h_big(flow_meter.cumula_flow, out_buf);
      break;
    case MAP_CUMULA_FLOW_L:
      fp32_to_u8_l_big(flow_meter.cumula_flow, out_buf);
      break;
    case MAP_FLOW_METER_TYPE:
      u16_to_u8_big(flow_meter.type, out_buf);
      break;
    case MAP_FLOW_METER_VER:
      u16_to_u8_big(flow_meter.version, out_buf);
      break;
    case MAP_FLOW_METER_SLAVE:
      u16_to_u8_big(flow_meter.slave, out_buf);
      break;
    case MAP_FLOW_METER_UART_BAUD_H:
      u16_to_u8_big(uart_list[FLOW_METER_UART].info.baudrate>>16, out_buf);
      break;
    case MAP_FLOW_METER_UART_BAUD_L:
      u16_to_u8_big(uart_list[FLOW_METER_UART].info.baudrate&0xFFFF, out_buf);
      break;
    case MAP_FLOW_METER_UART_BYTE:
      u16_to_u8_big(uart_list[FLOW_METER_UART].info.bytesize, out_buf);
      break;
    case MAP_FLOW_METER_UART_PARITY:
      u16_to_u8_big(uart_list[FLOW_METER_UART].info.parity, out_buf);
      break;
    case MAP_FLOW_METER_UART_STOP_BITS:
      u16_to_u8_big(uart_list[FLOW_METER_UART].info.stopbits, out_buf);
      break;
    case MAP_RTU_SLAVE:
      u16_to_u8_big(rtu.slave, out_buf);
      break;
    case MAP_RTU_UART_BAUD_H:
      u16_to_u8_big(uart_list[RTU_UART].info.baudrate>>16, out_buf);
      break;
    case MAP_RTU_UART_BAUD_L:
      u16_to_u8_big(uart_list[RTU_UART].info.baudrate&0xFFFF, out_buf);
      break;
    case MAP_RTU_UART_BYTE:
      u16_to_u8_big(uart_list[RTU_UART].info.bytesize, out_buf);
      break;
    case MAP_RTU_UART_PARITY:
      u16_to_u8_big(uart_list[RTU_UART].info.parity, out_buf);
      break;
    case MAP_RTU_UART_STOP_BITS:
      u16_to_u8_big(uart_list[RTU_UART].info.stopbits, out_buf);
      break;
    case MAP_AUTO_ADJ:
      u16_to_u8_big(flow_meter.auto_adj, out_buf);
      break;
    case MAP_SINGLE_ADJ_NO:
      u16_to_u8_big(actuator.single_adj_no, out_buf);
      break;
    case MAP_SCREEN_FLIP_TIME:
      u16_to_u8_big(lcd_page.flip_page_time, out_buf);
      break;
    case MAP_USR_NORMAL_PWD:
      u16_to_u8_big(0x0123, out_buf);
      break;
    case MAP_USR_TEST_PWD:
      u16_to_u8_big(0x1123, out_buf);
      break;
    case MAP_USR_SUP_PWD:
      u16_to_u8_big(0x2123, out_buf);
      break;
    case MAP_SAVE_CONF:
      u16_to_u8_big(flash.saved, out_buf);
      break;
    case MAP_REBOOT:
      u16_to_u8_big(0, out_buf);
      break;
    case MAP_MOTOR_RATIO:
      u16_to_u8_big(actuator.motor_ratio, out_buf);
      break;
    case MAP_MOTOR_TIME:
      u16_to_u8_big(actuator.motor_time, out_buf);
      break;
    case MAP_LPM_SWITCH:
      u16_to_u8_big(actuator.lpm_switch, out_buf);
      break;
    case MAP_MOTOR_REVERSE:
      u16_to_u8_big(motor.reverse, out_buf);
      break;
    case MAP_FLOW_POWER:
      u16_to_u8_big(FLOW_PWR_SWITCH_IS_ON, out_buf);
      break;
    case MAP_FLOW_REQ_TIME0:             ///< 无调节请求间隔
      u16_to_u8_big(flow_meter.req_time_0, out_buf);
      break;
    case MAP_FLOW_REQ_TIME1:             ///< 有调节请求间隔
      u16_to_u8_big(flow_meter.req_time_1, out_buf);
      break;
    case MAP_FLOW_MOTOR_WEIGHT_EDGE:
      u16_to_u8_big(flow_meter.weight_edge, out_buf);
      break;
    case MAP_PRECISION_RATIO:
      u16_to_u8_big(flow_meter.precision_ratio, out_buf);
      break;
    case MAP_H_VERSION:
      u16_to_u8_big(H_VERSION, out_buf);
      break;
    case MAP_S_VERSION:
      u16_to_u8_big(APP_VERSION, out_buf);
      break;
    case MAP_FLOW_COMPRO:
      u16_to_u8_big(flow_meter.compro, out_buf);
      break;
    case MAP_FLOW_COMMODE:
      u16_to_u8_big(flow_meter.commode, out_buf);
      break;
    case MAP_RTU_COMPRO:
      u16_to_u8_big(rtu.compro, out_buf);
      break;
    case MAP_RTU_COMMODE:
      u16_to_u8_big(rtu.commode, out_buf);
      break;
    default:
      *out_buf = 0xFF;
      *(out_buf+1) = 0xFF;
      return FALSE;
  }
  return TRUE;
}

u8 map_write_registers(u8 slave, u16 addr, u8 *in_buf) {
  UartInfo *info;
  switch(addr) {
    /** 中石化协议映射表 */
//    case MAP_PEC_DEVICE_CODE:
//      config.device_code = u8_to_u16_big(in_buf);
//      break;
//    case MAP_PEC_VENDOR_CODE:
//      config.vendor_code = u8_to_u16_big(in_buf);
//      break;
//    case MAP_PEC_PRODUCTION_YEAR:
//      config.production_year = u8_to_u16_big(in_buf);
//      break;
//    case MAP_PEC_PRODUCTION_MONTH:
//      config.production_month = u8_to_u16_big(in_buf);
//      break;
//    case MAP_PEC_PRODUCTION_DAY:
//      config.production_day = u8_to_u16_big(in_buf);
//      break;
//    case MAP_PEC_MODEL_H:
//      config.model_h = u8_to_u16_big(in_buf);
//      break;
//    case MAP_PEC_MODEL_M:
//      config.model_m = u8_to_u16_big(in_buf);
//      break;
//    case MAP_PEC_MODEL_L:
//      config.model_l = u8_to_u16_big(in_buf);
//      break;
//    case MAP_PEC_TUNING_RANGE:
//      config.tuning_range = u8_to_u16_big(in_buf);
//      break;
//    case MAP_PEC_SERIAL_NUMBER_H:
//      config.serial_number_h = u8_to_u16_big(in_buf);
//      break;
//    case MAP_PEC_SERIAL_NUMBER_L:
//      config.serial_number_l = u8_to_u16_big(in_buf);
//      break;
    case MAP_PEC_PROTOCOL:///< 
      config.protocol = u8_to_u16_big(in_buf);
      rtu.compro = (0 == config.protocol?1:0);
      break;
    case MAP_PEC_DEVICE_ADDR:
      actuator.slave = u8_to_u16_big(in_buf);
      break;
    case MAP_PEC_RTU_ADDR:
      rtu.slave = u8_to_u16_big(in_buf);
      break;
    case MAP_PEC_BAUD_RATE:
      uart_list[RTU_UART].info.baudrate = u8_to_u16_big(in_buf) * 1200;
      break;
    case MAP_PEC_CEILING_ALARM_ENABLE:
      config.ceiling_alarm_enable = u8_to_u16_big(in_buf);
      break;
    case MAP_PEC_LOWER_ALARM_ENABLE:
      config.lower_alarm_enable = u8_to_u16_big(in_buf);
      break;
    case MAP_PEC_INJECTION:
      config.injection = u8_to_u16_big(in_buf);
      flow_meter.daily_injection = (u32)(u16_2_fp32(config.injection, config.injection_digit) * 1000);
      break;
    case MAP_PEC_INJECTION_DIGIT:
      config.injection_digit = u8_to_u16_big(in_buf);
      break;
    case MAP_PEC_INJECTION_MAX:
      config.injection_max = u8_to_u16_big(in_buf);
      break;
    case MAP_PEC_INJECTION_UP_DELAY:
      config.injection_up_delay = u8_to_u16_big(in_buf);
      break;
    case MAP_PEC_INJECTION_MIN:
      config.injection_min = u8_to_u16_big(in_buf);
      break;
    case MAP_PEC_INJECTION_DOWN_DELAY:
      config.injection_down_delay = u8_to_u16_big(in_buf);
      break;
    case MAP_PEC_INJ_PRESSURE_RANGE_MAX:
      config.inj_pressure_range_max = u8_to_u16_big(in_buf);
      break;
    case MAP_PEC_INJ_PRESSURE_RANGE_MIN:
      config.inj_pressure_range_min = u8_to_u16_big(in_buf);
      break;
    case MAP_PEC_INJ_PRESSURE_RANGE_DIGIT:
      config.inj_pressure_range_digit = u8_to_u16_big(in_buf);
      break;
    case MAP_PEC_INJ_PRESSURE_MAX:
      config.inj_pressure_max = u8_to_u16_big(in_buf);
      break;
    case MAP_PEC_INJ_PRESSURE_UP_DELAY:
      config.inj_pressure_up_delay = u8_to_u16_big(in_buf);
      break;
    case MAP_PEC_INJ_PRESSURE_MIN:
      config.inj_pressure_min = u8_to_u16_big(in_buf);
      break;
    case MAP_PEC_INJ_PRESSURE_DOWN_DELAY:
      config.inj_pressure_down_delay = u8_to_u16_big(in_buf);
      break;
    case MAP_PEC_INJECTION_LESS_DIGIT:
      config.injection_less_digit = u8_to_u16_big(in_buf);
      break;
    case MAP_PEC_OPEN_ANGLE_DIGIT:
      config.open_angle_digit = u8_to_u16_big(in_buf);
      break;
    /** END 中石化协议映射表 */
    case MAP_AUTO_ADJ_NO:
      actuator.cumula_adj_no = u8_to_u16_big(in_buf);
      break;
    case MAP_HOUR:
      task_setsystime(bcd_to_u8(*(in_buf+1)), TIME_HOUR);
      break;
    case MAP_MINUTE:
      task_setsystime(bcd_to_u8(*(in_buf+1)), TIME_MINUTE);
      break;
    case MAP_SECOND:
      task_setsystime(bcd_to_u8(*(in_buf+1)), TIME_SECOND);
      break;
    case MAP_ANGLE:
      actuator.angle = u8_to_u16_big(in_buf);
      break;
    case MAP_INSTANT_FLOW_H:
      flow_meter.instant_flow = (flow_meter.instant_flow & 0xFFFF) | ((u32)u8_to_u16_big(in_buf)<<16);
      break;
    case MAP_INSTANT_FLOW_L:
      flow_meter.instant_flow = (flow_meter.instant_flow & 0xFFFF0000) | u8_to_u16_big(in_buf);
      break;
    case MAP_DAILY_INJECTION_H:
      flow_meter.daily_injection = ((u32)u8_to_u16_big(in_buf)<<16) | (flow_meter.daily_injection&0xFFFF);
      break;
    case MAP_DAILY_INJECTION_L:
      flow_meter.daily_injection = (flow_meter.daily_injection & 0xFFFF0000) | u8_to_u16_big(in_buf);
      break;
    case MAP_ACTUATOR_SLAVE:
      actuator.slave = u8_to_u16_big(in_buf);
      break;
    case MAP_LCD_LIGHT_TIME:
      lcd_page.light_time = u8_to_u16_big(in_buf);
      break;
    case MAP_OPENING_VALUE:
      actuator.open_angle = u8_to_u16_big(in_buf);
      break;
    case MAP_PRECISION:
      flow_meter.precision = u8_to_u16_big(in_buf);
      break;
    case MAP_FLOW_METER_TYPE:
      flow_meter.type = u8_to_u16_big(in_buf);
      break;
    case MAP_FLOW_METER_VER:
      flow_meter.version = u8_to_u16_big(in_buf);
      break;
    case MAP_FLOW_METER_SLAVE:
      flow_meter.slave = u8_to_u16_big(in_buf);
      break;
    case MAP_FLOW_METER_UART_BAUD_H:
      info = &uart_list[FLOW_METER_UART].info;
      info->baudrate = (info->baudrate&0xFFFF) | ((u32)u8_to_u16_big(in_buf)<<16);
      break;
    case MAP_FLOW_METER_UART_BAUD_L:
      info = &uart_list[FLOW_METER_UART].info;
      info->baudrate = (info->baudrate&0xFFFF0000) | u8_to_u16_big(in_buf);
      break;
    case MAP_FLOW_METER_UART_BYTE:
      info = &uart_list[FLOW_METER_UART].info;
      info->bytesize = u8_to_u16_big(in_buf);
      break;
    case MAP_FLOW_METER_UART_PARITY:
      info = &uart_list[FLOW_METER_UART].info;
      info->parity = u8_to_u16_big(in_buf);
      break;
    case MAP_FLOW_METER_UART_STOP_BITS:
      info = &uart_list[FLOW_METER_UART].info;
      info->stopbits = u8_to_u16_big(in_buf);
      break;
    case MAP_RTU_SLAVE:
      rtu.slave = u8_to_u16_big(in_buf);
      break;
    case MAP_RTU_UART_BAUD_H:
      info = &uart_list[RTU_UART].info;
      info->baudrate = (info->baudrate&0xFFFF) | ((u32)u8_to_u16_big(in_buf)<<16);
      break;
    case MAP_RTU_UART_BAUD_L:
      info = &uart_list[RTU_UART].info;
      info->baudrate = (info->baudrate&0xFFFF0000) | u8_to_u16_big(in_buf);
      break;
    case MAP_RTU_UART_BYTE:
      info = &uart_list[RTU_UART].info;
      info->bytesize = u8_to_u16_big(in_buf);
      break;
    case MAP_RTU_UART_PARITY:
      info = &uart_list[RTU_UART].info;
      info->parity = u8_to_u16_big(in_buf);
      break;
    case MAP_RTU_UART_STOP_BITS:
      info = &uart_list[RTU_UART].info;
      info->stopbits = u8_to_u16_big(in_buf);
      break;
    case MAP_AUTO_ADJ:
      flow_meter.auto_adj = u8_to_u16_big(in_buf);
      break;
    case MAP_SCREEN_FLIP_TIME:
      lcd_page.flip_page_time = u8_to_u16_big(in_buf);
      break;
    case MAP_USR_NORMAL_PWD:
      actuator.pwd_usr0 = u8_to_u16_big(in_buf);
      break;
    case MAP_USR_TEST_PWD:
      actuator.pwd_usr1 = u8_to_u16_big(in_buf);
      break;
    case MAP_USR_SUP_PWD:
      actuator.pwd_usr2 = u8_to_u16_big(in_buf);
      break;
    case MAP_SAVE_CONF:
      if(TRUE == *(in_buf + 1)) {
        flash.saved = FALSE;
        task_event_post(TASK_FLASH, EV_SAVE_CONFIG, 1000);
      }
      break;
    case MAP_REBOOT:
      if(TRUE == *(in_buf + 1)) {
        PMMCTL0 = PMMPW + PMMSWBOR;//电源控制复位
      }
      break;
    case MAP_MOTOR_REMOTE_CTL:
      if(TRUE == *(in_buf + 1)) {
        motor_ctl(TRUE, MOTOR_DIR_OPEN, actuator.motor_ratio, actuator.motor_time, MOTOR_ANGLE_POWER);
      } else {
        motor_ctl(TRUE, MOTOR_DIR_SHUT, actuator.motor_ratio, actuator.motor_time, MOTOR_ANGLE_POWER);
      }
      break;
    case MAP_MOTOR_ADJ:
      if(TRUE == *(in_buf + 1)) {
        motor_ctl(TRUE, MOTOR_DIR_SHUT, MOTOR_WAVE_DUTY_RATIO_DEF, MOTOR_ADJ_COUNT_DEF, MOTOR_ANGLE_ADJ);
      } else {
        motor_ctl(FALSE, MOTOR_DIR_OPEN, MOTOR_WAVE_DUTY_RATIO_DEF, MOTOR_ADJ_COUNT_DEF, MOTOR_ANGLE_ADJ);
      }
      break;
    case MAP_MOTOR_TEST:
      if(TRUE == *(in_buf + 1)) {
        motor_ctl(TRUE, MOTOR_DIR_SHUT, MOTOR_WAVE_DUTY_RATIO_DEF, MOTOR_ADJ_COUNT_DEF, MOTOR_TEST);
      } else {
        motor_ctl(FALSE, MOTOR_DIR_SHUT, MOTOR_WAVE_DUTY_RATIO_DEF, MOTOR_ADJ_COUNT_DEF, MOTOR_TEST);
      }
      break;
    case MAP_MOTOR_RATIO:
      actuator.motor_ratio = u8_to_u16_big(in_buf);
      break;
    case MAP_MOTOR_TIME:
      actuator.motor_time = u8_to_u16_big(in_buf);
      break;
    case MAP_LPM_SWITCH:
      if(TRUE == *(in_buf + 1)) {
        actuator.lpm_switch = TRUE;
      } else {
        actuator.lpm_switch = FALSE;
      }
      break;
    case MAP_MOTOR_REVERSE:
      if(TRUE == *(in_buf + 1)) {
        motor.reverse = TRUE;
      } else {
        motor.reverse = FALSE;
      }
      break;
    case MAP_FLOW_POWER:
      if(TRUE == *(in_buf + 1)) {
        FLOW_PWR_SWITCH_ON();
      } else {
        FLOW_PWR_SWITCH_OFF();
      }
      break;
    case MAP_FLOW_REQ_TIME0:             ///< 无调节请求间隔
      flow_meter.req_time_0 = u8_to_u16_big(in_buf);
      break;
    case MAP_FLOW_REQ_TIME1:             ///< 有调节请求间隔
      flow_meter.req_time_1 = u8_to_u16_big(in_buf);
      break;
    case MAP_FLOW_MOTOR_WEIGHT_EDGE:
      flow_meter.weight_edge = u8_to_u16_big(in_buf);
      if(flow_meter.weight_edge > FLOW_METER_WEIGHT_EDGE_MAX) {
        flow_meter.weight_edge = FLOW_METER_WEIGHT_EDGE_MAX;
      }
      break;
    case MAP_PRECISION_RATIO:
      flow_meter.precision_ratio = u8_to_u16_big(in_buf)&0xFF;
      break;
    case MAP_FLOW_COMPRO:
      flow_meter.compro = u8_to_u16_big(in_buf);
      break;
    case MAP_FLOW_COMMODE:
      flow_meter.commode = u8_to_u16_big(in_buf);
      break;
    case MAP_RTU_COMPRO:
      rtu.compro = u8_to_u16_big(in_buf);
      break;
    case MAP_RTU_COMMODE:
      rtu.commode = u8_to_u16_big(in_buf);
      break;
    default:
      return FALSE;
  }
  return TRUE;
}