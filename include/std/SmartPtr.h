#pragma once

#ifndef __cplusplus
#error "This C++ header was included in C file"
#endif

#ifdef DEBUG
extern int SmartPtrGlobalCounter;
#endif

class SmartPtrData {
public:
	SmartPtrData() : mRefCount(0) {}
	~SmartPtrData() {}
	void IncreaseRefCount() {
		mRefCount++;
	}
	int DecreaseRefCount() {
		return --mRefCount;
	}
private:
	int mRefCount;
};

template <class T>
class SmartPtr {

private:
	void Accept() {
		if (mData != NULL) {
			mData->IncreaseRefCount();
			#ifdef DEBUG
			SmartPtrGlobalCounter++;
			#endif
		}
	}

	void Destroy() {
		if (mData != NULL) {
			if (!mData->DecreaseRefCount()) {
				delete mData;
			}
			#ifdef DEBUG
			SmartPtrGlobalCounter--;
			#endif
		}
	}

public:
	SmartPtr(T* data = NULL)
		: mData(data)
	{
		Accept();
	}

	SmartPtr(const SmartPtr& n)
		: mData(n.mData)
	{
		Accept();
	}

	virtual ~SmartPtr() {
		Destroy();
	}

	T& operator = (const T& n) {
		if (this != &n) {
			Destroy();
			mData = n.mData;
			Accept();
		}
		return *this;
	}

	T* operator->() const {
		assert(mData != NULL);
		return mData;
	}

	operator bool() const {
		return mData != NULL;
	}

public:
	T* mData;
};
