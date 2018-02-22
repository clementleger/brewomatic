#include "Config.h"
#include "TempMax31865.h"

#define RREF 430.0

int TempMax31865::setup()
{
	 max_converter = new Adafruit_MAX31865(MAX31865_CS_PIN);
	 return max_converter->begin(MAX31865_WIRE_COUNT);
}

int TempMax31865::getTemp(float *value)
{
	*value = max_converter->temperature(100, RREF);

	uint8_t fault = max_converter->readFault();
	if (fault) {
		max_converter->clearFault();
		return 1;
	}

	return 0;
}
