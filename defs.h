/*
defs.h
the usual suspects
*/

#define PIN_HI(port,pin) PORT##port |= _BV(pin)
#define PIN_LO(port,pin) PORT##port &= ~_BV(pin)
