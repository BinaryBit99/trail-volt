
#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include <Adafruit_INA260.h>

/**
 * @brief  Initialize the state manager.
 */
void state_manager_init();

/**
 * @brief  Reads all sensor data and updates the system state.
 */
void state_manager_update_sensors();

/**
 * @brief  Updates next FSM state based on sensor readings.
 */
void state_manager_update_mode();

/**
 * @brief  Applies updates to the hardware based on the current system state.
 */
void state_manager_apply_hardware_updates();

#endif /* STATE_MANAGER_H */
