
#include "hmi_intro.h"
#include "hmi.h"

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "LCD_HD44780.h"

#include "board.h"



/***********************************************************************************/

void hmi_intro_init(void)
{

}

/***********************************************************************************/

void hmi_intro_show_screen(void)
{

    vLCD_HD44780_Clear();
    vLCD_HD44780_Puts(0, 1, "HV-PSU 0-500V 250W");
    vLCD_HD44780_Puts(5, 2, "MV TRONIX");
    vLCD_HD44780_Puts(5, 3, "Iniciando...");

    vTaskDelay(pdMS_TO_TICKS(2000));
    if(board_get_status() == BOARD_IS_READY)
    {
        
        hmi_set_screen(HMI_ID_SCREEN_DASHBOARD);
    }
}

/***********************************************************************************/

void hmi_intro_update_data(void)
{

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