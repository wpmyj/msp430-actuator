/**
 * @file  uart.c
 * @brief 串口驱动
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#include "uart.h"

Uart uart_list[UART_MAX_NUM]; ///< 串口列表

#if 0
#pragma vector=USCI_A0_VECTOR  // 串口0中断
UART_ISR_CREATE(0)
#endif

#pragma vector=USCI_A1_VECTOR  // 串口1中断
UART_ISR_CREATE(1)

#pragma vector=USCI_A2_VECTOR  // 串口2中断
UART_ISR_CREATE(2)

#if 0
#pragma vector=USCI_A3_VECTOR  // 串口3中断
UART_ISR_CREATE(3)
#endif

/**
 * @brief 有效波特率列表
 * @param[in] baud 波特率
 * @retval 有效波特率
 */
u32 uart_get_baud(u32 baud) {
  if(baud <= 300) {
    return 300;
  } else if(baud <= 600) {
    return 600;
  } else if(baud <= 1200) {
    return 1200;
  } else if(baud <= 2400) {
    return 2400;
  } else if(baud <= 4800) {
    return 4800;
  } else if(baud <= 9600) {
    return 9600;
  } else if(baud <= 14400) {
    return 14400;
  } else if(baud <= 19200) {
    return 19200;
  } else if(baud <= 38400) {
    return 38400;
  } else if(baud <= 56000) {
    return 56000;
  } else if(baud <= 57600) {
    return 57600;
  } else if(baud <= 115200) {
    return 115200;
  } else {
    return 9600;
  }
}

/**
 * @brief 有效校验方式列表
 * @param[in] parity 效校验方式
 * @retval 有效验方式
 */
u8 uart_get_parity(u8 parity) {
  if(parity <= 'E') {
    return 'E';
  } else if(parity <= 'N') {
    return 'N';
  } else if(parity <= 'O') {
    return 'O';
  } else {
    return 'N';
  }
}

void uart_init() {
  RS485_PWR_SWITCH_SEL &= ~RS485_PWR_SWITCH_BIT;
  RS485_PWR_SWITCH_DIR |= RS485_PWR_SWITCH_BIT;
  RS485_PWR_SWITCH_ON();
  
  FLOW_PWR_SWITCH_SEL &= ~FLOW_PWR_SWITCH_BIT;
  FLOW_PWR_SWITCH_DIR |= FLOW_PWR_SWITCH_BIT;
  FLOW_PWR_SWITCH_ON();
  
  UART_INIT(RTU_UART, 9600, 8, 'N', 1);
  UART_INIT(FLOW_METER_UART, 9600, 8, 'N', 1);
}

void uart_rcv(u8 no, u8 data) {
  Uart *uart = uart_list + no;
  switch (uart->state)
  {
    case STATE_RX_IDLE:
        uart->rcv_pos = 0;
        uart->rcv_buf[uart->rcv_pos++] = data;
        uart->state = STATE_RX_RCV;
        if(RTU_UART == no) {
          task_event_post(TASK_RTU, EV_FRAME_RECEIVED, RTU_UART_DELAY);
        } else if(FLOW_METER_UART == no) {
          task_event_post(TASK_FLOW_METER, EV_FRAME_RECEIVED, FLOW_METER_UART_DELAY);
        }
        break;
    case STATE_RX_RCV:
        if(uart->rcv_pos < MAX_BUF_SIZE) {
          uart->rcv_buf[uart->rcv_pos++] = data;
        }
        if(RTU_UART == no) {
          task_event_post(TASK_RTU, EV_FRAME_RECEIVED, RTU_UART_DELAY);
        } else if(FLOW_METER_UART == no) {\
          task_event_post(TASK_FLOW_METER, EV_FRAME_RECEIVED, FLOW_METER_UART_DELAY);
        }
        break;
  }
}

#if 0
void uart_snd(u8 no) {
  Uart *uart = uart_list + no;
  switch (uart->state)
  {
    case STATE_TX_XMIT:
      if(uart->snd_pos < uart->snd_len) {
        *uart->txbuf = uart->snd_buf[uart->snd_pos];
        ++uart->snd_pos;
      } else {
        uart->snd_pos = 0;
        uart->snd_len = 0;
        if(RTU_UART == no) {
          DELAY_US(1000);
          UART_ENABLE(no, TRUE, FALSE);
          uart->state = STATE_RX_IDLE;
        } else if(FLOW_METER_UART == no) {
          DELAY_US(1000);
          UART_ENABLE(no, TRUE, FALSE);
          uart->state = STATE_RX_IDLE;
        }
      }
      break;
  }
}

void uart_enable(u8 no, u8 rx, u8 tx) {
  Uart *uart = uart_list + no;
  if(TRUE == rx) {\
    if(UART_1 == no){
      UART1_DIR_OUT &= ~UART1_DIR_BIT;
    } else if(UART_2 == no){
      UART2_DIR_OUT &= ~UART2_DIR_BIT;
    }
    *uart->ie |= UCRXIE;
  } else {
    *uart->ie &= ~UCRXIE;
  }
  if(TRUE == tx) {
    if(UART_1 == no){
      UART1_DIR_OUT |= UART1_DIR_BIT;
    }else if(UART_2 == no){\
      UART2_DIR_OUT |= UART2_DIR_BIT;
    }
    *uart->ie |= UCTXIE;
    *uart->ifg = UCTXIFG;
  } else {
    *uart->ie &= ~UCTXIE;
  }
}
#endif