#ifndef _RECIPE_H
#define _RECIPE_H

#define MAX_STEP_COUNT	10

class BrewingStep {
	public:
		BrewingStep();
		/**
		 * Get step duration in mintue
		 */
		unsigned long getDuration() { return mDuration;};

		/**
		 * Return targeted temperature
		 * @return targeted temperature
		 */
		unsigned long getTargetTemp() { return mTargetTemp;};

		/**
		 * Should the controller enable the pump when doing this step
		 * @return a boolean, true if pump should be enabled
		 */
		bool getEnablePump() { return mEnablePump;};

		/**
		 * Should the controller enable the heater when doing this step
		 * @return a boolean, true if heater should be enabled
		 */
		bool getEnableHeater() { return mEnableHeater;};

		/**
		 * Should the controller enable the pump when doing this step
		 * @return a boolean, true if pump should be enabled
		 */
		bool getWaitAfterStep() { return mWaitAfterStep;};

		/**
		 * Get step name
		 * @return Step name
		 */
		const char *getName() { return mName;};

	private:
		bool mWaitAfterStep;
		bool mEnablePump;
		bool mEnableHeater;
		unsigned long mTargetTemp;
		unsigned long mDuration;
		const char *mName;
};

/**
 * All times are in minutes
 */

class Recipe {
	public:
		Recipe();
		BrewingStep *getNextBrewingStep();
		bool hasNextBrewingStep() { return mCurrentStep < mStepsCount;};
		void resetRecipe() {mCurrentStep = 0;};

		bool addBrewingStep(BrewingStep *step);

	private:
		BrewingStep *steps[MAX_STEP_COUNT];
		unsigned int mStepsCount;
		unsigned int mCurrentStep;
};

#endif
