#include "LCD_HD44780.h"
#include "stdbool.h"
#include "LCD_HD44780_cfg.h"

/* Private Datatypes */
typedef struct
{
    uint8_t DisplayControl;
    uint8_t DisplayFunction;
    uint8_t DisplayMode;
    uint8_t currentX;
    uint8_t currentY;

} LCD_HD44780_Options_t;

/* Private functions */
static void vLCD_HD44780_Cmd(uint8_t cmd);
static void vLCD_HD44780_Cmd4bit(uint8_t cmd);
static void vLCD_HD44780_Data(uint8_t data);

/* Private variable */
static LCD_HD44780_Options_t LCD_HD44780_Opts;

/* Pin definitions */
#define LCD_HD44780_RS_LOW()            HAL_GPIO_WritePin(_LCD_HD44780_RS_PORT, _LCD_HD44780_RS_PIN,GPIO_PIN_RESET); vLCD_HD44780_Delay_us(150)
#define LCD_HD44780_RS_HIGH()           HAL_GPIO_WritePin(_LCD_HD44780_RS_PORT, _LCD_HD44780_RS_PIN,GPIO_PIN_SET); vLCD_HD44780_Delay_us(150)
#define LCD_HD44780_E_LOW()             HAL_GPIO_WritePin(_LCD_HD44780_E_PORT,  _LCD_HD44780_E_PIN,GPIO_PIN_RESET); vLCD_HD44780_Delay_us(150)
#define LCD_HD44780_E_HIGH()            HAL_GPIO_WritePin(_LCD_HD44780_E_PORT,  _LCD_HD44780_E_PIN,GPIO_PIN_SET); vLCD_HD44780_Delay_us(150)
#define LCD_HD44780_E_BLINK()           LCD_HD44780_E_HIGH(); vLCD_HD44780_Delay_us(150); LCD_HD44780_E_LOW(); vLCD_HD44780_Delay_us(150)

#define LCD_HD44780_D7_LOW()            HAL_GPIO_WritePin(_LCD_HD44780_D7_PORT, _LCD_HD44780_D7_PIN, GPIO_PIN_RESET);
#define LCD_HD44780_D6_LOW()            HAL_GPIO_WritePin(_LCD_HD44780_D6_PORT, _LCD_HD44780_D6_PIN, GPIO_PIN_RESET);
#define LCD_HD44780_D5_LOW()            HAL_GPIO_WritePin(_LCD_HD44780_D5_PORT, _LCD_HD44780_D5_PIN, GPIO_PIN_RESET);
#define LCD_HD44780_D4_LOW()            HAL_GPIO_WritePin(_LCD_HD44780_D4_PORT, _LCD_HD44780_D4_PIN, GPIO_PIN_RESET);

#define LCD_HD44780_VCC_ENABLE()        HAL_GPIO_WritePin(_LCD_HD44780_VCC_PORT, _LCD_HD44780_VCC_PIN, GPIO_PIN_SET); vLCD_HD44780_Delay_ms(20)
#define LCD_HD44780_VCC_DISABLE()       HAL_GPIO_WritePin(_LCD_HD44780_VCC_PORT, _LCD_HD44780_VCC_PIN, GPIO_PIN_RESET);

/* Commands*/
#define LCD_HD44780_CLEARDISPLAY        0x01
#define LCD_HD44780_RETURNHOME          0x02
#define LCD_HD44780_ENTRYMODESET        0x04
#define LCD_HD44780_DISPLAYCONTROL      0x08
#define LCD_HD44780_CURSORSHIFT         0x10
#define LCD_HD44780_FUNCTIONSET         0x20
#define LCD_HD44780_SETCGRAMADDR        0x40
#define LCD_HD44780_SETDDRAMADDR        0x80

/* Flags for display entry mode */
#define LCD_HD44780_ENTRYRIGHT          0x00
#define LCD_HD44780_ENTRYLEFT           0x02
#define LCD_HD44780_ENTRYSHIFTINCREMENT 0x01
#define LCD_HD44780_ENTRYSHIFTDECREMENT 0x00

