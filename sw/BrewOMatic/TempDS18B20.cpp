#include "Config.h"
#include "TempDS18B20.h"

#include <OneWire.h>

OneWire oneWire(DS18B20_ONEWIRE_PIN);

int TempDS18B20::setup()
{
	
	ds = new DallasTemperature(&oneWire);
	ds->begin();
	if (ds->getDS18Count() != 1)
		return 1;
	if (!ds->getAddress(dsAddr, 0))
		return 1;

	return 0;
}

int TempDS18B20::getTemp(float *value)
{
	ds->requestTemperatures();
	*value = ds->getTempC(dsAddr);

	return 0;
}
