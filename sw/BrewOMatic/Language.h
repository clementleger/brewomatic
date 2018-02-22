#ifndef _LANGUAGE_H
#define _LANGUAGE_H

#include "BrewOMatic.h"

#define MAX_STR_LEN	30

enum brewStringIndex {
	STR_WELCOME,
	STR_FAILURE,
};

const char *getString(enum brewStringIndex strIdx);

#endif
