#include "state_manager.h"
#include "battery.h"
#include "charging.h"
#include "gpio.h"
#include "mode.h"
#include "debug.h"


void state_manager_init() {
    set_mode(MODE_MONITORING);
}

/**
 * @brief  Reads all sensor data and updates the system state.
 */
void state_manager_update_sensors() {
    battery_update_state();
    charging_update_state();
}

void state_manager_update_mode() {
    /* Modify application mode based on new state */
    battery_state_t battery_state = battery_get_state();
    charging_state_t charging_state = charging_get_state();

    bool needs_balancing = battery_balancing_needed();
    
    // Initialize next mode to current mode
    mode_e current_mode = get_mode();
    mode_e next_mode = current_mode;

    switch (current_mode) {
        case MODE_RECEIVING:
            if (needs_balancing) {
                next_mode = MODE_BALANCING;
                break;
            }
            if (charging_state.is_faulty) {
                next_mode = MODE_CHARGING_FAULT;
                break;
            }
            if (!is_receiving_charge()) {
                next_mode = MODE_MONITORING;
                break;
            }
            break;
        case MODE_CHARGING_FAULT:
            if (!charging_state.is_faulty) {
                next_mode = MODE_MONITORING;
            }
            break;
        case MODE_SUPPLYING:
            if (needs_balancing) {
                next_mode = MODE_BALANCING;
                break;
            }
            break;
        case MODE_MONITORING:
            if (needs_balancing) {
                next_mode = MODE_BALANCING;
                break;
            }
            if (is_receiving_charge()) {
                next_mode = MODE_RECEIVING;
                break;
            }
            break;
        case MODE_BALANCING:
            if (!needs_balancing) {
                next_mode = MODE_MONITORING;
                break;
            }
            break;
        default:
            next_mode = MODE_MONITORING;
            break;
    }

    if (next_mode != current_mode) {
        // For the purpose of debugging
        D_printf("Mode switched to: "); //D_println(next_mode);
    }

    set_mode(next_mode);   
}

/**
 * @brief  Applies updates to the hardware based on the current system state.
 */
void state_manager_apply_hardware_updates() {
    /*
     * This function is not responsible for checking whether the current state
     * is accurate, but only for reflecting this current state through hardware updates.
     */
    
    battery_state_t battery_state = battery_get_state();
    charging_state_t charging_state = charging_get_state();
    mode_e current_mode = get_mode();

    switch (current_mode) {
        case MODE_BALANCING: 
            // Don't charge while balancing
            charging_stop();
            // Determine which cell to balance
            if (battery_state.upper_cell_voltage_v - battery_state.lower_cell_voltage_v > 0) {
                battery_set_upper_discharge(true);
                battery_set_lower_discharge(false);
            } else {
                battery_set_upper_discharge(false);
                battery_set_lower_discharge(true);
            }
            break;
        case MODE_CHARGING_FAULT:
            // Stop charging
            charging_stop();
            // Stop discharging (shouldn't be anyways)
            battery_set_upper_discharge(false);
            battery_set_lower_discharge(false);
            break;
        case MODE_RECEIVING:
            battery_set_upper_discharge(false);
            battery_set_lower_discharge(false);

            uint8_t new_duty_cycle = charging_calculate_duty_cycle();
            charging_set_duty_cycle(new_duty_cycle);

            break;
        case MODE_SUPPLYING:
            charging_stop();
            battery_set_upper_discharge(false);
            battery_set_lower_discharge(false);
            break;
        case MODE_MONITORING:
            charging_stop();
            battery_set_upper_discharge(false);
            battery_set_lower_discharge(false);
            break;
        default:
            current_mode = MODE_MONITORING;
            break;
    }
    return;
}
