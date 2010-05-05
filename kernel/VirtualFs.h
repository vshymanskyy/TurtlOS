#pragma once
#include <std/String.h>
#include <std/List.h>
#include <std/SmartPtr.h>

namespace VirtualFs {

	class VirtualFsFileData;
	class VirtualFsDirData;

	typedef SmartPtr<VirtualFsFileData> File;
	typedef SmartPtr<VirtualFsDirData> Directory;
	typedef List<File> FileList;
	typedef List<Directory> DirList;

	class VirtualFsData
		: public SmartPtrData
	{
	public:
		String Name;		// The filename
		uint32_t Mask;		// The permissions mask
		uint32_t UID;		// The owning user
		uint32_t GID;		// The owning group
		uint32_t Flags;		// Includes the node type
		uint32_t Inode;		// This is device-specific - provides a way for a filesystem to identify files
		uint32_t Impl;		// An implementation-defined number
		SmartPtr<VirtualFsData> Link;	// Used by mountpoints and symlinks

		VirtualFsData() : Link(NULL) {}
		virtual ~VirtualFsData() {}
	};

	class VirtualFsFileData
		: public VirtualFsData
	{
	public:
		size_t Length;

		virtual size_t Read(size_t offset, size_t count, ptr buffer) = 0;
		virtual size_t Write(size_t offset, size_t count, const ptr buffer) = 0;

		virtual ~VirtualFsFileData() {}
	};

	class VirtualFsDirData
		: public VirtualFsData
	{
	public:
		virtual FileList GetFiles();
		virtual File FindFile(const String name);
		virtual DirList GetDirectories();
		virtual Directory FindDir(const String name);

		virtual ~VirtualFsDirData() {}
	};

}

