#pragma once
class BitArray
{
public:
	BitArray(size_t n);
	~BitArray();

	void ClearAll();
	void Set(size_t i);
	void Clear(size_t i);
	bool Test(size_t i) const;

	size_t FindFirstClear() const;
	size_t FindFirstSet() const;

private:
	size_t* mData;
	size_t mSize;
};

#define BITS_PER_ITEM		(8 * sizeof(size_t))
#define INDEX_FROM_BIT(a)	(a / BITS_PER_ITEM)
#define BIT_FROM_INDEX(a)	(a * BITS_PER_ITEM)
#define OFFSET_FROM_BIT(a)	(a % BITS_PER_ITEM)

BitArray::BitArray(size_t n)
	: mSize(n)
{
	mData = new size_t[INDEX_FROM_BIT(mSize)];
	ClearAll();
}

BitArray::~BitArray()
{
	delete[] mData;
}

void
BitArray::ClearAll()
{
	for (int i = 0; i < INDEX_FROM_BIT(mSize); i++)
		mData[i] = SIZE_T_MIN;
}

void
BitArray::Set(const size_t i)
{
	assert(i < mSize);
	mData[INDEX_FROM_BIT(i)] |= (0x1 << OFFSET_FROM_BIT(i));
}

void
BitArray::Clear(const size_t i)
{
	assert(i < mSize);
	mData[INDEX_FROM_BIT(i)] &= ~(0x1 << OFFSET_FROM_BIT(i));
}

bool
BitArray::Test(const size_t i) const
{
	assert(i < mSize);
	return (mData[INDEX_FROM_BIT(i)] & (0x1 << OFFSET_FROM_BIT(i)));
}

size_t
BitArray::FindFirstClear() const
{
	for (size_t i = 0; i < INDEX_FROM_BIT(mSize); i++) {
		if (mData[i] != SIZE_T_MAX) {
			for (size_t j = 0; j < BITS_PER_ITEM; j++) {
				if (!(mData[i] & (1 << j))) {
					return BIT_FROM_INDEX(i) + j;
				}
			}
		}
	}
	return (size_t)-1;
}

size_t
BitArray::FindFirstSet() const
{
	for (size_t i = 0; i < INDEX_FROM_BIT(mSize); i++) {
		if (mData[i] != SIZE_T_MIN) {						// nothing set, exit early.
			for (size_t j = 0; j < BITS_PER_ITEM; j++) {	// at least one bit is set here.
				if (mData[i] & (1 << j)) {
					return BIT_FROM_INDEX(i) + j;
				}
			}
		}
	}
	return (size_t)-1;
}

#undef BITS_PER_ITEM
#undef INDEX_FROM_BIT
#undef BIT_FROM_INDEX
#undef OFFSET_FROM_BIT
