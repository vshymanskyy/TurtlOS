#pragma once
#include "iDevFileSystem.h"
#include "DeviceManager.h"

using namespace DreamDDM;

namespace InitrdFs {

class InitrdFs;
struct InitrdHeader;
struct InitrdFileHeader;

class InitrdFsFileData: public VirtualFsFileData {
public:
	InitrdFsFileData(InitrdFs* root);

	size_t Read(size_t offset, size_t qty, ptr_t buffer);
	size_t Write(size_t offset, size_t qty, const ptr_t buffer);

	virtual ~InitrdFsFileData() {}

private:
	InitrdFs* _rootNode;
};

class InitrdFs
	: public iDevFileSystem
{
	friend class InitrdFsFileData;

public:
	InitrdFs(iDevDrive* drive);
	virtual ~InitrdFs();

	FileList GetFiles();
	File FindFile(const String name);

	String GetDescription() const;

private:
	uint32_t			_fileQty;
	InitrdFileHeader*	_fileHeaders;
};

class Listener: public DeviceManager::Listener {
public:
	void OnDriverRegistered(iDevice* dev);
};

}
