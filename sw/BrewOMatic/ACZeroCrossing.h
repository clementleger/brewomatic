#ifndef AC_ZERO_CROSSING_H
#define AC_ZERO_CROSSING_H

#define MAX_CALLBACKS	5

typedef void (*zeroCrossingCallback)(void *data);

class ACZeroCrossing
{
	public:
		static ACZeroCrossing& Instance(){ return m_instance; };
		void setup();
		int addCallback(zeroCrossingCallback cb, void *data);
		void removeCallback(int cbIdx);
		int getFrequency() { return acFrequency; };
		int getAcPeriodUs() { return acPeriodUs; };

	private:
		ACZeroCrossing();
		void computeFrequency();
		void classZeroCrossingInterrupt();

		static void zeroCrossingInterrupt() {
			m_instance.classZeroCrossingInterrupt();
		}

		/* Members */
		int acFrequency;
		unsigned int acPeriodUs;
		static ACZeroCrossing m_instance;
		int pin;
		/* Callbacks */
		zeroCrossingCallback callbacksFunc[MAX_CALLBACKS];
		void * callbacksData[MAX_CALLBACKS];

		int findFreeIdx();
};

#endif
