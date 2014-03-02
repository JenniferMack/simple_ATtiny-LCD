# ATtiny85 with 75HC595 Shift Register+LCD


Datasheets:

- ATtiny85 - [Complete](http://www.atmel.com/Images/Atmel-2586-AVR-8-bit-Microcontroller-ATtiny25-ATtiny45-ATtiny85_Datasheet.pdf) / [Summary](http://www.atmel.com/Images/Atmel-2586-AVR-8-bit-Microcontroller-ATtiny25-ATtiny45-ATtiny85_Datasheet-Summary.pdf)
- M74HC595 - [ST Micro](http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/CD00000339.pdf)
- HD44789U (LCD driver) - [Spark Fun](https://www.sparkfun.com/datasheets/LCD/HD44780.pdf)

## Datasheet Description

### ATtiny85

> The ATtiny25/45/85 is a low-power CMOS 8-bit microcont roller based on the AVR enhanced RISC architecture. By executing powerful instructions in a single clock cycle, the ATtiny25/45/85 achieves throughputs approaching 1 MIPS per MHz allowing the system designer to optimize power consumption versus processing speed.  

### M74HC595

> The M74HC595 device is a high speed CMOS 8-bit shift register with output latches (3-state) fabricated with silicon gate CMOS technology. This device contains an 8-bit serial in, parallel out shift register that feeds an 8-bit D-type storage register. The storage register has 8 3-state outputs. Separate clocks are provided for both the shift register and the storage register. The shift register has direct overriding clear, serial input, and serial output (standard) pins for cascading. Both the shift register and storage register use positive edge triggered clocks. All inputs are equipped with protection circuits against static discharge and transient excess voltage.


### HD44789U 

> The HD44780U dot-matrix liquid crystal display controller and driver LSI displays alphanumerics, Japanese kana characters, and symbols. It can be configured to drive a dot-matrix liquid crystal display under the control of a 4- or 8-bit microprocessor. Since all the functions such as display RAM, character generator, and liquid crystal driver, required for driving a dot-matrix liquid crystal display are internally provided on one chip, a minimal system can be interfaced with this controller/driver.


## Shift Register Usage

Bits are shifted sequentially, the first bit sent will be on QH after all 8 have been sent.

### 74HC595 Pin map

    MSB first
    ------------------------
    Pin | Name | Bit | Value          
    ------------------------            _____
     15 |  QA  |  0  | 1           QB -|  *  |- Vcc
      1 |  QB  |  1  | 2           QC -|     |- QA
      2 |  QC  |  2  | 4           QD -|     |- SI
      3 |  QD  |  3  | 8           QE -|     |- G
      4 |  QE  |  4  | 16          QF -|     |- RCK
      5 |  QF  |  5  | 32          QG -|     |- SCK
      6 |  QG  |  6  | 64          QH -|     |- SCLR
      7 |  QH  |  7  | 128        GND -|_____|- QH'

## ATtiny85 Usage

Three pins are need for outputs, SCK, RCK, SI.

### ATtiny85 Pins
           ____
    RST  -|*   |- VCC
    PB3  -|    |- PB2
    PB4  -|    |- PB1
    GND  -|____|- PB0


##  LCD

TODO: describe hookup & commands
