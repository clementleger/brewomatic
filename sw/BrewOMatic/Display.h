#ifndef _BREW_DISPLAY_H
#define _BREW_DISPLAY_H

#include "BrewOMatic.h"

class Display {
	public:
		virtual int setup(BrewOMatic *b) = 0;
};

#endif /* _BREW_DISPLAY_H */
