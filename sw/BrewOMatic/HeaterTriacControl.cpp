#include "HeaterTriacControl.h"
#include "ACZeroCrossing.h"
#include "SerialOutput.h"
#include "Arduino.h"
#include "Config.h"

#include <TimerOne.h>

/**
 * Control the triac gate by using a simple 
 */

void releaseTriacGate()
{
	/* Disable TRIAC gate */
	digitalWrite(TRIAC_CONTROL_PIN, LOW);
	Timer1.stop();
}

void setTimerDutyCycle(void *data)
{
	HeaterTriacControl *tc = (HeaterTriacControl *) data;

	if (tc->mDutyCycle == 0)
		return;

	/* Activate TRIAC gate */
	digitalWrite(TRIAC_CONTROL_PIN, HIGH);

	if (tc->mDutyCycle > 98)
		return;

	Timer1.initialize(tc->mTriacGateKeepTimeUs);
}

HeaterTriacControl::HeaterTriacControl()
{
	pinMode(TRIAC_CONTROL_PIN, OUTPUT);
	digitalWrite(TRIAC_CONTROL_PIN, LOW);

	Timer1.initialize(100);
	Timer1.attachInterrupt(releaseTriacGate);
	Timer1.stop();

	ACZeroCrossing::Instance().setup();

	mACPeriodUs = ACZeroCrossing::Instance().getAcPeriodUs();
	mDutyCycle = 0;
}

void HeaterTriacControl::enable(bool enable)
{
	if (enable) {		
		ACZeroCrossing::Instance().setCallback(setTimerDutyCycle, this);
	} else {
		ACZeroCrossing::Instance().removeCallback();
	}
}

void HeaterTriacControl::setDutyCycle(unsigned char value)
{

	unsigned long tmp = (value * (mACPeriodUs/2) / 100);

	mDutyCycle = value;
	noInterrupts();
	mTriacGateKeepTimeUs = tmp;
	interrupts();
}
