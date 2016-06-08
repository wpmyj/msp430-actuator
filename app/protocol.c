#include "protocol.h"
/**
  执行器与港仪流量计间通讯
  2400,N,8,1
  执行器发送:5A 30 31 
  5A:字头
  30 31：地址01(流量计地址,执行器中是不可变值01)
  港仪流量计返回:
  3D 30 31 20 20 30 2E 30 30 30 20 20 20 20 20 30 2E 35 33 30 34 03 96 FF 
  3D :字头
  30 31 :地址01
  20 20 30 2E 30 30 30:瞬时流量, “  0.000” （20为空格，30为0,2E为小数点）
  20 20 20 20 20 30 2E 35 33 30 34：累计流量，“     0.5304”（20为空格，30为0,2E为小数点，35为5,33为3,34为4）
  03 96  为和效验位(两个字节)
  FF    为结束符
*/

u8 gangyi_flow_req_pack(u8 *out_buf, u16 *out_len) {
  //5A 30 31
  out_buf[0] = 'Z';
  sprintf((char *)(out_buf + 1), "%02d", flow_meter.slave%100);
  //out_buf[1] = '0';
  //out_buf[2] = '1';
  *out_len = 3;
  return RTU_REQ;
}

u8 gangyi_flow_handler(u8 *in_buf, u16 in_len, u8 *out_buf, u16 *out_len) {
  if(in_len < 24) {
    return RTU_ERR;
  }
  if('=' != in_buf[0] || 0xFF != in_buf[23]) {
    return RTU_ERR;
  }
  u16 sum16_tmp = sum16(in_buf, 21);
  if(sum16_tmp != (in_buf[21]<<8 | in_buf[22])) {
    return RTU_ERR;
  }
  u16 slave = 0;
  if(1 != sscanf((const char *)(in_buf + 1), "%02d", &slave)) {
    return RTU_ERR;
  }
  if(slave != flow_meter.slave) {
    return RTU_ERR;
  }
  fp32 cumula_flow;
  fp32 instant_flow;
  if(2 != sscanf((const char *)(in_buf + 3), "%f %f", &instant_flow, &cumula_flow)) {
    return RTU_ERR;
  }
  flow_meter.instant_flow = (u32)(instant_flow * 1000);
  flow_meter.cumula_flow = cumula_flow * 1000;
  return RTU_REP;
}

/*
  执行器与富马流量计间通讯
  9600,N,8,1
    
  执行器发送:B1 B0 ((流量计地址,执行器中是不可变值01))
   富马流量计返回:
  31 30 4C 33 33 34 31 31 31 30 30 30 4C 33 33 34 31 31 31 30 30 30 53 33 30 30 30 30 30 30 30 30 53 33 30 30 30 30 30 30 30 30 59 45 45 45 45 45 45 59 45 45 45 45 45 45 57 45 45 45 45 45 45 57 45 45 45 45 45 45 44 
  以上数据是ASCC码格式
  1  0  L  3  3  4  1  1  1  0  0  0  L  3  3  4  1  1  1  0  0  0  S  3  0  0  0  0  0  0  0  0  S  3  0  0  0  0  0  0  0  0  Y  E  E  E  E  E  E  Y  E  E  E  E  E  E  W  E  E  E  E  E  E  W  E  E  E  E  E  E  D
    
  31 30:地址01,
  4C 33 33 34 31 31 31 30 30 30 4C 33 33 34 31 31 31 30 30 30:
  “L”表示总流量，“3”表示三位小数位，“3 4 1 1 1 0 0 0 ”表示00011.143m3，
  53 33 30 30 30 30 30 30 30 30 53 33 30 30 30 30 30 30 30 30：
  “S”表示瞬时流量，“3”表示三位小数位，“0 0 0 0 0 0 0 0”表示00000.000 m3/h，
  59 45 45 45 45 45 45 59 45 45 45 45 45 45 57 45 45 45 45 45 45 57 45 45 45 45 45 45 
  “Y”表示压力，“E”表示压力没有数据，
  “W”表示温度，“E”表示压力没有数据，
  44“结束符
*/

u8 fuma_flow_req_pack(u8 *out_buf, u16 *out_len) {
  //B1 B0
  out_buf[0] = 0xB1;
  out_buf[1] = 0xB0;
  *out_len = 2;
  return RTU_REQ;
}

