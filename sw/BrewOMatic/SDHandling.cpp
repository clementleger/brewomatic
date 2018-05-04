#include "Config.h"
#include "SerialOutput.h"
#include "Menu.h"

#if ENABLED(USE_SD_CARD)

#include "SDHandling.h"

SDCard SDCard::mInstance = SDCard();

SDCard::SDCard():
mRoot(),
mPresent(false)
{

}

int SDCard::init()
{
	int ret;

	ret = SD.begin(SD_CARD_CHIP_SELECT);
	if (!ret) {
		dbgOutput("Card init failed\n");
		return 1;
	}

	mRoot = SD.open("/");
	mPresent = true;
	dbgOutput("SD card present\n");
	
	return 0;
}

Menu *SDCard::createBrowseMenu()
{
	Menu *menu;
	int entryCount = 0;

	while(true) {
		File entry =  mRoot.openNextFile();
		if (!entry)
			break;

		if (!entry.isDirectory())
			entryCount++;
	}

	menu = new Menu(STR_BROWSE_SD_CARD, entryCount, NULL);

	mRoot.rewindDirectory();
	return menu;
}

#endif
