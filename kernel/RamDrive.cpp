#include "RamDrive.h"

#include <string.h>
#include <stdlib.h>

RamDrive::RamDrive(ptr_t data, const size_t blocks) :
	_data((char*) data), _blocksQty(blocks) {
}

RamDrive::~RamDrive() {

}

bool RamDrive::ReadBlock(size_t offset, ptr_t buffer) {
	memcpy(buffer, _data + (offset * BlockSize), BlockSize);
	return true;
}

bool RamDrive::WriteBlock(size_t offset, const ptr_t buffer) {
	memcpy(_data + (offset * BlockSize), buffer, BlockSize);
	return true;
}

size_t RamDrive::ReadBlocks(size_t offset, size_t qty, ptr_t buffer) {
	memcpy(buffer, _data + (offset * BlockSize), BlockSize * qty);
	return qty;
}

size_t RamDrive::WriteBlocks(size_t offset, size_t qty, const ptr_t buffer) {
	memcpy(_data + (offset * BlockSize), buffer, BlockSize * qty);
	return qty;
}

size_t RamDrive::Read(size_t offset, size_t qty, ptr_t buffer) {
	memcpy(buffer, _data + offset, qty);
	return qty;
}

size_t RamDrive::Write(size_t offset, size_t qty, const ptr_t buffer) {
	memcpy(_data + offset, buffer, qty);
	return qty;
}

size_t RamDrive::GetBlocksQty() {
	return _blocksQty;
}

bool RamDrive::IsCached() const {
	return true;
}

String RamDrive::GetDescription() const {
	return String("RamDrive");
}
