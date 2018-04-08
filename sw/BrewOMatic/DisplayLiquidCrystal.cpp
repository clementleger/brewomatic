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
	0b00000,
	0b01010,
	0b01010,
	0b11111,
	0b11111,
	0b11111,
	0b01110,
	0b00000
};

const static byte pump1[8] = {
	0b00000,
	0b01110,
	0b10101,
	0b11111,
	0b10101,
	0b01110,
	0b00000,
	0b00000
};

const static byte pump2[8] = {
	0b00000,
	0b01110,
	0b11011,
	0b10101,
	0b11011,
	0b01110,
	0b00000,
	0b00000
};

enum customChars{
	BEER_CHAR = 0,
	THERMOMETER_CHAR,
	DEGREE_CHAR,
	PLUG_CHAR,
	PUMP1_CHAR,
	PUMP2_CHAR,
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
	LIQUID_CRYSTAL_D7)
{
	lcd.begin(LIQUID_CRYSTAL_WIDTH, LIQUID_CRYSTAL_HEIGHT);
	lcd.createChar(BEER_CHAR, beer);
	lcd.createChar(THERMOMETER_CHAR, thermometer);
	lcd.createChar(DEGREE_CHAR, degree);
	lcd.createChar(PLUG_CHAR, plug);
	lcd.createChar(PUMP1_CHAR, pump1);
	lcd.createChar(PUMP2_CHAR, pump2);

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

void DisplayLiquidCrystal::displayIdle(BrewOMatic *b)
{
	lcd.clear();
	dispTitle(BREWOMATIC_VERSION_STRING);

	lcd.setCursor(0, 1);
	lcd.write(byte(BEER_CHAR));
	lcd.print(" ");
	lcd.print(getString(STR_IDLE));

	lcd.setCursor(0, 2);
	lcd.write(byte(THERMOMETER_CHAR));
	lcd.print(b->mCurrentTemp);
	lcd.write(byte(DEGREE_CHAR));

	lcd.setCursor(LIQUID_CRYSTAL_WIDTH/2, 2);
	lcd.write(byte(PLUG_CHAR));
	lcd.print(ACZeroCrossing::Instance().getFrequency());
	lcd.print("Hz");
}

void DisplayLiquidCrystal::displayMenu(BrewOMatic *b, Menu *m)
{
	MenuItem *item;
	lcd.clear();
	dispTitle(getString(m->mName));

	for (byte i = 0; i < m->getItemCount() && i < 3; i++) {
		lcd.setCursor(0, 1 + i);
		item = m->getItem(i);
		
		if (i == m->mSelected)
			lcd.print(">");
		else
			lcd.print(" ");
			
		lcd.print(getString(item->mTitle));
	}
}

void DisplayLiquidCrystal::displayBrewing(BrewOMatic *b)
{
	lcd.clear();
	dispTitle(b->mCurrentRecipe->mName);

	lcd.setCursor(0, 1);
	lcd.write(byte(BEER_CHAR));
	lcd.print(" ");
	lcd.print(b->mCurrentStep->mName);

	lcd.setCursor(0, 2);
	lcd.write(byte(THERMOMETER_CHAR));
	lcd.print((int) b->mCurrentTemp);
	lcd.write(byte(DEGREE_CHAR));
	lcd.print("/");
	lcd.print(b->mCurrentStep->mTargetTemp);
	lcd.write(byte(DEGREE_CHAR));

	if (b->mCurrentStep->mEnablePump) {
		
	}
}
