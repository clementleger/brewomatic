#include "Config.h"

#if ENABLED(TEMP_MAX31865)

#include "TempMax31865.h"

#define RREF 430.0

TempMax31865::TempMax31865():
mConverter(MAX31865_CS_PIN)
{
	 mConverter.begin(MAX31865_WIRE_COUNT);
	 mConverter.autoConvert(true);
}

int TempMax31865::getTemp(float *value)
{
	*value = mConverter.temperature(100, RREF);

	uint8_t fault = mConverter.readFault();
	if (fault) {
		mConverter.clearFault();
		return 1;
	}

	return 0;
}

#endif
