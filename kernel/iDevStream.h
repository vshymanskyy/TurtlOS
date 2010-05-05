#pragma once
#include "iDevice.h"

namespace DreamDDM {

	class iDevStream
		: public iDevice
	{

	public:
		virtual int ReadChar() = 0;
		virtual bool WriteChar(const int value) = 0;

		virtual size_t ReadString(String *string);
		virtual size_t WriteString(const String string);

		DriverType GetType() const;
		virtual ~iDevStream() {};

	protected:
		iDevStream();

	};

}

