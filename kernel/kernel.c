/**
 * @file  kernel.c
 * @brief 内核相关函数
 * @version 1.0
 * @author ygz
 * @date   2015.7.1
 */
#include "kernel.h"

/**
 * @brief 描述内核初始化流程
 * @return 无返回值
 */
void kernel_init() {
  cpu_init();
  task_timer_init();
#if 1
  WATCH_DOG_ENABLE();
#else
  WATCH_DOG_DISABLE();
#endif
}

/**
 * @brief 程序初始化前将内部看门狗关闭,防止大数组初始化时看门狗复位
 * @retval 1 成功
 */
int __low_level_init(void)
{
  /* Insert your low-level initializations here */
  //stop WDT
  WDTCTL = WDTPW + WDTHOLD;
  /*
  *Return value:
  *
  * 1 - Perform data segment initialization.
  * 0 - Skip data segment initialization.
  */
  return 1;
}