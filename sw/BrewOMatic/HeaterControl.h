#ifndef _HEATER_CONTROL_H
#define _HEATER_CONTROL_H

class HeaterControl {
	public:
		virtual void enable(bool enable) = 0;
		virtual void setDutyCycle(unsigned char value) = 0;
};

#endif
