#include "Config.h"

#if ENABLED(LIQUID_CRYSTAL_DISPLAY)

#include "Menu.h"
#include "Language.h"
#include "BrewOMatic.h"
#include "ACZeroCrossing.h"
#include "DisplayLiquidCrystal.h"

#include <Arduino.h>

const static byte degree[8] = {
	0b01100,
	0b10010,
	0b10010,
	0b01100,
	0b00000,
	0b00000,
	0b00000,
	0b00000
};

const static byte thermometer[8] = {
	0b00100,
	0b01010,
	0b01010,
	0b01010,
	0b01010,
	0b10001,
	0b10001,
	0b01110
};

const static byte clock[8] = {
	0b00000,
	0b01110,
	0b10011,
	0b10101,
	0b10001,
	0b01110,
	0b00000,
	0b00000
};

const static byte folder[8] = {
	0b00000,
	0b11100,
	0b11111,
	0b10001,
	0b10001,
	0b11111,
	0b00000,
	0b00000
};

const static byte beer[8] = {
	0b00000,
	0b11100,
	0b11111,
	0b11101,
	0b11101,
	0b11101,
	0b11110,
	0b00000
};

const static byte plug[8] = {
	0b01010,
	0b01010,
	0b11111,
	0b10001,
	0b10001,
	0b01110,
	0b00100,
	0b00100
};

const static byte pump1[8] = {
	0b01010,
	0b01010,
	0b10011,
	0b10101,
	0b10101,
	0b11001,
	0b01010,
	0b01010
};

const static byte fire1[8] = {
	0b00000,
	0b01000,
	0b01100,
	0b01100,
	0b01110,
	0b11111,
	0b11111,
	0b01110
};

#if ENABLED(LIQUID_CRYSTAL_ANIM)

const static byte pump2[8] = {
	0b01010,
	0b01010,
	0b10001,
	0b10111,
	0b11101,
	0b10001,
	0b01010,
	0b01010
};

const static byte pump3[8] = {
	0b01010,
	0b01010,
	0b10001,
	0b11101,
	0b10111,
	0b10001,
	0b01010,
	0b01010
};

const static byte pump4[8] = {
	0b01010,
	0b01010,
	0b11001,
	0b10101,
	0b10101,
	0b10011,
	0b01010,
	0b01010
};

const static byte pump5[8] = {
	0b01010,
	0b01010,
	0b10101,
	0b10101,
	0b10101,
	0b10101,
	0b01010,
	0b01010
};

#define PUMP_STATE_COUNT	5

static byte *pumpAnim[PUMP_STATE_COUNT] = {pump1, pump2, pump3, pump4, pump5};

const static byte fire2[8] = {
	0b00000,
	0b00100,
	0b01100,
	0b01100,
	0b01110,
	0b11111,
	0b11111,
	0b01110
};

const static byte fire3[8] = {
	0b00000,
	0b00000,
	0b00100,
	0b01100,
	0b01110,
	0b11111,
	0b11111,
	0b01110
};

const static byte fire4[8] = {
	0b00000,
	0b00000,
	0b01000,
	0b01100,
	0b01110,
	0b11111,
	0b11111,
	0b01110
};

#define HEAT_STATE_COUNT	4

static byte *heatAnim[HEAT_STATE_COUNT] = {fire1, fire2, fire3, fire4};

#endif

#define DISP_MENU_ENTRY	(LIQUID_CRYSTAL_HEIGHT)

enum customChars{
	BEER_CHAR = 0,
	THERMOMETER_CHAR,
	DEGREE_CHAR,
	PLUG_CHAR,
	PUMP_CHAR,
	HEAT_CHAR,
	CLOCK_CHAR,
};

void DisplayLiquidCrystal::dispTitle(const char *str)
{
	mLcd.setCursor((LIQUID_CRYSTAL_WIDTH - (strlen(str) + 2)) / 2, 0);
	mLcd.print("<");
	mLcd.print(str);
	mLcd.print(">");
}

