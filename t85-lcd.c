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
#include <string.h>
#include <util/delay.h>

// ATtiny85 pins
#define SROUT   PB0
#define SCK     PB1
#define RCK     PB2

// AVR shortcuts
#define PIN_HI(port,pin) PORT##port |= _BV(pin)
#define PIN_LO(port,pin) PORT##port &= ~_BV(pin)

// Function prototypes
void shiftByte(uint8_t shiftData, uint8_t bitOrder);
void lcdSend(uint8_t data, uint8_t cmdChar);
void lcdPrint(char *text);
void lcdCmd(uint8_t cmd);
void lcdInit(void);

int main(void)
{
    // PB0:2 as output
    DDRB |= 0x07;
    // startup lcd
    lcdInit();

    //TODO: bit order for text?
    lcdPrint("Jenny Mack");

    while (1==1)
    {
        //
    }
}

void lcdInit(void)
{
    // wait a bit
    _delay_ms(250);
    // Send raw, for 8-bit mode, pulsing enable
    shiftByte(0x31,0x01);
    shiftByte(0x30,0x01);
    shiftByte(0x31,0x01);
    shiftByte(0x30,0x01);
    shiftByte(0x31,0x01);
    shiftByte(0x30,0x01);
    shiftByte(0x21,0x01);
    shiftByte(0x20,0x01);
    // Confirm 4-bit mode, display off
    lcdCmd(0x28);
    _delay_us(500);
    // Display off - default, not sending
    //lcdCmd(0x08);
    //_delay_ms(5);
    // Clear display
    lcdCmd(0x01);
    _delay_us(500);
    // Increment, no shift - default, not sending
    //lcdCmd(0x06);
    //_delay_ms(5);
    // Display on, underline, no blink
    lcdCmd(0x0E);
}

void lcdPrint(char *text)
{
    int i;

    for (i=0;i<strlen(text);i++)
    {
        lcdSend(text[i], 0x03);
    }
}

void lcdCmd(uint8_t cmd)
{
    lcdSend(cmd, 0x01);
}

void lcdSend(uint8_t data, uint8_t cmdChar)
{
    uint8_t send;

    // mask lower 4-bits, add RS/Enable
    send = (data & 0xF0);
    shiftByte((send|cmdChar),1);

    _delay_us(500);
    // reset SR
    shiftByte(send,1);

    _delay_us(500);

    // mask upper 4-bits, shift lower to upper, add RS/Enable
    send = ((data & 0x0F) << 4);
    shiftByte((send|cmdChar),1);
    _delay_us(500);

    shiftByte(send,1);

    _delay_us(500);
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

