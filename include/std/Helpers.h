#ifndef INC_HELPERS_HPP
#define INC_HELPERS_HPP

#if _MSC_VER > 1000
	#pragma once
#endif

#ifndef __cplusplus
#error "This C++ header was included in C file"
#endif

#include <stddef.h>

#define STRINGIFY(x) #x

template <class T>
void Swap(T& a, T& b) { const T t = a; a = b; b = t; }

template <class T>
const T& Min(const T& a, const T& b) { return ((a < b) ? a : b); }

template <class T>
const T& Max(const T& a, const T& b) { return ((a > b) ? a : b); }

template <class T>
T& Min(T& a, T& b) { return ((a < b) ? a : b); }

template <class T>
T& Max(T& a, T& b) { return ((a > b) ? a : b); }

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
#ifndef DMALLOC
	/// @brief Declared as private to prevent allocation on heap
	void* operator new (size_t size);
	/// @brief Declared as private to prevent heap usage
	void operator delete (void *p);
#endif
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
	static T& Get(){
		static T* instance = new T();
		return *instance;
	}
protected:
	/// @brief Constructor
	Singleton(){}
	/// @brief Destructor
	~Singleton(){}
};

#endif // INC_HELPERS_HPP
