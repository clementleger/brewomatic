#ifndef _TEMP_DS18B20_H
#define _TEMP_DS18B20_H
#include "Config.h"

#if ENABLED(TEMP_DS18B20)

#include "TempProbe.h"

#include <DallasTemperature.h>

class TempDS18B20: public TempProbe
{
	public:
		TempDS18B20();
		int getTemp(float *value);

	private:
		OneWire oneWire;
		DallasTemperature mDs;
		DeviceAddress dsAddr;

};

#endif

#endif
