#include <stdio.h>
#include "condition.h"
#include "cpu.h"

void CCPU:: fpu_insn_exec(PINSN_BASIC_INFO p_insn,PINSN_EXEC_INFO p_insn_exec_info)
{
	union {unsigned long data;char buffer[128];unsigned long l[4];};
	REG_CODE reg;
	unsigned short cflags;

	switch(p_insn->p_instr_desc->index)
	{
	case PMINUB:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.PMINUB_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case PREFETCHT0:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case fmt_none:
		sse_core.PREFETCHT0_();

		break;
	}
	break;
	case PREFETCHT1:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case fmt_none:
		sse_core.PREFETCHT1_();

		break;
	}
	break;
	case PREFETCHT2:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case fmt_none:
		sse_core.PREFETCHT2_();

		break;
	}
	break;
	case PREFETCHTA:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case fmt_none:
		sse_core.PREFETCHTA_();

		break;
	}
	break;
	case addpd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.addpd_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case addps:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.addps_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case addsd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm64:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,16,p_insn->addr_size))
			return;
		sse_core.addsd_xmm_xm64(buffer);
		break;
	}
	break;
	case addss:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm32:
		if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,32,p_insn->addr_size))
			return;
		sse_core.addss_xmm_xm32(data);
		break;
	}
	break;
	case addsubpd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.addsubpd_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case addsubps:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.addsubps_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case andnpd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.andnpd_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case andnps:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.andnps_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case andpd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.andpd_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case andps:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.andps_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case blendpd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128Ib:
		if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,16,p_insn->addr_size))
			return;
		sse_core.blendpd_xmm_xm128Ib((unsigned short)data);
		break;
	}
	break;
	case blendps:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128Ib:
		if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,16,p_insn->addr_size))
			return;
		sse_core.blendps_xmm_xm128Ib((unsigned short)data);
		break;
	}
	break;
	case blendvpd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.blendvpd_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case blendvps:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.blendvps_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case cmppd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128Ib:
		if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,16,p_insn->addr_size))
			return;
		sse_core.cmppd_xmm_xm128Ib((unsigned short)data);
		break;
	}
	break;
	case cmpps:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128Ib:
		if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,16,p_insn->addr_size))
			return;
		sse_core.cmpps_xmm_xm128Ib((unsigned short)data);
		break;
	}
	break;
	case cmpsd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm64Ib:
	reg.reg_cat=RC_REGG16;
	reg.reg_no=0;
	data=reg_file.read_reg((reg);
		sse_core.cmpsd_xmm_xm64Ib(data);
	update_reg(reg,data);
		break;
	}
	break;
	case cmpss:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm32Ib:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,4,p_insn->addr_size))
			return;
		sse_core.cmpss_xmm_xm32Ib(buffer);
		break;
	}
	break;
	case comisd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm64:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,16,p_insn->addr_size))
			return;
		sse_core.comisd_xmm_xm64(buffer);
		break;
	}
	break;
	case comiss:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm32:
		if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,32,p_insn->addr_size))
			return;
		sse_core.comiss_xmm_xm32(data);
		break;
	}
	break;
	case cvtdq2ps:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.cvtdq2ps_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case cvtpd2dq:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.cvtpd2dq_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case cvtpd2pi:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.cvtpd2pi_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case cvtpd2ps:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.cvtpd2ps_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case cvtpi2pd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_mmm64:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,8,p_insn->addr_size))
			return;
		sse_core.cvtpi2pd_xmm_mmm64(buffer);
		break;
	}
	break;
	case cvtpi2ps:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_mmm64:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,8,p_insn->addr_size))
			return;
		sse_core.cvtpi2ps_xmm_mmm64(buffer);
		break;
	}
	break;
	case cvtps2dq:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.cvtps2dq_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case cvtps2pd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm64:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,16,p_insn->addr_size))
			return;
		sse_core.cvtps2pd_xmm_xm64(buffer);
		break;
	}
	break;
	case cvtps2pi:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mm_xm64:
		sse_core.cvtps2pi_mm_xm64();

		break;
	}
	break;
	case cvtsd2si:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case r32_xm64:
		sse_core.cvtsd2si_r32_xm64();

		break;
	}
	break;
	case cvtsd2ss:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm64:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,16,p_insn->addr_size))
			return;
		sse_core.cvtsd2ss_xmm_xm64(buffer);
		break;
	}
	break;
	case cvtsi2sd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_rm32:
		sse_core.cvtsi2sd_xmm_rm32();

		break;
	}
	break;
	case cvtsi2ss:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_rm32:
		sse_core.cvtsi2ss_xmm_rm32();

		break;
	}
	break;
	case cvtss2sd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm32:
		if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,32,p_insn->addr_size))
			return;
		sse_core.cvtss2sd_xmm_xm32(data);
		break;
	}
	break;
	case cvtss2si:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case r32_xm32:
		sse_core.cvtss2si_r32_xm32(p_insn->code[p_insn->prefix_bytes+1]&7);

		break;
	}
	break;
	case cvttpd2dq:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.cvttpd2dq_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case cvttpd2pi:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mm_xm128:
		sse_core.cvttpd2pi_mm_xm128();

		break;
	}
	break;
	case cvttps2dq:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.cvttps2dq_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case cvttps2pi:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mm_xm64:
		sse_core.cvttps2pi_mm_xm64();

		break;
	}
	break;
	case cvttsd2si:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case r32_xm64:
		sse_core.cvttsd2si_r32_xm64();

		break;
	}
	break;
	case cvttss2si:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case r32_xm32:
		sse_core.cvttss2si_r32_xm32(p_insn->code[p_insn->prefix_bytes+1]&7);

		break;
	}
	break;
	case divpd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.divpd_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case divps:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.divps_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case divsd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm64:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,16,p_insn->addr_size))
			return;
		sse_core.divsd_xmm_xm64(buffer);
		break;
	}
	break;
	case divss:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm32:
		if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,32,p_insn->addr_size))
			return;
		sse_core.divss_xmm_xm32(data);
		break;
	}
	break;
	case dppd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128Ib:
		if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,16,p_insn->addr_size))
			return;
		sse_core.dppd_xmm_xm128Ib((unsigned short)data);
		break;
	}
	break;
	case dpps:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128Ib:
		if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,16,p_insn->addr_size))
			return;
		sse_core.dpps_xmm_xm128Ib((unsigned short)data);
		break;
	}
	break;
	case extractps:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128Ib:
		if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,16,p_insn->addr_size))
			return;
		sse_core.extractps_xmm_xm128Ib((unsigned short)data);
		break;
	}
	break;
	case haddpd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.haddpd_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case haddps:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.haddps_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case hsubpd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.hsubpd_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case hsubps:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.hsubps_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case insertps:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128Ib:
		if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,16,p_insn->addr_size))
			return;
		sse_core.insertps_xmm_xm128Ib((unsigned short)data);
		break;
	}
	break;
	case lddqu:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.lddqu_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case maskmovdqu:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.maskmovdqu_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case maskmovq:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.maskmovq_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case maxpd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.maxpd_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case maxps:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.maxps_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case maxsd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm64:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,16,p_insn->addr_size))
			return;
		sse_core.maxsd_xmm_xm64(buffer);
		break;
	}
	break;
	case maxss:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm32:
		if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,32,p_insn->addr_size))
			return;
		sse_core.maxss_xmm_xm32(data);
		break;
	}
	break;
	case minpd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.minpd_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case minps:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.minps_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case minsd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm64:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,16,p_insn->addr_size))
			return;
		sse_core.minsd_xmm_xm64(buffer);
		break;
	}
	break;
	case minss:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm32:
		if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,32,p_insn->addr_size))
			return;
		sse_core.minss_xmm_xm32(data);
		break;
	}
	break;
	case movapd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.movapd_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case movaps:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.movaps_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case movd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mm_rm32:
		sse_core.movd_mm_rm32(p_insn->code[p_insn->prefix_bytes+1]&7);

		break;
	case xmm_rm32:
		sse_core.movd_xmm_rm32();

		break;
	}
	break;
	case movddup:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.movddup_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case movdq2q:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.movdq2q_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case movdqa:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.movdqa_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case movdqu:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.movdqu_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case movhlps:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.movhlps_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case movhpd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.movhpd_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case movhps:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.movhps_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case movlhpd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.movlhpd_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case movlhps:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.movlhps_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case movlpd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128v:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,10,p_insn->addr_size))
			return;
		sse_core.movlpd_xmm_xm128v(buffer);
		break;
	}
	break;
	case movlps:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128v:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,10,p_insn->addr_size))
			return;
		sse_core.movlps_xmm_xm128v(buffer);
		break;
	}
	break;
	case movmskpd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case r32_xmm:
		sse_core.movmskpd_r32_xmm();

		break;
	}
	break;
	case movmskps:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case r32_xmm:
		sse_core.movmskps_r32_xmm();

		break;
	}
	break;
	case movndq:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case m128_xmm:
		sse_core.movndq_m128_xmm();

		break;
	}
	break;
	case movntdqa:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm1_m128:
		sse_core.movntdqa_xmm1_m128();

		break;
	}
	break;
	case movnti:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case EvGv:
		sse_core.movnti_EvGv();

		break;
	}
	break;
	case movntpd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case m128_xmm:
		sse_core.movntpd_m128_xmm();

		break;
	}
	break;
	case movntps:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case m128_xmm:
		sse_core.movntps_m128_xmm();

		break;
	}
	break;
	case movntq:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case m64_mm:
		sse_core.movntq_m64_mm();

		break;
	}
	break;
	case movq:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xm64_xmm:
		sse_core.movq_xm64_xmm();

		break;
	case xmm_xm64:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,16,p_insn->addr_size))
			return;
		sse_core.movq_xmm_xm64(buffer);
		break;
	case mm_rm64:
		sse_core.movq_mm_rm64();

		break;
	case xmm_rm64:
		sse_core.movq_xmm_rm64();

		break;
	case mm_mmm64:
		sse_core.movq_mm_mmm64();

		break;
	}
	break;
	case movq2dq:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.movq2dq_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case movsd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm64:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,16,p_insn->addr_size))
			return;
		sse_core.movsd_xmm_xm64(buffer);
		break;
	}
	break;
	case movshdup:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.movshdup_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case movsldup:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.movsldup_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case movss:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm32:
		if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,32,p_insn->addr_size))
			return;
		sse_core.movss_xmm_xm32(data);
		break;
	}
	break;
	case movupd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.movupd_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case movups:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.movups_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case mpsadbw:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128Ib:
		if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,16,p_insn->addr_size))
			return;
		sse_core.mpsadbw_xmm_xm128Ib((unsigned short)data);
		break;
	}
	break;
	case mulpd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.mulpd_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case mulps:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.mulps_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case mulsd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm64:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,16,p_insn->addr_size))
			return;
		sse_core.mulsd_xmm_xm64(buffer);
		break;
	}
	break;
	case mulss:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm32:
		if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,32,p_insn->addr_size))
			return;
		sse_core.mulss_xmm_xm32(data);
		break;
	}
	break;
	case orpd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.orpd_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case orps:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.orps_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case packssdw:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.packssdw_xmm_xm128(p_insn->data_size,buffer);
		break;
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.packssdw_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case packsswb:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.packsswb_xmm_xm128(p_insn->data_size,buffer);
		break;
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.packsswb_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case packuswb:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.packuswb_xmm_xm128(p_insn->data_size,buffer);
		break;
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.packuswb_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case paddb:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.paddb_xmm_xm128(p_insn->data_size,buffer);
		break;
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.paddb_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case paddd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.paddd_xmm_xm128(p_insn->data_size,buffer);
		break;
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.paddd_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case paddq:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.paddq_xmm_xm128(p_insn->data_size,buffer);
		break;
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.paddq_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case paddsb:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.paddsb_xmm_xm128(p_insn->data_size,buffer);
		break;
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.paddsb_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case paddsw:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.paddsw_xmm_xm128(p_insn->data_size,buffer);
		break;
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.paddsw_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case paddusb:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.paddusb_xmm_xm128(p_insn->data_size,buffer);
		break;
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.paddusb_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case paddusw:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.paddusw_xmm_xm128(p_insn->data_size,buffer);
		break;
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.paddusw_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case paddw:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.paddw_xmm_xm128(p_insn->data_size,buffer);
		break;
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.paddw_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case pand:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.pand_xmm_xm128(p_insn->data_size,buffer);
		break;
	case mm_mm64:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,10,p_insn->addr_size))
			return;
		sse_core.pand_mm_mm64(buffer);
		break;
	}
	break;
	case pandn:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.pandn_xmm_xm128(p_insn->data_size,buffer);
		break;
	case mm_mm64:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,10,p_insn->addr_size))
			return;
		sse_core.pandn_mm_mm64(buffer);
		break;
	}
	break;
	case pavgb:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.pavgb_xmm_xm128(p_insn->data_size,buffer);
		break;
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.pavgb_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case pavgw:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.pavgw_xmm_xm128(p_insn->data_size,buffer);
		break;
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.pavgw_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case pcmpeqb:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.pcmpeqb_xmm_xm128(p_insn->data_size,buffer);
		break;
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.pcmpeqb_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case pcmpeqd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.pcmpeqd_xmm_xm128(p_insn->data_size,buffer);
		break;
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.pcmpeqd_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case pcmpeqw:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.pcmpeqw_xmm_xm128(p_insn->data_size,buffer);
		break;
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.pcmpeqw_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case pcmpgtb:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.pcmpgtb_xmm_xm128(p_insn->data_size,buffer);
		break;
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.pcmpgtb_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case pcmpgtd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.pcmpgtd_xmm_xm128(p_insn->data_size,buffer);
		break;
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.pcmpgtd_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case pcmpgtw:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.pcmpgtw_xmm_xm128(p_insn->data_size,buffer);
		break;
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.pcmpgtw_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case pextrw:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128Ib:
		if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,16,p_insn->addr_size))
			return;
		sse_core.pextrw_xmm_xm128Ib((unsigned short)data);
		break;
	}
	break;
	case pinsrw:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128Ib:
		if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,16,p_insn->addr_size))
			return;
		sse_core.pinsrw_xmm_xm128Ib((unsigned short)data);
		break;
	}
	break;
	case pmaddwd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.pmaddwd_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case pmaxsw:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.pmaxsw_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case pmaxub:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.pmaxub_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case pminsw:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.pminsw_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case pmovmskb:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.pmovmskb_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case pmulhuw:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.pmulhuw_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case pmullhw:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.pmullhw_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case pmulludq:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.pmulludq_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case pmullw:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.pmullw_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case por:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.por_xmm_xm128(p_insn->data_size,buffer);
		break;
	case mm_mm64:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,10,p_insn->addr_size))
			return;
		sse_core.por_mm_mm64(buffer);
		break;
	}
	break;
	case ppushd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mm_mm64:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,10,p_insn->addr_size))
			return;
		sse_core.ppushd_mm_mm64(buffer);
		break;
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.ppushd_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case psadbw:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mm_mm64:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,10,p_insn->addr_size))
			return;
		sse_core.psadbw_mm_mm64(buffer);
		break;
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.psadbw_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case pshufhw:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128Ib:
		if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,16,p_insn->addr_size))
			return;
		sse_core.pshufhw_xmm_xm128Ib((unsigned short)data);
		break;
	}
	break;
	case pshuflw:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128Ib:
		if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,16,p_insn->addr_size))
			return;
		sse_core.pshuflw_xmm_xm128Ib((unsigned short)data);
		break;
	}
	break;
	case pshufw:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128Ib:
		if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,16,p_insn->addr_size))
			return;
		sse_core.pshufw_xmm_xm128Ib((unsigned short)data);
		break;
	}
	break;
	case pslld:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mm_mm64:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,10,p_insn->addr_size))
			return;
		sse_core.pslld_mm_mm64(buffer);
		break;
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.pslld_xmm_xm128(p_insn->data_size,buffer);
		break;
	case xmm_xm128Ib:
		if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,16,p_insn->addr_size))
			return;
		sse_core.pslld_xmm_xm128Ib((unsigned short)data);
		break;
	}
	break;
	case pslldq:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128Ib:
		if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,16,p_insn->addr_size))
			return;
		sse_core.pslldq_xmm_xm128Ib((unsigned short)data);
		break;
	}
	break;
	case psllq:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mm_mm64:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,10,p_insn->addr_size))
			return;
		sse_core.psllq_mm_mm64(buffer);
		break;
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.psllq_xmm_xm128(p_insn->data_size,buffer);
		break;
	case xmm_xm128Ib:
		if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,16,p_insn->addr_size))
			return;
		sse_core.psllq_xmm_xm128Ib((unsigned short)data);
		break;
	}
	break;
	case psllw:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.psllw_xmm_xm128(p_insn->data_size,buffer);
		break;
	case mm_mm64:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,10,p_insn->addr_size))
			return;
		sse_core.psllw_mm_mm64(buffer);
		break;
	case xmm_xm128Ib:
		if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,16,p_insn->addr_size))
			return;
		sse_core.psllw_xmm_xm128Ib((unsigned short)data);
		break;
	}
	break;
	case psrad:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mm_mm64:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,10,p_insn->addr_size))
			return;
		sse_core.psrad_mm_mm64(buffer);
		break;
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.psrad_xmm_xm128(p_insn->data_size,buffer);
		break;
	case xmm_xm128Ib:
		if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,16,p_insn->addr_size))
			return;
		sse_core.psrad_xmm_xm128Ib((unsigned short)data);
		break;
	}
	break;
	case psraw:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128v:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,10,p_insn->addr_size))
			return;
		sse_core.psraw_xmm_xm128v(buffer);
		break;
	case xmm_xm128v:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,10,p_insn->addr_size))
			return;
		sse_core.psraw_xmm_xm128v(buffer);
		break;
	case xmm_xm128Ib:
		if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,16,p_insn->addr_size))
			return;
		sse_core.psraw_xmm_xm128Ib((unsigned short)data);
		break;
	}
	break;
	case psrldq:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128Ib:
		if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,16,p_insn->addr_size))
			return;
		sse_core.psrldq_xmm_xm128Ib((unsigned short)data);
		break;
	}
	break;
	case psubb:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mm_mm64:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,10,p_insn->addr_size))
			return;
		sse_core.psubb_mm_mm64(buffer);
		break;
	}
	break;
	case psubd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mm_mm64:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,10,p_insn->addr_size))
			return;
		sse_core.psubd_mm_mm64(buffer);
		break;
	}
	break;
	case psubq:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.psubq_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case psubsb:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mm_mm64:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,10,p_insn->addr_size))
			return;
		sse_core.psubsb_mm_mm64(buffer);
		break;
	}
	break;
	case psubsw:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.psubsw_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case psubusb:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mm_mm64:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,10,p_insn->addr_size))
			return;
		sse_core.psubusb_mm_mm64(buffer);
		break;
	}
	break;
	case psubusw:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.psubusw_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case psubw:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.psubw_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case punpckhbw:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mm_mm32:
		sse_core.punpckhbw_mm_mm32(p_insn->code[p_insn->prefix_bytes+1]&7);

		break;
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.punpckhbw_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case punpckhdq:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mm_mm32:
		sse_core.punpckhdq_mm_mm32(p_insn->code[p_insn->prefix_bytes+1]&7);

		break;
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.punpckhdq_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case punpckhqdq:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.punpckhqdq_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case punpckhwd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mm_mm32:
		sse_core.punpckhwd_mm_mm32(p_insn->code[p_insn->prefix_bytes+1]&7);

		break;
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.punpckhwd_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case punpcklbw:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.punpcklbw_xmm_xm128(p_insn->data_size,buffer);
		break;
	case mm_mm32:
		sse_core.punpcklbw_mm_mm32(p_insn->code[p_insn->prefix_bytes+1]&7);

		break;
	}
	break;
	case punpckldq:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mm_mm32:
		sse_core.punpckldq_mm_mm32(p_insn->code[p_insn->prefix_bytes+1]&7);

		break;
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.punpckldq_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case punpcklqdq:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.punpcklqdq_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case punpcklwd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case mm_mm32:
		sse_core.punpcklwd_mm_mm32(p_insn->code[p_insn->prefix_bytes+1]&7);

		break;
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.punpcklwd_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case pxor:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.pxor_xmm_xm128(p_insn->data_size,buffer);
		break;
	case mm_mm64:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,10,p_insn->addr_size))
			return;
		sse_core.pxor_mm_mm64(buffer);
		break;
	}
	break;
	case rcpps:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.rcpps_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case rcpss:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.rcpss_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case rsqrtps:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.rsqrtps_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case rsqrtss:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.rsqrtss_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case shufpd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128Ib:
		if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,16,p_insn->addr_size))
			return;
		sse_core.shufpd_xmm_xm128Ib((unsigned short)data);
		break;
	}
	break;
	case shufps:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128Ib:
		if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,16,p_insn->addr_size))
			return;
		sse_core.shufps_xmm_xm128Ib((unsigned short)data);
		break;
	}
	break;
	case sqrtps:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.sqrtps_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case sqrtsd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm64:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,16,p_insn->addr_size))
			return;
		sse_core.sqrtsd_xmm_xm64(buffer);
		break;
	}
	break;
	case sqrtss:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm32:
		if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,32,p_insn->addr_size))
			return;
		sse_core.sqrtss_xmm_xm32(data);
		break;
	}
	break;
	case subpd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.subpd_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case subps:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.subps_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case subsd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm64:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,16,p_insn->addr_size))
			return;
		sse_core.subsd_xmm_xm64(buffer);
		break;
	}
	break;
	case subss:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm32:
		if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,32,p_insn->addr_size))
			return;
		sse_core.subss_xmm_xm32(data);
		break;
	}
	break;
	case ucomisd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm64:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,16,p_insn->addr_size))
			return;
		sse_core.ucomisd_xmm_xm64(buffer);
		break;
	}
	break;
	case ucomiss:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm32:
		if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,32,p_insn->addr_size))
			return;
		sse_core.ucomiss_xmm_xm32(data);
		break;
	}
	break;
	case unpckhpd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.unpckhpd_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case unpckhps:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.unpckhps_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case unpcklpd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.unpcklpd_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case unpcklps:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.unpcklps_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case xorpd:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.xorpd_xmm_xm128(p_insn->data_size,buffer);
		break;
	}
	break;
	case xorps:
	switch (p_insn->p_instr_desc->op_fmt)
	{
	case xmm_xm128:
		if( !read_string(p_insn_exec_info->r_m.operand_desc.mem,buffer,p_insn->data_size==32?108:94,p_insn->addr_size))
			return;
		sse_core.xorps_xmm_xm128(p_insn->data_size,buffer);
		break;

	}
	}
}
