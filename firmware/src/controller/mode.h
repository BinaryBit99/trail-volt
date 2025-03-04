#ifndef MODE_H
#define	MODE_H

typedef enum {
  MODE_SLEEP,
  MODE_RECEIVING,
  MODE_SUPPLYING,
  MODE_MONITORING,
  MODE_BALANCING
} mode_e;

mode_e get_mode();
void set_mode(mode_e mode);

#endif	/* APPLICATION_H */



