#ifndef _HEATER_TRIAC_CONTROL_H
#define _HEATER_TRIAC_CONTROL_H

#include "HeaterInterface.h"

class HeaterTriacControl: public HeaterInterface {
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
