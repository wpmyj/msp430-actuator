/**
 * @file  adc.c
 * @brief adc驱动实现
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#include "adc.h"

u16 ad_channels[AD_CHANNEL_MAX_NUM];  ///< ad通道

/**
 * @brief adc驱动初始化
 * @return 无返回值
 */
void adc12_init() {
  ANGLE_R_AD_SEL |= ANGLE_R_AD_BIT;
  AD_24V_SEL     |= AD_24V_BIT;
  REFCTL0 = REFMSTR | REFVSEL_0 | REFON;
  ADC12CTL0 = ADC12ON | ADC12MSC | ADC12SHT0_8;
                                               
  ADC12CTL1 = ADC12SHP | ADC12CONSEQ_1;
  ADC12MCTL7 = ADC12INCH_7 | ADC12SREF_1;
  ADC12MCTL15 = ADC12INCH_15 | ADC12SREF_1 | ADC12EOS;
  ADC12IE = ADC12IE15;
  ADC12CTL0 |= ADC12ENC;
  //ADC12CTL0 |= ADC12SC;
}

/**
 * @brief adc开始转换
 * @return 无返回值
 */
void adc12_start() {
  ADC12CTL0 |= ADC12SC;
}

/**
 * @brief adc中断
 * @return 无返回值
 */
#pragma vector=ADC12_VECTOR
__interrupt void adc12_isr(void)
{
  switch(__even_in_range(ADC12IV,36))
  {
#if 0
  case  0:break;                           // Vector  0:  No interrupt
  case  2:break;                           // Vector  2:  ADC overflow
  case  4:break;                           // Vector  4:  ADC timing overflow
  case  6:break;                           // Vector  6:  ADC12IFG0
  case  8:break;                           // Vector  8:  ADC12IFG1
  case 10:break;                           // Vector 10:  ADC12IFG2
  case 12:break;                           // Vector 12:  ADC12IFG3
  case 14:break;                           // Vector 14:  ADC12IFG4
  case 16:break;                           // Vector 16:  ADC12IFG5
  case 18:break;                           // Vector 18:  ADC12IFG6
  case 20:break;                           // Vector 20:  ADC12IFG7
  case 22:break;                           // Vector 22:  ADC12IFG8
  case 24:break;                           // Vector 24:  ADC12IFG9
  case 26:break;                           // Vector 26:  ADC12IFG10
  case 28:break;                           // Vector 28:  ADC12IFG11
  case 30:break;                           // Vector 30:  ADC12IFG12
  case 32:break;                           // Vector 32:  ADC12IFG13
  case 34:break;                           // Vector 34:  ADC12IFG14
#endif
  case 36:
    ad_channels[AD_CHANNEL_ANGLE_R]=ADC12MEM7;
    ad_channels[AD_CHANNEL_24V]=ADC12MEM15;
    break;                                  // Vector 36:  ADC12IFG15
  default:break; 
  }
}