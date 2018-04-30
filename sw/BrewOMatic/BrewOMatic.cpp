
#include "Language.h"
#include "Menu.h"
#include "Config.h"
#include "BrewOMatic.h"
#include "ACZeroCrossing.h"

BrewOMatic brewOMatic;

BrewOMatic::BrewOMatic()
{
	mStepStarted = false;
	mState = STATE_IDLE;
	mSerialOutput = new SerialOutput();
	mCurrentMenu = NULL;
	mCurrentStep = NULL;
	mLastDispUpdate = 0;
	mIdleMenu = createIdleMenu();
	mBrewingMenu = createBrewingMenu();
}

void BrewOMatic::changeState(int state)
{
	dbgOutput("change state %d\n", state);
	mState = state;
	mUpdateDisplay = true;
	mCurrentMenu->mSelected = 0;
	mCurrentMenu = NULL;
}

int BrewOMatic::actionEnablePump()
{
	mCurrentStep->mEnablePump = !mCurrentStep->mEnablePump;
	digitalWrite(PUMP_CONTROL_PIN, mCurrentStep->mEnablePump);
	mUpdateDisplay = true;

	return mCurrentStep->mEnablePump;
}

int BrewOMatic::actionEnableHeater()
{
	mCurrentStep->mEnableHeater = !mCurrentStep->mEnableHeater;
	if (mCurrentStep->mEnableHeater) {
		mHeaterControl->setTargetTemp(mCurrentStep->mTargetTemp);
		mHeaterControl->setEnable(true);
	} else {
		mHeaterControl->setEnable(false);
	}
	mUpdateDisplay = true;

	return mCurrentStep->mEnableHeater;
}

void BrewOMatic::actionStopBrewing()
{
	changeState(STATE_IDLE);

	if (mCurrentStep) {
		delete mCurrentStep;
		mCurrentStep = NULL;
	}
	mHeaterControl->setEnable(false);
	digitalWrite(PUMP_CONTROL_PIN, 0);

	mDisp->enterIdle(this);
}

void BrewOMatic::actionStartBrewing()
{
	changeState(STATE_BREWING);

	mCurrentRecipe = createDefaultRecipe();
	mCurrentStep = mCurrentRecipe->mSteps.getNextElem();
	mDisp->enterBrewing(this);
}
void BrewOMatic::actionStartManual()
{
	changeState(STATE_MANUAL);

	mCurrentStep = createManualStep();
	mStepStartMillis = millis();

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

uint8_t BrewOMatic::handleButton(Menu *onPress)
{
	uint8_t button = mInput->getButtonPressed();

	if (mCurrentMenu == NULL && mState == STATE_MANUAL) {
		if (button == Input::BUTTON_NEXT) {
			mTargetTemp++;
			mUpdateDisplay = true;
		} else if (button == Input::BUTTON_PREV) {
			mTargetTemp--;
			mUpdateDisplay = true;
		}
	}

	switch (button) {
		case Input::BUTTON_OK:
			mBeeper->click();
			/* If not in menu, start menu when Ok is pressed */
			if (!mCurrentMenu) {
				mCurrentMenu = onPress;
				mDisp->enterMenu(this, mCurrentMenu);
				mUpdateDisplay = true;
			} else {
				/* In menu, execute the menu callback */
				menuItemCallback mCb = mCurrentMenu->getSelectedItem()->mCallback;
				if (mCb)
					mCb(mCurrentMenu->getSelectedItem(), this);
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

	if (millis() - mLastDispUpdate > SEC_TO_MS(2)) {
		mTempProbe->getTemp(&mCurrentTemp);
		mUpdateDisplay = true;
		mLastDispUpdate = millis();
	}
}

void BrewOMatic::checkDisplay()
{
	/* Refresh display */
	if (((millis() - mLastDispUpdate) > SEC_TO_MS(1))) {
		mLastDispUpdate = millis();
		if (!mCurrentMenu)
			mUpdateDisplay = true;
	}
}

void BrewOMatic::checkTemp()
{
	int ret;

	if ((millis() - mTempUpdate) > TEMP_SAMPLE_TIME_MS) {
		mTempUpdate = millis();
		ret = mTempProbe->getTemp(&mCurrentTemp);
		if (ret) {
			/* FIXME: Abort brewing if temp read failed multiple times */
		}
		mHeaterControl->handleHeating(mCurrentTemp);
	}	
}

void BrewOMatic::handleManual()
{
	unsigned char b = handleButton(mBrewingMenu);

	mHeaterControl->setTargetTemp(mTargetTemp);

	checkTemp();
	checkDisplay();
}

void BrewOMatic::setTargetTemp(unsigned int targetTemp)
{
	mTargetTemp = targetTemp;
	mHeaterControl->setTargetTemp(mTargetTemp);
}

void BrewOMatic::handleBrewing()
{
	unsigned char b = handleButton(mBrewingMenu);

	/* Start the current step */
	if (!mStepStarted) {
		mBeeper->beep(NOTE_B4, 20);

		dbgOutput("Start step %s\n", mCurrentStep->mName);
		if (mCurrentStep->mEnablePump)
			digitalWrite(PUMP_CONTROL_PIN, 1);

		if (mCurrentStep->mEnableHeater) {
			mHeaterControl->setEnable(true);
			setTargetTemp(mCurrentStep->mTargetTemp);
		}

		mStepStarted = true;
		mTempReached = false;
	}

	checkTemp();
	checkDisplay();

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

			mStepStarted = false;
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
	
	pinMode(PUMP_CONTROL_PIN, OUTPUT);
	digitalWrite(PUMP_CONTROL_PIN, 0);

	dbgOutput("Setup...\n");

	mBeeper = new Beeper();
	mBeeper->beep(NOTE_B4, 50);
	delay(50);
	mBeeper->beep(NOTE_C4, 50);
	
	mTempProbe = new TEMP_PROBE_TYPE();
	mDisp = new DISPLAY_TYPE();
	mHeaterControl = new HeaterControl(TEMP_SAMPLE_TIME_MS);
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

	if (mError && !ENABLE_DEBUG)
		while(1);

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


