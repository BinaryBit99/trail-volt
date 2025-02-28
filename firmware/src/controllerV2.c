#include <Wire.h>

// Constants
#define INA260_PV_ADDR 0x40    // INA260 for PV (current/voltage)
#define INA260_LOAD_ADDR 0x41  // INA260 for load current
#define PWM_PIN 9              // PWM output pin (D9)
#define MAX_DUTY 255           // Max duty cycle (8-bit)
#define MIN_DUTY 0             // Min duty cycle
#define DUTY_STEP 5            // Perturbation step size
#define SAMPLE_TIME 100        // Control loop interval (ms)

// Variables
float pv_voltage, pv_current, pv_power;
float load_current, load_power;
float prev_pv_power = 0;
int duty_cycle = 0;            // Initial duty cycle

void setup() {
  pinMode(PWM_PIN, OUTPUT);
  analogWrite(PWM_PIN, MIN_DUTY); // Start with 0% duty cycle
  
  Wire.begin();
  configureINA260(INA260_PV_ADDR);
  configureINA260(INA260_LOAD_ADDR);
  
  Serial.begin(9600);
}

void loop() {
  static unsigned long last_time = 0;
  unsigned long now = millis();
  
  if (now - last_time >= SAMPLE_TIME) {
    // Read PV voltage and current
    pv_voltage = readINA260Voltage(INA260_PV_ADDR);
    pv_current = readINA260Current(INA260_PV_ADDR);
    pv_power = pv_voltage * pv_current;
    
    // Read load current
    load_current = readINA260Current(INA260_LOAD_ADDR);
    load_power = pv_voltage * load_current;
    
    // Perturb and Observe Algorithm
    if (pv_power > prev_pv_power && load_power <= pv_power) {
      // Increase duty cycle if PV power increased and load power is within limits
      duty_cycle += DUTY_STEP;
    } else {
      // Decrease duty cycle if PV power decreased or load power exceeds PV power
      duty_cycle -= DUTY_STEP;
    }
    
    // Constrain duty cycle to valid range
    duty_cycle = constrain(duty_cycle, MIN_DUTY, MAX_DUTY);
    analogWrite(PWM_PIN, duty_cycle);
    
    // Update previous PV power value
    prev_pv_power = pv_power;
    
    // Debugging output
    Serial.print("PV Voltage: ");
    Serial.print(pv_voltage);
    Serial.print("V, PV Current: ");
    Serial.print(pv_current);
    Serial.print("A, PV Power: ");
    Serial.print(pv_power);
    Serial.print("W, Load Current: ");
    Serial.print(load_current);
    Serial.print("A, Load Power: ");
    Serial.print(load_power);
    Serial.print("W, Duty: ");
    Serial.print((duty_cycle / 255.0) * 100);
    Serial.println("%");
    
    last_time = now;
  }
}

float readINA260Voltage(uint8_t address) {
  Wire.beginTransmission(address);
  Wire.write(0x02); // Bus voltage register
  Wire.endTransmission();
  
  Wire.requestFrom(address, 2);
  uint16_t voltage_raw = (Wire.read() << 8) | Wire.read();
  return voltage_raw * 0.00125; // LSB = 1.25mV
}

float readINA260Current(uint8_t address) {
  Wire.beginTransmission(address);
  Wire.write(0x01); // Current register
  Wire.endTransmission();
  
  Wire.requestFrom(address, 2);
  uint16_t current_raw = (Wire.read() << 8) | Wire.read();
  return current_raw * 0.00125; // LSB = 1.25mA
}

void configureINA260(uint8_t address) {
  // Configure for 16x averaging, 1ms conversion time
  Wire.beginTransmission(address);
  Wire.write(0x00); // Configuration register
  Wire.write(0x6F); // Upper byte
  Wire.write(0x27); // Lower byte
  Wire.endTransmission();
}
