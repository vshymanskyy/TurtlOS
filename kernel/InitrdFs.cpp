#include "InitrdFs.h"
#include "DeviceManager.h"

using namespace DreamDDM;

namespace InitrdFs {

#define INITRDFS_FN_LEN 120
#define INITRDFS_MAGIC 0x73264320
#define INITRDFS_VERSION 0x01

struct InitrdHeader {
	uint32_t magic;
	uint8_t version;
	uint32_t qty;
};

struct InitrdFileHeader {
	uint32_t offset;
	uint32_t length;
	char name[INITRDFS_FN_LEN];
};

InitrdFsFileData::InitrdFsFileData(InitrdFs* root)
	: _rootNode(root)
{
}

size_t
InitrdFsFileData::Read (size_t offset, size_t qty, ptr buffer)
{
	return _rootNode->_drive->Read(_rootNode->_fileHeaders[Inode].offset+offset, qty, buffer);
}

size_t
InitrdFsFileData::Write (size_t offset, size_t qty, const ptr buffer)
{
	argused(offset);
	argused(qty);
	argused(buffer);
	return 0;
}

void
Listener::OnDriverRegistered(iDevice* dev)
{
	if (dev->GetType() == iDevice::DevTypeDrive) {
		iDevDrive* drive = static_cast<iDevDrive*>(dev);
		InitrdHeader header;
		drive->Read(0, sizeof(InitrdHeader), &header);
		if (header.magic == INITRDFS_MAGIC && header.version == INITRDFS_VERSION) {
			DeviceManager::Instance()->RegisterDevice(new InitrdFs(drive));
		}
	}
}

InitrdFs::InitrdFs(iDevDrive* drive)
	: iDevFileSystem(drive)
{
	InitrdHeader header;
	drive->Read(0, sizeof(InitrdHeader), &header);
	_fileQty = header.qty;
	_fileHeaders = new InitrdFileHeader[_fileQty];
	drive->Read(sizeof(InitrdHeader), sizeof(InitrdFileHeader)*_fileQty, _fileHeaders);
}

InitrdFs::~InitrdFs() {
	if (_fileHeaders) {
		delete[] _fileHeaders;
	}
}

FileList
InitrdFs::GetFiles()
{
	FileList result;
	for(unsigned i=0; i<_fileQty; i++) {
		File n(new InitrdFsFileData(this));
		n->Inode = i;
		n->Name = _fileHeaders[i].name;
		n->Length = _fileHeaders[i].length;
		n->UID = n->GID = n->Mask = 0;
		result.Append(n);
	}
	return result;
}

File
InitrdFs::FindFile(const String name)
{
	for(unsigned i=0; i<_fileQty; i++) {
		if (name == String(_fileHeaders[i].name)) {
			InitrdFsFileData* data = new InitrdFsFileData(this);
			data->Inode = i;
			data->Name = _fileHeaders[i].name;
			data->Length = _fileHeaders[i].length;
			data->UID = data->GID = data->Mask = 0;
			return File(data);
		}
	}
	return File();
}

String
InitrdFs::GetDescription() const{
	return String("InitrdFs on <") + _drive->GetDescription() + String(">");
}

}

