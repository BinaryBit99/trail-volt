#ifndef MODE_H
#define	MODE_H

typedef enum {
  MODE_SLEEP = 0,
  MODE_RECEIVING = 1,
  MODE_CHARGING_FAULT = 2,
  MODE_SUPPLYING = 3,
  MODE_MONITORING = 4,
  MODE_BALANCING = 5,
} mode_e;

mode_e get_mode();
void set_mode(mode_e mode);

#endif	/* APPLICATION_H */



