#include <Arduino.h>
#include "mppt.h"
#include "adc.h"

// Constants (adjust based on your hardware)
#define STEP_SIZE       1     // Perturbation step size (1-5%)
#define MIN_DUTY       0      // Minimum duty cycle (0%)
#define MAX_DUTY       255    // Maximum duty cycle (100%)
#define INITIAL_DUTY   127    // Starting duty cycle (~50%)

static charging_state_t prev_charging_state;
static bool first_run = true;
static int8_t dir = 1;         // Perturbation direction: 1 = increase, -1 = decrease
static uint8_t prev_duty = INITIAL_DUTY; // Track previous duty cycle

duty_cycles_t mppt_calculate_duty_cycles(charging_state_t charging_state) {
    uint8_t pwmDutyCycle = prev_duty; // Start with previous duty
    uint8_t loadDutyCycle = 0;

    if (first_run) {
        // Initialize previous state and duty cycle
        prev_charging_state = charging_state;
        first_run = false;
        prev_duty = INITIAL_DUTY;

        duty_cycles_t duty_cycles = {
            .duty_mppt = INITIAL_DUTY,
            .duty_load = loadDutyCycle
        };
        return duty_cycles;
    }

    // Get voltage/current readings
    float v_prev = prev_charging_state.power_metrics.ina_bus_voltage_v;
    float i_prev = prev_charging_state.power_metrics.ina_current_ma / 1000.0f; // mA → A
    float v_new = charging_state.power_metrics.ina_bus_voltage_v;
    float i_new = charging_state.power_metrics.ina_current_ma / 1000.0f;

    // Calculate power
    float prev_power = v_prev * i_prev;
    float current_power = v_new * i_new;
    float delta_power = current_power - prev_power;

    // --- Existing load control logic (unchanged) ---
    float delta_v = v_new - v_prev;
    float verror = fabs(delta_v);
    if (verror > 0) {
        loadDutyCycle -= STEP_SIZE;
    } else {
        loadDutyCycle += STEP_SIZE;
    }

    // --- P&O MPPT Algorithm ---
    if (delta_power > 0) {
        // Power increased: keep direction
        pwmDutyCycle += dir * STEP_SIZE;
    } else if (delta_power < 0) {
        // Power decreased: reverse direction
        dir *= -1;
        pwmDutyCycle += dir * STEP_SIZE;
    }

    // Constrain duty cycle
    pwmDutyCycle = constrain(pwmDutyCycle, MIN_DUTY, MAX_DUTY);

    // Update previous state and duty
    prev_duty = pwmDutyCycle;
    prev_charging_state = charging_state;

    duty_cycles_t duty_cycles = {
        .duty_mppt = pwmDutyCycle,
        .duty_load = loadDutyCycle
    };

    return duty_cycles;
}
