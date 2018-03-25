#include "Menu.h"
#include "Config.h"
#include "Language.h"
#include "BrewOMatic.h"

BrewOMatic brewOMatic;

void BrewOMatic::actionStopBrewing()
{
	dbgOutput("Stop brew\n");
	mState = STATE_IDLE;
	mUpdateDisplay = true;
	mCurrentMenu = NULL;
	
	delete mCurrentRecipe;
	mCurrentRecipe = NULL;
}

void BrewOMatic::actionStartBrewing()
{
	dbgOutput("Start brew\n");
	mState = STATE_BREWING;
	mUpdateDisplay = true;
	mCurrentMenu = NULL;

	mCurrentRecipe = createDefaultRecipe();
	mCurrentStep = mCurrentRecipe->mSteps.getNextElem();
}

void BrewOMatic::actionMenuBack()
{
	mCurrentMenu = mCurrentMenu->mParent;
	mUpdateDisplay = true;
}

unsigned char BrewOMatic::handleButton(Menu *onPress)
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
				return button;

			if (mCurrentMenu->mSelected > 0) {
				mCurrentMenu->mSelected--;
				mUpdateDisplay = true;
			}
		break;
		case Input::BUTTON_NEXT:
			if (mCurrentMenu == NULL)
				return button;

			if (mCurrentMenu->mSelected < (mCurrentMenu->getItemCount() - 1)) {
				mCurrentMenu->mSelected++;
				mUpdateDisplay = true;
			}
		break;
	}

	return button;
}


void BrewOMatic::handleIdle()
{
	handleButton(mIdleMenu);

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
	unsigned char b = handleButton(mBrewingMenu);

	//~ handleTemp();

	/* Start the current step */
	if (!mCurrentStep->mStarted) {
		mBeeper->beep(NOTE_B4, SEC_TO_MS(3));

		dbgOutput("Start step %s\n", mCurrentStep->mName);
		if (mCurrentStep->mEnablePump)
			digitalWrite(PUMP_CONTROL_PIN, 1);

		if (mCurrentStep->mEnableHeater)
			mHeaterControl->enable(true);

		mCurrentStep->mStarted = 1;
		mTempReached = 0;
	}

	/* Refresh display */
	if (!mCurrentMenu &&
	    ((millis() - mLastTempUpdate) > SEC_TO_MS(2))) {
		//~ mTempProbe->getTemp(&mCurrentTemp);
		mUpdateDisplay = true;
		mCurrentTemp = 22;
		mLastTempUpdate = millis();
	}

	/* Check if we reach the expected temperature */
	if (!mTempReached) {
		if (mCurrentTemp == mCurrentStep->mTargetTemp) {
			mBeeper->beep(NOTE_B4, SEC_TO_MS(3));
			mStepStartMillis = millis();
			mTempReached = true;
		}
	} else {
		/* Check if the step is done */
		if ((millis() - mStepStartMillis) >= SEC_TO_MS(mCurrentStep->mDuration * 60)) {

			dbgOutput("Stop step %s\n", mCurrentStep->mName);
			/* Stop the pump */
			digitalWrite(PUMP_CONTROL_PIN, 0);

			mCurrentStep = mCurrentRecipe->mSteps.getNextElem();
			if (!mCurrentStep) {
				actionStopBrewing();
			}
			mUpdateDisplay = true;
			mTempReached = false;
		}
	}
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
	
	pinMode(PUMP_CONTROL_PIN, OUTPUT);
	digitalWrite(PUMP_CONTROL_PIN, 0);

	dbgOutput("Setup...\n");

	mTempProbe = new TEMP_PROBE_TYPE();
	mDisp = new DISPLAY_TYPE();
	mHeaterControl = new HeaterTriacControl();
	mInput = new RotaryEncoder();
	mBeeper = new Beeper();

	//~ mTempProbe->getTemp(&mCurrentTemp);

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


