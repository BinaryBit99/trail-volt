/* 
 * File:   sensors.h
 * Author: Kyle James
 *
 * Created on February 15, 2025, 6:43 PM
 */

#ifndef SENSORS_H
#define	SENSORS_H

#include "mcc_generated_files/system/system.h"
#include "mcc_generated_files/adc/adc0.h"

typedef struct {
    uint16_t cell_1_voltage_mv;
    uint16_t cell_2_voltage_mv;
    uint16_t cell_1_temperature_c;
    uint16_t cell_2_temperature_c;
    uint16_t charge_rate_ma;
    uint8_t values_valid;
} sensor_state_t;

#define ADC_ACCUMULATION 4

void update_sensor_state(sensor_state_t *current_state);

#endif	/* SENSORS_H */

