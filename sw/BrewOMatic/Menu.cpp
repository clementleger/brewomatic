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

static void actionStartBrewing(BrewOMatic *b)
{
	b->actionStartBrewing();
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
	menu->mItems.addElem(new MenuItem(STR_ADJUST_TARGET, 0));
	menu->mItems.addElem(new MenuItem(STR_ADJUST_TEMP_OFFSET, 0));

	return menu;
}
