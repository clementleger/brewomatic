#include "Menu.h"
#include "Language.h"


Menu::Menu(enum brewStringIndex name, unsigned char count):
mItems(count + 1),
mName(name)
{
	/* Automaticcaly add the default */
	MenuItem *back = new MenuItem(STR_BACK);
	mItems.addElem(back);
}

/* Create Idle Menu Tree
 * This tree will be displayed when in idle mode
 */
Menu *createIdleMenu()
{
	Menu *menu = new Menu(STR_MAIN_MENU, 6);

	return menu;
}

/* Create brewing Menu Tree
 * This tree will be displayed if button pressed during brewing
 */
Menu *createBrewingMenu()
{

	Menu *menu = new Menu(STR_BREWING_MENU, 6);

	return menu;
}
