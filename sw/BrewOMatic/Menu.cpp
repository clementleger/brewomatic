#include "Menu.h"
#include "Language.h"


static void actionMenuBack(BrewOMatic *b)
{
	b->actionMenuBack();
}

static void actionStopBrewing(BrewOMatic *b)
{
	b->actionStopBrewing();
}

static void actionEnablePump(BrewOMatic *b)
{
	if (b->mCurrentStep) {
		b->mCurrentStep->mEnablePump = !b->mCurrentStep->mEnablePump;
	}
}

static void actionEnableHeater(BrewOMatic *b)
{
	if (b->mCurrentStep) {
		b->mCurrentStep->mEnableHeater = !b->mCurrentStep->mEnableHeater;
	}
}

static void actionStartBrewing(BrewOMatic *b)
{
	b->actionStartBrewing();
}

static void actionStartManual(BrewOMatic *b)
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
	Menu *menu = new Menu(STR_MAIN_MENU, 2, NULL);
	menu->mItems.addElem(new MenuItem(STR_START_BREWING, actionStartBrewing));
	menu->mItems.addElem(new MenuItem(STR_MANUAL_MODE, actionStartManual));
	menu->mItems.addElem(new MenuItem(STR_ADJUST_TEMP_OFFSET, 0));

	return menu;
}


/* Create brewing Menu Tree
 * This tree will be displayed if button pressed during brewing
 */
Menu *createBrewingMenu()
{

	Menu *menu = new Menu(STR_BREWING_MENU, 3, NULL);
	menu->mItems.addElem(new MenuItem(STR_STOP, actionStopBrewing));
	menu->mItems.addElem(new MenuItem(STR_ENABLE_HEATER, actionEnableHeater));
	menu->mItems.addElem(new MenuItem(STR_ENABLE_PUMP, actionEnablePump));
	menu->mItems.addElem(new MenuItem(STR_ADJUST_TEMP_OFFSET, 0));

	return menu;
}

