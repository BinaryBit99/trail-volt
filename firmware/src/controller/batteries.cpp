
#include "batteries.h"
#include "gpio.h"

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
