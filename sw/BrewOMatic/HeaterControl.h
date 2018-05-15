#ifndef _HEATER_CONTROL_H
#define _HEATER_CONTROL_H

#include <PID_v1.h>

#include "HeaterTriacControl.h"

class HeaterControl {
	public:
		HeaterControl(unsigned int sampleTime);
		void setEnable(bool enable);
		void setTargetTemp(double targetTemp);
		void handleHeating(double currentTemp);
		double mPidOutput;
	private:
		/* Fixme: use config */
		HeaterTriacControl mCtrl;
		unsigned char mEnable;
		double mTargetTemp;
		double mPidInput;
		PID mPid;
};

#endif
