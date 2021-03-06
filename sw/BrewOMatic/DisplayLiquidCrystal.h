#ifndef _DISPLAY_LIQUID_CRYSTAL_H
#define _DISPLAY_LIQUID_CRYSTAL_H

#include "Config.h"

#if ENABLED(LIQUID_CRYSTAL_DISPLAY)

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
		void enterManual(BrewOMatic *b);
		void displayManual(BrewOMatic *b);

	private:
		void createChar(byte idx, const byte progmemChar[8]);
		void dispTitle(const char *str);
		void loadAnimChar();
		void drawBool(bool status);
		void drawStatus(BrewOMatic *b, int row);
		void drawTime(unsigned long amillis);

		byte mLastMenuStart;
		LiquidCrystal mLcd;
		byte mPumpState;
		byte mHeatState;
};

#endif

#endif
