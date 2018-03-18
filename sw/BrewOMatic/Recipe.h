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
		Recipe(unsigned int stepCount, const char *name);

		List<Step *> mSteps;
		const char *mName;
};

Recipe *createDefaultRecipe();

#endif
