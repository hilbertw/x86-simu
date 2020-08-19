#include <stdio.h>
#include <assert.h>

#include "alucore.h"
#include "cpu.h"
extern bool step;
extern struct reg_update_rec reg_update_table[10];
extern int reg_update_rec_num;
void CCPU:: mrom_insn_exec(PINSN_BASIC_INFO p_insn,PINSN_EXEC_INFO p_insn_exec_info)
{
	int n;
	unsigned short cflag;
	unsigned long data1,data,port,off,addr_hi,addr_lo,result;

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
			bug();
		reg_update_rec_num++;
		break;
	case out:
		assert(p_insn_exec_info->type.reg_present);

		data=reg_file.read_reg(p_insn_exec_info->reg);
		n=(p_insn_exec_info->flag&EXEC_F_WORD)?p_insn->data_size:8;
		if(p_insn_exec_info->type.imm_present)
		{

			outport((unsigned short)p_insn_exec_info->imm,data,n);

		}
		else if(p_insn_exec_info->type.s_reg_present)
		{
			port=reg_file.read_reg(p_insn_exec_info->secondary_reg.reg);
			outport((unsigned short)port,data,n);
		}
		else
			bug();
		break;
	case ins:
		if(PREFIX_REP(p_insn->prefix))
		ins_r((p_insn_exec_info->flag&EXEC_F_WORD)?p_insn->data_size:8,p_insn->addr_size);
		else
		ins_((p_insn_exec_info->flag&EXEC_F_WORD)?p_insn->data_size:8,p_insn->addr_size);
		break;
	case outs:
		if(PREFIX_REP(p_insn->prefix))
		outs_r(PREFIX_SEG(p_insn->prefix),(p_insn_exec_info->flag&EXEC_F_WORD)?p_insn->data_size:8,p_insn->addr_size);
		else
		outs_(PREFIX_SEG(p_insn->prefix),(p_insn_exec_info->flag&EXEC_F_WORD)?p_insn->data_size:8,p_insn->addr_size);
		break;
	case invd:
		break;
	case intx: 
		if(p_insn_exec_info->type.imm_present)
		{
			int_call((unsigned char)p_insn_exec_info->imm,p_insn->data_size);
		}
		else bug();
		break;
	case  xchg:
		n=(p_insn_exec_info->flag&EXEC_F_WORD)?p_insn->data_size:8;
		if((p_insn_exec_info->op_presence&(OP_PRESENCE_RM|OP_PRESENCE_REG))==(OP_PRESENCE_RM|OP_PRESENCE_REG))
		{
			data=reg_file.read_reg(p_insn_exec_info->reg);
			if((p_insn_exec_info->op_presence&OP_PRESENCE_RM_R)==OP_PRESENCE_RM)
			{
				if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data1,n,p_insn->addr_size)) 
					return;
				if( !write_mem(p_insn_exec_info->r_m.operand_desc.mem,data,n,p_insn->addr_size)) 
					return;
			}
			else
			{
				data1=reg_file.read_reg(p_insn_exec_info->r_m.operand_desc.reg);
				update_reg(p_insn_exec_info->r_m.operand_desc.reg,data);
			}
			
			update_reg(p_insn_exec_info->reg,data1);
		}
		else if(p_insn_exec_info->op_presence==(OP_PRESENCE_REG_SEC|OP_PRESENCE_REG))
		{
			data=reg_file.read_reg(p_insn_exec_info->reg);
			data1=reg_file.read_reg(p_insn_exec_info->secondary_reg.reg);
			update_reg(p_insn_exec_info->reg,data1);
			update_reg(p_insn_exec_info->secondary_reg.reg,data);
		}

		break;
	//case smsw:
	//	//This instruction is provided for compatibility with the 80286; 
	//	//80386 programs should use MOV ..., CR0. 
	//	assert(p_insn_exec_info->type.rm_present);
	//	data=*(unsigned short *)&__CR0__;
	//	if (p_insn_exec_info->r_m.type==OT_REG)
	//	{
	//		assert(p_insn_exec_info->r_m.operand_desc.reg.reg_cat==RC_REGG16);
	//		reg_update_table[reg_update_rec_num].data=data;
	//		reg_update_table[reg_update_rec_num].reg=p_insn_exec_info->r_m.operand_desc.reg;
	//		reg_update_rec_num++;
	//	}
	//	else
	//		if( !write_mem(p_insn_exec_info->r_m.operand_desc.mem,data,16,p_insn->addr_size)) return;

	//	break;
	case enter:
		if(p_insn_exec_info->type.imm_present)
		{
			assert(p_insn_exec_info->type.s_imm_present);
			enter_((unsigned short)p_insn_exec_info->imm,p_insn_exec_info->secondary_imm.imm,p_insn->data_size);
		}
		else bug();
		break;
	case leave:
		leave_(p_insn->data_size);
		break;
	case movs:
		if(PREFIX_REP(p_insn->prefix))
		movs_r(PREFIX_SEG(p_insn->prefix),(p_insn_exec_info->flag&EXEC_F_WORD)?p_insn->data_size:8,
			p_insn->addr_size);
		else
		movs_(PREFIX_SEG(p_insn->prefix),(p_insn_exec_info->flag&EXEC_F_WORD)?p_insn->data_size:8,
			p_insn->addr_size);

		break;
	case lods:
		if(PREFIX_REP(p_insn->prefix))
		lods_r(PREFIX_SEG(p_insn->prefix),(p_insn_exec_info->flag&EXEC_F_WORD)?p_insn->data_size:8,
			p_insn->addr_size);
		else
		lods_(PREFIX_SEG(p_insn->prefix),(p_insn_exec_info->flag&EXEC_F_WORD)?p_insn->data_size:8,
			p_insn->addr_size);

		break;
	case stos:
		if(PREFIX_REP(p_insn->prefix))
		stos_r((p_insn_exec_info->flag&EXEC_F_WORD)?p_insn->data_size:8,
			p_insn->addr_size);
		else
		stos_((p_insn_exec_info->flag&EXEC_F_WORD)?p_insn->data_size:8,
			p_insn->addr_size);

		break;
	case scas:
		if(PREFIX_REPZ&p_insn->prefix)
		scas_r((p_insn_exec_info->flag&EXEC_F_WORD)?p_insn->data_size:8,
			p_insn->addr_size);
		else if(PREFIX_REPNZ&p_insn->prefix)
		scas_rnz((p_insn_exec_info->flag&EXEC_F_WORD)?p_insn->data_size:8,
			p_insn->addr_size);
		else
		scas_((p_insn_exec_info->flag&EXEC_F_WORD)?p_insn->data_size:8,
			p_insn->addr_size);

		break;
	case cmps:
		if(PREFIX_REPZ&p_insn->prefix)
		cmps_r(PREFIX_SEG(p_insn->prefix),(p_insn_exec_info->flag&EXEC_F_WORD)?p_insn->data_size:8,
			p_insn->addr_size);
		else if(PREFIX_REPNZ&p_insn->prefix)
		cmps_rnz(PREFIX_SEG(p_insn->prefix),(p_insn_exec_info->flag&EXEC_F_WORD)?p_insn->data_size:8,
			p_insn->addr_size);
		else
		cmps_(PREFIX_SEG(p_insn->prefix),(p_insn_exec_info->flag&EXEC_F_WORD)?p_insn->data_size:8,
			p_insn->addr_size);

		break;

	case bswap:
		assert(p_insn_exec_info->type.reg_present);
