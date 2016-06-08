/**
 * @file  rtu_task.c
 * @brief rtu任务实现
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#include "rtu_task.h"

//static u8 *rep_buf;  ///< 返回数据索引(用于主模式)
//static u16 rep_len;  ///< 返回数据长度
Rtu rtu;

void rtu_timer_handler() {
  if(task_timer.tick - rtu.s_time_a > RTU_COM_TIME_OUT) {
    if(0 == rtu.com_count) {
      rtu.status = FALSE;
    } else {
      rtu.com_count = 0;
    }
    rtu.s_time_a = task_timer.tick;
  }
}

/**
 * @brief RTU处理任务
 * @return 无返回值
 */
void rtu_task() {
  u16 event;
  u8 rc = 0;
  Uart *uart = &uart_list[RTU_UART];
  //u8 *values = uart_list[RTU_UART].snd_buf + WRITE_REGISTERS_POS;
  if(TRUE == task_event_get(TASK_RTU, &event))
  {
    switch (event)
    {
    case EV_READY:
      break;
    case EV_FRAME_RECEIVED:
      /* modbus */
      rc = modbus_handler(actuator.slave, rtu.commode, uart->rcv_buf, 
                          uart->rcv_pos, 
                          uart->snd_buf, 
                          &uart->snd_len);
      /* modbus 请求 */
      if(RTU_REQ == rc) {
        if(RTU_COM_PRO_OLD == rtu.compro || COM_MODE_MASTER == rtu.commode) {
          if(FALSE == is_upgrade_pack(uart->rcv_buf, uart->rcv_pos)) {
            uart->state = STATE_RX_IDLE;
            UART_ENABLE(RTU_UART, TRUE, FALSE);
            break;
          }
        }
        uart->state = STATE_TX_XMIT;
        UART_ENABLE(RTU_UART, FALSE, TRUE);
        //rtu.s_time_a = task_timer.tick;
        rtu.status = TRUE;
        ++rtu.com_count;
        break;
      }
      
      if(RTU_SLAVE_ERR == rc) {
        break;
      }
      
      rc = rtu_old_handler(uart->rcv_buf, 
                           uart->rcv_pos, 
                           uart->snd_buf, 
                           &uart->snd_len);
      if(RTU_REQ == rc) {
        uart->state = STATE_TX_XMIT;
        UART_ENABLE(RTU_UART, FALSE, TRUE);
        //rtu.s_time_a = task_timer.tick;
        rtu.status = TRUE;
        ++rtu.com_count;
        break;
      }
      uart->state = STATE_RX_IDLE;
      UART_ENABLE(RTU_UART, TRUE, FALSE);
      break;
    }
  }
  rtu_timer_handler();
}