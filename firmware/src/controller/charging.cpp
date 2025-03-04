
#include <Arduino.h>
#include "charging.h"
#include "gpio.h"
#include "adc.h"
#include <math.h>
#include <Adafruit_INA260.h>

static charging_state_t charging_state;
static Adafruit_INA260 ina260 = Adafruit_INA260();

void charging_init() {
    if (!ina260.begin()) {
       Serial.println("Couldn't find INA260");
       //while (1);
    }
}

charging_state_t charging_get_state() {
    return charging_state;
}

void charging_update_state() {
    charging_state.power_metrics.ina_bus_voltage = ina260.readBusVoltage();
    charging_state.power_metrics.ina_current = ina260.readCurrent();
    charging_state.power_metrics.ina_power = ina260.readPower();
    
    charging_state.power_metrics.charge_voltage_v = read_from_adc(CHARGE_VOLTAGE_PIN, CHARGE_VOLTAGE_DIVIDER_RATIO);
    charging_state.is_faulty = charging_current_within_limits();
}

uint8_t charging_calculate_duty_cycle() {
    uint8_t current_duty_cycle = charging_state.duty_cycle_uint8;
    float current_charging_voltage_v = charging_state.power_metrics.charge_voltage_v;
    
    float voltage_overshoot = current_charging_voltage_v - CHARGING_VOLTAGE_V;

    // If we are within the tolerance, keep the same duty cycle
    if (fabsf(voltage_overshoot) < CHARING_VOLTAGE_TOLERANCE) {
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

void charging_set_duty_cycle(uint8_t duty_cycle) {
    uint8_t sanitized_duty_cycle = constrain(duty_cycle, 0, 255);
    analogWrite(PWM_PIN, sanitized_duty_cycle);
    charging_state.duty_cycle_uint8 = sanitized_duty_cycle;
}

bool is_receiving_charge() {
    return true;
}

void charging_stop() {

    // Pull shut-down pill low on gate driver
    charging_set_duty_cycle(1);

    charging_state.is_charging = false;
    return;
}

bool charging_current_within_limits() {
    bool in_limits = (charging_state.power_metrics.ina_current < MAX_CHARGE_CURRENT_A) && (charging_state.power_metrics.ina_current > MIN_CHARGE_CURRENT_A );

    if (in_limits) {
        charging_state.is_faulty = false;
        return false;
    } else {
        charging_state.is_faulty = true;
        return true;
    }
}
