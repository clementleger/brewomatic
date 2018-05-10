#include <Arduino.h>
#include <avr/power.h>

#include "Config.h"
#include "SerialOutput.h"
#include "ACZeroCrossing.h"

#if ZERO_CROSSING_DETECT_PIN == 2
# define ZERO_CROSSING_IRQ	0
#elif ZERO_CROSSING_DETECT_PIN == 3
# define ZERO_CROSSING_IRQ	1
#endif

#define SEC_TO_MSECS	1000

ACZeroCrossing ACZeroCrossing::mInstance = ACZeroCrossing();

unsigned char computedFrequency = 0;

void computeFrequencyInterrupt()
{
	computedFrequency++;
}

void ACZeroCrossing::computeFrequency()
{
	unsigned long start = millis();

	dbgOutput("Detecting main frequency\n");
	/* Run for 1 second to compute main frequency */
	attachInterrupt(ZERO_CROSSING_IRQ, computeFrequencyInterrupt, RISING);
	while(millis() < (start + SEC_TO_MSECS));
	detachInterrupt(ZERO_CROSSING_IRQ);
	dbgOutput("End of main frequency detection: %d\n", computedFrequency / 2);

	if (abs(computedFrequency - 100) <= 2) {
		acFrequency = 50;
		acPeriodUs = 20000;
	} else if (abs(computedFrequency - 120) <= 2) {
		acFrequency = 60;
		acPeriodUs = 16666;
	} else {
		dbgOutput("Failed to detect main frequency\n");
	}
}

ACZeroCrossing::ACZeroCrossing()
{
	pinMode(ZERO_CROSSING_DETECT_PIN, INPUT_PULLUP);
	acFrequency = 0;
	acPeriodUs = 0;
}

void ACZeroCrossing::setup()
{
	computeFrequency();
	attachInterrupt(ZERO_CROSSING_IRQ, zeroCrossingInterrupt, RISING);
	removeCallback();
}

void ACZeroCrossing::setCallback(zeroCrossingCallback cb, void *data)
{
	noInterrupts();
	mCallbackFunc = cb;
	mCallbackData = data;
	interrupts();
}

void ACZeroCrossing::removeCallback()
{
	setCallback(NULL, NULL);
}
