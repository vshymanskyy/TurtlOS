#pragma once

namespace Patterns {


/** @brief Protects derived classes from being allocated on heap
	@ingroup Patterns
	
	The StackOnly class has a private new and delete operator,
	therefore derived classes cannot be allocated on heap.
	This class is meant to be used as a base-class for all classes,
	that have to be allocated on stack only.
*/
class StackOnly {
protected:
	/// @brief Constructor
	StackOnly(){}
	/// @brief Destructor
	~StackOnly(){}
private:
	/// @brief Declared as private to prevent allocation on heap
	void* operator new (size_t size);
	/// @brief Declared as private to prevent heap usage
	void operator delete (void *p);
};



/** @brief Protects derived classes from being copied
	@ingroup Patterns
	
	The NonCopyable class has a private copy constructor and assignment
	operator, therefore derived classes cannot be copied.
	Trying to copy a NonCopyable object will cause compile-time error.
*/
class NonCopyable {
protected:
	/// @brief Constructor
	NonCopyable(){}

	/// @brief Destructor
	~NonCopyable(){}

private:
	/// @brief Declared as private to prevent usage of copy constructor
	NonCopyable(const NonCopyable&);

	/// @brief Declared as private to prevent usage of assignment operator
	NonCopyable& operator=(const NonCopyable&);
};



/** @brief Singleton class template
	@ingroup Patterns
*/
template <typename T>
class Singleton
	: public NonCopyable
{

public:
	/** @brief Returns the instance of the singleton type
		When called for the first time, the singleton instance will be
		created. All subsequent calls will return a reference to the
		previously created instance.

		@return The singleton instance
	*/
	static T* Instance(){
		static T*_instance = new T();
		return instance;
	}

protected:
	/// @brief Constructor
	Singleton(){}

	/// @brief Destructor
	~Singleton(){}
};



/** @brief Autodeleting pointer
	@ingroup Patterns
	
	An autodelete pointer is declared as a local variable. It will delete the 
	pointer assigned to it when it goes out of scope
*/
template <class T>
class AutoPtr {
public:
	/// @brief 
	SmartPtr()
		: mPtr(NULL)
	{}
	
	/// @brief Assigns ownership of pointer to the SmartPtr
	SmartPtr(T* pointer)
		: mPtr(pointer)
	{}
	
	/// @brief Deletes the pointer that it owns
	~SmartPtr() {
		delete mPtr;
	}
	
	/// @brief Assigns ownership of pointer to the SmartPtr and deletes any previosly assigned pointer
	SmartPtr& operator = (T* pointer){
		if(mPtr && mPtr != pointer) {
			delete mPtr;
		}

		mPtr = pointer;
		return *this;
	}

	/// @brief Makes SmartPtr like a real pointer
	operator T*() {
		return mPtr;
	}

	/// @brief Dereferences pointer
	T *operator->() {
		return mPtr;
	}
	
private:
	const T* mPtr;
};



} // END namespace Patterns
