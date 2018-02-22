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
	tempProbe = new TEMP_PROBE_TYPE();
	heaterControl = new HeaterTriacControl();

}

void setup()
{
	brewOMatic.setup();
}

void loop()
{
	
}


