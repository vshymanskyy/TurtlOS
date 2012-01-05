#include "iDevDrive.h"

namespace DreamDDM {

const size_t iDevDrive::BlockSize = 512;

iDevDrive::iDevDrive() {

}

iDevDrive::~iDevDrive() {

}

size_t iDevDrive::ReadBlocks(size_t offset, size_t qty, ptr_t buffer) {
	size_t i = 0;
	for(; i<qty; i++) {
		if (!ReadBlock(offset+i, static_cast<char*>(buffer) + (i * BlockSize)))
			break;
	}
	return i;
}

size_t iDevDrive::WriteBlocks(size_t offset, size_t qty, const ptr_t buffer) {
	size_t i = 0;
	for(; i<qty; i++) {
		if (!WriteBlock(offset+i, static_cast<char*>(buffer) + (i * BlockSize)))
			break;
	}
	return i;
}

size_t iDevDrive::Read(size_t offset, size_t qty, ptr_t buffer) {
	argused(offset);
	argused(qty);
	argused(buffer);
	return 0;
}

size_t iDevDrive::Write(size_t offset, size_t qty, const ptr_t buffer) {
	argused(offset);
	argused(qty);
	argused(buffer);
	return 0;
}

bool iDevDrive::IsCached() const {
	return false;
}

iDevice::DriverType iDevDrive::GetType() const{
	return DevTypeDrive;
}

}