//To swap bytes in a word value (16-bit register), use the XCHG instruction. 
//When the BSWAP instruction references a 16-bit register, the result is undefined.
		if(p_insn_exec_info->reg.reg_cat==RC_REGG32)
		{
			data=reg_file.read_reg(p_insn_exec_info->reg);

#ifdef __GNUC__

	__asm__ __volatile__ (
		"mov %1,%%eax\n"		/* This set is 28 bytes */
		"bswap %%eax;" 
	    "mov %%eax,%0;" :"=m" (data) :"m" (data) 
		);
#else
			__asm
			{ 
				mov eax,data;
				bswap eax; 
				mov data,eax;
			}
#endif
			reg_update_table[reg_update_rec_num].data=data;
			reg_update_table[reg_update_rec_num].reg=p_insn_exec_info->reg;
			reg_update_rec_num++;
		}
		break;
	case hlt:
		if(__PE__==1&&__CPL__!=0) GP(0)

		halt=true;
		break;
	case lgdt:
		assert(p_insn_exec_info->op_presence==OP_PRESENCE_RM);
		lgdt_(p_insn_exec_info->r_m.operand_desc.mem,p_insn->data_size,p_insn->addr_size);
		break;
	case lidt:
		assert(p_insn_exec_info->op_presence==OP_PRESENCE_RM);
		lidt_(p_insn_exec_info->r_m.operand_desc.mem,p_insn->data_size,p_insn->addr_size);
		break;
	case sgdt:
		assert(p_insn_exec_info->op_presence==OP_PRESENCE_RM);
		sgdt_(p_insn_exec_info->r_m.operand_desc.mem,p_insn->data_size,p_insn->addr_size);
		break;
	case sidt:
		assert(p_insn_exec_info->op_presence==OP_PRESENCE_RM);
		sidt_(p_insn_exec_info->r_m.operand_desc.mem,p_insn->data_size,p_insn->addr_size);
		break;
	case  lldt:
		assert(p_insn_exec_info->op_presence&OP_PRESENCE_RM);
		if((p_insn_exec_info->op_presence&(OP_PRESENCE_RM_R))==(OP_PRESENCE_RM))
		{
			assert(p_insn->data_size==0x10);
			if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data1,0x10,p_insn->addr_size)) 
				return;
		}
		else
		{
			assert(p_insn_exec_info->r_m.operand_desc.reg.reg_cat==RC_REGG16);
			data=reg_file.read_reg(p_insn_exec_info->r_m.operand_desc.reg);
		}

		load_ldt(MK_SELECTOR(data));
		break;

	case mov:
		if(p_insn_exec_info->op_presence==(OP_PRESENCE_REG|OP_PRESENCE_RM_R))
		{
			if(p_insn_exec_info->flag&EXEC_F_DIR)
			{
				set_special_reg(p_insn_exec_info->r_m.operand_desc.reg,reg_file.read_reg(p_insn_exec_info->reg));

			}
			else
			{
				reg_update_table[reg_update_rec_num].data=get_special_reg(p_insn_exec_info->r_m.operand_desc.reg);
				reg_update_table[reg_update_rec_num].reg=p_insn_exec_info->reg;
				reg_update_rec_num++;
			}

		}else bug();
		break;
	case rdmsr:
		rdmsr_();
		break;
	case wrmsr:
		wrmsr_();
		break;
	case cpuid:
		cpuid_();
		break;
	case wbinvd:
		break;
	case cbw:
		if(p_insn_exec_info->flag&EXEC_F_WORD)
			 cdq_(p_insn->data_size);
		else cbw_(p_insn->data_size);
		break;
	case lds:
	case les:
	case lfs:
	case lgs:
	case lss:
		//
		//GvMs
		//
		assert(p_insn_exec_info->op_presence==(OP_PRESENCE_REG|OP_PRESENCE_RM));
		gen_mem_addr(p_insn_exec_info->r_m.operand_desc.mem,p_insn->addr_size,addr_hi,addr_lo);

		if( !read_mem_raw(p_insn_exec_info->r_m.operand_desc.mem.seg,
			addr_hi,addr_lo,off,p_insn->data_size,p_insn->addr_size)) return;

		update_reg(p_insn_exec_info->reg,off );

		gen_mem_addr_raw(addr_lo,p_insn->data_size>>3,p_insn->addr_size,addr_hi,addr_lo);
		if( !read_mem_raw(p_insn_exec_info->r_m.operand_desc.mem.seg,
			addr_hi,addr_lo,data,16,p_insn->addr_size)) return;

		if(p_insn->p_instr_desc->index==lds)set_sreg(DS,(unsigned short)data);
		else 	if(p_insn->p_instr_desc->index==les)set_sreg(ES,(unsigned short)data);
		else 	if(p_insn->p_instr_desc->index==lfs)set_sreg(FS,(unsigned short)data);
		else 	if(p_insn->p_instr_desc->index==lgs)set_sreg(GS,(unsigned short)data);
		/*if(p_insn->p_instr_desc->index==lss)*/
		else set_sreg(SS,(unsigned short)data);
		break;

	case aas:
		aas_();
		break;
	case aam:
		assert(p_insn_exec_info->op_presence==OP_PRESENCE_IMM);
		aam_((unsigned char)p_insn_exec_info->imm);
		break;
	case aad:
		assert(p_insn_exec_info->op_presence==OP_PRESENCE_IMM);
		aad_((unsigned char)p_insn_exec_info->imm);
		break;
	case RDTSC:
		rdtsc_();
		break;

	case RDPMC:
		rdpmc_();
		break;
	case iret:
		iret_(p_insn->data_size);
		break;

	case shrd:// operands: r/m,r, imm/cl
		assert((p_insn_exec_info->op_presence&(OP_PRESENCE_REG|OP_PRESENCE_RM))==(OP_PRESENCE_REG|OP_PRESENCE_RM));
		// cl
		if((p_insn_exec_info->op_presence&(OP_PRESENCE_REG_SEC|OP_PRESENCE_IMM))==OP_PRESENCE_REG_SEC)
			n=reg_file.read_reg(p_insn_exec_info->secondary_reg.reg);
		else
		{
			assert((p_insn_exec_info->op_presence&(OP_PRESENCE_REG_SEC|OP_PRESENCE_IMM))==
			OP_PRESENCE_IMM);
			n=p_insn_exec_info->imm;
		}

		if((p_insn_exec_info->op_presence&OP_PRESENCE_RM_R)==OP_PRESENCE_RM)
		{
			if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,p_insn->data_size,p_insn->addr_size)) 
				break;
		}
		else
		data=reg_file.read_reg(p_insn_exec_info->r_m.operand_desc.reg);

		data1=reg_file.read_reg(p_insn_exec_info->reg);

		cflag=__CFLAGS__;		
		result=alu_shrd(cflag,data,data1,n,p_insn->data_size);

		if((p_insn_exec_info->op_presence&OP_PRESENCE_RM_R)==OP_PRESENCE_RM)
		{
			if( !write_mem(p_insn_exec_info->r_m.operand_desc.mem,result,p_insn->data_size,p_insn->addr_size)) 
				break;
		}
		else
			update_reg(p_insn_exec_info->r_m.operand_desc.reg,result);

		//update_reg(p_insn_exec_info->reg,data1);

		__CFLAGS__=cflag;
		break;
	case shld:
		assert((p_insn_exec_info->op_presence&(OP_PRESENCE_REG|OP_PRESENCE_RM))==(OP_PRESENCE_REG|OP_PRESENCE_RM));
		// cl
		if((p_insn_exec_info->op_presence&(OP_PRESENCE_REG_SEC|OP_PRESENCE_IMM))==OP_PRESENCE_REG_SEC)
			n=reg_file.read_reg(p_insn_exec_info->secondary_reg.reg);
		else
		{
			assert((p_insn_exec_info->op_presence&(OP_PRESENCE_REG_SEC|OP_PRESENCE_IMM))==
			OP_PRESENCE_IMM);
			n=p_insn_exec_info->imm;
		}

		if((p_insn_exec_info->op_presence&OP_PRESENCE_RM_R)==OP_PRESENCE_RM)
		{
			if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,p_insn->data_size,p_insn->addr_size)) 
				break;
		}
		else
		data=reg_file.read_reg(p_insn_exec_info->r_m.operand_desc.reg);

		data1=reg_file.read_reg(p_insn_exec_info->reg);

		cflag=__CFLAGS__;		
		result=alu_shld(cflag,data,data1,n,p_insn->data_size);

		if((p_insn_exec_info->op_presence&OP_PRESENCE_RM_R)==OP_PRESENCE_RM)
		{
			if( !write_mem(p_insn_exec_info->r_m.operand_desc.mem,result,p_insn->data_size,p_insn->addr_size)) 
				break;
		}
		else
			update_reg(p_insn_exec_info->r_m.operand_desc.reg,result);

		//update_reg(p_insn_exec_info->reg,data1);
		__CFLAGS__=cflag;
		break;
	case lmsw:
		
		if((p_insn_exec_info->op_presence&OP_PRESENCE_RM_R)==OP_PRESENCE_RM)
		{
			assert(p_insn->data_size==16);
			if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,p_insn->data_size,p_insn->addr_size)) 
				break;
		}
		else
		{
			assert(p_insn_exec_info->r_m.operand_desc.reg.reg_cat==RC_REGG16);
			data=reg_file.read_reg(p_insn_exec_info->r_m.operand_desc.reg);
		}

		lmsw_((unsigned short)data,p_insn->data_size);
		break;
	case smsw:
		//This instruction is provided for compatibility with the 80286; 
		//80386 programs should use MOV ..., CR0. 
		assert(p_insn_exec_info->type.rm_present);
		if((p_insn_exec_info->op_presence&OP_PRESENCE_RM_R)==OP_PRESENCE_RM)
		{
			//SMSW m16 operand size 32, store CR0[15:0] in m16 (not m32)
			if( !write_mem(p_insn_exec_info->r_m.operand_desc.mem,cr[0],0x10,p_insn->addr_size)) 
				break;
		}
		else
		{
			//SMSW r16 operand size 16, store CR0[15:0] in r16
			//SMSW r32 operand size 32, zero-extend CR0[31:0], and store in r32
			update_reg(p_insn_exec_info->r_m.operand_desc.reg,cr[0]);
		}
		break;
	case daa:
		daa_();
		break;
	case das:
		das_();
		break;
	case rsm:
		if(smm)
		leave_smm();
		else UD_(0);
		break;
	case clts:
		__CR0__.ts=0;
		break;
	case lar:
		assert((p_insn_exec_info->op_presence&(OP_PRESENCE_REG|OP_PRESENCE_RM))==(OP_PRESENCE_REG|OP_PRESENCE_RM));
		if((p_insn_exec_info->op_presence&OP_PRESENCE_RM_R)==OP_PRESENCE_RM)
		{
			// regardless of the src/dest size, only 16 bit is used
			if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,16,p_insn->addr_size)) 
				break;
		}
		else
		{
			data=reg_file.read_reg(p_insn_exec_info->r_m.operand_desc.reg);
		}

		result=lar_(MK_SELECTOR(data),p_insn->data_size);
		if(__ZF__)
		{
			update_reg(p_insn_exec_info->reg,result);
		}
		break;
	case nop:
		break;
	case ltr:
		if((p_insn_exec_info->op_presence&OP_PRESENCE_RM_R)==OP_PRESENCE_RM)
		{
			assert(p_insn->data_size==16);
			if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,p_insn->data_size,p_insn->addr_size)) 
				break;
		}
		else
		{
			assert(p_insn_exec_info->r_m.operand_desc.reg.reg_cat==RC_REGG16);
			data=reg_file.read_reg(p_insn_exec_info->r_m.operand_desc.reg);
		}

		load_tr(MK_SELECTOR(data));

		break;
	case ud2:

		UD_(0);//used in BUG() in linux kernel
		//::step=true;
		break;
	case mfence:
	case lfence:
	case sfence:
		break;
	case xadd:
		// rm,r:
		//
		assert((p_insn_exec_info->op_presence&(OP_PRESENCE_REG|OP_PRESENCE_RM))==(OP_PRESENCE_REG|OP_PRESENCE_RM));

		n=(p_insn_exec_info->flag&EXEC_F_WORD)?p_insn->data_size:8;

		if((p_insn_exec_info->op_presence&OP_PRESENCE_RM_R)==OP_PRESENCE_RM)
		{
			if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,data,n,p_insn->addr_size)) 
				break;
		}
		else
		data=reg_file.read_reg(p_insn_exec_info->r_m.operand_desc.reg);

		data1=reg_file.read_reg(p_insn_exec_info->reg);

		cflag=__CFLAGS__;		
		result=alu_add(cflag,data,data1,p_insn->data_size);

		if((p_insn_exec_info->op_presence&OP_PRESENCE_RM_R)==OP_PRESENCE_RM)
		{
			if( !write_mem(p_insn_exec_info->r_m.operand_desc.mem,result,n,p_insn->addr_size)) 
				break;
		}
		else
			update_reg(p_insn_exec_info->r_m.operand_desc.reg,result);

		update_reg(p_insn_exec_info->reg,data);
		__CFLAGS__=cflag;
		break;
	case invlpg:
		gen_mem_addr(p_insn_exec_info->r_m.operand_desc.mem,p_insn->addr_size,addr_hi,addr_lo);
		mmu.invalid_tlb(addr_lo+BASE(descriptors[p_insn_exec_info->r_m.operand_desc.mem.seg]));
		break;

	case  cmpxchg:

		n=(p_insn_exec_info->flag&EXEC_F_WORD)?p_insn->data_size:8;

		cmpxchg_(p_insn_exec_info->r_m,reg_file.read_reg(p_insn_exec_info->reg),n,p_insn->addr_size);
		break;
	case str:
		data=___TR___;