DisplayLiquidCrystal::DisplayLiquidCrystal():
mLcd(LIQUID_CRYSTAL_RS,
	LIQUID_CRYSTAL_EN,
	LIQUID_CRYSTAL_D4,
	LIQUID_CRYSTAL_D5,
	LIQUID_CRYSTAL_D6,
	LIQUID_CRYSTAL_D7),
mPumpState(0),
mHeatState(0)
{
	const char *str;
	mLcd.begin(LIQUID_CRYSTAL_WIDTH, LIQUID_CRYSTAL_HEIGHT);
	mLcd.createChar(BEER_CHAR, (uint8_t *) beer);
	mLcd.createChar(THERMOMETER_CHAR, (uint8_t *) thermometer);
	mLcd.createChar(DEGREE_CHAR, (uint8_t *) degree);
	mLcd.createChar(PLUG_CHAR, (uint8_t *) plug);
	mLcd.createChar(PUMP_CHAR, (uint8_t *) pump1);
	mLcd.createChar(HEAT_CHAR, (uint8_t *) fire1);
	mLcd.createChar(CLOCK_CHAR, (uint8_t *) clock);

	dispTitle(getString(STR_BREWOMATIC));
	str = getString(STR_STARTING);
	mLcd.setCursor((LIQUID_CRYSTAL_WIDTH - strlen(str)) / 2, 1);
	mLcd.print(str);

	mLcd.setCursor((LIQUID_CRYSTAL_WIDTH - 3)/ 2, 2);
	mLcd.write(byte(BEER_CHAR));
	mLcd.print(" ");
	mLcd.write(byte(BEER_CHAR));

	mLcd.setCursor(LIQUID_CRYSTAL_WIDTH / 2 - 2, 2);
	mLcd.print("V");
	mLcd.write(BREWOMATIC_VERSION_STRING);
}

void DisplayLiquidCrystal::enterIdle(BrewOMatic *b)
{
	mLcd.clear();
	dispTitle(getString(STR_BREWOMATIC));

	mLcd.setCursor(0, 1);
	mLcd.write(byte(BEER_CHAR));
	mLcd.print(" ");
	mLcd.print(getString(STR_IDLE));

	mLcd.setCursor(0, 3);
	if (b->mError) {
		mLcd.print(getString(STR_ERROR));
		mLcd.print(": ");
	}

	mLcd.print(getString(b->mStatus));

	mLcd.setCursor(LIQUID_CRYSTAL_WIDTH/2, 2);
	mLcd.write(byte(PLUG_CHAR));
	mLcd.print(ACZeroCrossing::Instance().getFrequency());
	mLcd.print("Hz");
}

void DisplayLiquidCrystal::displayIdle(BrewOMatic *b)
{
	mLcd.setCursor(0, 2);
	mLcd.write(byte(THERMOMETER_CHAR));
	mLcd.print(b->mCurrentTemp);
	mLcd.write(byte(DEGREE_CHAR));
}

void DisplayLiquidCrystal::enterMenu(BrewOMatic *b, Menu *m)
{
	mLcd.clear();

	mLastMenuStart = 0;
}

void DisplayLiquidCrystal::displayMenu(BrewOMatic *b, Menu *m)
{
	MenuItem *item;
	byte row = 0;
	byte dispCount = DISP_MENU_ENTRY;

	mLcd.clear();
	if (m->getItemCount() < DISP_MENU_ENTRY) {
		mLastMenuStart = 0;
		dispCount = m->getItemCount();
	} else if (m->mSelected < mLastMenuStart) {
		mLastMenuStart = m->mSelected;
	} else if (m->mSelected > (mLastMenuStart + (DISP_MENU_ENTRY - 1))) {
		mLastMenuStart = m->mSelected - (DISP_MENU_ENTRY - 1);
	}

	for (byte i = mLastMenuStart; i < mLastMenuStart + dispCount; i++) {
		mLcd.setCursor(0, row);
		row++;

		item = m->getItem(i);

		if (i == m->mSelected)
			mLcd.print(">");
		else
			mLcd.print(" ");

		mLcd.print(item->getTitleStr());
	}
}

#if ENABLED(LIQUID_CRYSTAL_ANIM)

