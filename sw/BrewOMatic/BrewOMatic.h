#ifndef _BREWOMATIC_H
#define _BREWOMATIC_H

#include "Config.h"

#define panic(_str)	do { serialOutput(_str); while(1); } while(0);

#define STRINGIFY_(M) #M
#define STRINGIFY(M) STRINGIFY_(M)

#define LANGUAGE_INCL_(M) STRINGIFY_(lang/Lang##M.h)
#define LANGUAGE_INCL(M) LANGUAGE_INCL_(M)
#define INCLUDE_LANGUAGE LANGUAGE_INCL(DISP_LANGUAGE)

#define MAX_OBSERVERS	4
#define MAX_STR_SIZE	30

class BrewOMatic;

#include "Input.h"
#include "Recipe.h"
#include "SerialOutput.h"
#include "RotaryEncoder.h"
#include "TempMax31865.h"
#include "TempDS18B20.h"
#include "HeaterTriacControl.h"
#include "BrewOMaticObserver.h"

typedef enum {
	SUCCESS = 0,
	ERROR_RECIPE_EXECUTON_FAILED,
	ERROR_USER_CANCELLED,
} brewomaticError;

class BrewOMatic {
	public:
		void setup();
		void addObserver(BrewOMaticObserver *obs);
	private:
		float mTemperatureSetpoint;
		TempProbe *tempProbe;
		HeaterControl *heaterControl;
		SerialOutput *serialOutput;
		Input *input;
		
		BrewOMaticObserver *observers[MAX_OBSERVERS];
		unsigned int nbObservers;
		char buffer[MAX_STR_SIZE];

		void notifyStatusChanged(const char *status);
		void notifyTemperatureChanged(unsigned int temp);
		brewomaticError executeRecipe(Recipe *recipe);
		brewomaticError executeStep(Step *step);
		brewomaticError executeAction(Action *action);
};


#endif
