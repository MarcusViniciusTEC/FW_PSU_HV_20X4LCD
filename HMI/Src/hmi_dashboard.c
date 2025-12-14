#include "hmi_menu.h"
#include "hmi_menu_types.h"

#include <stdbool.h>
#include <stdio.h>


#include "hmi.h"

#include "main.h"
#include "adc.h"
#include "app.h"
#include "LCD_HD44780.h"

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "hmi_dashboard_types.h"


uint8_t cursor_arrow[8] =
{
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b11111
};

/***********************************************************************************/

static hmi_dash_ctrl_t hmi_dash_ctrl = {0};
static hmi_dash_edit_t hmi_dash_edit[NUMBER_OF_FIELDS] = {0};
static hmi_dash_set_cursor_t hmi_dash_set_cursor[NUMBER_OF_FIELDS][NUMBER_OF_FIELDS] = cursor_set_edit_default;

/***********************************************************************************/

static void hmi_dashboard_show_voltage(void);
static void hmi_dashboard_show_power(void);
static void hmi_dashboard_show_current(void);
static void hmi_dashboard_show_temp(void);
static void hmi_dashboard_show_out_status(void);
static void hmi_dashboard_hardware_status(void);
static void hmi_dashboard_show_target_voltage(void);
static void hmi_dashboard_show_target_current(void);
static void hmi_dashboard_show_cursor(void);
static void hmi_dashboard_show_fan_porcentage(void);
static void hmi_dashboard_increment_digit(void);
static void hmi_dashboard_decrement_digit(void);
static void hmi_dashboard_increment_field(void);
static void hmi_dashboard_decrement_field(void);
static void hmi_dashboard_out_state_toggle(void);
static void hmi_dashboard_exit(void);
static void hmi_dashboard_toggle_mode(void);
static void hmi_dashboard_blnk_cursor(void);

hmi_dash_out_stauts_t hmi_dash_get_out_status(void);


/***********************************************************************************/

uint32_t hmi_dashboard_get_target_current(void)
{
    return hmi_dash_ctrl.target[FIELD_CURRENT];
}

/***********************************************************************************/

uint32_t hmi_dashboard_get_target_voltage(void)
{   
    return hmi_dash_ctrl.target[FIELD_VOLTAGE];
}

/***********************************************************************************/

hmi_dash_out_stauts_t hmi_dash_get_out_status(void)
{

    return hmi_dash_ctrl.out_status;
}

/***********************************************************************************/

static void hmi_dashboard_show_current(void)
{
    vLCD_HD44780_Puts(14, 1, "1.500A");
}

/***********************************************************************************/

static void hmi_dashboard_show_voltage(void)
{
    
     vLCD_HD44780_Puts(3, 1, "500V");
  
}

/***********************************************************************************/

static void hmi_dashboard_show_fan_porcentage(void)
{
    vLCD_HD44780_Puts(17, 2, "30%");
}

/***********************************************************************************/

static void hmi_dashboard_show_power(void)
{
    uint32_t power =  243;  //app_get_centiwatts();
    char sz_string[28] = {0};
    snprintf(sz_string, sizeof(sz_string), "Pw:%03dW",
             (int)(power / 10));
    vLCD_HD44780_Puts(0, 2, sz_string); 
}

/***********************************************************************************/

static void hmi_dashboard_show_temp(void)
{
    uint32_t temp_tranformer = 230; /*app_get_temperature(ADC_TEMP_CH_TRANSFORMER);*/
    uint32_t temp_heatsink =  250;  /*app_get_temperature(ADC_TEMP_CH_HEATSINK);    */

    char sz_string[28] = {0};
    snprintf(sz_string, sizeof(sz_string), "%02d'",
             (int)(temp_tranformer / 10));
            vLCD_HD44780_Puts(13, 3, sz_string);
    
    snprintf(sz_string, sizeof(sz_string), "%02d'C",
             (int)(temp_heatsink/ 10));
            vLCD_HD44780_Puts(16, 3, sz_string); 
    
}

/***********************************************************************************/

static void hmi_dashboard_out_state_toggle(void)
{
    switch (hmi_dash_ctrl.out_status)
    {
    case OUT_STATUS_OFF:
        hmi_dash_ctrl.out_status = OUT_STATUS_RUN;
        break;
    case OUT_STATUS_RUN:
        hmi_dash_ctrl.out_status = OUT_STATUS_OFF;
        break;
    default:
        hmi_dash_ctrl.out_status = OUT_STATUS_OFF;
        break;
    } 
}

/***********************************************************************************/

static void hmi_dashboard_show_out_status(void)
{
    switch (hmi_dash_ctrl.out_status)
    {
    case OUT_STATUS_OFF:
        vLCD_HD44780_Puts(0, 3, "OFF");
        break;
    case OUT_STATUS_RUN:
        vLCD_HD44780_Puts(0, 3, "RUN");
        break;
    default:
        vLCD_HD44780_Puts(0, 3, " ? ");
        break;
    } 
}

