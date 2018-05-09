
#include "Language.h"
#include "Menu.h"
#include "Config.h"
#include "BrewOMatic.h"
#include "ACZeroCrossing.h"
#include "SDHandling.h"

BrewOMatic brewOMatic;

BrewOMatic::BrewOMatic()
{
	mState = STATE_IDLE;
	mCurrentMenu = NULL;
	mCurrentStep = NULL;
	mLastDispUpdate = 0;
	mTargetTemp = 0;
	mError = false;
}

void BrewOMatic::setCurrentMenu(Menu *m)
{
	mCurrentMenu = m;
	mUpdateDisplay = true;
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
	digitalWrite(PUMP_CONTROL_PIN, LOW);

	mDisp->enterIdle(this);
}

void BrewOMatic::actionStartDefaultRecipe()
{
	changeState(STATE_BREWING);

	mStatus = STR_STARTING;

	mCurrentRecipe = createDefaultRecipe();
	mDisp->enterBrewing(this);
	getNextStep();
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
	if (!mCurrentMenu)
		return;

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

/**
 * TODO: Find a way to factorize this code and let each mode
 * handle it's own buttons
 */
uint8_t BrewOMatic::handleButton(Menu *onPress)
{
	uint8_t button = mInput->getButtonPressed();

	/* Ugly non-generic case: we wait for user input and do not want
	 * any generic handling */
	if (mState == STATE_BREWING && mBrewingState == BREWING_PRE_STEP_ACTION)
		return button;

	switch (button) {
		case Input::BUTTON_OK:
			beeperClick();

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
	unsigned long curMillis = millis();

	if (mCurrentMenu != NULL)
		return;

	if (curMillis - mLastDispUpdate > SEC_TO_MS(2)) {
		mTempProbe->getTemp(&mCurrentTemp);
		mUpdateDisplay = true;
		mLastDispUpdate = curMillis;
	}
}

void BrewOMatic::updateDisplay()
{
	unsigned long curMillis = millis();

	/* Refresh display */
	if (((curMillis - mLastDispUpdate) > SEC_TO_MS(1))) {
		mLastDispUpdate = curMillis;
		if (!mCurrentMenu)
			mUpdateDisplay = true;
	}
}

void BrewOMatic::updateTemp()
{
	int ret;
	unsigned long curMillis = millis();

	if ((curMillis - mTempUpdate) > TEMP_SAMPLE_TIME_MS) {
		mTempUpdate = curMillis;
		ret = mTempProbe->getTemp(&mCurrentTemp);
		if (ret) {
			/* FIXME: Abort brewing if temp read failed multiple times */
		}
	}

	mHeaterControl->handleHeating(mCurrentTemp);
}

void BrewOMatic::handleManual()
{
	unsigned char b = handleButton(mBrewingMenu);

	if (b != Input::BUTTON_NONE) {
		if (b == Input::BUTTON_NEXT) {
			setTargetTemp(mTargetTemp + 1);
			mUpdateDisplay = true;
		} else if (b == Input::BUTTON_PREV) {
			if (mTargetTemp > 0) {
				setTargetTemp(mTargetTemp - 1);
				mUpdateDisplay = true;
			}
		}
	}

	updateTemp();
	updateDisplay();
}

void BrewOMatic::setTargetTemp(unsigned int targetTemp)
{
	mTargetTemp = targetTemp;
	mHeaterControl->setTargetTemp(mTargetTemp);
}

void BrewOMatic::startStep()
{
	beeperBeep(NOTE_B4, 20);

	dbgOutput("Start step %s\n", mCurrentStep->mName);
	if (mCurrentStep->mEnablePump)
		digitalWrite(PUMP_CONTROL_PIN, HIGH);

	if (mCurrentStep->mEnableHeater) {
		mHeaterControl->setEnable(true);
		setTargetTemp(mCurrentStep->mTargetTemp);
	}

	mStatus = STR_WAIT_TEMP;
	mBrewingState = BREWING_WAIT_TEMP_REACHED;

}

void BrewOMatic::waitEndOfStep()
{
	/* Check if the step is done */
	if (!((millis() - mStepStartMillis) >=
	    SEC_TO_MS(mCurrentStep->mDuration * 60)))
		return;

	dbgOutput("Stop step %s\n", mCurrentStep->mName);
	/* Stop the pump and heater */
	digitalWrite(PUMP_CONTROL_PIN, LOW);
	mHeaterControl->setEnable(false);

	mStatus = STR_BREWING;
	mBrewingState = BREWING_GET_NEXT_STEP;
}

void BrewOMatic::getNextStep()
{
	mUpdateDisplay = true;
	mCurrentStep = mCurrentRecipe->mSteps.getNextElem();
	if (!mCurrentStep) {
		/* Stop the pump and heater */
		digitalWrite(PUMP_CONTROL_PIN, LOW);
		mHeaterControl->setEnable(false);
		mBrewingState = BREWING_END;
		return;
	}

	mStepStartMillis = 0;
	if (mCurrentStep->mPreStepAction) {
		mCurrentAction = mCurrentStep->mPreStepAction;
		/* We are requesting user attention here !
		 * quit menu if any */
		actionMenuBack();
		mCurrentMenu = NULL;
		mStatus = STR_PRESS_OK;
		mBrewingState = BREWING_PRE_STEP_ACTION;
	} else {
		mBrewingState = BREWING_START_STEP;
	}
}

void BrewOMatic::waitTempReached()
{
	if (abs(mCurrentTemp - mCurrentStep->mTargetTemp) > 1)
		return;

	beeperBeep(NOTE_B4, 20);
	mStepStartMillis = millis();
	dbgOutput("Step %s reached temp\n", mCurrentStep->mName);
	mStatus = STR_TEMP_REACHED;
	mBrewingState = BREWING_WAIT_END_OF_STEP;
}

void BrewOMatic::waitUserAction()
{
	unsigned long curMillis = millis();
	if ((curMillis - mLastBeepTime) > SEC_TO_MS(2)) {
		beeperBeep(NOTE_B4, 20);
		mLastBeepTime = curMillis;
	}
}

void BrewOMatic::brewingEnd()
{
	waitUserAction();
}

void BrewOMatic::handleBrewing()
{
	unsigned char b = handleButton(mBrewingMenu);

	switch (mBrewingState) {
		case BREWING_GET_NEXT_STEP:
			getNextStep();
		break;
		case BREWING_PRE_STEP_ACTION:
			waitUserAction();
			if (b != Input::BUTTON_NONE) {
				mBrewingState = BREWING_START_STEP;
				mCurrentAction = NULL;
				mUpdateDisplay = true;
				mStatus = 0;
			}
		break;
		case BREWING_START_STEP:
			startStep();
		break;
		case BREWING_WAIT_TEMP_REACHED:
			waitTempReached();
		break;
		case BREWING_WAIT_END_OF_STEP:
			waitEndOfStep();
		break;
		case BREWING_END:
			brewingEnd();
			if (b != Input::BUTTON_NONE) {
				actionStopBrewing();
				mStatus = STR_BREWING_DONE;
				break;
			}
		break;
	}

	updateTemp();
	updateDisplay();
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

void BrewOMatic::setError(brewStringIndex err)
{
	mError = true;
	mStatus = err;
}

void BrewOMatic::setup()
{
	int ret;

	pinMode(PUMP_CONTROL_PIN, OUTPUT);
	digitalWrite(PUMP_CONTROL_PIN, LOW);

	dbgOutput("Setup...\n");

	beeperBeep(NOTE_B4, 50);
	delay(50);
	beeperBeep(NOTE_C4, 50);

	mTempProbe = new TEMP_PROBE_TYPE();
	mDisp = new DISPLAY_TYPE();
	mHeaterControl = new HeaterControl(TEMP_SAMPLE_TIME_MS);
	mInput = new RotaryEncoder();

	mError = false;
	mStatus = STR_READY;

	if (sdInit())
		mStatus = STR_NO_SD_CARD;

	mIdleMenu = createIdleMenu();
	mBrewingMenu = createBrewingMenu();

	ret = mTempProbe->getTemp(&mCurrentTemp);
	if (ret)
		setError(STR_TEMP_FAILED);

	if (ACZeroCrossing::Instance().getFrequency() == 0)
		setError(STR_MISSING_MAIN);

	delay(SEC_TO_MS(START_DELAY));

	mDisp->enterIdle(this);
	mDisp->displayIdle(this);

	if (mError && !ENABLED(DEBUG))
		while(1);

	dbgOutput("Setup OK\n");
}

/**
 * Basic C stuff
 */
void setup()
{
	Serial.begin(SERIAL_BAUDRATE);

	brewOMatic.setup();
}

void loop()
{
	dbgOutput("Entering main loop\n");
	brewOMatic.run();
}


