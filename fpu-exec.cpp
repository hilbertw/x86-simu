#include <stdio.h>
#include "condition.h"
#include "cpu.h"

void CCPU:: fpu_insn_exec(PINSN_BASIC_INFO p_insn,PINSN_EXEC_INFO p_insn_exec_info)
{
	union {unsigned long data;char buffer[512];};
	REG_CODE reg;
	unsigned short cflags;

	fpu87.save_opcode(p_insn);
	switch(p_insn->p_instr_desc->index)
	{
	case F2XM1:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case fmt_none:
		fpu87.F2XM1_();

		break;
	}
	break;
	case FABS:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case fmt_none:
		fpu87.FABS_();

		break;
	}
	break;
	case FADD:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case STi0:
		fpu87.FADD_STi0(p_insn->code[p_insn->prefix_bytes+1]&7);

		break;
	case ST0i:
		fpu87.FADD_ST0i(p_insn->code[p_insn->prefix_bytes+1]&7);

		break;
	case mem4r:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,4,p_insn->addr_size))
			return;
		fpu87.FADD_mem4r(buffer);
		break;
	case mem8r:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,8,p_insn->addr_size))
			return;
		fpu87.FADD_mem8r(buffer);
		break;
	}
	break;
	case FADDP:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case STi0:
		fpu87.FADDP_STi0(p_insn->code[p_insn->prefix_bytes+1]&7);

		break;
	}
	break;
	case FBLD:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mem10d:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,10,p_insn->addr_size))
			return;
		fpu87.FBLD_mem10d(buffer);
		break;
	}
	break;
	case FBSTP:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mem10d:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,10,p_insn->addr_size))
			return;
		fpu87.FBSTP_mem10d(buffer);
		break;
	}
	break;
	case FCHS:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case fmt_none:
		fpu87.FCHS_();

		break;
	}
	break;
	case FCLEX:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case fmt_none:
		fpu87.FCLEX_();

		break;
	}
	break;
	case FCMOVB:
	cflags=__CFLAGS__;
	if((eval_cond(cond_B,cflags))) {
		fpu87.FLD_STi(p_insn->code[p_insn->prefix_bytes+1]&7);
	}
	break;
	case FCMOVBE:
	cflags=__CFLAGS__;
	if((eval_cond(cond_NG,cflags))) {
		fpu87.FLD_STi(p_insn->code[p_insn->prefix_bytes+1]&7);
	}
	break;
	case FCMOVE:
	cflags=__CFLAGS__;
	if((eval_cond(cond_E,cflags))) {
		fpu87.FLD_STi(p_insn->code[p_insn->prefix_bytes+1]&7);
	}
	break;
	case FCMOVNB:
	cflags=__CFLAGS__;
	if((eval_cond(cond_NB,cflags))) {
		fpu87.FLD_STi(p_insn->code[p_insn->prefix_bytes+1]&7);
	}
	break;
	case FCMOVNBE:
	cflags=__CFLAGS__;
	if((eval_cond(cond_G,cflags))) {
		fpu87.FLD_STi(p_insn->code[p_insn->prefix_bytes+1]&7);
	}
	break;
	case FCMOVNE:
	cflags=__CFLAGS__;
	if((eval_cond(cond_NE,cflags))) {
		fpu87.FLD_STi(p_insn->code[p_insn->prefix_bytes+1]&7);
	}
	break;
	case FCMOVNU:
	cflags=__CFLAGS__;
	if((eval_cond(cond_PE,cflags))) {
		fpu87.FLD_STi(p_insn->code[p_insn->prefix_bytes+1]&7);
	}
	break;
	case FCMOVU:
	cflags=__CFLAGS__;
	if((eval_cond(cond_PO,cflags))) {
		fpu87.FLD_STi(p_insn->code[p_insn->prefix_bytes+1]&7);
	}
	break;
	case FCOMI:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case ST0i:
		cflags=__CFLAGS__;
		fpu87.FCOMI_ST0i(cflags,p_insn->code[p_insn->prefix_bytes+1]&7);
		__CFLAGS__=cflags;


		break;
	}
	break;
	case FCOMIP:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case ST0i:
		cflags=__CFLAGS__;
		fpu87.FCOMIP_ST0i(cflags,p_insn->code[p_insn->prefix_bytes+1]&7);
		__CFLAGS__=cflags;


		break;
	}
	break;
	case FCOS:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case fmt_none:
		fpu87.FCOS_();

		break;
	}
	break;
	case FDECSTP:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case fmt_none:
		fpu87.FDECSTP_();

		break;
	}
	break;
	case FDIV:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mem8r:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,8,p_insn->addr_size))
			return;
		fpu87.FDIV_mem8r(buffer);
		break;
	case ST0i:
		fpu87.FDIV_ST0i(p_insn->code[p_insn->prefix_bytes+1]&7);

		break;
	case STi0:
		fpu87.FDIV_STi0(p_insn->code[p_insn->prefix_bytes+1]&7);

		break;
	case mem4r:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,4,p_insn->addr_size))
			return;
		fpu87.FDIV_mem4r(buffer);
		break;
	}
	break;
	case FDIVP:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case STi0:
		fpu87.FDIVP_STi0(p_insn->code[p_insn->prefix_bytes+1]&7);

		break;
	}
	break;
	case FDIVR:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mem4r:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,4,p_insn->addr_size))
			return;
		fpu87.FDIVR_mem4r(buffer);
		break;
	case mem8r:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,8,p_insn->addr_size))
			return;
		fpu87.FDIVR_mem8r(buffer);
		break;
	case ST0i:
		fpu87.FDIVR_ST0i(p_insn->code[p_insn->prefix_bytes+1]&7);

		break;
	case STi0:
		fpu87.FDIVR_STi0(p_insn->code[p_insn->prefix_bytes+1]&7);

		break;
	}
	break;
	case FDIVRP:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case STi0:
		fpu87.FDIVRP_STi0(p_insn->code[p_insn->prefix_bytes+1]&7);

		break;
	}
	break;
	case FFREE:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case STi:
		fpu87.FFREE_STi(p_insn->code[p_insn->prefix_bytes+1]&7);

		break;
	}
	break;
	case FIADD:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mem4i:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,4,p_insn->addr_size))
			return;
		fpu87.FIADD_mem4i(data);
		break;
	case mem2i:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,2,p_insn->addr_size))
			return;
		fpu87.FIADD_mem2i((unsigned short)data);
		break;
	}
	break;
	case FICOM:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mem2i:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,2,p_insn->addr_size))
			return;
		fpu87.FICOM_mem2i((unsigned short)data);
		break;
	case mem4i:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,4,p_insn->addr_size))
			return;
		fpu87.FICOM_mem4i(data);
		break;
	}
	break;
	case FICOMP:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mem4i:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,4,p_insn->addr_size))
			return;
		fpu87.FICOMP_mem4i(data);
		break;
	case mem2i:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,2,p_insn->addr_size))
			return;
		fpu87.FICOMP_mem2i((unsigned short)data);
		break;
	}
	break;
	case FIDIV:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mem2i:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,2,p_insn->addr_size))
			return;
		fpu87.FIDIV_mem2i((unsigned short)data);
		break;
	case mem4i:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,4,p_insn->addr_size))
			return;
		fpu87.FIDIV_mem4i(data);
		break;
	}
	break;
	case FIDIVR:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mem2i:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,2,p_insn->addr_size))
			return;
		fpu87.FIDIVR_mem2i((unsigned short)data);
		break;
	case mem4i:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,4,p_insn->addr_size))
			return;
		fpu87.FIDIVR_mem4i(data);
		break;
	}
	break;
	case FILD:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mem2i:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,2,p_insn->addr_size))
			return;
		fpu87.FILD_mem2i((unsigned short)data);
		break;
	case mem4i:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,4,p_insn->addr_size))
			return;
		fpu87.FILD_mem4i(data);
		break;
	case mem8i:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,8,p_insn->addr_size))
			return;
		fpu87.FILD_mem8i((unsigned long *)buffer);
		break;
	}
	break;
	case FIMUL:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mem2i:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,2,p_insn->addr_size))
			return;
		fpu87.FIMUL_mem2i((unsigned short)data);
		break;
	case mem4i:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,4,p_insn->addr_size))
			return;
		fpu87.FIMUL_mem4i(data);
		break;
	}
	break;
	case FINCSTP:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case fmt_none:
		fpu87.FINCSTP_();

		break;
	}
	break;
	case FIST:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mem2i:
	data=fpu87.FIST_mem2i();

		if( !write_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,2,p_insn->addr_size))
			return;
		break;
	case mem4i:
	data=fpu87.FIST_mem4i();

		if( !write_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,4,p_insn->addr_size))
			return;
		break;
	}
	break;
	case FISTP:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mem4i:
	data=fpu87.FISTP_mem4i();

		if( !write_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,4,p_insn->addr_size))
			return;
		break;
	case mem8i:
		fpu87.FISTP_mem8i((unsigned long *)buffer);

		if( !write_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,8,p_insn->addr_size))
			return;
		break;
	case mem2i:
	data=fpu87.FISTP_mem2i();

		if( !write_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,2,p_insn->addr_size))
			return;
		break;
	}
	break;
	case FISTTP:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mem2i:
	data=fpu87.FISTTP_mem2i();

		if( !write_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,2,p_insn->addr_size))
			return;
		break;
	case mem4i:
	data=fpu87.FISTTP_mem4i();

		if( !write_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,4,p_insn->addr_size))
			return;
		break;
	case mem8i:
		fpu87.FISTTP_mem8i((unsigned long *)buffer);

		if( !write_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,8,p_insn->addr_size))
			return;
		break;
	}
	break;
	case FISUB:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mem2i:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,2,p_insn->addr_size))
			return;
		fpu87.FISUB_mem2i((unsigned short)data);
		break;
	case mem4i:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,4,p_insn->addr_size))
			return;
		fpu87.FISUB_mem4i(data);
		break;
	}
	break;
	case FISUBR:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mem2i:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,2,p_insn->addr_size))
			return;
		fpu87.FISUBR_mem2i((unsigned short)data);
		break;
	case mem4i:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,4,p_insn->addr_size))
			return;
		fpu87.FISUBR_mem4i(data);
		break;
	}
	break;
	case FLD:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mem8r:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,8,p_insn->addr_size))
			return;
		fpu87.FLD_mem8r(buffer);
		break;
	case STi:
		fpu87.FLD_STi(p_insn->code[p_insn->prefix_bytes+1]&7);

		break;
	case mem10r:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,10,p_insn->addr_size))
			return;
		fpu87.FLD_mem10r(buffer);
		break;
	case mem4r:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,4,p_insn->addr_size))
			return;
		fpu87.FLD_mem4r(buffer);
		break;
	}
	break;
	case FLD1:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case fmt_none:
		fpu87.FLD1_();

		break;
	}
	break;
	case FLDCW:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mem2i:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,2,p_insn->addr_size))
			return;
		fpu87.FLDCW_mem2i((unsigned short)data);
		break;
	}
	break;
	case FLDENV:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mem14:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,14,p_insn->addr_size))
			return;
		fpu87.FLDENV_mem14(buffer);
		break;
	}
	break;
	case FLDL2E:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case fmt_none:
		fpu87.FLDL2E_();

		break;
	}
	break;
	case FLDL2T:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case fmt_none:
		fpu87.FLDL2T_();

		break;
	}
	break;
	case FLDLG2:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case fmt_none:
		fpu87.FLDLG2_();

		break;
	}
	break;
	case FLDLN2:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case fmt_none:
		fpu87.FLDLN2_();

		break;
	}
	break;
	case FLDPI:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case fmt_none:
		fpu87.FLDPI_();

		break;
	}
	break;
	case FLDZ:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case fmt_none:
		fpu87.FLDZ_();

		break;
	}
	break;
	case FMUL:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mem4r:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,4,p_insn->addr_size))
			return;
		fpu87.FMUL_mem4r(buffer);
		break;
	case mem8r:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,8,p_insn->addr_size))
			return;
		fpu87.FMUL_mem8r(buffer);
		break;
	case ST0i:
		fpu87.FMUL_ST0i(p_insn->code[p_insn->prefix_bytes+1]&7);

		break;
	case STi0:
		fpu87.FMUL_STi0(p_insn->code[p_insn->prefix_bytes+1]&7);

		break;
	}
	break;
	case FMULP:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case STi0:
		fpu87.FMULP_STi0(p_insn->code[p_insn->prefix_bytes+1]&7);

		break;
	}
	break;
	case FNINIT:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case fmt_none:
		fpu87.FNINIT_();

		break;
	}
	break;
	case FNOP:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case fmt_none:
		fpu87.FNOP_();

		break;
	}
	break;
	case FNSTCW:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mem2i:
	data=fpu87.FNSTCW_mem2i();

		if( !write_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,2,p_insn->addr_size))
			return;
		break;
	}
	break;
	case FNSTENV:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mem14:
		fpu87.FNSTENV_mem14(buffer);

		if( !write_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,14,p_insn->addr_size))
			return;
		break;
	}
	break;
	case FNSTSW:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case fmt_AX:
	reg.reg_cat=RC_REGG16;
	reg.reg_no=0;
	data=fpu87.FNSTSW_fmt_AX();
	update_reg(reg,data);
		break;
	case mem2i:
	data=fpu87.FNSTSW_mem2i();

		if( !write_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,2,p_insn->addr_size))
			return;
		break;
	}
	break;
	case FPATAN:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case fmt_none:
		fpu87.FPATAN_();

		break;
	}
	break;
	case FPREM:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case fmt_none:
		fpu87.FPREM_();

		break;
	}
	break;
	case FPREM1:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case fmt_none:
		fpu87.FPREM1_();

		break;
	}
	break;
	case FPTAN:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case fmt_none:
		fpu87.FPTAN_();

		break;
	}
	break;
	case FRNDINT:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case fmt_none:
		fpu87.FRNDINT_();

		break;
	}
	break;
	case FRSTOR:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mem108:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		fpu87.FRSTOR_mem108(p_insn->data_size,buffer);
		break;
	}
	break;
	case FSAVE:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mem108:
		fpu87.FSAVE_mem108(p_insn->data_size,buffer);

		if( !write_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		break;
	}
	break;
	case FSCALE:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case fmt_none:
		fpu87.FSCALE_();

		break;
	}
	break;
	case FSIN:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case fmt_none:
		fpu87.FSIN_();

		break;
	}
	break;
	case FSINCOS:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case fmt_none:
		fpu87.FSINCOS_();

		break;
	}
	break;
	case FSQRT:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case fmt_none:
		fpu87.FSQRT_();

		break;
	}
	break;
	case FST:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mem8r:
		fpu87.FST_mem8r(buffer);

		if( !write_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,8,p_insn->addr_size))
			return;
		break;
	case STi:
		fpu87.FST_STi(p_insn->code[p_insn->prefix_bytes+1]&7);

		break;
	case mem4r:
		fpu87.FST_mem4r(buffer);

		if( !write_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,4,p_insn->addr_size))
			return;
		break;
	}
	break;
	case FSTP:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mem4r:
		fpu87.FSTP_mem4r(buffer);

		if( !write_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,4,p_insn->addr_size))
			return;
		break;
	case mem10r:
		fpu87.FSTP_mem10r(buffer);

		if( !write_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,10,p_insn->addr_size))
			return;
		break;
	case STi:
		fpu87.FSTP_STi(p_insn->code[p_insn->prefix_bytes+1]&7);

		break;
	case mem8r:
		fpu87.FSTP_mem8r(buffer);

		if( !write_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,8,p_insn->addr_size))
			return;
		break;
	}
	break;
	case FSUB:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case STi0:
		fpu87.FSUB_STi0(p_insn->code[p_insn->prefix_bytes+1]&7);

		break;
	case ST0i:
		fpu87.FSUB_ST0i(p_insn->code[p_insn->prefix_bytes+1]&7);

		break;
	case mem4r:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,4,p_insn->addr_size))
			return;
		fpu87.FSUB_mem4r(buffer);
		break;
	case mem8r:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,8,p_insn->addr_size))
			return;
		fpu87.FSUB_mem8r(buffer);
		break;
	}
	break;
	case FSUBP:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case STi0:
		fpu87.FSUBP_STi0(p_insn->code[p_insn->prefix_bytes+1]&7);

		break;
	}
	break;
	case FSUBR:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case ST0i:
		fpu87.FSUBR_ST0i(p_insn->code[p_insn->prefix_bytes+1]&7);

		break;
	case mem8r:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,8,p_insn->addr_size))
			return;
		fpu87.FSUBR_mem8r(buffer);
		break;
	case STi0:
		fpu87.FSUBR_STi0(p_insn->code[p_insn->prefix_bytes+1]&7);

		break;
	case mem4r:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,4,p_insn->addr_size))
			return;
		fpu87.FSUBR_mem4r(buffer);
		break;
	}
	break;
	case FSUBRP:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case STi0:
		fpu87.FSUBRP_STi0(p_insn->code[p_insn->prefix_bytes+1]&7);

		break;
	}
	break;
	case FTST:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case fmt_none:
		fpu87.FTST_();

		break;
	}
	break;
	case FUCOM:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case STi:
		fpu87.FUCOM_STi(p_insn->code[p_insn->prefix_bytes+1]&7);

		break;
	}
	break;
	case FUCOMI:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case ST0i:
		cflags=__CFLAGS__;
		fpu87.FUCOMI_ST0i(cflags,p_insn->code[p_insn->prefix_bytes+1]&7);
		__CFLAGS__=cflags;


		break;
	}
	break;
	case FUCOMIP:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case ST0i:
		cflags=__CFLAGS__;
		fpu87.FUCOMIP_ST0i(cflags,p_insn->code[p_insn->prefix_bytes+1]&7);
		__CFLAGS__=cflags;


		break;
	}
	break;
	case FUCOMP:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case fmt_none:
		fpu87.FUCOMP_();

		break;
	case STi:
		fpu87.FUCOMP_STi(p_insn->code[p_insn->prefix_bytes+1]&7);

		break;
	}
	break;
	case FUCOMPP:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case fmt_none:
		fpu87.FUCOMPP_();

		break;
	}
	break;
	case FXAM:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case fmt_none:
		fpu87.FXAM_();

		break;
	}
	break;
	case FXCH:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case STi:
		fpu87.FXCH_STi(p_insn->code[p_insn->prefix_bytes+1]&7);

		break;
	}
	break;
	case FXTRACT:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case fmt_none:
		fpu87.FXTRACT_();

		break;
	}
	break;
	case FYL2X:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case fmt_none:
		fpu87.FYL2X_();

		break;
	}
	break;
	case FYL2XP1:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case fmt_none:
		fpu87.FYL2XP1_();

		break;

	}
	}
}
