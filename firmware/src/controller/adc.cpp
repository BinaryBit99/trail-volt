#include <Arduino.h>
#include "adc.h"

#define ADC_SAMPLES 10

float adc_to_v(uint16_t adc_value, float voltage_divider_ratio) {
    float voltage = (adc_value * (float)ADC_VREF) / 1023;
    return voltage * voltage_divider_ratio;
}

float read_from_adc(int pin, float voltage_divider_ratio) {
    uint32_t total = 0;

    for (int i = 0; i < ADC_SAMPLES; i++) {
        total += analogRead(pin);
        delay(2);  // Small delay to ensure stable readings
    }

    uint16_t averaged_value = total / ADC_SAMPLES;
    return adc_to_v(averaged_value, voltage_divider_ratio);
}
