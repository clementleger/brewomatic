#ifndef _BREWOMATIC_H
#define _BREWOMATIC_H

#include "Config.h"

#define STRINGIFY_(M) #M
#define STRINGIFY(M) STRINGIFY_(M)

#define LANGUAGE_INCL_(M) STRINGIFY_(lang/Lang##M.h)
#define LANGUAGE_INCL(M) LANGUAGE_INCL_(M)
#define INCLUDE_LANGUAGE LANGUAGE_INCL(DISP_LANGUAGE)

#define MAX_OBSERVERS	4

class BrewOMatic {

	public:
		enum status {
			INIT,	/* System is starting initialization */
			IDLE	/* System is idle */
		};

		float getTemperatureSetpoint() { return mTemperatureSetpoint;};
		enum status getStatus() { return mStatus; };

	private:
		enum status mStatus;
		float mTemperatureSetpoint;
		
};

class BrewOMaticObserver {
	public:
		enum updatedValue {
			STATUS,
			TEMPERATURE_SETPOINT,
		};

		void update(BrewOMatic *b, enum updatedValue val);

};


#endif
