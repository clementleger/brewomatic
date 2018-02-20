#ifndef AC_ZERO_CROSSING_H
#define AC_ZERO_CROSSING_H

#define MAX_CALLBACKS	5

typedef void (*zero_crossing_callback)(void *data);

class ACZeroCrossing
{
	public:
		static ACZeroCrossing& Instance(){ return m_instance; };
		addCallback(zero_crossing_callback cb, void *data);
		int getFrequency() { return acFrequency; };

	private:
		ACZeroCrossing();
		void computeFrequency();
		void classZeroCrossingInterrupt();

		static void zeroCrossingInterrupt() {
			m_instance.classZeroCrossingInterrupt();
		}

		/* Members */
		int acFrequency;
		static ACZeroCrossing m_instance;
		int pin;
		/* Callbacks */
		zero_crossing_callback callbacksFunc[MAX_CALLBACKS];
		void * callbacksData[MAX_CALLBACKS];
		int callbacksNr;
};

#endif
