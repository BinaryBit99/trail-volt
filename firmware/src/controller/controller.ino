
#include "display.h"
#include "state.h"
#include "charging.h"
#include "gpio.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "batteries.h"
#include "state_manager.h"

#define LOOP_DELAY_MS 10

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_INA260 ina260 = Adafruit_INA260();

void setup() {
    Serial.begin(9600);
    Serial.println("Starting");

    // Initialize INA260
    if (!ina260.begin()) {
       Serial.println("Couldn't find INA260");
       //while (1);
    }
    
    configure_display(&display);
    draw_logo(&display);
    delay(2500);

    configure_gpio();

    analogReference(DEFAULT);

    state_manager_init();
}

void loop() {

    // Updates all the readings from peripherals
    state_manager_update_sensor_readings(&ina260);
    
    // Process the current state to determine which commands need to be executed
    system_command_t cmd = state_manager_process_state();
    
    // Apply the commands to update state and control hardware
    state_manager_apply_command(&cmd);
    
    // Update the display with current system state
    update_display(&display, state_manager_get_state());
    
    delay(10);
}
