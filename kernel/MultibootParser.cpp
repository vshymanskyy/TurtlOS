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
	uint32_t header_addr;
	uint32_t load_addr;
	uint32_t load_end_addr;
	uint32_t bss_end_addr;
	uint32_t entry_addr;
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
	uint32_t mem_lower;
	uint32_t mem_upper;
	uint32_t boot_device;
	uint32_t cmdline;
	uint32_t mods_count;
	uint32_t mods_addr;
	union {
		aout_symbol_table aout_sym;
		elf_section_header_table elf_sec;
	} u;
	uint32_t mmap_length;
	uint32_t mmap_addr;
	uint32_t drives_length;
	uint32_t drives_addr;
	uint32_t config_table;
	uint32_t boot_loader_name;
	uint32_t apm_table;
	struct {
		uint32_t control_info;
		uint32_t mode_info;
		uint32_t mode;
		uint32_t interface_seg;
		uint32_t interface_off;
		uint32_t interface_len;
	} vbe;
} PACKED;

struct module {
	uint32_t mod_start;
	uint32_t mod_end;
	uint32_t string;
	uint32_t reserved;
} PACKED;

struct memory_map {
	uint32_t size;
	uint32_t base_addr_low;
	uint32_t base_addr_high;
	uint32_t length_low;
	uint32_t length_high;
	uint32_t type;
} PACKED;


MultibootParser* MultibootParser::Instance()
{
	static MultibootParser instance;
	return &instance;
}

MultibootParser::MultibootParser() {
	assert(mbiMagic == MULTIBOOT_BOOTLOADER_MAGIC);
	mTable = (multiboot_info*)mbiTable;
	mModules = (mTable->flags && MULTIBOOT_FLAG_MODS) ? (module*)mTable->mods_addr : NULL;
	mMemMap  = (mTable->flags && MULTIBOOT_FLAG_MMAP) ? (memory_map*)mTable->mmap_addr : NULL;
}

uint32_t MultibootParser::GetModulesCount() const {
	return (mModules == NULL) ? 0 : mTable->mods_count;
}

char* MultibootParser::GetCommandLine() const {
	return (mTable && mTable->flags && MULTIBOOT_FLAG_CMDLINE) ? (char*)mTable->cmdline : NULL;
}

char* MultibootParser::GetLoader() const {
	return (mTable && mTable->flags && MULTIBOOT_FLAG_LOADER) ? (char*)mTable->boot_loader_name : NULL;
}

size_t MultibootParser::GetUpperMemory() const {
	return (mTable && mTable->flags && MULTIBOOT_FLAG_MEM) ? mTable->mem_upper : 0;
}

size_t MultibootParser::GetLowerMemory() const {
	return (mTable->flags && MULTIBOOT_FLAG_MEM) ? mTable->mem_lower : 0;
}

bool MultibootParser::GetModule(uint32_t index, Module& mod) const {
	if (!mTable || index >= mTable->mods_count || mModules == NULL)
		return false;
	module& m = mModules[index];
	mod.command = (char*)m.string;
	mod.data = (void*)m.mod_start;
	mod.length = m.mod_end - m.mod_start;
	return true;
}

uint32_t MultibootParser::GetMemoryMap(MemoryMap*& mmap) const {
	if (!(mTable && mTable->flags && MULTIBOOT_FLAG_MMAP) || mTable->mmap_length == 0 || mTable->mmap_addr == 0)
		return 0;

	mmap = new MemoryMap[mTable->mmap_length];
	int i = 0;
	for (memory_map* m = (memory_map*)mTable->mmap_addr;
			m < (memory_map*)(mTable->mmap_addr + mTable->mmap_length);
			m = (memory_map*)((uint8_t*)m + m->size + sizeof(m->size)))
	{
		mmap[i].addr	= (void*)(((uint64_t)m->base_addr_high << 32) | (uint64_t)m->base_addr_low);
		mmap[i].length	= (size_t)((uint64_t)m->length_high << 32) | (uint64_t)m->length_low;
		mmap[i].type	= (MemoryMap::Type)m->type;
		i++;
	}
	return i;
}

