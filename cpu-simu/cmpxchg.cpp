#include "alucore.h"
#include "cpu.h"
void CCPU::cmpxchg8b_(MEM_DESC &m_info,int addr_size)
{
	union
	{
		char m64[8];
		struct qword qw;
	};
	//
	// load m64 to tmp
	//
	if(!read_string(m_info,m64,8,addr_size))
	{
		return; // excption has been set
	}
	//
	// compare edx:eax with m64
	//
	if(__EDX__==qw.hi
		&&__EAX__==qw.lo)

	//
	// if equal, set zf, write ecx:ebx to m64
	//
	{
		__ZF__=1;
		qw.hi=__ECX__;
		qw.lo=__EBX__;

		write_string(m_info,m64,8,addr_size);
	}


	//
	// else, clear zf,  load m64 to edx:eax 
	//
	else
	{
		REG_CODE r1;
		__ZF__=0;

		r1.reg_cat=RC_REGG32;
		r1.reg_no=EDX;
		update_reg(r1,qw.hi);
		r1.reg_no=EAX;
		update_reg(r1,qw.lo);
	}
}

void CCPU::cmpxchg_(OPERAND_R_M &r_m,unsigned long  reg,int data_size,int addr_size)
{
	unsigned long dd;

	//
	// load r/m to dd
	//
	if(r_m.type==2)
	{
		if( !read_mem(r_m.operand_desc.mem,dd,data_size,addr_size)) 
			return;
	}
	else
	{
		dd=reg_file.read_reg(r_m.operand_desc.reg);
	}
	//
	// compare eax with r/m
	//
	unsigned short cflag=__CFLAGS__;
	alu_sub(cflag,__EAX__,dd,data_size);

	//
	// if equal, set zf, write ecx:ebx to m64
	//
	if ((cflag&0x40)!=0)
	{
		if(r_m.type==2)
		{
			if( !write_mem(r_m.operand_desc.mem,reg,data_size,addr_size)) 
				return;
		}
		else
		{
			update_reg(r_m.operand_desc.reg,reg);
		}
	}


	//
	// else, clear zf,  load r/m to:eax 
	//
	else
	{
		REG_CODE r1;

		r1.reg_cat=(data_size==8)?RC_REGG8:(data_size==16)?RC_REGG16:RC_REGG32;
		r1.reg_no=EAX;
		update_reg(r1,dd);
	}
	__CFLAGS__=cflag;
}
