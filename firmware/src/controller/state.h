
#ifndef STATE_H
#define	STATE_H

#include <stdint.h>

typedef enum {
  STATE_SLEEP,
  STATE_RECEIVING,
  STATE_SUPPLYING,
  STATE_MONITORING,
  STATE_BALANCING
} application_mode_t;

typedef struct {
    float upper_cell_voltage_v;
    float lower_cell_voltage_v;
    float cell_1_temperature_c;
    float cell_2_temperature_c;
    bool upper_discharging;
    bool lower_discharging;
} battery_status_t;

/* Child of charging status */
typedef struct {
  float ina_current;
  float ina_bus_voltage;
  float ina_power;
  float charge_voltage_v;
} power_metrics_t;

typedef struct {
  bool charging;
  bool power_is_increasing;
  uint8_t duty_cycle_uint8;
  power_metrics_t power_metrics;
} charging_status_t;

typedef struct {
  application_mode_t mode;
  battery_status_t battery_status;
  charging_status_t charging_status;
} system_state_t;

// Command structures to represent actions to take
typedef struct {
    bool should_balance;
    bool should_discharge_upper;
    bool should_discharge_lower;
} battery_command_t;

typedef struct {
    uint8_t target_duty_cycle;
    bool should_stop_charging;
} charging_command_t;

typedef struct {
    application_mode_t next_mode;
    battery_command_t battery_cmd;
    charging_command_t charging_cmd;
} system_command_t;

#endif	/* STATE_H */

