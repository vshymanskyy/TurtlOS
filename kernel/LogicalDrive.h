#pragma once
#include "iDevDrive.h"
#include "DeviceManager.h"

using namespace DreamDDM;

namespace LogicalDrive {

class LogicalDrive: public iDevDrive {

public:
	LogicalDrive(iDevDrive* drive, const size_t mOffset, const size_t mBlocksCount);
	~LogicalDrive();

	String GetDescription() const;

	bool ReadBlock(size_t offset, ptr buffer);
	bool WriteBlock(size_t offset, const ptr buffer);
	size_t ReadBlocks(size_t offset, size_t count, ptr buffer);
	size_t WriteBlocks(size_t offset, size_t count, const ptr buffer);

	size_t Read(size_t offset, size_t count, ptr buffer);
	size_t Write(size_t offset, size_t count, const ptr buffer);

	size_t GetBlocksCount();

	bool IsCached() const;

private:
	iDevDrive* mDrive;
	size_t mOffset, mBlocksCount;

};

class Listener: public DeviceManager::Listener {
public:
	void OnDriverRegistered(iDevice* dev);
};

}
