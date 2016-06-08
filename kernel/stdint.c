/**
 * @file  stdint.c
 * @brief 数据类型转换工具实现
 * @version 1.0
 * @author ygz
 * @date   2015.7.1
 */
#include "stdint.h"
#include <string.h>

/**
 * @brief 将整型数据转换为16位BCD格式
 * @param[in] n 需要转换的数据
 * @retval 转换后的BCD码
 */
u16 u16_to_bcd(u16 n) {
  if (n > 9999) {
    return 0;
  }
  return ((n/1000)<<12)|((n/100%10)<<8)|((n%100/10)<<4)|(n%10);
}

/**
 * @brief 将整型数据转换为8位BCD格式
 * @param[in] n 需要转换的数据
 * @retval 转换后的BCD码
 */
u8 u8_to_bcd(u8 n) {
  if (n > 99) {
    return 0;
  }
  return (((n%100/10)<<4)|(n%10));
}

/**
 * @brief 将BCD码转换为8位整型数据
 * @param[in] bcd 需要转换的BCD码
 * @retval 转换后的整型数据
 */
u8 bcd_to_u8(u8 bcd)
{
  return (bcd>>4)*10+(bcd&0x0F);
}

/**
 * @brief 将BCD码转换为16位整型数据
 * @param[in] bcd 需要转换的BCD码
 * @retval 转换后的整型数据
 */
u16 bcd_to_u16(u16 bcd)
{
  return ((u16)bcd_to_u8(bcd>>8))*100+bcd_to_u8(bcd&0xFF);
}

/**
 * @brief 将u8数组转换为u64整数,大端排序
 * @param[in] value 需要转换的u8数组
 * @retval 转换后的u64整数
 */
u64 u8_to_u64_big(u8 *value) {
  return value[7] | ((u16)value[6]<<8) | ((u32)value[5]<<16) | ((u32)value[4]<<24) | ((u64)value[3]<<32) | ((u64)value[2]<<40) | ((u64)value[1]<<48) | ((u64)value[0]<<56);
}

/**
 * @brief 将u8数组转换为u32整数,大端排序
 * @param[in] value 需要转换的u8数组
 * @retval 转换后的u32整数
 */
u32 u8_to_u32_big(u8 *value) {
  return value[3] | ((u16)value[2]<<8) | ((u32)value[1]<<16) | ((u32)value[0]<<24);
}

/**
 * @brief 将u8数组转换为u16整数,大端排序
 * @param[in] value 需要转换的u8数组
 * @retval 转换后的u16整数
 */
u16 u8_to_u16_big(u8 *value) {
  return value[1] | (u16)(value[0]<<8);
}

/**
 * @brief 将u64整数转换为u8数组,大端排序
 * @param[in] value 需要转换的u64整数
 * @param[out] out_buf 转换后的u8数组
 * @return 无返回值
 */
void u64_to_u8_big(u64 value, u8 *out_buf) {
  out_buf[0] = (value>>56)&0xFF;
  out_buf[1] = (value>>48)&0xFF;
  out_buf[2] = (value>>40)&0xFF;
  out_buf[3] = (value>>32)&0xFF;
  out_buf[4] = (value>>24)&0xFF;
  out_buf[5] = (value>>16)&0xFF;
  out_buf[6] = (value>>8)&0xFF;
  out_buf[7] = (value)&0xFF;
}

/**
 * @brief 将u32整数转换为u8数组,大端排序
 * @param[in] value 需要转换的u32整数
 * @param[out] out_buf 转换后的u8数组
 * @return 无返回值
 */
void u32_to_u8_big(u32 value, u8 *out_buf) {
  out_buf[0] = (value>>24)&0xFF;
  out_buf[1] = (value>>16)&0xFF;
  out_buf[2] = (value>>8)&0xFF;
  out_buf[3] = (value)&0xFF;
}

/**
 * @brief 将u16整数转换为u8数组,大端排序
 * @param[in] value 需要转换的u16整数
 * @param[out] out_buf 转换后的u8数组
 * @return 无返回值
 */
void u16_to_u8_big(u16 value, u8 *out_buf) {
  out_buf[0] = (value>>8)&0xFF;
  out_buf[1] = (value)&0xFF;
}

void fp32_to_u8_h_big(fp32 value, u8 *out_buf) {
  u8 tmp[4];
  memcpy(tmp, &value, sizeof(tmp));
  out_buf[0] = tmp[3];
  out_buf[1] = tmp[2];
}

void fp32_to_u8_l_big(fp32 value, u8 *out_buf) {
  u8 tmp[4];
  memcpy(tmp, &value, sizeof(tmp));
  out_buf[0] = tmp[1];
  out_buf[1] = tmp[0];
}

void fp32_to_u8_big(fp32 value, u8 *out_buf) {
  u8 tmp[4];
  memcpy(tmp, &value, sizeof(tmp));
  out_buf[0] = tmp[3];
  out_buf[1] = tmp[2];
  out_buf[2] = tmp[1];
  out_buf[3] = tmp[0];
}

fp32 u8_to_fp32_big(u8 *value) {
  u8 tmp[4];
  tmp[0] = value[3];
  tmp[1] = value[2];
  tmp[2] = value[1];
  tmp[3] = value[0];
  fp32 rc = 0.0f;
  memcpy(tmp, &rc, sizeof(fp32));
  return rc;
}

u32 fp32_2_u32(fp32 value, u8 digit) {
  u8 i;
  if(digit > 5) {
    return 0;
  }
  for(i=0;i < digit;++i) {
    value *= 10;
  }
  return (u32)value;
}

u16 fp32_2_u16_h(fp32 value, u8 digit) {
  u8 i;
  if(digit > 5) {
    return 0;
  }
  for(i=0;i < digit;++i) {
    value *= 10;
  }
  return ((u32)value)>>16;
}

u16 fp32_2_u16_l(fp32 value, u8 digit) {
  u8 i;
  if(digit > 5) {
    return 0;
  }
  for(i=0;i < digit;++i) {
    value *= 10;
  }
  return (u16)value;
}

u8 fp32_2_u8(fp32 value, u8 digit) {
  u8 i;
  if(digit > 5) {
    return 0;
  }
  for(i=0;i < digit;++i) {
    value *= 10;
  }
  return (u8)value;
}

fp32 u16_2_fp32(u16 value, u8 digit) {
  u8 i;
  if(digit > 5) {
    return 0;
  }
  fp32 v = (fp32)value;
  for(i=0;i < digit;++i) {
    v /= 10;
  }
  return v;
}