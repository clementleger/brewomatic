#ifndef _STATISTIC_H
#define _STATISTIC_H

#include "Config.h"

#if ENABLED(STAT_OUTPUT)

char statStart();
void statStop();
void statOutputTemp(unsigned char temp, unsigned char targetTemp);

#else

static char statStart()
{
	return 0;
}

static void statStop()
{

}
#endif

#endif /* _STATISTIC_H */
