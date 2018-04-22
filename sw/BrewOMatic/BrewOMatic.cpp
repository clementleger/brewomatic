#include "Menu.h"
#include "Config.h"
#include "Language.h"
#include "BrewOMatic.h"
#include "ACZeroCrossing.h"

BrewOMatic brewOMatic;

void BrewOMatic::actionStopBrewing()
{
	dbgOutput("Stop brew\n");
	mState = STATE_IDLE;
	mUpdateDisplay = true;
	mCurrentMenu->mSelected = 0;
	mCurrentMenu = NULL;

	if (mCurrentStep) {
		delete mCurrentStep;
		mCurrentStep = NULL;
	}
	mDisp->enterIdle(this);
}

void BrewOMatic::actionStartBrewing()
{
	dbgOutput("Start brew\n");
	mState = STATE_BREWING;
	mUpdateDisplay = true;
	mCurrentMenu = NULL;

	mCurrentRecipe = createDefaultRecipe();
	mCurrentStep = mCurrentRecipe->mSteps.getNextElem();
	mDisp->enterBrewing(this);
}
void BrewOMatic::actionStartManual()
{
	dbgOutput("Start manual\n");
	mState = STATE_MANUAL;
	mUpdateDisplay = true;
	mCurrentMenu = NULL;

	mCurrentStep = createManualStep();
	mStepStartMillis = millis();
	mHeaterControl->setEnable(true);
	mDisp->enterManual(this);
}

void BrewOMatic::actionMenuBack()
{
	mCurrentMenu->mSelected = 0;
	mCurrentMenu = mCurrentMenu->mParent;
	mUpdateDisplay = true;
	switch (mState) {
		case STATE_IDLE:
			mDisp->enterIdle(this);
		break;
		case STATE_BREWING:
			mDisp->enterBrewing(this);
		break;
		case STATE_MANUAL:
			mDisp->enterManual(this);
		break;
	}
}

unsigned char BrewOMatic::handleButton(Menu *onPress)
{
	char button = mInput->getButtonPressed();

	if (mCurrentMenu == NULL &&
		(mState == STATE_BREWING || mState == STATE_MANUAL)) {
		if (button == Input::BUTTON_NEXT) {
			mCurrentStep->mTargetTemp++;
			mUpdateDisplay = true;
		} else if (button == Input::BUTTON_PREV) {
			mCurrentStep->mTargetTemp--;
			mUpdateDisplay = true;
		}
	}

	switch (button) {
		case Input::BUTTON_OK:
			mBeeper->click();
			/* If not in menu, start menu when Ok is pressed */
			if (!mCurrentMenu) {
				mCurrentMenu = onPress;
				mUpdateDisplay = true;
			} else {
				/* In menu, execute the menu callback */
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

	return Input::BUTTON_NONE;
}


void BrewOMatic::handleIdle()
{
	handleButton(mIdleMenu);

	if (mCurrentMenu != NULL)
		return;

	if (millis() - mLastTempUpdate > SEC_TO_MS(2)) {
		mTempProbe->getTemp(&mCurrentTemp);
		mUpdateDisplay = true;
		mLastTempUpdate = millis();
	}
}

void BrewOMatic::handleManual()
{
	unsigned char b = handleButton(mBrewingMenu);
	int ret;

	ret = mTempProbe->getTemp(&mCurrentTemp);
	if (ret) {
		/* FIXME: Abort brewing if temp read failed multiple times */
	}
	mHeaterControl->setTargetTemp(mCurrentStep->mTargetTemp);
	mHeaterControl->handleHeating(mCurrentTemp);

	/* Refresh display */
	if (((millis() - mLastTempUpdate) > SEC_TO_MS(1))) {
		mLastTempUpdate = millis();
		if (!mCurrentMenu)
			mUpdateDisplay = true;
	}
}

void BrewOMatic::handleBrewing()
{
	int ret;
	unsigned char b = handleButton(mBrewingMenu);

	/* Start the current step */
	if (!mCurrentStep->mStarted) {
		mBeeper->beep(NOTE_B4, 20);

		dbgOutput("Start step %s\n", mCurrentStep->mName);
		if (mCurrentStep->mEnablePump)
			digitalWrite(PUMP_CONTROL_PIN, 1);

		if (mCurrentStep->mEnableHeater) {
			mHeaterControl->setEnable(true);
			mHeaterControl->setTargetTemp(mCurrentStep->mTargetTemp);
		}

		mCurrentStep->mStarted = 1;
		mTempReached = 0;
	}

	ret = mTempProbe->getTemp(&mCurrentTemp);
	if (ret) {
		/* FIXME: Abort brewing if temp read failed multiple times */
	}
	mHeaterControl->handleHeating(mCurrentTemp);

	/* Refresh display */
	if (((millis() - mLastTempUpdate) > SEC_TO_MS(1))) {
		mLastTempUpdate = millis();
		if (!mCurrentMenu)
			mUpdateDisplay = true;
	}

	/* Check if we reach the expected temperature */
	if (!mTempReached) {
		if (abs(mCurrentTemp - mCurrentStep->mTargetTemp) <= 1) {
			mBeeper->beep(NOTE_B4, 20);
			mStepStartMillis = millis();
			mTempReached = true;
			dbgOutput("Step %s reached temp\n", mCurrentStep->mName);
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
		case STATE_MANUAL:
			mDisp->displayManual(this);
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
			case STATE_MANUAL:
				handleManual();
			break;
		}
		handleDisplay();
	}
}

void BrewOMatic::setup()
{
	int ret;

	mState = STATE_IDLE;
	mSerialOutput = new SerialOutput();
	mCurrentMenu = NULL;
	mLastTempUpdate = 0;
	mIdleMenu = createIdleMenu();
	mBrewingMenu = createBrewingMenu();
	
	pinMode(PUMP_CONTROL_PIN, OUTPUT);
	digitalWrite(PUMP_CONTROL_PIN, 0);

	dbgOutput("Setup...\n");

	mBeeper = new Beeper();
	mBeeper->beep(NOTE_B4, 50);
	delay(50);
	mBeeper->beep(NOTE_C4, 50);
	
	mTempProbe = new TEMP_PROBE_TYPE();
	mDisp = new DISPLAY_TYPE();
	mHeaterControl = new HeaterControl();
	mInput = new RotaryEncoder();
	mError = 0;

	ret = mTempProbe->getTemp(&mCurrentTemp);
	if (ret)
		mError = STR_TEMP_FAILED;

	if (ACZeroCrossing::Instance().getFrequency() == 0)
		mError = STR_MISSING_MAIN;
		
	delay(SEC_TO_MS(START_DELAY));

	mDisp->enterIdle(this);
	mDisp->displayIdle(this);

#if 0
	if (mError)
		while(1);
#endif

	dbgOutput("Setup OK\n");
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


