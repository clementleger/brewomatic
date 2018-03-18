#ifndef _BREWOMATIC_H
#define _BREWOMATIC_H

#include "Config.h"

#define panic(_str)	do { mSerialOutput(_str); while(1); } while(0);

#define STRINGIFY_(M) #M
#define STRINGIFY(M) STRINGIFY_(M)

#define LANGUAGE_INCL_(M) STRINGIFY_(lang/Lang##M.h)
#define LANGUAGE_INCL(M) LANGUAGE_INCL_(M)
#define INCLUDE_LANGUAGE LANGUAGE_INCL(DISP_LANGUAGE)

#define MAX_OBSERVERS	4
#define MAX_STR_SIZE	30

#define BREWOMATIC_VERSION_STRING "BrewOMatic V0.1"

class BrewOMatic;

#include "Input.h"
#include "Beeper.h"
#include "Recipe.h"
#include "IdleInfo.h"
#include "TempDS18B20.h"
#include "TempMax31865.h"
#include "SerialOutput.h"
#include "DisplayUcglib.h"
#include "RotaryEncoder.h"
#include "HeaterTriacControl.h"

typedef enum {
	SUCCESS = 0,
	ERROR_RECIPE_EXECUTON_FAILED,
	ERROR_USER_CANCELLED,
} brewomaticError;

typedef enum {
	STATE_IDLE,
	STATE_BREWING,
} brewomaticState;

class BrewOMatic {
	public:
		void setup();
		void run();
	private:
		TempProbe *mTempProbe;
		Display *mDisp;
		HeaterControl *mHeaterControl;
		SerialOutput *mSerialOutput;
		Input *mInput;
		Beeper *mBeeper;

		brewomaticState mState;
		bool mInMenu;

		void handleIdle();
		void handleBrewing();
		void handleMenu();

		void displayIdle();
		void displayMenu();
		brewomaticError executeRecipe(Recipe *recipe);
		brewomaticError executeStep(Step *step);
		brewomaticError executeAction(Action *action);
};

#endif
