#include "supplying.h"
#include <Arduino.h>
#include "gpio.h"

bool supplying_is_enabled() {
    bool supplying_enable_status = digitalRead(BTN_1_PIN);

    if (supplying_enable_status == HIGH) {
        return true;
    }
    return false;
}
