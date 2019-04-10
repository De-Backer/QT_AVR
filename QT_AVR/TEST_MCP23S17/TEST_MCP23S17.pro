TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

# set project info
# add build step:
# custom Process step
# command : make
# Arguments : flash

# AVR info
DEVICE     = atmega1284p
DEFINES += __AVR_ATmega1284P__
CLOCK      = 20000000
PROGRAMMER = "dragon_isp -P usb 03eb:2107 -v"
FUSES      =  -U lfuse:w:0xE7:m

NAME = main

QMAKE_CC = avr-gcc
QMAKE_CXX = avr-g++
QMAKE_LINK = avr-g++
QMAKE_CFLAGS = -Wall -Os -Iusbdrv -mmcu=$${DEVICE}  -save-temps

# Customize the -mmcu flag

QMAKE_LFLAGS = -Os -Wl,--gc-sections -mmcu=$${DEVICE} -lm

QMAKE_EXTRA_TARGETS += eep hex flash

# Add any library paths needed to build your project

INCLUDEPATH +=/usr/avr/sys-root/include/
INCLUDEPATH +=../include/

# Source files you need for your project go here.

SOURCES +=  main.c

TARGET = $${NAME}.elf

QMAKE_CLEAN += $${NAME}.elf

eep.target = $${NAME}.eep

QMAKE_CLEAN += $${NAME}.eep

eep.commands = avr-objcopy -O ihex -j .eeprom

eep.commands += --set-section-flags=.eeprom=alloc,load --no-change-warnings

eep.commands += --change-section-lma .eeprom=0 $$TARGET $$eep.target

eep.depends = $$TARGET


hex.target = $${NAME}.hex

QMAKE_CLEAN += $${NAME}.hex

hex.commands = avr-objcopy -O ihex -R .eeprom $$TARGET $$hex.target

hex.depends = $${NAME}.eep

# make flash target to build and flash

flash.target = flash

# Customize the -p flag

flash.commands = avrdude -p $${DEVICE} -c $${PROGRAMMER}

flash.commands += -U flash:w:$${NAME}.hex

flash.depends = $${NAME}.hex

HEADERS += \
    ../include/RingBuffer.h\
    ../include/debug.h \
    ../include/sw_spi.h
