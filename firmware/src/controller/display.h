
#ifndef DISPLAY_H
#define	DISPLAY_H

#include "state.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define i2c_address 0x3c

#define DISP_WIDTH 128
#define DISP_HEIGHT 64

void configure_display(Adafruit_SH1106G *display);
void update_display(Adafruit_SH1106G *display, volatile const system_state_t *system_state);
void draw_logo(Adafruit_SH1106G *display);

#endif	/* DISPLAY_H */

