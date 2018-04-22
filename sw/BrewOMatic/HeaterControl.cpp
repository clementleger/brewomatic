#include "HeaterControl.h"
#include "SerialOutput.h"
#include "Config.h"


HeaterControl::HeaterControl():
mCtrl(),
mPid(&mPidInput, &mPidOutput, &mTargetTemp, PID_P_VALUE, PID_I_VALUE, PID_D_VALUE, DIRECT)
{
	mPid.SetOutputLimits(0, 100);
}

void HeaterControl::setEnable(bool enable)
{
	if (enable) {
		mPid.SetMode(AUTOMATIC);
		mCtrl.enable(true);
	} else {
		mPid.SetMode(MANUAL);
		mCtrl.enable(false);
	}
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
	if (ret)
		dbgOutput("PID output: %d\n", mPidOutput);
}
