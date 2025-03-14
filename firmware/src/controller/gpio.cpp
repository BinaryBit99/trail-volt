
#include "gpio.h"
#include <Arduino.h>

void gpio_init() {
    pinMode(PWM_PIN, OUTPUT);
    pinMode(TOTAL_CELL_ADC_PIN, INPUT);
    pinMode(LOWER_CELL_ADC_PIN, INPUT);
    pinMode(UPPER_CELL_TEMP_PIN, INPUT);
    pinMode(LOWER_CELL_TEMP_PIN, INPUT);
    pinMode(LOWER_CELL_DISCHARGE_PIN, OUTPUT);
    pinMode(UPPER_CELL_DISCHARGE_PIN, OUTPUT);

    // Keep pin high by default (shutdown off)
    pinMode(CHARGING_SHUT_DOWN_PIN, INPUT_PULLUP);
}
