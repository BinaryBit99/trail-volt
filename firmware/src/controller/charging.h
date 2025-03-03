
#ifndef CHARGING_H
#define	CHARGING_H

#include "state.h"
#include <Adafruit_INA260.h>

#define DUTY_CYCLE_STEP_UINT8 8

#define CHARGE_VOLTAGE_DIVIDER_RATIO 1.7

/**
 * The acceptable difference between CHARGING_VOLTAGE_V and our current charging.
 */
#define CHARING_VOLTAGE_TOLERANCE 0.050

#define CHARGING_VOLTAGE_V 8.2

/**
 * @brief  Determines if battery are in a state of charging.
 * @return True if batteries are charging.
 */
bool is_receiving_charge();

/**
 * @brief  Configures the charging pwm signal.
 */
void init_pwm();

/**
 * @brief  Adjusts the duty cycle based on the charging status provided.
 * @param  duty_cycle: The duty cycle.
 */
void set_charging_duty_cycle(uint8_t duty_cycle);

/**
 * @brief  Reads current from INA260.
 * @param  ina260: A pointer to the global INA260 instance.
 * @param  power_metrics: A pointer to the global power metrics.
 */
void update_power_metrics(Adafruit_INA260 *ina260, power_metrics_t *power_metrics);

/**
 * @brief  Sets the shutdown pin
 * @param  ina260: A pointer to the global INA260 instance.
 * @param  power_metrics: A pointer to the global power metrics.
 */
void stop_charging();

/**
 * @brief  Calculates the new duty cycle applied to gate driver input.
 * @param  old_duty_cycle: The duty cycle currently applied to the gate driver.
 * @return The new calculated PWM duty cycle.
 */
uint8_t calculate_duty_cycle(const charging_status_t *status);

#endif	/* CHARGING_H */



