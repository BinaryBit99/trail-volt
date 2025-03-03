
#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include "state.h"
#include <Adafruit_INA260.h>

/**
 * @brief  Initialize the state manager.
 */
void state_manager_init();

/**
 * @brief  Get the read-only pointer to the system state.
 * @return The read-only pointer to the system state.
 */
const system_state_t* state_manager_get_state();

/**
 * @brief  Updates the system sensor readings.
 * @param  ina260: A read-only pointer to the INA260 instance.
 */
void state_manager_update_sensor_readings(const Adafruit_INA260 *ina260);

/**
 * @brief  Processes the current system state.
 * @return The commands to be executed based on the system state.
 */
system_command_t state_manager_process_state();

/**
 * @brief  Applies the command to update system state or control hardware.
 */
void state_manager_apply_command(const system_command_t* cmd);

#endif /* STATE_MANAGER_H */
