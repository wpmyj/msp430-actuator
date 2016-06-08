/**
 * @file  lcd_font.h
 * @brief 字体描述
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#ifndef LCD_FONT_H_
#define LCD_FONT_H_
#include "../kernel/kernel.h"
#include "../drivers/drivers.h"

#define FONT_WCHAR_HIGHT    12   ///< 普通汉字宽度
#define FONT_WCHAR_WIDTH    12   ///< 普通汉字高度
#define FONT_NUM_HIGHT      12   ///< 普通数字宽度
#define FONT_NUM_WIDTH      6    ///< 普通数字高度

//extern const u8 font_num[16][FONT_NUM_WIDTH * 2];
//extern const u8 font_punc[18][FONT_NUM_WIDTH * 2];
//extern const u8 font_wchar[28][FONT_WCHAR_WIDTH * 2];
//
//extern const u8 font_num_10x10[10][10];
//extern const u8 font_punc_10x10[3][10];
//extern const u8 font_special_10x10[4][20];

extern const u8 font_num[16][FONT_NUM_WIDTH * 2];
extern const u8 font_num_10x10[10][10];
/* 14x14汉字 */
extern const u8 jiao_14[28];
extern const u8 du_14[28];
extern const u8 liu_14[28];
extern const u8 su_14[28];
extern const u8 pei_14[28];
extern const u8 zhu_14[28];

/* 12x12字符 */
/* 12x12字符 */
extern const u8 dot[12];
extern const u8 colon[12];
extern const u8 letter_P[12];
extern const u8 letter_M[12];
extern const u8 letter_N[12];
extern const u8 letter_V[12];
extern const u8 letter_F[12];
extern const u8 letter_L[12];
extern const u8 letter_O[12];
extern const u8 letter_W[12];
extern const u8 letter_R[12];
extern const u8 letter_T[12];
extern const u8 letter_U[12];
extern const u8 letter_s[12];
extern const u8 letter_percent[12];
extern const u8 letter_E[12];
extern const u8 letter_star[12];
extern const u8 letter_space[12];
extern const u8 letter_du[12];
extern const u8 bmp_m3[24];
extern const u8 bmp_p_h[24];
extern const u8 bmp_p_d[24];

/* 12x12汉字 */
extern const u8 di_12[24];
extern const u8 zhi_12[24];
extern const u8 bei_12[24];
extern const u8 guang_12[24];
extern const u8 kai_12[24];
extern const u8 du_12[24];
extern const u8 jing_12[24];
extern const u8 zhi_l_12[24];
extern const u8 liang_12[24];
extern const u8 lei_12[24];
extern const u8 zhu_12[24];
extern const u8 fu_12[24];
extern const u8 ma_12[24];
extern const u8 liu_12[24];
extern const u8 ji_12[24];
extern const u8 qing_12[24];
extern const u8 shu_12[24];
extern const u8 ru_12[24];
extern const u8 mi_12[24];
extern const u8 m_ma_12[24];
extern const u8 fan_12[24];
extern const u8 ping_12[24];
extern const u8 lei_x_12[24];
extern const u8 xing_12[24];
extern const u8 pei_12[24];
extern const u8 ban_12[24];
extern const u8 ben_12[24];
extern const u8 dian_12[24];
extern const u8 d_ji_12[24];
extern const u8 jiao_12[24];
extern const u8 zhun_12[24];
extern const u8 kong_12[24];
extern const u8 ce_12[24];
extern const u8 gang_12[24];
extern const u8 yi_12[24];
extern const u8 gong_12[24];
extern const u8 hao_12[24];
extern const u8 bao_12[24];
extern const u8 cun_12[24];
extern const u8 chong_12[24];
extern const u8 qi_12[24];
extern const u8 su_12[24];
extern const u8 shi_12[24];
extern const u8 jian_12[24];
extern const u8 fang_12[24];
extern const u8 xiang_12[24];
extern const u8 you_12[24];
extern const u8 wu_12[24];
extern const u8 quan_12[24];
extern const u8 q_zhi_12[24];
extern const u8 tong_12[24];
extern const u8 xin_12[24];
extern const u8 ling_12[24];
extern const u8 l_dian_12[24];
extern const u8 ying_12[24];
extern const u8 y_jian_12[24];
extern const u8 shou_12[24];
extern const u8 dong_12[24];
extern const u8 zui_12[24];
extern const u8 da_12[24];
extern const u8 xiao_12[24];
extern const u8 zhong_12[24];

/* 10x10字符 */
extern const u8 letter_space10[10];
extern const u8 letter_V10[10];
extern const u8 dot_10[10];
extern const u8 colon_10[10];
extern const u8 wchar_com_ok[20];
extern const u8 wchar_com_err[20];

#endif