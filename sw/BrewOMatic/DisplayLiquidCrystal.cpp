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
	lcd.setCursor((LIQUID_CRYSTAL_WIDTH - (strlen(str) + 2)) / 2, 0);
	lcd.print("<");
	lcd.print(str);
	lcd.print(">");
}

DisplayLiquidCrystal::DisplayLiquidCrystal():
lcd(LIQUID_CRYSTAL_RS,
	LIQUID_CRYSTAL_EN,
	LIQUID_CRYSTAL_D4,
	LIQUID_CRYSTAL_D5,
	LIQUID_CRYSTAL_D6,
	LIQUID_CRYSTAL_D7),
mPumpState(0),
mHeatState(0)
{
	lcd.begin(LIQUID_CRYSTAL_WIDTH, LIQUID_CRYSTAL_HEIGHT);
	lcd.createChar(BEER_CHAR, beer);
	lcd.createChar(THERMOMETER_CHAR, thermometer);
	lcd.createChar(DEGREE_CHAR, degree);
	lcd.createChar(PLUG_CHAR, plug);
	lcd.createChar(PUMP_CHAR, pump1);
	lcd.createChar(HEAT_CHAR, fire1);
	lcd.createChar(CLOCK_CHAR, clock);

	dispTitle(BREWOMATIC_VERSION_STRING);

	lcd.setCursor((LIQUID_CRYSTAL_WIDTH - strlen(getString(STR_STARTING))) / 2, 1);
	lcd.print(getString(STR_STARTING));
	
	lcd.setCursor((LIQUID_CRYSTAL_WIDTH - 3)/ 2, 2);
	lcd.write(byte(BEER_CHAR));
	lcd.print(" ");
	lcd.write(byte(BEER_CHAR));

	lcd.setCursor((LIQUID_CRYSTAL_WIDTH - (strlen(START_BANNER)))/ 2, 3);
	lcd.print(START_BANNER);
}

void DisplayLiquidCrystal::enterIdle(BrewOMatic *b)
{
	lcd.clear();
	dispTitle(BREWOMATIC_VERSION_STRING);

	lcd.setCursor(0, 1);
	lcd.write(byte(BEER_CHAR));
	lcd.print(" ");
	lcd.print(getString(STR_IDLE));

	lcd.setCursor(0, 3);
	if (b->mError) {
		lcd.print(getString(STR_ERROR));
		lcd.print(": ");
		lcd.print(getString(STR_MISSING_MAIN));
	} else {
		lcd.print(getString(STR_READY));
	}

	lcd.setCursor(LIQUID_CRYSTAL_WIDTH/2, 2);
	lcd.write(byte(PLUG_CHAR));
	lcd.print(ACZeroCrossing::Instance().getFrequency());
	lcd.print("Hz");
}

void DisplayLiquidCrystal::displayIdle(BrewOMatic *b)
{
	lcd.setCursor(0, 2);
	lcd.write(byte(THERMOMETER_CHAR));
	lcd.print(b->mCurrentTemp);
	lcd.write(byte(DEGREE_CHAR));
}

void DisplayLiquidCrystal::enterMenu(BrewOMatic *b, Menu *m)
{
	lcd.clear();

	mLastMenuStart = 0;
}

void DisplayLiquidCrystal::displayMenu(BrewOMatic *b, Menu *m)
{
	MenuItem *item;
	byte row = 0;
	byte dispCount = DISP_MENU_ENTRY;

	lcd.clear();
	if (m->getItemCount() < DISP_MENU_ENTRY) {
		mLastMenuStart = 0;
		dispCount = m->getItemCount();
	} else if (m->mSelected < mLastMenuStart) {
		mLastMenuStart = m->mSelected;
	} else if (m->mSelected > (mLastMenuStart + (DISP_MENU_ENTRY - 1))) {
		mLastMenuStart = m->mSelected - (DISP_MENU_ENTRY - 1);
	}

	for (byte i = mLastMenuStart; i < mLastMenuStart + dispCount; i++) {
		lcd.setCursor(0, row);
		row++;

		item = m->getItem(i);

		if (i == m->mSelected)
			lcd.print(">");
		else
			lcd.print(" ");

		lcd.print(getString(item->mTitle));
	}
}

void DisplayLiquidCrystal::loadAnimChar()
{
	lcd.createChar(PUMP_CHAR, pumpAnim[mPumpState]);

	mPumpState++;
	if (mPumpState == PUMP_STATE_COUNT)
		mPumpState = 0;

	lcd.createChar(HEAT_CHAR, heatAnim[mHeatState]);
	mHeatState++;
	if (mHeatState == HEAT_STATE_COUNT)
		mHeatState = 0;
}

void DisplayLiquidCrystal::drawBool(bool status)
{
	if (status) {
		lcd.print("On");
	} else {
		lcd.print("Off");
	}
}

void DisplayLiquidCrystal::enterBrewing(BrewOMatic *b)
{
	lcd.clear();
	lcd.setCursor(0, 3);
	dispTitle(b->mCurrentRecipe->mName);
}

void DisplayLiquidCrystal::drawStatus(BrewOMatic *b, int row)
{
	/* Draw temp status */
	lcd.setCursor(0, row);
	lcd.write(byte(THERMOMETER_CHAR));
	lcd.print((int) b->mCurrentTemp);
	lcd.write(byte(DEGREE_CHAR));
	lcd.print("/");
	lcd.print(b->mCurrentStep->mTargetTemp);
	lcd.write(byte(DEGREE_CHAR));

	loadAnimChar();
	lcd.setCursor(LIQUID_CRYSTAL_WIDTH / 2 + 1, row);
	/* Draw pump status */
	lcd.write(byte(PUMP_CHAR));
	drawBool(b->mCurrentStep->mEnablePump);

	lcd.setCursor(LIQUID_CRYSTAL_WIDTH / 2 + 6, row);

	/* Draw heater status */
	lcd.write(byte(HEAT_CHAR));
	drawBool(b->mCurrentStep->mEnableHeater);

	lcd.setCursor(LIQUID_CRYSTAL_WIDTH - 4, row + 1);
	if (b->mCurrentStep->mEnableHeater) {
		lcd.print((int) b->mHeaterControl->mPidOutput);
		lcd.print("% ");
	}
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
		lcd.print("0");
	lcd.print(elapsedMin);
	lcd.print(":");
	if (elapsedSec < 10)
		lcd.print("0");
	lcd.print(elapsedSec);
}

void DisplayLiquidCrystal::displayBrewing(BrewOMatic *b)
{
	lcd.setCursor(0, 0);
	lcd.write(byte(BEER_CHAR));
	lcd.print(" ");
	lcd.print(b->mCurrentStep->mName);

	drawStatus(b, 1);

	lcd.setCursor(0, 2);
	lcd.write(byte(CLOCK_CHAR));
	if (b->mTempReached) {
		drawTime(b->mStepStartMillis);
	} else {
		lcd.print("-");
	}
	lcd.print("/");
	lcd.print(b->mCurrentStep->mDuration);
	lcd.print(":00");
}

void DisplayLiquidCrystal::enterManual(BrewOMatic *b)
{
	lcd.clear();
	dispTitle(getString(STR_MANUAL_MODE));

	lcd.setCursor(0, 1);
	lcd.write(byte(BEER_CHAR));
	lcd.print(" ");
	lcd.print(getString(STR_MANUAL_MODE));
}

void DisplayLiquidCrystal::displayManual(BrewOMatic *b)
{

	drawStatus(b, 2);
	lcd.setCursor(0, 3);
	lcd.write(byte(CLOCK_CHAR));
	drawTime(b->mStepStartMillis);
}
