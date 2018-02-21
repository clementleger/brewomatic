#include "Config.h"
#include "HeaterTriacControl.h"
#include "ACZeroCrossing.h"
#include "Arduino.h"

void setTimerDutyCycle(void *data)
{

}

int HeaterTriacControl::setup()
{
	ACZeroCrossing::Instance().addCallback(setTimerDutyCycle, this);
	pinMode(TRIAC_CONTROL_PIN, OUTPUT);
	digitalWrite(TRIAC_CONTROL_PIN, LOW);
}

void HeaterTriacControl::enable(bool enable)
{
	/* Nothing to do */
}

void HeaterTriacControl::setDutyCycle(unsigned char value)
{
	if (value == 255) {
		digitalWrite(TRIAC_CONTROL_PIN, HIGH);
	} else if (value == 0) {
		digitalWrite(TRIAC_CONTROL_PIN, LOW);
	} else {
		dutyCycle = value;
	}
}
