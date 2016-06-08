/**
 * @file  lcd_task.c
 * @brief lcd任务实现
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#include "lcd_task.h"
#include "app.h"
#include <stdarg.h>
LcdPage lcd_page;          ///< lcd属性
/**
 * @brief 主页面显示
 * @return 无返回值
 */
void display_main_page();
/**
 * @brief 副0界面显示
 * @return 无返回值
 */
void display_sub0_page();
/**
 * @brief 副1界面显示
 * @return 无返回值
 */
void display_sub1_page();
/**
 * @brief 登陆界面显示
 * @return 无返回值
 */
void display_login_page();
/**
 * @brief 设置界面显示
 * @return 无返回值
 */
void display_setting_page();
/**
 * @brief 屏幕刷新
 * @return 无返回值
 */
void lcd_screen_handler();
/**
 * @brief 状态栏显示
 * @return 无返回值
 */
void display_status_bar();
/**
 * @brief 界面刷新
 * @return 无返回值
 */
void display_pages();
/**
 * @brief 打印普通标签到屏幕
 * @param[in] x 水平位置
 * @param[in] y 垂直位置
 * @param[in] item 选项卡
 * @param[in] font 字体大小
 * @param[in] format 字体类型
 * @param[in] ... 字体数据索引
 * @return 无返回值
 * @note format w 汉字 d 十进制数据 h 十六进制数据 f 浮点型数据(三位有效数字)
 */
void lcd_label(u8 x, u8 y, u8 item, u8 font, const u8 *format, ...);
/**
 * @brief 打印版本标签到屏幕
 * @param[in] x 水平位置
 * @param[in] y 垂直位置
 * @param[in] version 版本信息
 * @return 无返回值
 */
void lcd_version(u8 x, u8 y, u16 version);
/**
 * @brief 打印串口标签到屏幕
 * @param[in] x 水平位置
 * @param[in] y 垂直位置
 * @param[in] item 选项卡
 * @param[in] baud 波特率
 * @param[in] byte_size 数据位
 * @param[in] parity 校验方式
 * @param[in] stopbits 停止位
 * @return 无返回值
 */
void display_uart_para(u8 x, u8 y, u8 item, u32 baud, u8 byte_size, u8 parity, u8 stopbits, u8 compro, u8 commode);

/* LCD任务 */
/**
 * @brief LCD任务
 * @return 无返回值
 */
void lcd_task() {
  u16 event;
  if(TRUE == task_event_get(TASK_LCD, &event))
  {
    switch (event)
    {
    case EV_READY:
      break;
    case EV_FRAME_RECEIVED:
      lcd_clear_data();
      lcd_page.frame_time_b = 0;
      if(0 == lcd_page.light_time) {
        //lcd_light_on(FALSE);
      } else {
        lcd_page.light_time_a = task_timer.tick;
        lcd_light_on(TRUE);
      }
      break;
    }
  }
  lcd_screen_handler();
}


void lcd_screen_handler(){
  if(task_timer.tick - lcd_page.frame_time_b > 231) {
    display_pages();
    lcd_page.frame_time_b = task_timer.tick;
  }
  if(task_timer.tick - lcd_page.frame_time_a > lcd_page.frame_itv_time) {
    lcd_display_screen();
    lcd_page.frame_time_a = task_timer.tick;
  }
  if(1 != lcd_page.light_time && task_timer.tick - lcd_page.light_time_a > (u32)(lcd_page.light_time * 1000)) {
    lcd_light_on(FALSE);
    lcd_page.light_time_a = 0xFFFFFFFFFFFFFFFF;
  }
}
#if 0
void lcd_test_page() {
  u8 i, j;
  for(i = 0;i < 4;++i) {
    for(j = 0;j < 16;++j) {
      lcd_post_word(j*8, i*12 + j, FONT_NUM_WIDTH, FONT_NUM_HIGHT, font_num[(i*12 + j + lcd_page.page)%10], i%2);
    }
  }
  lcd_post_word(116, 0, FONT_WCHAR_WIDTH, FONT_WCHAR_HIGHT, font_wchar[lcd_page.page%10], FALSE);
  lcd_post_word(0, 52, FONT_WCHAR_WIDTH, FONT_WCHAR_HIGHT, font_wchar[(lcd_page.page+1)%10], TRUE);
}
#endif

