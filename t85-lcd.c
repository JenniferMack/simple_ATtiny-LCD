/*
+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+-~-+
Program: t85-lcd.c
Created on: Sun Feb  9 14:20:47 MST 2014
Notes:
Using 74HC164 shift register

 DESCRIPTION
These 8-bit shift registers feature AND-gated serial
inputs and an asynchronous clear (CLR) input. The
gated serial (A and B) inputs permit complete control
over incoming data; a low at either input inhibits entry
of the new data and resets the first flip-flop to the low
level at the next clock (CLK) pulse. A high-level input
enables the other input, which then determines the
state of the first flip-flop. Data at the serial inputs can
be changed while CLK is high or low, provided the
minimum setup time requirements are met. Clocking
occurs on the low-to-high-level transition of CLK.

Setup:
Hold input B high, input A is data.
Pin 8 is clock.
Pin 9 is pulled low, clears register
Pin 14 is Vcc
Pin 7 is Gnd

Output:
Bits are shift sequentially, so the first bit sent will be
on QH after all 8 have been sent.

TODO: This may be backwards
Pin | Bit | Value
-----------------
  3 |  0  | 1
  4 |  1  | 2
  5 |  2  | 4
  6 |  3  | 8
 10 |  4  | 16
 11 |  5  | 32
 12 |  6  | 64
 13 |  7  | 128

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
#define EABLE   PB1
#define RGSEL   PB2
#define SROUT   PB3
#define CLOCK   PB4

// Function prototypes
void shiftByte(uint8_t data);
void enableLCD(void);
void charORcmd(uint8_t opt);

int main(void)
{
    DDRB |= 0x1F;

    _delay_ms(2);
    shiftByte(0x30);
    charORcmd(0x00);
    enableLCD();
    _delay_ms(2);
    shiftByte(0x30);
    charORcmd(0x00);
    enableLCD();
    _delay_ms(2);
    shiftByte(0x30);
    charORcmd(0x00);
    enableLCD();

    _delay_ms(2);
    shiftByte(0x0E);
    charORcmd(0x00);
    enableLCD();
    _delay_ms(2);
    shiftByte(0x38);
    charORcmd(0x00);
    enableLCD();
    _delay_ms(2);
    shiftByte(0x01);
    charORcmd(0x00);
    enableLCD();

    for (uint8_t i=0x41; i<0x41+0x28; i++)
    {
        shiftByte(i);
        charORcmd(1);
        enableLCD();
    }

    while (1==1) {

        shiftByte(0x18);
        charORcmd(0);
        enableLCD();
        _delay_ms(200);
    }

    return 0;
}

void enableLCD(void)
{
    // pulse enable pin
    PORTB |= _BV(EABLE);
    _delay_ms(5);
    PORTB &= ~_BV(EABLE);
}

void charORcmd(uint8_t opt)
{
    if ( 0 == opt )
    {
        PORTB &= ~_BV(RGSEL);
    } else {
        PORTB |= _BV(RGSEL);
    }
}

/*
void LCDprint(uint8_t xxx)
{
    for (string stuff)
    {
        //
    }
}
*/

void shiftByte(uint8_t data)
// Shift out bits
{
    for (uint8_t i=0; i<8; i++)
    {
        // LSB first
        //if ( 0 == (data & _BV(i) )
        // MSB first
        if ( 0 == (data & _BV((7 - i))) )
        {
            PORTB &= ~_BV(SROUT);
        } else {
            PORTB |= _BV(SROUT);
        }
        // Pulse clock low to high
        PORTB &= ~_BV(CLOCK);
        PORTB |= _BV(CLOCK);
    }
    // set pins low
    PORTB &= ~_BV(SROUT);
    PORTB &= ~_BV(CLOCK);
}   

