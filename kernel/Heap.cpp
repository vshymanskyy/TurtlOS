#include "Heap.h"
#include "Console.h"

#include <stdlib.h>
#include <string.h>

#define MinBlockSpace 16


Heap::Heap(const void* start, const size_t size)
	: _size(size)
{
	Node* mFirst = (Node*)start;
	_head = (Node*)((char*)(mFirst - 1) + size);

	WriteBlock(mFirst, true, _head, _head);
	WriteBlock(_head, false, mFirst, mFirst);
	_nodesQty = 1;
}

Heap::~Heap()
{

}

inline Heap::Iterator Heap::First() const
{
	return Iterator(_head->_next);
}

inline Heap::Iterator Heap::End() const
{
	return Iterator(_head);
}

size_t Heap::GetFreeMemorySize() const
{
	size_t result = 0;
	for(Iterator it = First(); it != End(); it++) {
		if (it->_free) {
			result += it->GetSize();
		}
	}
	return result;
}

size_t Heap::GetAllocatedMemorySize() const
{
	size_t result = 0;
	for(Iterator it = First(); it != End(); it++) {
		if (!it->_free) {
			size_t size = it->GetSize();
			result += size;
		}
	}
	return result;
}

size_t Heap::GetMaxFreeBlock() const
{
	size_t result = 0;
	for(Iterator it = First(); it != End(); it++) {
		if (it->_free) {
			const size_t size = it->GetSize();
			if (size > result)
				result = size;
		}
	}
	return result;
}

size_t Heap::GetOverhead() const
{
	return sizeof(Heap) + sizeof(Node) * (_nodesQty + 1);
}

void Heap::Dump()
{
	int aQty = 0;
	int fQty = 0;
	size_t aSize = 0;
	size_t fSize = 0;
	size_t fMax = 0;

	debug_print("---------- Memory status ----------\n");

	for(Iterator it = First(); it != End(); it++) {
		const size_t size = it->GetSize();
		if (it->_free) {
			++fQty;
			fSize += size;
			if (size > fMax)
				fMax = size;
		} else {
			#ifdef DEBUG
			debug_print("%s:%d\n", it->_file, it->_line);
			#endif
			++aQty;
			aSize += size;
		}
	}
/*
	(*console)
		<< "Nodes #:   "  << (int)_nodesQty << "   A:" << aQty << "    F:" << fQty << endl
		<< "Size:      " << _size/1024 << "K +" << _size%1024 << "b" << endl
		<< "Allocated: " << aSize/1024 << "K +" << aSize%1024 << "b" << endl
		<< "Free:      " << fSize/1024 << "K +" << fSize%1024 << "b" << endl
		<< "Max free:  " << fMax/1024  << "K +" << fMax%1024 << "b" << endl
		<< "Inner use: " << GetOverhead()/1024 << "K +" << GetOverhead()%1024 << "b" << endl;
		*/
}

bool Heap::IsEmpty() const
{
	for(Iterator it = First(); it != End(); it++) {
		if (!it->_free) {
			return false;
		}
	}
	return true;
}

#ifdef DEBUG
void Heap::DumpDebug() const
{
	for(Iterator it = First(); it != End(); it++) {
		if (!it->_free) {
			debug_print("%s:%d [%d]\n", it->_file, it->_line, it->GetSize());
		}
	}
}

void* Heap::AllocateDebug(size_t size, const char* file, int line)
{
	if (!size)
		return NULL;
	Node* n = FindSutableNode(size);
	assert(n);

	const size_t bSize = n->GetSize();
	n->_free = false;
	n->_line = line;
	strncpy(n->_file, file, 256);

	if (bSize - size > MinBlockSpace*sizeof(Node)) {
		AddNodeAfter(n, (Node*)((char*)(n + 1) + size));
	}

	return (void*)(n + 1);
}
#endif

void* Heap::Allocate(size_t size)
{
	if (!size)
		return NULL;
	Node* n = FindSutableNode(size);
	assert(n);

	const size_t bSize = n->GetSize();
	n->_free = false;
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
		if ((cur+1) <= p && p < cur->_next) {
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
		n->_free = false;
		return p;
	}
	assert(n->_free == true);
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

	if (n->_next->_free)
		RemoveNode(n->_next);

	if (n->_prev->_free) {
		RemoveNode(n);
	} else {
		n->_free = true;
	}
}

void
Heap::AddNodeAfter(Node* curr, Node* node)
{
	CheckNode(curr);
	Node*& next = curr->_next;
	WriteBlock(node, true, curr, next);
	next->_prev = node;
	next = node;
	++_nodesQty;
}

void
Heap::RemoveNode(Node* n)
{
	CheckNode(n);
	Node* const next = n->_next;
	Node* const prev = n->_prev;
	CheckNode(next);
	CheckNode(prev);
	prev->_next = next;
	next->_prev = prev;
	--_nodesQty;
}

void
Heap::WriteBlock(Node* const at, const bool free, Node* const prev, Node* const next)
{
	at->_free = free;
	at->_prev = prev;
	at->_next = next;
	ProtectNode(at);
}

Heap::Node*
Heap::FindSutableNode(const size_t size) const
{
	Node* result = NULL;
	size_t resSize = (size_t)-1;
	for(Iterator it = First(); it != End(); it++) {
		if (it->_free) {
			const size_t curSize = it->GetSize();
			if (curSize == size)
				return it._node;
			else if (curSize > size && curSize < resSize) {
				resSize = curSize;
				result = it._node;
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
	Node* next = n->_next;
	Node* next2 = next->_next;
	Node* ins = (Node*)((char*)p + size);
	if (next->_free && (ins < next2)) {
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
	return (char*)_next - (char*)this - sizeof(Node);
}

