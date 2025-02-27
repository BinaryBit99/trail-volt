// main.ino
#include "sensors.h"
#include "display.h"
#include "state.h"
#include "power.h"
#include "gpio.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>



// PWM Configuration
#define PWM_PIN 9
#define PWM_FREQ 20000
#define PRESCALER 1
#define TOP_VALUE (F_CPU / (PRESCALER * PWM_FREQ) - 1)

// MPPT Parameters
#define STEP_SIZE 1      // Adjustment step size (1-5)
#define SAMPLE_DELAY 50  // Stabilization time after adjustment (ms)

volatile int currentDuty = TOP_VALUE / 2;  // Start at 50% duty
float prevPower = 0;
bool increasing = true;

battery_state_t battery_state = { 0, 0, 0, 0 };
charge_state_t charge_state = { 0, 0, 0 };

volatile application_state_t application_state = {STATE_MONITORING, battery_state, charge_state};

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO

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
    switch(application_state.mode) {
        case STATE_MONITORING:
            // Read battery voltages & thermistor temperatures
            update_battery_state(&application_state.battery_state);

            update_display(&display, &application_state);

            // We only leave monitoring if charging begins
            if (is_charging()) {
                application_state.mode = STATE_CHARGING;
            }

            break;
        case STATE_CHARGING:

            // Poll INA for current, voltage, and power readings
            update_charge_state(&application_state.charge_state, &ina260);

            // Adjust the duty cycle based on new power parameters
            adjust_duty_cycle(&application_state.charge_state);

            // Read battery voltages & thermistor temperatures
            update_battery_state(&application_state.battery_state);

            update_display(&display, &application_state);

            // Next state condition
            if (!is_charging()) {
                application_state.mode = STATE_MONITORING;
            }

            break;
        case STATE_SLEEP:
            break;
        default:
            application_state.mode = STATE_MONITORING;
            break;
    }
}
