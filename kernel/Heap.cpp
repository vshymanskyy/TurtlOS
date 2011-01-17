#include "Heap.h"
#include "Console.h"

#include <memory.h>
#include <string.h>

#define MinBlockSpace 16


Heap::Heap(const void* start, const size_t size)
	:mSize(size)
{
	Node* mFirst = (Node*)start;
	mHead = (Node*)((char*)(mFirst - 1) + size);

	WriteBlock(mFirst, true, mHead, mHead);
	WriteBlock(mHead, false, mFirst, mFirst);
	mNodesCount = 1;
}

Heap::~Heap() {

}

inline Heap::Iterator Heap::First() const{
	return Iterator(mHead->mNext);
}

inline Heap::Iterator Heap::End() const{
	return Iterator(mHead);
}

size_t Heap::GetFreeMemorySize() const{
	size_t result = 0;
	for(Iterator it = First(); it != End(); it++) {
		if (it->mFree) {
			result += it->GetSize();
		}
	}
	return result;
}

size_t Heap::GetAllocatedMemorySize() const{
	size_t result = 0;
	for(Iterator it = First(); it != End(); it++) {
		if (!it->mFree) {
			size_t size = it->GetSize();
			result += size;
		}
	}
	return result;
}

size_t Heap::GetMaxFreeBlock() const{
	size_t result = 0;
	for(Iterator it = First(); it != End(); it++) {
		if (it->mFree) {
			const size_t size = it->GetSize();
			if (size > result)
				result = size;
		}
	}
	return result;
}

size_t Heap::GetOverhead() const{
	return sizeof(Heap) + sizeof(Node) * (mNodesCount + 1);
}

void Heap::Dump() {
	int aCount = 0;
	int fCount = 0;
	size_t aSize = 0;
	size_t fSize = 0;
	size_t fMax = 0;

	(*console)
		<< "---------- Memory status ----------" << endl;

	for(Iterator it = First(); it != End(); it++) {
		const size_t size = it->GetSize();
		if (it->mFree) {
			++fCount;
			fSize += size;
			if (size > fMax)
				fMax = size;
		} else {
			#ifdef DEBUG
			(*console) << it->mFile << ":" << it->mLine << endl;
			#endif
			++aCount;
			aSize += size;
		}
	}

	(*console)
		<< "Nodes #:   "  << (int)mNodesCount << "   A:" << aCount << "    F:" << fCount << endl
		<< "Size:      " << mSize/1024 << "K +" << mSize%1024 << "b" << endl
		<< "Allocated: " << aSize/1024 << "K +" << aSize%1024 << "b" << endl
		<< "Free:      " << fSize/1024 << "K +" << fSize%1024 << "b" << endl
		<< "Max free:  " << fMax/1024  << "K +" << fMax%1024 << "b" << endl
		<< "Inner use: " << GetOverhead()/1024 << "K +" << GetOverhead()%1024 << "b" << endl;
}

bool Heap::IsEmpty() const {
	for(Iterator it = First(); it != End(); it++) {
		if (!it->mFree) {
			return false;
		}
	}
	return true;
}

#ifdef DEBUG
void Heap::DumpDebug() const{
	for(Iterator it = First(); it != End(); it++) {
		if (!it->mFree) {
			debug_print("%s:%d [%d]\n", it->mFile, it->mLine, it->GetSize());
		}
	}
}

void* Heap::AllocateDebug(size_t size, const char* file, int line) {
	if (!size)
		return NULL;
	Node* n = FindSutableNode(size);
	assert(n);

	const size_t bSize = n->GetSize();
	n->mFree = false;
	n->mLine = line;
	strncpy(n->mFile, file, 256);

	if (bSize - size > MinBlockSpace*sizeof(Node)) {
		AddNodeAfter(n, (Node*)((char*)(n + 1) + size));
	}

	return (void*)(n + 1);
}
#endif

void* Heap::Allocate(size_t size) {
	if (!size)
		return NULL;
	Node* n = FindSutableNode(size);
	assert(n);

	const size_t bSize = n->GetSize();
	n->mFree = false;
	if (bSize - size > MinBlockSpace*sizeof(Node)) {
		AddNodeAfter(n, (Node*)((char*)(n + 1) + size));
	}

	return (void*)(n + 1);
}

Heap::Node*
Heap::FindNodeContaining(void* p) const
{
	for(Iterator it = First(); it != End(); it++) {
		Node* cur = it;
		if ((cur+1) <= p && p < cur->mNext) {
			return cur;
		}
	}
	return NULL;
}

void*
Heap::AllocateAt(void* p, size_t size)
{
	Node* n = FindNodeContaining(p);
	if (!n)
		return p;
	//assert(n);
	if (p == (void*)(n+1) && n->GetSize() == size) {
		n->mFree = false;
		return p;
	}
	assert(n->mFree == true);
	assert(size < n->GetSize());
	//TODO: add node
	return p;
}

void
Heap::Free(void* p)
{
	if (!p) {
		return;
	}
	Node* n = (Node*)p - 1;
	CheckNode(n);

	if (n->mNext->mFree)
		RemoveNode(n->mNext);

	if (n->mPrev->mFree) {
		RemoveNode(n);
	} else {
		n->mFree = true;
	}
}

void
Heap::AddNodeAfter(Node* curr, Node* node)
{
	CheckNode(curr);
	Node*& next = curr->mNext;
	WriteBlock(node, true, curr, next);
	next->mPrev = node;
	next = node;
	++mNodesCount;
}

void
Heap::RemoveNode(Node* n)
{
	CheckNode(n);
	Node* const next = n->mNext;
	Node* const prev = n->mPrev;
	CheckNode(next);
	CheckNode(prev);
	prev->mNext = next;
	next->mPrev = prev;
	--mNodesCount;
}

void
Heap::WriteBlock(Node* const at, const bool free, Node* const prev, Node* const next)
{
	at->mFree = free;
	at->mPrev = prev;
	at->mNext = next;
	ProtectNode(at);
}

Heap::Node*
Heap::FindSutableNode(const size_t size) const
{
	Node* result = NULL;
	size_t resSize = (size_t)-1;
	for(Iterator it = First(); it != End(); it++) {
		if (it->mFree) {
			const size_t curSize = it->GetSize();
			if (curSize == size)
				return it.mNode;
			else if (curSize > size && curSize < resSize) {
				resSize = curSize;
				result = it.mNode;
			}
		}
	}
	return result;
}

void*
Heap::Reallocate(void* p, size_t size)
{
	if (!size) {
		Free(p);
		return NULL;
	}
	if (!p) {
		return Allocate(size);
	}

	Node* n = (Node*)p - 1;
	CheckNode(n);
	const size_t curSize = n->GetSize();
	if (size <= curSize) {
		return p;
	}
	Node* next = n->mNext;
	Node* next2 = next->mNext;
	Node* ins = (Node*)((char*)p + size);
	if (next->mFree && (ins < next2)) {
		RemoveNode(next);
		if (next2 - ins > MinBlockSpace)
			AddNodeAfter(n, ins);
		return p;
	} else {
		void* result = Allocate(size);
		memcpy(result, p, curSize);
		Free(p);
		return result;
	}
}

size_t
Heap::Node::GetSize()
{
	return (char*)mNext - (char*)this - sizeof(Node);
}

