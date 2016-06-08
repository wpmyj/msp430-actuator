/**
 * @file  flowmeter_task.c
 * @brief 流量计任务实现
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#include "flowmeter_task.h"
#include <stdlib.h>

FlowMeter flow_meter;  ///< 流量计任务属性
/**
 * @brief 定时处理(请求)
 * @return 无返回值
 */
void flowmeter_timer_handler();
/**
 * @brief 请求动作
 * @return 无返回值
 */
void flowmeter_req_action();
/**
 * @brief 数据处理动作
 * @param[in] in_buf 寄存器数据
 * @param[in,out] in_len 数据长度(寄存器数量*2)
 * @retval TRUE 成功
 * @retval FALSE 失败
 */
u8 flowmeter_modbus_data(u8 *in_buf, u8 in_len);

void flowmeter_data_action();

/**
 * @brief 流量计任务
 * @return 无返回值
 */
void flowmeter_task() {
  u16 event;
  u8 rc = 0;
  flowmeter_timer_handler();
  u8 *rep_buf;
  u16 rep_len;
  Uart *uart = &uart_list[FLOW_METER_UART];
  if(TRUE == task_event_get(TASK_FLOW_METER, &event))
  {
    switch (event)
    {
    case EV_READY:
      break;
    case EV_FRAME_RECEIVED:
      /* modbus */
      rc = modbus_handler(flow_meter.slave, flow_meter.commode, uart->rcv_buf, 
                          uart->rcv_pos, 
                          uart->snd_buf, 
                          &uart->snd_len);
      /* modbus 请求 */
      if(RTU_REQ == rc) {
        uart->state = STATE_TX_XMIT;
        UART_ENABLE(FLOW_METER_UART, FALSE, TRUE);
        if(TRUE == is_upgrade_pack(uart->rcv_buf, uart->rcv_pos)) {
          flow_meter.timeout_cnt = FLOW_PROTOCOL_MAX;
          flow_meter.time_a = task_timer.tick;
          //flow_meter.s_time_a = task_timer.tick;
          flow_meter.status = TRUE;
          ++flow_meter.com_count;
        }
        break;
      }
      if(RTU_SLAVE_ERR == rc) {
        break;
      }
      /* modbus 响应 */
      if(RTU_REP == rc) {
        rc = modbus_rep_handler(uart->rcv_buf, 
                                uart->rcv_pos, 
                                uart->snd_buf, 
                                &rep_buf, &rep_len);
        if(RTU_REP == rc) {
          if(FALSE == flowmeter_modbus_data(rep_buf, rep_len)) {
            //task_event_post_delay(TASK_FLOW_METER, EV_FRAME_SENT, 0);
          } else {
            flowmeter_data_action();
          }
        }
        flow_meter.timeout_cnt = 0;
        //flow_meter.s_time_a = task_timer.tick;
        flow_meter.status = TRUE;
        ++flow_meter.com_count;
        break;
      }
      
      /* 富马流量计协议 */
      rc = fuma_flow_handler(uart->rcv_buf, 
                          uart->rcv_pos, 
                          uart->snd_buf, 
                          &uart->snd_len);
      /* 富马流量计响应 */
      if(RTU_REP == rc) {
        flow_meter.type = FLOW_METER_TYPE_FUMA;
        flowmeter_data_action();
        flow_meter.timeout_cnt = 0;
        //flow_meter.s_time_a = task_timer.tick;
        flow_meter.status = TRUE;
        ++flow_meter.com_count;
        break;
      }
      
      /* 港仪流量计协议 */
      rc = gangyi_flow_handler(uart->rcv_buf, 
                          uart->rcv_pos, 
                          uart->snd_buf, 
                          &uart->snd_len);
      /* 港仪流量计响应 */
      if(RTU_REP == rc) {
        flow_meter.type = FLOW_METER_TYPE_GANGYI;
        flowmeter_data_action();
        flow_meter.timeout_cnt = 0;
        //flow_meter.s_time_a = task_timer.tick;
        flow_meter.status = TRUE;
        ++flow_meter.com_count;
        break;
      }
      uart->state = STATE_RX_IDLE;
      UART_ENABLE(FLOW_METER_UART, TRUE, FALSE);
      break;
    case EV_FRAME_SENT:
      //      modbus_read_registers_pack(2, 1, 3, 
      //                              uart_list[FLOW_METER_UART].snd_buf, 
      //                              &uart_list[FLOW_METER_UART].snd_len);
      //      modbus_write_register_pack(2, 1, 33, 
      //                              uart_list[FLOW_METER_UART].snd_buf, 
      //                              &uart_list[FLOW_METER_UART].snd_len);
      flowmeter_req_action();
      uart->state = STATE_TX_XMIT;
      UART_ENABLE(FLOW_METER_UART, FALSE, TRUE);
      break;
    case EV_TIMEOUT:
      if(FLOW_PROTOCOL_AUTO == flow_meter.compro) {
        ++flow_meter.protocol;
      }
      ++flow_meter.timeout_cnt;
      if(FLOW_PROTOCOL_MAX <= flow_meter.protocol) {
        flow_meter.protocol = FLOW_PROTOCOL_MODBUS;
      }
      if(flow_meter.timeout_cnt >= FLOW_PROTOCOL_MAX - 1) {
        flow_meter.timeout_cnt = 0;
        flow_meter.time_a = task_timer.tick;
        break;
      }
      task_event_post(TASK_FLOW_METER, EV_FRAME_SENT, 0);
      flow_meter.time_a = task_timer.tick;
      break;
    }
  }
}

