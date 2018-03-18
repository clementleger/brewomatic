#include "Recipe.h"


template<typename T>
List<T>::List(int maxElems) {
	mCurrentElem = 0;
	mElemCount = 0;
	mMaxElems = maxElems;

	mElems = new T[maxElems];
}


Step::Step():
mUserActions(MAX_USER_ACTION_COUNT)
{
	
}

Step::Step(const char *name, unsigned long duration,
	   unsigned long targetTemp, bool enablePump,
	   bool enableHeater,
	   unsigned int actionCount):
mEnablePump(enablePump),
mEnableHeater(enableHeater),
mTargetTemp(targetTemp),
mDuration(duration),
mName(name),
mUserActions(actionCount)
{
	
}

Recipe::Recipe(unsigned int stepCount, const char *name):
mSteps(stepCount),
mName(name)
{
	
}

Recipe *createDefaultRecipe()
{
	Recipe *recipe = new Recipe(6, "Default recipe");
	/* Heating water */
	Step *heating = new Step("Heating", 0, 50, false, true, 0);
	recipe->mSteps.addElem(heating);

	/* Add action: insert malt */
	Step *mashing1 = new Step("Mashing 1", 30, 63, true, true, 1);
	recipe->mSteps.addElem(mashing1);
	Step *mashing2 = new Step("Mashing 2", 30, 68, true, true, 0);
	recipe->mSteps.addElem(mashing2);
	Step *mashing3 = new Step("Mashing 3", 15, 70, true, true, 0);
	recipe->mSteps.addElem(mashing3);

	/* Add action: remove malt before boiling */
	Step *boiling = new Step("Boiling", 0, 100, false, true, 1);
	recipe->mSteps.addElem(boiling);

	/* Add action: insert chiller for cooling */
	Step *cooling = new Step("Cooling", 0, 20, false, false, 1);
	recipe->mSteps.addElem(cooling);

	return recipe;
}
