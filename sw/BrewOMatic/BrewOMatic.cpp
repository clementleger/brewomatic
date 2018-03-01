#include "Config.h"
#include "Language.h"
#include "BrewOMatic.h"

BrewOMatic brewOMatic;

void BrewOMatic::addObserver(BrewOMaticObserver *obs)
{
	if (nbObservers > MAX_OBSERVERS)
		return;

	observers[nbObservers] = obs;
	nbObservers++;
}

void BrewOMatic::notifyStatusChanged(const char *status)
{
	for(unsigned int i = 0; i < nbObservers; i++)
		observers[nbObservers]->updateStatus(this, status);
}

void BrewOMatic::setup()
{
	notifyStatusChanged(getString(STR_WELCOME));
	
	nbObservers = 0;
	serialOutput = new SerialOutput();
	tempProbe = new TEMP_PROBE_TYPE();
	heaterControl = new HeaterTriacControl();
	input = new RotaryEncoder();

	serialOutput->setup(this);
	tempProbe->setup();
	heaterControl->setup();
	input->setup();
}

void BrewOMatic::executeStep(BrewingStep *step)
{
	unsigned long start_millis = millis();
	unsigned long duration = step->getDuration() * 60 * 1000;

	/* pdate observer */

	while ((start_millis + millis()) < duration) {
		
	};
}

void BrewOMatic::executeRecipe(Recipe *recipe)
{
	BrewingStep *step;
	while (recipe->hasNextBrewingStep()) {
		step = getNextBrewingStep();

		

	}
}

/**
 * Basic C stuff
 */
void setup()
{
	brewOMatic.setup();
}

void loop()
{
	
}


