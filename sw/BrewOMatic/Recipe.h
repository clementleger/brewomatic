#ifndef _RECIPE_H
#define _RECIPE_H

#define MAX_STEP_COUNT		10
#define MAX_USER_ACTION_COUNT	4

#include "List.h"

class Action {
	public:
		const char *getDescription() {return mDescription;};
	private:
		const char *mDescription;
};

class Step {
	public:
		Step();
		Step(const char *name, unsigned long duration,
		     unsigned char targetTemp, bool enablePump,
		     bool enableHeater, unsigned char actionCount);

		bool mEnablePump;
		bool mEnableHeater;
		unsigned char mTargetTemp;
		unsigned long mDuration;
		const char *mName;
		unsigned char mStarted;

		/* List of user action requiring confirmation */
		List<Action *> mUserActions;
};

class Recipe {
	public:
		Recipe(unsigned char stepCount, const char *name);

		List<Step *> mSteps;
		const char *mName;
};

Recipe *createDefaultRecipe();
Step *createManualStep();

#endif
