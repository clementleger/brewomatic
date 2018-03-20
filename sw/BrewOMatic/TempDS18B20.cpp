#include "Config.h"
#include "TempDS18B20.h"

#include <OneWire.h>

TempDS18B20::TempDS18B20():
oneWire(DS18B20_ONEWIRE_PIN),
mDs(&oneWire)
{
	mDs.begin();
	if (mDs.getDS18Count() != 1)
		return;
	if (!mDs.getAddress(dsAddr, 0))
		return;
}

int TempDS18B20::getTemp(float *value)
{
	mDs.requestTemperatures();
	*value = mDs.getTempC(dsAddr);

	return 0;
}
