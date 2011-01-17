#include <std/String.h>
#include <string.h>
#include <memory.h>

String
String::Format(const char* format, ...)
{


	//char buf[1024];
	//va_list args;
	//va_start(args, format);
	//vsprintf(buf, format, args);
	//va_end(args);
	return String(format);
}

String::String() {
	mRefData = NULL;
}

String::String(const char* s)
	: mRefData(NULL)
{
	if (const unsigned len = strlen(s)) {
		mRefData = (RefCountedData*) malloc (len + 2 + sizeof(RefCountedData));
		memcpy (GetBuffer(), s, len+1);
		mRefData->refcount = 1;
		mRefData->length = len;
	}
}

String::String(const String& s)
	: mRefData(s.mRefData)
{
	mRefData->refcount++;
}

String::~String() {
	if (mRefData && !(--(mRefData->refcount))) {
		free(mRefData);
	}
}

String&
String::operator=(const String& s)
{
	if (this != &s) {
		if (mRefData && !(--(mRefData->refcount))) {
			free(mRefData);
		}
		mRefData = s.mRefData;
		if (mRefData) {
			++mRefData->refcount;
		}
	}
	return *this;
}

String&
String::operator+=(const String& s)
{
	if (!s.mRefData) {
		return *this;
	}

	if (!mRefData) {
		mRefData = s.mRefData;
		mRefData->refcount++;
		return *this;
	}

	const unsigned len = mRefData->length + s.mRefData->length;
	if (--(mRefData->refcount)) {
		RefCountedData* data = (RefCountedData*)malloc(len + 2 + sizeof(RefCountedData));
		memcpy((char*)(data+1), GetBuffer(), mRefData->length);
		memcpy((char*)(data+1) + mRefData->length, s.GetBuffer(), s.mRefData->length + 1);
		mRefData = data;
	} else {
		mRefData = (RefCountedData*)realloc(mRefData, len + 2 + sizeof(RefCountedData));
		memcpy((char*)(mRefData+1) + mRefData->length, s.GetBuffer(), s.mRefData->length + 1);
	}
	mRefData->length = len;
	mRefData->refcount = 1;
	return *this;
}

bool
String::operator==(const String& s) const
{
	if (Length() != s.Length())
		return false;
	return strcmp(GetBuffer(), s.GetBuffer()) == 0;
}

ptrdiff_t
String::Compare(const String& s) const
{
	if (Length() == s.Length()) {
		return strcmp(GetBuffer(), s.GetBuffer());
	} else {
		return (Length() < s.Length())?-1:1;
	}
}

bool
String::StartsWith(const String& s) const
{
	if (s.Length() > Length()) {
		return false;
	}
	char* s1 = GetBuffer();
	char* s2 = s.GetBuffer();
	while(*s2) {
		if (*s1++ != *s2++)
			return false;
	}
	return true;
}

bool
String::EndsWith(const String& s) const
{
	assert(s.Length() > 0);
	if (s.Length() > Length())
		return false;
	char* s1 = GetBuffer() + Length();
	char* s2 = s.GetBuffer() + s.Length();
	for(int i = s.Length()-1; i; i--) {
		if (*s1-- != *s2--)
			return false;
	}
	return true;
}

String
String::Substring(int offset, int count)
{
	argused(offset);
	argused(count);
	//TODO implement it
	return "";
}

String
String::Substring(int offset)
{
	return Substring(offset, Length() - offset);
}

