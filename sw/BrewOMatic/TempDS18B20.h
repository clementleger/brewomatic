#ifndef _TEMP_DS18B20_H
#define _TEMP_DS18B20_H

#include "TempProbe.h"

#include <DallasTemperature.h>

class TempDS18B20: public TempProbe
{
	public:
		int setup();
		int getTemp(float *value);

	private:
		DallasTemperature *ds;
		DeviceAddress dsAddr;
};

#endif
