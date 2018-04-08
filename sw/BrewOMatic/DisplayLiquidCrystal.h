#ifndef _DISPLAY_LIQUID_CRYSTAL_H
#define _DISPLAY_LIQUID_CRYSTAL_H

#include "Config.h"
#include "Display.h"

#include <LiquidCrystal.h>

class DisplayLiquidCrystal: public Display {
	public:
		DisplayLiquidCrystal();
		void displayIdle(BrewOMatic *b);
		void displayBrewing(BrewOMatic *b);
		void displayMenu(BrewOMatic *b, Menu *m);

	private:
		void dispTitle(const char *str);
		LiquidCrystal lcd;
};

#endif
