#ifndef _MENU_H
#define _MENU_H

#include "List.h"
#include "Language.h"

class BrewOMatic;
class Menu;
class MenuItem;

typedef void (*menuItemCallback)(MenuItem *item, BrewOMatic *b);

class MenuItem {
	public:
		MenuItem(brewStringIndex title, menuItemCallback callback):
			mTitle(title),
			mCallback(callback) {};

		brewStringIndex mTitle;
		menuItemCallback mCallback;
};

class Menu {
	public:
		Menu(brewStringIndex name, unsigned char count, Menu *parent);

		unsigned char getItemCount() {return mItems.mElemCount;};
		MenuItem *getItem(unsigned char i) {return mItems.mElems[i];};
		MenuItem *getSelectedItem() {return mItems.mElems[mSelected];};

		List<MenuItem *> mItems;
		brewStringIndex mName;
		Menu *mParent;
		unsigned char mSelected;
};

Menu *createIdleMenu();
Menu *createBrewingMenu();

#endif
