#include "memory-if.h"

//	All registers must be accessed using 32-bit loads and stores. 
//	Wider registers (64-bit or 256-bit) are defined and accessed as independent	multiple 32-bit registers. 
//	If a LOCK prefix is used with a MOV instruction that accesses theAPIC address space, 
//	  the prefix is ignored; that is, a locking operation does not take place.


void CMemoryInterface::mem_read_string(DWORD address_lo,DWORD address_hi,char *buff,int count)
{
	//
	//for efficiency, just think about two cases, apic register aor not
	//
	if(apic_reg(address_lo))
	{
		while( count)
		{
			DWORD data=p_lapic->read_register(address_lo-apic_base);

			if(count>=4)
			{
				*(DWORD*)buff=data;
				 buff+=4;
				 count-=4;
			}
			else if(count==2)
			{
			*(WORD*)buff=(WORD)data;
			break;
			}
			else
			{
			*buff=(char)data;
			break;
			}
		}
	}
	else
		p_biu->mem_read_string(address_lo,buff,count);
}

void CMemoryInterface::mem_write_string(DWORD address_lo,DWORD address_hi,char *buff,int count)
{
	//
	//for efficiency, just think about two cases, apic register aor not
	//
	if(apic_reg(address_lo))
	{
		while( count>= 4)
		{
			p_lapic->write_register(address_lo-apic_base,*(DWORD*)buff);
			buff+=4;
			count-=4;
		}
	}
	else
		p_biu->mem_write_string(address_lo,buff,count);
}
