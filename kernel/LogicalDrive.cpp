#include "LogicalDrive.h"
#include "DeviceManager.h"

using namespace DreamDDM;

namespace LogicalDrive {

LogicalDrive::LogicalDrive (iDevDrive* drive, const size_t offset, const size_t count)
	: mDrive(drive), mOffset(offset), mBlocksCount(count)
{
}

LogicalDrive::~LogicalDrive()
{
}

void
Listener::OnDriverRegistered (iDevice* dev)
{
	if (dev->GetType() == iDevice::DevTypeDrive) {
		iDevDrive* drive = static_cast<iDevDrive*>(dev);
		if (drive->IsCached()) {
			DeviceManager::Instance()->RegisterDevice(new LogicalDrive(drive, 0, drive->GetBlocksCount()));
		}
	}
}

size_t
LogicalDrive::Read(size_t offset, size_t count, ptr buffer)
{
	return mDrive->Read(mOffset*_BlockSize + offset, count, buffer);
}

size_t
LogicalDrive::Write(size_t offset, size_t count, const ptr buffer)
{
	return mDrive->Read(mOffset*_BlockSize + offset, count, buffer);
}

size_t
LogicalDrive::ReadBlocks(size_t offset, size_t count, ptr buffer)
{
	assert(offset + count <= mBlocksCount);
	return mDrive->ReadBlocks(offset + mOffset, count, buffer);
}

size_t
LogicalDrive::WriteBlocks(size_t offset, size_t count, const ptr buffer)
{
	assert(offset + count <= mBlocksCount);
	return mDrive->WriteBlocks(offset + mOffset, count, buffer);
}

size_t
LogicalDrive::GetBlocksCount()
{
	return mBlocksCount;
}

bool
LogicalDrive::LogicalDrive::ReadBlock(size_t offset, ptr buffer)
{
	assert(offset < mBlocksCount);
	return mDrive->ReadBlock(offset + mOffset, buffer);
}

bool
LogicalDrive::LogicalDrive::WriteBlock(size_t offset, const ptr buffer)
{
	assert(offset < mBlocksCount);
	return mDrive->WriteBlock(offset + mOffset, buffer);
}

bool
LogicalDrive::LogicalDrive::IsCached() const
{
	return mDrive->IsCached();
}

String
LogicalDrive::LogicalDrive::GetDescription() const{
	return String("Logical <") + mDrive->GetDescription() + String(">");
}

}
