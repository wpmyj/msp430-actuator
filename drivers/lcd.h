/**
 * @file  lcd.h
 * @brief lcd驱动描述(拓普微lm6029)
 * @version 1.0
 * @author ygz <yangguozheng@tjwzkj.com>
 * @date   2015.7.1
 */
#ifndef LCD_H_
#define LCD_H_
#include "../kernel/kernel.h"
#include <string.h>


#define LCD_BLK_SWITCH_DIR    P6DIR  ///< 背光控制管脚
#define LCD_BLK_SWITCH_OUT    P6OUT
#define LCD_BLK_SWITCH_BIT    BIT5

typedef struct Lcd {
  u8 light_on;
}Lcd;

extern Lcd lcd;
extern void lcd_init();
extern void lcd_post_word(u8 x, u8 y, u8 w, u8 h, const u8 *word, u8 r);
extern void lcd_display_screen();
extern void lcd_clear_data();
extern void lcd_light_on(u8 opt);

#endif