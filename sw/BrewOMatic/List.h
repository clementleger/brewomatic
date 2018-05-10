#ifndef _LIST_H
#define _LIST_H

template<typename T>
class List {
	public:
		List(int maxElems) {
			mElems = new T[maxElems];
			mCurrentElem = 0;
			mElemCount = 0;
			mMaxElems = maxElems;
		};
		
		bool hasNextElem() { return mCurrentElem < mElemCount;};
		void reset() {mCurrentElem = 0;};
		bool isEmpty() {return mElemCount == 0;};

		void addElem(T step)
		{
			mElems[mElemCount] = step;
			mElemCount++;
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

		unsigned char mElemCount;
		T *mElems;

	private:
		unsigned char mCurrentElem;
		unsigned char mMaxElems;
};

#endif
