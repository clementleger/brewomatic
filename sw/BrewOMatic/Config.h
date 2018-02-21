#ifndef _BREWOMATIC_CONFIG
#define _BREWOMATIC_CONFIG

/***********************************************************************
 *                       General Settings
 **********************************************************************/
/**
 * Languages to use:
 * - English: English language
 */
#define DISP_LANGUAGE	English

/**
 * Display type 
 */
#define DISP_TYPE	LiquidCrystal

/**
 * Liquid crystal screen configuration
 */
#define LIQUID_CRYSTAL_WIDTH	16
#define LIQUID_CRYSTAL_HEIGHT	2
#define LIQUID_CRYSTAL_RS	4
#define LIQUID_CRYSTAL_EN	5
#define LIQUID_CRYSTAL_D4	6
#define LIQUID_CRYSTAL_D5	7
#define LIQUID_CRYSTAL_D6	8
#define LIQUID_CRYSTAL_D7	9

/***********************************************************************
 *                       Display Control
 **********************************************************************/

/**
 * Set to 0 to disable serial output and save some bytes
 * Serial output will output informations periodically.
 */
#define SERIAL_OUTPUT	1

#define SERIAL_BAUDRATE 115200

/***********************************************************************
 *                       Temperature Control
 **********************************************************************/
/**
 * Temperature probe type, can be
 * TempMax31865: When using a PT100 probe
 * TempDS18B20: When using a ds18b20 sensor probe
 */
#define TEMP_PROBE_TYPE		TempMax31865

/**
 * When using TempMax31865 probe type, the hardware SPI bus is used.
 * However, the chip select can be configured here
 */
#define MAX31865_CS_PIN		10

/**
 * When using TempDS18B20 probe type, we use 1wire
 * The define which pin is used for the 1wire bus
 */
#define DS18B20_ONEWIRE_PIN	4

/***********************************************************************
 *                       Heater Control
 **********************************************************************/

/*
 * If using a system which requires crossing detection
 * Set this pin to an interruptible one
 */
#define ZERO_CROSSING_DETECT_PIN	2

/**
 * Heater cotnrol type, can be
 * TRIAC_CONTROL: When using a TRIAC to control AC.
 */
#define HEATER_CONTROL_TYPE		TRIAC_CONTROL

/*
 * When using a triac, set the pin number to the one asign to the
 * controlling pin
 */
#define TRIAC_CONTROL_PIN		4

/* Do not touch ! */
#include "ConfigSanity.h"

#endif
