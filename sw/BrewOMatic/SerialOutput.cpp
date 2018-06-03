#include "Config.h"
#include "BrewOMatic.h"
#include "SerialOutput.h"

#include <Arduino.h>
#include <stdarg.h>

#define DBG_MSG_MAX_SIZE	50
#define DBG_MSG_PREFIX		"DBG: "

#if ENABLED(USE_SERIAL)
MinimumSerial outSerial;
#endif

#if ENABLED(DEBUG)
void dbgOutput(const char *fmt, ... ) {
	char buf[DBG_MSG_MAX_SIZE] = DBG_MSG_PREFIX; // resulting string limited to 128 chars
	va_list args;
	va_start (args, fmt);
	vsnprintf(buf + strlen(DBG_MSG_PREFIX), DBG_MSG_MAX_SIZE - strlen(DBG_MSG_PREFIX), fmt, args);
	va_end (args);
	outSerial.print(buf);
}
#else

void dbgOutput(const char *fmt, ... ) {
}

#endif
