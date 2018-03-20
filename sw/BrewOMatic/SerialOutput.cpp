#include "Config.h"
#include "BrewOMatic.h"
#include "SerialOutput.h"

#include <Arduino.h>
#include <stdarg.h>

#define DBG_MSG_MAX_SIZE	50
#define DBG_MSG_PREFIX		"DBG: "

SerialOutput::SerialOutput()
{
	Serial.begin(SERIAL_BAUDRATE);
}

#if defined(ENABLE_DEBUG) && ENABLE_DEBUG == 1
void dbgOutput(const char *fmt, ... ) {
	char buf[DBG_MSG_MAX_SIZE] = DBG_MSG_PREFIX; // resulting string limited to 128 chars
	va_list args;
	va_start (args, fmt);
	vsnprintf(buf + strlen(DBG_MSG_PREFIX), DBG_MSG_MAX_SIZE - strlen(DBG_MSG_PREFIX), fmt, args);
	va_end (args);
	Serial.print(buf);
}
#else
void dbgOutput(const char *fmt, ... ) {
}
#endif
