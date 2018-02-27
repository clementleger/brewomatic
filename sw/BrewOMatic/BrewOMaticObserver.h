#ifndef _BREWOMATIC_OBSERVER_H
#define _BREWOMATIC_OBSERVER_H

class BrewOMatic;

class BrewOMaticObserver {
	public:
		virtual void updateStatus(BrewOMatic *b, const char *newStatus) = 0;
		virtual void updateTemperatureSetpoint(BrewOMatic *b, unsigned int value) = 0;
		virtual void updateTemperature(BrewOMatic *b, unsigned int value) = 0;
		virtual void updateBrewingStep(BrewOMatic *b, const char *newStep) = 0;

};

#endif
