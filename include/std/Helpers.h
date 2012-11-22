#ifndef _X_HELPERS_H_
#define _X_HELPERS_H_

#ifndef __cplusplus
#error "This C++ header was included in C file"
#endif

#include <stdlib.h>
#include <string.h>

template<typename T>
void Swap(T& a, T& b) { const T t = a; a = b; b = t; }

template<typename T>
const T& Min(const T& a, const T& b) { return ((a < b) ? a : b); }

template<typename T>
const T& Max(const T& a, const T& b) { return ((a > b) ? a : b); }

template<typename T>
T& Min(T& a, T& b) { return ((a < b) ? a : b); }

template<typename T>
T& Max(T& a, T& b) { return ((a > b) ? a : b); }

template<typename T>
const T& Min(const T& a, const T& b, const T& c) { return Min(Min(a, b), c); }

template<typename T>
const T& Max(const T& a, const T& b, const T& c) { return Max(Max(a, b), c); }

template<typename T>
T& Min(T& a, T& b, T& c) { return Min(Min(a, b), c); }

template<typename T>
T& Max(T& a, T& b, T& c) { return Max(Max(a, b), c); }

template<typename T>
const T& Min(const T& a, const T& b, const T& c, const T& d) { return Min(Min(Min(a, b), c), d); }

template<typename T>
const T& Max(const T& a, const T& b, const T& c, const T& d) { return Max(Max(Max(a, b), c), d); }

template<typename T>
T& Min(T& a, T& b, T& c, T& d) { return Min(Min(Min(a, b), c), d); }

template<typename T>
T& Max(T& a, T& b, T& c, T& d) { return Max(Max(Max(a, b), c), d); }

template<typename T>
T AbsDiff(const T& v1, const T& v2)
{
	return (v1 > v2) ? (v1 - v2) : (v2 - v1);
}

template<typename T>
bool AlmostEquals(const T& v1, const T& v2, const T& epsilon)
{
	return AbsDiff(v1, v2) < epsilon;
}

template<typename T>
bool InRange(const T& x, const T& a, const T& b)
{
	return (Min(a, b) <= x && x <= Max(a, b));
}

template<typename T>
bool OutOfRange(const T& x, const T& a, const T& b)
{
	return (x < Min(a, b) || Max(a, b) < x);
}

template <typename T>
T ClampRange(const T& x, const T& a, const T& b)
{
	if (x > Max(a, b)) {
		return Max(a, b);
	} else if (x < Min(a, b)) {
		return Min(a, b);
	} else {
		return x;
	}
}

template <typename T>
T LinearMap(T x, T in_min, T in_max, T out_min, T out_max)
{
	return ClampRange((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min, out_min, out_max);
}

/*
#include <time.h>

inline
void RandInit()
{
	srand ( time(NULL) );
}

inline
int RandRange(int a, int b)
{
	return rand() / (RAND_MAX / (b - a + 1)) + a;
}

inline
void MemRand(void* dst, unsigned len)
{
	char *d = (char *) dst;
	while (len--) {
		*d++ = (char)(rand() & 0xFF);
	}
}
*/
inline
char** CommandLineToArgv(const char* command, int* _argc)
{
	char** argv;
	char*  _argv;
	size_t len;
	size_t argc;
	char   a;
	size_t i, j;

	bool  in_QM;
	bool  in_TEXT;
	bool  in_SPACE;

	if (!command) {
		return NULL;
	}

	len = strlen(command);
	i = ((len+2)/2+1)*sizeof(void*);

	argv = (char**)malloc(i + (len+2));

	_argv = (char*)(((uint8_t*)argv)+i);

	argc = 0;
	argv[argc] = _argv;
	in_QM = false;
	in_TEXT = false;
	in_SPACE = true;
	i = 0;
	j = 0;

	while ((a = command[i])) {
		if(in_QM) {
			if(a == '\"') {
				in_QM = false;
			} else {
				_argv[j] = a;
				j++;
			}
		} else {
			switch(a) {
			case '\"':
				in_QM = true;
				in_TEXT = true;
				if(in_SPACE) {
					argv[argc] = _argv+j;
					argc++;
				}
				in_SPACE = false;
				break;
			case ' ':
			case '\t':
			case '\n':
			case '\r':
				if(in_TEXT) {
					_argv[j] = '\0';
					j++;
				}
				in_TEXT = false;
				in_SPACE = true;
				break;
			default:
				in_TEXT = true;
				if(in_SPACE) {
					argv[argc] = _argv+j;
					argc++;
				}
				_argv[j] = a;
				j++;
				in_SPACE = false;
				break;
			}
		}
		i++;
	}
	_argv[j] = '\0';
	argv[argc] = NULL;

	if (_argc) {
		(*_argc) = argc;
	}
	return argv;
}

/** @brief Protects derived classes from being allocated on heap
 @ingroup Patterns

 The StackOnly class has a private new and delete operator,
 therefore derived classes cannot be allocated on heap.
 This class is meant to be used as a base-class for all classes,
 that have to be allocated on stack only.
 */
class StackOnly
{
protected:
	/// @brief Constructor
	StackOnly()
	{
	}
	/// @brief Destructor
	~StackOnly()
	{
	}
private:
#ifndef DMALLOC
	/// @brief Declared as private to prevent allocation on heap
	void* operator new(size_t size);
	/// @brief Declared as private to prevent heap usage
	void operator delete(void *p);
#endif
};

/** @brief Protects derived classes from being copied
 @ingroup Patterns

 The NonCopyable class has a private copy constructor and assignment
 operator, therefore derived classes cannot be copied.
 Trying to copy a NonCopyable object will cause compile-time error.
 */
class NonCopyable
{
protected:
	/// @brief Constructor
	NonCopyable()
	{
	}
	/// @brief Destructor
	~NonCopyable()
	{
	}
private:
	/// @brief Declared as private to prevent usage of copy constructor
	NonCopyable(const NonCopyable&);
	/// @brief Declared as private to prevent usage of assignment operator
	NonCopyable& operator=(const NonCopyable&);
};

/** @brief Singleton class template
 @ingroup Patterns
 */
template<typename T>
class Singleton: public NonCopyable
{
public:
	/** @brief Returns the instance of the singleton type
	 When called for the first time, the singleton instance will be
	 created. All subsequent calls will return a reference to the
	 previously created instance.

	 @return The singleton instance
	 */
	static T& Get()
	{
		static T instance;
		return instance;
	}
protected:
	/// @brief Constructor
	Singleton()
	{
	}
	/// @brief Destructor
	~Singleton()
	{
	}
};

#endif // INC_HELPERS_HPP
