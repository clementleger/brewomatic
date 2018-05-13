#include "Config.h"
#include "SerialOutput.h"
#include "Menu.h"

#if ENABLED(USE_SD_CARD)

#include "SDHandling.h"

File sdRoot;
bool sdPresent = false;

int sdInit()
{
	int ret;

	ret = SD.begin(SD_CARD_CHIP_SELECT);
	if (!ret)
		return 1;

	sdRoot = SD.open("/");
	sdPresent = true;

	return 0;
}

#define MAX_LINE_SIZE	40

static void actionExecuteRecipe(MenuItem *item, BrewOMatic *b)
{
	MenuItemStr *it = item;
	uint8_t line[MAX_LINE_SIZE];
	int avail;

	dbgOutput("Starting %s\n", it->getTitleStr());

	File file = SD.open(it->getTitleStr());

	/* TODO: parse recipe */
}

Menu *sdCreateBrowseMenu(Menu *parent)
{
	Menu *menu;
	int entryCount = 0;

	if (!sdPresent)
		return NULL;

	while(true) {
		File entry = sdRoot.openNextFile();
		if (!entry)
			break;

		if (!entry.isDirectory())
			entryCount++;

		entry.close();
	}
	sdRoot.rewindDirectory();

	menu = new Menu(STR_BROWSE_SD_CARD, entryCount, parent);
	while(true) {
		File entry =  sdRoot.openNextFile();
		if (!entry)
			break;

		if (!entry.isDirectory()) {
			MenuItemStr *it = new MenuItemStr(strdup(entry.name()),
							  actionExecuteRecipe);
			menu->mItems.addElem(it);
		}
		entry.close();
	}
	return menu;
}

#endif
