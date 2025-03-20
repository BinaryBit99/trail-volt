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

    charging_state.power_metrics.mppt_voltage_v = read_from_adc(MPPT_VOLTAGE_PIN, MPPT_DIVIDER_RATIO);
    charging_state.is_faulty = !charging_current_within_limits();
}

void charging_set_duty_cycles(duty_cycles_t duty_cycles) {
    analogWrite(CHARGE_PWM_PIN, duty_cycles.duty_mppt);
    analogWrite(LOAD_PWM, duty_cycles.duty_load); // updates LOAD_PWM pin
    charging_state.duty_cycles = duty_cycles;
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
    duty_cycles_t duty_cycles = {
        .duty_mppt = 0,
        .duty_load = 255,// Load is inverted
    };
    charging_set_duty_cycles(duty_cycles);

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
    int charge_enable_switch = digitalRead(BTN_2_PIN);
    if (charge_enable_switch == HIGH) {
        return true;
    }
    return false;
}