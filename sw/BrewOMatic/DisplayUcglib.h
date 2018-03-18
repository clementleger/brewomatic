#ifndef _DISPLAY_UCGLIB_H
#define _DISPLAY_UCGLIB_H

#include "Display.h"

#include "Ucglib.h"

class DisplayUcglib: public Display {
	public:
		DisplayUcglib();
		void displayIdle(BrewOMatic *b);
	private:
	
		Ucglib_ILI9341_18x240x320_HWSPI ucg;
};

#endif
