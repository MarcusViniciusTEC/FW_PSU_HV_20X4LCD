#ifndef LCD_HD44780_H
#define LCD_HD44780_H

#include "stm32f4xx_hal.h"

#define DEF_LCD_ICON_BATTERY        0
#define DEF_LCD_ICON_CELSIUS        1
#define DEF_LCD_ICON_BLUETOOTH      2
#define DEF_LCD_ICON_PRESSURE       3
#define DEF_LCD_ICON_TEMPERATURE    4
#define DEF_LCD_ICON_FLOW           5
#define DEF_LCD_ICON_PULSE          6
#define DEF_LCD_ICON_CURRENT        7

void vLCD_HD44780_Init(void);
void vLCD_HD44780_DisplayOn(void);
void vLCD_HD44780_DisplayOff(void);
void vLCD_HD44780_Clear(void);
void vLCD_HD44780_Puts(uint8_t x, uint8_t y, char *str);
void vLCD_HD44780_BlinkOn(void);
void vLCD_HD44780_BlinkOff(void);
void vLCD_HD44780_CursorOn(void);
void vLCD_HD44780_CursorOff(void);
void vLCD_HD44780_ScrollLeft(void);
void vLCD_HD44780_ScrollRight(void);
void vLCD_HD44780_CreateChar(uint8_t location, uint8_t *data);
void vLCD_HD44780_PutCustom(uint8_t x, uint8_t y, uint8_t location);
void vLCD_HD44780_Put(uint8_t Data);
void vLCD_HD44780_Backlight(uint8_t state);
void vLCD_HD44780_CursorSet(uint8_t col, uint8_t row);

#endif

