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

brewomaticError BrewOMatic::executeAction(Action *action)
{
	/* Wait confirmation from user */

	return SUCCESS;
}

brewomaticError BrewOMatic::executeStep(Step *step)
{
	Action *action;
	brewomaticError error;
	unsigned long start_millis = millis();
	unsigned long duration = step->getDuration() * 60 * 1000;

	if (!step->getUserActions().isEmpty()){
		
		while (step->getUserActions().hasNextElem()) {
			action = step->getUserActions().getNextElem();
			error = executeAction(action);
			if (error)
				return error;
		}
	}

	/* update observer */

	while ((start_millis + millis()) < duration) {
		/* Control PID for temperature */
	};

	return SUCCESS;
}

brewomaticError BrewOMatic::executeRecipe(Recipe *recipe)
{
	Step *step;
	brewomaticError error;
	recipe->getSteps().reset();

	while (recipe->getSteps().hasNextElem()) {
		step = recipe->getSteps().getNextElem();

		error = executeStep(step);
		if (error)
			return error;
	}

	return SUCCESS;
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


