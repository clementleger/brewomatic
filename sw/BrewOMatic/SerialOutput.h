#ifndef _SERIAL_OUTPUT_H
#define _SERIAL_OUTPUT_H

class BrewOMatic;

class SerialOutput {
	public:
		SerialOutput();
};

/* Emergency serial output */
void dbgOutput(const char *fmt, ...);

#endif
