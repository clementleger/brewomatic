#ifndef _DISPLAY_H
#define _DISPLAY_H

class BrewOMatic;

class Display {
	public:
		virtual void displayIdle(BrewOMatic *b) = 0;
};

#endif /* _DISPLAY_H */
