#include "temperature.h"
#include "adc.h"

float get_resistance(int pin) {
    float voltage = read_from_adc(pin, THERMISTOR_ADC_DIVISION);

    float drop;
    if (IS_HIGH_SIDE) {
        drop = 5.0f - voltage;
    } else {
        drop = voltage;
    }

    float resistance = (5.0f * SERIES_RESISTOR) / drop;
}

float temperature_get_upper_cell() {
    float resistance = get_resistance(UPPER_CELL_TEMP_PIN);

    /*
     * Yet to be implemented.
     */

    return 0;
}

float temperature_get_lower_cell() {
    float resistance = get_resistance(LOWER_CELL_TEMP_PIN);

    /*
     * Yet to be implemented.
     */
     
    return 0;
}
