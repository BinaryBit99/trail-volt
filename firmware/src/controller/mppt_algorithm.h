#ifndef TEMPERATURE_H
#define	TEMPERATURE_H

#include "charging.h"
#include "gpio.h"
#include "adc.h"
#include <math.h>
#include <Adafruit_INA260.h>
#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

//tuning parameters
#define TOLERANCE 0.01f       // tolerance for MPP condition
#define STEP_SIZE 0.001f      // linear step sizes for now that are small for testing purposes
#define MAX_DUTY 1.0f         // Maximum duty cycle
#define MIN_DUTY 0.0f         // Minimum duty cycle

float round_to_nearest_tenth(float value);

duty_cycles_t mppt_calculate_duty_cycles(charging_state_t charging_state);

#endif
