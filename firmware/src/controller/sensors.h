/* 
 * File:   sensors.h
 * Author: Kyle James
 *
 * Created on February 15, 2025, 6:43 PM
 */

#ifndef SENSORS_H
#define	SENSORS_H

#include <stdint.h>

// Configuration constants
const float CELL_VOLTAGE_DIVIDER_RATIO = 1; // Assuming 3:1 voltage divider
const float ADC_VREF = 5; // Arduino reference voltage

// Pin definitions using digital pins
const int CELL1_VOLTAGE_PIN = A0;
const int CELL2_VOLTAGE_PIN = A1;
const int CELL1_TEMP_PIN = A2;     
const int CELL2_TEMP_PIN = A3;  
// A4 & A5 used for I2C  
const int CHARGE_RATE_PIN = A6;    

typedef struct {
    uint16_t cell_1_voltage_mv;
    uint16_t cell_2_voltage_mv;
    uint16_t cell_1_temperature_c;
    uint16_t cell_2_temperature_c;
    uint16_t charge_rate_ma;
    uint8_t values_valid;
} sensor_state_t;

#define CELL_VOLTAGE_DIVIDER_RATIO 1

/**
 * @brief  Updates sensor state based on values taken from ADC0.
 * @param  sensor_state: A pointer to the application sensor state.
 */
void update_sensor_state(sensor_state_t *sensor_state);

#endif	/* SENSORS_H */


