
/* 
 * File:   power.c
 * Author: Kyle James
 *
 * Created on February 17, 2025, 12:19 PM
 */

#include "power.h"
#include "sensors.h"
#include <Arduino.h>


uint16_t update_charging_status(charging_status_t *charging_status, const Adafruit_INA260 *ina260) {

    // Make a copy before updating for comparison
    charging_status_t old_charging_status = *charging_status;

    // Get new values from INA260
    update_power_metrics(ina260, &charging_status->power_metrics);

    uint8_t new_duty_cycle;

    float old_power = old_charging_status.power_metrics.ina_power;
    float new_power = charging_status->power_metrics.ina_power;

    // Perturb and Observe Algorithm
    if (new_power > old_power) {
       // Keep moving in same direction
       new_duty_cycle += charging_status->power_is_increasing ? MPPT_STEP_SIZE : -MPPT_STEP_SIZE;
    } else {
       // Reverse direction
       charging_status->power_is_increasing = !(charging_status->power_is_increasing);
       new_duty_cycle += charging_status->power_is_increasing ? MPPT_STEP_SIZE : -MPPT_STEP_SIZE;
     }

     // Ensure duty stays within bounds
    charging_status->current_duty_cycle = constrain(new_duty_cycle, 0, 100);
}



void adjust_duty_cycle(const charging_status_t *charging_status) {

    uint8_t duty_cycle = (uint8_t)(charging_status->current_duty_cycle * 255 / 100);

    Serial.print("Setting duty cycle of: "); Serial.print(charging_status->current_duty_cycle); Serial.print(" to "); Serial.println(duty_cycle); 

    analogWrite(PWM_PIN, duty_cycle);
}

int is_charging() {
    return 0;
}