/* Flags for display on/off control */
#define LCD_HD44780_DISPLAYON           0x04
#define LCD_HD44780_CURSORON            0x02
#define LCD_HD44780_BLINKON             0x01

/* Flags for display/cursor shift */
#define LCD_HD44780_DISPLAYMOVE         0x08
#define LCD_HD44780_CURSORMOVE          0x00
#define LCD_HD44780_MOVERIGHT           0x04
#define LCD_HD44780_MOVELEFT            0x00

/* Flags for function set */
#define LCD_HD44780_8BITMODE            0x10
#define LCD_HD44780_4BITMODE            0x00
#define LCD_HD44780_2LINE               0x08
#define LCD_HD44780_1LINE               0x00
#define LCD_HD44780_5x10DOTS            0x04
#define LCD_HD44780_5x8DOTS             0x00

static void vLCD_HD44780_Delay_us(uint16_t us)
{
    uint32_t microSec;
    uint32_t deltaTimeTicks = 0;
#if _LCD_HD44780_USING_SYSTICK || _LCD_HD44780_USING_TIM2
    uint32_t div = 0;
    uint32_t startTimeTicks = 0;
#if _LCD_HD44780_USING_TIM2
    uint32_t currentTimeTicks = 0;
    uint32_t previousTimeTicks = 0;
#endif
#endif

#if _LCD_HD44780_USING_SYSTICK
    if((SysTick->CTRL & SysTick_CTRL_ENABLE_Msk) != 0U)
    {
        div = (uint32_t)(( SysTick->LOAD + 1) / 1000);

        if (div > 0)
        {
            microSec = (uint32_t)(us);
            startTimeTicks = (uint32_t)( (HAL_GetTick() * 1000) + (1000 - (SysTick->VAL / div)) );
        } else
        {
            microSec = (uint32_t)( 5 * us );
        }
    } else
    {
        microSec = (uint32_t)( 5 * us );
    }

    while (deltaTimeTicks < microSec)
    {
        if (div > 0)
            deltaTimeTicks = ((HAL_GetTick() * 1000) + (1000 - (SysTick->VAL / div))) - (startTimeTicks);
        else
            deltaTimeTicks += 1;
        
        __NOP();
    }
#elif _LCD_HD44780_USING_TIM2
    if (__HAL_RCC_TIM2_IS_CLK_ENABLED())
    {
        div = (uint32_t)(( TIM2->ARR + 1) / 1000);

        if (div > 0)
        {
            microSec = (uint32_t)(us);
            startTimeTicks = (uint32_t)(TIM2->CNT / div);
        } else
        {
            microSec = (uint32_t)( 5 * us );
        }

        previousTimeTicks = startTimeTicks;
    } else
    {
        microSec = (uint32_t)( 5 * us );
    }

    while (deltaTimeTicks < microSec)
    {
        if (div > 0)
        {
            currentTimeTicks = (uint32_t)(TIM2->CNT / div);
            deltaTimeTicks += (currentTimeTicks > previousTimeTicks) ? (currentTimeTicks - previousTimeTicks) : (previousTimeTicks - currentTimeTicks);
            previousTimeTicks = currentTimeTicks;
        }
        else
            deltaTimeTicks += 1;
        
        __NOP();
    }
#else
    microSec = (uint32_t)( 10 * us );

    while (deltaTimeTicks < microSec)
    {
        deltaTimeTicks += 1;
        __NOP();
    }
#endif
}

static void vLCD_HD44780_Delay_ms(uint8_t ms)
{
    HAL_Delay(ms);
}

