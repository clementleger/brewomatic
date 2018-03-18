#include "Config.h"
#include "HeaterTriacControl.h"
#include "ACZeroCrossing.h"
#include "Arduino.h"

#include <TimerOne.h>

#define TRIAC_MIN_GATE_TIME_US	10

void releaseTriacGate()
{
	/* Disable TRIAC gate */
	digitalWrite(TRIAC_CONTROL_PIN, LOW);
}

void activateTriacGate()
{
	/* Activate TRIAC gate */
	digitalWrite(TRIAC_CONTROL_PIN, HIGH);

	/* Configure the time to pop in TRIAC_MIN_GATE_TIME_US
	 * in order to release gate */
	Timer1.initialize(TRIAC_MIN_GATE_TIME_US);
	Timer1.attachInterrupt(releaseTriacGate);
}

void setTimerDutyCycle(void *data)
{
	HeaterTriacControl *tc = (HeaterTriacControl *) data;
	/* Setup timer to power on TRIAC gate */
	Timer1.initialize(tc->getTriacTriggerTimeUs());
	Timer1.attachInterrupt(activateTriacGate);
}

HeaterTriacControl::HeaterTriacControl()
{
	Timer1.initialize();
	pinMode(TRIAC_CONTROL_PIN, OUTPUT);
	digitalWrite(TRIAC_CONTROL_PIN, LOW);

	ACZeroCrossing::Instance().setup();
}

void HeaterTriacControl::enable(bool enable)
{
	/* Nothing to do */
}

void HeaterTriacControl::setDutyCycle(unsigned char value)
{
	if (value == 100) {
		ACZeroCrossing::Instance().removeCallback(cbIdx);
		cbIdx = -1;
		digitalWrite(TRIAC_CONTROL_PIN, HIGH);
	} else if (value == 0) {
		ACZeroCrossing::Instance().removeCallback(cbIdx);
		cbIdx = -1;
		digitalWrite(TRIAC_CONTROL_PIN, LOW);
	} else {
		unsigned int periodUs = ACZeroCrossing::Instance().getAcPeriodUs();

		/* We need to control half-waves so divid the AC period by 2
		 * TODO: The scale is non linear since we chop a sine wave
		 * compute lookup table 
		 */
		triacTriggerTimeUs = (periodUs / 2) / 100;
		triacTriggerTimeUs *= (100 - value);

		if (cbIdx == -1)	
			cbIdx = ACZeroCrossing::Instance().addCallback(setTimerDutyCycle, this);
	}
}
