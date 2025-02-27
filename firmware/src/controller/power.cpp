
/* 
 * File:   power.c
 * Author: Kyle James
 *
 * Created on February 17, 2025, 12:19 PM
 */

#include "power.h"
#include "sensors.h"
#include <Arduino.h>


uint16_t update_charge_state(charge_state_t *charge_state, const Adafruit_INA260 *ina260) {

    // Make a copy before updating for comparison
    charge_state_t old_charge_state = *charge_state;

    // Get new values from INA260
    update_power_state(ina260, &charge_state->power_state);

    uint8_t new_duty_cycle;

    float old_power = old_charge_state.power_state.ina_power;
    float new_power = charge_state->power_state.ina_power;

    // Perturb and Observe Algorithm
    if (new_power > old_power) {
       // Keep moving in same direction
       new_duty_cycle += charge_state->power_is_increasing ? MPPT_STEP_SIZE : -MPPT_STEP_SIZE;
    } else {
       // Reverse direction
       charge_state->power_is_increasing = !(charge_state->power_is_increasing);
       new_duty_cycle += charge_state->power_is_increasing ? MPPT_STEP_SIZE : -MPPT_STEP_SIZE;
     }

     // Ensure duty stays within bounds
    charge_state->current_duty_cycle = constrain(new_duty_cycle, 0, 100);
}



void adjust_duty_cycle(const charge_state_t *charge_state) {

    uint8_t duty_cycle = (uint8_t)(charge_state->current_duty_cycle * 255 / 100);

    Serial.print("Setting duty cycle of: "); Serial.print(charge_state->current_duty_cycle); Serial.print(" to "); Serial.println(duty_cycle); 

    analogWrite(PWM_PIN, duty_cycle);
}

int is_charging() {
    return 0;
}