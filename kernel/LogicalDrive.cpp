#include "LogicalDrive.h"
#include "DeviceManager.h"

using namespace DreamDDM;

namespace LogicalDrive {

LogicalDrive::LogicalDrive (iDevDrive* drive, const size_t offset, const size_t blocksQty)
	: _drive(drive), _startBlock(offset), _blocksQty(blocksQty)
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
			DeviceManager::Instance()->RegisterDevice(new LogicalDrive(drive, 0, drive->GetBlocksQty()));
		}
	}
}

size_t
LogicalDrive::Read(size_t offset, size_t qty, ptr buffer)
{
	return _drive->Read(_startBlock*BlockSize + offset, qty, buffer);
}

size_t
LogicalDrive::Write(size_t offset, size_t qty, const ptr buffer)
{
	return _drive->Read(_startBlock*BlockSize + offset, qty, buffer);
}

size_t
LogicalDrive::ReadBlocks(size_t offset, size_t qty, ptr buffer)
{
	assert(offset + qty <= _blocksQty);
	return _drive->ReadBlocks(offset + _startBlock, qty, buffer);
}

size_t
LogicalDrive::WriteBlocks(size_t offset, size_t qty, const ptr buffer)
{
	assert(offset + qty <= _blocksQty);
	return _drive->WriteBlocks(offset + _startBlock, qty, buffer);
}

size_t
LogicalDrive::GetBlocksQty()
{
	return _blocksQty;
}

bool
LogicalDrive::LogicalDrive::ReadBlock(size_t offset, ptr buffer)
{
	assert(offset < _blocksQty);
	return _drive->ReadBlock(offset + _startBlock, buffer);
}

bool
LogicalDrive::LogicalDrive::WriteBlock(size_t offset, const ptr buffer)
{
	assert(offset < _blocksQty);
	return _drive->WriteBlock(offset + _startBlock, buffer);
}

bool
LogicalDrive::LogicalDrive::IsCached() const
{
	return _drive->IsCached();
}

String
LogicalDrive::LogicalDrive::GetDescription() const{
	return String("Logical <") + _drive->GetDescription() + String(">");
}

}
