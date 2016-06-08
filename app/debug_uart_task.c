/**
 * @file  debug_uart_task.c
 * @brief 调试串口任务实现
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#if 0
#include "debug_uart_task.h"

/**
 * @brief 调试串口任务入口
 * @return 无返回值
 */
void debug_uart_task() {
  u16 event;
  if(TRUE == task_event_get(TASK_DEBUG_UART, &event))
  {
    switch (event)
    {
    case EV_READY:
      break;
    case EV_FRAME_RECEIVED:
      if(RTU_REQ == modbus_handler(uart_list[DEBUG_UART].rcv_buf, 
                                uart_list[DEBUG_UART].rcv_pos, 
                                uart_list[DEBUG_UART].snd_buf, 
                                &uart_list[DEBUG_UART].snd_len)) {
                                  uart_list[DEBUG_UART].state = STATE_TX_XMIT;
                                  UART_ENABLE(DEBUG_UART, FALSE, TRUE);
                                }
      else {
        uart_list[DEBUG_UART].state = STATE_RX_IDLE;
      }
      break;
    case EV_FRAME_SENT:
      break;
    }
  }
}
#endif