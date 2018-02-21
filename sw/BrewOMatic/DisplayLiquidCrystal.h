#ifndef _DISPLAY_LIQUID_CRYSTAL_H
#define _DISPLAY_LIQUID_CRYSTAL_H

#include "Display.h"

#include <LiquidCrystal.h>

class DisplayLiquidCrystal: public Display, public BrewOMaticObserver {
	public:
		int setup();
		void update(BrewOMatic *b, enum updatedValue val);
	private:
		LiquidCrystal *lcd;
};

#endif
