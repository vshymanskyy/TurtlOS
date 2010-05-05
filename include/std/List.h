#pragma once

/// Represents a generic double-linked list
template <class T>
class List
{
private:
	/// List node
	struct Node {
		/// Pointer to previous item
		Node *mPrev;
		/// Pointer to next item
		Node *mNext;
		/// The data
		T mData;

		/// Constructor
		Node(const T& data, Node* prev, Node* next);
	};

	/// Node at the end of the list
	struct EndNode {
		/// Pointer to last item
		Node *mPrev;
		/// Pointer to first item
		Node *mNext;

		/// Constructor
		EndNode();
	};

public:
	/// List iterator
	class Iterator {

	private:
		/// Pointer to node
		Node* mNode;

	public:
		/// Constructor
		Iterator(Node* node);

		/// Destructor
		~Iterator() {}

		Node* getNode() const;

		/// Comparison operator
		bool operator ==(const Iterator& it) const;
		/// Comparison operator
		bool operator !=(const Iterator& it) const;

		/// Moves to next item
		Iterator operator ++();
		/// Moves to previous item
		Iterator operator --();
	};

public:
	/// Constructor
	List();

	/// Copy constructor
	List(const List& lst);

	/// Assignment operator
	List& operator = (const List& lst);

	/// Destructor
	~List();

	/// Returns the size of the list
	unsigned Count() const;

	/// Returns the amount of memory in bytes that is used by this list
	size_t MemoryConsumption();

	/// Checks if list is empty
	bool IsEmpty() const;

	/// Returns iterator that points to the first item in the list
	Iterator First() const;

	/// Returns iterator that points to the last item in the list
	Iterator Last() const;

	/// Returns iterator that represents the end of the list
	Iterator End() const;

	/// Clears the list and removes all nodes
	void Clear();

	void DeleteAll();

	/// Appends an item to the back of the list
	/// @param item Item to append
	void Append(const T& item);

	/// Prepends an item to the front of the list
	/// @param item Item to prepend
	void Prepend(const T& item);

	/// Inserts an item after a specified iterator
	/// @param it Iterator
	/// @param item Item to insert
	void InsertAfter(const Iterator& it, const T& item);

	/// Inserts an item before a specified iterator
	/// @param it Iterator
	/// @param item Item to insert
	void InsertBefore(const Iterator& it, const T& item);

	/// Removes an item that is pointed to by a specified iterator
	/// @param it Iterator
	void RemoveAt(const Iterator& it);

	/// @param it Iterator
	/// @returns An item at iterator position
	T& operator[](const Iterator& it);

	/// @param it Iterator
	/// @returns Constant item at iterator position
	const T& operator[](const Iterator& it)const;

	/// Finds first occurrence of an item
	/// @param item Item to find
	/// @returns Iterator pointing to found item or End if not found
	Iterator FindFirst(const T& item) const;

	/// Finds last occurrence of an item
	/// @param item Item to find
	/// @returns Iterator pointing to found item or End if not found
	Iterator FindLast(const T& item) const;

private:
	/// Ending of list
	EndNode mHead;
	/// Count of items
	unsigned mCount;
};

#include "List.inline"

