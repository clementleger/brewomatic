#ifndef _RECIPE_H
#define _RECIPE_H

#define MAX_STEP_COUNT		10
#define MAX_USER_ACTION_COUNT	4

template<typename T>
class List {
	public:
		List(int maxElems);
		T getNextElem();
		bool hasNextElem() { return mCurrentElem < mElemCount;};
		void reset() {mCurrentElem = 0;};
		bool isEmpty() {return mElemCount == 0;};
		bool addElem(T elem);

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
		 * Get step name
		 * @return Step name
		 */
		const char *getName() { return mName;};

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
		Recipe();
		List<Step *> getSteps() {return mSteps;};
	private:
		List<Step *> mSteps;
};

#endif
