#include "Arduino.h"

#include "Config.h"

void pumpSet(int value)
{
	unsigned char percent = map(value, 0, 100, 0, 255);
	analogWrite(PUMP_CONTROL_PIN, percent);
}
