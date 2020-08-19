#ifndef __MemoryLayer_H__
#define __MemoryLayer_H__
#include "biu.h"
#include "apic.h"
#include "config.h"


#ifndef DWORD

typedef unsigned long DWORD;
typedef unsigned short WORD;

#endif
//
//	Within the 4-KByte APIC register area. 
//	Register offsets are aligned on 128-bit boundaries. 

class CMemoryInterface
{
public:
	CBIU *p_biu; // pgysical memory
	CLocalAPIC* p_lapic;// apic memory mapped register
	unsigned long apic_base; //at reset:  0xfee00000

public:
	CMemoryInterface():apic_base(CFG_LAPIC_BASE)  {}

	void mem_read_string(DWORD  address_lo,DWORD address_hi,char *buff,int count);
	void mem_write_string(DWORD  address_lo,DWORD address_hi,char *buff,int count);


	void set_base(DWORD address_lo){apic_base=address_lo;}
	bool apic_reg(DWORD address_lo){return (address_lo& 0xfffff000)==apic_base;}
	void mem_write_byte(DWORD address,char dat){p_biu->mem_write_byte(address,dat);}
	void mem_write_short(DWORD address,short dat){p_biu->mem_write_short(address,dat);}

	char mem_read_byte(DWORD address)
	{
		return 	(apic_reg(address))?(char)p_lapic->read_register(address-apic_base):p_biu->mem_read_byte(address);
	}
	short mem_read_short(DWORD address)
	{
		return (apic_reg(address))?(short)p_lapic->read_register(address-apic_base):p_biu->mem_read_short(address);
	}
	int mem_read_int(DWORD address_lo)
	{
	return (apic_reg(address_lo))?p_lapic->read_register(address_lo-apic_base):p_biu->mem_read_int(address_lo);
	}
	void mem_write_int(DWORD address_lo,int dat)
	{
		if(apic_reg(address_lo))
			p_lapic->write_register(address_lo-apic_base,dat);
		else
			p_biu->mem_write_int(address_lo,dat);
	}
};



#endif   /*    __MemoryLayer_H__   */
   