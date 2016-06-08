/**
 * @file  flash.c
 * @brief flash驱动实现
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#include "flash.h"

u8 flash_tmp[FLASH_CODE_SEGMENT_SIZE];  ///< flash底层缓冲区

#if 0
void flash_erase_one_seg(u8 *addr) {
  while(BUSY == (FCTL3&BUSY));
  FCTL3 = FWKEY;                            // Clear Lock bit
  FCTL1 = FWKEY+ERASE;                      // Set Erase bit
  *(u16 *)addr = 0;                         // Dummy write to erase Flash seg
  while(BUSY == (FCTL3&BUSY));
  FCTL3 = FWKEY+LOCK;                       // Set LOCK bit
}
#endif

/**
 * @brief 写一个扇区数据到flash
 * @param[in] addr 扇区起始地址
 * @param[in] value 数据
 * @param[in] seg_szie 扇区大小
 * @return 无返回值
 */
void flash_write_one_seg(u8 *addr, u8 *value, u16 seg_szie) {
  u16 i;
  while(BUSY == (FCTL3&BUSY));
  FCTL3 = FWKEY;                            // Clear Lock bit
  FCTL1 = FWKEY+ERASE;                      // Set Erase bit
  *(u16 *)addr = 0;                         // Dummy write to erase Flash seg
  while(BUSY == (FCTL3&BUSY));
  FCTL1 = FWKEY+WRT;                        // Set WRT bit for write operation

  for (i = 0; i < seg_szie; i++)
  {
    *addr++ = *value++;                     // Write value to flash
    while(!(WAIT & FCTL3));
  }
  FCTL1 = FWKEY;                            // Clear WRT bit
  while(BUSY == (FCTL3&BUSY));
  FCTL3 = FWKEY+LOCK;                       // Set LOCK bit
}

#define flash_ramcpy memcpy  ///< 重定向copy函数
#if 0
void flash_ramcpy(u8 *dst, u8 *src, u32 len) {
  if(0 == len) {
    return;
  }
  u32 i;
  for(i = 0;i < len;++i) {
    *dst++ = *src++;
  }
}
#endif

/**
 * @brief 写flash(不带禁止中断)
 * @param[in] addr 扇区起始地址
 * @param[in] offset 地址偏移
 * @param[in] value 数据
 * @param[in] len 长度
 * @param[in] seg_size 扇区大小
 * @retval TRUE 成功
 * @retval FALSE 失败
 */
u8 flash_write(u8 *addr, u32 offset, u8 *value, u32 len, u16 seg_size) {
  if(0 == len) {
    return FALSE;
  }
  u16 i;
  //__disable_interrupt();                    // 5xx Workaround: Disable global
                                            // interrupt while erasing. Re-Enable
                                            // GIE if needed
  //u8 tmp[FLASH_SEGMENT_SIZE];
  u8 *flash_addr = addr + (offset/seg_size*seg_size);
  u32 count_left = offset%seg_size;
  u32 count_right = (offset + len)%seg_size;
  
  u32 seg_num = ((offset + len)/seg_size - offset/seg_size);
  
  flash_ramcpy(flash_tmp, flash_addr, seg_size);
  if(seg_size - count_left > len) {
    flash_ramcpy(flash_tmp + count_left, value, len);
    value += len;
  } else {
    flash_ramcpy(flash_tmp + count_left, value, seg_size - count_left);
    value += seg_size - count_left;
  }
  flash_write_one_seg(flash_addr, flash_tmp, seg_size);
  flash_addr += seg_size;
  
  for(i = 1;i < seg_num;++i) {
    flash_write_one_seg(flash_addr, value, seg_size);
    flash_addr += seg_size;
    value += seg_size;
  }
  
  if(0 != count_right && seg_size - count_left < len) {
    flash_ramcpy(flash_tmp, flash_addr, seg_size);
    flash_ramcpy(flash_tmp, value, count_right);
    flash_write_one_seg(flash_addr, flash_tmp, seg_size);
    flash_addr += seg_size;
    value += count_right;
  }
  //__enable_interrupt();
  return TRUE;
}