void display_pages() {
  switch(lcd_page.page) {
    case PAGE_MAIN:
      display_main_page();
      break;
    case PAGE_SUB0:
      display_sub0_page();
      break;
    case PAGE_SUB1:
      display_sub1_page();
      break;
    case PAGE_LOGIN:
      display_login_page();
      break;
    case PAGE_SETTING:
      display_setting_page();
      break;
  }
}

void display_status_bar() {
  //lcd_test_page();
  u8 x_offset = 6 + lcd_page.x;
  u8 x = x_offset, y = lcd_page.y;
  u8 d_tmp[2];
  d_tmp[0] = (u32)actuator.voltage/10;
  if(d_tmp[0]) {
    lcd_post_word(x, y, 5, 10, font_num_10x10[d_tmp[0]], FALSE);x+=5;
    d_tmp[1] = (u32)actuator.voltage%10;
    lcd_post_word(x, y, 5, 10, font_num_10x10[d_tmp[1]], FALSE);x+=5;
    lcd_post_word(x, y, 5, 10, letter_space10, FALSE);x+=5;
  } else {
    d_tmp[0] = (u32)actuator.voltage%10;
    lcd_post_word(x, y, 5, 10, font_num_10x10[d_tmp[0]], FALSE);x+=5;
    d_tmp[1] = (u8)((fp32)(actuator.voltage - (u32)actuator.voltage)*10)%10;
    lcd_post_word(x, y, 5, 10, dot_10, FALSE);x+=5;
    lcd_post_word(x, y, 5, 10, font_num_10x10[d_tmp[1]], FALSE);x+=5;
  }
  //d_tmp[1] = (u8)((fp32)(actuator.voltage - (u32)actuator.voltage)*10)%10;
  lcd_post_word(x, y, 5, 10, letter_V10, FALSE);x+=10;
  
  x = x_offset + 25;
  lcd_label(x, y, 0xFF, 10, "d", actuator.cumula_adj_no);
  x = x_offset + 50;
  const u8 *rtu_s = wchar_com_err;
  const u8 *flow_meter_s = wchar_com_err;
  if(TRUE == rtu.status) {
    rtu_s = wchar_com_ok;
  }
  if(TRUE == flow_meter.status) {
    flow_meter_s = wchar_com_ok;
  }
  lcd_label(x, y, 0xFF, 10, "ww", rtu_s, flow_meter_s);
  
  x = x_offset + 75;
  lcd_label(x, y, 0xFF, 10, "t", task_timer.sys_tick);
}
  
void display_main_page() {
  display_status_bar();
  u8 x_offset = 24 + lcd_page.x;
  u8 x = x_offset, y = lcd_page.y + 15;
  /* 角度 */
  lcd_label(x, y, 0xFF, 14, "wwfc", jiao_14, du_14, actuator.angle_tmp, letter_du);
  
  /* 流速 */
  x=x_offset;y+=16;
  //lcd_label(x, y, 0xFF, 14, "wwd", liu_14, su_14, (u16)(flow_meter.instant_flow/1000));x += 61;
  lcd_label(x, y, 0xFF, 14, "wwf", liu_14, su_14, (fp32)flow_meter.instant_flow/1000);x += 61;
  lcd_label(x, y, 0xFF, 12, "ww", bmp_m3, bmp_p_h);
  
  //lcd_label(x, y, 0xFF, 14, "wwdc", liu_14, su_14, ad_channels[AD_CHANNEL_ANGLE_R], letter_M);
  
  /* 日配注入 */
  x=x_offset;y+=16;
#if 1
  lcd_label(x, y, 0xFF, 14, "wwd", pei_14, zhu_14, (u16)(flow_meter.daily_injection/1000));x += 61;
#else
  lcd_label(x, y, 0xFF, 14, "wwf", pei_14, zhu_14, (fp32)flow_meter.mean_flow/1000);x += 61;
#endif
  lcd_label(x, y, 0xFF, 12, "ww", bmp_m3, bmp_p_d);
}

