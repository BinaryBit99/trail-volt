
#include <SPI.h>
#include <Wire.h>
#include "display.h"
#include "Fonts/GFX_fonts/Font5x5Fixed.h"
#include "Fonts/GFX_fonts/Font5x7FixedMono.h"
#include "Fonts/FreeSansBold6pt7b.h"
#include "debug.h"

static Adafruit_SH1106G display = Adafruit_SH1106G(DISP_WIDTH, DISP_HEIGHT, &Wire, OLED_RESET);

void display_draw_logo() {
    // Define the logo bitmap
    static const unsigned char PROGMEM logo_bits[] = {
        // '3', 128x40px
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe3, 0x80, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 
        0x00, 0x1e, 0x00, 0x07, 0xff, 0x80, 0x00, 0x00, 0xe3, 0x80, 0x70, 0x1c, 0x00, 0x07, 0x00, 0x00, 
        0x00, 0x3e, 0x00, 0x07, 0xff, 0x80, 0x00, 0x00, 0xe3, 0x80, 0x70, 0x1c, 0x00, 0x07, 0x06, 0x00, 
        0x00, 0x3e, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x03, 0x80, 0x70, 0x38, 0x00, 0x07, 0x06, 0x00, 
        0x00, 0x7e, 0x00, 0x00, 0x70, 0x00, 0x01, 0x80, 0x03, 0x80, 0x38, 0x38, 0x0c, 0x07, 0x0f, 0x00, 
        0x00, 0x7e, 0x00, 0x00, 0x70, 0x1f, 0xcf, 0xf0, 0xe3, 0x80, 0x38, 0x38, 0x3f, 0x87, 0x1f, 0xe0, 
        0x00, 0xfe, 0x00, 0x00, 0x70, 0x1f, 0xcf, 0xf0, 0xe3, 0x80, 0x38, 0x70, 0x7f, 0xc7, 0x1f, 0xe0, 
        0x00, 0xff, 0xf8, 0x00, 0x70, 0x1e, 0x0c, 0x30, 0xe3, 0x80, 0x1c, 0x70, 0xe1, 0xc7, 0x07, 0x00, 
        0x01, 0xff, 0xf8, 0x00, 0x70, 0x1c, 0x00, 0x70, 0xe3, 0x80, 0x1c, 0x70, 0xe1, 0xe7, 0x06, 0x00, 
        0x01, 0xff, 0xf0, 0x00, 0x70, 0x1c, 0x03, 0xf0, 0xe3, 0x80, 0x1c, 0xe0, 0xe0, 0xe7, 0x06, 0x00, 
        0x03, 0xff, 0xf0, 0x00, 0x70, 0x1c, 0x0f, 0xf0, 0xe3, 0x80, 0x0e, 0xe1, 0xc0, 0xe7, 0x06, 0x00, 
        0x03, 0xff, 0xe0, 0x00, 0x70, 0x1c, 0x1e, 0x30, 0xe3, 0x80, 0x0e, 0xe0, 0xe0, 0xe7, 0x06, 0x00, 
        0x07, 0xff, 0xe0, 0x00, 0x70, 0x1c, 0x1c, 0x30, 0xe3, 0x80, 0x0f, 0xc0, 0xe1, 0xc7, 0x07, 0x00, 
        0x07, 0xff, 0xc0, 0x00, 0x70, 0x1c, 0x1c, 0x78, 0xe3, 0x80, 0x07, 0xc0, 0xf3, 0xc7, 0x87, 0x00, 
        0x00, 0x1f, 0xc0, 0x00, 0x70, 0x1c, 0x1f, 0xfc, 0xe3, 0xe0, 0x07, 0xc0, 0x7f, 0x83, 0xc7, 0xe0, 
        0x00, 0x0f, 0x80, 0x00, 0x70, 0x1c, 0x0f, 0x9c, 0xe1, 0xe0, 0x07, 0x80, 0x3f, 0x03, 0xc3, 0xe0, 
        0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    display.clearDisplay();
    display.drawBitmap(0, 12, logo_bits, 128, 40, SH110X_WHITE);
    display.display();
}

void display_init() {
    // Initialize the display
    if (!display.begin(i2c_address, true)) {
        D_printlnf("SH1106 allocation failed");
        for (;;); // Don't proceed, loop forever
    }

    // Clear the buffer
    display.clearDisplay();

    // Set the font
    display.setFont(&Font5x5Fixed);
    display.setTextColor(SH110X_WHITE);
    display.setRotation(0);
}

void display_update() {

    battery_state_t battery_state = battery_get_state();
    charging_state_t charging_state = charging_get_state();
    mode_e mode = get_mode();

    display.clearDisplay();
    
    // Battery dimensions and positions
    uint8_t battery_width = 16;
    uint8_t battery_height = 35;
    uint8_t terminal_width = 6;
    uint8_t terminal_height = 3;
    uint8_t battery_spacing = 8;
    uint8_t start_x = 0;
    uint8_t start_y = 10; // Moved down slightly to fit temperature text above
    
    // Text area for sensor values
    uint8_t text_start_x = start_x + 2 * battery_width + battery_spacing + 8;
    uint8_t text_start_y = 0;
    uint8_t line_height = 8;
    
    // Maximum battery voltage
    float max_voltage = 4.2;
    
    // Draw Battery 1
    display.drawRect(start_x, start_y + terminal_height, battery_width, battery_height, SH110X_WHITE);
    display.fillRect(start_x + (battery_width - terminal_width) / 2, start_y, terminal_width, terminal_height, SH110X_WHITE);
    
    // Calculate fill level for Battery 1
    float volt1 = battery_state.upper_cell_voltage_v;
    uint8_t fill_height1 = (volt1 * battery_height) / max_voltage;
    if (fill_height1 > battery_height) fill_height1 = battery_height;
    
    // Draw fill level
    display.fillRect(start_x + 2, start_y + terminal_height + battery_height - fill_height1, battery_width - 4, fill_height1, SH110X_WHITE);
    
    // Draw Battery 2
    uint8_t batt2_x = start_x + battery_width + battery_spacing;
    display.drawRect(batt2_x, start_y + terminal_height, battery_width, battery_height, SH110X_WHITE);
    display.fillRect(batt2_x + (battery_width - terminal_width) / 2, start_y, terminal_width, terminal_height, SH110X_WHITE);
    
    // Calculate fill level for Battery 2
    float volt2 = battery_state.lower_cell_voltage_v;
    uint8_t fill_height2 = (volt2 * battery_height) / max_voltage;
    if (fill_height2 > battery_height) fill_height2 = battery_height;
    
    // Draw fill level
    display.fillRect(batt2_x + 2, start_y + terminal_height + battery_height - fill_height2, battery_width - 4, fill_height2, SH110X_WHITE);
    
    /* Need to adjust temperature reading posistions, as they are not directly attached to individual cells */

    // display.setCursor(start_x + (battery_width - 14) / 2, start_y - 5);
    // display.print(battery_state.upper_cell_temperature_c);
    // display.print("C");

    // display.setCursor(batt2_x + (battery_width - 14) / 2, start_y - 5);
    // display.print(battery_state.lower_cell_temperature_c);
    // display.print("C");
    
    // Display voltage readings under respective batteries
    // Battery 1 voltage
    display.setCursor(start_x + (battery_width - 15) / 2, start_y + terminal_height + battery_height + 10);
    display.print(volt1, 2);  // Print voltage with 2 decimal places
    
    // Battery 2 voltage
    display.setCursor(batt2_x + (battery_width - 15) / 2, start_y + terminal_height + battery_height + 10);
    display.print(volt2, 2);  // Print voltage with 2 decimal places

    // Display other sensor values on the right side
    uint8_t y_pos = text_start_y + 10;

    float total_voltage = volt1 + volt2;
    float percentage_duty_cycle = charging_state.duty_cycles.duty_mppt * ((float)100/255);

    switch (mode) {
        case MODE_SLEEP:

            break;
        case MODE_RECEIVING:

            display.setFont(&FreeSansBold6pt7b);
            display.setCursor(text_start_x, y_pos);
            display.print("RECEIVING");
            display.setTextSize(1);
            display.setFont(&Font5x5Fixed);
            y_pos += line_height;

            // Total cell voltage
            display.setCursor(text_start_x, y_pos);
            display.print("V-BAT: ");
            display.print(total_voltage, 2);  // Print average voltage with 2 decimal places
            display.print("V");
            y_pos += line_height;

            // Supply voltage as seen by the batteries taken from ADC
            display.setCursor(text_start_x, y_pos);
            display.print("V-SUP-DIV: ");
            display.print(charging_state.power_metrics.charge_voltage_v, 2);  // Print average voltage with 2 decimal places
            display.print("V");
            y_pos += line_height;


            // Supply voltage as seen by the batteries taken from INA
            display.setCursor(text_start_x, y_pos);
            display.print("V-SUP-INA: ");
            display.print(charging_state.power_metrics.ina_bus_voltage_v, 2);  // Print average voltage with 2 decimal places
            display.print("V");
            y_pos += line_height;

            // Supply current as seen by the batteries
            display.setCursor(text_start_x, y_pos);
            display.print("A-SUP: ");
            display.print(charging_state.power_metrics.ina_current_ma, 2);  // Print average voltage with 2 decimal places
            display.print("mA");
            y_pos += line_height;

            display.setCursor(text_start_x, y_pos);
            display.print("Duty Cycle: ");
            display.print(percentage_duty_cycle, 0);
            display.print("%");
            y_pos += line_height;

            display.setCursor(text_start_x, y_pos);
            if (battery_state.lower_discharging) {
                display.print("DRAINING LOWER");
            } else if (battery_state.upper_discharging) {
                display.print("DRAINING UPPER");
            }
            y_pos += line_height;

            break;
        case MODE_BALANCING:
            display.setFont(&FreeSansBold6pt7b);
            display.setCursor(text_start_x, y_pos);
            display.print("BALANCING");
            display.setTextSize(1);
            display.setFont(&Font5x5Fixed);
            y_pos += line_height;

            // Total voltage
            display.setCursor(text_start_x, y_pos);
            display.print("V-BAT: ");
            display.print(total_voltage, 2);  // Print average voltage with 2 decimal places
            display.print("V");
            y_pos += line_height;

            display.setCursor(text_start_x, y_pos);
            display.print(F("CELL-DIFF: "));
            display.print(battery_get_voltage_difference(), 2);  // Print average voltage with 2 decimal places
            display.print("V");
            y_pos += line_height;

            display.setCursor(text_start_x, y_pos);
            display.print("Duty Cycle: ");
            display.print(percentage_duty_cycle, 0);
            display.print("%");
            y_pos += line_height;

            display.setCursor(text_start_x, y_pos);
            if (battery_state.lower_discharging) {
                display.print("DRAINING LOWER");
            } else if (battery_state.upper_discharging) {
                display.print("DRAINING UPPER");
            }
            y_pos += line_height;

            break;
        case MODE_SUPPLYING:
            display.setFont(&FreeSansBold6pt7b);
            display.setCursor(text_start_x, y_pos);
            display.print("SUPPLYING");
            display.setTextSize(1);
            display.setFont(&Font5x5Fixed);
            y_pos += line_height;
            break;
        case MODE_CHARGING_FAULT:
            display.setFont(&FreeSansBold6pt7b);
            display.setCursor(text_start_x, y_pos);
            display.print("FAULT");
            display.setFont(&Font5x5Fixed);
            y_pos += line_height;
            break;
        case MODE_MONITORING:

            display.setFont(&FreeSansBold6pt7b);
            display.setCursor(text_start_x, y_pos);
            display.print("MONITORING");
            display.setFont(&Font5x5Fixed);
            y_pos += line_height;

            display.setCursor(text_start_x, y_pos);
            display.print("V-BAT: ");
            display.print(total_voltage, 2);  // Print average voltage with 2 decimal places
            display.print("V");
            y_pos += line_height;

            display.setCursor(text_start_x, y_pos);
            display.print("Duty Cycle: ");
            display.print(percentage_duty_cycle, 0);
            display.print("%");
            y_pos += line_height;

            display.setCursor(text_start_x, y_pos);
            if (battery_state.lower_discharging) {
                display.print("DRAINING LOWER");
                
            } else if (battery_state.upper_discharging) {
                display.print("DRAINING UPPER");
            }
            break;
        default:
            break;
    }

    display.display();
}

