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
	uint32_t count;
};

struct InitrdFileHeader {
	uint32_t offset;
	uint32_t length;
	char name[INITRDFS_FN_LEN];
};

InitrdFsFileData::InitrdFsFileData(InitrdFs* root)
	: mRootNode(root)
{
}

size_t
InitrdFsFileData::Read (size_t offset, size_t count, ptr buffer)
{
	return mRootNode->mDrive->Read(mRootNode->mFileHeaders[Inode].offset+offset, count, buffer);
}

size_t
InitrdFsFileData::Write (size_t offset, size_t count, const ptr buffer)
{
	argused(offset);
	argused(count);
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
	mCount = header.count;
	mFileHeaders = new InitrdFileHeader[mCount];
	drive->Read(sizeof(InitrdHeader), sizeof(InitrdFileHeader)*mCount, mFileHeaders);
}

InitrdFs::~InitrdFs() {
	if (mFileHeaders) {
		delete[] mFileHeaders;
	}
}

FileList
InitrdFs::GetFiles()
{
	FileList result;
	for(unsigned i=0; i<mCount; i++) {
		File n(new InitrdFsFileData(this));
		n->Inode = i;
		n->Name = mFileHeaders[i].name;
		n->Length = mFileHeaders[i].length;
		n->UID = n->GID = n->Mask = 0;
		result.Append(n);
	}
	return result;
}

File
InitrdFs::FindFile(const String name)
{
	for(unsigned i=0; i<mCount; i++) {
		if (name == String(mFileHeaders[i].name)) {
			InitrdFsFileData* data = new InitrdFsFileData(this);
			data->Inode = i;
			data->Name = mFileHeaders[i].name;
			data->Length = mFileHeaders[i].length;
			data->UID = data->GID = data->Mask = 0;
			return File(data);
		}
	}
	return File();
}

String
InitrdFs::GetDescription() const{
	return String("InitrdFs on <") + mDrive->GetDescription() + String(">");
}

}

