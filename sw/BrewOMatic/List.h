#ifndef _LIST_H
#define _LIST_H

template<typename T>
class List {
	public:
		List(int maxElems) {
			mCurrentElem = 0;
			mElemCount = 0;
			mMaxElems = maxElems;
			mElems = new T[maxElems];
		};
		
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

#endif
