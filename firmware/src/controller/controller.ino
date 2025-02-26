// main.ino
#include "sensors.h"
#include "display.h"
#include "state.h"
#include "power.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#include <Wire.h>
#include <Adafruit_INA260.h>

Adafruit_INA260 ina260 = Adafruit_INA260();

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

volatile application_state_t application_state = STATE_MONITORING;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

sensor_state_t sensor_state = {0, 0, 0, 0, false};

void setup() {
    Serial.begin(115200);
    pinMode(PWM_PIN, OUTPUT);

    // Initialize PWM Timer
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1A |= (1 << COM1A1) | (1 << WGM11);
    TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS10);
    ICR1 = TOP_VALUE;
    OCR1A = currentDuty;

    // Initialize INA260
    if (!ina260.begin()) {
       Serial.println("Couldn't find INA260");
       while (1);
    }
    configure_display(&display);
    draw_logo(&display);
    delay(2500);

    pinMode(CELL1_VOLTAGE_PIN, INPUT);
    pinMode(CELL2_VOLTAGE_PIN, INPUT);
    pinMode(CELL1_TEMP_PIN, INPUT);
    pinMode(CELL2_TEMP_PIN, INPUT);
    pinMode(CHARGE_RATE_PIN, INPUT);
}

void loop() {
    // Read voltage and current
    float voltage = ina260.readBusVoltage();
    float current = ina260.readCurrent();
    float power = voltage * current;
    switch(application_state) {
        case STATE_MONITORING:
            check_update_state(&application_state);
            update_sensor_state(&sensor_state);
            update_display(&display, &sensor_state, &application_state);
            break;
        case STATE_CHARGING:
            while (is_charging()) {}
            application_state = STATE_MONITORING;
            break;
        case STATE_SLEEP:
            break;
        default:
            break;
    }
    // Perturb and Observe Algorithm
    if (power > prevPower) {
       // Keep moving in same direction
       currentDuty += increasing ? STEP_SIZE : -STEP_SIZE;
    } else {
       // Reverse direction
       increasing = !increasing;
       currentDuty += increasing ? STEP_SIZE : -STEP_SIZE;
     }

     // Ensure duty stays within bounds
     currentDuty = constrain(currentDuty, 0, TOP_VALUE);

     // Update PWM and store power
     OCR1A = currentDuty;
     prevPower = power;

     // Add debug output
     Serial.print("Voltage: "); Serial.print(voltage, 2); Serial.print("V\t");
     Serial.print("Current: "); Serial.print(current, 2); Serial.print("A\t");
     Serial.print("Power: "); Serial.print(power, 2); Serial.print("W\t");
     Serial.print("Duty: "); Serial.println(map(currentDuty, 0, TOP_VALUE, 0, 1000)/10.0, 1);

     delay(SAMPLE_DELAY);
}
