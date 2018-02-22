#include "Config.h"

#if TEMP_PROBE_TYPE == MAX31865
#include <Adafruit_MAX31865.h>
#include <TimerOne.h>
#elif TEMP_PROBE_TYPE == TempDS18B20
#include <OneWire.h>
#include <DallasTemperature.h>
#endif
