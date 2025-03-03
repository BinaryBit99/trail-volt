
#include "batteries.h"
#include "gpio.h"
#include "adc.h"
#include <math.h>

void set_upper_discharge(bool discharge) {
    if (discharge) {
        digitalWrite(UPPER_CELL_DISCHARGE_PIN, HIGH);
    } else {
        digitalWrite(UPPER_CELL_DISCHARGE_PIN, LOW);
    }
}

void set_lower_discharge(bool discharge) {
    if (discharge) {
        digitalWrite(LOWER_CELL_DISCHARGE_PIN, HIGH);
    } else {
        digitalWrite(LOWER_CELL_DISCHARGE_PIN, LOW);
    }
}

void update_battery_readings(battery_status_t *battery_status) {
    // Read cell voltages
    float total_voltage = read_from_adc(TOTAL_CELL_ADC_PIN, TOTAL_CELL_ADC_DIVISION);

    battery_status->lower_cell_voltage_v = read_from_adc(LOWER_CELL_ADC_PIN, LOWER_CELL_ADC_DIVISION);
    battery_status->upper_cell_voltage_v = total_voltage - battery_status->lower_cell_voltage_v;

    // Read temperatures (no voltage divider)
    battery_status->cell_1_temperature_c = read_from_adc(CELL1_TEMP_PIN, 1.0);
    battery_status->cell_2_temperature_c = read_from_adc(CELL2_TEMP_PIN, 1.0);

    float upper_cell_v = battery_status->upper_cell_voltage_v;
    float lower_cell_v = battery_status->lower_cell_voltage_v;
}

float get_cell_voltage_difference(const battery_status_t* status) {
    return fabsf(status->upper_cell_voltage_v - status->lower_cell_voltage_v);
}

bool is_balancing_needed(const battery_status_t* status) {
    float voltage_diff = get_cell_voltage_difference(status);
    return (voltage_diff > BALANCE_THRESHOLD_V);
}

