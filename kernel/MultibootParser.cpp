#include "MultibootParser.h"

#define MULTIBOOT_HEADER_MAGIC          0x1BADB002
#define MULTIBOOT_BOOTLOADER_MAGIC      0x2BADB002

#define MULTIBOOT_FLAG_MEM     0x001
#define MULTIBOOT_FLAG_DEVICE  0x002
#define MULTIBOOT_FLAG_CMDLINE 0x004
#define MULTIBOOT_FLAG_MODS    0x008
#define MULTIBOOT_FLAG_AOUT    0x010
#define MULTIBOOT_FLAG_ELF     0x020
#define MULTIBOOT_FLAG_MMAP    0x040
#define MULTIBOOT_FLAG_CONFIG  0x080
#define MULTIBOOT_FLAG_LOADER  0x100
#define MULTIBOOT_FLAG_APM     0x200
#define MULTIBOOT_FLAG_VBE     0x400

extern uint32_t mbiMagic;
extern uint32_t mbiTable;

struct multiboot_header {
	uint32_t magic;
	uint32_t flags;
	uint32_t checksum;
	uint32_t headerAddr;
	uint32_t loadAddr;
	uint32_t loadEndAddr;
	uint32_t bssEndAddr;
	uint32_t entryAddr;
} PACKED;

struct aout_symbol_table {
	uint32_t tabsize;
	uint32_t strsize;
	uint32_t addr;
	uint32_t reserved;
} PACKED;

struct elf_section_header_table {
	uint32_t num;
	uint32_t size;
	uint32_t addr;
	uint32_t shndx;
} PACKED;

struct multiboot_info {
	uint32_t flags;
	uint32_t memLower;
	uint32_t memUpper;
	uint32_t bootDevice;
	uint32_t cmdLine;
	uint32_t modQty;
	uint32_t modAddr;
	union {
		aout_symbol_table aoutSym;
		elf_section_header_table elfSec;
	} u;
	uint32_t mmapLength;
	uint32_t mmapAddr;
	uint32_t drivesLength;
	uint32_t drivesAddr;
	uint32_t configTable;
	uint32_t bootLoaderName;
	uint32_t apmTable;
	struct {
		uint32_t controlInfo;
		uint32_t modeInfo;
		uint32_t mode;
		uint32_t interface_seg;
		uint32_t interfaceOff;
		uint32_t interfaceLen;
	} vbe;
} PACKED;

struct module {
	uint32_t modStart;
	uint32_t modEnd;
	uint32_t string;
	uint32_t reserved;
} PACKED;

struct memory_map {
	uint32_t size;
	uint32_t baseAddrLow;
	uint32_t baseAddrHigh;
	uint32_t lengthLow;
	uint32_t lengthHigh;
	uint32_t type;
} PACKED;


MultibootParser* MultibootParser::Instance()
{
	static MultibootParser instance;
	return &instance;
}

MultibootParser::MultibootParser() {
	assert(mbiMagic == MULTIBOOT_BOOTLOADER_MAGIC);
	_table = (multiboot_info*)mbiTable;
	_modules = (_table->flags && MULTIBOOT_FLAG_MODS) ? (module*)_table->modAddr : NULL;
	_memMap  = (_table->flags && MULTIBOOT_FLAG_MMAP) ? (memory_map*)_table->mmapAddr : NULL;
}

uint32_t MultibootParser::GetModulesQty() const {
	return (_modules == NULL) ? 0 : _table->modQty;
}

char* MultibootParser::GetCommandLine() const {
	return (_table && _table->flags && MULTIBOOT_FLAG_CMDLINE) ? (char*)_table->cmdLine : NULL;
}

char* MultibootParser::GetLoader() const {
	return (_table && _table->flags && MULTIBOOT_FLAG_LOADER) ? (char*)_table->bootLoaderName : NULL;
}

size_t MultibootParser::GetUpperMemory() const {
	return (_table && _table->flags && MULTIBOOT_FLAG_MEM) ? _table->memUpper : 0;
}

size_t MultibootParser::GetLowerMemory() const {
	return (_table->flags && MULTIBOOT_FLAG_MEM) ? _table->memLower : 0;
}

bool MultibootParser::GetModule(uint32_t index, Module& mod) const {
	if (!_table || index >= _table->modQty || !_modules) {
		return false;
	}
	module& m = _modules[index];
	mod.command = (char*)m.string;
	mod.data = (void*)m.modStart;
	mod.length = m.modEnd - m.modStart;
	return true;
}

uint32_t MultibootParser::GetMemoryMap(MemoryMap*& mmap) const {
	if (!(_table && _table->flags && MULTIBOOT_FLAG_MMAP) || !_table->mmapLength || !_table->mmapAddr) {
		return 0;
	}

	mmap = new MemoryMap[_table->mmapLength];
	int i = 0;
	for (memory_map* m = (memory_map*)_table->mmapAddr;
			m < (memory_map*)(_table->mmapAddr + _table->mmapLength);
			m = (memory_map*)((uint8_t*)m + m->size + sizeof(m->size)))
	{
		mmap[i].addr	= (void*)(((uint64_t)m->baseAddrHigh << 32) | (uint64_t)m->baseAddrLow);
		mmap[i].length	= (size_t)((uint64_t)m->lengthHigh << 32) | (uint64_t)m->lengthLow;
		mmap[i].type	= (MemoryMap::Type)m->type;
		i++;
	}
	return i;
}
