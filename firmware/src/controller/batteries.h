
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
 * @brief  Sets GPIO pins fed to cell balancing circuit and sets is_balancing flag.
 * @param  battery_status: A pointer to the current battery status.
 */
void balance_cells(battery_status_t *battery_status);

/**
 * @brief  Updates sensor state based on values taken from ADC0.
 * @param  battery_status: A pointer to the global battery status.
* @return  True is balancing is needed.
 */
bool update_battery_status(battery_status_t *battery_status);

#endif	/* BATTERIES_H */
