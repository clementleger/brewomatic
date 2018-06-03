#ifndef _SERIAL_OUTPUT_H
#define _SERIAL_OUTPUT_H

#include "MinimumSerial.h"

extern MinimumSerial outSerial;

void dbgOutput(const char *fmt, ...);

#endif
