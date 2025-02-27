
/* 
 * File:   display.h
 * Author: Kyle James
 *
 * Created on February 15, 2025, 6:40 PM
 */

#ifndef DISPLAY_H
#define	DISPLAY_H

#include "sensors.h"
#include "state.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define i2c_address 0x3c

void configure_display(Adafruit_SH1106G *display);
void update_display(Adafruit_SH1106G *display, volatile const system_state_t *system_state);
void draw_logo(Adafruit_SH1106G *display);

#endif	/* DISPLAY_H */

