
#include "gpio.h"
#include <Arduino.h>

void configure_gpio() {
    pinMode(PWM_PIN, OUTPUT);
    pinMode(CELL1_VOLTAGE_PIN, INPUT);
    pinMode(CELL2_VOLTAGE_PIN, INPUT);
    pinMode(CELL1_TEMP_PIN, INPUT);
    pinMode(CELL2_TEMP_PIN, INPUT);
}
