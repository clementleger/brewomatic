#ifndef _DISPLAY_UCGLIB_H
#define _DISPLAY_UCGLIB_H

#include "Display.h"
#include "Language.h"

#include "Ucglib.h"

class DisplayUcglib: public Display {
	public:
		DisplayUcglib();
		void displayIdle(BrewOMatic *b);
		void displayMenu(BrewOMatic *b, Menu *m);
		void displayBrewing(BrewOMatic *b);
	private:
		void clearDisplayZone();
		Ucglib_ILI9341_18x240x320_HWSPI ucg;
		unsigned int lastState;
		void displayModeTemp(BrewOMatic *b, brewStringIndex mode);
};

#endif
