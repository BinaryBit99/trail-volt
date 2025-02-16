/* 
 * File:   sensors.c
 * Author: Kyle James
 *
 * Created on February 15, 2025, 9:29 PM
 */

#include "sensors.h"
#include <avr/delay.h>

uint16_t convert_adc_value(uint16_t adc_value);
uint16_t read_from_adc(adc_channel_t channel);

void update_sensor_state(sensor_state_t *current_state)
{
    adc_result_t value;
    
    /* ADC0 */
    value = read_from_adc(ADC0_CHANNEL_AIN0);
    current_state->cell_1_voltage_mv = value;
    
    /* ADC1 */
    value = read_from_adc(ADC0_CHANNEL_AIN1);
    current_state->cell_2_voltage_mv = value;
    
    /* ADC2 */
    value = read_from_adc(ADC0_CHANNEL_AIN2);
    current_state->cell_1_temperature_c = value;
    
    /* ADC3 */
    value = read_from_adc(ADC0_CHANNEL_AIN3);
    current_state->cell_2_temperature_c = value;
    
    /* ADC4 */
    value = read_from_adc(ADC0_CHANNEL_AIN4);
    current_state->charge_rate_ma = value;
    
    // Set valid
    current_state->values_valid = true;
}

uint16_t read_from_adc(adc_channel_t channel)
{
    ADC0_ChannelSelect(channel);
    _delay_us(10);
    
    ADC0_ConversionStart();
    while (!ADC0_IsConversionDone());
    
    adc_result_t adc_result = ADC0_ConversionResultGet();
    ADC0_ConversionDoneInterruptFlagClear();
    
    return convert_adc_value(adc_result);
}

uint16_t convert_adc_value(uint16_t adc_value)
{
    return adc_value / ADC_ACCUMULATION;
}
    


