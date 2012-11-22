#ifndef _X_LIST_H_
#define _X_LIST_H_

#ifndef __cplusplus
#error "This C++ header was included in C file"
#endif

//#include <XDebug.h>
#include <std/Helpers.h>

#define X_ASSERT assert
#define X_DBG(expr) expr

/// Represents a generic double-linked list
template<class T>
class List
{
private:
	/// List node
	struct Node
	{
		/// Pointer to previous item
		Node *mPrev;
		/// Pointer to next item
		Node *mNext;
		/// The data
		T mData;

		/// Constructor
		explicit Node(const T& data, Node* prev, Node* next)
				: mPrev(prev), mNext(next), mData(data)
		{
		}
	};

	/// Node at the end of the list
	struct EndNode
	{
		/// Pointer to last item
		Node *mLast;
		/// Pointer to first item
		Node *mFirst;

		/// Constructor
		EndNode()
				: mLast((Node*) this), mFirst((Node*) this)
		{
		}
	};

public:
	/// List iterator
	class It
	{

		friend class List<T> ;

	private:
		/// Pointer to node
		Node* mNode;

	public:
		/// Constructor
		explicit It(Node* node)
				: mNode(node)
		{
		}

		/// Destructor
		~It()
		{
		}

		/// Comparison operator
		bool operator ==(const It& it) const
		{
			return mNode == it.mNode;
		}
		/// Comparison operator
		bool operator !=(const It& it) const
		{
			return mNode != it.mNode;
		}

		/// Moves to next item
		It& operator ++()
		{
			mNode = mNode->mNext;
			return (*this);
		}
		/// Moves to previous item
		It& operator --()
		{
			mNode = mNode->mPrev;
			return (*this);
		}

		/// Moves to next item
		It operator +(int i) const
		{
			It res = *this;
			while (i--) {
				res.mNode = res.mNode->mNext;
			}
			return res;
		}
		/// Moves to previous item
		It operator -(int i) const
		{
			It res = *this;
			while (i--) {
				res.mNode = res.mNode->mPrev;
			}
			return res;
		}

		//T& operator *() { return _node->_data; }
		//T* operator ->() { return &(_node->_data); }
	};

public:
	/// Constructor
	List()
			: mCount(0)
	{
	}

	/// Copy constructor
	List(const List& lst);

	/// Assignment operator
	List& operator =(const List& lst);

	/// Destructor
	~List()
	{
		Clear();
	}

	/// Returns the size of the list
	unsigned Count() const
	{
		return mCount;
	}

	/// Checks if list is empty
	bool IsEmpty() const
	{
		return !mCount;
	}

	/// Returns the amount of memory in bytes that is used by this list
	size_t MemoryConsumption()
	{
		return sizeof(List) + sizeof(Node) * mCount;
	}

	/// Returns the first item in the list
	T& Front() const
	{
		return mHead.mFirst->mData;
	}

	/// Returns the last item in the list
	T& Back() const
	{
		return mHead.mLast->mData;
	}

	/// Returns the first item in the list
	T PopFront()
	{
		It it = First();
		return Remove(it);
	}

	/// Returns the last item in the list
	T PopBack()
	{
		It it = Last();
		return Remove(it);
	}

	/// Returns iterator that points to the first item in the list
	It First() const
	{
		return It(mHead.mFirst);
	}

	/// Returns iterator that points to the last item in the list
	It Last() const
	{
		return It(mHead.mLast);
	}

	/// Returns iterator that represents the end of the list
	It End() const
	{
		return It((Node*) &mHead);
	}

	/// @param it Iterator
	/// @returns An item at iterator position
	T& operator[](const It& it)
	{
		X_ASSERT(it != End());
		return it.mNode->mData;
	}

	/// @param it Iterator
	/// @returns Constant item at iterator position
	const T& operator[](const It& it) const
	{
		X_ASSERT(it != End());
		return it.mNode->mData;
	}

	/// @param it Iterator
	/// @returns An item at iterator position
	T& Get(const It& it)
	{
		X_ASSERT(it != End());
		return it.mNode->mData;
	}

	/// @param it Iterator
	/// @returns Constant item at iterator position
	const T& Get(const It& it) const
	{
		X_ASSERT(it != End());
		return it.mNode->mData;
	}

	/// Clears the list and removes all nodes
	void Clear();

	/// Appends an item to the back of the list
	/// @param item Item to append
	It Append(const T& item);

	/// Prepends an item to the front of the list
	/// @param item Item to prepend
	It Prepend(const T& item);

	/// Inserts an item after a specified iterator
	/// @param it Iterator
	/// @param item Item to insert
	It InsertAfter(It& it, const T& item);

	/// Inserts an item before a specified iterator
	/// @param it Iterator
	/// @param item Item to insert
	It InsertBefore(It& it, const T& item);

	/// Removes an item that is pointed to by a specified iterator
	/// Iterator is automatically decreased
	/// @param it Iterator
	T Remove(It& it);

