#ifndef GPIO_H
#define	GPIO_H

#include <Arduino.h>

const int PWM_PIN = PD3;

const int CELL1_VOLTAGE_PIN = A0;
const int CELL2_VOLTAGE_PIN = A1;
const int CELL1_TEMP_PIN = A2;     
const int CELL2_TEMP_PIN = A3;  
// A4 & A5 are used for I2C  


void configure_gpio();

#endif /* GPIO_H */
