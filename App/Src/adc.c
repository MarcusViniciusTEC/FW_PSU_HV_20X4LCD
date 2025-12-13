
#include "adc.h"
#include "adc_types.h"
#include "ADS1015_ADS1115.h"

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#define NUMBER_OF_MODULES_ADC 1
#define MODULE_DEFAULT 0

/***********************************************************************************/

ADS1xx5_I2C adc;


/***********************************************************************************/

void adc_thread(void const *pvParameters);
static adc_ctrl_t adc_ctrl[ADC_NUMBER_OF_CHANELS] = {0};

static uint16_t window[ADC_NUMBER_OF_CHANELS][MEDIAN_SIZE] = {0};
static uint8_t index_adc[ADC_NUMBER_OF_CHANELS] = {0};

/***********************************************************************************/

void adc_init(void);
void ADC_ADS1115_init(void);
uint32_t ADC_ADS1115_get_raw(adc_channels_t channel);

/***********************************************************************************/

adc_res_divider_stauts_t adc_get_res_divider_status(void)
{
    return adc_ctrl[ADC_CH_VOLTAGE].resistor_status;
}

/***********************************************************************************/

void adc_init(void)
{
    ADC_ADS1115_init();

    TaskHandle_t xHandle = NULL;
    if (xTaskCreate((TaskFunction_t)adc_thread,
                    "ADC",
                    512,
                    NULL,
                    osPriorityRealtime,
                    &xHandle) != pdPASS)
    {
    }
    else
    {
        HAL_GPIO_WritePin(LED_BOARD_GPIO_Port, LED_BOARD_Pin, 1);
    }
}

/***********************************************************************************/

void ADC_ADS1115_init(void)
{
    ADS1115(&adc, &hi2c1, ADS_ADDR_GND);
    ADSsetGain(&adc, GAIN_ONE);
}

/***********************************************************************************/

uint32_t median_filter(uint8_t ch, uint16_t new_sample)
{
    window[ch][index_adc[ch]] = new_sample;

    index_adc[ch]++;
    if (index_adc[ch] >= MEDIAN_SIZE)
        index_adc[ch] = 0;

    uint16_t temp[MEDIAN_SIZE];
    for (uint8_t i = 0; i < MEDIAN_SIZE; i++)
        temp[i] = window[ch][i];

    for (uint8_t i = 0; i < MEDIAN_SIZE - 1; i++)
        for (uint8_t j = i + 1; j < MEDIAN_SIZE; j++)
            if (temp[j] < temp[i]) {
                uint16_t t = temp[i];
                temp[i] = temp[j];
                temp[j] = t;
            }
    return temp[MEDIAN_SIZE / 2];
}

/***********************************************************************************/

uint16_t ema_filter(adc_channels_t channel, uint16_t new_sample)
{
    static uint16_t filtered[ADC_NUMBER_OF_CHANELS] = {0};
    filtered[channel] = filtered[channel] + ((new_sample - filtered[channel]) / 2);
    return filtered[channel];
}

/***********************************************************************************/

uint32_t ADC_ADS1115_get_raw(adc_channels_t channel)
{
   return adc_ctrl[channel].raw_value; 
}   

/***********************************************************************************/

uint32_t ADC_ADS1115_GET_Raw_temp(adc_channels_temp_t temp_channel)
{
    return adc_ctrl[ADC_CH_TEMP].raw_value_temp[temp_channel];
}

/***********************************************************************************/

static void mux_select_channel(adc_channels_t channel)
{
    switch (channel)
    {
    case ADC_TEMP_CH_TRANSFORMER:
        HAL_GPIO_WritePin(MUX_S0_GPIO_Port, MUX_S0_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MUX_S1_GPIO_Port, MUX_S1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MUX_S2_GPIO_Port, MUX_S2_Pin, GPIO_PIN_RESET);
        break;
    case ADC_TEMP_CH_HEATSINK:
        HAL_GPIO_WritePin(MUX_S0_GPIO_Port, MUX_S0_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MUX_S1_GPIO_Port, MUX_S1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MUX_S2_GPIO_Port, MUX_S2_Pin, GPIO_PIN_RESET);
        break;
    default:
        break;
    }
}

/***********************************************************************************/

static void adc_read_temp(void)
{
    for(uint8_t index_sense_temp = 0; index_sense_temp < NUMBER_OF_SENSE_TEMP; index_sense_temp++)
    {
        mux_select_channel(index_sense_temp);
        vTaskDelay(pdMS_TO_TICKS(10));
        adc_ctrl[ADC_CH_TEMP].raw_value_temp[index_sense_temp] = ADSreadADC_SingleEnded(&adc, ADC_CH_TEMP);
    }
}

/***********************************************************************************/

void adc_thread(void const *pvParameters)
{
    for (;;)
    {
        for(uint8_t channel_index = 0; channel_index < ADC_NUMBER_OF_CHANELS; channel_index++)
        {  
            adc_ctrl[channel_index].raw_value = ADSreadADC_SingleEnded(&adc, channel_index);
            adc_read_temp();
        }      
        vTaskDelay(50);
    }
}

/***********************************************************************************/