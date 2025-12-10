
#include "hmi_intro.h"
#include "hmi.h"

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"


/***********************************************************************************/

void hmi_intro_init(void)
{

}

/***********************************************************************************/

void hmi_intro_show_screen(void)
{
    
}

/***********************************************************************************/

void hmi_intro_update_data(void)
{
    vTaskDelay(pdMS_TO_TICKS(1000));
    hmi_set_screen(HMI_ID_SCREEN_MENU);
}

/***********************************************************************************/

void hmi_intro_update_button(button_id_t button_id, button_press_type_t button_press_type)
{

}

/***********************************************************************************/

void hmi_intro_update_encoder(enc_state_t enc_state)
{


}

/***********************************************************************************/