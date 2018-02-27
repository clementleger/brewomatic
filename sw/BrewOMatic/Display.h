#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "BrewOMatic.h"

class Display {
	public:
		virtual int setup(BrewOMatic *b) = 0;
};

#endif /* _DISPLAY_H */
