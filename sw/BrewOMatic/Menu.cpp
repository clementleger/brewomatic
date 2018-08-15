#include "Menu.h"
#include "Language.h"
#include "SDHandling.h"
#include "BrewOMatic.h"

static void actionMenuBack(__attribute__((unused)) MenuItem *item, BrewOMatic *b)
{
	b->actionMenuBack(false);
}

static void actionStopBrewing(MenuItem *item, BrewOMatic *b)
{
	b->actionStopBrewing();
}

static void actionEnablePump(MenuItem *item, BrewOMatic *b)
{
	bool ret = b->actionEnablePump();
	MenuItemStrIdx *it = (MenuItemStrIdx *) item;

	it->mTitle = ret ? STR_DISABLE_PUMP : STR_ENABLE_PUMP;
}

static void actionStartManual(MenuItem *item, BrewOMatic *b)
{
	b->actionStartManual();
}

static void actionNextStep(MenuItem *item, BrewOMatic *b)
{
	b->actionNextStep();
}

static void actionSubmenu(MenuItem *item, BrewOMatic *b)
{
	MenuItemStrIdxMenu *it = (MenuItemStrIdxMenu *) item;
	b->setCurrentMenu(it->mMenu);
}

Menu::Menu(brewStringIndex name, unsigned char count, Menu *parent):
mItems(count + 1),
mName(name),
mParent(parent),
mSelected(0)
{
	/* Automatically add the default */
	MenuItemStrIdx *back = new MenuItemStrIdx(STR_BACK, actionMenuBack, ICON_BACK);
	mItems.addElem(back);
}

/* Create Idle Menu Tree
 * This tree will be displayed when in idle mode
 */
Menu *createIdleMenu()
{
	Menu *sdCardMenu;
	Menu *menu = new Menu(STR_MAIN_MENU, 2, NULL);

	menu->mItems.addElem(new MenuItemStrIdx(STR_MANUAL_MODE, actionStartManual, ICON_EMPTY));

	sdCardMenu = sdCreateBrowseMenu(menu);
	if (sdCardMenu) {
		menu->mItems.addElem(new MenuItemStrIdxMenu(STR_BROWSE_SD_CARD, actionSubmenu, sdCardMenu, ICON_FOLDER));
	} else {
		menu->mItems.addElem(new MenuItemStrIdx(STR_NO_SD_CARD, 0, ICON_CROSS));
	}

	return menu;
}


/* Create brewing Menu Tree
 * This tree will be displayed if button pressed during brewing
 */
Menu *createBrewingMenu()
{

	Menu *menu = new Menu(STR_BREWING_MENU, 3, NULL);
	menu->mItems.addElem(new MenuItemStrIdx(STR_STOP, actionStopBrewing, ICON_CROSS));
	menu->mItems.addElem(new MenuItemStrIdx(STR_ENABLE_PUMP, actionEnablePump, ICON_PUMP));
	menu->mItems.addElem(new MenuItemStrIdx(STR_NEXT_STEP, actionNextStep, ICON_EMPTY));

	return menu;
}

