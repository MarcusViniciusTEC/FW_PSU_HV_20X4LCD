#ifndef LCD_HD44780_CONFIG_H
#define LCD_HD44780_CONFIG_H

#include "stm32f4xx_hal.h"
#include "main.h"

#define _LCD_HD44780_USING_SYSTICK      1
#define _LCD_HD44780_USING_TIM2         0

#define _LCD_HD44780_COLS               20
#define _LCD_HD44780_ROWS               4

#define _LCD_HD44780_RS_PORT            LCD_RS_GPIO_Port
#define _LCD_HD44780_RS_PIN             LCD_RS_Pin

#define _LCD_HD44780_E_PORT             LCD_EN_GPIO_Port
#define _LCD_HD44780_E_PIN              LCD_EN_Pin

#define _LCD_HD44780_D4_PORT            LCD_DB4_GPIO_Port
#define _LCD_HD44780_D4_PIN	            LCD_DB4_Pin

#define _LCD_HD44780_D5_PORT            LCD_DB5_GPIO_Port
#define _LCD_HD44780_D5_PIN             LCD_DB5_Pin

#define _LCD_HD44780_D6_PORT            LCD_DB6_GPIO_Port
#define _LCD_HD44780_D6_PIN             LCD_DB6_Pin

#define _LCD_HD44780_D7_PORT            LCD_DB7_GPIO_Port
#define _LCD_HD44780_D7_PIN             LCD_DB7_Pin

#define _LCD_HD44780_VCC_PORT           VCC_LCD_EN_GPIO_Port
#define _LCD_HD44780_VCC_PIN            VCC_LCD_EN_Pin

#endif

