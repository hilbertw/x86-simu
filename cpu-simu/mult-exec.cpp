#include <stdio.h>
#include "cpu.h"
extern struct reg_update_rec reg_update_table[10];
extern int reg_update_rec_num;

void udiv32(unsigned long divident_hi,unsigned long divident_lo,unsigned long mdivisor,
			 unsigned long& Remainder,unsigned long& Quotient   );
void idiv32(unsigned long divident_hi,unsigned long divident_lo,unsigned long mdivisor,
			 unsigned long& Remainder,unsigned long& Quotient   );
void umult32(unsigned long muliplicant,unsigned long muliplier,
			 unsigned long& prod_hi,unsigned long& prod_lo);
void imult32(unsigned long muliplicant,unsigned long muliplier,
			 unsigned long& prod_hi,unsigned long& prod_lo);

void CCPU:: mult_insn_exec(PINSN_BASIC_INFO p_insn,PINSN_EXEC_INFO p_insn_exec_info)
{
	unsigned long op1,op2,q,r,p_hi,p_lo,p;
	REG_CODE r1,r2;
	int data_size,imm_size;
	switch(p_insn->p_instr_desc->index)
	{
	case imul3://IMUL r,r/m,imm              

		assert((p_insn_exec_info->op_presence&(OP_PRESENCE_RM|OP_PRESENCE_REG|OP_PRESENCE_IMM))
			==(OP_PRESENCE_RM|OP_PRESENCE_REG|OP_PRESENCE_IMM));

		if((p_insn_exec_info->op_presence&OP_PRESENCE_RM_R)==OP_PRESENCE_RM)
		{
			// always word
			op2=0;
			if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,op2,p_insn->data_size,p_insn->addr_size)) 
				return;
		}
		else
		{
			op2=reg_file.read_reg(p_insn_exec_info->r_m.operand_desc.reg);
		}
		//
		//  imm present,sign-extended immediate byte
		//

		imm_size=(p_insn_exec_info->flag&EXEC_F_WORD)?p_insn->data_size:8;

		if(imm_size==8) op1=(int)(char)p_insn_exec_info->imm;
		else if(imm_size==16) op1=(int)(short)p_insn_exec_info->imm;
		else op1=p_insn_exec_info->imm;

		if(p_insn->data_size==32)
			imult32(op1,op2,p_hi,p_lo);
		else
			p_lo=(short)op1*(short)op2;

		//
		// result in reg, there is no byte op, so product is always, 32bits
		//
		update_reg(p_insn_exec_info->reg,p_lo);

		break;
	case imul2://IMUL r,r/m              
		assert((p_insn_exec_info->op_presence&(OP_PRESENCE_RM|OP_PRESENCE_REG))
			==(OP_PRESENCE_RM|OP_PRESENCE_REG));

		if((p_insn_exec_info->op_presence&OP_PRESENCE_RM_R)==OP_PRESENCE_RM)
		{
			// always word
			op2=0;
			if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,op2,p_insn->data_size,p_insn->addr_size)) 
				return;
		}
		else
		{
			op2=reg_file.read_reg(p_insn_exec_info->r_m.operand_desc.reg);
		}
		//
		// op1 is reg
		//
		op1=reg_file.read_reg(p_insn_exec_info->reg);

		if(p_insn->data_size==32)
			imult32(op1,op2,p_hi,p_lo);
		else
			p_lo=(short)op1*(short)op2;

		//
		// result in reg, there is no 8bit op,
		//
		update_reg(p_insn_exec_info->reg,p_lo);

		break;
	case imul1://IMUL r/m
		assert((p_insn_exec_info->op_presence&OP_PRESENCE_RM)
			==OP_PRESENCE_RM);

		data_size=(p_insn_exec_info->flag&EXEC_F_WORD)?p_insn->data_size:8;
		if((p_insn_exec_info->op_presence&OP_PRESENCE_RM_R)==OP_PRESENCE_RM)
		{
			// always word
			op2=0;
			if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,op2,p_insn->data_size,p_insn->addr_size)) 
				return;
		}
		else
		{
			op2=reg_file.read_reg(p_insn_exec_info->r_m.operand_desc.reg);
		}
		// mul r/m8
		r1.reg_no=AL;
		r2.reg_no=DL;
		switch(data_size)
		{
		case 8:
			r1.reg_cat=RC_REGG16;
			p_lo=(char)__AL__*(char)op2;
			update_reg(r1,p_lo);
			break;
		case 16:
			r2.reg_cat=r1.reg_cat=RC_REGG16;
			p=(short)__AX__*(short)op2;
			update_reg(r1,(short)p);
			update_reg(r2,p>>16);
			break;
		case 32:
			r2.reg_cat=r1.reg_cat=RC_REGG32;
			op1=(long)__EAX__;
			imult32(op1,op2,p_hi,p_lo);
			update_reg(r1,p_lo);
			update_reg(r2,p_hi);
		break;
		}

		//The CF and OF flags are set when significant bits are carried into the upper half of the result. 
		//The CF and OF flags are cleared when the result fits exactly in the lower half of the result.

		break;
	case mul:
		data_size=(p_insn_exec_info->flag&EXEC_F_WORD)?p_insn->data_size:8;

		assert((p_insn_exec_info->op_presence&OP_PRESENCE_RM)==OP_PRESENCE_RM);

		if((p_insn_exec_info->op_presence&OP_PRESENCE_RM_R)==OP_PRESENCE_RM)
		{
			op2=0;
			if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,op2,data_size,p_insn->addr_size)) 
				return;
		}
		else
		{
			op2=reg_file.read_reg(p_insn_exec_info->r_m.operand_desc.reg);
		}
		// mul r/m8
		r1.reg_no=AL;
		r2.reg_no=DL;
		switch(data_size)
		{
		case 8:
			r1.reg_cat=RC_REGG16;
			p_lo=(unsigned char)__AL__*(unsigned char)op2;
			update_reg(r1,p_lo);
			break;
		case 16:
			r2.reg_cat=r1.reg_cat=RC_REGG16;
			p=(unsigned short)__AX__*(unsigned short)op2;
			update_reg(r1,(unsigned short)p);
			update_reg(r2,p>>16);
			break;
		case 32:
			r2.reg_cat=r1.reg_cat=RC_REGG32;
			op1=(unsigned long)__EAX__;
			umult32(op1,op2,p_hi,p_lo);
			update_reg(r1,p_lo);
			update_reg(r2,p_hi);
		break;
		}
		break;
	case idiv:
		assert((p_insn_exec_info->op_presence&OP_PRESENCE_RM)==OP_PRESENCE_RM);
		data_size=(p_insn_exec_info->flag&EXEC_F_WORD)?p_insn->data_size:8;
		if((p_insn_exec_info->op_presence&OP_PRESENCE_RM_R)==OP_PRESENCE_RM)
		{
			op2=0;
			if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,op2,data_size,p_insn->addr_size)) 
				return;
		}
		else
		{
			op2=reg_file.read_reg(p_insn_exec_info->r_m.operand_desc.reg);
		}
		r1.reg_cat=RC_REGG8;
		r2.reg_cat=RC_REGG8;
		switch(data_size)
		{
		case 8: 
			op1=( short)__AX__;
	/*		___AL___=op1/(char)op2;
			___AH___=op1%(char)op2;*/
			r1.reg_no=AL;
			r2.reg_no=AH;
			r1.reg_cat=RC_REGG8;
			r2.reg_cat=RC_REGG8;
			update_reg(r1,(short)op1/( char)op2);
			update_reg(r2,(short)op1%( char)op2);
			break;
		case 16: 
			op1=(unsigned short)__AX__|(((unsigned short)__DX__)<<16);
			//__AX__=( int)op1/(  short)op2;
			//__DX__=( int)op1%(  short)op2;
			r1.reg_no=AX;
			r2.reg_no=DX;
			r1.reg_cat=RC_REGG16;
			r2.reg_cat=RC_REGG16;
			update_reg(r1,( int)op1/( char)op2);
			update_reg(r2,( int)op1%( char)op2);

			break;
		case 32: 
			idiv32(__EDX__,__EAX__,op2,r,q);
			//__EDX__=r;
			//__EAX__=q;
			r1.reg_no=EAX;
			r2.reg_no=EDX;
			r1.reg_cat=RC_REGG32;
			r2.reg_cat=RC_REGG32;
			update_reg(r1,q);
			update_reg(r2,r);
			break;
		default:
			bug();
		}
		break;
	case udiv:
		assert((p_insn_exec_info->op_presence&OP_PRESENCE_RM)==OP_PRESENCE_RM);

		data_size=(p_insn_exec_info->flag&EXEC_F_WORD)?p_insn->data_size:8;
		if((p_insn_exec_info->op_presence&OP_PRESENCE_RM_R)==OP_PRESENCE_RM)
		{
			op2=0;
			if( !read_mem(p_insn_exec_info->r_m.operand_desc.mem,op2,data_size,p_insn->addr_size)) 
				return;
		}
		else
		{
			op2=reg_file.read_reg(p_insn_exec_info->r_m.operand_desc.reg);
		}
		if(op2==0)// divded by 0
		{
			DE_(0);
		}
		else
		switch(data_size)
		{
		case 8: 
			op1=(unsigned short)__AX__;
			//___AL___=(unsigned short)op1/(unsigned char)op2;
			//___AH___=(unsigned short)op1%(unsigned char)op2;
			r1.reg_no=AL;
			r2.reg_no=AH;
			r1.reg_cat=RC_REGG8;
			r2.reg_cat=RC_REGG8;
			update_reg(r1,op1/(unsigned char)op2);
			update_reg(r2,op1%(unsigned char)op2);
			break;
		case 16: 
			op1=(unsigned short)__AX__|(((unsigned short)__DX__)<<16);
			//__AX__=op1/op2;
			//__DX__=op1%op2;
			r1.reg_no=AX;
			r2.reg_no=DX;
			r1.reg_cat=RC_REGG16;
			r2.reg_cat=RC_REGG16;
			update_reg(r1,(unsigned int)op1/(unsigned short)op2);
			update_reg(r2,(unsigned int)op1%(unsigned short)op2);
			break;
		case 32: 
			udiv32(__EDX__,__EAX__,op2,r,q);
			//__EDX__=r;
			//__EAX__=q;
			r1.reg_no=EAX;
			r2.reg_no=EDX;
			r1.reg_cat=RC_REGG32;
			r2.reg_cat=RC_REGG32;
			update_reg(r1,q);
			update_reg(r2,r);
			break;
		default:
			bug();
		}
		break;
	default:
		bug();
	}
}
void udiv32(unsigned long divident_hi,unsigned long divident_lo,unsigned long divisor,
			 unsigned long& Remainder,unsigned long& Quotient   )
{
	unsigned long r,q;

#ifdef __GNUC__

	__asm__ __volatile__ (
		"mov %2,%%edx;"
		"mov %3,%%eax;"
		"div %4;"
		"mov %%edx,%0;"
		"mov %%eax,%1;":"=m"(r) ,"=m"(q): "m" (divident_hi),"m"(divident_lo) ,"r"(divisor)  
		);
#else
	__asm 
	{
		mov edx,divident_hi;
		mov eax,divident_lo;
		div divisor;
		mov r,edx;
		mov q,eax;
	}
#endif

	Remainder=r;
	Quotient=q;

}
void idiv32(unsigned long divident_hi,unsigned long divident_lo,unsigned long divisor,
			 unsigned long& Remainder,unsigned long& Quotient   )
 {
	unsigned long r,q;
#ifdef __GNUC__

	__asm__ __volatile__ (
		"mov %2,%%edx;"
		"mov %3,%%eax;"
		"idiv %4;"
		"mov %%edx,%0;"
		"mov %%eax,%1;":"=m"(r) ,"=m"(q): "m" (divident_hi),"m"(divident_lo) ,"r"(divisor) 
		);
#else
	__asm 
	{
		mov edx,divident_hi;
		mov eax,divident_lo;
		idiv divisor;
		mov r,edx;
		mov q,eax;
	}
#endif
	Remainder=r;
	Quotient=q;
}


