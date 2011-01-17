#pragma once

#ifdef DEBUG
	#include <assert.h>
	#define HeapHeadSig 0x96
	#define HeapFootSig 0xB5

	#define CheckNode(n) { \
		if (n->_headSig != HeapHeadSig || n->_footSig != HeapFootSig) { \
		assert(n->_headSig == HeapHeadSig); \
		assert(n->_footSig == HeapFootSig);\
		} \
	}
	#define ProtectNode(n) {n->_headSig = HeapHeadSig; n->_footSig = HeapFootSig;}
#else
	#define CheckNode(n)
	#define ProtectNode(n)
#endif

/// Memory heap
class Heap {

private:
	/// Heap node
	struct Node {
#ifdef DEBUG
		uint8_t	_headSig;
#endif
		bool	_free;
		Node*	_prev;
		Node*	_next;
#ifdef DEBUG
		char	_file[256];
		int		_line;
		uint8_t	_footSig;
#endif
		size_t GetSize();
	};

	/// Node iterator
	struct Iterator {
		/// Pointer to current node
		Node* _node;

		/// Constructor
		Iterator(Node* node) : _node(node) {}
		~Iterator() {}

		/// Comparison operator
		bool operator !=(const Iterator& it) const{	return _node != it._node;	}

		Node* operator ->() { return _node; }

		operator Node* () {
			return _node;
		}

		/// Moves to next node
		Iterator operator ++() {		_node = _node->_next;	CheckNode(_node);	return (*this);	}
		/// Moves to next node
		Iterator operator ++(int) {	_node = _node->_next;	CheckNode(_node);	return Iterator(_node->_prev); }
	};

private:

	/**
	* @brief Returns iterator that points to the first item in the list
	*/
	Iterator First() const;

	/**
	* @brief Returns iterator that represents the end of the list
	*/
	Iterator End() const;

	Node* FindSutableNode(const size_t size) const;
	Node* FindNodeContaining(void* p) const;

	void WriteBlock(Node* const at, const bool free, Node* const prev, Node* const next);

	void AddNodeAfter(Node* curr, Node* node);

	void RemoveNode(Node* n);

public:

	Heap(const void* start, const size_t size);

	~Heap();

	#ifdef DEBUG
	void* AllocateDebug(size_t size, const char* file, int line);
	#endif
	
	void* Allocate(size_t size);

	void* AllocateAt(void* p, size_t size);

	void* Reallocate(void* p, size_t size);

	void Free(void* p);

	size_t GetFreeMemorySize() const;

	size_t GetAllocatedMemorySize() const;

	size_t GetMaxFreeBlock() const;

	size_t GetOverhead() const;

	bool IsEmpty() const;

	void Dump();
	void DumpDebug() const;

private:
	Node*	_head;
	size_t	_nodesQty;
	size_t	_size;
};

