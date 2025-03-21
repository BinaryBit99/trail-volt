
#ifndef ADC_H
#define	ADC_H

#define ADC_VREF 5.0f

/**
 * @brief  Converts an ADC value to millivolts
 * @param  adc_value: Raw ADC reading (0-1023 for Arduino)
 * @param  voltage_divider_ratio: The ratio by which the voltage is divided
 * @return Converted voltage in volts
 */
float adc_to_v(uint16_t adc_value, float voltage_divider_ratio);

/**
 * @brief  Reads value from specified digital pin using ADC
 * @param  pin: The digital pin number to read from
 * @param  voltage_divider_ratio: The ratio by which the voltage is divided
 * @return ADC reading in volts
 */
float read_from_adc(int pin, float voltage_divider_ratio);

#endif	/* ADC_H */
