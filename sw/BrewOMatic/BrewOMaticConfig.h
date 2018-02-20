#ifndef _BREWOMATIC_CONFIG
#define _BREWOMATIC_CONFIG

/***********************************************************************
 *                       Temperature Control
 **********************************************************************/
/**
 * Temperature probe type, can be
 * TempMax31865: When using a PT100 probe
 * TempDS18B20: When using a ds18b20 sensor probe
 */
#define TEMP_PROBE_TYPE	TempMax31865

/**
 * When using TempMax31865 probe type, the hardware SPI bus is used.
 * Hopwever, the chip select can be configured here
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
#include "BrewOMaticConfigSanity.h"

#endif
