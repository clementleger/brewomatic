#include "DisplayLiquidCrystal.h"

int DisplayLiquidCrystal::setup()
{
	lcd = new LiquidCrystal(LIQUID_CRYSTAL_RS,
				LIQUID_CRYSTAL_EN,
				LIQUID_CRYSTAL_D4,
				LIQUID_CRYSTAL_D5,
				LIQUID_CRYSTAL_D6,
				LIQUID_CRYSTAL_D7);
	lcd->begin(LIQUID_CRYSTAL_WIDTH, LIQUID_CRYSTAL_HEIGHT);

	return 0;
}

void DisplayLiquidCrystal::update(BrewOMatic *b, enum updatedValue val)
{
	switch (val) {
		case STATUS:
			break;
		case TEMPERATURE_SETPOINT:

		default:
			break;
	}
}
