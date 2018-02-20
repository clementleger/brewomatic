#ifndef _BREW_DISPLAY_H
#define _BREW_DISPLAY_H

class BrewDisplay {
	public:
		virtual int setup() = 0;
		virtual int dispIcon() = 0;
		virtual int dispStatus() = 0;
		virtual int dispBanner() = 0;
		virtual int dispTemp() = 0;
};

#endif /* _BREW_DISPLAY_H */
