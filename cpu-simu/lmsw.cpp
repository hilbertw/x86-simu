#include <stdio.h>
#include "cpu.h"
#include "log.h"
void CCPU::lmsw_(unsigned short msw, int OperandSize)
{
	assert(OperandSize==16);
	
	//#GP(0) if the current privilege level is not 0; 
	if(__CPL__!=0) GP(0);

	//LMSW loads the machine status word (part of CR0) from the source operand.
	//cr0[0:3]<-src[0:3]

    cr0_cache=cr[0]&=~0xf;
	cr0_cache|=(msw&0xf);

	if((cr0_cache&1)&&!(cr[0]&1))
	{
		cr0_pending=true;
	}
	else
	{
		cr[0]=cr0_pending;
		cr0_pending=false;
	}
}
