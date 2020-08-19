#include "mch.h"
#include "log.h"
extern CMCH mch;

char *conf_entry_type[]=
{
"Processor",                  // 0 20 One entry per processor.
"Bus",                       // 1 8 One entry per bus.
"I/O APIC",                  // 2 8 One entry per I/O APIC.
"I/O Interrupt Assignment",  // 3 8 One entry per bus interrupt source.
"Local Interrupt Assignment",// 4 8 One entry per
};

char *int_type[]=
{
"INT",// Signal is a vectored interrupt; vector is supplied by APIC redirection table.
"NMI",// Signal is a nonmaskable interrupt.
"SMI",// Signal is a system management interrupt.
"ExtINT",// Signal is a vectored interrupt; vector is supplied by external PIC.
         //For example, if an 8259 is used as the external PIC, the source is
         //the 8259 INTR output line, and the vector is supplied by the 8259.
};

void dump_mp_base_tbl(unsigned long addr)
{
	unsigned int i,j,k;
	unsigned int size=mch.word(addr+4);

	for(i=44;i<size;i+=k)
	{
		j=mch.byte(addr+i);
		k=8;// entry length 
			
		if(j<5)
		{
			log0("%s\n",conf_entry_type[j]);
			switch(j)
			{
			case 0:
				log0("apic id:%x,model:%x,  %s,%s,\n",mch.byte(addr+i+1),mch.dword(addr+i+4)&0xfff,(mch.byte(addr+i+3)&2)?"BSP":"AP",(mch.byte(addr+i+3)&1)?"Enabled":"Disabled");
				k=20;
				break;
			case 1:
				log0("id:%x %c%c%c%c%c%c\n",mch.byte(addr+i+1),
					mch.byte(addr+i+2),
					mch.byte(addr+i+3),
					mch.byte(addr+i+4),
					mch.byte(addr+i+5),
					mch.byte(addr+i+6),
					mch.byte(addr+i+7));
				break;
			case 2:
				log0("id:%x,addr:%x, %s\n",mch.byte(addr+i+1),mch.dword(addr+i+4),(mch.byte(addr+i+3)&1)?"Enabled":"Disabled");
				break;
			case 3:
				log0("%s bus:%x, irq:%x,ioapic:%x intin#%d\n",int_type[mch.byte(addr+i+1)],mch.byte(addr+i+4),mch.byte(addr+i+5),mch.byte(addr+i+6),mch.byte(addr+i+7));
				break;
			case 4:
				log0("%s bus:%x, irq:%x local apic :%x, lint#%d\n",int_type[mch.byte(addr+i+1)],mch.byte(addr+i+4),mch.byte(addr+i+5),mch.byte(addr+i+6),mch.byte(addr+i+7));
				break;
			}
		}
	}
}

unsigned long search_mp_conf_tbl(unsigned long addr, unsigned int size)
{
	unsigned int i;
	// 16 byte boundar
	for(i=0;i<size;i+=0x10)
	{
		//¡°_MP_"
		if(mch.dword(addr+i)==0x5f504d5f)
		{
			char rev=mch.byte(addr+i+9);
			log0("MP v %s found\n", rev==1?"1.1":"1.4");
			return mch.dword(addr+i+4);
		}
	}

	return 0;

}
void dump_mp()
{
	unsigned long apic_conf_tbl,oem_tbl_ptr;
	//
	// ebda
	//
	unsigned long ebda=mch.dword(0x40e);// 40:0e

    apic_conf_tbl=search_mp_conf_tbl(((ebda>>16)<<4)+(unsigned short )ebda,0x400);
	if(apic_conf_tbl!=0) goto dump_mp_conf_tbl;

	//
	// e000:0~f000:ffff
	//
    apic_conf_tbl=search_mp_conf_tbl(0xe0000,0x20000);
	if(apic_conf_tbl!=0)goto dump_mp_conf_tbl;
	
	log0(" MP  not found.\n");
	return ;
dump_mp_conf_tbl:

	dump_mp_base_tbl(apic_conf_tbl );

	//oem_tbl_ptr=mch.dword(apic_conf_tbl+28);
	//if(oem_tbl_ptr!=0)

}