
#include <Arduino.h>
#include "charging.h"
#include "gpio.h"
#include "adc.h"
#include <math.h>

uint8_t calculate_duty_cycle(uint8_t current_duty_cycle, float current_charging_voltage_v);

void update_power_metrics(Adafruit_INA260 *ina260, power_metrics_t *power_metrics) {
    power_metrics->ina_bus_voltage = ina260->readBusVoltage();
    power_metrics->ina_current = ina260->readCurrent();
    power_metrics->ina_power = ina260->readPower();
    
    power_metrics->charge_voltage_v = read_from_adc(CHARGE_VOLTAGE_PIN, CHARGE_VOLTAGE_DIVIDER_RATIO);
}

void update_charging_status(charging_status_t *charging_status, const Adafruit_INA260 *ina260) {

    // Make a copy before updating for comparison
    charging_status_t old_charging_status = *charging_status;

    // Get new values from INA260
    update_power_metrics(ina260, &charging_status->power_metrics);

    uint8_t new_duty_cycle = calculate_duty_cycle(charging_status->duty_cycle_uint8, charging_status->power_metrics.charge_voltage_v);
    charging_status->duty_cycle_uint8 = new_duty_cycle;
}

/**
 * @brief  Calculates the new duty cycle applied to gate driver input.
 * @param  old_duty_cycle: The duty cycle currently applied to the gate driver.
 * @return The new calculated PWM duty cycle.
 */
uint8_t calculate_duty_cycle(uint8_t current_duty_cycle, float current_charging_voltage_v) {

    float voltage_overshoot = current_charging_voltage_v - CHARGING_VOLTAGE_V;

    // If we are within the tolerance, keep the same duty cycle
    if (fabs(voltage_overshoot) < CHARING_VOLTAGE_TOLERANCE) {
        return current_duty_cycle;
    }

    if (voltage_overshoot > 0) {
        // Increase duty cycle -> increase voltage
        return constrain(current_duty_cycle - DUTY_CYCLE_STEP_UINT8, 0, 255);
    } else {
        // Decrease duty cycle -> decrease voltage
        return constrain(current_duty_cycle + DUTY_CYCLE_STEP_UINT8, 0, 255);
    }
}

void adjust_duty_cycle(const charging_status_t *charging_status) {

    uint8_t duty_cycle = constrain(charging_status->duty_cycle_uint8, 0, 255);

    analogWrite(PWM_PIN, duty_cycle);
}

int is_receiving_charge() {
    return 0;
}

