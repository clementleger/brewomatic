#ifndef _SERIAL_OUTPUT_H
#define _SERIAL_OUTPUT_H

#include "BrewOMaticObserver.h"

class SerialOutput: public BrewOMaticObserver {

	public:
		int setup(BrewOMatic *b);
		/* Observer interface */
		void updateStatus(BrewOMatic *b, const char *newStatus);
		void updateTemperatureSetpoint(BrewOMatic *b, unsigned int value);
		void updateTemperature(BrewOMatic *b, unsigned int temp);
		void updateBrewingStep(BrewOMatic *b, const char *newStep);

};

/* Emergency serial output */
void serialOutput(const char *str);

#endif
