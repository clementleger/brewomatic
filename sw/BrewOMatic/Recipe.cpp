#include "Recipe.h"



template<typename T>
T List<T>::getNextElem()
{
	T elem;
	if (mCurrentElem == mElemCount)
		return 0;

	elem = mElems[mCurrentElem];
	mCurrentElem++;

	return elem;
}

template<typename T>
List<T>::List(int maxElems) {
	mCurrentElem = 0;
	mElemCount = 0;
	mMaxElems = maxElems;

	mElems = new T[maxElems];
	
}

template<typename T>
bool List<T>::addElem(T step)
{
	if (mElemCount == MAX_STEP_COUNT)
		return false;

	mElems[mElemCount] = step;
	mElemCount++;

	return true;
}

Step::Step():
mUserActions(MAX_USER_ACTION_COUNT)
{
	
}

Recipe::Recipe():
mSteps(MAX_STEP_COUNT)
{
	
}
