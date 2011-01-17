#pragma once
#include "iDevDrive.h"

using namespace DreamDDM;

class RamDrive
	: public iDevDrive
{

public:
	//RamDrive(const size_t blocks);
	RamDrive(ptr data, const size_t blocks);
	virtual ~RamDrive();

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
	char*	_data;
	size_t	_blocksQty;
};
