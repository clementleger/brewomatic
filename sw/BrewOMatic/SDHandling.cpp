#include "Config.h"
#include "SerialOutput.h"
#include "Menu.h"

#if ENABLED(USE_SD_CARD)

#include "SDHandling.h"
#include "BrewOMatic.h"

#include "SPI.h"
#include "SdFat.h"

SdFat sd;
bool sdPresent = false;

int sdInit()
{
	int ret;

	ret = sd.begin(SD_CARD_CHIP_SELECT, SD_SCK_MHZ(50));
	if (!ret) {
		return 1;
	}

	sdPresent = true;

	return 0;
}

#define MAX_LINE_SIZE	40

static uint8_t fileGetLine(SdBaseFile *file, char line[MAX_LINE_SIZE])
{
	int c;
	uint8_t ret = 0;
	uint8_t cur_offset = 0;

	while (1) {
		c = file->read();

		/* End of file */
		if (c == -1) {
			ret = 1;
			break;
		}

		if (c == '\n') {
			ret = 1;
			break;
		}

		line[cur_offset] = c;

		/* We hit max line size (consider \0) */
		if (cur_offset == (MAX_LINE_SIZE - 2))
			break;

		cur_offset++;
	}

	line[cur_offset+1] = 0;

	return ret;
}

#define MAX_TOKEN 6

static uint8_t splitLine(char line[MAX_LINE_SIZE], char *ptrs[MAX_TOKEN])
{
	uint8_t token = 1;

	ptrs[0] = line;
	while(line) {
		if (line[0] == ';') {
			token++;
			ptrs[token] = line + 1;
			line[0] = 0;
		} else {
			line++;
		}
	}

	return token;
}

static int checkRecipeHeader(char *fields[MAX_TOKEN], uint8_t count)
{
	if (count != 3)
		return 1;

	if (fields[0][0] != 'R')
		return 1;

	if (strncmp(fields[1], BREWOMATIC_VERSION_STRING, strlen(BREWOMATIC_VERSION_STRING)))
		return 1;

	return 0;
}

#define MAX_RECIPE_STEPS	16

static uint8_t fileParseRecipe(SdBaseFile *file)
{
	char line[MAX_LINE_SIZE];
	uint8_t ret, count;
	Recipe *recipe;
	char *fields[MAX_TOKEN];

	/* Get the first line */
	ret = fileGetLine(file, line);
	count = splitLine(line, fields);

	if (checkRecipeHeader(fields, count))
		return 1;

	recipe = new Recipe(MAX_RECIPE_STEPS, fields[3]);
	do {
		ret = fileGetLine(file, line);
		
		
	} while(ret != -1);

}

static void actionExecuteRecipe(MenuItem *item, BrewOMatic *b)
{
	MenuItemStr *it = (MenuItemStr *) item;
	SdBaseFile file;
	uint8_t ret;

	dbgOutput("Starting %s\n", it->getTitleStr());

	file.open(it->getTitleStr(), O_READ);

	ret = fileParseRecipe(&file);
	if (ret) {
		b->mError = true;
		b->mStatus = STR_INVALID_RECIPE;
		b->actionMenuBack(true);
		return;
	}
}

#define MAX_NAME_SIZE	30

Menu *sdCreateBrowseMenu(Menu *parent)
{
	Menu *menu;
	int entryCount = 0;
	SdBaseFile file;
	char name[MAX_NAME_SIZE];

	if (!sdPresent)
		return NULL;

	sd.vwd()->rewind();
	while (file.openNext(sd.vwd(), O_READ)) {

		if (file.isDir()) {
			file.close();
			continue;
		}

		entryCount++;
		file.close();
	}

	menu = new Menu(STR_BROWSE_SD_CARD, entryCount, parent);

	sd.vwd()->rewind();
	while (file.openNext(sd.vwd(), O_READ)) {

		if (file.isDir()) {
			file.close();
			continue;
		}

		file.getName(name, MAX_NAME_SIZE);
		MenuItemStr *it = new MenuItemStr(strdup(name), actionExecuteRecipe);

		menu->mItems.addElem(it);

		file.close();
	}

	return menu;
}

#endif
