#ifndef MPPT_H
#define	MPPT_H

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

/**
 * The acceptable difference between CHARGING_VOLTAGE_V and our current charging.
 */
#define CHARING_VOLTAGE_TOLERANCE 0.050f
/*
 * At this error value, set the pwm step to minimum.
 */
#define SLOW_STEP_THESHOLD_V 1.00f
#define CHARGING_VOLTAGE_V 8.2f


//tuning parameters
#define TOLERANCE 0.01f       // tolerance for MPP condition
#define STEP_SIZE 1           // linear step sizes for now that are small for testing purposes
#define MAX_DUTY 1.0f         // Maximum duty cycle
#define MIN_DUTY 0.0f         // Minimum duty cycle

duty_cycles_t mppt_calculate_duty_cycles(charging_state_t charging_state);

#endif /* MPPT_H */
