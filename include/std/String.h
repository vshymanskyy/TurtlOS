#ifndef _INC_STRING_H
#define _INC_STRING_H

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
	};

	/// Pointer to actual data
	RefCountedData* mRefData;

	char* GetBuffer() const {
		return mRefData ? (char*)(mRefData+1) : NULL;
	}

	/// Compares data to another String
	/// @param s String to compare
	/// @return Difference
	ptrdiff_t Compare(const String& s)const;

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
	bool StartsWith(const String& s)const;

	/// @param s The String to check against being postfix
	bool EndsWith(const String& s)const;

	/// @param offset The offset of substring
	/// @returns substring
	String Substring(int offset);

	/// @param offset The offset of substring
	/// @param count The length of substring
	/// @returns substring
	String Substring(int offset, int count);

	/// Sets data to a new value
	void Set(const String& s);

	/// @returns pointer to the constant data
	operator const char*() const;

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
	return mRefData ? mRefData->length : 0;
}

inline
String operator+(const String& s1, const String& s2) {
	return (String(s1)+=s2);
}

inline
String::operator const char*() const
{
	return GetBuffer();
}

inline
const char&
String::operator[](int index) const
{
	return (GetBuffer())[index];
}


inline
bool
String::operator!=(const String& s) const
{
	return Compare(s)!=0;
}

inline
bool
String::operator <(const String& s) const
{
	return Compare(s) <0;
}

inline
bool
String::operator >(const String& s) const
{
	return Compare(s) >0;
}

inline
bool
String::operator<=(const String& s) const
{
	return Compare(s)<=0;
}

inline
bool
String::operator>=(const String& s) const
{
	return Compare(s)>=0;
}

#endif // _INC_STRING_H