void display_sub0_page() {
  display_status_bar();
  u8 x_offset = 3 + lcd_page.x;
  u8 x = x_offset, y = lcd_page.y + 16;
  u8 x1_offset = x_offset + 62;
  /* 地址 */
  lcd_label(x, y, 0xFF, 12, "wwd", di_12, zhi_12, actuator.slave);
  /* 背光 */
  x = x1_offset;
  lcd_label(x, y, 0xFF, 12, "wwdc", bei_12, guang_12, lcd_page.light_time, letter_s);
//  /* 开度 */
//  x=x_offset;y+=16;
//  lcd_label(x, y, 0xFF, 12, "wwd", kai_12, du_12, actuator.open_angle);
  /* 通信 */
  x=x_offset;y+=16;
  lcd_label(x, y, 0xFF, 12, "wwh", tong_12, xin_12, rtu.com_count<<8 | flow_meter.com_count);
  /* 精度 */
  x = x1_offset;
  lcd_label(x, y, 0xFF, 12, "wwdc", jing_12, du_12, flow_meter.precision, letter_percent);
  /* 累注 */
  x=x_offset;y+=16;
  lcd_label(x, y, 0xFF, 12, "ww", lei_12, zhu_12);
  x += 28;
  char tmp[21] = "";
  u64 cumula_flow_i = (u64)flow_meter.cumula_flow;
  u16 cumula_flow_f = (u16)(1000 * (flow_meter.cumula_flow - (fp32)cumula_flow_i));
  u8 rc = sprintf(tmp, "%011lld.%03d", cumula_flow_i%100000000000L, cumula_flow_f);
  if(15 != rc) {
    return;
  }
  u8 i;
  for(i = 0;i < rc;++i) {
    if('.' == tmp[i]) {
      lcd_post_word(x, y, 6, 12, dot, FALSE);x+=6;
    } else {
      lcd_post_word(x, y, 6, 12, font_num[tmp[i] - '0'], FALSE);x+=6;
    }
  }
  //x+=2;
  //lcd_label(x, y, 0xFF, 12, "c", letter_L);
  //lcd_label(x, y, 0xFF, 12, "wwd", lei_12, zhu_12, (u16)(flow_meter.cumula_flow/1000));
}

void display_sub1_page() {
  display_status_bar();
  u8 x_offset = 5 + lcd_page.x;
  u8 x = x_offset, y = lcd_page.y + 16;
  x += 27;
  const u8 *flow_name_0 = fu_12;
  const u8 *flow_name_1 = ma_12;
  if(FLOW_METER_TYPE_GANGYI == flow_meter.type) {
    flow_name_0 = gang_12;
    flow_name_1 = yi_12;
  }
  lcd_label(x, y, 0xFF, 12, "wwwww", flow_name_0, flow_name_1, liu_12, liang_12, ji_12);
//  /* 流量计版本号 */
//  x=x_offset + 75;
//  lcd_version(x, y, flow_meter.version);
  /* 流量计串口参数 */
  x=x_offset + 7;y+=16;
  lcd_label(x, y, 0xFF, 12, "vvvv", letter_F, letter_L, letter_O, letter_W);x += 28;
  display_uart_para(x, y, 0xFF, 
                    uart_list[FLOW_METER_UART].info.baudrate,
                    uart_list[FLOW_METER_UART].info.bytesize, 
                    uart_list[FLOW_METER_UART].info.parity, 
                    uart_list[FLOW_METER_UART].info.stopbits,
                    flow_meter.compro, flow_meter.commode);
  /* RTU串口参数 */
  x=x_offset + 10;y+=16;
  lcd_label(x, y, 0xFF, 12, "vvv", letter_R, letter_T, letter_U);x += 25;
  display_uart_para(x, y, 0xFF, 
                    uart_list[RTU_UART].info.baudrate,
                    uart_list[RTU_UART].info.bytesize, 
                    uart_list[RTU_UART].info.parity, 
                    uart_list[RTU_UART].info.stopbits,
                    rtu.compro, rtu.commode);
}

void lcd_password(u8 x, u8 y, u8 index, u8 *pwd, u8 len, u8 v) {
  u8 i;
  for(i = 0;i < len;++i) {
    if(index == i) {
      lcd_post_word(x, y, 6, 12, font_num[pwd[i]%0x10], TRUE);x+=8;
    } else if(v) {
      lcd_post_word(x, y, 6, 12, font_num[pwd[i]%0x10], FALSE);x+=8;
    } else {
      lcd_post_word(x, y, 6, 12, letter_star, FALSE);x+=8;
    }
  }
}

