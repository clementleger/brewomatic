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

Recipe *createDefaultRecipe()
{
	Recipe *recipe = new Recipe(6, "Default recipe");
	/* Heating water */
	Step *heating = new Step(STR_HEATING, 10, 26, false, 0);
	recipe->mSteps.addElem(heating);

	Action *insertMalt = new Action(STR_INSERT_MALT);
	Step *mashing1 = new Step(STR_MASHING, 30, 64, true, 1);
	mashing1->mPreStepAction = insertMalt;
	recipe->mSteps.addElem(mashing1);
	Step *mashing2 = new Step(STR_MASHING, 30, 68, true, 0);
	recipe->mSteps.addElem(mashing2);
	Step *mashing3 = new Step(STR_MASHING, 10, 72, true, 0);
	recipe->mSteps.addElem(mashing3);

	Action *removeMalt = new Action(STR_REMOVE_MALT);
	Step *boiling = new Step(STR_BOILING, 0, 100, false, 1);
	boiling->mPreStepAction = removeMalt;
	recipe->mSteps.addElem(boiling);

	Action *cooler = new Action(STR_SETUP_COOLER);
	Step *cooling = new Step(STR_COOLING, 0, 20, false, 1);
	cooling->mPreStepAction = cooler;
	recipe->mSteps.addElem(cooling);

	return recipe;
}

Step *createManualStep()
{
	return new Step(STR_MANUAL_MODE, 0, 25, false, 0);
}
