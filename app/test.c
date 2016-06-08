#include "test.h"
extern u8 flowmeter_modbus_data(u8 *in_buf, u8 in_len);
extern void flowmeter_data_action();
void test() {
//  u8 in_buf[24] = "=02  12.56      0.567";
//  u8 in_buf2[71] = "20L334111000L334111000S300000000S300000000YEEEEEEYEEEEEEWEEEEEEWEEEEEED";
//  in_buf[21] = 0x00;
//  in_buf[22] = 0x00;
//  in_buf[23] = 0xFF;
//  u8 out_buf[300];
//  u16 out_len;
//  //gangyi_flow_handler(in_buf, 24, out_buf, &test_len);
//  gangyi_flow_req_pack(out_buf, &out_len);
//  gangyi_flow_handler(in_buf, 24, out_buf, &out_len);
//
//  fuma_flow_req_pack(out_buf, &out_len);
//  fuma_flow_handler(in_buf2, 71, out_buf, &out_len);
  
  //u8 *test_p = out_buf;
//  u8 in_buf[] = {0x01, 0x03, 0x0C, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xC6, 0xEA};
//  u8 out_buf[300];
//  u16 rep_len;
//  modbus_handler(in_buf, 
//                 sizeof(in_buf), 
//                 out_buf, 
//                 &rep_len);
//  flowmeter_modbus_data(in_buf, 0x0C);
//  flowmeter_data_action();
//  flowmeter_data_action();
//  flowmeter_data_action();
//  flowmeter_data_action();
//  flowmeter_data_action();
  //u8 abc[] = {0xCC, 0x01, 0x0B, 0x0F, 0x08, 0x15, 0x0B, 0x15, 0x18, 0x00, 0x00, 0x00, 0x00, 0x01, 0x3C, 0xEE};
//  u8 abc1[] = {0xCC, 0x01, 0x06, 0x01, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD9, 0xEE};
//  u8 abcd[300];
//  u16 abcl;
//  rtu_old_handler(abc1, 16, abcd, &abcl);
  u8 a[8] = {0x12, 0x34, 0x11, 0x00, 0x00, 0x43, 0x21, 0x11};
  flow_jinda_modbus_rep_handler(a, 8);
  return;
}