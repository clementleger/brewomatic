#include "Config.h"
#include "Language.h"
#include "BrewOMatic.h"
#include "DisplayUcglib.h"

#include <SPI.h>

#define FONT_SIZE		16
#define VERTICAL_SPACING	8
#define TITLE_TEXT_X		16
#define TITLE_TEXT_Y		(FONT_SIZE + VERTICAL_SPACING)

#define STATUS_TEXT_X	(TITLE_TEXT_X)
#define STATUS_TEXT_Y	(TITLE_TEXT_Y + 2 * VERTICAL_SPACING + FONT_SIZE)

DisplayUcglib::DisplayUcglib():
ucg(UCGLIB_CD_PIN, UCGLIB_CS_PIN, UCGLIB_RST_PIN)
{
	ucg.begin(UCG_FONT_MODE_TRANSPARENT);
	ucg.setRotate270();
	ucg.clearScreen();

	ucg.setFont(ucg_font_inb16_mf);
	ucg.setColor(255, 165, 0);
	ucg.drawString(TITLE_TEXT_X, TITLE_TEXT_Y, 0, BREWOMATIC_VERSION_STRING);
	ucg.drawLine(0, TITLE_TEXT_Y + 8, ucg.getWidth(), TITLE_TEXT_Y + 8);
}

void DisplayUcglib::displayIdle(BrewOMatic *b)
{
	char buf[20];
	getStringInBuffer(STR_MODE, buf, 20);
	buf[strlen(buf)] = ':';
	getStringInBuffer(STR_IDLE, buf + strlen(buf), 20 - strlen(buf));
	ucg.setColor(255, 255, 255);
	ucg.drawString(STATUS_TEXT_X, STATUS_TEXT_Y, 0, buf);
}
