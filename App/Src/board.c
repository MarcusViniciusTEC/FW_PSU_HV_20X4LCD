
#include "board.h"

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "hmi.h"
#include "encoder.h"
#include "LCD_HD44780.h"

#include "board_types.h"

/***********************************************************************************/

void board_thread(void const *pvParameters);
void board_init(void);
board_status_t board_get_status(void);

static board_ctrl_t board_ctrl = {0};

/***********************************************************************************/

void board_init(void)
{

    board_ctrl.status = BOARD_IS_INITIALIZING;

    TaskHandle_t xHandleBOARD = NULL;
    if (xTaskCreate((TaskFunction_t)board_thread,
                    "BOARD",
                    128,
                    NULL,
                    osPriorityNormal,
                    &xHandleBOARD) != pdPASS)
    {
    }
    else
    {
    }
}


/***********************************************************************************/

board_status_t board_get_status(void)
{
    return board_ctrl.status;
}


/***********************************************************************************/

void board_thread(void const *pvParameters)
{
    board_errors_t board_error = 0;
    vLCD_HD44780_Init();
    hmi_init();
    encoder_init();

    board_ctrl.status = BOARD_IS_READY;

    if(board_error == 0)
    {
       // HAL_GPIO_WritePin(BOARD_LED_GPIO_Port, BOARD_LED_Pin, GPIO_PIN_SET);
    }

    for(;;)
    {

        vTaskDelay(100);
    }
}

/***********************************************************************************/