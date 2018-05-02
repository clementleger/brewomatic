#include "Menu.h"
#include "Config.h"
#include "Language.h"
#include "BrewOMatic.h"
#include "DisplayUcglib.h"

#include <SPI.h>

#define TEXT_COLOR		255, 255, 255
#define BACKGROUND_COLOR	0, 0, 0

#define FONT_SIZE		16
#define VERTICAL_SPACING	10
#define TITLE_TEXT_X		16
#define TITLE_TEXT_Y		(FONT_SIZE + VERTICAL_SPACING)

#define SEPARATOR_Y		(TITLE_TEXT_Y + 8)

#define STATUS_TEXT_X		(TITLE_TEXT_X)
#define STATUS_TEXT_Y		(SEPARATOR_Y + VERTICAL_SPACING + FONT_SIZE)

#define TEMP_TEXT_X		(STATUS_TEXT_X)
#define TEMP_TEXT_Y		(STATUS_TEXT_Y + VERTICAL_SPACING + FONT_SIZE)

#define STEP_TEXT_X		(TEMP_TEXT_X)
#define STEP_TEXT_Y		(TEMP_TEXT_Y + VERTICAL_SPACING + FONT_SIZE)

#define TARGET_TEXT_X		(STEP_TEXT_X)
#define TARGET_TEXT_Y		(STEP_TEXT_Y + VERTICAL_SPACING + FONT_SIZE)

#define DURATION_TEXT_X		(TARGET_TEXT_X)
#define DURATION_TEXT_Y		(TARGET_TEXT_Y + VERTICAL_SPACING + FONT_SIZE)

#define SELECTED_X		(20)
#define MENU_TEXT_X		(SELECTED_X + FONT_SIZE + 5)
#define MENU_TEXT_Y		(SEPARATOR_Y + VERTICAL_SPACING + FONT_SIZE)

DisplayUcglib::DisplayUcglib():
ucg(UCGLIB_CD_PIN, UCGLIB_CS_PIN, UCGLIB_RST_PIN)
{
	lastState = 0;
	ucg.begin(UCG_FONT_MODE_TRANSPARENT);
	ucg.setRotate270();

	ucg.setColor(BACKGROUND_COLOR);
	ucg.drawBox(0, 0, ucg.getWidth(), ucg.getHeight());

	ucg.setFont(ucg_font_10x20_mf);
	ucg.setColor(255, 165, 0);
	ucg.drawString(TITLE_TEXT_X, TITLE_TEXT_Y, 0, BREWOMATIC_VERSION_STRING);
	ucg.drawHLine(0, SEPARATOR_Y, ucg.getWidth());
	ucg.setColor(TEXT_COLOR);

	ucg.drawString(STATUS_TEXT_X, STATUS_TEXT_Y, 0, getString(STR_STARTING));
}

void DisplayUcglib::clearDisplayZone()
{
	ucg.setColor(BACKGROUND_COLOR);
	ucg.drawBox(0, SEPARATOR_Y + 1, ucg.getWidth(), ucg.getHeight() - (SEPARATOR_Y + 1));
	ucg.setColor(TEXT_COLOR);
}

void DisplayUcglib::displayIdle(BrewOMatic *b)
{
	if (lastState != 1) {
		lastState = 1;
		clearDisplayZone();
	}
	displayModeTemp(b, STR_IDLE);
}

void DisplayUcglib::displayModeTemp(BrewOMatic *b, brewStringIndex mode)
{
	char buf[20] = {0};
	getStringInBuffer(STR_MODE, buf, 20);
	buf[strlen(buf)] = ':';
	getStringInBuffer(mode, buf + strlen(buf), 20 - strlen(buf));
	ucg.drawString(STATUS_TEXT_X, STATUS_TEXT_Y, 0, buf);
	ucg.setFontMode(UCG_FONT_MODE_SOLID);
	ucg.setColor(0, TEXT_COLOR);
	ucg.setColor(1, BACKGROUND_COLOR);
	getStringInBuffer(STR_TEMPERATURE, buf, 20);
	sprintf(buf + strlen(buf), ": %d°C ", (unsigned char) b->mCurrentTemp);
	ucg.drawString(TEMP_TEXT_X, TEMP_TEXT_Y, 0, buf);
}

void DisplayUcglib::displayBrewing(BrewOMatic *b)
{
	char buf[30] = {0};
	if (lastState != 3) {
		lastState = 3;
		clearDisplayZone();
	}

	displayModeTemp(b, STR_BREWING);

	getStringInBuffer(STR_STEP, buf, 30);
	sprintf(buf + strlen(buf), ": %s       ", b->mCurrentStep->mName);
	ucg.drawString(STEP_TEXT_X, STEP_TEXT_Y, 0, buf);

	getStringInBuffer(STR_TARGET_TEMP, buf, 30);
	sprintf(buf + strlen(buf), ": %d°C ", (unsigned char) b->mCurrentStep->mTargetTemp );
	ucg.drawString(TARGET_TEXT_X, TARGET_TEXT_Y, 0, buf);

	if (b->mStepStartMillis != 0) {
		getStringInBuffer(STR_DURATION, buf, 30);
		sprintf(buf + strlen(buf), ": %dmin", (unsigned char) b->mCurrentStep->mDuration );
	} else {
		getStringInBuffer(STR_WAIT_TEMP, buf, 30);
	}
	ucg.drawString(DURATION_TEXT_X, DURATION_TEXT_Y, 0, buf);
}

void DisplayUcglib::displayMenu(BrewOMatic *b, Menu *m)
{
	MenuItem *item;
	if (lastState != 2) {
		lastState = 2;
		clearDisplayZone();
		ucg.setFontMode(UCG_FONT_MODE_SOLID);

		ucg.setColor(TEXT_COLOR);
		for (byte i = 0; i < m->getItemCount(); i++) {
			item = m->getItem(i);
			ucg.drawString(MENU_TEXT_X, MENU_TEXT_Y + i * (FONT_SIZE + VERTICAL_SPACING), 0, getString(item->mTitle));
		}
	}

	/* Clear selection indicator */
	ucg.setColor(BACKGROUND_COLOR);
	ucg.drawBox(SELECTED_X, MENU_TEXT_Y - FONT_SIZE, FONT_SIZE, m->getItemCount() * (FONT_SIZE + VERTICAL_SPACING));

	/* Draw new selection indicator */
	char offset = m->mSelected * (FONT_SIZE + VERTICAL_SPACING);
	ucg.setColor(0, 122, 255);
	ucg.drawTriangle(SELECTED_X,
			 MENU_TEXT_Y - FONT_SIZE + offset,
			 SELECTED_X + FONT_SIZE,
			 MENU_TEXT_Y - (FONT_SIZE / 2) + offset,
			 SELECTED_X,
			 MENU_TEXT_Y + offset);

}
