#ifndef _DISPLAY_LIQUID_CRYSTAL_H
#define _DISPLAY_LIQUID_CRYSTAL_H

#include "Display.h"
#include "BrewOMaticObserver.h"

#include <LiquidCrystal.h>

class DisplayLiquidCrystal: public Display, public BrewOMaticObserver {
	public:
		int setup(BrewOMatic *b);
		void updateStatus(BrewOMatic *b, const char *newStatus);
		void updateStep(BrewOMatic *b, const char *newStep);
		void updateTemperatureSetpoint(BrewOMatic *b, unsigned int value);
		void updateTemperature(BrewOMatic *b, unsigned int temp);

	private:
		LiquidCrystal *lcd;
};

#endif
