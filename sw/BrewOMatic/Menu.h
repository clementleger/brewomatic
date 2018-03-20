#ifndef _MENU_H
#define _MENU_H

#include "List.h"
#include "Language.h"

class BrewOMatic;
class Menu;

typedef void (*menuItemCallback)(BrewOMatic *b);

class MenuItem {
	public:
		MenuItem(enum brewStringIndex title): mTitle(title) {};

		enum brewStringIndex mTitle;
		menuItemCallback callback;
		Menu *mSubmenu;
};

class Menu {
	public:
		Menu(enum brewStringIndex name, unsigned char count);

		List<MenuItem *> mItems;
		enum brewStringIndex mName;
		Menu *mParent;
};

Menu *createIdleMenu();

#endif