//
//IMUL clears the overflow and carry flags under the following conditions: 
//
//   Instruction Form    Condition for Clearing CF and OF
//   r/m8                AL := sign-extend of AL to 16 bits
//   r/m16               AX := sign-extend of AX to 32 bits
//   r/m32               EDX:EAX := sign-extend of EAX to 32 bits
//   r16,r/m16           Result exactly fits within r16
//   r/32,r/m32          Result exactly fits within r32
//   r16,r/m16,imm16     Result exactly fits within r16
//   r32,r/m32,imm32     Result exactly fits within r32


void umult32(unsigned long muliplicant,unsigned long muliplier,
		 unsigned long& prod_hi,unsigned long& prod_lo)
{
	unsigned long lo,hi;
#ifdef __GNUC__

	__asm__ __volatile__ (
		"mov %2,%%edx;"
		"mov %3,%%eax;"
		"mul %%edx;"
		"mov %%edx,%0;"
		"mov %%eax,%1;":"=m"(hi) ,"=m"(lo): "m" (muliplicant),"m"(muliplier) 
		);
#else
	__asm 
	{
		mov edx,muliplicant;
		mov eax,muliplier;
		mul edx;
		mov hi,edx;
		mov lo,eax;
	}
#endif

	prod_hi=hi;
	prod_lo=lo;
}
void imult32(unsigned long muliplicant,unsigned long muliplier,
		 unsigned long& prod_hi,unsigned long& prod_lo)
{
	unsigned long lo,hi;
#ifdef __GNUC__

	__asm__ __volatile__ (
		"mov %2,%%edx;"
		"mov %3,%%eax;"
		"imul %%edx;"
		"mov %%edx,%0;"
		"mov %%eax,%1;" :"=m"(hi) ,"=m"(lo): "m" (muliplicant),"m"(muliplier)
		);
#else
	__asm 
	{
		mov edx,muliplicant;
		mov eax,muliplier;
		imul edx;
		mov hi,edx;
		mov lo,eax;
	}
#endif
	prod_hi=hi;
	prod_lo=lo;
}