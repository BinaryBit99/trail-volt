
#include "state_manager.h"
#include "batteries.h"
#include "charging.h"
#include "gpio.h"

// Private state - hidden from other files
static system_state_t current_state = {0};

void state_manager_init() {
    current_state.mode = STATE_MONITORING;
}

const system_state_t* state_manager_get_state() {
    return &current_state;
}

void state_manager_update_sensor_readings(const Adafruit_INA260 *ina260) {
    // Update battery status
    update_battery_readings(&current_state.battery_status);
    
    // Update power metrics
    update_power_metrics(ina260, &current_state.charging_status.power_metrics);
}

system_command_t state_manager_process_state() {
    system_command_t cmd = {0};
    
    // Set the next mode based on current state
    cmd.next_mode = current_state.mode;
    
    // Determine if balancing is needed
    cmd.battery_cmd.should_balance = is_balancing_needed(&current_state.battery_status);
    
    // Logic to determine cell discharge commands
    if (cmd.battery_cmd.should_balance) {
        if (current_state.battery_status.upper_cell_voltage_v > 
            current_state.battery_status.lower_cell_voltage_v) {
            cmd.battery_cmd.should_discharge_upper = true;
            cmd.battery_cmd.should_discharge_lower = false;
        } else {
            cmd.battery_cmd.should_discharge_upper = false;
            cmd.battery_cmd.should_discharge_lower = true;
        }
    }
    
    // Determine charging commands
    if (current_state.mode == STATE_RECEIVING) {
        cmd.charging_cmd.target_duty_cycle = calculate_duty_cycle(
            &current_state.charging_status);
    } else if (current_state.mode == STATE_BALANCING) {
        cmd.charging_cmd.should_stop_charging = true;
    }
    
    // State transition logic
    switch (current_state.mode) {
        case STATE_MONITORING:
            if (cmd.battery_cmd.should_balance) {
                cmd.next_mode = STATE_BALANCING;
            } else if (is_receiving_charge()) {
                cmd.next_mode = STATE_RECEIVING;
            }
            break;
            
        case STATE_RECEIVING:
            if (cmd.battery_cmd.should_balance) {
                cmd.next_mode = STATE_BALANCING;
            } else if (!is_receiving_charge()) {
                cmd.next_mode = STATE_MONITORING;
            }
            break;
            
        case STATE_BALANCING:
            if (!cmd.battery_cmd.should_balance) {
                cmd.next_mode = STATE_MONITORING;
            }
            break;
        default:
            cmd.next_mode = STATE_MONITORING;
            break;
    }
    
    return cmd;
}

void state_manager_apply_command(const system_command_t* cmd) {

    /**
     * Here we execute the commands that our application has determined necessary.
     *
     * We map the the should_[command] to their physical meanings.
     * 
     * Ex. The should_balance command maps to the toggling of our cell balancing GPIO pins)
     */

    // Apply mode change
    current_state.mode = cmd->next_mode;

    // Apply battery commands
    if (cmd->battery_cmd.should_balance) {
        // Update the flags in our system state
        current_state.battery_status.upper_discharging = 
            cmd->battery_cmd.should_discharge_upper;
        current_state.battery_status.lower_discharging = 
            cmd->battery_cmd.should_discharge_lower;
        
        // Set GPIO pins according to this new state
        set_upper_discharge(cmd->battery_cmd.should_discharge_upper);
        set_lower_discharge(cmd->battery_cmd.should_discharge_lower);
    } else {
        // No balancing needed, ensure both GPIOs set low
        current_state.battery_status.upper_discharging = false;
        current_state.battery_status.lower_discharging = false;
        set_upper_discharge(false);
        set_lower_discharge(false);
    }
    
    // Apply charging commands
    if (cmd->charging_cmd.should_stop_charging) {
        stop_charging();
        current_state.charging_status.duty_cycle_uint8 = 0;
    } else {
        current_state.charging_status.duty_cycle_uint8 = 
            cmd->charging_cmd.target_duty_cycle;
        set_charging_duty_cycle(cmd->charging_cmd.target_duty_cycle);
    }
}
