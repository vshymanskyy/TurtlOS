#pragma once
#include "iDevDrive.h"
#include "DeviceManager.h"

using namespace DreamDDM;

namespace LogicalDrive {

class LogicalDrive: public iDevDrive {

public:
	LogicalDrive(iDevDrive* drive, const size_t offset, const size_t blocksQty);
	~LogicalDrive();

	String GetDescription() const;

	bool ReadBlock(size_t offset, ptr buffer);
	bool WriteBlock(size_t offset, const ptr buffer);
	size_t ReadBlocks(size_t offset, size_t qty, ptr buffer);
	size_t WriteBlocks(size_t offset, size_t qty, const ptr buffer);

	size_t Read(size_t offset, size_t qty, ptr buffer);
	size_t Write(size_t offset, size_t qty, const ptr buffer);

	size_t GetBlocksQty();

	bool IsCached() const;

private:
	iDevDrive*	_drive;
	size_t		_startBlock;
	size_t		_blocksQty;

};

class Listener: public DeviceManager::Listener {
public:
	void OnDriverRegistered(iDevice* dev);
};

}
