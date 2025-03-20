
#ifndef CHARGING_H
#define	CHARGING_H

#include "mode.h"

// App Voltage = ADC reading * CHARGE_VOLTAGE_DIVIDER_RATIO
#define MPPT_DIVIDER_RATIO 3.925f

#define MIN_CHARGE_CURRENT_A 0.5f
#define MAX_CHARGE_CURRENT_A 2.0f

typedef struct {
  float ina_current_ma;
  float ina_bus_voltage_v;
  float ina_power_w;
  float mppt_voltage_v;
} power_metrics_t;

typedef struct {
  uint8_t duty_mppt;
  uint8_t duty_load;
} duty_cycles_t;

typedef struct {
  bool is_charging;
  bool is_faulty;
  duty_cycles_t duty_cycles;
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
 * @brief  Adjusts the charging duty cycles based on the charging status provided.
 * @param  duty_cyles: The new duty cycles.
 */
void charging_set_duty_cycles(duty_cycles_t duty_cyles);

/**
 * @brief  Sets the shutdown pin
 */
void charging_stop();

/**
 * @brief  Determines whether the charging current is within the allowable limits.
 * @return True if within the allowable limits.
 */
bool charging_current_within_limits();

#endif	/* CHARGING_H */



