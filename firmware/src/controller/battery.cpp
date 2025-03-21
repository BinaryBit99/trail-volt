#include "battery.h"
#include "gpio.h"
#include "adc.h"
#include <math.h>
#include "temperature.h"

static battery_state_t battery_state;

battery_state_t battery_get_state() {
    return battery_state;
}

void battery_set_upper_discharge(bool discharge) {
    if (discharge) {
        digitalWrite(C2_BALANCING_PIN, HIGH);
        battery_state.upper_discharging = true;
    } else {
        digitalWrite(C2_BALANCING_PIN, LOW);
        battery_state.upper_discharging = false;
    }
}

void battery_set_lower_discharge(bool discharge) {
    if (discharge) {
        digitalWrite(C1_BALANCING_PIN, HIGH);
        battery_state.lower_discharging = true;
    } else {
        digitalWrite(C1_BALANCING_PIN, LOW);
        battery_state.lower_discharging = false;
    }
}

void battery_update_state() {
    // Read cell voltages
    float total_voltage = read_from_adc(TOTAL_CELL_ADC_PIN, TOTAL_CELL_ADC_DIVISION);

    battery_state.lower_cell_voltage_v = read_from_adc(LOWER_CELL_ADC_PIN, LOWER_CELL_ADC_DIVISION);
    battery_state.upper_cell_voltage_v = total_voltage - battery_state.lower_cell_voltage_v;

    // Read temperatures (no voltage divider)
    battery_state.series_temperature_c = temperature_get_series_reading();
    battery_state.ground_temperature_c = temperature_get_ground_reading();
}

float battery_get_voltage_difference() {
    return fabsf(battery_state.upper_cell_voltage_v - battery_state.lower_cell_voltage_v);
}

bool battery_balancing_needed() {
    float voltage_diff = battery_get_voltage_difference();
    return (voltage_diff > BALANCE_THRESHOLD_V);
}

