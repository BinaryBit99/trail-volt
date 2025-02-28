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

const float ADC_VREF = 5; // Arduino reference voltage


#define CELL_VOLTAGE_DIVIDER_RATIO 1

/**
 * @brief  Updates sensor state based on values taken from ADC0.
 * @param  battery_status: A pointer to the global battery status.
 */
void update_battery_status(battery_status_t *battery_status);


/**
 * @brief  Reads current from INA260.
 * @param  ina260: A pointer to the global INA260 instance.
 * @param  power_metrics: A pointer to the global power metrics.
 */
void update_power_metrics(Adafruit_INA260 *ina260, power_metrics_t *power_metrics);

#endif	/* SENSORS_H */