fp32 float_create(u32 value_i, u16 n) {
  if(n > 8) {
    n = 8;
  }
  u8 i;
  fp32 value = value_i;
  for(i = 0;i < n;++i) {
    value /= 10;
  }
  return value;
}

fp32 float_create_reverse(u8 plus, u8 n, u8 *data, u8 len) {
  if(len > 8) {
    len = 8;
  }
  if(n > 8) {
    n = 8;
  }
  u8 len_tmp = len;
  u8 buf_tmp[10] = "";
  fp32 value = 0.0;
  data += len;
  u8 i;
  for(i = 0;i < len_tmp;++i) {
    if(i == len - n) {
      buf_tmp[i] = '.';
      len_tmp += 1;
    } else {
      buf_tmp[i] = *(--data);
    }
  }
  if(1 != sscanf((const char *)buf_tmp, "%f", &value)) {
    value = 0;
  }
  if('-' == plus) {
    value = -value;
  }
  return value;
}

u8 int2_create_reverse(u8 *data) {
  u8 buf_tmp[3] = "";
  u16 value = 0;
  data += 2;
  u8 i;
  for(i = 0;i < 2;++i) {
    buf_tmp[i] = *(--data);
  }
  if(1 != sscanf((const char *)(buf_tmp), "%02d", &value)) {
    value = 0;
  }
  return value;
}

u8 fuma_flow_handler(u8 *in_buf, u16 in_len, u8 *out_buf, u16 *out_len) {
  if(in_len < 71) {
    return RTU_ERR;
  }
  if('D' != in_buf[70]) {
    return RTU_ERR;
  }
  u8 slave = int2_create_reverse(in_buf);
  if(slave != flow_meter.slave) {
    return RTU_ERR;
  }
  fp32 cumula_flow = 0;
  fp32 instant_flow = 0;
  u8 index = 2;
  if('L' == in_buf[index]) {
    cumula_flow = float_create_reverse('+', in_buf[index + 1] - '0', in_buf + index + 2, 8);
    index += 10;
    index += 10;
  }
  if('S' == in_buf[index]) {
    instant_flow = float_create_reverse('+', in_buf[index + 1] - '0', in_buf + index + 2, 8);
    index += 10;
    index += 10;
  }
  flow_meter.instant_flow = (u32)(instant_flow * 1000);
  flow_meter.cumula_flow = cumula_flow;
  return RTU_REP;
}

/*
  8002 RTU下发命令
  1、下发参数命令
   CC 01 02 00 00 03 E8 07 00 01 00 01 C2 EE 
  CC 01 02 00 00 27 10 08 00 01 00 01 0F EE
  CC 01 02 00 00 00 00 05 10 01 00 00 E5 EE
  CC:字头
  01：地址（执行器地址）
  02:功能码，下发参数
  00 00 03 E8：日配注量（单位是升，要换算为m3,03E8首先变换为十进制，除以1000后，为立方米的数值1，配置1m3，00 00 27 10设置日配注10m3）
  07:为调节精度-百分数，7%
  00：自动调节标志位，00为自动调节，其他为手动调节
  01：当日流量起始计算时间 一天中的几点钟
  00 00 00：保留位
  00 D7：效验位（从字头（包括字头）到效验位前的全部字节和）
  EE ：结束符（一个字节的结束符）
    
  正确回应：CC 01 02 00 01 01 00 D1 EE EE
    
    
  2、读取执行器的值，并下发时间
  CC 01 01 00 00 27 10 00 00 31 00 01 36 EE
  CC 01 01 01 00 27 10 00 00 31 00 01 37 EE
  CC:字头
  01：地址（执行器地址）
  01:功能码，读取执行器的值，并下发时间
  00 00 27 10：/日期时间---1970年的秒（）
  00 00: 保留位
  00 31：电池电压(此数据位3.5版执行器没有使用)
  00 00：保留位
  01 36：效验位（从字头（包括字头）到效验位前的全部字节和）
  EE ：结束符（一个字节的结束符）
  3、返回数据格式
  CC 01 01 00 1E 00 00 00 00 00 00 2B 84 00 00 00 00 1A 01 02 2E 21 3D 01 6B 00 1A 01 01 1A 00 00 9D F4 00 04 77 EE EE 

  CC 01 01 00 1E 00 00 00 00 00 00 2B 84 00 00 00 00 1F 01 02 00 21 3D 01 6B 00 44 00 01 1F 00 00 DD 7E 00 04 46 EE EE 

  CC 01 01 00 1E FF FF FF FF FF FF FF FF FF FF FF FF 1F 00 02 00 21 3D 01 6B 00 44 00 01 1F 00 01 88 9E 00 0F 56 EE EE （执行器与流量计不通讯时的返回数据）


  CC:字头
  01：地址（执行器地址）
  01:功能码，读取执行器的值，并下发时间
  00 1E：数据长度 1E转换为十进制为30
  00 00 00 00 00 00 2B 84：总流量---升，数据转换为十进制，在转换为m3
  00 00 00 00：流速---升/小时，数据转换为十进制，在转换为m3/h
  1F：单向连续调节次数，
  01：流量计通信状态，00表示与流量计通讯异常，01表示与流量计通讯正常
  02：流量计型号 1-一诺 2-富马? 3-大港1  4-大港2  5-蓝天 6-蓝天modbus（现阶段只使用了富马、大港1的型号）
  00：当前角度
  21 3D 01 6B：版本号 33，61，01，107，33硬件版本，107软件版本
  00 44：当前累计调节次数，数据转换为十进制68次
  00：自动调节标志位，00为自动调节，01为手动调节
  01：阀门关闭状态标志 这个是状态而不是设置命令 1--关闭? 0--调节（此位一直为1，）
  1F：单向连续调节角度
  00 01 88 9E：系统累计上电时间（秒）每次下电此数据清零
  00：保留
  0F 56：效验位（从字头（包括字头）到效验位前的全部字节和）
  EE EE：结束符（两个字节的结束符）
*/

