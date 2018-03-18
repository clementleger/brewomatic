#include "Config.h"
#include "BrewOMatic.h"
#include "SerialOutput.h"

#include <Arduino.h>
#include <stdarg.h>

SerialOutput::SerialOutput()
{
	Serial.begin(SERIAL_BAUDRATE);
}

void dbgOutput(const char *fmt, ... ) {
        char buf[128]; // resulting string limited to 128 chars
        va_list args;
        va_start (args, fmt);
        vsnprintf(buf, 128, fmt, args);
        va_end (args);
        Serial.print(buf);
}