void display_login_page() {
  u8 x_offset = 35 + lcd_page.x;
  u8 x = x_offset, y = lcd_page.y + 16;
  lcd_label(x, y, 0xFF, 12, "wwwww", qing_12, shu_12, ru_12, mi_12, m_ma_12);
  
  x=x_offset + 13;y+=16;
  lcd_password(x, y, lcd_page.status - ITEM_FOCUS_FIRST, actuator.pwd_tmp, 4, FALSE);
}

void display_setting_page() {
  u8 x_offset = 3 + lcd_page.x;
  u8 x = x_offset, y = lcd_page.y + 0;
  u8 x1_offset = x_offset + 62;
  //u8 focus = FALSE;
  if(lcd_page.hover_item <= ITEM_REBOOT) {
    /* 地址 */
    lcd_label(x, y, ITEM_ADDR, 12, "wwd", di_12, zhi_12, actuator.slave);
    x += 27;
    /* 背光 */
    x = x1_offset;
    lcd_label(x, y, ITEM_BACK_LIGHT, 12, "wwdc", bei_12, guang_12, lcd_page.light_time, letter_s);
    
    /* 精度 */
    x = x_offset;y+=16;
    lcd_label(x, y, ITEM_PRECISION, 12, "wwdc", jing_12, du_12, flow_meter.precision, letter_percent);
    
    /* 配注 */
    x=x1_offset;
    lcd_label(x, y, ITEM_INJECTION, 12, "wwd", pei_12, zhu_12, (u16)(flow_meter.daily_injection/1000));
    
    /* 硬件 */
    x = x_offset;y+=16;
    lcd_label(x, y, ITEM_H_VERSION, 12, "wwh", ying_12, y_jian_12, H_VERSION);
    /* 版本 */
    x=x1_offset;
    lcd_label(x, y, ITEM_ACTUATOR_VERSION, 12, "wwh", ban_12, ben_12, APP_VERSION);
    /* 保存 */
    x = x_offset;y+=16;
    lcd_label(x, y, ITEM_SAVE_CONF, 12, "wwd", bao_12, cun_12, flash.saved);
    /* 重启 */
    x=x1_offset;
    lcd_label(x, y, ITEM_REBOOT, 12, "wwd", chong_12, qi_12, 0);
    
  } else if(lcd_page.hover_item >= ITEM_FLOW_UART && lcd_page.hover_item <= ITEM_FLOW_ADDR) {
    /* 流量计串口参数 */
    x=x_offset + 7;//y+=8;
    lcd_label(x, y, ITEM_FLOW_UART, 12, "vvvv", letter_F, letter_L, letter_O, letter_W);x += 28;
    UartInfo *info = &uart_list[FLOW_METER_UART].info;
    display_uart_para(x, y, ITEM_FLOW_UART, 
                    info->baudrate,
                    info->bytesize, 
                    info->parity, 
                    info->stopbits,
                    flow_meter.compro, flow_meter.commode);
    /* RTU串口参数 */
    x=x_offset + 10;y+=16;
    lcd_label(x, y, ITEM_RTU_UART, 12, "vvv", letter_R, letter_T, letter_U);x += 25;
    info = &uart_list[RTU_UART].info;
    display_uart_para(x, y, ITEM_RTU_UART, 
                    info->baudrate,
                    info->bytesize, 
                    info->parity, 
                    info->stopbits,
                    rtu.compro, rtu.commode);
    /* 流量计类型 */
    x=x_offset;y+=16;
    x += 14;
    lcd_label(x, y, ITEM_FLOW_TYPE, 12, "wwwwwd", liu_12, liang_12, ji_12, lei_x_12, xing_12, flow_meter.type);
    /* 流量计地址 */
    x=x_offset + 15;y+=16;
    lcd_label(x, y, ITEM_FLOW_ADDR, 12, "wwwwwd", liu_12, liang_12, ji_12, di_12, zhi_12, flow_meter.slave);
  } else if(lcd_page.hover_item >= ITEM_MOTOR_REMOTE_CTL && lcd_page.hover_item <= ITEM_MOTOR_MANUAL_ADJ) {
    /* 电机遥控 */
    lcd_label(x, y, ITEM_MOTOR_REMOTE_CTL, 12, "wwfc", dian_12, d_ji_12, actuator.angle_tmp, letter_du); //实时显示角度
    /* 自动调节/手动调节 */
    x=x1_offset;
    lcd_label(x, y, ITEM_MOTOR_MANUAL_ADJ, 12, "wwd", shou_12, dong_12, flow_meter.auto_adj);
  } else if(lcd_page.hover_item >= ITEM_MOTOR_ADJ && lcd_page.hover_item <= ITEM_LPM_SWITCH) {
    /* 角度校准 */
    lcd_label(x, y, ITEM_MOTOR_ADJ, 12, "wwfc", jiao_12, zhun_12, actuator.angle_tmp, letter_du);
    /* 电机测试 */
    x=x1_offset;
    lcd_label(x, y, ITEM_MOTOR_TEST, 12, "wwd", d_ji_12, ce_12, ad_channels[AD_CHANNEL_ANGLE_R]); //显示角度AD值

    //ITEM_MOTOR_ANGLE_AD_MIN,         ///< 角度电阻AD最小值
    //ITEM_MOTOR_ANGLE_AD_MID,         ///< 角度电阻AD中间值
    //ITEM_MOTOR_ANGLE_AD_MAX,         ///< 角度电阻AD最大值
    /* 角度电阻AD最小值 */
    x=x_offset;y+=16;
    lcd_label(x, y, ITEM_MOTOR_ANGLE_AD_MIN, 12, "wwd", zui_12, xiao_12, actuator.angle_ad_min);
    /* 角度电阻AD中间值 */
    x=x1_offset;
    lcd_label(x, y, ITEM_MOTOR_ANGLE_AD_MID, 12, "wwd", zhong_12, q_zhi_12, actuator.angle_ad_mid);
    /* 角度电阻AD最大值 */
    x=x_offset;y+=16;
    lcd_label(x, y, ITEM_MOTOR_ANGLE_AD_MAX, 12, "wwd", zui_12, da_12, actuator.angle_ad_max);
	
    /* 电机占空比 */
    x=x1_offset;
    lcd_label(x, y, ITEM_MOTOR_RATIO, 12, "wwd", su_12, du_12, actuator.motor_ratio);
    /* 电机运行时间 */
    x=x_offset;y+=16;
    lcd_label(x, y, ITEM_MOTOR_TIME, 12, "wwfc", shi_12, jian_12, (fp32)actuator.motor_time/1000, letter_s);
    /* 低功耗 */
    x=x1_offset;
    lcd_label(x, y, ITEM_LPM_SWITCH, 12, "wwd", gong_12, hao_12, actuator.lpm_switch);
  } else if(lcd_page.hover_item >= ITEM_MOTOR_REVERSE && lcd_page.hover_item <= ITEM_FLOW_REQ_TIME1) {
    /* 电机方向 */
    lcd_label(x, y, ITEM_MOTOR_REVERSE, 12, "wwd", fang_12, xiang_12, motor.reverse);
    /* 精度权值 */
    x=x1_offset;
    lcd_label(x, y, ITEM_PRECISION_RATIO, 12, "wwfc", jing_12, quan_12, (fp32)flow_meter.precision_ratio/10, letter_percent);    
    /* 权值边缘 */
    x=x_offset;y+=16;
    lcd_label(x, y, ITEM_FLOW_MOTOR_WEIGHT_EDGE, 12, "wwd", quan_12, q_zhi_12, flow_meter.weight_edge);
    /* 无调节请求间隔 */
    x = x1_offset;
    lcd_label(x, y, ITEM_FLOW_REQ_TIME0, 12, "wwfc", wu_12, jian_12, (fp32)flow_meter.req_time_0/1000, letter_s);
    /* 有调节请求间隔 */
    x=x_offset;y+=16;
    lcd_label(x, y, ITEM_FLOW_REQ_TIME1, 12, "wwfc", you_12, jian_12, (fp32)flow_meter.req_time_1/1000, letter_s);
    /* *** */
    //x = x1_offset;
    /* *** */
    //x=x_offset;y+=16;
    
    /* 零点 ITEM_SUN_RISE_HOUR */
#if 0
    x=x_offset;y+=16;
    lcd_label(x, y, ITEM_SUN_RISE_HOUR, 12, "wwd", ling_12, l_dian_12, flow_meter.sun_rise_hour);
#endif
//    /* 保存参数 */
//    x=x_offset;y+=16;
//    lcd_label(x, y, ITEM_SAVE_CONF, 12, "wwd", bao_12, cun_12, flash.saved);
//    /* 重启 */
//    x = x1_offset;
//    lcd_label(x, y, ITEM_REBOOT, 12, "wwd", chong_12, qi_12, 0);
  }
}

