#include "BrewOMaticConfig.h"

#include "TempMax31865.h"
#include "TempDS18B20.h"
#include "HeaterTriacControl.h"

TempProbe *tempProbe = new TEMP_PROBE_TYPE();

HeaterControl *heaterControl = new HeaterTriacControl();

void setup()
{
	tempProbe->setup();
	heaterControl->setup();
	//~ display->setup();
	//~ display->dispBanner(WELCOME_STR);
}

void loop()
{
	
}
