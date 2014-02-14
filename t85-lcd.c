/*
+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+
Program: t85-lcd.c
Created on: Thu Feb 13 19:00:47 MST 2014
Notes:
Using 74HC595 shift register

 DESCRIPTION

Setup:
Hold input B high, input A is data.
Pin 8 is clock.
Pin 9 is pulled low, clears register
Pin 14 is Vcc
Pin 7 is Gnd

Output:
Bits are shift sequentially, so the first bit sent will be
on QH after all 8 have been sent.

Setup using MSB first:
    Pin | Name | Bit | Value
    ------------------------
     15 |  QA  |  0  | 1
      1 |  QB  |  1  | 2
      2 |  QC  |  2  | 4
      3 |  QD  |  3  | 8
      4 |  QE  |  4  | 16
      5 |  QF  |  5  | 32
      6 |  QG  |  6  | 64
      7 |  QH  |  7  | 128

ATtiny85:
           ----
    RST  -|    |- VCC
    PB3  -|    |- PB2
    PB4  -|    |- PB1
    GND  -|    |- PB0
           ----
+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+
*/

#include <avr/io.h>
#include <util/delay.h>
// ATtiny85 pins
#define SROUT   PB0
#define SCK     PB1
#define RCK     PB2

// Function prototypes
void shiftByte(uint8_t shiftData, uint8_t bitOrder);

int main(void)
{
    DDRB |= 0x1F;

    while (1==1) {
        for (uint8_t i=0; i<256; i++)
        {
            shiftByte(i,1);
            _delay_ms(150);
        }
    }

    return 0;
}

void shiftByte(uint8_t shiftData, uint8_t bitOrder)
// Shift out bits
{
    for (uint8_t i=0; i<8; i++)
    {
        if ( 0 == bitOrder )
        {
            // LSB first
            if ( 0 == shiftData & _BV(i) )
                PORTB &= ~_BV(SROUT);
            else
                PORTB |= _BV(SROUT);
        } else {
            // MSB first
            if ( 0 == ( shiftData & _BV((7 - i)) ) )
                PORTB &= ~_BV(SROUT);
            else
                PORTB |= _BV(SROUT);
        }
        // Pulse shift register clock low to high
        PORTB |= _BV(SCK);
        PORTB &= ~_BV(SCK);
    }
    // Pulse RCK to put data on output pins
    PORTB |= _BV(RCK);
    PORTB &= ~_BV(RCK);

    // all outputs low
    PORTB &= ~_BV(SROUT);
    PORTB &= ~_BV(SCK);
    PORTB &= ~_BV(RCK);

}   

