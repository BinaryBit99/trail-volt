
#ifndef BATTERIES_H
#define	BATTERIES_H

#include "state.h"

#define TOTAL_CELL_ADC_DIVISION 1
#define LOWER_CELL_ADC_DIVISION 1

/**
 * The tolerable difference between the upper and lower cell voltages.
 */
#define BALANCE_THRESHOLD_V 0.1

/**
 * @brief  Sets GPIO pin fed to upper cell discharge accordingly.
 * @param  discharge: True to discharge the cell.
 */
void set_upper_discharge(bool discharge);

/**
 * @brief  Sets GPIO pin fed to lower cell discharge accordingly.
 * @param  discharge: True to discharge the cell.
 */
void set_lower_discharge(bool discharge);

/**
 * @brief  Updates the battery readings based on values taken from ADC0.
 * @param  battery_status: A pointer to the global battery status.
 */
void update_battery_readings(battery_status_t *battery_status);

/**
 * @brief  Determines whether cell balancing is needed based on the battery status.
 * @param  status: A pointer to the global battery status.
 * @return True if balancing is needed.
 */
bool is_balancing_needed(const battery_status_t* status);

/**
 * @brief  Get the voltage difference between the upper cell voltage and the lower cell voltage.
 * @param  status: A pointer to the global battery status.
 * @return The difference between the upper and lower cell voltages.
 */
float get_cell_voltage_difference(const battery_status_t* status);

#endif	/* BATTERIES_H */
