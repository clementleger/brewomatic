#include "Arduino.h"

#include "Config.h"
#include "Statistic.h"
#include "SerialOutput.h"

#if ENABLED(STAT_OUTPUT)

#if ENABLED(STAT_OUTPUT_SD) && ENABLED(USE_SD_CARD)
#include "SdFat.h"
static SdBaseFile *logFile;
#endif

char statStart()
{
	char ret = 0;
#if ENABLED(STAT_OUTPUT_SD)&& ENABLED(USE_SD_CARD)
	logFile = new SdBaseFile("brew.log", O_WRITE | O_TRUNC | O_CREAT);
	if (logFile == NULL)
		ret = 1;
#endif
	return ret;
}

#define MAX_BUF_SIZE	10

static char buf[MAX_BUF_SIZE];

void statOutputTemp(unsigned char temp, unsigned char targetTemp)
{
	memset(buf, 0, MAX_BUF_SIZE);

	itoa(temp, buf, 10);
	buf[strlen(buf)] = ':';
	itoa(targetTemp, &buf[strlen(buf)], 10);
	buf[strlen(buf)] = '\n';
	int size = strlen(buf);
#if ENABLED(STAT_OUTPUT_SD) && ENABLED(USE_SD_CARD)
	logFile->write(buf, size);
	logFile->sync();
#endif
#if ENABLED(STAT_OUTPUT_SERIAL)
	outSerial.print(buf);
#endif
}

void statStop()
{
#if ENABLED(STAT_OUTPUT_SD) && ENABLED(USE_SD_CARD)
	logFile->close();
	delete logFile;
#endif
}

#endif
