### Pinout Diagram

| Pin | Function      | Description                        |
|-----|---------------|------------------------------------|
| A0  | ADC0          | Cell 1 Battery Voltage             |
| A1  | ADC1          | Cell 1 Battery Voltage             |
| A2  | ADC2          | Thermistor Voltage 1               |
| A3  | ADC3          | Thermistor Voltage 2               |
| A4  | SDA           | I2C SDA                            |
| A5  | SCL           | I2C SCL                            |
| D4  | DIGITAL OUT   | Upper Cell Discharge BJT Signal    |
| D3  | PWM OUTPUT    | PWM Output wave                    |
| D2  | DIGITAL OUT   | Lower Cell Discharge BJT Signal    |


### ADC
For each ADC input put, add a 150kOhm pull-down resistor.
