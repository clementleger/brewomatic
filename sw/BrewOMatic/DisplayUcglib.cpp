#include "Config.h"
#include "BrewOMatic.h"
#include "DisplayUcglib.h"

#include <SPI.h>

#define TITLE_TEXT_X	24
#define TITLE_TEXT_Y	24

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
	
}
