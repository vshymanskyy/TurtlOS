#pragma once

#include <assert.h>
#include <stddef.h>

#include "Helpers.h"

/// Represents a generic double-linked list
template <class T>
class List
{
private:
	/// List node
	struct Node {
		/// Pointer to previous item
		Node *_prev;
		/// Pointer to next item
		Node *_next;
		/// The data
		T _data;

		/// Constructor
		explicit Node(const T& data, Node* prev, Node* next)
			: _prev(prev)
			, _next(next)
			, _data(data)
		{}
	};

	/// Node at the end of the list
	struct EndNode {
		/// Pointer to last item
		Node *_last;
		/// Pointer to first item
		Node *_first;

		/// Constructor
		EndNode()
			: _last((Node*)this)
			, _first((Node*)this)
		{}
	};

public:
	/// List iterator
	class It {

	friend class List<T>;

	private:
		/// Pointer to node
		Node* _node;

	public:
		/// Constructor
		explicit It(Node* node) : _node(node) {}

		/// Destructor
		~It() {}

		/// Comparison operator
		bool operator ==(const It& it) const { return _node == it._node; }
		/// Comparison operator
		bool operator !=(const It& it) const { return _node != it._node; }

		/// Moves to next item
		It& operator ++() { _node = _node->_next; return (*this); }
		/// Moves to previous item
		It& operator --() { _node = _node->_prev; return (*this); }

		/// Moves to next item
		It& operator +(int i) { while(i--) { _node = _node->_next; } return (*this); }
		/// Moves to previous item
		It& operator -(int i) { while(i--) { _node = _node->_prev; } return (*this); }

		T& operator *() { return _node->_data; }
		T* operator ->() { return &(_node->_data); }
	};

public:
	/// Constructor
	List() : _count(0) {}

	/// Copy constructor
	List(const List& lst);

	/// Assignment operator
	List& operator = (const List& lst);

	/// Destructor
	~List() { Clear(); }

	/// Returns the size of the list
	unsigned Count() const { return _count; }

	/// Checks if list is empty
	bool IsEmpty() const { return !_count; }

	/// Returns the amount of memory in bytes that is used by this list
	size_t MemoryConsumption() { return sizeof(List) + sizeof(Node) * _count; }

	/// Returns the first item in the list
	T& Front() const { return _head._first->_data; }

	/// Returns the last item in the list
	T& Back() const { return _head._last->_data; }

	/// Returns the first item in the list
	T PopFront() { It it = First(); return Remove(it); }

	/// Returns the last item in the list
	T PopBack() { It it = Last(); return Remove(it); }

	/// Returns iterator that points to the first item in the list
	It First() const { return It(_head._first); }

	/// Returns iterator that points to the last item in the list
	It Last() const { return It(_head._last); }

	/// Returns iterator that represents the end of the list
	It End() const { return It((Node*)&_head); }

	/// @param it Iterator
	/// @returns An item at iterator position
	T& operator[](const It& it) { assert(it != End()); return it._node->_data; }

	/// @param it Iterator
	/// @returns Constant item at iterator position
	const T& operator[](const It& it) const { assert(it != End()); return it._node->_data; }

	/// Clears the list and removes all nodes
	void Clear();

	/// Appends an item to the back of the list
	/// @param item Item to append
	void Append(const T& item);

	/// Prepends an item to the front of the list
	/// @param item Item to prepend
	void Prepend(const T& item);

	/// Inserts an item after a specified iterator
	/// @param it Iterator
	/// @param item Item to insert
	void InsertAfter(It& it, const T& item);

	/// Inserts an item before a specified iterator
	/// @param it Iterator
	/// @param item Item to insert
	void InsertBefore(It& it, const T& item);

	/// Removes an item that is pointed to by a specified iterator
	/// Iterator is automatically decreased
	/// @param it Iterator
	T Remove(It& it);

	/// Finds first occurrence of an item
	/// @param item Item to find
	/// @returns Iterator pointing to found item or End if not found
	It FindBefore(const It& i, const T& item) const;

	/// Finds last occurrence of an item
	/// @param item Item to find
	/// @returns Iterator pointing to found item or End if not found
	It FindAfter(const It& i, const T& item) const;

	void SwapLinks(const It& a, const It& b) {
		a._node->_prev->_next = b._node;
		a._node->_next->_prev = b._node;
		b._node->_prev->_next = a._node;
		b._node->_next->_prev = a._node;
		Swap(a._node->_prev, b._node->_prev);
		Swap(a._node->_next, b._node->_next);
	}

	void SwapValues(const It& a, const It& b) {
		Swap(a._node->_data, b._node->_data);
	}

	void Reverse() {
		for (It it = First(); it != End(); --it) {
			Swap(it._node->_prev, it._node->_next);
		}
		Swap(_head._last, _head._first);
	}

	void RotateLeft(int n = 1) {

	}

	void RotateRight(int n = 1) {

	}

private:
	/// Ending of list
	EndNode _head;
	/// Count of items
	unsigned _count;
};

//*******************************************
//  List
//*******************************************

template <class T>
List<T>::List(const List& lst)
	: _count(0)
{
	for(It it=lst.First(); it!=lst.End(); ++it){
		Append(lst[it]);
	}
}

template <class T>
List<T>&
List<T>::operator = (const List& lst){
	Clear();
	for(It it=lst.First(); it!=lst.End(); it++){
		Append(lst[it]);
	}
	return *this;
}

template <class T>
void
List<T>::Clear()
{
	Node* tmp;
	Node* c = _head._first;
	while (c != (Node*)&_head){
		tmp = c;
		c = c->_next;
		delete tmp;
	}
	_head._first = (Node*)&_head;
	_head._last = (Node*)&_head;
	_count = 0;
}

template <class T>
void
List<T>::Append(const T& item)
{
	Node*& prev = _head._last;
	Node* const node = new Node(item, prev, (Node*)&_head);
	assert(node);
	prev->_next = node;
	prev = node;
	_count++;
}

template <class T>
void
List<T>::Prepend(const T& item)
{
	Node*& next = _head._first;
	Node* const node = new Node(item, (Node*)&_head, next);
	assert(node);
	next->_prev = node;
	next = node;
	_count++;
}

template <class T>
void
List<T>::InsertAfter(It& it, const T& item)
{
	Node*& next = it._node->_next;
	Node* const node = new Node(item, it._node, next);
	assert(node);
	next->_prev = node;
	next = node;
	it._node = node;
	_count++;
}

template <class T>
void
List<T>::InsertBefore(It& it, const T& item)
{
	Node*& prev = it._node->_prev;
	Node* const node = new Node(item, prev, it._node);
	assert(node);
	prev->_next = node;
	prev = node;
	it._node = node;
	_count++;
}

template <class T>
T
List<T>::Remove(It& it)
{
	assert(it != End());
	Node* const next = it._node->_next;
	Node* const prev = it._node->_prev;
	prev->_next = next;
	next->_prev = prev;
	T result = it._node->_data;
	delete it._node;
	it._node = prev;
	_count--;
	return result;
}

template <class T>
typename List<T>::It
List<T>::FindAfter(const It& i, const T& item) const
{
	for(It it = i; it != End(); ++it){
		if(it._node->_data == item) return it;
	}
	return End();
}

template <class T>
typename List<T>::It
List<T>::FindBefore(const It& i, const T& item) const
{
	for(It it = i; it != End(); --it){
		if(it._node->_data == item) return it;
	}
	return End();
}
