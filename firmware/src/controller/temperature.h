#ifndef TEMPERATURE_H
#define	TEMPERATURE_H

#define THERMISTOR_BETA_VALUE 3428 // For temperatures up to ~80°C
#define IS_HIGH_SIDE 1
#define SERIES_RESISTOR 10000.0f

#define THERMISTOR_ADC_DIVISION 4

#include "gpio.h"

/**
 * @brief  Gets the temperature from the thermistor attached the common series connection.
 * @return The temperature in C.
 */
float temperature_get_series_reading();

/**
 * @brief  Gets the temperature from the thermistor attached the common ground connection.
 */
float temperature_get_ground_reading();

#endif /* TEMPERATURE_H */
