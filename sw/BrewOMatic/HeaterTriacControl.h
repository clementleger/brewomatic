#ifndef _HEATER_TRIAC_CONTROL_H
#define _HEATER_TRIAC_CONTROL_H

#include "HeaterInterface.h"

class HeaterTriacControl: public HeaterInterface {
	public:
		HeaterTriacControl();
		void enable(bool enable);
		void setDutyCycle(unsigned char value);

		unsigned long mTriacGateKeepTimeUs;
		unsigned char mDutyCycle;
		unsigned long mACPeriodUs;
	private:
};

#endif