	/// Finds first occurrence of an item
	/// @param item Item to find
	/// @returns Iterator pointing to found item or End if not found
	template <class Operation>
	It FindBefore(const It& i, Operation op) const {
		for (It it = i; it != End(); ++it) {
			if (op(it.mNode->mData))
				return it;
		}
		return End();
	}

	/// Finds last occurrence of an item
	/// @param item Item to find
	/// @returns Iterator pointing to found item or End if not found
	template <class Operation>
	It FindAfter(const It& i, Operation op) const {
		for (It it = i; it != End(); ++it) {
			if (op(it.mNode->mData))
				return it;
		}
		return End();
	}

	/// Finds first occurrence of an item
	/// @param item Item to find
	/// @returns Iterator pointing to found item or End if not found
	It FindBefore(const It& i, const T& item) const;

	/// Finds last occurrence of an item
	/// @param item Item to find
	/// @returns Iterator pointing to found item or End if not found
	It FindAfter(const It& i, const T& item) const;

	void SwapLinks(const It& a, const It& b)
	{
		a.mNode->mPrev->mNext = b.mNode;
		a.mNode->mNext->mPrev = b.mNode;
		b.mNode->mPrev->mNext = a.mNode;
		b.mNode->mNext->mPrev = a.mNode;
		Swap(a.mNode->mPrev, b.mNode->mPrev);
		Swap(a.mNode->mNext, b.mNode->mNext);
	}

	void SwapValues(const It& a, const It& b)
	{
		Swap(a.mNode->mData, b.mNode->mData);
	}

	void Reverse()
	{
		for (It it = First(); it != End(); --it) {
			Swap(it.mNode->mPrev, it.mNode->mNext);
		}
		Swap(mHead.mLast, mHead.mFirst);
	}

	void RotateLeft(int n = 1)
	{

	}

	void RotateRight(int n = 1)
	{

	}

private:
	/// Ending of list
	EndNode mHead;
	/// Count of items
	unsigned mCount;
};

//*******************************************
//  List
//*******************************************

template<class T>
List<T>::List(const List& lst)
		: mCount(0)
{
	for (It it = lst.First(); it != lst.End(); ++it) {
		Append(lst[it]);
	}
}

template<class T>
List<T>&
List<T>::operator =(const List& lst)
{
	Clear();
	for (It it = lst.First(); it != lst.End(); ++it) {
		Append(lst[it]);
	}
	return *this;
}

template<class T>
void List<T>::Clear()
{
	Node* tmp;
	Node* c = mHead.mFirst;
	while (c != (Node*) &mHead) {
		tmp = c;
		c = c->mNext;
		delete tmp;
	}
	mHead.mFirst = (Node*) &mHead;
	mHead.mLast = (Node*) &mHead;
	mCount = 0;
}

template<class T>
typename List<T>::It List<T>::Append(const T& item)
{
	Node*& prev = mHead.mLast;
	Node* const node = new Node(item, prev, (Node*) &mHead);
	X_ASSERT(node);
	prev->mNext = node;
	prev = node;
	mCount++;
	return It(node);
}

template<class T>
typename List<T>::It List<T>::Prepend(const T& item)
{
	Node*& next = mHead.mFirst;
	Node* const node = new Node(item, (Node*) &mHead, next);
	X_ASSERT(node);
	next->mPrev = node;
	next = node;
	mCount++;
	return It(node);
}

template<class T>
typename List<T>::It List<T>::InsertAfter(It& it, const T& item)
{
	Node*& next = it.mNode->mNext;
	Node* const node = new Node(item, it.mNode, next);
	X_ASSERT(node);
	next->mPrev = node;
	next = node;
	it.mNode = node;
	mCount++;
	return it;
}

template<class T>
typename List<T>::It List<T>::InsertBefore(It& it, const T& item)
{
	Node*& prev = it.mNode->mPrev;
	Node* const node = new Node(item, prev, it.mNode);
	X_ASSERT(node);
	prev->mNext = node;
	prev = node;
	it.mNode = node;
	mCount++;
	return it;
}

template<class T>
T List<T>::Remove(It& it)
{
	X_ASSERT(it != End());
	Node* const next = it.mNode->mNext;
	Node* const prev = it.mNode->mPrev;
	prev->mNext = next;
	next->mPrev = prev;
	T result = it.mNode->mData;
	delete it.mNode;
	it.mNode = prev;
	mCount--;
	return result;
}


template<class T>
typename List<T>::It List<T>::FindAfter(const It& i, const T& item) const
{
	for (It it = i; it != End(); ++it) {
		if (it.mNode->mData == item)
			return it;
	}
	return End();
}

template<class T>
typename List<T>::It List<T>::FindBefore(const It& i, const T& item) const
{
	for (It it = i; it != End(); --it) {
		if (it.mNode->mData == item)
			return it;
	}
	return End();
}

#endif /* _X_LIST_ */
