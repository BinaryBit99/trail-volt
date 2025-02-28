/* 
 * File:   state.h
 * Author: Kyle James
 *
 * Created on February 17, 2025, 11:55 AM
 */

#ifndef STATE_H
#define	STATE_H

#include <stdint.h>

typedef enum {
  STATE_SLEEP,
  STATE_RECEIVING,
  STATE_SUPPLYING,
  STATE_MONITORING
} application_mode_t;

typedef struct {
    int upper_cell_voltage_mv;
    int lower_cell_voltage_mv;
    int cell_1_temperature_c;
    int cell_2_temperature_c;
    bool upper_discharging;
    bool lower_discharging;
} battery_status_t;

/* Child of charging status */
typedef struct {
  float ina_current;
  float ina_bus_voltage;
  float ina_power;
} power_metrics_t;

typedef struct {
  bool charging;
  bool power_is_increasing;
  uint8_t perc_duty_cycle;
  power_metrics_t power_metrics;
} charging_status_t;

typedef struct {
  application_mode_t mode;
  battery_status_t battery_status;
  charging_status_t charging_status;
} system_state_t;

#endif	/* STATE_H */

