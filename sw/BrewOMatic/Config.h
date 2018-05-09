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
 * Enable beeper support
 */
#define USE_BEEPER	1

/**
 * Pin where the beeper is connected (must be a PWM one)
 */
#define BEEPER_PIN	6

/**
 * Duration of beeper click sound
 */
#define BEEPER_CLICK_DURATION	5

/**
 * Enable debug over UART
 */
#define DEBUG	1

/**
 * Delay before starting in seconds
 */
#define START_DELAY	0

/**
 * Banner displyed at start
 */
#define START_BANNER	"It's Brew'o'Clock !"


/***********************************************************************
 *                       SD Card
 **********************************************************************/

#define USE_SD_CARD	1

/**
 * SD Card chip select
 */
#define SD_CARD_CHIP_SELECT	10

/***********************************************************************
 *                       Inputs
 **********************************************************************/

/**
 * Enable rotary encoder support
 */
#define ENABLE_ROTARY_ENCODER	1

/**
 * Pin where the beeper is connected
 */
#define ROTARY_ENCODER_PIN_A		3
#define ROTARY_ENCODER_PIN_B		4
#define ROTARY_ENCODER_SWITCH_PIN	A3

/**
 * Reverse encoder rotation direction
 */
#define ROTARY_ENCODER_DIR_REVERSE	1

/***********************************************************************
 *                       Display Control
 **********************************************************************/

/**
 * Display type
 * DisplayUcglib: for ILI9341
 */
#define DISPLAY_TYPE	DisplayLiquidCrystal

/**
 * Liquid crystal screen configuration
 */
#define LIQUID_CRYSTAL_WIDTH	20
#define LIQUID_CRYSTAL_HEIGHT	4

#define LIQUID_CRYSTAL_RS	A4
#define LIQUID_CRYSTAL_EN	A2
#define LIQUID_CRYSTAL_D4	A1
#define LIQUID_CRYSTAL_D5	A5
#define LIQUID_CRYSTAL_D6	A0
#define LIQUID_CRYSTAL_D7	9

/**
 * Ucglib screen configuration
 */
#define UCGLIB_DISPLAY_TYPE	Ucglib_ILI9341_18x240x320_HWSPI

#define UCGLIB_CD_PIN	A1	
#define UCGLIB_CS_PIN	A2
#define UCGLIB_RST_PIN	A3

/**
 * Set to 0 to disable serial output and save some bytes
 * Serial output will output informations periodically.
 */
#define SERIAL_OUTPUT	1

/**
 * Serial baudrate to use
 */
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
#define MAX31865_CS_PIN		8

/**
 * Number of wire your PT100 probe is using
 * This define can be:
 * - MAX31865_2WIRE: Use 2 wires
 * - MAX31865_3WIRE: Use 3 wires
 * - MAX31865_4WIRE: Use 4 wires
 */
#define MAX31865_WIRE_COUNT	MAX31865_3WIRE

/**
 * When using TempDS18B20 probe type, we use 1wire
 * The define which pin is used for the 1wire bus
 */
#define DS18B20_ONEWIRE_PIN	4

/**
 * PID values
 * We are talking about blackmagic here !
 */

#define PID_P_VALUE	2
#define PID_I_VALUE	5
#define PID_D_VALUE	1

/**
 * Temperature and heating control delay
 */
#define TEMP_SAMPLE_TIME_MS	100

/***********************************************************************
 *                       Heater Control
 **********************************************************************/

/**
 * If using a system which requires crossing detection
 * Set this pin to an interruptible one
 */
#define ZERO_CROSSING_DETECT_PIN	2

/**
 * Heater control type, can be
 * TRIAC_CONTROL: When using a TRIAC to control AC.
 */
#define HEATER_CONTROL_TYPE		TRIAC_CONTROL

/**
 * When using a triac, set the pin number to the one asign to the
 * controlling pin
 */
#define TRIAC_CONTROL_PIN		7

/***********************************************************************
 *                       Pump Control
 **********************************************************************/
/**
 * When using TempMax31865 probe type, the hardware SPI bus is used.
 * However, the chip select can be configured here
 */
#define PUMP_CONTROL_PIN	5

/* Do not touch ! */
#include "ConfigSanity.h"

#endif
