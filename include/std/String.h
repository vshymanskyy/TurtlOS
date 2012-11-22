#ifndef _X_STRING_H_
#define _X_STRING_H_

#ifndef __cplusplus
#error "This C++ header was included in C file"
#endif


#define X_ASSERT assert
#define X_DBG(expr) expr

/// An ANSI COW string, length is stored internally
class String
{
private:

	/// String data wrapper (to support reference counting)
	struct RefCountedData {
		/// Length of the string
		unsigned length;
		/// Count of references
		int refcount;
		/// Data buffer
		char buffer[1];
	};

	static RefCountedData EMPTY;

	/// Pointer to actual data
	RefCountedData* mRefData;

	/// Compares data to another String
	/// @param s String to compare
	/// @return Difference
	ssize_t Compare(const String& s)const;

	/// @param s Length of the buffer
	String(size_t len);

public:
	/// Default constructor
	String();
	/// @param s ANSI string to copy
	String(const char* s);
	/// @param s String to copy
	String(const String& s);
	/// Destructor
	~String();

	/// Constructs a string according to a specified format,
	/// just like sprintf function.
	/// @param format Format
	static String Format(const char* format, ...);

	/// @param s String to assign to
	/// @returns Reference to self
	String& operator  =(const String& s);

	/// @param s Sting to append
	/// @returns Result ;)
	String& operator +=(const String& s);

	/// @returns The length of data
	unsigned Length() const;

	/// @param s The String to check against being prefix
	bool StartsWith(const String& s) const;

	/// @param s The String to check against being postfix
	bool EndsWith(const String& s) const;

	/// @param offset The offset of substring
	/// @returns substring
	String Substring(int offset) const;

	/// @param offset The offset of substring
	/// @param length The length of substring
	/// @returns substring
	String Substring(int offset, unsigned length) const;

	int	Find(const String& s, int offset = 0) const;

	/// Sets data to a new value
	void Set(const String& s);

	/// @returns pointer to the constant data
	operator const char*() const { return mRefData->buffer; }

	operator char*() { return mRefData->buffer; }

	//operator bool() const { return Length() != 0; };

	/// @param index the index of char
	/// @returns reference to a char with the specified index
	const char& operator[](int index) const;

	/// @param s String to compare with
	bool operator==(const String& s)const;
	/// @param s String to compare with
	bool operator!=(const String& s)const;
	/// @param s String to compare with
	bool operator <(const String& s)const;
	/// @param s String to compare with
	bool operator >(const String& s)const;
	/// @param s String to compare with
	bool operator<=(const String& s)const;
	/// @param s String to compare with
	bool operator>=(const String& s)const;
};

inline
unsigned String::Length() const{
	return mRefData->length;
}

inline
String operator+(const String& s1, const String& s2) {
	return (String(s1)+=s2);
}

inline
const char&
String::operator[](int index) const
{
	if (index < 0) index += mRefData->length;
	if (index < 0 || index >= (int)mRefData->length) return mRefData->buffer[mRefData->length];
	return mRefData->buffer[index];
}

inline
bool
String::operator==(const String& s) const
{
	return Compare(s) == 0;
}

inline
bool
String::operator!=(const String& s) const
{
	return Compare(s) != 0;
}

inline
bool
String::operator <(const String& s) const
{
	return Compare(s) < 0;
}

inline
bool
String::operator >(const String& s) const
{
	return Compare(s) > 0;
}

inline
bool
String::operator<=(const String& s) const
{
	return Compare(s) <= 0;
}

inline
bool
String::operator>=(const String& s) const
{
	return Compare(s) >= 0;
}

#endif /* _X_STRING_H_ */

