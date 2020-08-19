#include "mch.h"
#include "log.h"
extern CMCH mch;

char *madt_struct_type[]=
{
 "Processor Local APIC",
 "I/O APIC",
 "Interrupt Source Override",
 "Non-maskable Interrupt Source (NMI)",
 "Local APIC NMI Structure",
 "Local APIC Address Override Structure",
 "I/O SAPIC",
 "Local SAPIC",
 "Platform Interrupt Sources"
};

void dump_madt(unsigned long addr)
{
	unsigned int i,j,k;
	unsigned int size;

	size=mch.dword(addr+4);

	for(i=44;i<size;i)
	{
		j=mch.byte(addr+i);
		k=mch.byte(addr+i+1);
		
		if(j<9)
		{
			log0("%s\n",madt_struct_type[j]);
			switch(j)
			{
			case 0:
				log0("processor id:%x,apic id:%x %s\n",mch.byte(addr+i+2),mch.byte(addr+i+3),mch.byte(addr+i+4)?"Enabled":"Disabled");
				break;
			case 1:
				log0("apic id:%x address:%x base:%x\n",mch.word(addr+i+2),mch.dword(addr+i+4),mch.dword(addr+i+8));
				
				break;
			case 2:
				log0("source:irq#%x->irq#%x\n",mch.byte(addr+i+3),mch.byte(addr+i+4));
				break;
			case 5:
				log0("base:%x\n",mch.dword(addr+i+4));
				
				break;
			}
		}
		i+=k;

	}
}

void dump_fadt(unsigned long addr)
{
	unsigned int i,j,k;

	log0("facs:%08x\n",mch.dword(addr+36));
	log0("dsdt:%08x\n",mch.dword(addr+40));
	log0("Preferred_PM_Profile:%x\n",mch.byte(addr+45));
	log0("SCI_INT:%04x\n",mch.word(addr+46));
	log0("SMI_CMD:%08x\n",mch.dword(addr+48));
	log0("ACPI_ENABLE:%x\n",mch.byte(addr+52));
	log0("ACPI_ENABLE:%x\n",mch.byte(addr+53));
	log0("S4BIOS_REQ:%x\n",mch.byte(addr+54));
	log0("PSTATE_CNT:%x\n",mch.byte(addr+55));
	log0("PM1a_EVT_BLK:%08x\n",mch.dword(addr+56));
	log0("PM1b_EVT_BLK:%08x\n",mch.dword(addr+60));
	log0("PM1a_CNT_BLK:%08x\n",mch.dword(addr+64));
	log0("PM1b_CNT_BLK:%08x\n",mch.dword(addr+68));
	log0("PM2_CNT_BLK:%08x\n",mch.dword(addr+72));
	log0("PM_TMR_BLK:%08x\n",mch.dword(addr+76));
	log0("GPE0_BLK:%08x\n",mch.dword(addr+80));
	log0("GPE1_BLK:%08x\n",mch.dword(addr+84));
}

void dump_rsdt(unsigned long addr)
{
	unsigned int i,j,k;
	unsigned int size;

	size=mch.dword(addr+4);

	for(i=36;i<size;i+=4)
	{
		j=mch.dword(addr+i);
		
		log0("%08x ",j);
		if(j<CFG_RAM_SIZE)
		{
			k=mch.dword(j);
			log0("%c%c%c%c\n",(char)k,(char)(k>>8),(char)(k>>16),(char)(k>>24));
			if(k==0x43495041) dump_madt(j);
			if(k==0x50434146) dump_fadt(j);
		}
	}
}
unsigned long search_rdp(unsigned long addr, unsigned int size)
{
	unsigned int i;
	// 16 byte boundar
	for(i=0;i<size;i+=0x10)
	{
		//¡°RSD PTR "
		if(mch.dword(addr+i)==0x20445352
			&&mch.dword(addr+i+4)==0x20525450)
		{
			// checksum. first 20 bytes must sum to 0

			// extended checksum.
			return addr+i;
		}
	}

	return 0;

}
void dump_acpi()
{
	unsigned long rdp;
	//
	// ebda
	//
	unsigned long ebda=mch.dword(0x40e);// 40:0e

    rdp=search_rdp(((ebda>>16)<<4)+(unsigned short )ebda,0x400);
	if(rdp!=0) goto dump_rdp;

	//
	// e000:0~f000:ffff
	//
    rdp=search_rdp(0xe0000,0x20000);
	if(rdp==0) return ;
dump_rdp:
	// 
	log0("rsdt:%08x, size:%x\n",mch.dword(rdp+16),mch.dword(rdp+20));

	dump_rsdt(mch.dword(rdp+16));

	if(mch.byte(rdp+15)==2)
	log0("xdst:%08x%08x \n",mch.dword(rdp+28),mch.dword(rdp+24));
}