/***********************************************************************************/

static void hmi_dashboard_hardware_status(void)
{
    vLCD_HD44780_Puts(8, 3, "Rdy");
}

/***********************************************************************************/

static void hmi_dashboard_show_target_voltage(void)
{
    uint8_t d1 = (hmi_dash_ctrl.target[FIELD_VOLTAGE] / 100);
    uint8_t d2 = (hmi_dash_ctrl.target[FIELD_VOLTAGE] / 10) % 10;
    uint8_t d3 = (hmi_dash_ctrl.target[FIELD_VOLTAGE] % 10);
    char sz_string[20] = {0};
    snprintf(sz_string, sizeof(sz_string),
         "%c%u%uV",
         (d1 == 0) ? ' ' : ('0' + d1),
         d2,
         d3);
    vLCD_HD44780_Puts(3, 0, sz_string);
}

/***********************************************************************************/

static void hmi_dashboard_show_target_current(void)
{
    char sz_string[20] = {0};
    uint8_t d1 = (hmi_dash_ctrl.target[FIELD_CURRENT] / 100);
    uint8_t d2 = (hmi_dash_ctrl.target[FIELD_CURRENT] / 10) % 10;
    uint8_t d3 = (hmi_dash_ctrl.target[FIELD_CURRENT] % 10);
    snprintf(sz_string, sizeof(sz_string), "%01u.%01u%01u0A", /*ADICIONADO UM ZERO APENAS POR ESTÉTICA*/
    d1, 
    d2,
    d3);
    vLCD_HD44780_Puts(14, 0, sz_string);
}

/***********************************************************************************/

static void hmi_dashboard_show_cursor(void)
{

}

/***********************************************************************************/

static void hmi_dashboard_hide_cursor(void)
{
    vLCD_HD44780_Puts(hmi_dash_set_cursor[hmi_dash_ctrl.field][ hmi_dash_edit[hmi_dash_ctrl.field].index ].col , hmi_dash_set_cursor[hmi_dash_ctrl.field][hmi_dash_edit[hmi_dash_ctrl.field].index].row, " ");
}


/***********************************************************************************/

static void hmi_dashboard_increment_field(void)
{
    hmi_dash_edit[hmi_dash_ctrl.field].index++;
    if (hmi_dash_edit[hmi_dash_ctrl.field].index > 1)
    {
        hmi_dash_edit[hmi_dash_ctrl.field].index = 1;
    }
}

/***********************************************************************************/

static void hmi_dashboard_decrement_field(void)
{
    hmi_dash_edit[hmi_dash_ctrl.field].index--;
    if (hmi_dash_edit[hmi_dash_ctrl.field].index > 1)
    {
        hmi_dash_edit[hmi_dash_ctrl.field].index = 0;
    }
}

/***********************************************************************************/

static void hmi_dashboard_increment_digit(void)
{
    hmi_dash_set_cursor[hmi_dash_ctrl.field]  [hmi_dash_edit[hmi_dash_ctrl.field].index].value++;
    uint16_t value[NUMBER_OF_FIELDS] = {0};
    if(hmi_dash_set_cursor[hmi_dash_ctrl.field] [INDEX_SECOND_DIGIT-1].value > MAX_DIGIT)
    {
        hmi_dash_set_cursor[hmi_dash_ctrl.field] [INDEX_SECOND_DIGIT-1].value = 0;
        hmi_dash_edit[hmi_dash_ctrl.field].first_digit++;
    }
    if(hmi_dash_set_cursor[hmi_dash_ctrl.field] [INDEX_THIRD_DIGIT-1].value > MAX_DIGIT)
    {
        hmi_dash_set_cursor[hmi_dash_ctrl.field] [INDEX_THIRD_DIGIT-1].value = 0;
        hmi_dash_set_cursor[hmi_dash_ctrl.field] [INDEX_SECOND_DIGIT-1].value ++;
    }
    value[hmi_dash_ctrl.field] += hmi_dash_edit[hmi_dash_ctrl.field].first_digit *  100;
    value[hmi_dash_ctrl.field] += hmi_dash_set_cursor[hmi_dash_ctrl.field][INDEX_SECOND_DIGIT - 1].value * 10;
    value[hmi_dash_ctrl.field] += hmi_dash_set_cursor[hmi_dash_ctrl.field][INDEX_THIRD_DIGIT - 1].value;

    if(value[FIELD_CURRENT] > 200)
    {
        value[hmi_dash_ctrl.field] = 200;
    }
    if(value[FIELD_VOLTAGE] > 500)
    {
        value[hmi_dash_ctrl.field] = 500;
    }
    hmi_dash_ctrl.target[hmi_dash_ctrl.field] = value[hmi_dash_ctrl.field];
}

