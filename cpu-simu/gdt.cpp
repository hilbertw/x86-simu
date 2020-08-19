#include <stdio.h>
#include "cpu.h"

bool CCPU::lgdt_(MEM_DESC &m_info, int OperandSize,int addr_size)
{
	assert(OperandSize==16||OperandSize==32);

	unsigned long addr_hi,addr_lo;

	assert(m_info.seg<MAX_SEG);
	gen_mem_addr(m_info,addr_size,addr_hi,addr_lo);

	unsigned long limit;
	unsigned long base;
	if (!read_mem_raw(m_info.seg,addr_hi,addr_lo,limit,16,addr_size)) return false;

   if( OperandSize == 16)
   {
		if (!read_mem_raw(m_info.seg,addr_hi,addr_lo+2,base,24,addr_size)) return false;
	    gdt_limit=(unsigned short)limit;
	    gdt_base= base & 0xffffff; /* 24 bits of base loaded */
   }
   else
   {
		if (!read_mem_raw(m_info.seg,addr_hi,addr_lo+2,base,32,addr_size)) return false;
		gdt_limit=(unsigned short)limit;
	    gdt_base= base; /* 32 bits of base loaded */
   }
   return true;
}

bool CCPU::sgdt_(MEM_DESC &m_info, int OperandSize,int addr_size)
{
	assert(OperandSize==16||OperandSize==32);

	unsigned long addr_hi,addr_lo;

	assert(m_info.seg<MAX_SEG);
	gen_mem_addr(m_info,addr_size,addr_hi,addr_lo);


	if (!write_mem_raw(m_info.seg,addr_hi,addr_lo,gdt_limit,16,addr_size)) return false;

   if( OperandSize == 16)
   {
	   //the next three bytes are assigned the BASE field of the register, and the fourth byte is written with zero. The last byte is undefined. 
	   unsigned long tmp_base=gdt_base&0xffffff;

		if (!write_mem_raw(m_info.seg,addr_hi,addr_lo+2,tmp_base,24,addr_size)) return false;
   }
   else
   {
		if (!write_mem_raw(m_info.seg,addr_hi,addr_lo+2,gdt_base,32,addr_size)) return false;
   }
   return true;
}