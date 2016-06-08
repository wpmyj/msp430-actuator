/**
 * @file  rtu_task.h
 * @brief rtu任务描述
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#ifndef RTU_TASK_H_
#define RTU_TASK_H_
#include "../kernel/kernel.h"
#include "../drivers/drivers.h"
#include "modbus.h"
#include "protocol.h"
#include <string.h>

#define RTU_COM_TIME_OUT        600000L ///< RTU通信超时10分钟

typedef struct Rtu {
  u8 slave;
  u8 status;
  u64 s_time_a;
  u8 com_count;
  u8 compro;               ///< 通信协议
  u8 commode;              ///< 通信模式
}Rtu;

extern Rtu rtu;
extern void rtu_task();

#endif