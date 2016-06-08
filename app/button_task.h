/**
 * @file  button_task.h
 * @brief °´Å¥ÈÎÎñÃèÊö
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#ifndef BUTTON_TASK_H_
#define BUTTON_TASK_H_
#include "../kernel/kernel.h"
#include "../drivers/drivers.h"
#include "lcd_task.h"

extern Button button_list[BUTTON_MAX_NUM];
extern void button_task();
  
#endif