l_sxx:
		if((p_insn_exec_info->op_presence&OP_PRESENCE_RM_R)==OP_PRESENCE_RM)
		{
			assert(p_insn->data_size==16);
			if( !write_mem(p_insn_exec_info->r_m.operand_desc.mem,data,16,p_insn->addr_size)) 
				break;
		}
		else
		{
			assert(p_insn_exec_info->r_m.operand_desc.reg.reg_cat==RC_REGG16);
			update_reg(p_insn_exec_info->r_m.operand_desc.reg,data);
		}
		break;
	case sldt:
		data=___LDT___;
		goto l_sxx;
		break;
	case int3:
		::step=true;
		int3_(p_insn->data_size);
		break;
	case cmpxchg8b:
		cmpxchg8b_(p_insn_exec_info->r_m.operand_desc.mem,p_insn->addr_size);
		break;
	default:

		dump_stack();
		UD_(0);
		::step=true;
		break;
	}
}



//
//
//
//Instruction      Description      Useful to     Protected from
//                                 Application?   Application?
//LLDT      Load LDT Register        No         Yes
//SLDT      Store LDT Register       No         No
//LGDT      Load GDT Register        No         Yes
//SGDT      Store GDT Register       No         No
//LTR       Load Task Register       No         Yes
//STR       Store Task Register      No         No
//LIDT      Load IDT Register        No         Yes
//SIDT      Store IDT Register       No         No
//
//SMSW      Store MSW                Yes        No
//LMSW      Load MSW                 No         Yes
//CLTS      Clear TS flag in CR0     No         Yes
//ARPL      Adjust RPL              Yes         No
//LAR       Load Access Rights      Yes         No
//LSL       Load Segment Limit      Yes         No
//VERR      Verify for Reading      Yes         No
//VERW      Verify for Writing      Yes         No
//MOV DRn   Load and store debug registers
//                                   No          Yes
//MOV CRn   Load and store control registers
//                                   No         Yes
//INVD      Invalidate cache, no writeback
//                                   No          Yes
//WBINVD    Invalidate cache, with writeback
//                                   No          Yes
//INVLPG    Invalidate TLB entry     No          Yes
//HLT       Halt Processor           No          Yes
//LOCK      Bus Lock                 Yes         No
//RSM       Return from system management mode
//                                        No     Yes
//RDMSR    Read Model-Specific Registers  No     Yes
//WRMSR    Write Model-Specific Registers No     Yes
//RDPMC    Read Performance-Monitoring Counter
//                                     Yes       Yes
//RDTSC    Read Time-Stamp Counter    Yes        Yes
//RDTSCP   Read Serialized Time-Stamp Counter  
//                                     Yes        Yes
//
//XGETBV  Return the state of the the 
//        XFEATURE_ENABLED_MASK register  Yes     No
//XSETBV  Enable one or more processor 
//         extended states                  No    Yes