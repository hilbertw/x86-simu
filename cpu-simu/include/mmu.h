#ifndef __MMU_H__
#define __MMU_H__

#include "tlb.h"
#include "memory-if.h"
#include "sysreg.h"


typedef unsigned int RW_STATUS;

class CMMU
{
public:
	const static unsigned int PAGE_FAULT_NONE=0; // successful
	const static unsigned int PAGE_FAULT_USER=4; // super mode 0
	const static unsigned int PAGE_FAULT_WRITE=2;// read 0
	const static unsigned int PAGE_FAULT_PROT=1; // protection
	const static unsigned int PAGE_FAULT_RSVD=8; // reserved bits in pxe
	const static unsigned int PAGE_FAULT_ID=0x10; // insn fetch
	const static unsigned int PAGE_FAULT_NP=0x20; //not present

	CMemoryInterface *p_memory;
	//CCacheInterface *p_cache;
	unsigned long *cr;
private:
	CTLB tlb[2];      // itlb, dtlb
	unsigned int a20m;//a20m# input signal
public:
	CMMU():a20m(-1)	{}

	void set_a20_mask(unsigned long mask){a20m=mask;  }
	//
	// load code line
	//
	RW_STATUS fetch(unsigned int linear_address, bool user, char *buffer, int count);

	//
	// ld data
	//
    RW_STATUS ld(unsigned int linear_address, bool user, char *buffer, int count);
    RW_STATUS lddw(unsigned int linear_address, bool user,unsigned  long *pdw);
    RW_STATUS ldw(unsigned int linear_address, bool user,unsigned  short *pw);
    RW_STATUS ldb(unsigned int linear_address, bool user, unsigned char *pc);
	//
	// st data
	//
    RW_STATUS st(unsigned int linear_address, bool user, char *buffer, int count);
    RW_STATUS stdw(unsigned int linear_address, bool user,unsigned  long dw);
    RW_STATUS stw(unsigned int linear_address, bool user,unsigned  short w);
    RW_STATUS stb(unsigned int linear_address, bool user, unsigned char c);

	//
	// ld data by phy address
	//
    void phy_ld(unsigned int physical_address, char *buffer, int count)
	{
	   if(count==1)phy_ldb(physical_address,(unsigned char *)buffer);
		else if(count==2)phy_ldw(physical_address,(unsigned short *)buffer);
		else if(count==4)phy_lddw(physical_address,(unsigned long *)buffer);
		else p_memory->mem_read_string(physical_address,0,buffer,count);
	}
    void phy_lddw(unsigned int physical_address,unsigned  long *pdw)
    {*pdw=p_memory->mem_read_int (physical_address);}
    void phy_ldw(unsigned int physical_address, unsigned  short *pw)
	{*pw=p_memory->mem_read_short (physical_address);}
    void phy_ldb(unsigned int physical_address, unsigned char *pc)
	{*pc=p_memory->mem_read_byte (physical_address);}
	//
	// st data by phy address
	//
    void phy_st(unsigned int physical_address, char *buffer, int count)
	{
	   if(count==1)phy_stb(physical_address,*(unsigned char *)buffer);
		else if(count==2)phy_stw(physical_address,*(unsigned short *)buffer);
		else if(count==4)phy_stdw(physical_address,*(unsigned long *)buffer);
		else p_memory->mem_write_string(physical_address,0,buffer,count);
	}
    void phy_stdw(unsigned int physical_address, unsigned  long dw)
	{p_memory->mem_write_int (physical_address,dw);}
    void phy_stw(unsigned int physical_address, unsigned  short w)
	{p_memory->mem_write_short (physical_address,w);}
    void phy_stb(unsigned int physical_address, unsigned char c)
	{p_memory->mem_write_byte (physical_address,c);}

	RW_STATUS read(unsigned int linear_address,bool code, bool user, char *buffer, int count);
	RW_STATUS  write(unsigned int linear_address, bool user, char *buffer, int count);
private:
	RW_STATUS to_physical_address(unsigned int linear_address,bool code, bool user, bool write, 
		unsigned int &physical_address,unsigned long &page_size, bool &pcd, bool &pwt);
	RW_STATUS do_address_translation(unsigned int linear_address,bool user, bool write, PTE *out_pte,TLB_ENTRY_TYPE * p_type);
	RW_STATUS do_address_translation_pae(unsigned int linear_address,bool user, bool write, PAE32_PTE4K *out_pte,TLB_ENTRY_TYPE * p_type);
	RW_STATUS do_address_translation_pae_pse(unsigned int linear_address,bool user, bool write, PTE *out_pte,TLB_ENTRY_TYPE * p_type);
	RW_STATUS do_address_translation_pse(unsigned int linear_address,bool user, bool write, PTE *out_pte,TLB_ENTRY_TYPE * p_type);

	RW_STATUS get_tlb_entry(unsigned int linear_address,bool code, bool user, bool write,PTE *out_pte);
	RW_STATUS check_page_fault(PXE pxe, bool user, bool write);
public:
	void invalid_tlb(unsigned long vaddr ) {tlb[0].invalid(vaddr);tlb[1].invalid(vaddr);}
	void invalid_all_tlb(bool global) {tlb[0].invalid_all(global);tlb[1].invalid_all(global);}
	void flush_caches() {}
};

#define _cr4_	(*(CTRL_REG4*)&cr[4])
#define _cr3_	(*(CTRL_REG3*)&cr[3])
#define _cr2_	(*(CTRL_REG2*)&cr[2])
#define _cr0_	(*(CTRL_REG0*)&cr[0])

#define _cr3_pae_	(*(PAE_CR3*)&cr[3])
#endif   /*   __MMU_H__  */