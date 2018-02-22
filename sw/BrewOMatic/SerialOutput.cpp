#include "SerialOutput.h"

int SerialOutput::setup(BrewOMatic *b)
{
	Serial.begin(SERIAL_BAUDRATE);
	b->addObserver(this);
	return 0;
}

void SerialOutput::updateStatus(BrewOMatic *b, const char *newStatus)
{
	Serial.print("STATUS:");
	Serial.println(newStatus);
}

void SerialOutput::updateBrewingStep(BrewOMatic *b, const char *newStep)
{
	Serial.print("BREWING_STEP:");
	Serial.println(newStep);
}

void SerialOutput::updateTemperatureSetpoint(BrewOMatic *b, unsigned int value)
{
	Serial.print("TEMPERATURE_SETPOINT:");
	Serial.println(value);
}

void SerialOutput::updateTemperature(BrewOMatic *b, unsigned int temp)
{
	Serial.print("TEMPERATURE:");
	Serial.println(temp);
}
