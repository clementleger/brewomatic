#ifndef _LANGUAGE_H
#define _LANGUAGE_H

#include "BrewOMatic.h"

#define MAX_STR_LEN	30

enum brewStringIndex {
	STR_WELCOME,
	STR_FAILURE,
	STR_MAIN_MENU,
	STR_BREWING_MENU,
	STR_BACK,
	STR_IDLE,
	STR_BREWING,
	STR_STOP,
	STR_MODE,
	/* Always last member of enum */
	STR_COUNT
};

const char *getString(enum brewStringIndex strIdx);

void getStringInBuffer(enum brewStringIndex strIdx, char *buf, int len);

#endif
