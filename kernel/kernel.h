/**
 * @file  kernel.h
 * @brief 输出内核相关接口
 * @version 1.0
 * @author ygz
 * @date   2015.7.1
 */
#ifndef KERNEL_H_
#define KERNEL_H_

#include "cpu.h"
#include "stdint.h"
#include "task.h"
#include "watchdog.h"

extern void kernel_init();
#endif