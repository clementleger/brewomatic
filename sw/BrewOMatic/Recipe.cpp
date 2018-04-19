#include "Recipe.h"
#include "BrewOMatic.h"

Step::Step():
mUserActions(MAX_USER_ACTION_COUNT)
{
	
}

Step::Step(const char *name, unsigned long duration,
	   unsigned char targetTemp, bool enablePump,
	   bool enableHeater,
	   unsigned char actionCount):
mEnablePump(enablePump),
mEnableHeater(enableHeater),
mTargetTemp(targetTemp),
mDuration(duration),
mName(name),
mStarted(0),
mUserActions(actionCount)
{
}

Recipe::Recipe(unsigned char stepCount, const char *name):
mSteps(stepCount),
mName(name)
{
	
}

Recipe *createDefaultRecipe()
{
	Recipe *recipe = new Recipe(6, "Default recipe");
	/* Heating water */
	Step *heating = new Step("Heating", 10, 26, false, true, 0);
	recipe->mSteps.addElem(heating);

	/* Add action: insert malt */
	Step *mashing1 = new Step("Mashing", 30, 64, true, true, 1);
	recipe->mSteps.addElem(mashing1);
	Step *mashing2 = new Step("Mashing", 30, 68, true, true, 0);
	recipe->mSteps.addElem(mashing2);
	Step *mashing3 = new Step("Mashing", 10, 72, true, true, 0);
	recipe->mSteps.addElem(mashing3);

	/* Add action: remove malt before boiling */
	Step *boiling = new Step("Boiling", 0, 100, false, true, 1);
	recipe->mSteps.addElem(boiling);

	/* Add action: insert chiller for cooling */
	Step *cooling = new Step("Cooling", 0, 20, false, false, 1);
	recipe->mSteps.addElem(cooling);

	return recipe;
}
