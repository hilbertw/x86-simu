#include <stdio.h>
#include <assert.h>
#include "cpu.h"
void CCPU::cbw_(int OperandSize)
{

	if (OperandSize == 16){ /* instruction = CBW */
	//AX := SignExtend(AL);
		__AX__=(short)(char)__AL__;
	}else { /* OperandSize = 32, instruction = CWDE */
	   //EAX := SignExtend(AX);
		__EAX__=(long)(short)__AX__;
	}
}
void CCPU::cdq_(int OperandSize)
{
	if (OperandSize == 16) /* CWD instruction */
	{
	   if (__AX__ &0x8000) __DX__ = 0xFFFF; 
		else  __DX__ = 0; 

	}else /* OperandSize = 32, CDQ instruction */ 
	{ 
	   if (__EAX__ & 0x80000000)  __EDX__ = 0xffffFFFF; 
		else __EDX__ = 0; 
	}
}

