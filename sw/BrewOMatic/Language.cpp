#include "Language.h"
#include <avr/pgmspace.h>

#define STR_DEFINE(__idx, __str)	const char STR_PTR_ ##__idx[] PROGMEM = __str;
#include INCLUDE_LANGUAGE
#undef STR_DEFINE

#define STR_DEFINE(__idx, __str)	STR_PTR_ ##__idx,
const char *const strArray[STR_COUNT] PROGMEM = {
#include INCLUDE_LANGUAGE
};
const char *undef = "Undefined";

static char localBuffer[MAX_STR_LEN];

void getStringInBuffer(brewStringIndex strIdx, char *buf, int len)
{
	strncpy_P(buf, (char *) pgm_read_word(&strArray[strIdx]), len);
}

const char *getString(brewStringIndex strIdx)
{
	getStringInBuffer(strIdx, localBuffer, MAX_STR_LEN);

	return localBuffer;
}
