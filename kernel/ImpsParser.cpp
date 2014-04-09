#include "ImpsParser.h"
#include "Processors.h"
#include "Console.h"

#include <string.h>

#define IMPS_MAX_CPUS			0xFF
#define IMPS_FPS_IMCRP_BIT		0x80
#define IMPS_FPS_DEFAULT_MAX	7
#define IMPS_FPS_SIGNATURE		('_' | ('M'<<8) | ('P'<<16) | ('_'<<24))
#define IMPS_CTH_SIGNATURE		('P' | ('C'<<8) | ('M'<<16) | ('P'<<24))

#define	IMPS_FLAG_ENABLED	1
#define	IMPS_FLAG_BOOT		2

#define LAPIC_ADDR_DEFAULT		0xFEE00000uL

enum ctEntryType{ etCPU, etBUS, etIOAPIC, etIO_INT, etLOCAL_INT };
enum ctInterruptType{ itINT, itNMI, itSMI, itEXTINT };

struct FPS{
	uint32_t	Signature;			//This 4 uint8_t signature is the ASCII string "_MP_" which the OS should use to find this structure.
	uint32_t	pConfigTable;		//This is a 4 uint8_t pointer to the MP configuration structure which contains information about the multiprocessor configuration.
	uint8_t		Length;				//This is a 1 uint8_t value specifying the length of this structure in 16 uint8_t paragraphs. This should be 1.
	uint8_t		Version;			//This is a 1 uint8_t value specifying the version of the multiprocessing specification. Either 1 denoting version 1.1, or 4 denoting version 1.4.
	uint8_t		Checksum;			//The sum of all bytes in this floating pointer structure including this checksum uint8_t should be zero.
	uint8_t		Features[5];		//This contains feature flags.
} PACKED;

struct ConfigTable{
	uint32_t	Signature;			//This 4 uint8_t signature is the ASCII string "PCMP" which confirms that this table is present.
	uint16_t	BaseTableLength;	//This 2 uint8_t value represents the length of the base table in bytes, including the header, starting from offset 0.
	uint8_t		SpecRev;			//This 1 uint8_t value represents the revision of the specification which the system complies to. A value of 1 indicates version 1.1, a value of 4 indicates version 1.4.
	uint8_t		Checksum;			//The sum of all bytes in the base table including this checksum and reserved bytes must add to zero.
	uint8_t		OemID[8];			//An ASCII string that identifies the manufacturer of the system. This string is not null terminated.
	uint8_t		ProductID[12];		//An ASCII string that identifies the product family of the system. This string is not null terminated.
	uint32_t	pOEM_Table;			//An optional pointer to an OEM-defined configuration table. If no OEM table is present, this field is zero.
	uint16_t	OEM_TableSize;		//The size (if it exists) of the OEM table. If the OEM table does not exist, this field is zero.
	uint16_t	EntryQty;			//The number of entries following this base header table in Memory:: This allows software to find the end of the table when parsing the entries.
	uint32_t	LAPIC_Address;		//The physical address where each processor's local APIC is mapped. Each processor memory maps its own local APIC into this address range.
	uint16_t	ExtTableLength;		//The total size of the extended table (entries) in bytes. If there are no extended entries, this field is zero.
	uint8_t		ExtTableChecksum;	//A checksum of all the bytes in the extended table. All off the bytes in the extended table must sum to this value. If there are no extended entries, this field is zero.
	uint8_t		Reserved;
} PACKED;

struct cteCPU{					//An entry describing a processor in the system. One entry per processor.
	uint8_t		EntryType;			// must be 0
	uint8_t		LAPIC_id;			//This is the unique APIC ID number for the processor.
	uint8_t		LAPIC_Ver;			//This is bits 0-7 of the Local APIC version number register.
	uint8_t		Flags;
	uint32_t	Signature;			//This is the etCPU signature as would be returned by the CPUID instruction. If the processor does not support the CPUID instruction, the BIOS fills this value according to the values in the specification.
	uint32_t	Features;			//This is the feature flags as would be returned by the CPUID instruction. If the processor does not support the CPUID instruction, the BIOS fills this value according to values in the specification.
	uint64_t	Reserved;
} PACKED;

struct cteBUS{
	uint8_t		EntryType;			// must be 1
	uint8_t		id;
	char		Type[6];
} PACKED;

struct cteIOAPIC{				//An entry describing an IO APIC present in the system. One entry per IO APIC::
	uint8_t		EntryType;			// must be 2
	uint8_t		id;					//This is the ID of this IO APIC::
	uint8_t		Version;			//This is bits 0-7 of the IO APIC's version register.
	uint8_t		Flags;
	uint32_t	Address;			//This contains the physical base address where this IO APIC is mapped.
} PACKED;

