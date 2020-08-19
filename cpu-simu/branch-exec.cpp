#include <stdio.h>
#include "cpu.h"
#include "condition.h"
#include "log.h"
extern struct reg_update_rec reg_update_table[10];
extern int reg_update_rec_num;
void CCPU:: branch_insn_exec(PINSN_BASIC_INFO p_insn,PINSN_EXEC_INFO p_insn_exec_info)
{
	unsigned long off,seg,addr_hi,addr_lo;
	unsigned long data_size, addr_size;
	addr_size=p_insn->addr_size;
	data_size=p_insn->data_size;
	//
	// switch from/to  protection mode pending?
	//
	if( cr0_pending)
	{
		//
		// branch unit always decode length itself for relative branch
		//
#if 0

		switch(p_insn->p_instr_desc->op_fmt)
		{
		case Jv:
			data_size=addr_size=0x10;

			if(p_insn->prefix&PREFIX_ASIZE)
				addr_size =0x20;
			if(p_insn->prefix&PREFIX_DSIZE)
				data_size =0x20;
			
			__NEXT_EIP__=__EIP__+((data_size==32)?5:3);
			if(p_insn->code[0]==0x0f) __NEXT_EIP__++;
			break;
		case Ap:
			break;
		}
#endif

		switch(p_insn->p_instr_desc->op_fmt)
		{
		case Jv:
			//
			// decode addr size, data size, next eip
			//
			// to setup big real mode, rel jump is used rather than far jump
			//if(cr0_cache&1)
			//{
			//	data_size =0x20;
			//	addr_size =0x20;
			//}
			//else
			//{
			//	data_size =0x10;
			//	addr_size =0x10;
			//}
			//if(p_insn->prefix&PREFIX_ASIZE) addr_size ^=0x30; 
			//if(p_insn->prefix&PREFIX_DSIZE) data_size ^=0x30; 
			__NEXT_EIP__=__EIP__+((data_size==32)?5:3);
			if(p_insn->code[0]==0x0f) __NEXT_EIP__++;
			break;

		}

		update_cr0();
	
	}

	switch(p_insn->p_instr_desc->index)
	{
	case jmpr: 
		assert(p_insn_exec_info->type.imm_present);
		near_rel_jump(p_insn_exec_info->imm,data_size);
		break;
	case jmp: 
		if(p_insn_exec_info->type.imm_present)
		{
			off=p_insn_exec_info->imm;

		}
		else if(p_insn_exec_info->type.rm_present)
		{
			if (p_insn_exec_info->r_m.type==OT_REG)
				off=reg_file.read_reg(p_insn_exec_info->r_m.operand_desc.reg);
			else 
			{
				off=0;
				if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,off,data_size,addr_size)) 
				return;
			}
		}
		else
			bug();

		near_abs_jump(off,data_size);
		break;
	case jmpf: 
		if(p_insn_exec_info->type.imm_present)
		{
			assert(p_insn_exec_info->type.s_imm_present);
			far_jump((unsigned short)p_insn_exec_info->secondary_imm.imm,p_insn_exec_info->imm,data_size);
		}else 
		bug();

		break;
	case jmpfp: 
		assert(p_insn_exec_info->type.rm_present);
		assert(p_insn_exec_info->r_m.type!=OT_REG);

		off=0;
		seg=0;
		gen_mem_addr(p_insn_exec_info->r_m.operand_desc.mem,addr_size,addr_hi,addr_lo);

		if( !read_mem_raw(p_insn_exec_info->r_m.operand_desc.mem.seg,addr_hi,addr_lo,off,data_size,addr_size)) 
				return;

		addr_lo+=data_size>>3;// how to make addr_hi plus carry
		if( !read_mem_raw(p_insn_exec_info->r_m.operand_desc.mem.seg,addr_hi,addr_lo,seg,16,addr_size)) 
				return;

		far_jump((unsigned short)seg,off,data_size);

		break;
	case callf: 
		if(p_insn_exec_info->type.imm_present)
		{
			assert(p_insn_exec_info->type.s_imm_present);
			far_call((unsigned short)p_insn_exec_info->secondary_imm.imm,p_insn_exec_info->imm,p_insn->data_size);
		}
		else bug();
		break;
	case callfp: 
		assert(p_insn_exec_info->type.rm_present);
		assert(p_insn_exec_info->r_m.type!=OT_REG);

		off=0;
		seg=0;
		gen_mem_addr(p_insn_exec_info->r_m.operand_desc.mem,addr_size,addr_hi,addr_lo);

		if( !read_mem_raw(p_insn_exec_info->r_m.operand_desc.mem.seg,addr_hi,addr_lo,off,data_size,addr_size)) 
				return;

		addr_lo+=data_size>>3;// how to make addr_hi plus carry
		if( !read_mem_raw(p_insn_exec_info->r_m.operand_desc.mem.seg,addr_hi,addr_lo,seg,16,addr_size)) 
				return;

		far_call((unsigned short)seg,off,data_size);
		break;

	case call:
		 if(p_insn_exec_info->type.rm_present)
		{
			if (p_insn_exec_info->r_m.type==OT_REG)
				off=reg_file.read_reg(p_insn_exec_info->r_m.operand_desc.reg);
			else 
			{
				off=0;
				if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,off,data_size,addr_size)) 
				return;
			}
			near_abs_call(off,data_size);

			break;
		}

	case callr:
		assert(p_insn_exec_info->type.imm_present);
		near_rel_call(p_insn_exec_info->imm,data_size);
		break;
	case ret:
		near_ret(0,data_size);
		break;
	case retn:
		assert(p_insn_exec_info->type.imm_present);
		near_ret((short)p_insn_exec_info->imm,data_size);
		break;
	case retf:
		far_ret(0,data_size);
		break;
	case retfn:
		assert(p_insn_exec_info->type.imm_present);
		far_ret((short)p_insn_exec_info->imm,data_size);
		break;
	case loopnz:
		loop_nz ((char)p_insn_exec_info->imm,data_size,addr_size);break;
	case loopz:
		loop_z ((char)p_insn_exec_info->imm,data_size,addr_size);break;
	case loop:
		loop_ ((char)p_insn_exec_info->imm,data_size,addr_size);break;
	case jcxz:
		jcxz_ ((char)p_insn_exec_info->imm,data_size,addr_size);break;

	default: 
		if(p_insn->p_instr_desc->index>=JO
			&&p_insn->p_instr_desc->index<=JG)
		{
			unsigned short cflags=__CFLAGS__;
			assert(p_insn_exec_info->type.imm_present);

			if(eval_cond((COND)(p_insn->p_instr_desc->index-JO),cflags))
			near_rel_jump(p_insn_exec_info->imm,data_size);
		}
		else
		bug();
	}
}