#include "HeaterTriacControl.h"
#include "ACZeroCrossing.h"
#include "SerialOutput.h"
#include "Arduino.h"
#include "Config.h"

#include <TimerOne.h>

#define TRIAC_GATE_KEEP_TIME_US	300

#define TIMER_PERIOD		100

/**
 * Control the triac gate by using a simple 
 */

uint8_t ticks = 0;
uint8_t startTriacTicks = 0;
uint8_t stopTriacTicks = 0;

static void timerIt()
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

static void startTimer(uint8_t startTick, uint8_t stopTick)
{
	/* Reset ticks */
	ticks = 0;

	/* First tick will trigger right after timer setup,
	 * add 1 tick to each counter for correct time */
	startTriacTicks = startTick + 1;
	stopTriacTicks = stopTick + 1;

	/* Start Timer */
	TCNT1 = 0;
	Timer1.resume();
}

void setTimerDutyCycle(void *data)
{
	HeaterTriacControl *tc = (HeaterTriacControl *) data;

	/* When using a really low duty cycle (1)
	 * the end of previous timer could probably not have been reached.
	 * Hence, stop it right now */
	digitalWrite(TRIAC_CONTROL_PIN, LOW);

	/* Nothing to do, just return */
	if (tc->mDutyCycle == 0)
		return;

	/* Full power, enabled triac right now */
	if (tc->mDutyCycle == 100) {
		/* Activate TRIAC gate */
		digitalWrite(TRIAC_CONTROL_PIN, HIGH);
		/* Set start tick greater than stp tick to trigger stop tick only */
		startTimer(0xFF, tc->mTriacGateKeepTick);
	} else {
		startTimer(tc->mTriacEnableTick, tc->mTriacEnableTick + tc->mTriacGateKeepTick);
	}
}

HeaterTriacControl::HeaterTriacControl()
{
	unsigned long ACPeriodUs;
	unsigned long tickDuration;

	pinMode(TRIAC_CONTROL_PIN, OUTPUT);
	digitalWrite(TRIAC_CONTROL_PIN, LOW);

	ACZeroCrossing::Instance().setup();

	ACPeriodUs = ACZeroCrossing::Instance().getAcPeriodUs();
	if (ACPeriodUs == 0)
		return;

	tickDuration = ACPeriodUs / 2;

	/* Since the triac takes some time to trigger,
	 * we can't make all possible combination of variation
	 * Morover, add 10 since we can have some delay while executing */ 
	tickDuration -= (TRIAC_GATE_KEEP_TIME_US);
	tickDuration /= 100;

	mDutyCycle = 0;

	mTriacGateKeepTick = TRIAC_GATE_KEEP_TIME_US / tickDuration;

	Timer1.initialize(tickDuration);
	Timer1.attachInterrupt(timerIt);
	Timer1.stop();
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
	mDutyCycle = value;
	mTriacEnableTick = 100 - value;
}
