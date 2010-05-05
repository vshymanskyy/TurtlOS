#include "VirtualFs.h"

namespace VirtualFs {

FileList
VirtualFsDirData::GetFiles()
{
	return FileList();
}

File
VirtualFsDirData::FindFile(const String name)
{
	return File();
}

DirList
VirtualFsDirData::GetDirectories()
{
	return DirList();
}

Directory
VirtualFsDirData::FindDir(const String name)
{
	return Directory();
}

}

