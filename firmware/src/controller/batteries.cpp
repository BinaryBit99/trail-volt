
#include "batteries.h"
#include "gpio.h"
#include "adc.h"

void handle_cell_balancing(battery_status_t *battery_status) {

    int upper_cell_mv = battery_status->upper_cell_voltage_mv;
    int lower_cell_mv = battery_status->lower_cell_voltage_mv;

    if (upper_cell_mv - lower_cell_mv > BALANCE_THRESHOLD_MV) {
        // Discharge upper cell
        digitalWrite(UPPER_CELL_DISCHARGE_PIN, HIGH);
        digitalWrite(LOWER_CELL_DISCHARGE_PIN, LOW);  // Ensure the lower cell is not discharged
        battery_status->upper_discharging = true;
        battery_status->lower_discharging = false;
    } else if (upper_cell_mv - lower_cell_mv < -BALANCE_THRESHOLD_MV) {
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

void update_battery_status(battery_status_t *battery_status) {
    // Read cell voltages
    int total_voltage = read_from_adc(TOTAL_CELL_ADC_PIN, TOTAL_CELL_ADC_DIVISION);

    battery_status->lower_cell_voltage_mv = read_from_adc(LOWER_CELL_ADC_PIN, LOWER_CELL_ADC_DIVISION);
    battery_status->upper_cell_voltage_mv = total_voltage - battery_status->lower_cell_voltage_mv;

    // Read temperatures (no voltage divider)
    battery_status->cell_1_temperature_c = read_from_adc(CELL1_TEMP_PIN, 1.0);
    battery_status->cell_2_temperature_c = read_from_adc(CELL2_TEMP_PIN, 1.0);
}
