
#ifndef GPIO_H
#define	GPIO_H

#include <Arduino.h>
 
/* Thermistor at ground connection */
const int THERMISTOR_1_PIN = A0;

/* Thermistor at series connection */
const int THERMISTOR_2_PIN = A1;

const int TOTAL_CELL_ADC_PIN = A2;
const int LOWER_CELL_ADC_PIN = A3;

// A4 & A5 are used for I2C  

/* A6 not functional on current board */
const int CHARGE_VOLTAGE_PIN = A7;

/* General purpose switch. */
const int SWITCH_1_PIN = 0;

/* Load side PWM duty cycle pin */
const int LOAD_PWM = 10;

/* Fed into the shutdown pin for our gate driver. Is active low. */
const int H_BRIDGE_SHUT_DOWN_PIN_AL = 5;

/* USB enable pin, allows USB port to supply charge. */
const int USB_ENABLE_PIN = 6;

/* Lower cell balancing active-high */
const int C1_BALANCING_PIN = 7;

/* Upper cell balancing active-high */
const int C2_BALANCING_PIN = 8;

/* PWM signal fed into the gate driver IC, controls gate voltages of the upper & lower NMOS. */
const int CHARGE_PWM_PIN = 9;


/**
 * @brief  Initializes GPIO pins.
 */
void gpio_init();

#endif /* GPIO_H */
