#ifndef CHARGING_H
#define	CHARGING_H

#include "state.h"
#include <Adafruit_INA260.h>

#define MPPT_STEP_SIZE 1

#define CHARGE_VOLTAGE_DIVIDER_RATIO (13/18)

/**
 * @brief  Determines if battery are in a state of charging.
 * @return 1 if batteries are charging, 0 if not.
 */
int is_receiving_charge();

/**
 * @brief  Updates the global charging status and calculates the appropriate PWM duty cycle.
 * @param  charging_status: A pointer to the global charging status.
 * @return The new calculated PWM duty cycle.
 */
uint8_t update_charging_status(charging_status_t *charging_status, const Adafruit_INA260 *ina260);

/**
 * @brief  Configures of MPPT pwm signal.
 */
void init_pwm();

/**
 * @brief  Adjusts the duty cycle based on the charging status provided.
 * @param  charge_status: A pointer to the global charging status.
 */
void adjust_duty_cycle(const charging_status_t *charging_status);

/**
 * @brief  Reads current from INA260.
 * @param  ina260: A pointer to the global INA260 instance.
 * @param  power_metrics: A pointer to the global power metrics.
 */
void update_power_metrics(Adafruit_INA260 *ina260, power_metrics_t *power_metrics);

#endif	/* CHARGING_H */



