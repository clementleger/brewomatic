#include "HeaterControl.h"
#include "SerialOutput.h"
#include "Config.h"


HeaterControl::HeaterControl():
mCtrl(),
mEnable(0),
mPid(&mPidInput, &mPidOutput, &mTargetTemp, PID_P_VALUE, PID_I_VALUE, PID_D_VALUE, DIRECT)
{
	mPid.SetOutputLimits(0, 100);
}

void HeaterControl::setEnable(bool enable)
{
	if (enable) {
		if (mEnable)
			return;
		mPid.SetMode(AUTOMATIC);
		mCtrl.enable(true);
	} else {
		if (!mEnable)
			return;
		mPid.SetMode(MANUAL);
		mCtrl.enable(false);
	}

	mEnable = enable;
}

void HeaterControl::setTargetTemp(double targetTemp)
{
	mTargetTemp = targetTemp;
	
}

void HeaterControl::handleHeating(double currentTemp)
{
	int ret;

	mPidInput = currentTemp;
	ret = mPid.Compute();
	if (ret) {
		dbgOutput("PID output: %d\n", (int) mPidOutput);
		mCtrl.setDutyCycle((unsigned char) mPidOutput);
	}
}
