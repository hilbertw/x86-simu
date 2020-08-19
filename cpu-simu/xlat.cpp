#include "cpu.h"

void CCPU::xlat_ (char seg,int  AddressSize)
{
	unsigned long bx,al;

	REG_CODE reg;
	reg.reg_cat=RC_REGG8;
	reg.reg_no=0;
	al=reg_file.read_reg(reg);

	if(seg==DEF_SEG) seg=DS;

	if (AddressSize  ==  16)
	{
		REG_CODE reg_bx;
		reg_bx.reg_cat=RC_REGG16;
		reg_bx.reg_no=BX&7;

		unsigned long addr_hi,addr_lo;


		bx=reg_file.read_reg(reg_bx);
		gen_mem_addr_raw(bx,al,AddressSize,addr_hi,addr_lo);

		al=0;
		if(!read_mem_raw(seg,addr_hi,addr_lo,al,8,AddressSize)) return;

		//__AL__ = DS:[__BX__ + ZeroExtend(__AL__)];
	} else { /* AddressSize  ==  32 */
		//__AL__ = DS:[EBX + ZeroExtend(__AL__)];

		REG_CODE reg_bx;
		reg_bx.reg_cat=RC_REGG32;
		reg_bx.reg_no=BX&7;

		unsigned long addr_hi,addr_lo;


		bx=reg_file.read_reg(reg_bx);
		gen_mem_addr_raw(bx,al,AddressSize,addr_hi,addr_lo);

		al=0;
		if(!read_mem_raw(seg,addr_hi,addr_lo,al,8,AddressSize)) return;
	}
	update_reg(reg,al);
}