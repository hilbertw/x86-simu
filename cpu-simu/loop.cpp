
#include <stdio.h>
#include "cpu.h"

void CCPU:: loop_z(int rel, int OperandSize, int AddressSize)
{
	unsigned long CountReg;
	if( AddressSize == 16)
	{
		//CountReg is CX ;CountReg := CountReg - 1;
		CountReg=__CX__=__CX__-1;
	}
	else
	{
		//ELSE CountReg is ECX
		CountReg=__ECX__=__ECX__-1;
	}

   // BranchCond := (ZF = 1) AND (CountReg <> 0);
	if(CountReg!=0 && __ZF__==1)
		near_rel_jump(rel,OperandSize);
}
void CCPU:: loop_nz(int rel, int OperandSize, int AddressSize)
{
	unsigned long CountReg;
	if( AddressSize == 16)
	{
		//CountReg is CX ;CountReg := CountReg - 1;
		CountReg=__CX__=__CX__-1;
	}
	else
	{
		//ELSE CountReg is ECX
		CountReg=__ECX__=__ECX__-1;
	}

   //  BranchCond := (ZF = 0) AND (CountReg <> 0);
	if(CountReg!=0 && __ZF__==0)
		near_rel_jump(rel,OperandSize);
}

void CCPU:: loop_(int rel, int OperandSize, int AddressSize)
{
	unsigned long CountReg;
	if( AddressSize == 16)
	{
		//CountReg is CX ;CountReg := CountReg - 1;
		CountReg=__CX__=__CX__-1;
	}
	else
	{
		//ELSE CountReg is ECX
		CountReg=__ECX__=__ECX__-1;
	}

   //  (CountReg <> 0);
	if(CountReg!=0 )
		near_rel_jump(rel,OperandSize);
}



void CCPU:: jcxz_(int rel, int OperandSize, int AddressSize)
{
	unsigned long CountReg;
	if( AddressSize == 16)
	{
		//CountReg is CX ;CountReg := CountReg - 1;
		CountReg=__CX__;
	}
	else
	{
		//ELSE CountReg is ECX
		CountReg=__ECX__;
	}

   //  (CountReg == 0);
	if(CountReg==0 )
		near_rel_jump(rel,OperandSize);
}