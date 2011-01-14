#pragma once

#include <assert.h>
#include <stddef.h>

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
		Node(const T& data, Node* prev, Node* next);
	};

	/// Node at the end of the list
	struct EndNode {
		/// Pointer to last item
        Node *_last;
		/// Pointer to first item
        Node *_first;

		/// Constructor
		EndNode();
	};

public:
	/// List iterator
    class It {

	private:
		/// Pointer to node
        Node* _node;

	public:
		/// Constructor
        It(Node* node);

		/// Destructor
        ~It() {}

		Node* getNode() const;

		/// Comparison operator
        bool operator ==(const It& it) const;
		/// Comparison operator
        bool operator !=(const It& it) const;

		/// Moves to next item
        It operator ++();
		/// Moves to previous item
        It operator --();
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
    It First() const;

	/// Returns iterator that points to the last item in the list
    It Last() const;

	/// Returns iterator that represents the end of the list
    It End() const;

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
    void InsertAfter(const It& it, const T& item);

	/// Inserts an item before a specified iterator
	/// @param it Iterator
	/// @param item Item to insert
    void InsertBefore(const It& it, const T& item);

	/// Removes an item that is pointed to by a specified iterator
	/// @param it Iterator
    void RemoveAt(const It& it);

	/// @param it Iterator
	/// @returns An item at iterator position
    T& operator[](const It& it);

	/// @param it Iterator
	/// @returns Constant item at iterator position
    const T& operator[](const It& it)const;

	/// Finds first occurrence of an item
	/// @param item Item to find
	/// @returns Iterator pointing to found item or End if not found
    It FindFirst(const T& item) const;

	/// Finds last occurrence of an item
	/// @param item Item to find
	/// @returns Iterator pointing to found item or End if not found
    It FindLast(const T& item) const;

private:
	/// Ending of list
    EndNode _head;
	/// Count of items
    unsigned _count;
};

//*******************************************
//  List::Node
//*******************************************

template <class T>
inline
List<T>::Node::Node(const T& data, Node* prev, Node* next)
    : _prev(prev)
    , _next(next)
    , _data(data)
{
}

template <class T>
inline
List<T>::EndNode::EndNode()
    : _last((Node*)this)
    , _first((Node*)this)
{
}

//*******************************************
//  List::Iterator
//*******************************************

template <class T>
inline
List<T>::It::It(Node* node)
    : _node(node)
{
}

template <class T>
inline
typename List<T>::Node*
List<T>::It::getNode() const
{
    return _node;
}

template <class T>
inline
bool
List<T>::It::operator ==(const It& it) const
{
    return _node == it._node;
}

template <class T>
inline
bool
List<T>::It::operator !=(const It& it) const
{
    return _node != it._node;
}

template <class T>
inline
typename List<T>::It
List<T>::It::operator ++()
{
    _node = _node->_next;
    return (*this);
}

template <class T>
inline
typename List<T>::It
List<T>::It::operator --()
{
    _node = _node->_prev;
    return (*this);
}

//*******************************************
//  List
//*******************************************

template <class T>
inline
List<T>::List()
    : _count(0)
{
}

template <class T>
inline
List<T>::List(const List& lst)
    : _count(0)
{
    for(It it=lst.First(); it!=lst.End(); ++it){
        Append(lst[it]);
    }
}

template <class T>
inline
List<T>&
List<T>::operator = (const List& lst){
    Clear();
    for(It it=lst.First(); it!=lst.End(); it++){
        Append(lst[it]);
    }
    return *this;
}

template <class T>
inline
List<T>::~List()
{
    Clear();
}

template <class T>
unsigned
List<T>::Count() const
{
    return _count;
}

template <class T>
inline
size_t
List<T>::MemoryConsumption()
{
    return sizeof(List) + sizeof(Node) * _count;
}

template <class T>
inline
bool
List<T>::IsEmpty() const
{
    return !_count;
}

template <class T>
inline
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
inline
void
List<T>::DeleteAll()
{
    Node* tmp;
    Node* c = _head._first;
    while (c != (Node*)&_head){
        tmp = c;
        c = c->_next;
        delete tmp->_data;
        delete tmp;
    }
    _head._first = (Node*)&_head;
    _head._last = (Node*)&_head;
    _count = 0;
}

template <class T>
inline
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
inline
void
List<T>::Prepend(const T& item)
{
    Node*& next = _head->mPrev;
    Node* const node = new Node(item, _head, next);
    assert(node);
    next->_prev = node;
    next = node;
    _count++;
}

template <class T>
inline
void
List<T>::InsertAfter(const It& it, const T& item)
{
    Node*& next = it._node->_prev;
    Node* const node = new Node(item, it._node, next);
    assert(node);
    next->_prev = node;
    next = node;
    _count++;
}

template <class T>
inline
void
List<T>::InsertBefore(const It& it, const T& item)
{
    Node*& prev = it._node->_prev;
    Node* const node = new Node(item, prev, it._node);
    assert(node);
    prev->_next = node;
    prev = node;
    _count++;
}

template <class T>
inline
void
List<T>::RemoveAt(const It& it)
{
    assert(it!=End());
    Node* const next = it.getNode()->_next;
    Node* const prev = it.getNode()->_prev;
    prev->_next = next;
    next->_prev = prev;
    delete it.getNode();
    _count--;
}

template <class T>
inline
T&
List<T>::operator[](const It& it)
{
    assert(it!=End());
    return it.getNode()->_data;
}

template <class T>
inline
const T&
List<T>::operator[](const It& it) const
{
    assert(it!=End());
    return it.getNode()->_data;
}

template <class T>
inline
typename List<T>::It
List<T>::FindFirst(const T& item) const
{
    It it = First();
    for(; it != End(); ++it){
        if(it.getNode()->_data == item)
            break;
    }
    return it;
}

template <class T>
inline
typename List<T>::It
List<T>::FindLast(const T& item) const
{
    It it = Last();
    for(; it != End(); it--){
        if(it.getNode()->_data == item)
            break;
    }
    return it;
}

template <class T>
inline
typename List<T>::It
List<T>::First() const{
    return It(_head._first);
}

template <class T>
inline
typename List<T>::It
List<T>::Last() const{
    return It(_head._last);
}

template <class T>
inline
typename List<T>::It
List<T>::End() const{
    return It((Node*)&_head);
}
