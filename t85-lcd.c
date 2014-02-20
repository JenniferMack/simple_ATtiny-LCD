/*
+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+
Program: t85-lcd.c
Created on: Thu Feb 13 19:00:47 MST 2014
Notes: 74HC595 shift register
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
// Send MSB first
#define SHIFT_BIT_ORDER     0xF

// Function prototypes
void shiftByte(uint8_t shiftData);
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

    // Test output
    lcdPrint("@shotbyjenn");
    lcdCmd(0xC0);
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
    // Send raw, in 8-bit mode, pulsing enable the hard way
    // If sent with lcdSend(), will get an enable pulse on 
    // upper and lower nibble. Which breaks everything.
    shiftByte(0x31);
    shiftByte(0x30);
    shiftByte(0x31);
    shiftByte(0x30);
    shiftByte(0x31);
    shiftByte(0x30);
    // Start 4-bit mode, with 8-bit command,
    // must send with only one enable pulse
    shiftByte(0x21);
    shiftByte(0x20);
    // Resend 4-bit mode & display off, as upper/lower bits
    lcdCmd(0x28);
    _delay_us(500);
    // Clear display, while display off
    // Seems odd, but required
    lcdCmd(0x01);
    _delay_us(500);
    // Increment, no shift - default, not sending
    //lcdCmd(0x06);
    //_delay_ms(5);
    //
    // Display on, underline, no blink
    lcdCmd(0x0C);
    // Ready for use
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
    shiftByte((send|cmdChar));
    _delay_us(500);

    // Enable low, keeping data on output
    shiftByte(send);
    _delay_us(500);

    // mask upper 4-bits, shift lower to upper, add RS/Enable
    send = ((data & 0x0F) << 4);
    shiftByte((send|cmdChar));
    _delay_us(500);

    // Enable low, keeping data on output
    shiftByte(send);
    _delay_us(500);
}

void shiftByte(uint8_t shiftData)
// Shift out bits, MSB or LSB first
{
    uint8_t i;
    
    for (i=0; i<8; i++)
    {
        if ( 0 == SHIFT_BIT_ORDER )
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

