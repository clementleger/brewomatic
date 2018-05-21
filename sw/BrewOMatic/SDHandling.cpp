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
			line[cur_offset] = 0;
			break;
		}

		if (c == '\n') {
			line[cur_offset] = 0;
			goto exit;
		}

		line[cur_offset] = c;

		/* We hit max line size (consider \0) */
		if (cur_offset == (MAX_LINE_SIZE - 2))
			break;

		cur_offset++;
	}

	line[cur_offset+1] = 0;

exit:
	return ret;
}

#define MAX_TOKEN 4

static uint8_t splitLine(char line[MAX_LINE_SIZE], char *ptrs[MAX_TOKEN])
{
	uint8_t token = 1;

	ptrs[0] = line;

	while(line[0]) {
		if (line[0] == ';') {
			line[0] = 0;
			ptrs[token] = line + 1;
			token++;
			if (token == MAX_TOKEN)
				break;
		}
		line++;
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
#define MAX_ACTION_COUNT	4

static brewStringIndex getStepStr(const char c)
{
	switch (c) {
		case 'H': return STR_HEATING;
		case 'M': return STR_MASHING;
		case 'B': return STR_BOILING;
		case 'C': return STR_COOLING;
	}
}

static brewStringIndex getActionStr(const char c)
{
	switch (c) {
		case 'I': return STR_INSERT_MALT;
		case 'R': return STR_REMOVE_MALT;
		case 'C': return STR_SETUP_COOLER;
		case 'H': return STR_INSERT_HOP;
	}
}

static Recipe *fileParseRecipe(SdBaseFile *file)
{
	char line[MAX_LINE_SIZE];
	uint8_t ret, count;
	Recipe *recipe;
	Step *step = NULL;
	char *fields[MAX_TOKEN];
	int duration, targetTemp;
	bool pumpEnable;
	brewStringIndex idx;
	Action *action;

	/* Get the first line */
	ret = fileGetLine(file, line);
	count = splitLine(line, fields);

	if (checkRecipeHeader(fields, count)) {
		return NULL;
	}

	recipe = new Recipe(MAX_RECIPE_STEPS, fields[3]);

	do {
		ret = fileGetLine(file, line);
		/* Check end of file */
		if (ret)
			break;
			
		if (line[0] == '#')
			continue;

		count = splitLine(line, fields);
		if (count <= 2) {
			goto err;
		}

		switch (fields[0][0]) {
			case 'H':
			case 'M':
			case 'B':
			case 'C':
				if (count != 4) {
					goto err;
				}

				duration = atoi(fields[1]);
				targetTemp = atoi(fields[2]);
				pumpEnable = fields[3][0] == '0' ? false : true;
				idx = getStepStr(fields[0][0]);
				step = new Step(idx, duration, targetTemp, pumpEnable, MAX_ACTION_COUNT);
				recipe->mSteps.addElem(step);
			break;
			case 'A':
				if (count != 3 || step == NULL) {
					goto err;
				}

				idx = getActionStr(fields[1][0]);
				duration = atoi(fields[2]);
				action = new Action(idx, duration);
				step->mUserActions.addElem(action);				
			break;
		}
	} while(1);

	return recipe;
err:
	delete recipe;

	return NULL;
}

static void actionExecuteRecipe(MenuItem *item, BrewOMatic *b)
{
	MenuItemStr *it = (MenuItemStr *) item;
	SdBaseFile file;
	Recipe *recipe;

	dbgOutput("Starting %s\n", it->getTitleStr());

	file.open(it->getTitleStr(), O_READ);

	recipe = fileParseRecipe(&file);

	if (recipe == NULL) {
		b->mError = true;
		b->mStatus = STR_INVALID_RECIPE;
		b->actionMenuBack(true);
		return;
	}

	b->mCurrentRecipe = recipe;
	b->actionStartBrewing();
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
		MenuItemStr *it = new MenuItemStr(strdup(name), actionExecuteRecipe, ICON_EMPTY);

		menu->mItems.addElem(it);

		file.close();
	}

	return menu;
}

#endif