void flowmeter_timer_handler() {
  if(task_timer.tick - flow_meter.time_a > flow_meter.itv_time) {
    flow_meter.req_type = FLOW_METER_INSTANT_FLOW_REQ;
    if(COM_MODE_MASTER == flow_meter.commode) {
      if(FLOW_PROTOCOL_AUTO != flow_meter.compro) {
        flow_meter.protocol = flow_meter.compro;
      }
      task_event_post(TASK_FLOW_METER, EV_FRAME_SENT, 0);
    }
    flow_meter.time_a = task_timer.tick;
  }
  if(task_timer.tick - flow_meter.s_time_a > FLOW_COM_TIME_OUT) {
    if(0 == flow_meter.com_count) {
      flow_meter.status = FALSE;
    } else {
      flow_meter.com_count = 0;
    }
    flow_meter.s_time_a = task_timer.tick;
  }
}

void flowmeter_req_action() {
  //u8 *values = uart_list[FLOW_METER_UART].snd_buf + WRITE_REGISTERS_POS;
  Uart *uart = &uart_list[FLOW_METER_UART];
  if(FLOW_METER_INSTANT_FLOW_REQ == flow_meter.req_type) {
    if(FLOW_PROTOCOL_MODBUS == flow_meter.protocol) {
      modbus_read_registers_pack(flow_meter.slave, 
                                  FLOW_METER_INSTANT_FLOW_ADDR, 
                                  FLOW_METER_INSTANT_FLOW_REG_LEN, 
                                  uart->snd_buf, 
                                  &uart->snd_len);
    } else if(FLOW_PROTOCOL_FUMA == flow_meter.protocol) {
      fuma_flow_req_pack(uart->snd_buf, 
                         &uart->snd_len);
    } else if(FLOW_PROTOCOL_GANGYI == flow_meter.protocol) {
      gangyi_flow_req_pack(uart->snd_buf, 
                           &uart->snd_len);
    } else if(FLOW_PROTOCOL_JINDA == flow_meter.protocol) {
      modbus_read_input_registers_pack(flow_meter.slave, 
                                  FLOW_JINDA_REG_ADDR, 
                                  FLOW_JINDA_REG_LEN,
                                  uart->snd_buf, 
                                  &uart->snd_len);
    }
    task_event_post(TASK_FLOW_METER, EV_TIMEOUT, 2000);
  }
}

u32 flow_mean_value(u32 *values, u16 len) {
  if(len < 1) {
    return 0;
  }
  if(len < 2) {
    return values[0];
  }
  if(len < 3) {
    return ((u64)values[0] + (u64)values[1])/2;
  }
  if(len > INSTANT_FLOW_TMP_NUM_MAX) {
    len = INSTANT_FLOW_TMP_NUM_MAX;
  }
  u16 i;
  u64 sum = 0;
  u16 max = values[0], min = values[0];
  for(i = 0;i < len;++i) {
    if(values[i]>max) {
      max=values[i];
    }
    if(values[i]<min) {
      min=values[i];
    }
    sum += values[i];
  }
  sum -= max + min;
  return (sum/(len-2));
}

void flowmeter_alarm_handler() {
  static u64 injection_less_time = 0;
  static fp32 cumula_flow_tmp = 0;
  static u16 flow_up_count = 0;
  static u16 flow_down_count = 0;
  u32 instant_flow = flow_meter.instant_flow/10;
  if(config.injection_max/24 < instant_flow) {
    ++flow_up_count;
    flow_down_count = 0;
    flow_meter.injection_down_alarm = 0;
  } else if(config.injection_min/24 > instant_flow) {
    ++flow_down_count;
    flow_up_count = 0;
    flow_meter.injection_up_alarm = 0;
  } else {
    flow_up_count = 0;
    flow_down_count = 0;
    flow_meter.injection_up_alarm = 0;
    flow_meter.injection_down_alarm = 0;
  }
  
  if(flow_up_count > config.injection_up_delay/flow_meter.req_time_0) {
    flow_meter.injection_up_alarm = 0x1;
  } else if(flow_down_count > config.injection_down_delay/flow_meter.req_time_0) {
    flow_meter.injection_down_alarm = 0x1;
  }
  flow_meter.injection_up_alarm &= config.ceiling_alarm_enable;
  flow_meter.injection_down_alarm &= config.ceiling_alarm_enable;
  
  if(0 == injection_less_time) {
    flow_meter.injection_less = 0;
    cumula_flow_tmp = flow_meter.cumula_flow;
    injection_less_time = task_timer.tick;
  } else {
    if(task_timer.tick > injection_less_time + 1000L * 3600) {
      fp32 injection_less = (flow_meter.cumula_flow - cumula_flow_tmp) - (fp32)flow_meter.daily_injection/24;
      flow_meter.injection_less = fp32_2_u16_l(injection_less, config.injection_less_digit);
      injection_less_time = task_timer.tick;
    }
  }
}

