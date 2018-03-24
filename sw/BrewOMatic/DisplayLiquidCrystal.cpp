#include "DisplayLiquidCrystal.h"

#include <Arduino.h>

DisplayLiquidCrystal::DisplayLiquidCrystal():
lcd(LIQUID_CRYSTAL_RS,
	LIQUID_CRYSTAL_EN,
	LIQUID_CRYSTAL_D4,
	LIQUID_CRYSTAL_D5,
	LIQUID_CRYSTAL_D6,
	LIQUID_CRYSTAL_D7)
{
	lcd->begin(LIQUID_CRYSTAL_WIDTH, LIQUID_CRYSTAL_HEIGHT);
}

void DisplayLiquidCrystal::displayIdle(BrewOMatic *b)
{
}

void DisplayLiquidCrystal::displayMenu(BrewOMatic *b, Menu *m)
{

}

void DisplayLiquidCrystal::displayBrewing(BrewOMatic *b)
{
}
