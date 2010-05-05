#pragma once
#include "iDevDrive.h"
#include "DeviceManager.h"

using namespace DreamDDM;

namespace CachedDrive {

class CachedDrive: public iDevDrive {

public:
	CachedDrive(iDevDrive* drive, size_t cachedBlocksCount);
	~CachedDrive();

	String GetDescription() const;

	bool ReadBlock(size_t offset, ptr buffer);
	bool WriteBlock(size_t offset, const ptr buffer);

	size_t Read(size_t offset, size_t count, ptr buffer);
	size_t Write(size_t offset, size_t count, const ptr buffer);

	size_t GetBlocksCount();

	bool IsLogical() const;
	bool IsCached() const;

private:
	iDevDrive* mDrive;
	size_t mCachedBlocksCount;

	struct CachedBlock {
		size_t offset;
		bool is_dirty;
		char* data;

		CachedBlock();
		~CachedBlock();
	}* mEntries;
};

class Listener: public DeviceManager::Listener {
public:
	void OnDriverRegistered(iDevice* dev);
};

}
