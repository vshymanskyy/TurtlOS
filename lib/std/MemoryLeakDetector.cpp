/** 
 * @file MemoryLeakDetector.cpp
 * Contains implementation of memory leak detector
 */

#define MLD_INTERNAL_LOCK
#include "MemoryLeakDetector.h"

#undef malloc
#undef calloc
#undef realloc
#undef free
#undef strdup
#undef new
#undef delete

#ifdef _DEBUG

class CMld
{
public:
	enum EAllocType
	{
		eUnused,
		eMalloc,
		eNew,
		eArrayNew,

		eAllocTypeQty
	};

	struct MemoryBlockInfo
	{
		const char*	mFile;
		unsigned	mLine;
		size_t		mSize;
		void*		mPointer;
		EAllocType	mType;

		MemoryBlockInfo()
			: mFile		("")
			, mLine		(0)
			, mSize		(0)
			, mPointer	(NULL)
			, mType		(eUnused)
		{
		}

		MemoryBlockInfo(const char* aFile, unsigned aLine, void* aPtr, size_t aSize, EAllocType aType)
			: mFile		(aFile)
			, mLine		(aLine)
			, mSize		(aSize)
			, mPointer	(aPtr)
			, mType		(aType)
		{
		}
	};

	enum EConstatnts {
		eBlocksQty = 10*1024
	};

public:
	~CMld()
	{
		Dump();
	}

	void Dump()
	{
		printf("------------  Memory Leak Detector Report  --------------\n");

		size_t totalSize = 0;
		for(int i=0; i<eBlocksQty; i++)
		{
			const MemoryBlockInfo& it = mBlocks[i];

			if(it.mType != eUnused)
			{
				totalSize += it.mSize;
				printf("%p %s:%d (%d bytes)\n", it.mPointer, it.mFile, it.mLine, it.mSize);
			}
		}
		printf("Total leaked: %d bytes\n", totalSize);

		printf("---------------------------------------------------------\n");
	}

	void TrackAllocation(const char* aFile, unsigned aLine, void* aPtr, size_t aSize, EAllocType aType)
	{
		for(int i=0; i<eBlocksQty; i++)
		{
			if(mBlocks[i].mType == eUnused)
			{
				mBlocks[i] = MemoryBlockInfo(aFile, aLine, aPtr, aSize, aType);
				return;
			}
		}
	}

	void TrackDeallocation(const char* aFile, unsigned aLine, void* aPtr, EAllocType aType)
	{
		for(int i=0; i<eBlocksQty; i++)
		{
			const MemoryBlockInfo& it = mBlocks[i];
			if(it.mType != eUnused && it.mPointer == aPtr)
			{
				if (it.mType != aType) {
					printf("Warning: Wrong alloc/free combination of address %p\n", aPtr);
					printf("   %s at %s:%d\n", AllocTypeStr[it.mType], it.mFile, it.mLine);
					printf("   %s at %s:%d\n", DeallocTypeStr[aType], aFile, aLine);
				}
				mBlocks[i].mType = eUnused;
				return;
			}
		}
		printf("Warning: Deallocation of unknown address %p\n", aPtr);
		printf("   %s at %s:%d\n", DeallocTypeStr[aType], aFile, aLine);
	}

	MemoryBlockInfo* FindBlock(void* aPtr)
	{
		for(int i=0; i<eBlocksQty; i++)
		{
			if(mBlocks[i].mPointer == aPtr)
			{
				return &mBlocks[i];
			}
		}
		return NULL;
	}
private:
	static const char* AllocTypeStr[eAllocTypeQty];
	static const char* DeallocTypeStr[eAllocTypeQty];

	MemoryBlockInfo mBlocks[eBlocksQty];

};

static CMld __mld;

const char* CMld::AllocTypeStr[eAllocTypeQty] =
{
	"<unused>",
	"malloc",
	"new",
	"new[]"
};

const char* CMld::DeallocTypeStr[eAllocTypeQty] =
{
	"<unused>",
	"free",
	"delete",
	"delete[]"
};


void* _mld_malloc(size_t aSize, const char* aFile, int aLine)
{
	void* aPtr = malloc(aSize);
	__mld.TrackAllocation(aFile, aLine, aPtr, aSize, CMld::eMalloc);
	return aPtr;
}

void* _mld_calloc(size_t aNum, size_t aSize, const char* aFile, int aLine)
{
	void* aPtr = calloc(aNum, aSize);
	__mld.TrackAllocation(aFile, aLine, aPtr, aSize * aNum, CMld::eMalloc);
	return aPtr;
}


void* _mld_realloc(void* aPtr, size_t aSize, const char* aFile, int aLine)
{
	void* nptr = realloc(aPtr, aSize);

	if(nptr!=NULL && aPtr==NULL && aSize>0) // block allocated
	{
		__mld.TrackAllocation(aFile, aLine, aPtr, aSize, CMld::eMalloc);
	}
	else if(nptr==NULL && aPtr!=NULL && aSize == 0) // block deallocated
	{
		__mld.TrackDeallocation(aFile, aLine, aPtr, CMld::eMalloc);
	}
	else if(nptr!=NULL)	// block modified
	{
		CMld::MemoryBlockInfo* info = __mld.FindBlock(aPtr);
		info->mPointer = nptr;
		info->mFile = aFile;
		info->mLine = aLine;
		info->mSize = aSize;
		info->mType = CMld::eMalloc;
	}

	return nptr;
}

char* _mld_strdup(const char* str, const char* aFile, int aLine)
{
#ifdef WINCE
	char* str2 = _strdup(str);
#else
	char* str2 = strdup(str);
#endif
	__mld.TrackAllocation(aFile, aLine, str2, strlen(str), CMld::eMalloc);
	return str2;
}

void _mld_free(void* aPtr, const char* aFile, int aLine)
{
	__mld.TrackDeallocation(aFile, aLine, aPtr, CMld::eMalloc);
	free(aPtr);
}

void* operator new(size_t aSize, const char* aFile, int aLine)
{
	void* aPtr = malloc(aSize);
	__mld.TrackAllocation(aFile, aLine, aPtr, aSize, CMld::eNew);
	return aPtr;
}

void* operator new[](size_t aSize, const char* aFile, int aLine)
{
	void* aPtr = malloc(aSize);
	__mld.TrackAllocation(aFile, aLine, aPtr, aSize, CMld::eArrayNew);
	return aPtr;
}

void operator delete(void* aPtr, const char* aFile, int aLine)
{
	__mld.TrackDeallocation(aFile, aLine, aPtr, CMld::eNew);
	free(aPtr);
}
void operator delete[](void* aPtr, const char* aFile, int aLine)
{
	__mld.TrackDeallocation(aFile, aLine, aPtr, CMld::eArrayNew);
	free(aPtr);
}

void operator delete(void* aPtr)
{
	__mld.TrackDeallocation("<unknown>", 0, aPtr, CMld::eNew);
	free(aPtr);
}

void operator delete[](void* aPtr)
{
	__mld.TrackDeallocation("<unknown>", 0, aPtr, CMld::eArrayNew);
	free(aPtr);
}

void _mld_dump()
{
	__mld.Dump();
}

#endif
