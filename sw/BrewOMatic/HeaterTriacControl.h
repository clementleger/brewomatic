#ifndef _HEATER_TRIAC_CONTROL_H
#define _HEATER_TRIAC_CONTROL_H

#include "HeaterInterface.h"
#include <Arduino.h>

class HeaterTriacControl: public HeaterInterface {
	public:
		HeaterTriacControl();
		void enable(bool enable);
		void setDutyCycle(unsigned char value);

		uint8_t mTriacEnableTick;
		uint8_t mTriacGateKeepTick;
		uint8_t mDutyCycle;
	private:
};

#endif
