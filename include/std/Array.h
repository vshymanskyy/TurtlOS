#ifndef _INC_Array_H
#define _INC_Array_H

#ifndef __cplusplus
#error "This C++ header was included in C file"
#endif

/// Represents a generic Array
template <class T, unsigned SIZE>
class Array {

public:
	/// Array iterator
    class It {
		friend class Array<T, SIZE> ;

	private:
		/// Index of element
		unsigned mIndex;

		/// Constructor
        It(const unsigned index) :
			mIndex(index) {
		}

	public:
		/// Destructor
        ~It() {
		}

		/// Comparison operator
        bool operator ==(const It& it) const {
			return mIndex == it.mIndex;
		}
		/// Comparison operator
        bool operator !=(const It& it) const {
			return mIndex != it.mIndex;
		}

		/// Moves to next item
        It operator ++() {
			++mIndex;
			return (*this);
		}

		/// Moves to previous item
        It operator --() {
			--mIndex;
			return (*this);
		}

	};

	/// Constructor
	Array() :
        _count(0) {
	}

	/// Destructor
	~Array() {
	}

	/// Returns the size of the list
	int Count() const {
        return _count;
	}

	/// Clears the Array
	void Clear() {
        _count = 0;
	}

	/// Checks if Array is empty
	bool IsEmpty() const {
        return !_count;
	}

	/// Checks if Array is full
	bool IsFull() const {
        return _count >= SIZE;
	}

	/// Returns iterator that points to the first item in the Array
    It First() const {
        return It(0);
	}

	/// Returns iterator that points to the last item in the Array
    It Last() const {
        return It(_count - 1);
	}

	/// Returns iterator that represents the end of the Array
    It End() const {
        return It(_count);
	}

	/// Appends an item to the back of the Array
	/// @param item Item to append
	void Append(const T& item) {
		assert(!IsFull());
        _data[_count++] = item;
	}

	/// Inserts an item on a specified position
	/// @param index Position to insert at
	/// @param item Item to insert
    void Insert(const It it, const T& item) {
		assert(!IsFull());
        assert(it.mIndex <= _count);
        for (unsigned i = _count; i > it.mIndex; --i) {
            _data[i] = _data[i - 1];
		}
        _count++;
        _data[it.mIndex] = item;
	}

	/// Removes an item on a specified position
	/// @param index Position to remove at
    void RemoveAt(const It it) {
        assert(it.mIndex < _count);
        for (unsigned i = it.mIndex + 1; i < _count; ++i) {
            _data[i - 1] = _data[i];
		}
        _count--;
	}

	/// Finds first occurrence of an item
	/// @param item Item to find
	/// @returns Iterator pointing to found item or End if not found
    It FindFirst(const T& item) const {
        for (unsigned i = 0; i < _count; ++i) {
            if (_data[i] == item) {
                return It(i);
			}
		}
        return It(_count);
	}

	/// @param it Iterator
	/// @returns Constant item at iterator position
    T& operator[](const It& it) {
        assert(it.mIndex < _count);
        return _data[it.mIndex];
	}

	/// @param it Iterator
	/// @returns An item at iterator position
    const T& operator[](const It& it) const {
        assert(it.mIndex < _count);
        return _data[it.mIndex];
	}

private:
	/// Items count
    unsigned _count;
	/// Data buffer
    T _data[SIZE];
};

#endif // _INC_Array_H
