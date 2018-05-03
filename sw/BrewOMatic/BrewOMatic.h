#ifndef _BREWOMATIC_H
#define _BREWOMATIC_H

#include "Config.h"

#define panic(_str)	do { mSerialOutput(_str); while(1); } while(0);

#define MAX_STR_SIZE	30

#define BREWOMATIC_VERSION_STRING "BrewOMatic V0.1"

#define SEC_TO_MS(_sec)	(_sec * 1000)

class BrewOMatic;

#include "Input.h"
#include "Beeper.h"
#include "Recipe.h"
#include "Language.h"
#include "IdleInfo.h"
#include "SDHandling.h"
#include "TempDS18B20.h"
#include "TempMax31865.h"
#include "SerialOutput.h"
#include "DisplayUcglib.h"
#include "RotaryEncoder.h"
#include "HeaterControl.h"
#include "DisplayLiquidCrystal.h"

typedef enum {
	SUCCESS = 0,
	ERROR_RECIPE_EXECUTON_FAILED,
	ERROR_USER_CANCELLED,
} brewomaticError;

typedef enum {
	STATE_IDLE,
	STATE_BREWING,
	STATE_MANUAL,
} brewomaticState;

typedef enum {
	BREWING_GET_NEXT_STEP,
	BREWING_PRE_STEP_ACTION,
	BREWING_START_STEP,
	BREWING_WAIT_TEMP_REACHED,
	BREWING_WAIT_END_OF_STEP,
	BREWING_END,
} brewingState;

class BrewOMatic {
	public:
		void setup();
		void run();
		BrewOMatic();

		/* Callbacks */
		void actionStopBrewing();
		void actionStartBrewing();
		void actionStartManual();
		int actionEnablePump();
		int actionEnableHeater();
		void actionMenuBack();
		float mCurrentTemp;

		Recipe *mCurrentRecipe;
		Step *mCurrentStep;
		Action *mCurrentAction;

		HeaterControl *mHeaterControl;
		unsigned long mStepStartMillis;
		brewStringIndex mStatus;
		bool mError;
	private:
		brewomaticState mState;

		TempProbe *mTempProbe;
		Display *mDisp;
		Input *mInput;
		Beeper *mBeeper;

		unsigned long mLastDispUpdate;
		unsigned long mTempUpdate;
		unsigned long mLastBeepTime;
		unsigned int mTargetTemp;
		void setTargetTemp(unsigned int);
		void setError(brewStringIndex err);

		brewingState mBrewingState;
		Menu *mCurrentMenu;
		Menu *mIdleMenu;
		Menu *mBrewingMenu;
		bool mUpdateDisplay;

		void getNextStep();
		void startStep();
		void waitEndOfStep();
		void waitTempReached();
		void waitUserAction();
		void brewingEnd();

		void updateTemp();
		void updateDisplay();

		void changeState(int state);
		void handleIdle();
		void handleBrewing();
		void handleMenu();
		void handleManual();

		void handleDisplay();
		void displayMenu();
		unsigned char handleButton(Menu *onPress);
};

#endif
