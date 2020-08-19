#include <stdio.h>
#include "cpu.h"
#include "alucore.h"
#include "condition.h"
#include "log.h"
void CCPU:: alu_insn_exec(PINSN_BASIC_INFO p_insn,PINSN_EXEC_INFO p_insn_exec_info)
{
	unsigned long op1=0,op2=0,result=0;
	unsigned short cflag=__CFLAGS__;
	REG_CODE dst_reg;
	int data_size=(p_insn_exec_info->flag&EXEC_F_WORD)?p_insn->data_size:8;
	//
	//  r/m,imm
	//
	dst_reg.reg_cat=RC_NOTHING;
	switch(p_insn_exec_info->op_presence&(OP_PRESENCE_RM_R|OP_PRESENCE_IMM|OP_PRESENCE_REG|OP_PRESENCE_REG_SEC))
	{
	case OP_PRESENCE_RM_R|OP_PRESENCE_IMM:
		dst_reg=p_insn_exec_info->r_m.operand_desc.reg;
		op1=reg_file.read_reg(dst_reg);
	 	op2=p_insn_exec_info->imm;
		break;
	case OP_PRESENCE_REG|OP_PRESENCE_IMM:
		dst_reg=p_insn_exec_info->reg;
		op1=reg_file.read_reg(dst_reg);
	 	op2=p_insn_exec_info->imm;
		break;

	case OP_PRESENCE_RM |OP_PRESENCE_IMM:
		if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,op1,data_size,p_insn->addr_size)) 
			return;

		op2=p_insn_exec_info->imm;

	    break;
	case OP_PRESENCE_RM_R|OP_PRESENCE_REG:

		if(p_insn_exec_info->flag&EXEC_F_DIR)
		{
			dst_reg=p_insn_exec_info->reg;
			op2=reg_file.read_reg(p_insn_exec_info->r_m.operand_desc.reg);
		}
		else
		{
			dst_reg=p_insn_exec_info->r_m.operand_desc.reg;
			op2=reg_file.read_reg(p_insn_exec_info->reg);
		}
		op1=reg_file.read_reg(dst_reg);
		break;
	case OP_PRESENCE_RM|OP_PRESENCE_REG:
		if(p_insn_exec_info->flag&EXEC_F_DIR)
		{
			dst_reg=p_insn_exec_info->reg;
			op1=reg_file.read_reg(dst_reg);
			if(! read_mem(p_insn_exec_info->r_m.operand_desc.mem,op2,data_size,p_insn->addr_size)) 
				return;

		}
		else
		{
			if(! read_mem(p_insn_exec_info->r_m.operand_desc.mem,op1,data_size,p_insn->addr_size)) 
				return;
			op2=reg_file.read_reg(p_insn_exec_info->reg);	
		}
		break;
	case OP_PRESENCE_RM_R|OP_PRESENCE_REG_SEC:
		dst_reg=p_insn_exec_info->r_m.operand_desc.reg;
		op1=reg_file.read_reg(dst_reg);
		op2=reg_file.read_reg(p_insn_exec_info->secondary_reg.reg);
		break;
	case OP_PRESENCE_RM|OP_PRESENCE_REG_SEC:
		if(! read_mem(p_insn_exec_info->r_m.operand_desc.mem,op1,data_size,p_insn->addr_size)) 
			return;
		op2=reg_file.read_reg(p_insn_exec_info->secondary_reg.reg);
		break;
	case OP_PRESENCE_REG|OP_PRESENCE_REG_SEC:
		dst_reg=p_insn_exec_info->reg;
		op1=reg_file.read_reg(dst_reg);
		op2=reg_file.read_reg(p_insn_exec_info->secondary_reg.reg);
		break;
	case OP_PRESENCE_RM_R: // SETcc r/m
		dst_reg=p_insn_exec_info->r_m.operand_desc.reg;
		op1=reg_file.read_reg(dst_reg);
		break;
	case OP_PRESENCE_RM: // SETcc r/m
		if(! read_mem(p_insn_exec_info->r_m.operand_desc.mem,op1,data_size,p_insn->addr_size)) 
			return;
		break;
	case OP_PRESENCE_REG:
		dst_reg=p_insn_exec_info->reg;
		op1=reg_file.read_reg(dst_reg);
		break;
	}


	switch(p_insn->p_instr_desc->index)
	{
	case cmc:
		__CF__=__CF__==1?0:1;
		return;
	case cli:

		__IF__=0;
		return;
	case sti:

		__IF__=1;
		return;
	case cld:

		__DF__=0;
		return;
	case std_:

		__DF__=1;
		return;
	case clc:
		__CF__=0;
		return;
	case stc:
		__CF__=1;
		return;
	case test:

		alu_and(cflag,op1,op2,data_size);

		__CFLAGS__=cflag;
		return;
	case cmp:

		alu_sub(cflag,op1,op2,data_size);

		__CFLAGS__=cflag;
		return;
	case and_:
		result=alu_and(cflag,op1,op2,data_size);
		break;
	case rol:
		result=alu_rol(cflag,op1,op2,data_size);
		break;
	case ror:
		result=alu_ror(cflag,op1,op2,data_size);
		break;
	case rcl:
		result=alu_rcl(cflag,op1,op2,data_size);
		break;
	case rcr:
		result=alu_rcr(cflag,op1,op2,data_size);
		break;
	case xor_:
		result=alu_xor(cflag,op1,op2,data_size);
		break;
	case or_:
		result=alu_or(cflag,op1,op2,data_size);
		break;
	case sal:
		result=alu_sal(cflag,op1,op2,data_size);
		break;
	case shr:
		result=alu_shr(cflag,op1,op2,data_size);
		break;
	case sar:
		result=alu_sar(cflag,op1,op2,data_size);
		break;
	case add:
		result=alu_add(cflag,op1,op2,data_size);
		break;
	case inc:
		result=alu_inc(cflag,op1,data_size);
		break;
	case dec:
		result=alu_dec(cflag,op1,data_size);
		break;
	case not_:
		result=alu_not(cflag,op1,data_size);
		break;
	case neg:
		result=alu_neg(cflag,op1,data_size);
		break;

	case bsr:
		result=alu_bsr(cflag,op2,data_size);
		break;
	case bsf:
		result=alu_bsf(cflag,op2,data_size);
		break;

	case subb:
		result=alu_subb(cflag,op1,op2,data_size);

		//log0("subb %x,%x(%x)=%x(%x) %x\n",op1,op2,__CFLAGS__&1,result,cflag&1,data_size);
		break;
	case sub:
		result=alu_sub(cflag,op1,op2,data_size);
		break;
	case adc:
		result=alu_adc(cflag,op1,op2,data_size);
		break;
	case movzx:
		result=alu_movzx(cflag,op2,data_size);
		data_size=p_insn->data_size;
		break;
	case movsx:
		result=alu_movsx(cflag,op2,data_size);
		data_size=p_insn->data_size;
		break;
	default:
		if(p_insn->p_instr_desc->index>=SETO
			&&p_insn->p_instr_desc->index<=SETG)
			result=(eval_cond((COND)(p_insn->p_instr_desc->index-SETO),cflag))?1:0;
		else
		 bug();
	}

	if(dst_reg.reg_cat==RC_NOTHING)
	{
		if(!write_mem(p_insn_exec_info->r_m.operand_desc.mem,result,data_size,p_insn->addr_size))
			return;
	}
	else
	{
		reg_update_table[reg_update_rec_num].data=result;
		reg_update_table[reg_update_rec_num].reg=dst_reg;
		reg_update_rec_num++;
	}
	__CFLAGS__=cflag;
}
