#include "iDevFileSystem.h"

namespace DreamDDM {

iDevFileSystem::iDevFileSystem (iDevDrive* drive)
	: _drive(drive)
{
}

iDevice::DriverType
iDevFileSystem::GetType() const
{
	return DevTypeFileSystem;
}

}
