#include "iDevStream.h"

namespace DreamDDM {

iDevStream::iDevStream() {

}

size_t
iDevStream::ReadString(String* string)
{
	argused(string);
	return 0;
}

size_t
iDevStream::WriteString(const String string)
{
	argused(string);
	return 0;
}

iDevice::DriverType
iDevStream::GetType() const
{
	return DevTypeStream;
}

}
