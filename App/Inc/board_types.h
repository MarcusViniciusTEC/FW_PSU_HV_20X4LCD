#ifndef __BOARD_TYPES_H_
#define __BOARD_TYPES_H_



/***********************************************************************************/





typedef enum
{
    NO_ERROR = 0U,
    ERRO_ENCORDER_INIT, 
    ERRO_HMI_INIT,
    ERRO_APP_INIT,
    ERRO_LEDS_INIT,
    ERRO_LED_INIT,
    ERRO_ADC_INIT,
    ERRO_BUZZER_INIT,
    ERRO_DAC_INIT,
    NUMBER_OF_ERRORS

}board_errors_t;

typedef enum
{
    BOARD_IS_READY = 0U,
    BOARD_IS_INITIALIZING,
    BOARD_IS_FAULT,
}board_status_t;


typedef struct 
{
    board_status_t status;
}board_ctrl_t;


/***********************************************************************************/

#endif