#include "cpu.h"
CCPU cpu[CFG_N_CPUS];

void processors_init()
{
	//
	// setup cpus
	//
	for(int i=0;i<CFG_N_CPUS;i++)
	{

		cpu[i].setup(i+1,i==0);
	
		//
		// apic bus
		//
		//apic_bus.register_node(&cpu[i].lapic);
	}
}

void dump_all_cpus()
{
		for(int cpu_no=0;cpu_no<CFG_N_CPUS;cpu_no++)
		{
			if(cpu[cpu_no].halt) continue;

			log0("CPU%d:\n",cpu_no);
			cpu[cpu_no].dump_state();
			display_disasm(cpu[cpu_no].reg_file.sreg[CS],
				BASE(cpu[cpu_no].descriptors[CS]),
				cpu[cpu_no].eip(),1,true);
		}
}