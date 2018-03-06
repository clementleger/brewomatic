#ifndef _RECIPE_H
#define _RECIPE_H

#define MAX_STEP_COUNT		10
#define MAX_USER_ACTION_COUNT	4

template<typename T>
class List {
	public:
		List(int maxElems);
		
		bool hasNextElem() { return mCurrentElem < mElemCount;};
		void reset() {mCurrentElem = 0;};
		bool isEmpty() {return mElemCount == 0;};

		bool addElem(T step)
		{
			if (mElemCount == mMaxElems)
				return false;

			mElems[mElemCount] = step;
			mElemCount++;

			return true;
		};

		T getNextElem()
		{
			T elem;
			if (mCurrentElem == mElemCount)
				return 0;

			elem = mElems[mCurrentElem];
			mCurrentElem++;

			return elem;
		}

	private:
		T *mElems;
		unsigned int mElemCount;
		unsigned int mCurrentElem;
		unsigned int mMaxElems;
};

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
		     unsigned long targetTemp, bool enablePump,
		     bool enableHeater, unsigned int actionCount);

		/**
		 * Get step duration in mintue
		 * @return step duration
		 */
		unsigned long getDuration() { return mDuration;};

		/**
		 * Get step duration in mintue
		 * @return step duration
		 */
		void setDuration(unsigned long duration) { mDuration = duration;};

		/**
		 * Return targeted temperature
		 * @return targeted temperature
		 */
		unsigned long getTargetTemp() { return mTargetTemp;};

		/**
		 * Return targeted temperature
		 * @return targeted temperature
		 */
		void setTargetTemp(unsigned long targetTemp) { mTargetTemp = targetTemp;};

		/**
		 * Should the controller enable the pump when doing this step
		 * @return a boolean, true if pump should be enabled
		 */
		bool getEnablePump() { return mEnablePump;};

		/**
		 * Should the controller enable the pump when doing this step
		 * @return a boolean, true if pump should be enabled
		 */
		void setEnablePump(bool enablePump) {  mEnablePump = enablePump;};

		/**
		 * Should the controller enable the heater when doing this step
		 * @return a boolean, true if heater should be enabled
		 */
		bool getEnableHeater() { return mEnableHeater;};

		/**
		 * Should the controller enable the heater when doing this step
		 * @return a boolean, true if heater should be enabled
		 */
		void setEnableHeater(bool enableHeater) { mEnableHeater = enableHeater;};

		/**
		 * Get step name
		 * @return Step name
		 */
		const char *getName() { return mName;};

		/**
		 * Get step name
		 * @return Step name
		 */
		void setName(const char *name) { mName = name;};

		/**
		 * Get the list of user actions to execute before running
		 * the step.
		 */
		List<Action *> getUserActions() {return mUserActions;};

	private:
		bool mEnablePump;
		bool mEnableHeater;
		unsigned long mTargetTemp;
		unsigned long mDuration;
		const char *mName;

		/* List of user action requiring confirmation */
		List<Action *> mUserActions;
};

class Recipe {
	public:
		Recipe(unsigned int stepCount);
		List<Step *> getSteps() {return mSteps;};
	private:
		List<Step *> mSteps;
};

Recipe *createDefaultRecipe();

#endif
