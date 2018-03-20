#include "Config.h"
#include "Language.h"
#include "BrewOMatic.h"

BrewOMatic brewOMatic;



void BrewOMatic::displayIdle()
{
	dbgOutput("state: idle\n");
	mDisp->displayIdle(this);
}

void BrewOMatic::displayMenu()
{
	
}

void BrewOMatic::handleIdle()
{
	char button = mInput->getButtonPressed();
	if (button == Input::BUTTON_OK) {
		
		if (!mInMenu) {
			mBeeper->click();
			mInMenu = true;
			displayMenu();
		}
	}
}

void BrewOMatic::handleBrewing()
{
	/* Execute action */
	Recipe *recipe = createDefaultRecipe();

	executeRecipe(recipe);

	delete recipe;
}


void BrewOMatic::run()
{
	while(1) {
		/* Display menu */
		switch (mState) {
			case STATE_IDLE:
				handleIdle();
			break;
			case STATE_BREWING:
				handleBrewing();
			break;
		}
	}
}

void BrewOMatic::setup()
{
	mState = STATE_IDLE;
	mInMenu = false;
	mSerialOutput = new SerialOutput();

	dbgOutput("Starting setup...\n");

	mTempProbe = new TEMP_PROBE_TYPE();
	mDisp = new DISPLAY_TYPE();
	mHeaterControl = new HeaterTriacControl();
	mInput = new RotaryEncoder();
	mBeeper = new Beeper();

	displayIdle();
	dbgOutput("Setup OK\n");
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
	unsigned long duration = step->mDuration * 60 * 1000;

	if (!step->mUserActions.isEmpty()){
		while (step->mUserActions.hasNextElem()) {
			action = step->mUserActions.getNextElem();
			error = executeAction(action);
			if (error)
				return error;
		}
	}

	/* Update observer */
	while ((start_millis + millis()) < duration) {
		/* Control PID for temperature */
	};

	return SUCCESS;
}

brewomaticError BrewOMatic::executeRecipe(Recipe *recipe)
{
	Step *step;
	brewomaticError error;
	recipe->mSteps.reset();

	while (recipe->mSteps.hasNextElem()) {
		step = recipe->mSteps.getNextElem();

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
	dbgOutput("Entering main loop\n");
	brewOMatic.run();
}


