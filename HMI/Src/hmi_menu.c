
#include "hmi_menu.h"
#include "hmi_menu_types.h"

#include <stdbool.h>
#include <stdio.h>


#include "hmi.h"

/***********************************************************************************/

static const hmi_menu_data_t hmi_menu_item_vector[HMI_MENU_NUMBER_OF_ITEMS] = hmi_menu_vector_item_default;

static hmi_menu_ctrl_t hmi_menu_ctrl = {0};

/***********************************************************************************/

static void hmi_menu_show_menu_itens(void);
static void hmi_menu_show_menu_cursor(void);
static void hmi_menu_increment_item_index(void);
static void hmi_menu_decrement_item_index(void);



/***********************************************************************************/

void hmi_menu_init(void)
{
    hmi_menu_ctrl.submenu_state = SUBMENU_OFF;
    
}

/***********************************************************************************/

static void hmi_menu_increment_item_index(void)
{
    
    hmi_menu_ctrl.item_index++;
    if(hmi_menu_ctrl.item_index > HMI_MENU_ID_SETTINGS)
    {
        hmi_menu_ctrl.item_index = HMI_MENU_ID_SETTINGS;
    }
}

/***********************************************************************************/

static void hmi_menu_decrement_item_index(void)
{
    hmi_menu_ctrl.item_index--;
    if(hmi_menu_ctrl.item_index > HMI_MENU_ID_SETTINGS)
    {
        hmi_menu_ctrl.item_index = HMI_MENU_ID_DASHBOARD;
    }
}

/***********************************************************************************/
static void hmi_menu_show_menu_cursor(void)
{

}

/***********************************************************************************/

static void hmi_menu_show_menu_itens(void)
{

}

/***********************************************************************************/

void hmi_menu_show_screen(void)
{
    hmi_menu_show_menu_itens();
    hmi_menu_show_menu_cursor();
}

/***********************************************************************************/

void hmi_menu_update_data(void)
{
    
}

/***********************************************************************************/

void hmi_menu_update_button(button_id_t button_id, button_press_type_t button_press_type)
{
    switch (button_id)
    {
    case BUTTON_LEFT_ID:
        break;
    case BUTTON_RIGHT_ID:

        break;
    case BUTTON_ENC_ID:
        switch (button_press_type)
        {
        case BUTTON_SHORT_PRESS:
            switch (hmi_menu_item_vector[hmi_menu_ctrl.item_index].id)
            {
            case HMI_MENU_ID_DASHBOARD:
                hmi_set_screen(HMI_ID_SCREEN_DASHBOARD);
                break;
            case HMI_MENU_ID_VOLTAGE_RAMP:
                break;
            case HMI_MENU_ID_CURRENT_RAMP:
                break;
            case HMI_MENU_ID_SETTINGS:
                break;
            }
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

void hmi_menu_update_encoder(enc_state_t enc_state)
{
    switch (enc_state)
    {
    case ENC_STATE_CCW:
        hmi_menu_increment_item_index();
        break;
    case ENC_STATE_CW:
        hmi_menu_decrement_item_index();
        break;
    default:
        break;
    }

    hmi_menu_show_menu_cursor();
}

/***********************************************************************************/