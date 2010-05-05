#ifndef _INC_Array_H
#define _INC_Array_H

/// Represents a generic Array
template<class T, unsigned SIZE>
class Array {

public:
	/// Array iterator
	class Iterator {
		friend class Array<T, SIZE> ;

	private:
		/// Index of element
		unsigned mIndex;

		/// Constructor
		Iterator(const unsigned index) :
			mIndex(index) {
		}

	public:
		/// Destructor
		~Iterator() {
		}

		/// Comparison operator
		bool operator ==(const Iterator& it) const {
			return mIndex == it.mIndex;
		}
		/// Comparison operator
		bool operator !=(const Iterator& it) const {
			return mIndex != it.mIndex;
		}

		/// Moves to next item
		Iterator operator ++() {
			++mIndex;
			return (*this);
		}
		/// Moves to next item
		Iterator operator ++(int) {
			return Iterator(mIndex++);
		}
		/// Moves to previous item
		Iterator operator --() {
			--mIndex;
			return (*this);
		}
		/// Moves to previous item
		Iterator operator --(int) {
			return Iterator(mIndex--);
		}
	};

	/// Constructor
	Array() :
		mCount(0) {
	}

	/// Destructor
	~Array() {
	}

	/// Returns the size of the list
	int Count() const {
		return mCount;
	}

	/// Clears the Array
	void Clear() {
		mCount = 0;
	}

	/// Checks if Array is empty
	bool IsEmpty() const {
		return !mCount;
	}

	/// Checks if Array is full
	bool IsFull() const {
		return mCount >= SIZE;
	}

	/// Returns iterator that points to the first item in the Array
	Iterator First() const {
		return Iterator(0);
	}

	/// Returns iterator that points to the last item in the Array
	Iterator Last() const {
		return Iterator(mCount - 1);
	}

	/// Returns iterator that represents the end of the Array
	Iterator End() const {
		return Iterator(mCount);
	}

	/// Appends an item to the back of the Array
	/// @param item Item to append
	void Append(const T& item) {
		assert(!IsFull());
		mData[mCount++] = item;
	}

	/// Inserts an item on a specified position
	/// @param index Position to insert at
	/// @param item Item to insert
	void Insert(const Iterator it, const T& item) {
		assert(!IsFull());
		assert(it.mIndex <= mCount);
		for (unsigned i = mCount; i > it.mIndex; --i) {
			mData[i] = mData[i - 1];
		}
		mCount++;
		mData[it.mIndex] = item;
	}

	/// Removes an item on a specified position
	/// @param index Position to remove at
	void RemoveAt(const Iterator it) {
		assert(it.mIndex < mCount);
		for (unsigned i = it.mIndex + 1; i < mCount; ++i) {
			mData[i - 1] = mData[i];
		}
		mCount--;
	}

	/// Finds first occurrence of an item
	/// @param item Item to find
	/// @returns Iterator pointing to found item or End if not found
	Iterator FindFirst(const T& item) const {
		for (unsigned i = 0; i < mCount; ++i) {
			if (mData[i] == item) {
				return Iterator(i);
			}
		}
		return Iterator(mCount);
	}

	/// @param it Iterator
	/// @returns Constant item at iterator position
	T& operator[](const Iterator& it) {
		assert(it.mIndex < mCount);
		return mData[it.mIndex];
	}

	/// @param it Iterator
	/// @returns An item at iterator position
	const T& operator[](const Iterator& it) const {
		assert(it.mIndex < mCount);
		return mData[it.mIndex];
	}

private:
	/// Items count
	unsigned mCount;
	/// Data buffer
	T mData[SIZE];
};

#endif // _INC_Array_H
