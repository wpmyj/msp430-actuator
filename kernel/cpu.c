/**
 * @file  cpu.c
 * @brief 系统cpu初始化，主要包含系统时钟初始化
 * @version 1.0
 * @author ygz
 * @date   2015.7.1
 */
#include "cpu.h"

/**
 * @brief 430低频时钟(32768hz时钟)的初始化
 * @return 无返回值
 */
void cpu_clock_low_init(void)
{    
  //初始化为32768晶振
  P7SEL |= (BIT0 | BIT1);                   //0x03; // Port7 select XT1
  //XT1启动,低功耗模式,最小电流功耗
  UCSCTL6 &= ~(XT1OFF + XTS + XT1DRIVE_0);  //XT1 On, XTS控制为低频晶振，XT1DRIVE_3晶振频率电流消耗模式                                        
  UCSCTL6 |= XCAP_3 ;                       //xt1低频模式下的内部电容 Internal load cap
}

/**
 * @brief 30内部时钟Dco的初始化(也就是fll锁相环的初始化)
 * @return 无返回值
 */
void cpu_clock_dco_init(void )
{
  
  UCSCTL3 |= SELREF__REFOCLK + FLLREFDIV_0; //  选择FLL参考源自内部的32768hz，REFO  Set DCO FLL reference = REFO 32768Hz
  __bis_SR_register(SCG0);                  //禁止锁相环 System clock generator 0. This bit, when set, turns off the FLL+ loop control
  UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
  UCSCTL1 = DCORSEL_5;                      // Select DCO range 32MHz operation
  UCSCTL2 = FLLD_1 + FLLN_A; //374          //(FLLN_A + 1) * 32768 = 12.288MHz
  // UCSCTL2 = FLLD_1 + 0xb5;         // 锁相环的分频为1  Set DCO Multiplier for 12MHz 
  // (N + 1) * FLLRef = Fdco
  // (0xb5(181) + 1) * 32768 = 5.963776MHz
  //(374 + 1) * 32768 = 12.288MHz
  
  __bic_SR_register(SCG0);          // Enable the FLL control loop
  
  __delay_cycles(DELAY_AFTER_FLL_CTL);
}

/**
 * @brief 等待430时钟部分的稳定
 * @return 无返回值
 */
void cpu_waitclock_stabilize(void)
{
  int i = 0;
  do
  {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG);//不管初始化那种晶振，所有的错误标志都需要清除
    SFRIFG1 &= ~OFIFG;                      // 特殊功能寄存器 Clear fault flags
    for(i = 0;i< 0xFFFF;i ++);              // Delay for Osc to stabilize
    
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag 
}

/**
 * @brief 430内部时钟源的选择，包括aclk mclk smclk时钟源的选择
 * @return 无返回值
 */
void cpu_clock_select(void)
{
  
  UCSCTL4 = SELA__XT1CLK | SELM__DCOCLKDIV;/* | SELS__DCOCLKDIV;*/  // Set ACLK
  //UCSCTL5 |= DIVA__2;                                        //2分频    最终ACLK时钟为6144000
}

/**
 * @brief CPU初始化
 * @return 无返回值
 */
void cpu_init(void)
{
  cpu_clock_low_init();
  cpu_clock_dco_init();      //倍频置NMhz
  cpu_waitclock_stabilize(); //必须在所有的时钟配置完毕后再进行等待查询，不能单独的对xt1或者dco进行等待查询
  cpu_clock_select();        //ACLKd的时钟源为XT1CLK，Selects the SMCLK source 为DCOCLKDIV，Selects the MCLK source 为DCOCLKDIV
}

/**
 * @brief 硬性延时
 * @param[in] m 毫秒
 * @return 无返回值
 */
void delayms(u64 m)         // 12MHz Xtal, close to ms value
{
  u64 i;
  for(i=0; i<m; i++) {
    DELAY_MS(1);
  }
}