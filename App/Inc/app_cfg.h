

#ifndef __APP_CFG_H_
#define __APP_CFG_H_

#define RESISTOR 10000.0f // Resistor do divisor de tensão
#define VREF 3.28f
#define BETA 2950.0f
#define T0 298.15f
#define R0_NTC 10000.0f

#define CALC_ADC_TO_VOLTAGE(adc_value, max_raw_value_adc)           (float) (adc_value*(VREF/max_raw_value_adc)) 
#define CALC_VOLTAGE_TO_RESISTENCE(voltage)                         (float) ((VREF/voltage - 1.0F)*RESISTOR)
#define CALC_RESISTANCE_TO_CELSIUS(resistence)                      (float) (((1.0f / (1.0f-T0 + (1.0f/BETA)*logf(resistence/R0_NTC)))- 273.15)*100)


#endif