/* 
 * File:   sensors.h
 * Author: Kyle James
 *
 * Created on February 15, 2025, 6:43 PM
 */

#ifndef SENSORS_H
#define	SENSORS_H

#include <stdint.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_INA260.h>

#include "state.h"
#include "gpio.h"

const int pin = A0;

// Configuration constants
const float CELL_VOLTAGE_DIVIDER_RATIO = 1; // Assuming 3:1 voltage divider
const float ADC_VREF = 5; // Arduino reference voltage


#define CELL_VOLTAGE_DIVIDER_RATIO 1

/**
 * @brief  Updates sensor state based on values taken from ADC0.
 * @param  battery_state: A pointer to the global battery state.
 */
void update_battery_state(battery_state_t *battery_state);


/**
 * @brief  Reads current from INA260.
 */
void update_power_state(Adafruit_INA260 *ina260, power_state_t *power_state);

#endif	/* SENSORS_H */


