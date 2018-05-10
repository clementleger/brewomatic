#ifndef AC_ZERO_CROSSING_H
#define AC_ZERO_CROSSING_H

#define MAX_CALLBACKS	5

typedef void (*zeroCrossingCallback)(void *data);

class ACZeroCrossing
{
	public:
		static ACZeroCrossing& Instance(){ return mInstance; };
		void setup();
		void setCallback(zeroCrossingCallback cb, void *data);
		void removeCallback();
		int getFrequency() { return acFrequency; };
		int getAcPeriodUs() { return acPeriodUs; };

	private:
		ACZeroCrossing();
		void computeFrequency();

		static void zeroCrossingInterrupt() {
			if (mInstance.mCallbackFunc) {
				mInstance.mCallbackFunc(mInstance.mCallbackData);
			}
		}

		/* Members */
		int acFrequency;
		unsigned int acPeriodUs;
		static ACZeroCrossing mInstance;

		/* Callbacks */
		zeroCrossingCallback mCallbackFunc;
		void *mCallbackData;
};

#endif
