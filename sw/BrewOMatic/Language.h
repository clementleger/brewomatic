#ifndef _LANGUAGE_H
#define _LANGUAGE_H

#include "BrewOMatic.h"

enum brewStringIndex {
	STR_WELCOME,
	STR_FAILURE,
};

int getString(char *buffer, int length, enum brewStringIndex strIdx);

#endif
