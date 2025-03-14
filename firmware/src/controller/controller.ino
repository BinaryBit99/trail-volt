/* Setting DEBUG to 1 enables serial logging. */
#include "debug.h"

#include "display.h"
#include "mode.h"
#include "charging.h"
#include "gpio.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "battery.h"
#include "state_manager.h"

#define LOOP_DELAY_MS 10

void setup() {
    D_SerialBegin(9600);
    delay(1000);
    D_printlnf("Starting.");

    gpio_init();
    charging_init();
    state_manager_init();

    display_init();
    display_draw_logo();
    delay(2500);
}

void loop() {
    // Update the application state (take ADC readings etc)
    state_manager_update_sensors();
    // Determine the next FSM state based on the new updated state
    state_manager_update_mode();
    // Make hardware updates
    state_manager_apply_hardware_updates();
    // Update display
    display_update();
    
    delay(LOOP_DELAY_MS);
}
