#ifndef _LANGUAGE_H
#define _LANGUAGE_H

#include "BrewOMatic.h"

#define MAX_STR_LEN	40

enum brewStringIndexEnum {
	STR_WELCOME,
	STR_FAILURE,
	STR_MAIN_MENU,
	STR_BREWING_MENU,
	STR_BACK,
	STR_IDLE,
	STR_BREWING,
	STR_STOP,
	STR_MODE,
	STR_ADJUST_TEMP_OFFSET,
	STR_STARTING,
	STR_START_BREWING,
	STR_TEMPERATURE,
	STR_TARGET_TEMP,
	STR_STEP,
	STR_DURATION,
	STR_WAIT_TEMP,
	STR_MISSING_MAIN,
	STR_ERROR,
	STR_READY,
	STR_TEMP_FAILED,
	STR_MANUAL_MODE,
	STR_ENABLE_PUMP,
	STR_ENABLE_HEATER,
	STR_DISABLE_PUMP,
	STR_DISABLE_HEATER,
	/* Always last member of enum */
	STR_COUNT
};

typedef char brewStringIndex;

const char *getString(brewStringIndex strIdx);

void getStringInBuffer(brewStringIndex strIdx, char *buf, int len);

#endif
