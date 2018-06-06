#ifndef _SDHANDLING_H
#define _SDHANDLING_H

#include "Config.h"
#include "Arduino.h"
#include "Menu.h"

#if ENABLED(USE_SD_CARD)


int sdInit();
Menu *sdCreateBrowseMenu(Menu *parent);

#else

static inline int sdInit() {
	return 1;
}

static inline Menu *sdCreateBrowseMenu(Menu *parent) {
	return NULL;
}

#endif

#endif /* _SDHANDLING_H */