/***********************************************************************************/

static void hmi_dashboard_decrement_digit(void)
{
    hmi_dash_set_cursor[hmi_dash_ctrl.field]  [hmi_dash_edit[hmi_dash_ctrl.field].index].value--;
    uint16_t value[NUMBER_OF_FIELDS] = {0};
    if(hmi_dash_edit[hmi_dash_ctrl.field].first_digit > MIN_DIGIT)
    {
        if(hmi_dash_set_cursor[hmi_dash_ctrl.field] [INDEX_SECOND_DIGIT-1].value > MAX_DIGIT)
        {
            hmi_dash_edit[hmi_dash_ctrl.field].first_digit--;
            hmi_dash_set_cursor[hmi_dash_ctrl.field] [INDEX_SECOND_DIGIT-1].value = MAX_DIGIT;
        }
    }
    else if(hmi_dash_edit[hmi_dash_ctrl.field].first_digit  == MIN_DIGIT)
    {
        if(hmi_dash_set_cursor[hmi_dash_ctrl.field] [INDEX_SECOND_DIGIT-1].value > MAX_DIGIT )
        {
            hmi_dash_set_cursor[hmi_dash_ctrl.field] [INDEX_SECOND_DIGIT-1].value = MIN_DIGIT;
        }
    }
    if(hmi_dash_set_cursor[hmi_dash_ctrl.field] [INDEX_SECOND_DIGIT-1].value > MIN_DIGIT)
    {
        if(hmi_dash_set_cursor[hmi_dash_ctrl.field] [INDEX_THIRD_DIGIT-1].value > MAX_DIGIT)
        {
            hmi_dash_set_cursor[hmi_dash_ctrl.field] [INDEX_SECOND_DIGIT-1].value--;
            hmi_dash_set_cursor[hmi_dash_ctrl.field] [INDEX_THIRD_DIGIT-1].value = MAX_DIGIT;
        }
    }
    else if(hmi_dash_set_cursor[hmi_dash_ctrl.field] [INDEX_SECOND_DIGIT-1].value  == MIN_DIGIT)
    {
        if(hmi_dash_set_cursor[hmi_dash_ctrl.field] [INDEX_THIRD_DIGIT-1].value > MAX_DIGIT )
        {
            hmi_dash_set_cursor[hmi_dash_ctrl.field] [INDEX_THIRD_DIGIT-1].value = MIN_DIGIT;
        }
    }
    value[hmi_dash_ctrl.field] += hmi_dash_edit[hmi_dash_ctrl.field].first_digit *  100;
    value[hmi_dash_ctrl.field] += hmi_dash_set_cursor[hmi_dash_ctrl.field][INDEX_SECOND_DIGIT - 1].value * 10;
    value[hmi_dash_ctrl.field] += hmi_dash_set_cursor[hmi_dash_ctrl.field][INDEX_THIRD_DIGIT - 1].value;
    hmi_dash_ctrl.target[hmi_dash_ctrl.field] = value[hmi_dash_ctrl.field];
}

/***********************************************************************************/

void hmi_dashboard_toggle_mode(void)
{
    switch (hmi_dash_ctrl.field)
    {
    case FIELD_VOLTAGE:
        hmi_dash_ctrl.field = FIELD_CURRENT;
        break;
    case FIELD_CURRENT:
        hmi_dash_ctrl.field = FIELD_VOLTAGE;
        break;
    default:
        break;
    }
}

/***********************************************************************************/

void hmi_dashboard_init(void)
{
    hmi_dash_ctrl.field = FIELD_VOLTAGE;
    hmi_dash_ctrl.out_status = OUT_STATUS_OFF;
    hmi_dash_edit[FIELD_VOLTAGE].index = INDEX_SECOND_DIGIT-1;
    hmi_dash_edit[FIELD_CURRENT].index = INDEX_THIRD_DIGIT-1;
    hmi_dash_ctrl.display_update = DISPLAY_NOT_UPDTATING_EVENT;
    hmi_dash_ctrl.cursor.blnk_state = CURSOR_STATE_IDLE;
}

/***********************************************************************************/

void hmi_dashboard_show_screen(void)
{
    vLCD_HD44780_Clear();
    vLCD_HD44780_Puts(0, 0, "Vs:");
    vLCD_HD44780_Puts(0, 1, "Vo:");

    vLCD_HD44780_Puts(11, 0, "Is:");
    vLCD_HD44780_Puts(11, 1, "Io:");

    hmi_dashboard_show_current();
    hmi_dashboard_show_voltage();
    hmi_dashboard_show_target_current();
    hmi_dashboard_show_target_voltage();
    hmi_dashboard_show_out_status();
    hmi_dashboard_show_temp();
    hmi_dashboard_hardware_status();
    hmi_dashboard_show_fan_porcentage();
    hmi_dashboard_show_power();

    hmi_dash_ctrl.display_update = DISPLAY_UPDATING_DATA;
}

