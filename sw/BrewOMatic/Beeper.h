#ifndef _BEEPER_H
#define _BEEPER_H

#include "BrewOMatic.h"

class Beeper: public BrewOMaticObserver {
	public:
		int setup(BrewOMatic *b);
		void updateStatus(BrewOMatic *b, const char *newStatus);
		void updateBrewingStep(BrewOMatic *b, const char *newStep);
		void updateTemperatureSetpoint(BrewOMatic *b, unsigned int value);
		void updateTemperature(BrewOMatic *b, unsigned int temp);

};

#endif
