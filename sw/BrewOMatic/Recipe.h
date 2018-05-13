#ifndef _RECIPE_H
#define _RECIPE_H

#define MAX_STEP_COUNT		10
#define MAX_USER_ACTION_COUNT	4

#include "Language.h"
#include "List.h"

class Action {
	public:
		Action(brewStringIndex descIdx, const unsigned long time):
		mDescIdx(descIdx),
		mTime(time) {};

		Action(brewStringIndex descIdx):
		mDescIdx(descIdx) {};

		brewStringIndex getDescIdx() {return mDescIdx;};
	private:
		brewStringIndex mDescIdx;
		const unsigned long mTime;
};

class Step {
	public:
		Step();
		Step(brewStringIndex name, unsigned long duration,
		     unsigned char targetTemp, bool enablePump,
		     unsigned char actionCount);

		bool mEnablePump;
		unsigned char mTargetTemp;
		unsigned long mDuration;
		brewStringIndex mName;

		/* Action to be run before executing the step */
		Action *mPreStepAction;
		/* List of user action requiring confirmation
		 * during brewing */
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
