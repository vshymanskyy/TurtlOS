#include "CachedDrive.h"
#include "DeviceManager.h"

#include <memory.h>

using namespace DreamDDM;

namespace CachedDrive {

void
Listener::OnDriverRegistered (iDevice* dev)
{
	if (dev->GetType() == iDevice::DevTypeDrive) {
		iDevDrive* drive = static_cast<iDevDrive*>(dev);
		if (!drive->IsCached()) {
			DeviceManager::Instance()->RegisterDevice(new CachedDrive(drive, drive->GetBlocksCount()));
		}
	}
}

CachedDrive::CachedDrive(iDevDrive* drive, size_t cachedBlocksCount)
	: mDrive(drive), mCachedBlocksCount(cachedBlocksCount)
{

}

CachedDrive::~CachedDrive() {

}

size_t
CachedDrive::Read(size_t offset, size_t count, ptr buffer)
{
	return mDrive->Read(offset, count, buffer);
}

size_t
CachedDrive::Write(size_t offset, size_t count, const ptr buffer)
{
	return mDrive->Write(offset, count, buffer);
}

size_t
CachedDrive::GetBlocksCount()
{
	return mDrive->GetBlocksCount();
}

bool
CachedDrive::ReadBlock(size_t offset, ptr buffer)
{	return mDrive->ReadBlock(offset, buffer);
}

bool
CachedDrive::WriteBlock(size_t offset, const ptr buffer)
{	return mDrive->WriteBlock(offset, buffer);
}

bool
CachedDrive::IsCached() const
{
	return true;
}

String
CachedDrive::GetDescription() const
{
	return String("Cached <") + mDrive->GetDescription() + String(">");
}

CachedDrive::CachedBlock::CachedBlock()
{
	data = new char[CachedDrive::_BlockSize];
	memset(data, 0, CachedDrive::_BlockSize);
}

CachedDrive::CachedBlock::~CachedBlock()
{
	delete[] data;
}

}
