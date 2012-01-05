#pragma once
#include "iDevDrive.h"
#include "DeviceManager.h"

using namespace DreamDDM;

namespace CachedDrive {

class CachedDrive: public iDevDrive {

public:
	CachedDrive(iDevDrive* drive, size_t cachedBlocksQty);
	~CachedDrive();

	String GetDescription() const;

	bool ReadBlock(size_t offset, ptr_t buffer);
	bool WriteBlock(size_t offset, const ptr_t buffer);

	size_t Read(size_t offset, size_t qty, ptr_t buffer);
	size_t Write(size_t offset, size_t qty, const ptr_t buffer);

	size_t GetBlocksQty();

	bool IsLogical() const;
	bool IsCached() const;

private:
	iDevDrive*	_drive;
	size_t		_cachedBlocksQty;

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
