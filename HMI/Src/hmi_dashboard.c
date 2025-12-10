#include "hmi_menu.h"
#include "hmi_menu_types.h"

#include <stdbool.h>
#include <stdio.h>


#include "hmi.h"

#include "main.h"
#include "adc.h"
#include "app.h"

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "hmi_dashboard_types.h"


/***********************************************************************************/



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


/***********************************************************************************/



/***********************************************************************************/

static void hmi_dashboard_show_current_in(void)
{

}

/***********************************************************************************/

static void hmi_dashboard_show_current(void)
{

}

/***********************************************************************************/

static void hmi_dashboard_show_voltage(void)
{
  
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

}

/***********************************************************************************/

static void hmi_dashboard_show_target_current(void)
{

}

/***********************************************************************************/

static void hmi_dashboard_show_cursor(void)
{

}

static void hmi_dashboard_clear_cursor_last_mode(void)
{
    

}

/***********************************************************************************/
static void hmi_dashboard_increment_field(void)
{

}

/***********************************************************************************/

static void hmi_dashboard_decrement_field(void)
{

}

/***********************************************************************************/

static void hmi_dashboard_increment_digit(void)
{

}

/***********************************************************************************/

static void hmi_dashboard_decrement_digit(void)
{

}

/***********************************************************************************/

void hmi_dashboard_toggle_mode(void)
{

}

/***********************************************************************************/

void hmi_dashboard_init(void)
{

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

        break;
    case BUTTON_RIGHT_ID:

        break;
    case BUTTON_OUT_STATE_ID:

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

}

/***********************************************************************************/

void hmi_dashboard_update_encoder(enc_state_t enc_state)
{


    switch (enc_state)
    {
    case ENC_STATE_CCW:

        break;
    case ENC_STATE_CW:

        break;
    default:
        break;
    }

}

/***********************************************************************************/