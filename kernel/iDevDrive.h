#pragma once
#include "iDevice.h"

namespace DreamDDM {

	class iDevDrive
		: public iDevice
	{

	public:
		virtual bool ReadBlock(size_t offset, ptr buffer) = 0;
		virtual bool WriteBlock(size_t offset, const ptr buffer) = 0;
		virtual size_t GetBlocksQty() = 0;

		virtual size_t ReadBlocks(size_t offset, size_t qty, ptr buffer);
		virtual size_t WriteBlocks(size_t offset, size_t qty, const ptr buffer);

		virtual size_t Read(size_t offset, size_t qty, ptr buffer);
		virtual size_t Write(size_t offset, size_t qty, const ptr buffer);

		virtual bool IsCached() const;

		DriverType GetType() const;
		virtual ~iDevDrive();

	protected:
		iDevDrive();

		static const size_t BlockSize;

	};

}

