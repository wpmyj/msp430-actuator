/**
 * @file  flash.h
 * @brief flash驱动描述
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#ifndef FLASH_H_
#define FLASH_H_
#include "../kernel/kernel.h"
#include <string.h>

#define FLASH_INFO_ADDR               0x1880   ///< 信息区起始地址
#define FLASH_RUN_ADDR                0x5c00   ///< 主程序起始地址
#define FLASH_BACKUP_ADDR             0x25c00  ///< 备份区起始地址
#define FLASH_INFO_SEGMENT_SIZE       128      ///< 信息区扇区大小
#define FLASH_CODE_SEGMENT_SIZE       512      ///< 主flash扇区大小

extern u8 flash_tmp[FLASH_CODE_SEGMENT_SIZE];
extern u8 flash_write(u8 *addr, u32 offset, u8 *value, u32 len, u16 seg_szie);
extern void flash_write_one_seg(u8 *addr, u8 *value, u16 seg_szie);

/**
 * @brief 写flash(带禁止中断)
 * @param[in] ADDR 扇区起始地址
 * @param[in] OFFSET 地址偏移
 * @param[in] VALUE 数据
 * @param[in] LEN 长度
 * @param[in] SEG_SIZE 扇区大小
 * @return 无返回值
 */
#define FLASH_WRITE(ADDR, OFFSET, VALUE, LEN, SEG_SIZE)\
  __disable_interrupt();\
  flash_write(ADDR, OFFSET, VALUE, LEN, SEG_SIZE);\
  __enable_interrupt();

#endif