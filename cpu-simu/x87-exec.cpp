#include <stdio.h>
#include "cpu.h"
extern struct reg_update_rec reg_update_table[10];
extern int reg_update_rec_num;
void CCPU:: fpu_insn_exec(PINSN_BASIC_INFO p_insn,PINSN_EXEC_INFO p_insn_exec_info)
{
	int n;
	switch(p_insn->p_instr_desc->index)
	{
	case in:
		assert(p_insn_exec_info->type.reg_present);

		reg_update_table[reg_update_rec_num].reg=p_insn_exec_info->reg;
		n=(p_insn_exec_info->flag&EXEC_F_WORD)?p_insn->data_size:8;
		if(p_insn_exec_info->type.imm_present)
		{

			reg_update_table[reg_update_rec_num].data=inport((unsigned short)p_insn_exec_info->imm,n);

		}
		else if(p_insn_exec_info->type.s_reg_present)
		{
			reg_update_table[reg_update_rec_num].data=inport(reg_file.read_reg(p_insn_exec_info->secondary_reg.reg),n);
		}
		else
			assert(0);
		reg_update_rec_num++;
		break;
	case jmpr: 
		assert(p_insn_exec_info->type.imm_present);
		near_rel_jump(p_insn_exec_info->imm,p_insn->data_size);
		break;
	case jmp: 
		if(p_insn_exec_info->type.imm_present)
		{
		}
		else if(p_insn_exec_info->type.rm_present)
		{
		}
		else if(p_insn_exec_info->type.reg_present)
		{
		}
		else
			assert(0);
		break;
	case jmpf: 
		if(p_insn_exec_info->type.imm_present)
		{
			assert(p_insn_exec_info->type.s_imm_present);
			far_jump((unsigned short)p_insn_exec_info->secondary_imm.imm,p_insn_exec_info->imm,p_insn->data_size);
		}
		else if(p_insn_exec_info->type.rm_present)
		{
		}
		else if(p_insn_exec_info->type.reg_present)
		{
		}
		else
			assert(0);
		break;
	default:
		assert(0);
	}
}