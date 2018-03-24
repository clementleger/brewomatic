#include "Menu.h"
#include "Config.h"
#include "Language.h"
#include "BrewOMatic.h"

BrewOMatic brewOMatic;

void BrewOMatic::actionStopBrewing()
{
	mState = STATE_IDLE;
	mCurrentMenu = NULL;
	mUpdateDisplay = true;
}

void BrewOMatic::actionStartBrewing()
{
	mState = STATE_BREWING;
	mUpdateDisplay = true;
	mCurrentMenu = NULL;
}

void BrewOMatic::actionMenuBack()
{
	mCurrentMenu = mCurrentMenu->mParent;
	mUpdateDisplay = true;
}

int BrewOMatic::handleButton(Menu *onPress)
{
	char button = mInput->getButtonPressed();

	switch (button) {
		case Input::BUTTON_OK:
			mBeeper->click();
			if (!mCurrentMenu) {
				mCurrentMenu = onPress;
				mUpdateDisplay = true;
			} else {
				menuItemCallback mCb = mCurrentMenu->getSelectedItem()->mCallback;
				if (mCb)
					mCb(this);
			}
		break;
		case Input::BUTTON_PREV:
			if (mCurrentMenu == NULL)
				return 1;

			if (mCurrentMenu->mSelected > 0) {
				mCurrentMenu->mSelected--;
				mUpdateDisplay = true;
			}
		break;
		case Input::BUTTON_NEXT:
			if (mCurrentMenu == NULL)
				return 1;

			if (mCurrentMenu->mSelected < (mCurrentMenu->getItemCount() - 1)) {
				mCurrentMenu->mSelected++;
				mUpdateDisplay = true;
			}
		break;
	}
	return 0;
}


void BrewOMatic::handleIdle()
{
	if(handleButton(mIdleMenu))
		return;
	
	if (mCurrentMenu != NULL)
		return;

	if (millis() - mLastTempUpdate > SEC_TO_MS(2)) {
		//~ mTempProbe->getTemp(&mCurrentTemp);
		mUpdateDisplay = true;
		mCurrentTemp = 20;
		mLastTempUpdate = millis();
	}
}

void BrewOMatic::handleBrewing()
{
	if(handleButton(mBrewingMenu))
		return;

	/* Execute action */
	//~ Recipe *recipe = createDefaultRecipe();

	//~ executeRecipe(recipe);

	//~ delete recipe;
}

void BrewOMatic::handleDisplay()
{	
	if (!mUpdateDisplay)
		return;

	if (mCurrentMenu) {
		mDisp->displayMenu(this, mCurrentMenu);
		mUpdateDisplay = false;
		return;
	}

	switch (mState) {
		case STATE_IDLE:
			mDisp->displayIdle(this);
		break;
		case STATE_BREWING:
			mDisp->displayBrewing(this);
		break;
	}

	mUpdateDisplay = false;
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
		handleDisplay();
	}
}

void BrewOMatic::setup()
{
	mState = STATE_IDLE;
	mSerialOutput = new SerialOutput();
	mCurrentMenu = NULL;
	mLastTempUpdate = 0;
	mIdleMenu = createIdleMenu();
	mBrewingMenu = createBrewingMenu();

	dbgOutput("Starting setup...\n");

	mTempProbe = new TEMP_PROBE_TYPE();
	mDisp = new DISPLAY_TYPE();
	mHeaterControl = new HeaterTriacControl();
	mInput = new RotaryEncoder();
	mBeeper = new Beeper();

	mDisp->displayIdle(this);
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


