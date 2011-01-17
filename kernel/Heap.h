#pragma once

#ifdef DEBUG
	#include <assert.h>
	#define HeapHeadSig 0x96
	#define HeapFootSig 0xB5

	#define CheckNode(n) { \
		if (n->mHeadSig != HeapHeadSig || n->mFootSig != HeapFootSig) { \
		assert(n->mHeadSig == HeapHeadSig); \
		assert(n->mFootSig == HeapFootSig);\
		} \
	}
	#define ProtectNode(n) {n->mHeadSig = HeapHeadSig; n->mFootSig = HeapFootSig;}
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
		uint8_t mHeadSig;
#endif
		bool mFree;
		Node *mPrev, *mNext;
#ifdef DEBUG
		char mFile[256];
		int mLine;
		uint8_t mFootSig;
#endif
		size_t GetSize();
	};

	/// Node iterator
	struct Iterator {
		/// Pointer to current node
		Node* mNode;

		/// Constructor
		Iterator(Node* node) : mNode(node) {}
		~Iterator() {}

		/// Comparison operator
		bool operator !=(const Iterator& it) const{	return mNode != it.mNode;	}

		Node* operator ->() { return mNode; }

		operator Node* () {
			return mNode;
		}

		/// Moves to next node
		Iterator operator ++() {		mNode = mNode->mNext;	CheckNode(mNode);	return (*this);	}
		/// Moves to next node
		Iterator operator ++(int) {	mNode = mNode->mNext;	CheckNode(mNode);	return Iterator(mNode->mPrev); }
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
	Node* mHead;
	size_t mNodesCount;
	size_t mSize;
};

