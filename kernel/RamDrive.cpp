#include "RamDrive.h"

#include <memory.h>

RamDrive::RamDrive(ptr data, const size_t blocks) :
	mData((char*) data), mBlocksCount(blocks) {
}

RamDrive::~RamDrive() {

}

bool RamDrive::ReadBlock(size_t offset, ptr buffer) {
	memcpy(buffer, mData + (offset * _BlockSize), _BlockSize);
	return true;
}

bool RamDrive::WriteBlock(size_t offset, const ptr buffer) {
	memcpy(mData + (offset * _BlockSize), buffer, _BlockSize);
	return true;
}

size_t RamDrive::ReadBlocks(size_t offset, size_t count, ptr buffer) {
	memcpy(buffer, mData + (offset * _BlockSize), _BlockSize * count);
	return count;
}

size_t RamDrive::WriteBlocks(size_t offset, size_t count, const ptr buffer) {
	memcpy(mData + (offset * _BlockSize), buffer, _BlockSize * count);
	return count;
}

size_t RamDrive::Read(size_t offset, size_t count, ptr buffer) {
	memcpy(buffer, mData + offset, count);
	return count;
}

size_t RamDrive::Write(size_t offset, size_t count, const ptr buffer) {
	memcpy(mData + offset, buffer, count);
	return count;
}

size_t RamDrive::GetBlocksCount() {
	return mBlocksCount;
}

bool RamDrive::IsCached() const {
	return true;
}

String RamDrive::GetDescription() const {
	return String("RamDrive");
}
