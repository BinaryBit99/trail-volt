// main.ino
#include "sensors.h"
#include "display.h"
#include "state.h"
#include "power.h"
#include "gpio.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "batteries.h"

battery_status_t battery_status = { 0, 0, 0, 0 };
charging_status_t charging_status = { 0, 0, 0,0, false, false };

volatile system_state_t system_state = {STATE_MONITORING, battery_status, charging_status};

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
}

void loop() {
    switch(system_state.mode) {
        case STATE_MONITORING:
            // Read battery voltages & thermistor temperatures
            update_battery_status(&system_state.battery_status);

            // Check if cells need passive balancing
            handle_cell_balancing(&system_state.battery_status);

            update_display(&display, &system_state);
            
            // We only leave monitoring if charging begins
            if (is_charging()) {
                system_state.mode = STATE_CHARGING;
            }

            break;
        case STATE_CHARGING:

            // Poll INA for current, voltage, and power readings
            update_charging_status(&system_state.charging_status, &ina260);

            // Adjust the duty cycle based on new power parameters
            adjust_duty_cycle(&system_state.charging_status);

            // Read battery voltages & thermistor temperatures
            update_battery_status(&system_state.battery_status);

            update_display(&display, &system_state);

            // Next state condition
            if (!is_charging()) {
                system_state.mode = STATE_MONITORING;
            }

            break;
        case STATE_SLEEP:
            break;
        default:
            system_state.mode = STATE_MONITORING;
            break;

        Serial.println("out");
    }
}
