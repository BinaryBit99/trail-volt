#include "temperature.h"
#include "adc.h"

#include <math.h>
#include <stdint.h>

#define ADC_MAX   1023.0f   // Arduino Nano has a 10-bit ADC (0-1023)

typedef uint16_t u16; // Use unsigned 16 bit readings for NANO


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

// The below function returns the temp. of thermistor based on the varying resistance.

float getTempfromThermistor(int pin) {
    const float R0 = 10000.0f;
    const float R_fixed = 40000.0f;
    const float Beta = 3380.0f;            // These were found in datasheet.
    const float T0 = 298.15f;
    
    float r_ntc = get_resistance(pin);        // This seems more abstract; less clutter for code at the expense of less modularity.
    
    if (adc_value <= 0 || adc_value >= ADC_MAX) return -999.0f; // Use this right away to maintain 'safe' bounds - can adjust if needed. I set ADC_MAX to 5.

    // Use a Steinhart equation for conversions.
    float steinhart = (1.0f / T0) + (1.0f / Beta) * log(r_ntc / R0);
    float t_kelvin = 1.0f / steinhart;
    float t_celsius = t_kelvin - 273.15f;
    
    return t_celsius;

}

void testTemperature(float t_celsius){
    int lowerBound = 15;
    int higherBound = 40;
    if(t_celsius > lowerBound && u32_tempK < t_celsius){
        PORTA.OUT |= 0x01;
    }
    else{
        PORTA.OUT &= ~(0x01);
    }
}



