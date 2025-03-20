#include <Arduino.h>
#include "mppt.h"
#include "adc.h"

/**
 * Save the previous charging state for context when determining rate of change.
 */
static charging_state_t prev_charging_state;

duty_cycles_t mppt_calculate_duty_cycles(charging_state_t charging_state) {

    // Initial conditions below:
    float v_prev, i_prev, v_new, i_new; // These guys will be our variables used for inc. cond. alg.
    float delta_v, delta_i;     
    int MPPT_voltage_1024;
    uint8_t pwmDutyCycle = 0;      // Set duty cycle to 0% initially - this is what we are manipulating for the algorithm.
    uint8_t loadDutyCycle = 0;


    // below are all the 'initial' SETUP conditions
    // Initial sensor readings
    v_prev = prev_charging_state.power_metrics.ina_bus_voltage_v;
    i_prev = prev_charging_state.power_metrics.ina_current_ma;
    
    // Voltage and Current for MPPT (Update Values)
    MPPT_voltage_1024 = charging_state.power_metrics.mppt_voltage_v;
    // Our cell voltage total cell voltage will be read from INA
    int currCellVoltage = charging_state.power_metrics.ina_bus_voltage_v;
    
    while(currCellVoltage != 8.20) {
        if(currCellVoltage < 8.20){
            loadDutyCycle += STEP_SIZE;
        } else{
            loadDutyCycle -= STEP_SIZE;
        }
    }

    v_new = charging_state.power_metrics.ina_bus_voltage_v;
    i_new = charging_state.power_metrics.ina_current_ma;
    
    // Calculate the changes (increments)
    delta_v = v_new - v_prev;
    delta_i = i_new - i_prev;
    
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
    prev_charging_state = charging_state;

    duty_cycles_t duty_cycles = {
        .duty_mppt = pwmDutyCycle,
        .duty_load = loadDutyCycle,
    };

    return duty_cycles;
}
