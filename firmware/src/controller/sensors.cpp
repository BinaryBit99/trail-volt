/*
 * File:   sensors.ino
 * Author: Kyle James (Arduino adaptation)
 *
 * Created on February 17, 2025
 */

#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>

#include "sensors.h"
#include "gpio.h"

/**
 * @brief  Converts an ADC value to millivolts
 * @param  adc_value: Raw ADC reading (0-1023 for Arduino)
 * @param  voltage_divider_ratio: The ratio by which the voltage is divided
 * @return Converted voltage in millivolts
 */
uint16_t adc_to_mv(uint16_t adc_value, float voltage_divider_ratio) {
    // Arduino uses 10-bit ADC (0-1023)
    float voltage = (adc_value * ADC_VREF) / 1024;
    return (uint16_t)(voltage * 1000 / voltage_divider_ratio);
}

/**
 * @brief  Reads value from specified digital pin using ADC
 * @param  pin: The digital pin number to read from
 * @param  voltage_divider_ratio: The ratio by which the voltage is divided
 * @return ADC reading in millivolts
 */
uint16_t read_from_adc(int pin, float voltage_divider_ratio) {
    // For Arduino Uno/Nano/etc, we need to use analogRead(A0 + pin)
    // This maps digital pins to their analog input numbers
    uint16_t raw_value = analogRead(pin);
    //Serial.print("ADC ("); Serial.print(pin); Serial.print("): "); Serial.println(raw_value);
    return adc_to_mv(raw_value, voltage_divider_ratio);
}

void update_battery_state(battery_state_t *battery_state) {
    int test = A0;

    // Read cell voltages
    battery_state->cell_1_voltage_mv = read_from_adc(CELL1_VOLTAGE_PIN, CELL_VOLTAGE_DIVIDER_RATIO);
    battery_state->cell_2_voltage_mv = read_from_adc(CELL2_VOLTAGE_PIN, CELL_VOLTAGE_DIVIDER_RATIO);
    
    // Read temperatures (no voltage divider)
    battery_state->cell_1_temperature_c = read_from_adc(CELL1_TEMP_PIN, 1.0);
    battery_state->cell_2_temperature_c = read_from_adc(CELL2_TEMP_PIN, 1.0);
}

void update_power_state(Adafruit_INA260 *ina260, power_state_t *power_state) {
    power_state->ina_bus_voltage = ina260->readBusVoltage();
    power_state->ina_current = ina260->readCurrent();
    power_state->ina_power = ina260->readPower();
}