void lcd_version(u8 x, u8 y, u16 version) {
  lcd_post_word(x, y, 6, 12, letter_V, FALSE);x+=6;
  lcd_post_word(x, y, 6, 12, font_num[version%1000/100], FALSE);x+=6;
  lcd_post_word(x, y, 6, 12, dot, FALSE);x+=6;
  lcd_post_word(x, y, 6, 12, font_num[version%100/10], FALSE);x+=6;
  lcd_post_word(x, y, 6, 12, dot, FALSE);x+=6;
  lcd_post_word(x, y, 6, 12, font_num[version%10], FALSE);x+=6;
}

void lcd_label(u8 x, u8 y, u8 item, u8 font, const u8 *format, ...) {
  va_list ap;
  u16 d;
  u64 ld;
  fp32 f;
  u8 *s;
  const u8 *dot_tmp = dot;
  const u8 *colon_tmp = colon;
  u8 select = FALSE;
  u8 d_tmp[6];
  u8 i, j;
  u8 num_font = font;
  u8 focus = FALSE;
  u8 num_focus = FALSE;
  u8 d_left = 2;
  u8 d_top = 0;
  if(14 == font) {
    num_font = 12;
    d_left = 4;
    d_top = 2;
  } else if(10 == font) {
    num_font = 10;
    dot_tmp = dot_10;
    colon_tmp = colon_10;
  }
  if(item == lcd_page.hover_item) {
    if(ITEM_HOVER == lcd_page.status) {
      select = TRUE;
    } else {
      focus = TRUE;
    }
  }

  va_start(ap,fmt);
  while (*format){
    switch (*format) {
      case 'w':
        s=va_arg(ap,u8 *);
        lcd_post_word(x, y, font, font, s, select);x+=font + 1;
        break;
      case 'c':
        s=va_arg(ap,u8 *);
        x += d_left;
        lcd_post_word(x, y + d_top, num_font/2, num_font, s, FALSE);x+=num_font + 1;
        break;
      case 'v':
        s=va_arg(ap,u8 *);
        lcd_post_word(x, y, num_font/2, num_font, s, select);x+=num_font/2;
        break;
      case 'd': /* int */
        d = va_arg(ap, int);
        d_tmp[0] = d%10000/1000;
        d_tmp[1] = d%1000/100;
        d_tmp[2] = d%100/10;
        d_tmp[3] = d%10;
        x += d_left;j=1;
        for(i=0;i < 4;++i) {
          num_focus = FALSE;
          if(j > 0) {
            if(TRUE == focus && ITEM_FOCUS_ALL == lcd_page.status) {
              num_focus = TRUE;
            } else if(TRUE == focus && j == lcd_page.status - 1) {
              num_focus = TRUE;
            } else {
              num_focus = FALSE;
            }
            if(10 == font) {
              lcd_post_word(x, y + d_top, num_font/2, num_font, font_num_10x10[d_tmp[i]], num_focus);x+=num_font/2;
            } else {
              lcd_post_word(x, y + d_top, num_font/2, num_font, font_num[d_tmp[i]], num_focus);x+=num_font/2;
            }
            ++j;
          }
        }
        break;
      case 'h': /* int */
        d = va_arg(ap, int);
        d_tmp[0] = d/0x1000%0x10;
        d_tmp[1] = d/0x100%0x10;
        d_tmp[2] = d/0x10%0x10;
        d_tmp[3] = d%0x10;
        x += d_left;j=1;
        for(i=0;i < 4;++i) {
          num_focus = FALSE;
          if(j > 0) {
            if(TRUE == focus && ITEM_FOCUS_ALL == lcd_page.status) {
              num_focus = TRUE;
            } else if(TRUE == focus && j == lcd_page.status - 1) {
              num_focus = TRUE;
            } else {
              num_focus = FALSE;
            }
            if(10 == font) {
              lcd_post_word(x, y + d_top, num_font/2, num_font, font_num_10x10[d_tmp[i]], num_focus);x+=num_font/2;
            } else {
              lcd_post_word(x, y + d_top, num_font/2, num_font, font_num[d_tmp[i]], num_focus);x+=num_font/2;
            }
            ++j;
          }
        }
        break;
      case 'f': /* float */
        f = va_arg(ap, fp32);
        d = (u16)f;
        ld = (u64)((f - d)*100);
        d_tmp[0] = d%100/10;
        d_tmp[1] = d%10;
        d_tmp[2] = ld%100/10;
        d_tmp[3] = ld%10;
        x += d_left;j=0;
        for(i=0;i < 4;++i) {
          if((0 == j) && (d_tmp[i] || 1 == i)) {
            ++j;
          }
          if(j > 3) {
            break;
          }
          num_focus = FALSE;
          if(j > 0) {
            if(TRUE == focus && ITEM_FOCUS_ALL == lcd_page.status) {
              num_focus = TRUE;
            } else if(TRUE == focus && j == lcd_page.status - 1) {
              num_focus = TRUE;
            } else {
              num_focus = FALSE;
            }
            if(2 == i) {
              lcd_post_word(x, y + d_top, num_font/2, num_font, dot_tmp, FALSE);x+=num_font/2;
            }
            lcd_post_word(x, y + d_top, num_font/2, num_font, font_num[d_tmp[i]], num_focus);x+=num_font/2;
            ++j;
          }
        }
        break;
      case 't':
        x += 2;
        ld = va_arg(ap, u64);
        d_tmp[0] = ld/1000/3600%24;
        d_tmp[1] = d_tmp[0]%10;
        d_tmp[0] = d_tmp[0]/10%10;
        d_tmp[2] = ld/1000/60%60;
        d_tmp[3] = d_tmp[2]%10;
        d_tmp[2] = d_tmp[2]/10%10;
        d_tmp[4] = ld/1000%60;
        d_tmp[5] = d_tmp[4]%10;
        d_tmp[4] = d_tmp[4]/10%10;
        for(i = 0;i < 6;++i) {
          lcd_post_word(x, y, num_font/2, num_font, font_num_10x10[d_tmp[i]], FALSE);x+=num_font/2;
          if(i < 5 && i%2) {
            lcd_post_word(x, y, num_font/2, num_font, colon_tmp, FALSE);x+=num_font/2;
          }
        }
        break;
    }
    ++format;
  }
  va_end(ap);

}

