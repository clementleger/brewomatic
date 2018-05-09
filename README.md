# Disclaimer

I am not experienced with brewing so, be indulgent with what you could found in the following explanations.

# Why ?

While trying to make my own brewing system, I did not find any easy to use and well explained system for small brewing setup.
OpenArdBir code is not well architectured and does not support a full featured screen or an SD card.
I wanted a configurable system to match user needs.

#  Brew-O-Matic

Brew-O-Matic aims to be an easy to use brewing system for really (really (really)) small  all-in-one brewing setup.
It contains the following features:
* Temperature probes:
 * PT100 via MAX31865
 * DS18B20
* Input
 * Rotary Encoder
* LCD support
 * LCD2004 (Discount Controller)
 * ILI9341 (via uCglig, incomplete support)
* Pump control via MOSFET
* Heating element support
 * 50/60Hz main automatic detection
 * PID controlled heating
* Beeper
* Internationalization

Some feature can be opt out such as beeper and sd card.
Others can be choosen according to your hardware

## Design

BrewOMatic is designed to easily add a temperature sensor, input, display, etc.

## Adding a language

Languages support can easily be added. You simply have to add a Lang<YourLanguage>.h file to the lang directory.
To do that, simply recopy the LangEnglish.h file and translate the existing strings.
Be careful not to reorder entries ! (tech hint: gcc-avr does not support designated initializer)

# Building

## Configuration

In order to configure BrewOMatic to suit your configuration, you can modify Config.h
All options are clearly documented. The default configuration suit the custom brewing controller.

## Needed libraries

Depending on your configuration, you will need the following libraries:
* SD
* PIDv1
* LiquidCrystal
* TimerOne
* uCGlib
* Rotary Encoder
* TempDS18B20

## Command

Using and arduino ide, load the BrewOMatic.ino file into the IDE.
Compile it and upload it to the arduino.

# Hardware

A kicad pcb is available which contains support for 20A load control, pump and LCD 2004 support (such as for 3D printer).
The main components needed to control the brewing system are the following
* Arduino
* TRIAC for heater
* MOSFET for pump

The PCB is provided as an example configuration and I am not responsible of any damages you could have.