u8 rtu_old_handler(u8 *in_buf, u16 in_len, u8 *out_buf, u16 *out_len) {
  if(in_len < 14) {
    return RTU_ERR;
  }
  if(0xCC != in_buf[0] || 0xEE != in_buf[in_len - 1]) {
    return RTU_ERR;
  }
  
  u8 slave = in_buf[1];
  if(slave != actuator.slave) {
    return RTU_ERR;
  }
  
  u16 sum16_tmp = sum16(in_buf, in_len - 3);
  if(sum16_tmp != (in_buf[in_len - 3]<<8 | in_buf[in_len - 2])) {
    return RTU_ERR;
  }
  
  u8 fun_no = in_buf[2];
  u8 *tmp_buf = in_buf + 3;
  if(0x01 == fun_no) {
    task_timer.sys_tick = 1000L * ((u64)u8_to_u32_big(tmp_buf) + TIME_ZONE * 3600L); // dongbaqu
    out_buf[0] = 0xCC;
    out_buf[1] = actuator.slave;
    out_buf[2] = 0x01;
    out_buf[3] = 0x00;
    out_buf[4] = 0x1E;
    tmp_buf = out_buf + 5;
    u64_to_u8_big((u64)(flow_meter.cumula_flow * 1000), tmp_buf);
    tmp_buf += sizeof(u64);
    u32_to_u8_big(flow_meter.instant_flow, tmp_buf);
    tmp_buf += sizeof(u32);
    *tmp_buf++ = actuator.single_adj_no;
    *tmp_buf++ = flow_meter.status;
    *tmp_buf++ = flow_meter.type;
    *tmp_buf++ = (u8)actuator.angle;
    u32_to_u8_big((H_VERSION<<16) | APP_VERSION, tmp_buf);
    tmp_buf += sizeof(u32);
    u16_to_u8_big(actuator.cumula_adj_no, tmp_buf);
    tmp_buf += sizeof(u16);
    *tmp_buf++ = flow_meter.auto_adj;
    *tmp_buf++ = 0;
    *tmp_buf++ = 0;
    u32_to_u8_big((u32)(task_timer.tick/1000), tmp_buf);
    tmp_buf += sizeof(u32);
    *tmp_buf++ = 0;
    u16 sum16_tmp = sum16(out_buf, 35);
    *tmp_buf++ = sum16_tmp>>8;
    *tmp_buf++ = sum16_tmp&0xFF;
    *tmp_buf++ = 0xEE;
    *tmp_buf++ = 0xEE;
    *out_len = 39;
  } else if(0x02 == fun_no) {
    u32 daily_injection = u8_to_u32_big(tmp_buf);
    tmp_buf += sizeof(u32);
    u8 precision = *tmp_buf;
    tmp_buf += sizeof(u8);
    u8 auto_adj = *tmp_buf;
    tmp_buf += sizeof(u8);
    u8 sun_rise_hour = *tmp_buf;
    if(FLOW_METER_ONLY_MANUAL == auto_adj) {
      auto_adj = flow_meter.auto_adj;
    }
    if(FLOW_METER_ONLY_MANUAL == flow_meter.auto_adj) {
      auto_adj = FLOW_METER_ONLY_MANUAL;
    }
    if(flow_meter.daily_injection != daily_injection
      || flow_meter.precision != precision
      || flow_meter.auto_adj != auto_adj
      || flow_meter.sun_rise_hour != sun_rise_hour) {
      flow_meter.daily_injection = daily_injection;
      flow_meter.precision = precision;
      flow_meter.auto_adj = auto_adj;
#if 0
      if(flow_meter.sun_rise_hour != sun_rise_hour) {
        actuator.day_time = 0;
      }
#endif
      flow_meter.sun_rise_hour = sun_rise_hour;
      flash.saved = FALSE;
      task_event_post(TASK_FLASH, EV_SAVE_CONFIG, 1000);
    }
    //tmp_buf += sizeof(u8);
    //CC 01 02 00 01 01 00 D1 EE EE
    out_buf[0] = 0xCC;
    out_buf[1] = actuator.slave;
    out_buf[2] = 0x02;
    out_buf[3] = 0x00;
    out_buf[4] = 0x01;
    out_buf[5] = 0x01;
    u16 sum16_tmp = sum16(out_buf, 6);
    out_buf[6] = sum16_tmp>>8;
    out_buf[7] = sum16_tmp&0xFF;
    out_buf[8] = 0xEE;
    out_buf[9] = 0xEE;
    *out_len = 10;
  } else if(0x06 == fun_no) {
    u32 daily_injection = 1000L * u8_to_u16_big(tmp_buf);
    tmp_buf += sizeof(u16);
    u8 precision = *tmp_buf;
    if(flow_meter.daily_injection != daily_injection
      || flow_meter.precision != precision) {
      flow_meter.daily_injection = daily_injection;
      flow_meter.precision = precision;
      
      flash.saved = FALSE;
      task_event_post(TASK_FLASH, EV_SAVE_CONFIG, 1000);
    }
//    out_buf[0] = 0xCC;
//    out_buf[1] = actuator.slave;
//    out_buf[2] = 0x0F;
//    out_buf[3] = 0x00;
//    out_buf[4] = 0x00;
//    u16 sum16_tmp = sum16(out_buf, 5);
//    tmp_buf = out_buf + 5;
//    *tmp_buf++ = sum16_tmp>>8;
//    *tmp_buf++ = sum16_tmp&0xFF;
//    *tmp_buf++ = 0xEE;
//    *tmp_buf++ = 0xEE;
//    *out_len = 9;
    return RTU_REQ_NR;
  } else if(0x0A == fun_no) { 
    u8 auto_adj = *tmp_buf;
    if(FLOW_METER_ONLY_MANUAL == auto_adj) {
      auto_adj = flow_meter.auto_adj;
    }
    if(FLOW_METER_ONLY_MANUAL == flow_meter.auto_adj) {
      auto_adj = FLOW_METER_ONLY_MANUAL;
    }
    if(auto_adj != flow_meter.auto_adj) {
      flow_meter.auto_adj = auto_adj;
      flash.saved = FALSE;
      task_event_post(TASK_FLASH, EV_SAVE_CONFIG, 1000);
    }
    return RTU_REQ_NR;
  } else if(0x0B == fun_no) {
    task_timer.sys_tick = 3600L * 24 * 360 * (*tmp_buf++);
    task_timer.sys_tick += 3600L * 24 * 30 * (*tmp_buf++);
    task_timer.sys_tick += 3600L * 24 * (*tmp_buf++);
    task_timer.sys_tick += 3600L * (*tmp_buf++);
    task_timer.sys_tick += 60L * (*tmp_buf++);
    task_timer.sys_tick += (*tmp_buf++);
    task_timer.sys_tick *= 1000L;
    out_buf[0] = 0xCC;
    out_buf[1] = actuator.slave;
    out_buf[2]=0x0B;
    out_buf[3] = 0;
    out_buf[4] = 21;
    u32 cumula_flow = (u32)flow_meter.cumula_flow;
    out_buf[5] = (cumula_flow>>16)&0xFF;
    out_buf[6] = (cumula_flow>>8)&0xFF;
    out_buf[7] = cumula_flow&0xFF;
    
    u16 cumula_flow_f = (u16)(((u16)(flow_meter.cumula_flow*1000))%1000*65.536);
    out_buf[8] = cumula_flow_f>>8;
    out_buf[9] = cumula_flow_f&0xFF;

    u16 instant = flow_meter.instant_flow/1000;
    out_buf[10] = (instant>>8)&0xFF;
    out_buf[11] = instant&0xFF;

    out_buf[12] = (u8)(flow_meter.instant_flow%1000*0.256);



    out_buf[13] = flow_meter.daily_injection>>24;
    out_buf[14] = 0; //单向连续调节次数
    u8 i;
    for (i=15;i<23;i++)
    {
        out_buf[i] = 0;
    }

    out_buf[18] = (u8)(actuator.angle);//角度 整数

    out_buf[23] = actuator.cumula_adj_no>>8;//调节次数

    out_buf[24] = actuator.cumula_adj_no&0xFF;//  调节次数

    out_buf[25] = flow_meter.auto_adj;//shou dong zi dong 手动是1 自动是0
    u16 sum16_tmp = sum16(out_buf, 26);
    out_buf[26] = sum16_tmp >> 8;
    out_buf[27] = sum16_tmp;
    out_buf[28] = 0xEE;
    out_buf[29] = 0xEE;
    *out_len = 30;
  } else {
    return RTU_ERR;
  }
  return RTU_REQ;//return RTU_ERR;
}

