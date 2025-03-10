#ifndef TEMPERATURE_H
#define	TEMPERATURE_H

#define THERMISTOR_BETA_VALUE 3428 // For temperatures up to ~80°C
#define IS_HIGH_SIDE 1
#define SERIES_RESISTOR 10000.0f

#define THERMISTOR_ADC_DIVISION 4

#include "gpio.h"

/**
 * @brief  Gets the temperature from the thermistor attached the upper cell.
 * @return The temperature in C.
 */
float temperature_get_upper_cell();

/**
 * @brief  Gets the temperature from the thermistor attached the lower cell.
 * @return The temperature in C.
 */
float temperature_get_lower_cell();

#endif /* TEMPERATURE_H */
