#ifndef _HEATER_CONTROL_H
#define _HEATER_CONTROL_H

#include <PID_v1.h>

#include "HeaterTriacControl.h"

class HeaterControl {
	public:
		HeaterControl();
		void setEnable(bool enable);
		void setTargetTemp(double targetTemp);
		void handleHeating(double currentTemp);
		double mPidOutput;
	private:
		/* Fixme: use config */
		HeaterTriacControl mCtrl;
		double mTargetTemp;
		double mPidInput;
		PID mPid;
		unsigned char mEnable;
};

#endif
