#include "DisplayLiquidCrystal.h"

int DisplayLiquidCrystal::setup(BrewOMatic *b)
{
	lcd = new LiquidCrystal(LIQUID_CRYSTAL_RS,
				LIQUID_CRYSTAL_EN,
				LIQUID_CRYSTAL_D4,
				LIQUID_CRYSTAL_D5,
				LIQUID_CRYSTAL_D6,
				LIQUID_CRYSTAL_D7);
	lcd->begin(LIQUID_CRYSTAL_WIDTH, LIQUID_CRYSTAL_HEIGHT);

	b->addObserver(this);

	return 0;
}

void DisplayLiquidCrystal::updateStatus(BrewOMatic *b, const char *newStatus)
{
}

void DisplayLiquidCrystal::updateBrewingStep(BrewOMatic *b, const char *newStep)
{
}

void DisplayLiquidCrystal::updateTemperatureSetpoint(BrewOMatic *b, unsigned int value)
{
}

void DisplayLiquidCrystal::updateTemperature(BrewOMatic *b, unsigned int temp)
{
}
