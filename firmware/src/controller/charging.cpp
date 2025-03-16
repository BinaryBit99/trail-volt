#include <Arduino.h>
#include "charging.h"
#include "gpio.h"
#include "adc.h"
#include <math.h>
#include <Adafruit_INA260.h>
#include "debug.h"

#define K 1  // Constant for scaling.

static charging_state_t charging_state;
static Adafruit_INA260 ina260 = Adafruit_INA260();

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

uint8_t charging_calculate_duty_cycle() {
    uint8_t current_duty_cycle = charging_state.duty_cycle_uint8;
    float current_charging_voltage_v = charging_state.power_metrics.ina_bus_voltage_v;
    
    float critical = 1;        // try critical with 1 for now.

    float voltage_overshoot = current_charging_voltage_v - CHARGING_VOLTAGE_V;
    float error = fabs(voltage_overshoot);

    // If we are within the tolerance, keep the same duty cycle
    if (fabsf(voltage_overshoot) < CHARING_VOLTAGE_TOLERANCE) {
        return current_duty_cycle;
    }

    float alpha = 1.0 + (K * log(error / critical));        // by default log function uses base e
    if(alpha < 0) {        // clamp alpha to zero, if alpha <0
        alpha = 0;
    }
    float duty_cycle_step_uint8 = pow(error,alpha) + 1.0;

    // Ensure duty cycle step converges to minimum value at certain threshold
    if (error < SLOW_STEP_THESHOLD_V) {
        duty_cycle_step_uint8 = 1;
    }    

    if (voltage_overshoot > 0) {
        // Decrease duty cycle -> decrease voltage
        return constrain(current_duty_cycle - duty_cycle_step_uint8, 0, 255);
    } else {
        // Increase duty cycle -> increase voltage
        return constrain(current_duty_cycle + duty_cycle_step_uint8, 0, 255);
    }
}

void charging_set_duty_cycle(uint8_t duty_cycle) {
    uint8_t sanitized_duty_cycle = constrain(duty_cycle, 0, 255);
    analogWrite(CHARGE_PWM_PIN, sanitized_duty_cycle);
    charging_state.duty_cycle_uint8 = sanitized_duty_cycle;
}

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
