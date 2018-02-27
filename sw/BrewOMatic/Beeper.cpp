#include "Beeper.h"

int Beeper::setup(BrewOMatic *b)
{
	pinMode(BEEPER_PIN, OUTPUT);

	b->addObserver(this);

	return 0;
}

void Beeper::updateStatus(BrewOMatic *b, const char *newStatus)
{

}

void Beeper::updateBrewingStep(BrewOMatic *b, const char *newStep)
{

}

void Beeper::updateTemperatureSetpoint(BrewOMatic *b, unsigned int value)
{

}

void Beeper::updateTemperature(BrewOMatic *b, unsigned int temp)
{

}

