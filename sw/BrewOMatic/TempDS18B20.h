#ifndef _TEMP_DS18B20_H
#define _TEMP_DS18B20_H

#include "TempProbe.h"

#include <DallasTemperature.h>

class TempDS18B20: public TempProbe
{
	public:
		TempDS18B20();
		int getTemp(float *value);

	private:
		DallasTemperature mDs;
		DeviceAddress dsAddr;
		OneWire oneWire;

};

#endif
