
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

uint8_t calculate_duty_cycle(const charging_status_t *status) {
    uint8_t current_duty_cycle = status->duty_cycle_uint8;
    float current_charging_voltage_v = status->power_metrics.charge_voltage_v;
    
    float voltage_overshoot = current_charging_voltage_v - CHARGING_VOLTAGE_V;

    // If we are within the tolerance, keep the same duty cycle
    if (fabs(voltage_overshoot) < CHARING_VOLTAGE_TOLERANCE) {
        return current_duty_cycle;
    }

    if (voltage_overshoot > 0) {
        // Decrease duty cycle -> decrease voltage
        return constrain(current_duty_cycle - DUTY_CYCLE_STEP_UINT8, 0, 255);
    } else {
        // Increase duty cycle -> increase voltage
        return constrain(current_duty_cycle + DUTY_CYCLE_STEP_UINT8, 0, 255);
    }
}

void set_charging_duty_cycle(uint8_t duty_cycle) {
    uint8_t sanitized_duty_cycle = constrain(duty_cycle, 0, 255);
    analogWrite(PWM_PIN, sanitized_duty_cycle);
}

bool is_receiving_charge() {
    return true;
}


void stop_charging() {
    return;
}
