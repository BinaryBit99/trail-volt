
#ifndef BATTERY_H
#define	BATTERY_H

#include "mode.h"

#define TOTAL_CELL_ADC_DIVISION 1
#define LOWER_CELL_ADC_DIVISION 1

/**
 * The tolerable difference between the upper and lower cell voltages.
 */
#define BALANCE_THRESHOLD_V 0.1

typedef struct {
    float upper_cell_voltage_v;
    float lower_cell_voltage_v;
    float cell_1_temperature_c;
    float cell_2_temperature_c;
    bool upper_discharging;
    bool lower_discharging;
} battery_state_t;

/**
 * @brief  Gets the battery state.
 * @return The battery state.
 */
battery_state_t battery_get_state();

/**
 * @brief  Sets GPIO pin fed to upper cell discharge accordingly.
 * @param  discharge: True to discharge the cell.
 */
void battery_set_upper_discharge(bool discharge);

/**
 * @brief  Sets GPIO pin fed to lower cell discharge accordingly.
 * @param  discharge: True to discharge the cell.
 */
void battery_set_lower_discharge(bool discharge);

/**
 * @brief  Updates the battery state based on values taken from ADC0.
 */
void battery_update_state();

/**
 * @brief  Determines whether cell balancing is needed based on the battery status.
 * @return True if balancing is needed.
 */
bool battery_balancing_needed();

/**
 * @brief  Get the voltage difference between the upper cell voltage and the lower cell voltage.
 * @return The difference between the upper and lower cell voltages.
 */
float battery_get_voltage_difference();

#endif	/* BATTERY_H */

