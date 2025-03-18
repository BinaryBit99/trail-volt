
#ifndef CHARGING_H
#define	CHARGING_H

#include "mode.h"

// App Voltage = ADC reading * CHARGE_VOLTAGE_DIVIDER_RATIO
#define CHARGE_VOLTAGE_DIVIDER_RATIO 3.925f

/**
 * The acceptable difference between CHARGING_VOLTAGE_V and our current charging.
 */
#define CHARING_VOLTAGE_TOLERANCE 0.050f

/*
 * At this error value, set the pwm step to minimum.
 */
#define SLOW_STEP_THESHOLD_V 1.00f

#define CHARGING_VOLTAGE_V 8.2f

#define MIN_CHARGE_CURRENT_A 0.5f
#define MAX_CHARGE_CURRENT_A 2.0f

typedef struct {
  float ina_current_ma;
  float ina_bus_voltage_v;
  float ina_power_w;
  float charge_voltage_v;
} power_metrics_t;

typedef struct {
  bool is_charging;
  bool is_faulty;
  uint8_t duty_cycle_uint8;
  power_metrics_t power_metrics;
} charging_state_t;

/**
 * @brief  Initialize charging.
 */
void charging_init();

/**
 * @brief  Get the charging state.
 * @return The charging state.
 */
charging_state_t charging_get_state();

/**
 * @brief  Updates the charging state.
 */
void charging_update_state();

/**
 * @brief  Determines if battery charging is enabled via hardware switch.
 * @return True if batteries are charging.
 */
bool charging_is_enabled();

/**
 * @brief  Adjusts the duty cycle based on the charging status provided.
 * @param  duty_cycle: The duty cycle.
 */
void charging_set_duty_cycle(uint8_t duty_cycle);

/**
 * @brief  Sets the shutdown pin
 */
void charging_stop();

/**
 * @brief  Calculates the new duty cycle applied to gate driver input.
 * @return The new calculated PWM duty cycle.
 */
uint8_t charging_calculate_duty_cycle();

/**
 * @brief  Determines whether the charging current is within the allowable limits.
 * @return True if within the allowable limits.
 */
bool charging_current_within_limits();

#endif	/* CHARGING_H */