void DisplayLiquidCrystal::loadAnimChar()
{
	mLcd.createChar(PUMP_CHAR, pumpAnim[mPumpState]);

	mPumpState++;
	if (mPumpState == PUMP_STATE_COUNT)
		mPumpState = 0;

	mLcd.createChar(HEAT_CHAR, heatAnim[mHeatState]);
	mHeatState++;
	if (mHeatState == HEAT_STATE_COUNT)
		mHeatState = 0;
}
#else

void DisplayLiquidCrystal::loadAnimChar()
{
}

#endif

void DisplayLiquidCrystal::drawBool(bool status)
{
	if (status) {
		mLcd.print("On");
	} else {
		mLcd.print("Off");
	}
}

void DisplayLiquidCrystal::enterBrewing(BrewOMatic *b)
{
	mLcd.clear();
}

void DisplayLiquidCrystal::drawStatus(BrewOMatic *b, int row)
{
	/* Draw temp status */
	mLcd.setCursor(0, row);
	mLcd.write(byte(THERMOMETER_CHAR));
	mLcd.print((int) b->mCurrentTemp);
	mLcd.write(byte(DEGREE_CHAR));
	mLcd.print("/");
	mLcd.print(b->mTargetTemp);
	mLcd.write(byte(DEGREE_CHAR));

	loadAnimChar();
	mLcd.setCursor(LIQUID_CRYSTAL_WIDTH / 2 + 1, row);
	/* Draw pump status */
	mLcd.write(byte(PUMP_CHAR));
	drawBool(b->mCurrentStep->mEnablePump);

	mLcd.setCursor(LIQUID_CRYSTAL_WIDTH / 2 + 6, row);

	/* Draw heater status */
	mLcd.write(byte(HEAT_CHAR));
	drawBool(1);

	mLcd.setCursor(LIQUID_CRYSTAL_WIDTH - 4, row + 1);
	mLcd.print((int) b->mHeaterControl->mPidOutput);
	mLcd.print("% ");
}

void DisplayLiquidCrystal::drawTime(unsigned long amillis)
{
	unsigned long elapsed;
	unsigned int elapsedMin, elapsedSec;

	elapsed = millis() - amillis;
	elapsedMin = elapsed / ((unsigned long) 1000 * 60);
	elapsedSec = (elapsed - (elapsedMin * 60 * 1000));
	elapsedSec /= 1000;
	if (elapsedMin < 10)
		mLcd.print("0");
	mLcd.print(elapsedMin);
	mLcd.print(":");
	if (elapsedSec < 10)
		mLcd.print("0");
	mLcd.print(elapsedSec);
}

void DisplayLiquidCrystal::displayBrewing(BrewOMatic *b)
{
	mLcd.setCursor(0, 0);
	mLcd.write(byte(BEER_CHAR));
	mLcd.print(" ");
	mLcd.print(getString(b->mCurrentStep->mName));

	drawStatus(b, 1);

	mLcd.setCursor(0, 2);
	mLcd.write(byte(CLOCK_CHAR));
	if (b->mStepStartMillis != 0) {
		drawTime(b->mStepStartMillis);
	} else {
		mLcd.print("-");
	}
	mLcd.print("/");
	mLcd.print(b->mCurrentStep->mDuration);
	mLcd.print(":00");

	if (b->mStatus) {
		mLcd.setCursor(0, 3);
		mLcd.print(getString(b->mStatus));
	}
}

void DisplayLiquidCrystal::enterManual(BrewOMatic *b)
{
	mLcd.clear();
	dispTitle(getString(STR_MANUAL_MODE));

	mLcd.setCursor(0, 1);
	mLcd.write(byte(BEER_CHAR));
	mLcd.print(" ");
	mLcd.print(getString(STR_MANUAL_MODE));
}

void DisplayLiquidCrystal::displayManual(BrewOMatic *b)
{

	drawStatus(b, 2);
	mLcd.setCursor(0, 3);
	mLcd.write(byte(CLOCK_CHAR));
	drawTime(b->mStepStartMillis);
}

#endif
