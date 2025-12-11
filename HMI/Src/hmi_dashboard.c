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


/***********************************************************************************/



static hmi_dash_ctrl_t hmi_dash_ctrl = {0};
static hmi_dash_edit_t hmi_dash_edit[NUMBER_OF_FIELDS] = {0};
hmi_dash_set_cursor_t hmi_dash_set_cursor[NUMBER_OF_FIELDS][NUMBER_OF_FIELDS] = cursor_set_edit_default;


/***********************************************************************************/

static void hmi_dashboard_show_voltage(void);
static void hmi_dashboard_show_power(void);
static void hmi_dashboard_show_current(void);
static void hmi_dashboard_show_temp(void);
static void hmi_dashboard_show_time(void);
static void hmi_dashboard_show_out_status(void);
static void hmi_dashboard_show_target_voltage(void);
static void hmi_dashboard_show_target_current(void);
static void hmi_dashboard_show_cursor(void);
static void hmi_dashboard_show_current_in(void);
static void hmi_dashboard_increment_digit(void);
static void hmi_dashboard_decrement_digit(void);
static void hmi_dashboard_increment_field(void);
static void hmi_dashboard_decrement_field(void);
static void hmi_dashboard_out_state_toggle(void);
static void hmi_dashboard_exit(void);
static void hmi_dashboard_toggle_mode(void);
static void hmi_dashboard_clear_cursor_last_mode(void);


/***********************************************************************************/

uint32_t hmi_dashboard_get_target_current(void)
{

}

/***********************************************************************************/

uint32_t hmi_dashboard_get_target_voltage(void)
{   

}

/***********************************************************************************/


static void hmi_dashboard_show_current_in(void)
{

}

/***********************************************************************************/

static void hmi_dashboard_show_current(void)
{
    vLCD_HD44780_Puts(14, 1, "1.500A");
}

/***********************************************************************************/

static void hmi_dashboard_show_voltage(void)
{
    
     vLCD_HD44780_Puts(4, 1, "500.0V");
  
}

/***********************************************************************************/

static void hmi_dashboard_show_power(void)
{


}

/***********************************************************************************/

static void hmi_dashboard_show_temp(void)
{


}

/***********************************************************************************/

static void hmi_dashboard_show_time(void)
{
  
}

/***********************************************************************************/

static void hmi_dashboard_out_state_toggle(void)
{
    
}

/***********************************************************************************/

static void hmi_dashboard_show_out_status(void)
{

}

/***********************************************************************************/

static void hmi_dashboard_show_target_voltage(void)
{
    char sz_string[20] = {0};
    snprintf(sz_string, sizeof(sz_string), "%u%u", hmi_dash_set_cursor[FIELD_VOLTAGE]  [INDEX_SECOND_DIGIT-1].value, hmi_dash_set_cursor[FIELD_VOLTAGE]  [INDEX_THIRD_DIGIT-1].value);

    vLCD_HD44780_Puts(5, 0, sz_string);
}

/***********************************************************************************/

static void hmi_dashboard_show_target_current(void)
{
    char sz_string[20] = {0};
    snprintf(sz_string, sizeof(sz_string), "%u%u", hmi_dash_set_cursor[FIELD_CURRENT]  [INDEX_SECOND_DIGIT-1].value, hmi_dash_set_cursor[FIELD_CURRENT]  [INDEX_THIRD_DIGIT-1].value);

    vLCD_HD44780_Puts(16, 0, sz_string);
}

/***********************************************************************************/

static void hmi_dashboard_show_cursor(void)
{

    
    vLCD_HD44780_CursorSet(hmi_dash_set_cursor[hmi_dash_ctrl.field][ hmi_dash_edit[hmi_dash_ctrl.field].index ].col, hmi_dash_set_cursor[hmi_dash_ctrl.field][hmi_dash_edit[hmi_dash_ctrl.field].index].row);
    
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
    hmi_dash_set_cursor  [hmi_dash_ctrl.field]  [hmi_dash_edit[hmi_dash_ctrl.field].index]  .value++;
}

/***********************************************************************************/

static void hmi_dashboard_decrement_digit(void)
{
    hmi_dash_set_cursor  [hmi_dash_ctrl.field]  [hmi_dash_edit[hmi_dash_ctrl.field].index]  .value--;
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
    vLCD_HD44780_Clear();

    hmi_dash_ctrl.field = FIELD_VOLTAGE;
    hmi_dash_edit[hmi_dash_ctrl.field].index =0;

    vLCD_HD44780_CursorOn();
    vLCD_HD44780_Puts(0, 0, "SET:");
    vLCD_HD44780_Puts(0, 1, "OUT:");

    hmi_dashboard_show_current();
    hmi_dashboard_show_voltage();
    hmi_dashboard_show_target_current();
    hmi_dashboard_show_target_voltage();
    hmi_dashboard_show_cursor();
    
}

/***********************************************************************************/

void hmi_dashboard_show_screen(void)
{

}

/***********************************************************************************/

void hmi_dashboard_update_data(void)
{

}

/***********************************************************************************/

void hmi_dashboard_exit(void)
{

}

/***********************************************************************************/

void hmi_dashboard_update_button(button_id_t button_id, button_press_type_t button_press_type)
{

    switch (button_id)
    {
    case BUTTON_LEFT_ID:
        hmi_dashboard_decrement_field();
        break;
    case BUTTON_RIGHT_ID:
        hmi_dashboard_increment_field();
        break;
    case BUTTON_OUT_STATE_ID:
        hmi_dashboard_toggle_mode();
        break;
    case BUTTON_ENC_ID:
        switch (button_press_type)
        {
        case BUTTON_SHORT_PRESS:

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

    hmi_dashboard_show_cursor();

}

/***********************************************************************************/

void hmi_dashboard_update_encoder(enc_state_t enc_state)
{
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


    hmi_dashboard_show_target_current();
    hmi_dashboard_show_target_voltage();
    hmi_dashboard_show_cursor();

}

/***********************************************************************************/