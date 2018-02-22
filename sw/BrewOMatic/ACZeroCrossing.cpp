#include <Arduino.h>
#include <avr/power.h>

#include "Config.h"
#include "ACZeroCrossing.h"

#if ZERO_CROSSING_DETECT_PIN == 2
# define ZERO_CROSSING_IRQ	0
#elif ZERO_CROSSING_DETECT_PIN == 3
# define ZERO_CROSSING_IRQ	1
#endif

#define SEC_TO_MSECS	1000

ACZeroCrossing ACZeroCrossing::m_instance = ACZeroCrossing();

unsigned char computedFrequency = 0;

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

	if (abs(computedFrequency - 100) <= 2) {
		acFrequency = 50;
		acPeriodUs = 20000;
	} else if (abs(computedFrequency - 120) <= 2) {
		acFrequency = 60;
		acPeriodUs = 16666;
	}
}

void ACZeroCrossing::classZeroCrossingInterrupt()
{
	int i;

	for (i = 0; i < MAX_CALLBACKS; i++) {
		if (callbacksFunc[i]) {
			callbacksFunc[i](callbacksData[i]);
		}
	}
}

ACZeroCrossing::ACZeroCrossing()
{
	pinMode(ZERO_CROSSING_DETECT_PIN, INPUT);
	acFrequency = 0;
	acPeriodUs = 0;
	computeFrequency();
	attachInterrupt(ZERO_CROSSING_IRQ, zeroCrossingInterrupt, RISING);
	
	for (int i = 0; i < MAX_CALLBACKS; i++) {
		callbacksFunc[i] = NULL;
		callbacksData[i] = NULL;
	}
}

int ACZeroCrossing::findFreeIdx()
{
	for(int i = 0; i < MAX_CALLBACKS; i++) {
		if (callbacksFunc[i] == NULL) {
			return i;
		}
	}

	return -1;
}

int ACZeroCrossing::addCallback(zeroCrossingCallback cb, void *data)
{
	int idx;

	idx = findFreeIdx();
	if (idx < 0)
		return -1;

	callbacksFunc[idx] = cb;
	callbacksData[idx] = data;

	return 0;
}

void ACZeroCrossing::removeCallback(int cbIdx)
{
	if (cbIdx < 0)
		return;
	callbacksFunc[cbIdx] = NULL;
	callbacksData[cbIdx] = NULL;
}
