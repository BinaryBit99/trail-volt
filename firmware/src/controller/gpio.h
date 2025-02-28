#ifndef GPIO_H
#define	GPIO_H

#include <Arduino.h>

const int PWM_PIN = PD3;

const int TOTAL_CELL_ADC_PIN = A0;
const int LOWER_CELL_ADC_PIN = A1;
const int CELL1_TEMP_PIN = A2;     
const int CELL2_TEMP_PIN = A3;

const int LOWER_CELL_DISCHARGE_PIN = PD2;
const int UPPER_CELL_DISCHARGE_PIN = PD4;
// A4 & A5 are used for I2C  

void configure_gpio();

#endif /* GPIO_H */
