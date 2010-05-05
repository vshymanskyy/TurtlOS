#include "iDevFileSystem.h"

namespace DreamDDM {

iDevFileSystem::iDevFileSystem (iDevDrive* drive)
	: mDrive(drive)
{
}

iDevice::DriverType
iDevFileSystem::GetType() const
{
	return DevTypeFileSystem;
}

}
