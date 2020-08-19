#include  "cpu.h"
#include  "mch.h"
#include  "log.h"

void reset_8042();
void reset_8259();
void reset_8237();
void reset_a20();

void reset_smi();

extern CMCH mch;
extern CCPU cpu[CFG_N_CPUS];
static char *sz_reset[]={"keyboard ","PCI","Port 92h","Button",""};
// reset by:
//      0  8042
//      1  cf9 
//      4   92
//
void reset(int i)
{
	if( i>=sizeof(sz_reset)/sizeof(sz_reset[0]))
		i=sizeof(sz_reset)/sizeof(sz_reset[0])-1;

	log("%s RESET...\n",sz_reset[i]);

	//
	// do cold reset in all cases now..
	//

	reset_8042();
	reset_8237();
	reset_8259();
	mch.reset();
	reset_a20();
	reset_smi();

	for(int i=0;i<CFG_N_CPUS;i++)
		cpu[i].reset();
}