/***********************************************************************************/

void hmi_dashboard_update_data(void)
{
    switch (hmi_dash_ctrl.display_update)
    {
    case DISPLAY_NOT_UPDTATING_EVENT:
        break;
    case DISPLAY_UPDATING_EVENT:
        hmi_dashboard_show_cursor();
        hmi_dashboard_show_out_status();
        hmi_dashboard_hardware_status();
        hmi_dash_ctrl.display_update = DISPLAY_UPDATING_DATA;
        break;
    case DISPLAY_UPDATING_DATA:
        hmi_dashboard_show_fan_porcentage();
        hmi_dashboard_show_voltage();
        hmi_dashboard_show_current();
        hmi_dashboard_show_power();
        hmi_dashboard_show_temp();
        hmi_dashboard_blnk_cursor();
    default:
        break;
    }
}

/***********************************************************************************/

void hmi_dashboard_exit(void)
{
    hmi_dash_ctrl.display_update = DISPLAY_NOT_UPDTATING_EVENT;
    vLCD_HD44780_Clear();
}

/***********************************************************************************/

static void hmi_dashboard_blnk_cursor(void)
{   
    switch (hmi_dash_ctrl.cursor.blnk_state)
    {
    case CURSOR_STATE_IDLE:
        hmi_dash_ctrl.cursor.blnk_state = CURSOR_STATE_SHOW_NUMBER;
        break;
    case CURSOR_STATE_SHOW_NUMBER:
        hmi_dashboard_show_target_voltage();
        hmi_dashboard_show_target_current();
        hmi_dash_ctrl.cursor.last_time_show_cursor = xTaskGetTickCount();
        hmi_dash_ctrl.cursor.blnk_state = CURSOR_STATE_WAIT_SHOW_DELAY;
        break;
    case CURSOR_STATE_WAIT_SHOW_DELAY:
        if(xTaskGetTickCount() - hmi_dash_ctrl.cursor.last_time_show_cursor >= 300)
        {
            hmi_dash_ctrl.cursor.blnk_state = CURSOR_STATE_HIDE_NUMBER;
        }
        break;
    case CURSOR_STATE_HIDE_NUMBER:
        hmi_dashboard_hide_cursor();
        hmi_dash_ctrl.cursor.last_time_hide_cursor = xTaskGetTickCount();
        hmi_dash_ctrl.cursor.blnk_state = CURSOR_STATE_WAIT_HIDE_DELAY;
        break;
    case CURSOR_STATE_WAIT_HIDE_DELAY:
        if(xTaskGetTickCount() - hmi_dash_ctrl.cursor.last_time_hide_cursor >= 50)
        {
            hmi_dash_ctrl.cursor.blnk_state = CURSOR_STATE_IDLE;
        }
        break;
    default:
        break;
    }
}

/***********************************************************************************/

void hmi_dashboard_update_button(button_id_t button_id, button_press_type_t button_press_type)
{
    hmi_dash_ctrl.display_update = DISPLAY_NOT_UPDTATING_EVENT;

    switch (button_id)
    {
    case BUTTON_LEFT_ID:
        hmi_dashboard_decrement_field();
        break;
    case BUTTON_RIGHT_ID:
        hmi_dashboard_increment_field();
        break;
    case BUTTON_SEL_CC_CV_ID:
        hmi_dashboard_out_state_toggle();
        hmi_dashboard_show_out_status();
        break;
    case BUTTON_OUT_STATE_ID:
        hmi_dashboard_toggle_mode();
        break;
    case BUTTON_ENC_ID:
        switch (button_press_type)
        {
        case BUTTON_SHORT_PRESS:
            hmi_dashboard_exit();
            hmi_set_screen(HMI_ID_SCREEN_MENU);
            break;
        case BUTTON_LONG_PRESS:
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    if(hmi_get_screen() != HMI_ID_SCREEN_MENU)
    {
        hmi_dash_ctrl.display_update = DISPLAY_UPDATING_EVENT;
    }
}

/***********************************************************************************/

void hmi_dashboard_update_encoder(enc_state_t enc_state)
{
    hmi_dash_ctrl.display_update = DISPLAY_NOT_UPDTATING_EVENT;

    switch (enc_state)
    {
    case ENC_STATE_CCW:
        hmi_dashboard_increment_digit();
        break;
    case ENC_STATE_CW:
         hmi_dashboard_decrement_digit();
        break;
    default:
        break;
    }

    hmi_dashboard_show_target_voltage();
    hmi_dashboard_show_target_current();
    hmi_dash_ctrl.display_update = DISPLAY_UPDATING_EVENT;
}

/***********************************************************************************/