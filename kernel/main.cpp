#include <global.h>
#include "Console.h"
#include "CliDesktop.h"
#include "CliFrame.h"

#include "DeviceManager.h"
#include "RamDrive.h"
//#include "LogicalDrive.h"
//#include "CachedDrive.h"
#include "InitrdFs.h"

#include <lapic.h>
#include <keyboard.h>
#include <mouse.h>

#include <std/Utils.h>

#include "MultibootParser.h"
#include "ImpsParser.h"
#include "Processors.h"

#include "Heap.h"

Console* console;
CliDesktop* desktop;

int main();

int main() {
	desktop = new CliDesktop(Rect(0, 0, 80, 25));
	console = new Console(Rect(1, 1, 78, 23));

	desktop->AddChild(console);

	if (char* bootl = MultibootParser::Instance()->GetLoader()) {
		(*console) << "Loader:  " << bootl << endl;
	}
	(*console) << "Kernel:  TurtlOS " __string_def(TARGET_PATH) " (" << __COMPILER__ << " on " << __DATE__ << ' ' << __TIME__ << ")" << endl;
	if (char* cmdl = MultibootParser::Instance()->GetCommandLine()) {
		(*console) << "Command: " << cmdl << endl;
	}
	
	/*
	MultibootParser::MemoryMap* mmap = NULL;
	uint32_t mmap_len = MultibootParser::Instance()->GetMemoryMap(mmap);
	static const char* mmapTypes[6] = { "Usable", "Reserved", "Acpi recl", "Acpi nvs", "Bad" };
	if (mmap) {
		for (uint32_t i = 0; i < mmap_len; i++) {
			(*console) << mmap[i].addr << ": " << (uint64_t)mmap[i].length << " - " << mmapTypes[mmap[i].type - 1] << endl;
		}
		delete[] mmap;
	}*/

	DeviceManager::Instance()->RegisterListener(new InitrdFs::Listener());
	for (uint32_t i = 0; i < MultibootParser::Instance()->GetModulesQty(); i++) {
		MultibootParser::Module mod;
		if (MultibootParser::Instance()->GetModule(i, mod)) {
			//(*console) << "Module:  " << mod.command << " at " << mod.data << endl;
			mallocat(mod.data, mod.length);
			DeviceManager::Instance()->RegisterDevice(new RamDrive(mod.data, mod.length / 512 + 1));
		}
	}

	lapicInit();

	//(*console) << "LAPIC at: " << (ptr_t)lapicGetBase() << endl;

	ImpsParser::Instance();
	Processors::Instance()->Startup();


	keyboardInit();
	//mouseInit();

	for(;;) {
		(*console) << (char)keyboardGetChar();
	}

	delete console;
	delete desktop;

	return 0;
}
