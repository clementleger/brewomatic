#include "Config.h"
#include "BrewOMatic.h"
#include "DisplayUcglib.h"

#include <SPI.h>

#define MAIN_TEXT_X	20
#define MAIN_TEXT_Y	20

DisplayUcglib::DisplayUcglib():
ucg(UCGLIB_CD_PIN, UCGLIB_CS_PIN, UCGLIB_RST_PIN)
{
	ucg.begin(UCG_FONT_MODE_TRANSPARENT);
	ucg.clearScreen();
	ucg.setColor(100, 100, 100);
	ucg.drawBox(0, 0, 10, 10);
}

void DisplayUcglib::displayIdle(BrewOMatic *b)
{
	ucg.setColor(255, 255, 255);
	ucg.drawBox(0, 0, 40, 40);
}
