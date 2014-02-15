/*
+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+
Program: t85-lcd.c
Created on: Thu Feb 13 19:00:47 MST 2014
Notes:

Using 74HC595 shift register

Description:
The M74HC595 device is a high speed CMOS 8-bit shift register with output latches (3-state) fabricated with silicon gate CMOS technology.

This device contains an 8-bit serial in, parallel out shift register that feeds an 8-bit D-type storage register. The storage register has 8 3-state outputs. Separate clocks are provided for both the shift register and the storage register.

The shift register has direct overriding clear, serial input, and serial output (standard) pins for cascading. Both the shift register and storage register use positive edge triggered clocks. If both clocks are connected together, the shift register state will always be one clock pulse ahead of the storage register.

All inputs are equipped with protection circuits against static discharge and transient excess voltage.

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
           ____
    RST  -|    |- VCC
    PB3  -|    |- PB2
    PB4  -|    |- PB1
    GND  -|____|- PB0

+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+
*/

#include <avr/io.h>
#include <util/delay.h>
#include "defs.h"

// ATtiny85 pins
#define SROUT   PB0
#define SCK     PB1
#define RCK     PB2

// Function prototypes
void shiftByte(uint8_t shiftData, uint8_t bitOrder);

int main(void)
{
    // PB0:2 as output
    DDRB |= 0x07;

    uint8_t i, j[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

    while (1==1)
    {
        for (i=0;i<8;i++) {
            shiftByte(j[i],0);
            _delay_ms(125);
        }
            
        for (i=0;i<8;i++) {
            shiftByte(j[i],1);
            _delay_ms(125);
        }
    }
}

void shiftByte(uint8_t shiftData, uint8_t bitOrder)
// Shift out bits, MSB or LSB first
{
    for (uint8_t i=0; i<8; i++)
    {
        if ( 0 == bitOrder )
        {
            // LSB first
            if ( 0 == ( shiftData & _BV(i) ) )
                PIN_LO(B,SROUT);
            else
                PIN_HI(B,SROUT);
        } else {
            // MSB first
            if ( 0 == ( shiftData & _BV(7-i) ) )
                PIN_LO(B,SROUT);
            else
                PIN_HI(B,SROUT);
        }
        // Pulse shift register clock low to high
        PIN_HI(B,SCK);
        PIN_LO(B,SCK);
    }
    // Pulse RCK to put data on output pins
    PIN_HI(B,RCK);
    PIN_LO(B,RCK);

    // Finish with all outputs low
    PIN_LO(B,SROUT);
}   

