#ifndef _MENU_H
#define _MENU_H

#include "List.h"
#include "Language.h"

#include <stdint.h>

class BrewOMatic;
class Menu;
class MenuItem;

enum iconClass {
	ICON_EMPTY = 0,
	ICON_BACK,
	ICON_FOLDER,
	ICON_CROSS,
	ICON_PUMP,
};

typedef uint8_t brewMenuIcon;

typedef void (*menuItemCallback)(MenuItem *item, BrewOMatic *b);

class MenuItem {
	public:
		MenuItem(menuItemCallback callback, brewMenuIcon icon):
		mCallback(callback), mIcon(icon) {};
		virtual const char *getTitleStr() = 0;

		menuItemCallback mCallback;
		Menu *mSubMenu;
		brewMenuIcon mIcon;
};

class MenuItemStrIdx : public MenuItem {
	public:
		MenuItemStrIdx(brewStringIndex title, menuItemCallback callback, brewMenuIcon icon):
			MenuItem(callback, icon),
			mTitle(title) {};
		const char *getTitleStr() { return getString(mTitle);};

		brewStringIndex mTitle;
};

class MenuItemStr : public MenuItem {
	public:
		MenuItemStr(const char *title, menuItemCallback callback, brewMenuIcon icon):
			MenuItem(callback, icon),
			mTitle(title) {};
		const char *getTitleStr() { return mTitle;};

		const char *mTitle;
};

class MenuItemStrIdxMenu : public MenuItemStrIdx {
	public:
		MenuItemStrIdxMenu(brewStringIndex title, menuItemCallback callback, Menu *menu, brewMenuIcon icon):
			MenuItemStrIdx(title, callback, icon),
			mMenu(menu) {};

		Menu *mMenu;
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
