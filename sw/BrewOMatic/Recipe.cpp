#include "Recipe.h"
#include "BrewOMatic.h"

Step::Step(brewStringIndex name, unsigned long duration,
	   unsigned char targetTemp, bool enablePump,
	   unsigned char actionCount):
mEnablePump(enablePump),
mTargetTemp(targetTemp),
mDuration(duration),
mName(name),
mPreStepAction(NULL),
mUserActions(actionCount)
{
}

Recipe::Recipe(unsigned char stepCount, const char *name):
mSteps(stepCount),
mName(strdup(name))
{

}

Recipe::~Recipe()
{
	Step *step;

	delete mName;
	mSteps.reset();
	while ((step = mSteps.getNextElem())) {
		delete step;
	}
}

Step::~Step()
{
	Action *act;

	if (mPreStepAction)
		delete mPreStepAction;

	mUserActions.reset();
	while ((act = mUserActions.getNextElem())) {
		delete act;
	}

}

Step *createManualStep()
{
	return new Step(STR_MANUAL_MODE, 0, 25, false, 0);
}
