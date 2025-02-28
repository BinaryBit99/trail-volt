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
#include "batteries.h"

/**
 * @brief  Converts an ADC value to millivolts
 * @param  adc_value: Raw ADC reading (0-1023 for Arduino)
 * @param  voltage_divider_ratio: The ratio by which the voltage is divided
 * @return Converted voltage in millivolts
 */
int adc_to_mv(uint16_t adc_value, float voltage_divider_ratio) {
    // Arduino uses 10-bit ADC (0-1023)
    float voltage = (adc_value * ADC_VREF) / 1024;
    return (voltage * 1000 / voltage_divider_ratio);
}

/**
 * @brief  Reads value from specified digital pin using ADC
 * @param  pin: The digital pin number to read from
 * @param  voltage_divider_ratio: The ratio by which the voltage is divided
 * @return ADC reading in millivolts
 */
int read_from_adc(int pin, float voltage_divider_ratio) {
    uint16_t raw_value = analogRead(pin);
    return adc_to_mv(raw_value, voltage_divider_ratio);
}

void update_battery_status(battery_status_t *battery_status) {
    // Read cell voltages
    int total_voltage = read_from_adc(TOTAL_CELL_ADC_PIN, TOTAL_CELL_ADC_DIVISION);

    battery_status->lower_cell_voltage_mv = read_from_adc(LOWER_CELL_ADC_PIN, LOWER_CELL_ADC_DIVISION);
    battery_status->upper_cell_voltage_mv = total_voltage - battery_status->lower_cell_voltage_mv;

    // Read temperatures (no voltage divider)
    battery_status->cell_1_temperature_c = read_from_adc(CELL1_TEMP_PIN, 1.0);
    battery_status->cell_2_temperature_c = read_from_adc(CELL2_TEMP_PIN, 1.0);
}

void update_power_metrics(Adafruit_INA260 *ina260, power_metrics_t *power_metrics) {
    power_metrics->ina_bus_voltage = ina260->readBusVoltage();
    power_metrics->ina_current = ina260->readCurrent();
    power_metrics->ina_power = ina260->readPower();
}
