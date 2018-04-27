#include "HeaterTriacControl.h"
#include "ACZeroCrossing.h"
#include "SerialOutput.h"
#include "Arduino.h"
#include "Config.h"

#include <TimerOne.h>

#define TRIAC_GATE_KEEP_TIME_US	300

#define TIMER_PERIOD	100

/**
 * Control the triac gate by using a simple 
 */

uint8_t ticks = 0;
uint8_t startTriacTicks = 0;
uint8_t stopTriacTicks = 0;

void timerIt()
{
	ticks++;
	if (ticks == startTriacTicks) {
		/* Disable TRIAC gate */
		digitalWrite(TRIAC_CONTROL_PIN, HIGH);
	}

	if (ticks == stopTriacTicks) {
		digitalWrite(TRIAC_CONTROL_PIN, LOW);
		Timer1.stop();
	}
}

void setTimerDutyCycle(void *data)
{
	HeaterTriacControl *tc = (HeaterTriacControl *) data;
	uint8_t triagCycleTrigTime = ceil(TRIAC_GATE_KEEP_TIME_US / TIMER_PERIOD);

	/* If duty cycle is less than the timle we need to trigger the triac
	 * return */
	if (tc->mDutyCycle < triagCycleTrigTime)
		return;

	/* Activate TRIAC gate */
	digitalWrite(TRIAC_CONTROL_PIN, HIGH);
	/* Reset ticks */
	ticks = 0;
	startTriacTicks = 0;
	stopTriacTicks = startTriacTicks + triagCycleTrigTime;
	/* Start Timer */
	TCNT1 = 1;
	Timer1.resume();
}

HeaterTriacControl::HeaterTriacControl()
{
	pinMode(TRIAC_CONTROL_PIN, OUTPUT);
	digitalWrite(TRIAC_CONTROL_PIN, LOW);

	Timer1.initialize(TIMER_PERIOD);
	Timer1.attachInterrupt(timerIt);
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
