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

float temperature_get_series_reading() {
    float resistance = get_resistance(THERMISTOR_2_PIN);

    /*
     * Yet to be implemented.
     */

    return 0;
}

float temperature_get_ground_reading() {
    float resistance = get_resistance(THERMISTOR_1_PIN);

    /*
     * Yet to be implemented.
     */
     
    return 0;
}