u8 flow_fuma_modbus_rep_handler(u8 *in_buf, u8 in_len) {
  if(FLOW_METER_INSTANT_FLOW_REG_LEN * 2 != in_len || NULL == in_buf) {
    return FALSE;
  }
  u32 cumula_flow_i = u8_to_u32_big(in_buf);
  in_buf += sizeof(u32);
  u16 cumula_flow_f = u8_to_u16_big(in_buf);
  in_buf += sizeof(u16);
  u32 instant_flow_i = u8_to_u32_big(in_buf);
  in_buf += sizeof(u32);
  u16 instant_flow_f = u8_to_u16_big(in_buf);
  fp32 cumula_flow = float_create(cumula_flow_i, cumula_flow_f);
  fp32 instant_flow = float_create(instant_flow_i, instant_flow_f);
  flow_meter.instant_flow = (u32)(instant_flow * 1000);
  flow_meter.cumula_flow = cumula_flow;
  return TRUE;
}

u8 flow_jinda_modbus_rep_handler(u8 *in_buf, u8 in_len) {
  if(FLOW_JINDA_REG_LEN * 2 != in_len || NULL == in_buf) {
    return FALSE;
  }
  u16 instant_flow_i = u8_to_u16_big(in_buf);
  in_buf += sizeof(u16);
  u8 instant_flow_f = (*in_buf)>>4;
  in_buf += sizeof(u8);
  u8 i;
  fp32 instant_flow = (fp32)instant_flow_i;
  for(i=0;i < instant_flow_f;++i) {
    instant_flow /= 10;
  }
  
  u32 cumula_flow_i = u8_to_u32_big(in_buf);
  in_buf += sizeof(u32);
  u8 cumula_flow_f = (*in_buf)>>4;
  in_buf += sizeof(u8);
  
  fp32 cumula_flow = (fp32)cumula_flow_i;
  for(i=0;i < cumula_flow_f;++i) {
    cumula_flow /= 10;
  }
  
  flow_meter.instant_flow = (u32)(instant_flow * 1000);
  flow_meter.cumula_flow = cumula_flow;
  return TRUE;
}

u16 sum16(u8 *buf, u16 len) {
  u16 sum = 0; /* sum */
  /* pass through message buffer */
  while (len--) {
    sum += *buf++; /* calculate the sum  */
  }
  return sum;
}