#include "iDevice.h"
#include "DeviceManager.h"

namespace DreamDDM {

iDevice::iDevice()
{
}

iDevice::~iDevice()
{
	DeviceManager::Instance()->UnregisterDevice(this);
}

String
iDevice::GetDescription() const
{
	return "Unknown Device";
}

iDevice::DriverType
iDevice::GetType() const
{
	return DevTypeUnknown;
}

}
