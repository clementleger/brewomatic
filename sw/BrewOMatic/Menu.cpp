#include "Menu.h"
#include "Language.h"


static void actionMenuBack(MenuItem *item, BrewOMatic *b)
{
	b->actionMenuBack();
}

static void actionStopBrewing(MenuItem *item, BrewOMatic *b)
{
	b->actionStopBrewing();
}

static void actionEnablePump(MenuItem *item, BrewOMatic *b)
{
	bool ret = b->actionEnablePump();

	item->mTitle = ret ? STR_DISABLE_PUMP : STR_ENABLE_PUMP;
}

static void actionEnableHeater(MenuItem *item, BrewOMatic *b)
{
	bool ret = b->actionEnableHeater();

	item->mTitle = ret ? STR_DISABLE_HEATER : STR_ENABLE_HEATER;
}

static void actionStartBrewing(MenuItem *item, BrewOMatic *b)
{
	b->actionStartBrewing();
}

static void actionStartManual(MenuItem *item, BrewOMatic *b)
{
	b->actionStartManual();
}

Menu::Menu(brewStringIndex name, unsigned char count, Menu *parent):
mItems(count + 1),
mName(name),
mParent(parent),
mSelected(0)
{
	/* Automaticcaly add the default */
	MenuItem *back = new MenuItem(STR_BACK, actionMenuBack);
	mItems.addElem(back);
}

/* Create Idle Menu Tree
 * This tree will be displayed when in idle mode
 */
Menu *createIdleMenu()
{
	Menu *menu = new Menu(STR_MAIN_MENU, 4, NULL);
	menu->mItems.addElem(new MenuItem(STR_START_BREWING, actionStartBrewing));
	menu->mItems.addElem(new MenuItem(STR_MANUAL_MODE, actionStartManual));
	menu->mItems.addElem(new MenuItem(STR_ENABLE_PUMP, actionEnablePump));
	menu->mItems.addElem(new MenuItem(STR_ADJUST_TEMP_OFFSET, 0));

	return menu;
}


/* Create brewing Menu Tree
 * This tree will be displayed if button pressed during brewing
 */
Menu *createBrewingMenu()
{

	Menu *menu = new Menu(STR_BREWING_MENU, 4, NULL);
	menu->mItems.addElem(new MenuItem(STR_STOP, actionStopBrewing));
	menu->mItems.addElem(new MenuItem(STR_ENABLE_HEATER, actionEnableHeater));
	menu->mItems.addElem(new MenuItem(STR_ENABLE_PUMP, actionEnablePump));
	menu->mItems.addElem(new MenuItem(STR_ADJUST_TEMP_OFFSET, 0));

	return menu;
}

