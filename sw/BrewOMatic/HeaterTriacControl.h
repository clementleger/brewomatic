#ifndef _HEATER_TRIAC_CONTROL_H
#define _HEATER_TRIAC_CONTROL_H

#include "HeaterControl.h"

class HeaterTriacControl: public HeaterControl {
	public:
		HeaterTriacControl();
		void enable(bool enable);
		void setDutyCycle(unsigned char value);

		unsigned char mPulseCount;
		unsigned char mPulseSkipCount;
		unsigned char mFrequency;
	private:
		int mCbIdx;
};

#endif
