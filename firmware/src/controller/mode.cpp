#include "mode.h"

static mode_e mode;

mode_e get_mode() {
    return mode;
}

void set_mode(mode_e new_mode) {
    mode = new_mode;
}
