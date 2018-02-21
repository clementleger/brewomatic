#include "Language.h"
#include <avr/pgmspace.h>

#define STR_DEFINE(__idx, __str)	const char STR_PTR_ ##__idx[] = __str;
#include INCLUDE_LANGUAGE
#undef STR_DEFINE

#define STR_DEFINE(__idx, __str)	[STR_ ##__idx] = STR_PTR_ ##__idx,

const char *const str_array[] = {
#include INCLUDE_LANGUAGE
};

int getString(char *buffer, int length, enum brewStringIndex strIdx)
{
	strncpy_P(buffer, str_array[strIdx], length);
	return 0;
}
