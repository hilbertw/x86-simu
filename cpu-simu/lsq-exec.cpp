#include <stdio.h>
#include "cpu.h"
#include "condition.h"
extern bool step;
extern struct reg_update_rec reg_update_table[10];
extern int reg_update_rec_num;
void CCPU:: lsq_insn_exec(PINSN_BASIC_INFO p_insn,PINSN_EXEC_INFO p_insn_exec_info)
{
	unsigned long src,dest ;
	REG_CODE dst_reg;
	int data_size=(p_insn_exec_info->flag&EXEC_F_WORD)?p_insn->data_size:8;
	unsigned short cflags=__CFLAGS__;

	switch(p_insn->p_instr_desc->index)
	{
	case CMOVO  : 
	case CMOVNO	:
	case CMOVB	:
	case CMOVNB	:
	case CMOVE	:
	case CMOVNE	:
	case CMOVNA	:
	case CMOVA  :
	case CMOVS	:
	case CMOVNS	:
	case CMOVPE	:
	case CMOVPO	:
	case CMOVL	:
	case CMOVNL	:
	case CMOVNG : 
	case CMOVG	:
		if(!(eval_cond((COND)(p_insn->p_instr_desc->index-CMOVO),cflags))) break;
		
		
	case mov:

		dst_reg.reg_cat=RC_NOTHING;
		switch(p_insn_exec_info->op_presence&(OP_PRESENCE_RM_R|OP_PRESENCE_IMM|OP_PRESENCE_REG))
		{
		case OP_PRESENCE_RM_R|OP_PRESENCE_IMM:
			dst_reg=p_insn_exec_info->r_m.operand_desc.reg;
			src=p_insn_exec_info->imm;
			break;
		case OP_PRESENCE_REG|OP_PRESENCE_IMM:
			dst_reg=p_insn_exec_info->reg;

		case OP_PRESENCE_RM |OP_PRESENCE_IMM:
			src=p_insn_exec_info->imm;
			break;
		case OP_PRESENCE_RM_R|OP_PRESENCE_REG:
			if(p_insn_exec_info->flag&EXEC_F_DIR)
			{			
				dst_reg=p_insn_exec_info->reg;
				src=reg_file.read_reg(p_insn_exec_info->r_m.operand_desc.reg);

			}
			else
			{
				dst_reg=p_insn_exec_info->r_m.operand_desc.reg;
				src=reg_file.read_reg(p_insn_exec_info->reg);
			}
			break;
		case OP_PRESENCE_RM|OP_PRESENCE_REG:
			if(p_insn_exec_info->flag&EXEC_F_DIR)
			{
				dst_reg=p_insn_exec_info->reg;
				if(! read_mem(p_insn_exec_info->r_m.operand_desc.mem,src,data_size,p_insn->addr_size)) 
					return;
			}
			else
			{
				src=reg_file.read_reg(p_insn_exec_info->reg);
			}
			break;
		default:
			bug() ;
		}
		if(dst_reg.reg_cat==RC_NOTHING)
		{
			if(!write_mem(p_insn_exec_info->r_m.operand_desc.mem,src,data_size,p_insn->addr_size))
				return;
		}
		else
		{
			reg_update_table[reg_update_rec_num].data=src;
			reg_update_table[reg_update_rec_num].reg=dst_reg;
			reg_update_rec_num++;


			if(dst_reg.reg_cat==RC_REGSEG)
				set_sreg(dst_reg.reg_no,(unsigned short)src);
		}
		break;
	case in:
		assert(p_insn_exec_info->type.reg_present);

		reg_update_table[reg_update_rec_num].reg=p_insn_exec_info->reg;
		data_size =(p_insn_exec_info->flag&EXEC_F_WORD)?p_insn->data_size:8;
		if(p_insn_exec_info->type.imm_present)
		{

			reg_update_table[reg_update_rec_num].data=inport((unsigned short)p_insn_exec_info->imm,data_size);

		}
		else if(p_insn_exec_info->type.s_reg_present)
		{
			reg_update_table[reg_update_rec_num].data=inport(reg_file.read_reg(p_insn_exec_info->secondary_reg.reg),data_size);
		}
		else
			bug() ;
		reg_update_rec_num++;
		break;

	case pusha:
		Pusha(p_insn->data_size);
		break;
	case popa:
		Popa(p_insn->data_size);
		break;
	case push:
		switch (p_insn_exec_info->op_presence&(OP_PRESENCE_RM_R|OP_PRESENCE_IMM|OP_PRESENCE_REG))
		{
		case OP_PRESENCE_RM_R:
			src=reg_file.read_reg(p_insn_exec_info->r_m.operand_desc.reg);
			break;
		case OP_PRESENCE_RM:
			if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,src,p_insn->data_size,p_insn->addr_size)) 
			return;
			break;
		case OP_PRESENCE_REG:
			src=reg_file.read_reg(p_insn_exec_info->reg);
			break;
		case OP_PRESENCE_IMM:
			src =/*(p_insn->data_size==8)?(unsigned char)p_insn_exec_info->imm:*/p_insn_exec_info->imm;
			break;
		default:
			bug() ;
		}
	
		Push(src,p_insn->data_size);
		break;
	case pop:
		if(!Pop(dest,p_insn->data_size)) break;

		switch (p_insn_exec_info->op_presence)
		{
		case OP_PRESENCE_RM_R:
			assert (p_insn_exec_info->reg.reg_cat!=RC_REGSEG);
			update_reg(p_insn_exec_info->r_m.operand_desc.reg,dest);
			break;
		case OP_PRESENCE_RM:
			write_mem(p_insn_exec_info->r_m.operand_desc.mem,dest,p_insn->data_size,p_insn->addr_size);
			break;
		case OP_PRESENCE_REG:
			if(p_insn_exec_info->reg.reg_cat==RC_REGSEG)
				set_sreg(p_insn_exec_info->reg.reg_no,(unsigned short)dest);
			else
				update_reg(p_insn_exec_info->reg,dest);
			break;
		default:
			bug() ;
		}

		break;
	case pushf:
		if(p_insn->data_size==32) Push(__EFLAGS__,p_insn->data_size);
		else if(p_insn->data_size==16)	Push(__CFLAGS__,p_insn->data_size);
		else bug() ;
		break;
	case popf:
 
		if(p_insn->data_size==32) Popfd();
		else if(p_insn->data_size==16)	
		{
			Pop(dest,p_insn->data_size);
			__CFLAGS__=(unsigned short)dest;
		}
		else bug() ;
		break;
	case xlat:
		xlat_(PREFIX_SEG(p_insn->prefix),p_insn->addr_size);
		break;

	case lea:
		assert ((p_insn_exec_info->op_presence&OP_PRESENCE_RM_R)==OP_PRESENCE_RM);
		gen_mem_addr(p_insn_exec_info->r_m.operand_desc.mem,p_insn->addr_size,dest,src);
		assert (p_insn_exec_info->type.reg_present);
		update_reg(p_insn_exec_info->reg,src);
		break;

	case lahf:
		lahf_();
		break;
	case sahf:
		sahf_();
		break;
	case prefetch:	
		break;

	default:
		::step=true;
	}
}