
#include "display.h"
#include "state.h"
#include "charging.h"
#include "gpio.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "batteries.h"

volatile system_state_t system_state = {STATE_RECEIVING, NULL, NULL};

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

    // Read battery voltages & thermistor temperatures
    bool needs_balancing = update_battery_status(&system_state.battery_status);

    // Poll power metrics
    update_charging_status(&system_state.charging_status, &ina260);

    switch(system_state.mode) {
        case STATE_MONITORING:

            if (needs_balancing) {
                system_state.mode = STATE_BALANCING;
                break;
            }

            // We only leave monitoring if charging begins
            if (is_receiving_charge()) {
                system_state.mode = STATE_RECEIVING;
            }

            break;
        case STATE_RECEIVING:

            if (needs_balancing) {
                system_state.mode = STATE_BALANCING;
                break;
            }

            // Adjust the duty cycle based on new power parameters
            adjust_duty_cycle(system_state.charging_status.duty_cycle_uint8);

            // Next state condition
            if (!is_receiving_charge()) {
                system_state.mode = STATE_MONITORING;
            }

            break;
        case STATE_BALANCING:

            system_state.charging_status.duty_cycle_uint8 = 0;
            // Stop charging until batteries are balanced
            adjust_duty_cycle(&system_state.charging_status);
            
            stop_supply_current();

            // Adjust GPIOs according to new battery status
            balance_cells(&system_state.battery_status);

            if (!needs_balancing) {
                system_state.mode = STATE_MONITORING;
            }

            break;
        case STATE_SLEEP:
            break;
        default:
            system_state.mode = STATE_MONITORING;
            break;

    }
    
    // Refresh display
    update_display(&display, &system_state);
}
