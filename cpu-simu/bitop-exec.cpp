#include <stdio.h>
#include "cpu.h"
#include "alucore.h"
#include "log.h"
//extern struct reg_update_rec reg_update_table[10];
//extern int reg_update_rec_num;
//
//
// bit base  reg: bitoffset %= operand size
//           mem:bitoffset %=operand size, byte offset=bitoffset /operand size
//
void CCPU:: bitop_insn_exec(PINSN_BASIC_INFO p_insn,PINSN_EXEC_INFO p_insn_exec_info)
{
	unsigned long bit_base,bit_offset,addr_hi,addr_lo,result=0;
	unsigned short cflag=__CFLAGS__;
	REG_CODE dst_reg;

	//
	// bit offset :imm/reg
	//

	if((p_insn_exec_info->op_presence&(OP_PRESENCE_IMM|OP_PRESENCE_REG))==OP_PRESENCE_IMM)
	 	bit_offset=p_insn_exec_info->imm;
	else
		bit_offset=reg_file.read_reg(p_insn_exec_info->reg);
	//
	// bit base: r/m
	//

	if((p_insn_exec_info->op_presence&OP_PRESENCE_RM_R)==OP_PRESENCE_RM_R)
	{
		dst_reg=p_insn_exec_info->r_m.operand_desc.reg;
		bit_base=reg_file.read_reg(p_insn_exec_info->r_m.operand_desc.reg);

	}
	else
	{	
		dst_reg.reg_cat=RC_NOTHING;

		gen_mem_addr(p_insn_exec_info->r_m.operand_desc.mem,p_insn->addr_size,addr_hi,addr_lo);

		gen_mem_addr_raw(addr_lo,(bit_offset/p_insn->data_size)*(p_insn->data_size>>3),
			p_insn->addr_size,addr_hi,addr_lo);
		if( !read_mem_raw(p_insn_exec_info->r_m.operand_desc.mem.seg,
			addr_hi,addr_lo,bit_base,p_insn->data_size,p_insn->addr_size)) return;
	}

	bit_offset%=(p_insn->data_size);

	switch(p_insn->p_instr_desc->index)
	{

	case btr:
		result=alu_btr(cflag,bit_base,bit_offset,p_insn->data_size);
		break;
	case btc:
		result=alu_btc(cflag,bit_base,bit_offset,p_insn->data_size);
		break;
	case bts:
		result=alu_bts(cflag,bit_base,bit_offset,p_insn->data_size);
		//log0("bts [%x],%x(%x)=%x(%x) %x\n",bit_base,bit_offset,__CFLAGS__&1,result,cflag&1,p_insn->data_size);
		break;
	case bt:
		alu_bt(cflag,bit_base,bit_offset,p_insn->data_size);
		goto ret;// no result to write back
		break;
	default:
		bug();
	}


	if(dst_reg.reg_cat==RC_NOTHING)
	{
		if( !write_mem_raw(p_insn_exec_info->r_m.operand_desc.mem.seg,
			addr_hi,addr_lo,result,p_insn->data_size,p_insn->addr_size)) return;
	}
	else
	{
		reg_update_table[reg_update_rec_num].data=result;
		reg_update_table[reg_update_rec_num].reg=dst_reg;
		reg_update_rec_num++;
	}
ret:
	__CFLAGS__=cflag;
}
