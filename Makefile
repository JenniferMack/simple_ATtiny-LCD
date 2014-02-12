# Name: Makefile
# Author: Jenny Mack
# Copyright: Jenny Mack, 2014
# License: GNU GPL

PROJECT    = t85-lcd

# DEVICE ....... The AVR device you compile for
# CLOCK ........ Target AVR clock rate in Hertz
# PGM_DEVICE ... The AVR device you compile for (avrdude name)
# PGM_FLAGS  ... Options to avrdude which define the hardware you use for
#                uploading to the AVR and the interface where this hardware
#                is connected.
# FUSES ........ Parameters for avrdude to flash the fuses appropriately.

DEVICE     = attiny85
#DEVICE     = attiny84
#DEVICE     = atmega328p

# 1 MHz
CLOCK      = 1000000
# 8 MHz
#CLOCK      = 8000000
# 16 MHz
#CLOCK      = 16000000
# 20 MHz
#CLOCK      = 20000000

PGM_DEVICE = t85
#PGM_DEVICE = t84
#PGM_DEVICE = m328p

# Option -B3 needed for MCUs running at 1MHz, when using USBasp
# and JP3 is open.
PGM_FLAGS  = -q -P /dev/usbasp -c usbasp -B3

## Default for ATTiny85 (internal 1MHz clock)
#FUSES      = -U lfuse:w:0x62:m -U hfuse:w:0xDF:m -U efuse:w:0xFF:m
## ATTiny85 (internal 8MHz clock)
#FUSES      = -U lfuse:w:0xE2:m -U hfuse:w:0xDF:m -U efuse:w:0xFF:m

## Default for ATTiny84 (internal 1MHz clock)
#FUSES      = -U lfuse:w:0x62:m -U hfuse:w:0xDF:m -U efuse:w:0xFF:m
## ATTiny84 (internal 8MHz clock)
#FUSES      = -U lfuse:w:0xE2:m -U hfuse:w:0xDF:m -U efuse:w:0xFF:m

## Default for ATMega328P (internal 1MHz clock, 2048 word boot flash)
#FUSES      = -U lfuse:w:0x62:m -U hfuse:w:0xD9:m -U efuse:w:0xFF:m
## ATMega328P (internal 8MHz clock, 2048 word boot flash)
#FUSES      = -U lfuse:w:0xE2:m -U hfuse:w:0xDF:m -U efuse:w:0xFF:m

# Usage:
# make          : compiles program
# make size     : computes size of program
# make flash    : compiles and uploads
# make fuses    : sets fuses on target MCU
# make clean    : cleans project directory
# make wipe     : erases MCU
# make reset    : cleans project directory & erases MCU

######################################################################
CC         = avr-gcc
AVRDUDE    = avrdude
RM         = rm -f
OBJECTS    = $(patsubst %.c,%.o, $(wildcard *.c))
CFLAGS     = -std=c99 -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) -fno-exceptions
######################################################################

all: $(PROJECT).hex size

$(PROJECT).hex: $(PROJECT).elf
	$(RM) $(PROJECT).hex
	-avr-objcopy -j .text -j .data -O ihex $(PROJECT).elf $(PROJECT).hex

$(PROJECT).elf: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(PROJECT).elf

flash: all
	$(AVRDUDE) $(PGM_FLAGS) -p $(PGM_DEVICE) -U flash:w:$(PROJECT).hex:i

size: $(PROJECT).elf
	-avr-size --mcu=$(DEVICE) -C $(PROJECT).elf

fuses:
	$(AVRDUDE) $(PGM_FLAGS) -p $(PGM_DEVICE) $(FUSES)

clean:
	$(RM) $(PROJECT).hex $(PROJECT).elf
	$(RM) $(OBJECTS)

wipe:
	$(AVRDUDE) -e

reset:
	clean wipe
