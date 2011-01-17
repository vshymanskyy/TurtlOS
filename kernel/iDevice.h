#pragma once
#include <std/String.h>

namespace DreamDDM {

	class iDevice {

	public:
		enum DriverType{
			DevTypeUnknown,
			//-----------------------
			DevTypeDrive,
			DevTypeFileSystem,
			DevTypeStream,
			//-----------------------
			DevTypeQty
		};

		virtual String GetDescription() const = 0;
		virtual DriverType GetType() const;

		virtual ~iDevice();

	protected:
		iDevice();

	};

}
