
#include <Arduino.h>
#include "adc.h"

int adc_to_mv(uint16_t adc_value, float voltage_divider_ratio) {
    // Arduino uses 10-bit ADC (0-1023)
    float voltage = (adc_value * (float)ADC_VREF) / 1024;
    return (voltage * 1000 / voltage_divider_ratio);
}

int read_from_adc(int pin, float voltage_divider_ratio) {
    uint16_t raw_value = analogRead(pin);
    return adc_to_mv(raw_value, voltage_divider_ratio);
}
