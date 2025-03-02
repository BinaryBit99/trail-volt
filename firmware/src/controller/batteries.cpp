
#include "batteries.h"
#include "gpio.h"
#include "adc.h"
#include <math.h>

void balance_cells(battery_status_t *battery_status) {

    float upper_cell_v = battery_status->upper_cell_voltage_v;
    float lower_cell_v = battery_status->lower_cell_voltage_v;

    if (upper_cell_v - lower_cell_v > BALANCE_THRESHOLD_V) {
        // Discharge upper cell
        digitalWrite(UPPER_CELL_DISCHARGE_PIN, HIGH);
        digitalWrite(LOWER_CELL_DISCHARGE_PIN, LOW);  // Ensure the lower cell is not discharged
        battery_status->upper_discharging = true;
        battery_status->lower_discharging = false;
    } else if (upper_cell_v - lower_cell_v < -BALANCE_THRESHOLD_V) {
        // Discharge lower cell
        digitalWrite(UPPER_CELL_DISCHARGE_PIN, LOW);
        digitalWrite(LOWER_CELL_DISCHARGE_PIN, HIGH);
        battery_status->upper_discharging = false;
        battery_status->lower_discharging = true;
    } else {
        digitalWrite(UPPER_CELL_DISCHARGE_PIN, LOW);
        digitalWrite(LOWER_CELL_DISCHARGE_PIN, LOW);
        battery_status->upper_discharging = false;
        battery_status->lower_discharging = false;
    }
}

bool update_battery_status(battery_status_t *battery_status) {
    // Read cell voltages
    float total_voltage = read_from_adc(TOTAL_CELL_ADC_PIN, TOTAL_CELL_ADC_DIVISION);

    battery_status->lower_cell_voltage_v = read_from_adc(LOWER_CELL_ADC_PIN, LOWER_CELL_ADC_DIVISION);
    battery_status->upper_cell_voltage_v = total_voltage - battery_status->lower_cell_voltage_v;

    // Read temperatures (no voltage divider)
    battery_status->cell_1_temperature_c = read_from_adc(CELL1_TEMP_PIN, 1.0);
    battery_status->cell_2_temperature_c = read_from_adc(CELL2_TEMP_PIN, 1.0);

    float upper_cell_v = battery_status->upper_cell_voltage_v;
    float lower_cell_v = battery_status->lower_cell_voltage_v;

    float difference = fabs(upper_cell_v - lower_cell_v);

    if (difference > BALANCE_THRESHOLD_V) {
        return true;
    } else {
        return false;
    }
}