void vLCD_HD44780_Init(void)
{
    /* Wait VCC be stable*/
    HAL_Delay(20);

    /* Set cursor pointer to beginning for LCD */
    LCD_HD44780_Opts.currentX = 0;
    LCD_HD44780_Opts.currentY = 0;
    LCD_HD44780_Opts.DisplayFunction = LCD_HD44780_4BITMODE | LCD_HD44780_5x8DOTS | LCD_HD44780_1LINE;

    if (_LCD_HD44780_ROWS > 1)
        LCD_HD44780_Opts.DisplayFunction |= LCD_HD44780_2LINE;

    /* Try to set 4bit mode */
    vLCD_HD44780_Cmd4bit(0x03);
    vLCD_HD44780_Delay_ms(5);

    /* Second try */
    vLCD_HD44780_Cmd4bit(0x03);
    vLCD_HD44780_Delay_ms(5);

    /* Third goo! */
    vLCD_HD44780_Cmd4bit(0x03);
    vLCD_HD44780_Delay_ms(5);

    /* Set 4-bit interface */
    vLCD_HD44780_Cmd4bit(0x02);
    vLCD_HD44780_Delay_ms(5);

    /* Set # lines, font size, etc. */
    vLCD_HD44780_Cmd(LCD_HD44780_FUNCTIONSET | LCD_HD44780_Opts.DisplayFunction);

    /* Turn the display on with no cursor or blinking default */
    LCD_HD44780_Opts.DisplayControl = LCD_HD44780_DISPLAYON;
    vLCD_HD44780_DisplayOn();
    vLCD_HD44780_Clear();

    /* Default font directions */
    LCD_HD44780_Opts.DisplayMode = LCD_HD44780_ENTRYLEFT | LCD_HD44780_ENTRYSHIFTDECREMENT;
    vLCD_HD44780_Cmd(LCD_HD44780_ENTRYMODESET | LCD_HD44780_Opts.DisplayMode);
    vLCD_HD44780_Delay_ms(5);

}

void vLCD_HD44780_Clear(void)
{
    vLCD_HD44780_Cmd(LCD_HD44780_CLEARDISPLAY);
    vLCD_HD44780_Delay_ms(5);
}

void vLCD_HD44780_Puts(uint8_t x, uint8_t y, char *str)
{
    vLCD_HD44780_CursorSet(x, y);
    while (*str)
    {
        if (LCD_HD44780_Opts.currentX >= _LCD_HD44780_COLS)
        {
            LCD_HD44780_Opts.currentX = 0;
            LCD_HD44780_Opts.currentY++;
            vLCD_HD44780_CursorSet(LCD_HD44780_Opts.currentX, LCD_HD44780_Opts.currentY);
        }
        if (*str == '\n')
        {
            LCD_HD44780_Opts.currentY++;
            vLCD_HD44780_CursorSet(LCD_HD44780_Opts.currentX, LCD_HD44780_Opts.currentY);
        } else if (*str == '\r')
        {
            vLCD_HD44780_CursorSet(0, LCD_HD44780_Opts.currentY);
        } else
        {
            vLCD_HD44780_Data(*str);
            LCD_HD44780_Opts.currentX++;
        }
        str++;
    }
}

void vLCD_HD44780_DisplayOn(void)
{
    LCD_HD44780_Opts.DisplayControl |= LCD_HD44780_DISPLAYON;
    vLCD_HD44780_Cmd(LCD_HD44780_DISPLAYCONTROL | LCD_HD44780_Opts.DisplayControl);
}

void vLCD_HD44780_DisplayOff(void)
{
    LCD_HD44780_Opts.DisplayControl &= ~LCD_HD44780_DISPLAYON;
    vLCD_HD44780_Cmd(LCD_HD44780_DISPLAYCONTROL | LCD_HD44780_Opts.DisplayControl);
}

void vLCD_HD44780_BlinkOn(void)
{
    LCD_HD44780_Opts.DisplayControl |= LCD_HD44780_BLINKON;
    vLCD_HD44780_Cmd(LCD_HD44780_DISPLAYCONTROL | LCD_HD44780_Opts.DisplayControl);
}

