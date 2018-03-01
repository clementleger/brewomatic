#include "Recipe.h"


BrewingStep::BrewingStep()
{
	
}

BrewingStep *Recipe::getNextBrewingStep()
{
	BrewingStep *step;
	if (mCurrentStep == mStepsCount)
		return 0;

	step = steps[mCurrentStep];
	mCurrentStep++;

	return step;
}



Recipe::Recipe()
{
	mCurrentStep = 0;
	mStepsCount = 0;
}
