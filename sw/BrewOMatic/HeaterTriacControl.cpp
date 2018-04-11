#include "Config.h"
#include "HeaterTriacControl.h"
#include "ACZeroCrossing.h"
#include "Arduino.h"

#include <TimerOne.h>

#define TRIAC_MIN_GATE_TIME_US	500

/**
 * Control the triac gate by using a simple 
 */

void releaseTriacGate()
{
	/* Disable TRIAC gate */
	digitalWrite(TRIAC_CONTROL_PIN, LOW);
}

void setTimerDutyCycle(void *data)
{
	HeaterTriacControl *tc = (HeaterTriacControl *) data;

	tc->mPulseCount++;
	if (tc->mPulseCount == (tc->mFrequency * 2))
		tc->mPulseCount = 0;

	if (tc->mPulseSkipCount == 0)
		return;

	tc->mPulseSkipCount--;

	/* Activate TRIAC gate */
	digitalWrite(TRIAC_CONTROL_PIN, HIGH);

	/* Configure the time to pop in TRIAC_MIN_GATE_TIME_US
	 * in order to release gate */
	Timer1.initialize(TRIAC_MIN_GATE_TIME_US);
	Timer1.attachInterrupt(releaseTriacGate);
}

HeaterTriacControl::HeaterTriacControl()
{
	Timer1.initialize();
	pinMode(TRIAC_CONTROL_PIN, OUTPUT);
	digitalWrite(TRIAC_CONTROL_PIN, LOW);
	mCbIdx = -1;

	ACZeroCrossing::Instance().setup();
	
	mFrequency = ACZeroCrossing::Instance().getFrequency();
}

void HeaterTriacControl::enable(bool enable)
{
	if (enable)
		mCbIdx = ACZeroCrossing::Instance().addCallback(setTimerDutyCycle, this);
	else
		ACZeroCrossing::Instance().removeCallback(mCbIdx);
}

void HeaterTriacControl::setDutyCycle(unsigned char value)
{
	/*
	 * Compute number of pulse to skip
	 * Depending on frequency, we either have 60 or 50 level 
	 * because we will only skip full periods
	 */
	unsigned long tmp = value * mFrequency / 100;
	mPulseSkipCount = tmp * 2;
}
