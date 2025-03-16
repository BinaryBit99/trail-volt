
#include "gpio.h"
#include <Arduino.h>

void gpio_init() {
    pinMode(THERMISTOR_1_PIN, INPUT);
    pinMode(THERMISTOR_2_PIN, INPUT);

    pinMode(TOTAL_CELL_ADC_PIN, INPUT);
    pinMode(LOWER_CELL_ADC_PIN, INPUT);

    pinMode(SWITCH_1_PIN, INPUT);

    // Keep pin high by default (shutdown off)
    pinMode(H_BRIDGE_SHUT_DOWN_PIN_AL, INPUT_PULLUP);

    pinMode(USB_ENABLE_PIN, OUTPUT);

    pinMode(C1_BALANCING_PIN, OUTPUT);
    pinMode(C2_BALANCING_PIN, OUTPUT);

    pinMode(CHARGE_PWM_PIN, OUTPUT);
}
