#include <Arduino.h>
#include "mppt.h"

float round_to_nearest_tenth(float value) {
    return roundf(value * 10.0f) / 10.0f;
}

duty_cycles_t mppt_calculate_duty_cycles(charging_state_t charging_state) {

    // Initial conditions below:
    float v_prev, i_prev, v_new, i_new; // These guys will be our variables used for inc. cond. alg.
    float delta_v, delta_i;     
    int MPPT_voltage_1024;
    float dutyCycle = 0.0f;      // Set duty cycle to 0% initially - this is what we are manipulating for the algorithm.
    float loadDutyCycle = 0.0f;
    float divider_ratio = (180/310);
    float MPPT_voltage_raw;
    float total_cell_voltage_raw;
    float MPPT_voltage_rawscaled;
    float total_cell_voltage_rawscaled;


    // below are all the 'initial' SETUP conditions
    // Initial sensor readings
    v_prev = charging_state.power_metrics.ina_bus_voltage_v;
    i_prev = charging_state.power_metrics.ina_current_ma;
    
    // Fetch the current Voltage and Current for MPPT (Update Values)

    MPPT_voltage_1024 = analogRead(MPPT_VOLTAGE_PIN); // READ IN FROM A6
    int currCellVoltage = analogRead(TOTAL_CELL_ADC_PIN);

    total_cell_voltage_raw = (currCellVoltage) * (5/1024);  // Re-scale everything.
    MPPT_voltage_raw = (MPPT_voltage_1024) * (5/1024);
    
    MPPT_voltage_rawscaled = MPPT_voltage_raw / divider_ratio; // Actualized analog voltage from schematic
    total_cell_voltage_rawscaled = total_cell_voltage_raw / divider_ratio;
    total_cell_voltage_rawscaled = round_to_nearest_tenth(total_cell_voltage_rawscaled);   // to help make things converge while ensuring accuracy

    while(total_cell_voltage_rawscaled != 8.20) {
        if(total_cell_voltage_rawscaled < 8.20){
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
            dutyCycle += STEP_SIZE;
            
        else if(delta_i < 0)
            dutyCycle -= STEP_SIZE;
           
    } else {
        // calculate the incremental conductance and compare with the negative instantaneous conductance
        float incCond = delta_i / delta_v;
        float instCond = -i_new / v_new;
        if (fabs(incCond - instCond) < TOLERANCE) {
            // at MPP: do nothing, or maintain the current duty cycle
        } else if (incCond > instCond) {
            // operating point is to the left of MPP, increase voltage (by increasing duty cycle)
            dutyCycle += STEP_SIZE;
    
        } else if (incCond < instCond) {
            // operating point is to the right of MPP, decrease voltage (by decreasing duty cycle)
            dutyCycle -= STEP_SIZE;
            
        }
    }
    // constrain dutyCycle within [MIN_DUTY, MAX_DUTY]
    if (dutyCycle > MAX_DUTY) dutyCycle = MAX_DUTY;
    if (dutyCycle < MIN_DUTY) dutyCycle = MIN_DUTY;

    //update previous measurements
    v_prev = v_new;
    i_prev = i_new;    

    duty_cycles_t duty_cycles = {
        .duty_mppt = dutyCycle,
        .duty_load = loadDutyCycle,
    };

    return duty_cycles;
}
