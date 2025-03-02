
#include <Arduino.h>
#include "charging.h"
#include "gpio.h"
#include "adc.h"

void update_power_metrics(Adafruit_INA260 *ina260, power_metrics_t *power_metrics) {
    power_metrics->ina_bus_voltage = ina260->readBusVoltage();
    power_metrics->ina_current = ina260->readCurrent();
    power_metrics->ina_power = ina260->readPower();
    
    power_metrics->charge_voltage_mv = read_from_adc(CHARGE_VOLTAGE_PIN, CHARGE_VOLTAGE_DIVIDER_RATIO);
}

uint8_t update_charging_status(charging_status_t *charging_status, const Adafruit_INA260 *ina260) {

    // Make a copy before updating for comparison
    charging_status_t old_charging_status = *charging_status;

    // Get new values from INA260
    update_power_metrics(ina260, &charging_status->power_metrics);

    uint8_t new_perc_duty_cycle = charging_status;

    float old_power = old_charging_status.power_metrics.ina_power;
    float new_power = charging_status->power_metrics.ina_power;

    // Perturb and Observe Algorithm
    if (new_power > old_power) {
       // Keep moving in same direction
       new_perc_duty_cycle += charging_status->power_is_increasing ? MPPT_STEP_SIZE : -MPPT_STEP_SIZE;
    } else {
       // Reverse direction
       charging_status->power_is_increasing = !(charging_status->power_is_increasing);
       new_perc_duty_cycle += charging_status->power_is_increasing ? MPPT_STEP_SIZE : -MPPT_STEP_SIZE;
     }

     // Ensure duty stays within bounds
    return constrain(new_perc_duty_cycle, 0, 100);
}



void adjust_duty_cycle(const charging_status_t *charging_status) {

    uint8_t duty_cycle = (uint8_t)(charging_status->perc_duty_cycle * 255 / 100);

    duty_cycle = constrain(duty_cycle, 0, 255);

    analogWrite(PWM_PIN, duty_cycle);
}

int is_receiving_charge() {
    return 0;
}

