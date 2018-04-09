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
		void enterIdle(BrewOMatic *b);
		void enterMenu(BrewOMatic *b, Menu *m);
		void enterBrewing(BrewOMatic *b);

	private:
		void dispTitle(const char *str);
		void loadAnimChar();
		void drawStatus(bool status);
		byte mPumpState;
		byte mHeatState;
		LiquidCrystal lcd;
};

#endif
