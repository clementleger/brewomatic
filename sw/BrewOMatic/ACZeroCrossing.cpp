#include <Arduino.h>

#include "Config.h"
#include "ACZeroCrossing.h"

#if ZERO_CROSSING_DETECT_PIN == 2
# define ZERO_CROSSING_IRQ	0
#elif ZERO_CROSSING_DETECT_PIN == 3
# define ZERO_CROSSING_IRQ	1
#endif

#define SEC_TO_MSECS	1000

ACZeroCrossing ACZeroCrossing::m_instance = ACZeroCrossing();

int computedFrequency = 0;

void computeFrequencyInterrupt()
{
	computedFrequency++;
}

void ACZeroCrossing::computeFrequency()
{
	unsigned long start = millis();

	/* Run for 1 second to compute main frequency */
	attachInterrupt(ZERO_CROSSING_IRQ, computeFrequencyInterrupt, RISING);
	while(millis() < (start + SEC_TO_MSECS));
	detachInterrupt(ZERO_CROSSING_IRQ);
	acFrequency = computedFrequency;

	if (abs(acFrequency - 50) <= 1) {
		acFrequency = 50;
	} else if (abs(acFrequency - 60) <= 1) {
		acFrequency = 60;
	}
}

void ACZeroCrossing::classZeroCrossingInterrupt()
{
	int i;

	for (i = 0; i < callbacksNr; i++) {
		callbacksFunc[i](callbacksData[i]);
	}
}

ACZeroCrossing::ACZeroCrossing()
{
	pinMode(ZERO_CROSSING_DETECT_PIN, INPUT);
	acFrequency = 0;
	computeFrequency();
	attachInterrupt(ZERO_CROSSING_IRQ, zeroCrossingInterrupt, RISING);
	callbacksNr = 0;
}

int ACZeroCrossing::addCallback(zero_crossing_callback cb, void *data)
{
	if (callbacksNr >= MAX_CALLBACKS)
		return 1;

	callbacksFunc[callbacksNr] = cb;
	callbacksData[callbacksNr] = data;

	callbacksNr++;
}
