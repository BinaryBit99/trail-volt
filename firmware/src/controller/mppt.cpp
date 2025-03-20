#include <Arduino.h>
#include "mppt.h"
#include "adc.h"

/**
 * Save the previous charging state for context when determining rate of change.
 */
static charging_state_t prev_charging_state;

duty_cycles_t mppt_calculate_duty_cycles(charging_state_t charging_state) {

    static bool firstRun = true;

    static duty_cycles_t duty_cycles = {.duty_mppt = INITIAL_DUTY, .duty_load = 0};

    if (first_run) {
        prev_charging_state = charging_state;
        first_run = false;
        return duty_cycles;
    }

    // Initial conditions below:
    float v_prev, i_prev, v_new, i_new; // These guys will be our variables used for inc. cond. alg.
    float delta_v, delta_i;     
    int MPPT_voltage_1024;
    uint8_t pwmDutyCycle;      // Set duty cycle to 0% initially - this is what we are manipulating for the algorithm.
    uint8_t loadDutyCycle;


    // below are all the 'initial' SETUP conditions
    // Initial sensor readings
    v_prev = prev_charging_state.power_metrics.ina_bus_voltage_v;
    i_prev = prev_charging_state.power_metrics.ina_current_ma;
    
    // Voltage and Current for MPPT (Update Values)
    MPPT_voltage_1024 = charging_state.power_metrics.mppt_voltage_v;
    // Our cell voltage total cell voltage will be read from INA
    int currCellVoltage = charging_state.power_metrics.ina_bus_voltage_v;

    float error = 0.50;

    float upperRange = 9;
    float lowerRange = 8;

    float newvar = currCellVoltage - error;
    
    

    v_new = charging_state.power_metrics.ina_bus_voltage_v;
    i_new = charging_state.power_metrics.ina_current_ma;
    
    // Calculate the changes (increments)
    delta_v = v_new - v_prev;
    delta_i = i_new - i_prev;

    float verror = fabs(delta_v);
    
    if(verror > 0) {
        loadDutyCycle -= STEP_SIZE;
    } else {
        loadDutyCycle += STEP_SIZE;
    }
    
    // if the voltage change is negligible
    if (fabs(delta_v) < 1e-6) {
        if(delta_i > 0)
            pwmDutyCycle += STEP_SIZE;
            
        else if(delta_i < 0)
            pwmDutyCycle -= STEP_SIZE;
           
    } else {
        // calculate the incremental conductance and compare with the negative instantaneous conductance
        float incCond = delta_i / delta_v;
        float instCond = -i_new / v_new;
        if (fabs(incCond - instCond) < TOLERANCE) {
            // at MPP: do nothing, or maintain the current duty cycle
        } else if (incCond > instCond) {
            // operating point is to the left of MPP, increase voltage (by increasing duty cycle)
            pwmDutyCycle += STEP_SIZE;
    
        } else if (incCond < instCond) {
            // operating point is to the right of MPP, decrease voltage (by decreasing duty cycle)
            pwmDutyCycle -= STEP_SIZE;
            
        }
    }
    // constrain dutyCycle within [MIN_DUTY, MAX_DUTY]
    if (pwmDutyCycle > MAX_DUTY) pwmDutyCycle = MAX_DUTY;
    if (pwmDutyCycle < MIN_DUTY) pwmDutyCycle = MIN_DUTY;  

    // Update previous charging state for next call

    uint8_t pwmDutyCycle = duty_cycles.duty_mppt;
    prev_charging_state = charging_state;

    duty_cycles_t duty_cycles = {
        .duty_mppt = pwmDutyCycle,
        .duty_load = loadDutyCycle,
    };

    return duty_cycles;
}

#include <Arduino.h>
#include "mppt.h"
#include "adc.h"

// Constants (adjust these based on your hardware)
#define STEP_SIZE       1    // Small duty cycle increments (1-5%)
#define TOLERANCE      0.01  // Threshold to consider conductance matched
#define MIN_DUTY       0     // 0% duty cycle minimum
#define MAX_DUTY       255   // 100% duty cycle for 8-bit PWM (adjust if using different resolution)
#define INITIAL_DUTY   127   // Start at ~50% duty cycle

static charging_state_t prev_charging_state;
static bool first_run = true; // Track first execution

duty_cycles_t mppt_calculate_duty_cycles(charging_state_t charging_state) {
    // Initialize on first run
    static duty_cycles_t duty_cycles = {
        .duty_mppt = INITIAL_DUTY,
        .duty_load = 0
    };

    if(first_run) {
        prev_charging_state = charging_state;
        first_run = false;
        return duty_cycles; // Return initial value without processing
    }

    // Convert current from mA to Amps for calculations
    float v_prev = prev_charging_state.power_metrics.ina_bus_voltage_v;
    float i_prev = prev_charging_state.power_metrics.ina_current_ma / 1000.0;
    
    float v_new = charging_state.power_metrics.ina_bus_voltage_v;
    float i_new = charging_state.power_metrics.ina_current_ma / 1000.0;
    
    float delta_v = v_new - v_prev;
    float delta_i = i_new - i_prev;

    // Get previous duty cycle value for continuity
    uint8_t pwmDutyCycle = duty_cycles.duty_mppt;
    uint8_t loadDutyCycle = duty_cycles.duty_load;

    // Handle zero current scenario (common at startup)
    if(i_new == 0) {
        pwmDutyCycle += STEP_SIZE; // Perturb to find operating point
    }
    else if(fabs(delta_v) < 1e-6) { // Voltage change negligible
        if(delta_i > 0) pwmDutyCycle += STEP_SIZE;
        else if(delta_i < 0) pwmDutyCycle -= STEP_SIZE;
    } else {
        float incCond = delta_i / delta_v;
        float instCond = -i_new / v_new;
        
        if(fabs(incCond - instCond) < TOLERANCE) {
            // Maintain current duty cycle at MPP
        } else if(incCond > instCond) {
            pwmDutyCycle += STEP_SIZE; // Move left on P-V curve
        } else {
            pwmDutyCycle -= STEP_SIZE; // Move right on P-V curve
        }
    }

    // Apply boundaries
    pwmDutyCycle = constrain(pwmDutyCycle, MIN_DUTY, MAX_DUTY);

    // Update persistent state
    prev_charging_state = charging_state;

    // Return new values
    return duty_cycles_t {
        .duty_mppt = pwmDutyCycle,
        .duty_load = loadDutyCycle
    };
}