void display_uart_para(u8 x, u8 y, u8 item, u32 baud, u8 bytesize, u8 parity, u8 stopbits, u8 compro, u8 commode) {
  u8 focus = ITEM_FOCUS_FIRST==lcd_page.status&&item==lcd_page.hover_item?TRUE:FALSE;
  lcd_post_word(x, y, 6, 12, font_num[baud/100000%10], focus);x+=6;
  lcd_post_word(x, y, 6, 12, font_num[baud/10000%10], focus);x+=6;
  lcd_post_word(x, y, 6, 12, font_num[baud/1000%10], focus);x+=6;
  lcd_post_word(x, y, 6, 12, font_num[baud/100%10], focus);x+=6;
  lcd_post_word(x, y, 6, 12, font_num[baud/10%10], focus);x+=6;
  lcd_post_word(x, y, 6, 12, font_num[baud%10], focus);x+=8;
  
  focus = ITEM_FOCUS_SECOND==lcd_page.status&&item==lcd_page.hover_item?TRUE:FALSE;
  lcd_post_word(x, y, 6, 12, font_num[bytesize%10], focus);x+=8;
  
  const u8 *parity_tmp = letter_N;
  if('O' == parity) {
    parity_tmp = letter_O;
  } else if('E' == parity) {
    parity_tmp = letter_E;
  } else {
    parity_tmp = letter_N;
  }
  focus = ITEM_FOCUS_THIRD==lcd_page.status&&item==lcd_page.hover_item?TRUE:FALSE;
  lcd_post_word(x, y, 6, 12, parity_tmp, focus);x+=8;
  focus = ITEM_FOCUS_FOURTH==lcd_page.status&&item==lcd_page.hover_item?TRUE:FALSE;
  lcd_post_word(x, y, 6, 12, font_num[stopbits%10], focus);x+=8;
  
  focus = ITEM_FOCUS_FIFTH==lcd_page.status&&item==lcd_page.hover_item?TRUE:FALSE;
  lcd_post_word(x, y, 6, 12, font_num[compro%0x10], focus);x+=8;
  focus = ITEM_FOCUS_SIXTH==lcd_page.status&&item==lcd_page.hover_item?TRUE:FALSE;
  lcd_post_word(x, y, 6, 12, font_num[commode%0x10], focus);
}