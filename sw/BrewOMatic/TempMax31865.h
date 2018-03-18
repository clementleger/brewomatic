#ifndef _TEMP_MAX31865_H
#define _TEMP_MAX31865_H

#include "TempProbe.h"

#include <Adafruit_MAX31865.h>

class TempMax31865: public TempProbe
{
	public:
		TempMax31865();
		int getTemp(float *value);

	private:
		Adafruit_MAX31865 mConverter;
};

#endif
