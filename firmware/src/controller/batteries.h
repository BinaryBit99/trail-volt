#ifndef BATTERIES_H
#define	BATTERIES_H

#include "state.h"

#define TOTAL_CELL_ADC_DIVISION 1
#define LOWER_CELL_ADC_DIVISION 1

#define BALANCE_THRESHOLD_MV 100

/**
 * @brief  Sets GPIO pins fed to cell balancing circuit and sets is_balancing flag.
 * @param  battery_status: A pointer to the current battery status.
 */
void handle_cell_balancing(battery_status_t *battery_status);

#endif	/* BATTERIES_H */
