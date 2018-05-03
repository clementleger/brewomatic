#ifndef _SDHANDLING_H
#define _SDHANDLING_H

#include "Config.h"

#if ENABLED(USE_SD_CARD)

#include "Recipe.h"
#include "Menu.h"

#include <SPI.h>
#include <SD.h>

class SDCard {
	public:
		static SDCard& Instance(){ return mInstance; };
		SDCard();
		int init();
		bool present() {return mPresent;};

	private:
		Sd2Card mCard;
		SdVolume mVolume;
		SdFile mRoot;
		static SDCard mInstance;
		bool mPresent;
};

#endif

#endif /* _SDHANDLING_H */
