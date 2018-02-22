#include "Language.h"
#include <avr/pgmspace.h>

#define MAX_STR_LEN	30

#define STR_DEFINE(__idx, __str)	const char STR_PTR_ ##__idx[] = __str;
#include INCLUDE_LANGUAGE
#undef STR_DEFINE

#define STR_DEFINE(__idx, __str)	[STR_ ##__idx] = STR_PTR_ ##__idx,

const char *const str_array[] = {
#include INCLUDE_LANGUAGE
};

static char localBuffer[MAX_STR_LEN];

const char *getString(enum brewStringIndex strIdx)
{
	strncpy_P(localBuffer, str_array[strIdx], MAX_STR_LEN);

	return localBuffer;
}
