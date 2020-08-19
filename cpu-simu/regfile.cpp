#define _CRT_SECURE_NO_DEPRECATE 1
#include <assert.h>
#include "stdio.h"
#include "regfile.h"

unsigned char CREGFILE::get_reg8(int regno)
{
	assert (regno<8);

	return((unsigned char*)&reg32[regno%4])[regno/4];
}	
unsigned short CREGFILE::get_reg16(int regno)
{
	assert (regno<8);

	return *(unsigned short*)&reg32[regno];
}
unsigned int CREGFILE::get_reg32(int regno)
{
	assert (regno<8);

	return reg32[regno];
}
void CREGFILE::set_reg8(int regno,unsigned char data)
{
	assert (regno<8);

	((unsigned char*)&reg32[regno%4])[regno/4]=data;
}		
void CREGFILE::set_reg16(int regno,unsigned short data)
{
	assert (regno<8);

	*(unsigned short*)&reg32[regno]=data;
}
void CREGFILE::set_reg32(int regno,unsigned int data)
{
	assert (regno<8);
	reg32[regno]=data;
}


unsigned int CREGFILE::read_reg(REG_CODE reg)
{
	unsigned int v=0;
	switch(reg.reg_cat)
	{
	case RC_REGG8: v=get_reg8(reg.reg_no);break;
	case RC_REGG16: v=get_reg16(reg.reg_no);break;
	case RC_REGG32: v=get_reg32(reg.reg_no);break;
	case RC_REGSEG: v=sreg[reg.reg_no];break;
	}
	return v;
}
void CREGFILE::write_reg(REG_CODE reg,unsigned int data)
{
	switch(reg.reg_cat)
	{
	case RC_REGG8: set_reg8(reg.reg_no,data);break;
	case RC_REGG16: set_reg16(reg.reg_no,data);break;
	case RC_REGG32: set_reg32(reg.reg_no,data);break;
	case RC_REGSEG: sreg[reg.reg_no]=data;break;
	}
}


void CREGFILE::print(char buffer[], bool mode)
{
	int pos;
	if(mode)
	{
		pos=sprintf(buffer,"AX=%04X ",(unsigned short)reg32[0]);
		pos+=sprintf(buffer+pos,"BX=%04X ",(unsigned short)reg32[3]);
		pos+=sprintf(buffer+pos,"CX=%04X ",(unsigned short)reg32[1]);
		pos+=sprintf(buffer+pos,"DX=%04X ",(unsigned short)reg32[2]);
		pos+=sprintf(buffer+pos,"SP=%04X ",(unsigned short)reg32[4]);
		pos+=sprintf(buffer+pos,"BP=%04X ",(unsigned short)reg32[5]);
		pos+=sprintf(buffer+pos,"SI=%04X ",(unsigned short)reg32[6]);
		pos+=sprintf(buffer+pos,"DI=%04X ",(unsigned short)reg32[7]);
		pos+=sprintf(buffer+pos,"IP=%04X\n",(unsigned short)eip);
	}
	else
	{
		pos=sprintf(buffer,"EAX=%08X ",reg32[0]);
		pos+=sprintf(buffer+pos,"EBX=%08X ",reg32[3]);
		pos+=sprintf(buffer+pos,"ECX=%08X ",reg32[1]);
		pos+=sprintf(buffer+pos,"EDX=%08X ",reg32[2]);
		pos+=sprintf(buffer+pos,"ESP=%08X ",reg32[4]);
		pos+=sprintf(buffer+pos,"EBP=%08X ",reg32[5]);
		pos+=sprintf(buffer+pos,"ESI=%08X ",reg32[6]);
		pos+=sprintf(buffer+pos,"EDI=%08X ",reg32[7]);
		pos+=sprintf(buffer+pos,"EIP=%08X\n",eip);
	}
	pos+=sprintf(buffer+pos,"ES=%04X ",sreg[0]);
	pos+=sprintf(buffer+pos,"CS=%04X ",sreg[1]);
	pos+=sprintf(buffer+pos,"SS=%04X ",sreg[2]);
	pos+=sprintf(buffer+pos,"DS=%04X ",sreg[3]);
	pos+=sprintf(buffer+pos,"FS=%04X ",sreg[4]);
	pos+=sprintf(buffer+pos,"GS=%04X ",sreg[5]);
	pos+=sprintf(buffer+pos,"EFLAGS=%08X ",eflags);
#define MASK_ZF 0x40
#define MASK_AF 0x10
#define MASK_PF 0x4
#define MASK_OF 0x800
#define MASK_CF 0x1 
#define MASK_SF 0x80
#define MASK_DF 0x400 
#define MASK_IF 0x200
	pos+=sprintf(buffer+pos,"%s ",(eflags& MASK_OF)?"OV":"NV");
	pos+=sprintf(buffer+pos,"%s ",(eflags& MASK_DF)?"DN":"UP");
	pos+=sprintf(buffer+pos,"%s ",(eflags& MASK_IF)?"EI":"DI");
	pos+=sprintf(buffer+pos,"%s ",(eflags& MASK_SF)?"NG":"PL");
	pos+=sprintf(buffer+pos,"%s ",(eflags& MASK_ZF)?"ZR":"NZ");
	pos+=sprintf(buffer+pos,"%s ",(eflags& MASK_AF)?"AC":"NA");
	pos+=sprintf(buffer+pos,"%s ",(eflags& MASK_PF)?"PE":"PO");
	pos+=sprintf(buffer+pos,"%s\n",(eflags& MASK_CF)?"CY":"NC");

}