#ifndef _DISPLAY_LIQUID_CRYSTAL_H
#define _DISPLAY_LIQUID_CRYSTAL_H

#include "Config.h"
#include "Display.h"

#include <LiquidCrystal.h>

class DisplayLiquidCrystal: public Display {
	public:
		DisplayLiquidCrystal();

	private:
		LiquidCrystal *lcd;
};

#endif
