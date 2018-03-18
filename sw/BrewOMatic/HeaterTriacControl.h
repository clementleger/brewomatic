#ifndef _HEATER_TRIAC_CONTROL_H
#define _HEATER_TRIAC_CONTROL_H

#include "HeaterControl.h"

class HeaterTriacControl: public HeaterControl {
	public:
		HeaterTriacControl();
		void enable(bool enable);
		void setDutyCycle(unsigned char value);
		unsigned int getTriacTriggerTimeUs() { return triacTriggerTimeUs; };
	private:
		int cbIdx;
		unsigned int triacTriggerTimeUs;
};

#endif
