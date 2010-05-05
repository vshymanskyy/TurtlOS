#pragma once

#include "iDevDrive.h"
#include "VirtualFs.h"

using namespace VirtualFs;

namespace DreamDDM {

	class iDevFileSystem
		: public iDevice
		, public VirtualFsDirData
	{

	public:
		DriverType GetType() const;
		virtual ~iDevFileSystem() {};

	protected:
		iDevFileSystem(iDevDrive* drive);

	protected:
		iDevDrive* mDrive;

	};

}