struct cteINT{
	uint8_t		EntryType;			// must be 3 or 4
	uint8_t		int_type;
	uint16_t	flags;
	uint8_t		source_bus_id;
	uint8_t		source_bus_irq;
	uint8_t		dest_apic_id;
	uint8_t		dest_apic_intin;
} PACKED;

static
FPS*
FindFPS(size_t start, size_t end)
{
	for(FPS* p = (FPS*)start; p < (FPS*)end; p++) {
		if (p->Signature == IMPS_FPS_SIGNATURE
			&& p->Length == 1
			&& (p->Version == 1 || p->Version == 4)
			&& !memsum(p, 16))
		{
			return(p);
		}
	}
	return NULL;
}

static
bool
CheckFPS(FPS *fps)
{
	if (fps->Features[0] > IMPS_FPS_DEFAULT_MAX)
		return false;
	if (fps->pConfigTable) {
		ConfigTable *ct = (ConfigTable*)((size_t)(fps->pConfigTable));
		if (ct->Signature != IMPS_CTH_SIGNATURE || memsum(ct, ct->BaseTableLength))
			return false;
		if (ct->SpecRev != fps->Version)
			return false;
		if (ct->ExtTableLength && ((memsum((void*)(((size_t)ct) + ct->BaseTableLength), ct->ExtTableLength) + ct->ExtTableChecksum) & 0xFF))
			return false;
	} else if (!fps->Features[0])
		return false;
	return true;
}

ImpsParser::ImpsParser()
{
	_fps = FindFPS(0xF0000, 0xFFFFF);
	if (!_fps) {
		debug_print("[ImpsParser] FPS not found!\n");
		Processors::Instance()->AddCpu(0, true, true);
		return;
	}

	assert(CheckFPS(_fps));
	_configTable = (ConfigTable*)_fps->pConfigTable;
	assert(_configTable);

	//__debug_print("IMPS v1 %d BSP APIC config: \" %s mode\", address: 0x%x \n", mFps->Version, (mFps->Features[1] & IMPS_FPS_IMCRP_BIT)?"IMCR and PIC":"Virtual Wire", ct ? ct->LAPIC_Address : LAPIC_ADDR_DEFAULT);

	_lapicAddr = _configTable ? _configTable->LAPIC_Address : LAPIC_ADDR_DEFAULT;

	char* p = (char*)_configTable + sizeof(ConfigTable);
	int count = _configTable->EntryQty;

	while (count-- > 0) {
		switch(*p) {
			case etCPU:
				AddCPU((cteCPU*)p);
				p+=sizeof(cteCPU);
				break;
			case etBUS:
				//AddBUS((cteBUS*)p);
				p+=sizeof(cteBUS);
				break;
			case etIOAPIC:
				//AddIOAPIC((cteIOAPIC*)p);
				p+=sizeof(cteIOAPIC);
				break;
			case etIO_INT:
				//AddINT((cteINT*)p);
				p+=sizeof(cteINT);
				break;
			case etLOCAL_INT:
				//AddLINT((cteINT*)p);
				p+=sizeof(cteINT);
				break;
			default:
				assert(false);
		}
	}
}

void
ImpsParser::AddCPU(cteCPU *cpu)
{
	Processors::Instance()->AddCpu(cpu->LAPIC_id, cpu->Flags & IMPS_FLAG_ENABLED, cpu->Flags & IMPS_FLAG_BOOT);
}
/*
void
ImpsParser::AddBUS(cteBUS *bus)
{
// 	char str[7];
// 	memcpy(str, bus->Type, 6);
//  	str[6] = 0;
//  	__debug_print("%s bus %d\n", str, bus->id);
}

void
ImpsParser::AddIOAPIC(cteIOAPIC *ioapic)
{
	//__debug_print("I/O APIC %d ver %d, address: 0x%x  %s\n", ioapic->id, ioapic->Version, ioapic->Address, (ioapic->Flags & IMPS_FLAG_ENABLED)?"":"DISABLED");
}

void
ImpsParser::AddINT(cteINT *interr)
{
	//__debug_print("int%d: %d[%d]-%d[%d]>%d[%d]\n",interr->source_bus_id,interr->source_bus_id,interr->int_type,interr->flags,interr->dest_apic_id,interr->dest_apic_intin);
}

void
ImpsParser::AddLINT(cteINT *interr)
{
	//__debug_print("Local int%d: %d[%d]-%d[%d]>%d[%d]\n",interr->source_bus_id,interr->source_bus_id,interr->int_type,interr->flags,interr->dest_apic_id,interr->dest_apic_intin);
}
*/
ImpsParser*
ImpsParser::Instance()
{
	static ImpsParser instance;
	return &instance;
}

ImpsParser::~ImpsParser()
{
}

size_t
ImpsParser::GetLapicAddr()
{
	return _lapicAddr;
}
