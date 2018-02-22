#ifndef _BREWOMATIC_H
#define _BREWOMATIC_H

#include "Config.h"

#define STRINGIFY_(M) #M
#define STRINGIFY(M) STRINGIFY_(M)

#define LANGUAGE_INCL_(M) STRINGIFY_(lang/Lang##M.h)
#define LANGUAGE_INCL(M) LANGUAGE_INCL_(M)
#define INCLUDE_LANGUAGE LANGUAGE_INCL(DISP_LANGUAGE)

#define MAX_OBSERVERS	4
#define MAX_STR_SIZE	30

#include "TempMax31865.h"
#include "TempDS18B20.h"
#include "HeaterTriacControl.h"

class BrewOMaticObserver;

class BrewOMatic {
	public:
		void setup();
		void addObserver(BrewOMaticObserver *obs);
	private:
		float mTemperatureSetpoint;
		TempProbe *tempProbe;
		HeaterControl *heaterControl;
		BrewOMaticObserver *observers[MAX_OBSERVERS];
		unsigned int nbObservers;
		char buffer[MAX_STR_SIZE];

		void notifyStatusChanged(const char *status);
		void notifyTemperatureChanged(unsigned int temp);
		
};

class BrewOMaticObserver {
	public:
		virtual void updateStatus(BrewOMatic *b, const char *newStatus) = 0;
		virtual void updateTemperatureSetpoint(BrewOMatic *b, unsigned int value) = 0;
		virtual void updateTemperature(BrewOMatic *b, unsigned int value) = 0;
		virtual void updateBrewingStep(BrewOMatic *b, const char *newStep) = 0;

};


#endif
