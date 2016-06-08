/**
 * @file  task.c
 * @brief 系统任务模型实现
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#include "task.h"

//u64 task_timer_tick = 0; //任务调度当前时间ms
//u64 sys_timer_tick = 0;  //系统当前时间ms
Task task_list[TASK_MAX_NUM];

TaskTimer task_timer;

/**
 * @brief 任务调度
 * @return 无返回值
 */
void task_scheduling(void)
{
  while(1) {
    u8 i;
    for (i=0; i < TASK_MAX_NUM; ++i)       // 逐个任务时间处理
    {
      if (task_list[i].run && NULL != task_list[i].task_hook)  // 任务运行标志
      {
        task_list[i].task_hook();          // 运行任务
        task_list[i].run = TASK_STOP;      // 标志清0
      }
    }
    //P7OUT ^= BIT5;//测试
  }
}

/**
 * @brief 任务时钟初始化
 * @return 无返回值
 */
void task_timer_init( void )
{
  task_timer.status = TIMER_STATE_TASK_HANDLER;
  TB0CCR0 = TASK_TIMER_CYCLE_TIME;
  TB0CTL  |= MC__UP | TASSEL__ACLK | TACLR;
  TB0CCTL0 |= CCIE;
}

/**
 * @brief 任务定时器中断
 * @return 无返回值
 */
#pragma vector = TIMER0_B0_VECTOR
__interrupt void task_timer_isr(void) 
{
  u8 i;
  for (i=0; i < TASK_MAX_NUM; ++i)                       // 逐个任务时间处理
  {
    if (task_list[i].timer > 0)                      // 时间不为0
    {
      task_list[i].timer--;                          // 减去一个节拍
    } else {
      task_list[i].timer = task_list[i].itv_time;    // 恢复计时器值，从新下一次
      task_list[i].run = TASK_RUN;                   // 任务可以运行
    }
  }
  switch(task_timer.status) {
    case TIMER_STATE_EMPTY:
      TB0CCR0 = TASK_TIMER_EMPTY_CYCLE_TIME;
      task_timer.status = TIMER_STATE_EMPTY_HANDLER;
    case TIMER_STATE_TASK_HANDLER:
      task_timer.sys_tick += 5;
      task_timer.tick += 5;
      break;
    case TIMER_STATE_TASK:
      TB0CCR0 = TASK_TIMER_CYCLE_TIME;
      task_timer.status = TIMER_STATE_TASK_HANDLER;
    case TIMER_STATE_EMPTY_HANDLER:
      task_timer.tick += 1000;
      task_timer.sys_tick += 1000;
      P7OUT ^= BIT5;
      if(task_timer.tick - task_timer.time_a > task_timer.itv_time) {
        LOW_POWER_MODE_OFF();
      }
      break;
  }
}

/**
 * @brief 提交延时事件
 * @param[in] task 任务描述
 * @param[in] event 事件类型
 * @param[in] delay 事件延时(毫秒)
 * @retval TRUE 成功
 * @retval FALSE 失败
 */
u8 task_event_post(u8 task, u16 event, u64 delay) {
  task_list[task].evt_on = TRUE;
  task_list[task].evt = event;
  task_list[task].evt_time_a = task_timer.tick;
  task_list[task].evt_time_b = delay;
  return TRUE;
}

/**
 * @brief 获取当前事件
 * @param[in] task 任务描述
 * @param[out] event 当前事件
 * @retval TRUE 成功
 * @retval FALSE 失败
 */
u8 task_event_get(u8 task, u16 *event) {
  if(TRUE == task_list[task].evt_on 
     && task_timer.tick - task_list[task].evt_time_a >= task_list[task].evt_time_b)
  {
    *event = task_list[task].evt;
    task_list[task].evt_on = FALSE;
    return TRUE;
  } else {
    return FALSE;
  }
}
    
void task_setsystime(u64 tm, u8 opt) {
  u8 tmp0, tmp1;
  switch(opt) {
    case TIME_TIME:
      task_timer.sys_tick = tm;
      break;
    case TIME_DAY:
      break;
    case TIME_HOUR:
      tmp0 = task_timer.sys_tick/1000/3600%24;
      tmp1 = tm%24;
      task_timer.sys_tick += (tmp1 - tmp0)*1000L*3600;
      break;
    case TIME_MINUTE:
      tmp0 = task_timer.sys_tick/1000/60%60;
      tmp1 = tm%60;
      task_timer.sys_tick += (tmp1 - tmp0)*1000L*60;
      break;
    case TIME_SECOND:
      tmp0 = task_timer.sys_tick/1000%60;
      tmp1 = tm%60;
      task_timer.sys_tick += (tmp1 - tmp0)*1000L;
      break;
  }
}
