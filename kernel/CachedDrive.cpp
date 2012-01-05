#include "CachedDrive.h"
#include "DeviceManager.h"

#include <string.h>

using namespace DreamDDM;

namespace CachedDrive {

void
Listener::OnDriverRegistered (iDevice* dev)
{
	if (dev->GetType() == iDevice::DevTypeDrive) {
		iDevDrive* drive = static_cast<iDevDrive*>(dev);
		if (!drive->IsCached()) {
			DeviceManager::Instance()->RegisterDevice(new CachedDrive(drive, drive->GetBlocksQty()));
		}
	}
}

CachedDrive::CachedDrive(iDevDrive* drive, size_t cachedBlocksQty)
	: _drive(drive)
	, _cachedBlocksQty(cachedBlocksQty)
{

}

CachedDrive::~CachedDrive() {

}

size_t
CachedDrive::Read(size_t offset, size_t qty, ptr_t buffer)
{
	return _drive->Read(offset, qty, buffer);
}

size_t
CachedDrive::Write(size_t offset, size_t qty, const ptr_t buffer)
{
	return _drive->Write(offset, qty, buffer);
}

size_t
CachedDrive::GetBlocksQty()
{
	return _drive->GetBlocksQty();
}

bool
CachedDrive::ReadBlock(size_t offset, ptr_t buffer)
{	return _drive->ReadBlock(offset, buffer);
}

bool
CachedDrive::WriteBlock(size_t offset, const ptr_t buffer)
{	return _drive->WriteBlock(offset, buffer);
}

bool
CachedDrive::IsCached() const
{
	return true;
}

String
CachedDrive::GetDescription() const
{
	return String("Cached <") + _drive->GetDescription() + String(">");
}

CachedDrive::CachedBlock::CachedBlock()
{
	data = new char[CachedDrive::BlockSize];
	memset(data, 0, CachedDrive::BlockSize);
}

CachedDrive::CachedBlock::~CachedBlock()
{
	delete[] data;
}

}
