/**
 * @file  stdint.h
 * @brief 基本数据类型定义
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#ifndef STDINT_H_
#define STDINT_H_
typedef unsigned char           bool;    ///< 开关量
typedef unsigned char           uint8;   ///< 无符号8位整型变量
typedef unsigned char           u8;      ///< 无符号8位整型变量
typedef signed char             int8;    ///< 有符号8位整型变量
typedef unsigned short          uint16;  ///< 无符号16位整型变量
typedef unsigned short          u16;     ///< 无符号16位整型变量
typedef signed short            int16;   ///< 有符号16位整型变量
typedef unsigned long           uint32;  ///< 无符号32位整型变量
typedef unsigned long           u32;     ///< 无符号32位整型变量
typedef signed long             int32;    ///< 有符号32位整型变量
typedef unsigned long long      uint64;  ///< 有符号64位整型变量
typedef unsigned long long      u64;     ///< 有符号64位整型变量
typedef signed long long        int64;   ///< 无符号64位整型变量
typedef float                    fp32;    ///< 单精度浮点数(32位长度)
typedef double                   fp64;    ///< 双精度浮点数(64位长度)
#define NULL                      0        ///< 空指针

/**
 * @brief 布尔值
 */
enum {
  FALSE, ///< 假
  TRUE,  ///< 真
};

extern u8 bcd_to_u8(u8 bcd);
extern u16 u16_to_bcd(u16 n);
extern u16 bcd_to_u16(u16 bcd);
extern u8 u8_to_bcd(u8 n);
extern u64 u8_to_u64_big(u8 *value);
extern u32 u8_to_u32_big(u8 *value);
extern u16 u8_to_u16_big(u8 *value);
extern void u64_to_u8_big(u64 value, u8 *out_buf);
extern void u32_to_u8_big(u32 value, u8 *out_buf);
extern void u16_to_u8_big(u16 value, u8 *out_buf);
extern void fp32_to_u8_h_big(fp32 value, u8 *out_buf);
extern void fp32_to_u8_l_big(fp32 value, u8 *out_buf);
extern void fp32_to_u8_big(fp32 value, u8 *out_buf);
extern fp32 u8_to_fp32_big(u8 *value);

extern u32 fp32_2_u32(fp32 value, u8 digit);
extern u16 fp32_2_u16_h(fp32 value, u8 digit);
extern u16 fp32_2_u16_l(fp32 value, u8 digit);
extern u8 fp32_2_u8(fp32 value, u8 digit);
extern fp32 u16_2_fp32(u16 value, u8 digit);
#endif