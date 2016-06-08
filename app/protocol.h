/**
 * @file  protocol.h
 * @brief Ö´ÐÐÆ÷Ð­Òé
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include "../kernel/kernel.h"
#include "../drivers/drivers.h"
#include "modbus.h"
#include "app.h"
#include <stdio.h>

enum {
  RTU_COM_PRO_AUTO,
  RTU_COM_PRO_MODBUS_RTU,
  RTU_COM_PRO_OLD,
  RTU_COM_PRO_MAX,
};

enum {
  COM_MODE_SLAVE,
  COM_MODE_MASTER,
};

extern u8 gangyi_flow_handler(u8 *in_buf, u16 in_len, u8 *out_buf, u16 *out_len);
extern u8 gangyi_flow_req_pack(u8 *out_buf, u16 *out_len);
extern u8 fuma_flow_handler(u8 *in_buf, u16 in_len, u8 *out_buf, u16 *out_len);
extern u8 fuma_flow_req_pack(u8 *out_buf, u16 *out_len);
extern u8 rtu_old_handler(u8 *in_buf, u16 in_len, u8 *out_buf, u16 *out_len);
extern u8 flow_fuma_modbus_rep_handler(u8 *in_buf, u8 in_len);
extern u8 flow_jinda_modbus_rep_handler(u8 *in_buf, u8 in_len);
extern u16 sum16(u8 *buf, u16 len);
extern fp32 float_create(u32 value_i, u16 n);

#endif