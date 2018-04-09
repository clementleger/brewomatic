#ifndef _DISPLAY_H
#define _DISPLAY_H

class BrewOMatic;
class Menu;

class Display {
	public:
		virtual void enterIdle(BrewOMatic *b) = 0;
		virtual void enterMenu(BrewOMatic *b, Menu *m) = 0;
		virtual void enterBrewing(BrewOMatic *b) = 0;
		virtual void displayIdle(BrewOMatic *b) = 0;
		virtual void displayMenu(BrewOMatic *b, Menu *m) = 0;
		virtual void displayBrewing(BrewOMatic *b) = 0;
};

#endif /* _DISPLAY_H */