void vLCD_HD44780_BlinkOff(void)
{
    LCD_HD44780_Opts.DisplayControl &= ~LCD_HD44780_BLINKON;
    vLCD_HD44780_Cmd(LCD_HD44780_DISPLAYCONTROL | LCD_HD44780_Opts.DisplayControl);
}

void vLCD_HD44780_CursorOn(void)
{
    LCD_HD44780_Opts.DisplayControl |= LCD_HD44780_CURSORON;
    vLCD_HD44780_Cmd(LCD_HD44780_DISPLAYCONTROL | LCD_HD44780_Opts.DisplayControl);
}

void vLCD_HD44780_CursorOff(void)
{
    LCD_HD44780_Opts.DisplayControl &= ~LCD_HD44780_CURSORON;
    vLCD_HD44780_Cmd(LCD_HD44780_DISPLAYCONTROL | LCD_HD44780_Opts.DisplayControl);
}

void vLCD_HD44780_ScrollLeft(void)
{
    vLCD_HD44780_Cmd(LCD_HD44780_CURSORSHIFT | LCD_HD44780_DISPLAYMOVE | LCD_HD44780_MOVELEFT);
}

void vLCD_HD44780_ScrollRight(void)
{
    vLCD_HD44780_Cmd(LCD_HD44780_CURSORSHIFT | LCD_HD44780_DISPLAYMOVE | LCD_HD44780_MOVERIGHT);
}

void vLCD_HD44780_CreateChar(uint8_t location, uint8_t *data)
{
    uint8_t i;

    /* We have 8 locations available for custom characters */
    location &= 0x07;
    vLCD_HD44780_Cmd(LCD_HD44780_SETCGRAMADDR | ( location << 3 ));

    for (i = 0; i < 8; i++)
    {
        vLCD_HD44780_Data(data[i]);
    }
}

void vLCD_HD44780_PutCustom(uint8_t x, uint8_t y, uint8_t location)
{
    vLCD_HD44780_CursorSet(x, y);
    vLCD_HD44780_Data(location);
}

static void vLCD_HD44780_Cmd(uint8_t cmd)
{
    LCD_HD44780_RS_LOW();
    vLCD_HD44780_Cmd4bit(cmd >> 4);
    vLCD_HD44780_Cmd4bit(cmd & 0x0F);
}

static void vLCD_HD44780_Data(uint8_t data)
{
    LCD_HD44780_RS_HIGH();
    vLCD_HD44780_Cmd4bit(data >> 4);
    vLCD_HD44780_Cmd4bit(data & 0x0F);
}

static void vLCD_HD44780_Cmd4bit(uint8_t cmd)
{
    HAL_GPIO_WritePin(_LCD_HD44780_D7_PORT, _LCD_HD44780_D7_PIN, ( (GPIO_PinState)( cmd & 0x08 ) == 0x08 ) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(_LCD_HD44780_D6_PORT, _LCD_HD44780_D6_PIN, ( (GPIO_PinState)( cmd & 0x04 ) == 0x04 ) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(_LCD_HD44780_D5_PORT, _LCD_HD44780_D5_PIN, ( (GPIO_PinState)( cmd & 0x02 ) == 0x02 ) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(_LCD_HD44780_D4_PORT, _LCD_HD44780_D4_PIN, ( (GPIO_PinState)( cmd & 0x01 ) == 0x01 ) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    LCD_HD44780_E_BLINK();
}

void vLCD_HD44780_CursorSet(uint8_t col, uint8_t row)
{
    uint8_t row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
    if (row >= _LCD_HD44780_ROWS)
        row = 0;
    LCD_HD44780_Opts.currentX = col;
    LCD_HD44780_Opts.currentY = row;
    vLCD_HD44780_Cmd(LCD_HD44780_SETDDRAMADDR | ( col + row_offsets[row] ));
}

void vLCD_HD44780_Put(uint8_t Data)
{
    vLCD_HD44780_Data(Data);
}
