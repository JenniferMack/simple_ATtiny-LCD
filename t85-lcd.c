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

With LCD:

LCD must be in 4-bit mode, and shift register is sending data and r/s+enable command. LCD will only recoginze new data on rising clock, so SR must be cleared between nibbles.

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
void lcd_print(uint8_t &text);
void lcd_cmd(uint8_t cmd);
void lcd_init(void);
void lcd_shift(void);

int main(void)
{
    // PB0:2 as output
    DDRB |= 0x07;

}

void lcd_init(void)
{
    // Send 3 times for init
    lcd_cmd(0x30);
    lcd_cmd(0x30);
    lcd_cmd(0x30);
    // Set 4-bit mode, 2 lines, 5x7 dots
    lcd_cmd(0x28);
    // Display on, underline, no blink
    lcd_cmd(0x0E);
}

void lcd_print(uint8_t &text)
{
    //TODO: send string
}

void lcd_cmd(uint8_t cmd)
{
    uint8_t send;

    // Check for data in lower 4-bits, if zero, ok to send.
    if ( 0 == (cmd & 0x0F) )
    {
        // add RS+Enable to cmd
        send = cmd | 0x03;
        shiftByte(send,0);
    } else {
        // mask lower 4-bits, add RS+Enable
        send = (cmd & 0xF0) | 0x03;
        shiftByte(send,0);
        // reset SR
        shiftByte(0x00,0);
        // mask upper 4-bits, shift lower to upper, add RS+Enable
        send = ((cmd & 0x0F) << 4) | 0x03;
        shiftByte(send,0);
    }

    // Reset SR 
    shiftByte(0x00,0)
}

void lcd_shift(void)
{
    //TODO: parameters, 4-bit twice, clear SR after each send
}

void shiftByte(uint8_t shiftData, uint8_t bitOrder)
// Shift out bits, MSB or LSB first
{
    uint8_t i;
    
    for (i=0; i<8; i++)
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

