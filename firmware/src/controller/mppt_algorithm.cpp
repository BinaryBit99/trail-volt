#include <Arduino.h>
#include "charging.h"
#include "gpio.h"
#include "adc.h"
#include <math.h>
#include <Adafruit_INA260.h>
#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

//tuning parameters
#define TOLERANCE 0.01f       // tolerance for MPP condition
#define STEP_SIZE 0.001f      // linear step sizes for now that are small for testing purposes
#define MAX_DUTY 1.0f         // Maximum duty cycle
#define MIN_DUTY 0.0f         // Minimum duty cycle

static charging_state_t charging_state;
static duty_cycles duty;
static Adafruit_INA260 ina260 = Adafruit_INA260();

// Initial conditions below:

float v_prev, i_prev, v_new, i_new; // These guys will be our variables used for inc. cond. alg.
float delta_v, delta_i;     
int MPPT_voltage_1024;
float dutyCycle = 0f;      // Set duty cycle to 0% initially - this is what we are manipulating for the algorithm.
charging_set_duty_cycle(dutyCycle);   
set_load_duty(0); // Set load duty cycle to 0 initially
float loadDutyCycle = 0f;
float divider_ratio = (180/310);
float MPPT_voltage_raw;
float total_cell_voltage_raw;
float MPPT_voltage_rawscaled;
float total_cell_voltage_rawscaled;



void charging_init() {
    if (!ina260.begin()) {
        D_printlnf("Couldn't find INA260");
        while (1);
    } else {
        D_printlnf("Found INA260");
    }
}

charging_state_t charging_get_state() {
    return charging_state;
}

void charging_update_state() {
    charging_state.power_metrics.ina_bus_voltage_v = ina260.readBusVoltage() / (float)1000;
    charging_state.power_metrics.ina_current_ma = ina260.readCurrent();
    charging_state.power_metrics.ina_power_w = ina260.readPower() / (float)1000;
    charging_state.power_metrics.charge_voltage_v = read_from_adc(CHARGE_VOLTAGE_PIN, CHARGE_VOLTAGE_DIVIDER_RATIO);
    charging_state.is_faulty = !charging_current_within_limits();
}

float roundToNearestTenth(float value) {
    return roundf(value * 10.0f) / 10.0f;
}

void charging_calculate_duty_cycle() {
    
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
    total_cell_voltage_rawscaled = roundToNearestTenth(total_cell_voltage_rawscaled);   // to help make things converge while ensuring accuracy

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
    duty.mppt_duty = dutyCycle;        // Update PWM duty cycle for the converter
    duty.load_duty = loadDutyCycle;    // update PWM DUTY cycle to load
    //update previous measurements
    v_prev = v_new;
    i_prev = i_new;
    
    // insert delay for next sampling period 
    // delay_ms(SAMPLING_PERIOD_MS);
    
}

void charging_set_all_duty_cycle(struct duty) {        // One function all together for both PWM signals.
    uint8_t sanitized_duty_cycle = constrain(duty.duty_mppt, 0, 255);
    analogWrite(CHARGE_PWM_PIN, sanitized_duty_cycle);
    charging_state.duty_cycle_uint8 = sanitized_duty_cycle;
    uint8_t dl_cycle = constrain(duty.duty_load, 0, 255);
    analogWrite(LOAD_PWM, dl_cycle); // updates LOAD_PWM pin
}

// void set_load_duty(uint8_t duty_load_cycle) {
//     uint8_t dl_cycle = constrain(duty_load_cycle, 0, 255);
//     analogWrite(LOAD_PWM, dl_cycle); // updates LOAD_PWM pin
// }

bool is_receiving_charge() {
    return true;
}

void charging_set_shutdown_pin(bool shutdown) {
    if (shutdown) {
        pinMode(H_BRIDGE_SHUT_DOWN_PIN_AL, OUTPUT);
        digitalWrite(H_BRIDGE_SHUT_DOWN_PIN_AL, LOW);
    } else {
        pinMode(H_BRIDGE_SHUT_DOWN_PIN_AL, INPUT_PULLUP);
    }
}

void charging_stop() {
    // Pull shut-down pin low on gate driver
    charging_set_shutdown_pin(true);

    // As an extra step, set duty cycle to 0
    charging_set_duty_cycle(0);

    charging_state.is_charging = false;
    return;
}

bool charging_current_within_limits() {
    bool in_limits = (charging_state.power_metrics.ina_current_ma < MAX_CHARGE_CURRENT_A) && (charging_state.power_metrics.ina_current_ma > MIN_CHARGE_CURRENT_A );

    /*
     * Since our hardware is not yet setup for determining current values, return true.
     */

    return true;
}

bool charging_is_enabled() {
    int charge_enable_switch = digitalRead(SWITCH_1_PIN);
    if (charge_enable_switch == HIGH) {
        return true;
    }
    return false;
}
