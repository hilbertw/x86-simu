#include "log.h"
#include "apic.h"
#include "cpu.h"
#include "config.h"
extern CCPU cpu[];
extern CIoAPIC io_apic;

void dump_apic()
{
	for(int i=0;i<CFG_N_CPUS;i++)
		cpu[i].lapic.dump();

	io_apic.dump();
}