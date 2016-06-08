/**
 * @file  modbus.h
 * @brief modbus协议描述(modbus-rtu)
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#ifndef MODBUS_H_
#define MODBUS_H_
#include "../kernel/kernel.h"
#include "../drivers/drivers.h"

/**
 * @brief modbus解析状态
 */
enum {
  RTU_ERR,     ///< 协议解析错误
  RTU_REQ,     ///< 请求
  RTU_REP,     ///< 回应
  RTU_REQ_NR,  ///< 请求无回应
  RTU_SLAVE_ERR,
};

#define WRITE_REGISTERS_POS    7   ///< 读寄存器返回时,寄存器数据偏移

extern u8 modbus_handler(u8 slave, u8 master, u8 *in_buf, u16 in_len, u8 *out_buf, u16 *out_len);
extern u8 modbus_rep_handler(u8 *in_buf, u16 in_len, u8 *req_buf, 
                      u8 **out_buf, u16 *out_len);
extern u8 modbus_read_registers_pack(u8 slave, u16 addr, u16 len, 
                              u8 *out_buf, u16 *out_len);
extern u8 modbus_write_register_pack(u8 slave, u16 addr, u16 value, 
                              u8 *out_buf, u16 *out_len);

extern u8 modbus_write_registers_pack(u8 slave, u16 addr, u16 v_len,
                               u8 *out_buf, u16 *out_len);

extern u8 modbus_read_input_registers_pack(u8 slave, u16 addr, u16 len, 
                              u8 *out_buf, u16 *out_len);

#endif