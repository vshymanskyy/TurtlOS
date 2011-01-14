#include "Console.h"
#include "CliDesktop.h"
#include "CliFrame.h"

#include "DeviceManager.h"
#include "RamDrive.h"
//#include "LogicalDrive.h"
//#include "CachedDrive.h"
#include "InitrdFs.h"

#include <hal/lapic.h>
#include <hal/cpu.h>

#include <std/HexDump.h>

#include "MultibootParser.h"
#include "ImpsParser.h"
#include "Processors.h"

#include "Heap.h"

#include "Log.h"
extern Heap heap;

Console* console;
Console* console2;
CliDesktop* desktop;

int main();

const char* data = "hello world";

int main() {
	desktop = new CliDesktop(Rect(0, 0, 80, 25));
	console = new Console(Rect(0, 0, 39, 25));

	console2 = new Console(Rect(40, 0, 39, 25));

	desktop->AddChild(console);
	desktop->AddChild(console2);

	if (char* bootl = MultibootParser::Instance()->GetLoader()) {
		(*console) << "Loader:  " << bootl << endl;
	}
	(*console) << "Kernel:  TurtlOS " << __string(TARGET) << " (" << __COMPILER__ << " on " << __DATE__ << ' ' << __TIME__ << ")" << endl;
	if (char* cmdl = MultibootParser::Instance()->GetCommandLine()) {
		(*console) << "Command: " << cmdl << endl;
	}

	MultibootParser::MemoryMap* mmap = NULL;
	uint32_t mmap_len = MultibootParser::Instance()->GetMemoryMap(mmap);
	static const char* mmapTypes[6] = { "usable", "reserved", "acpi recl", "acpi nvs", "bad" };
	if (mmap) {
		for (uint32_t i = 0; i < mmap_len; i++) {
			(*console) << mmap[i].addr << ": " << mmap[i].length << " - " << mmapTypes[mmap[i].type - 1] << endl;
		}
		delete[] mmap;
	}

	DeviceManager::Instance()->RegisterListener(new InitrdFs::Listener());
	for (uint32_t i = 0; i < MultibootParser::Instance()->GetModulesCount(); i++) {
		MultibootParser::Module mod;
		if (MultibootParser::Instance()->GetModule(i, mod)) {
			(*console) << "Module:  " << mod.command << " at " << mod.data << endl;
			mallocat(mod.data, mod.length);
			DeviceManager::Instance()->RegisterDevice(new RamDrive(mod.data, mod.length / 512 + 1));
		}
	}

	lapicInit();

	(*console) << "LAPIC at: " << (ptr)lapicGetBase() << endl;

	ImpsParser::Instance();
	Processors::Instance()->Startup();

	LogManager::Instance()->AddLogger(new BasicLogViewer(console));

	Log l ("main");
	l() << "hello";

	Processors::Instance()->~Processors();
	DeviceManager::Instance()->~DeviceManager();


	delete console;
	delete console2;
	delete desktop;

	return 0;
}

