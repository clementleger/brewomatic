#ifndef _HEATER_INTERFACE_H
#define _HEATER_INTERFACE_H

class HeaterInterface {
	public:
		virtual void enable(bool enable) = 0;
		virtual void setDutyCycle(unsigned char value) = 0;
};

#endif
