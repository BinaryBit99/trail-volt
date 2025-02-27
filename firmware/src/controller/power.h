/* 
 * File:   power.h
 * Author: Kyle James
 *
 * Created on February 17, 2025, 12:18 PM
 */

#ifndef POWER_H
#define	POWER_H

#include "state.h"
#include <Adafruit_INA260.h>

#define MPPT_STEP_SIZE 1

/**
 * @brief  Determines if battery are in a state of charging.
 * @return 1 if batteries are charging, 0 if not.
 */
int is_charging();

/**
 * @brief  Updates the global charge_state parameters and calculates the appropriate PWM duty cycle.
 * @param  charge_state: The global charge_state.
 * @return The new calculated PWM duty cycle.
 */
uint16_t update_charge_state(charge_state_t *charge_state, const Adafruit_INA260 *ina260);

/**
 * @brief  Configures of MPPT pwm signal.
 */
void init_pwm();

/**
 * @brief  Adjusts the duty cycle based on the charge state provided.
 * @param  charge_state: The global charge_state.
 */
void adjust_duty_cycle(const charge_state_t *charge_state);



#endif	/* POWER_H */