void flowmeter_data_action() {
  flowmeter_alarm_handler();
  if(FLOW_METER_ADJ_AUTO != flow_meter.auto_adj) { //自动调节判断
    flow_meter.req_type = FLOW_METER_INSTANT_FLOW_REQ;
    flow_meter.itv_time = flow_meter.req_time_0;
    return;
  }
  u32 dst_instant_flow = flow_meter.daily_injection/24;
  u32 instant_flow = flow_meter.instant_flow;
  u8 instant_index;
  fp32 precision_ratio = (fp32)flow_meter.precision_ratio/10;
  fp32 precision = (fp32)flow_meter.precision/100;
  precision = dst_instant_flow * precision * precision_ratio;
  if(dst_instant_flow > instant_flow + precision) {
    if(flow_meter.weight < 0) {
      flow_meter.weight = 0;
    } else {
#if TRUE==USE_MEAN_VALUE
      instant_index = flow_meter.weight;
      flow_meter.instant_flow_tmp[instant_index] = instant_flow;
#endif
      flow_meter.weight += 1;
    }
  } else if(instant_flow >  dst_instant_flow + precision) {
    if(flow_meter.weight > 0) {
      flow_meter.weight = 0;
    } else {
#if TRUE==USE_MEAN_VALUE
      instant_index = -flow_meter.weight;
      flow_meter.instant_flow_tmp[instant_index] = instant_flow;
#endif
      flow_meter.weight -= 1;
    }
  } else {
    flow_meter.weight = 0;
  }
  fp32 time_ratio = 1.0;
  u16 motor_time = actuator.motor_time;
  if(flow_meter.weight_edge < 1) {
    flow_meter.req_type = FLOW_METER_INSTANT_FLOW_REQ;
    return;
  }
  if(flow_meter.weight >= flow_meter.weight_edge) {
#if TRUE==USE_MEAN_VALUE
    instant_flow = flow_mean_value(flow_meter.instant_flow_tmp, flow_meter.weight_edge);
    flow_meter.mean_flow = instant_flow;
#endif
    time_ratio = (fp32)(dst_instant_flow - instant_flow)/dst_instant_flow;
    motor_time = (u16)(motor_time * time_ratio);
    if(motor_time < MOTOR_RUN_DURATION_TIME_MIN) {
      motor_time = MOTOR_RUN_DURATION_TIME_MIN;
    } else if(motor_time > actuator.motor_time) {
      motor_time = actuator.motor_time;
    }
    motor_ctl(TRUE, MOTOR_DIR_OPEN, actuator.motor_ratio, motor_time, MOTOR_ANGLE_POWER); //调大(开)
    flow_meter.itv_time = flow_meter.req_time_1;
    flow_meter.weight = 0;
    ++actuator.cumula_adj_no;
    if(actuator.cumula_adj_no > FLOW_METER_CUMULA_ADJ_NO_MAX) {
      actuator.cumula_adj_no = 0;
    }
  } else if(flow_meter.weight <= -flow_meter.weight_edge) {
#if TRUE==USE_MEAN_VALUE
    instant_flow = flow_mean_value(flow_meter.instant_flow_tmp, flow_meter.weight_edge);
    flow_meter.mean_flow = instant_flow;
#endif
    time_ratio = (fp32)(instant_flow - dst_instant_flow)/dst_instant_flow;
    motor_time = (u16)(motor_time * time_ratio);
    if(motor_time < MOTOR_RUN_DURATION_TIME_MIN) {
      motor_time = MOTOR_RUN_DURATION_TIME_MIN;
    } else if(motor_time > actuator.motor_time) {
      motor_time = actuator.motor_time;
    }
    motor_ctl(TRUE, MOTOR_DIR_SHUT, actuator.motor_ratio, motor_time, MOTOR_ANGLE_POWER);//调小(关)
    flow_meter.itv_time = flow_meter.req_time_1;
    flow_meter.weight = 0;
    ++actuator.cumula_adj_no;
    if(actuator.cumula_adj_no > FLOW_METER_CUMULA_ADJ_NO_MAX) {
      actuator.cumula_adj_no = 0;
    }
  } else {
    flow_meter.itv_time = flow_meter.req_time_0;
  }
  flow_meter.req_type = FLOW_METER_INSTANT_FLOW_REQ;
}

u8 flowmeter_modbus_data(u8 *in_buf, u8 in_len) {
  if(FLOW_METER_INSTANT_FLOW_REQ == flow_meter.req_type) {
    u8 rc = FALSE;
    if(FLOW_PROTOCOL_MODBUS == flow_meter.protocol) {
      rc = flow_fuma_modbus_rep_handler(in_buf, in_len);
    }
    else if(FLOW_PROTOCOL_JINDA == flow_meter.protocol) {
      rc = flow_jinda_modbus_rep_handler(in_buf, in_len);
    }
    return rc;
  }
  return FALSE;
}
