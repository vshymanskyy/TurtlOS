#ifndef MULTIBOOT_H_
#define MULTIBOOT_H_

class MultibootParser {

	struct multiboot_info*	_table;
	struct module*			_modules;
	struct memory_map*		_memMap;

	MultibootParser();

public:

	static MultibootParser* Instance();

	struct Module {
		void*		data;
		uint32_t	length;
		char*		command;
	};

	struct MemoryMap {
		enum Type {
		    USABLE = 1,
		    RESERVED = 2,
		    ACPI_RECL = 3,
		    ACPI_NVS = 4,
		    BAD = 5
		};

		void* addr;
		size_t length;
		Type type;
	};

	bool GetModule(uint32_t index, Module& mod) const;
	uint32_t GetMemoryMap(MemoryMap*& mmap) const;

	uint32_t GetModulesQty() const;

	size_t GetUpperMemory() const;
	size_t GetLowerMemory() const;
	char* GetCommandLine() const;
	char* GetLoader() const;
};

#endif /* MULTIBOOT_H_ */
