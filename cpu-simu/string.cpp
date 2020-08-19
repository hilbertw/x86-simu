#include "cpu.h"
#include "alucore.h"
#include "log.h"
#pragma warning (disable:4018)


bool  CCPU::movs_ (char override_seg,int OperandSize,int AddressSize) 
{
	assert(OperandSize==16||OperandSize==32||OperandSize==8);
	assert(AddressSize==16||AddressSize==32);
	int data_bytes=OperandSize>>3;

	char seg=(PREFIX_SEG(override_seg)==NO_SEGOVR)?DS:PREFIX_SEG(override_seg);
	unsigned long tmp;
	unsigned addr_lo=(AddressSize==16)?__SI__:__ESI__;


	if(!read_mem_raw(seg,0,addr_lo,tmp, OperandSize,AddressSize))
	return false;

	addr_lo=(AddressSize==16)?__DI__:__EDI__;
	// es can not be overriden
	if(!write_mem_raw(ES,0,addr_lo,tmp, OperandSize,AddressSize))
	return false;

	if (AddressSize==32)
	{ 

		if (__DF__ == 0)
		{
			__ESI__ = __ESI__ + data_bytes;
			__EDI__ = __EDI__ + data_bytes;
		} else {
			__ESI__ = __ESI__ - data_bytes;
			__EDI__ = __EDI__ - data_bytes;
		}
	}
	else if (AddressSize==16)
	{ 

		if (__DF__ == 0)
		{
			__SI__ = __SI__ + data_bytes;
			__DI__ = __DI__ + data_bytes;
		} else {
			__SI__ = __SI__  -data_bytes;
			__DI__ = __DI__  -data_bytes;
		}
	}
	return true;

}


bool  CCPU::stos_ (int OperandSize,int AddressSize) 
{
	assert(OperandSize==16||OperandSize==32||OperandSize==8);
	assert(AddressSize==16||AddressSize==32);
	int data_bytes=OperandSize>>3;

	unsigned long tmp=(OperandSize==16)?__AX__:(OperandSize==32)?__EAX__:__AL__;


	unsigned addr_lo=(AddressSize==16)?__DI__:__EDI__;
	 //The ES segment cannot be overridden with a segment override prefix.
	if(!write_mem_raw(ES,0,addr_lo,tmp, OperandSize,AddressSize))
	return false;

	if (AddressSize==32)
	{ 

		if (__DF__ == 0)
		{
			__EDI__ = __EDI__ + data_bytes;
		} else {
			__EDI__ = __EDI__  -data_bytes;
		}
	}
	else if (AddressSize==16)
	{ 

		if (__DF__ == 0)
		{
			__DI__ = __DI__ + data_bytes;
		} else {
			__DI__ = __DI__  -data_bytes;
		}
	}
	return true;

}


bool  CCPU::lods_ (char override_seg,int OperandSize,int AddressSize) 
{
	assert(OperandSize==16||OperandSize==32||OperandSize==8);
	assert(AddressSize==16||AddressSize==32);
	int data_bytes=OperandSize>>3;

	char seg=(PREFIX_SEG(override_seg)==NO_SEGOVR)?DS:PREFIX_SEG(override_seg);

	unsigned long tmp;
	unsigned addr_lo=(AddressSize==16)?__SI__:__ESI__;

	if(!read_mem_raw(seg,0,addr_lo,tmp, OperandSize,AddressSize))
	return false;

	if (OperandSize==16)__AX__=(unsigned short)tmp; 
	else if (OperandSize==32)__EAX__=tmp;
	else  __AL__=(unsigned char)tmp;

	if (AddressSize==32)
	{ 

		if (__DF__ == 0)
		{
			__ESI__ = __ESI__ + data_bytes;
		} else {
			__ESI__ = __ESI__  -data_bytes;
		}
	}
	else if (AddressSize==16)
	{ 

		if (__DF__ == 0)
		{
			__SI__ = __SI__ + data_bytes;
		} else {
			__SI__ = __SI__  -data_bytes;
		}
	}
	return true;

}
//
//Compares the byte, word, or double word specified with the memory operand with the value in the AL, AX, or EAX register, 
//and sets the status flags in the EFLAGS register according to the results. 
//The memory operand address is read from either the ES:EDI or the ES:DI registers (depending on the address-size 
//attribute of the instruction, 32 or 16, respectively). 
//The ES segment cannot be overridden with a segment override prefix.

bool  CCPU::scas_ (int OperandSize,int AddressSize) 
{
	assert(OperandSize==16||OperandSize==32||OperandSize==8);
	assert(AddressSize==16||AddressSize==32);
	int data_bytes=OperandSize>>3;

	unsigned long tmp;
	unsigned addr_lo=(AddressSize==16)?__DI__:__EDI__;

	//The ES segment cannot be overridden with a segment override prefix
	if(!read_mem_raw(ES,0,addr_lo,tmp, OperandSize,AddressSize))
	return false;

	unsigned long src=(OperandSize==16)?__AX__:(OperandSize==32)?__EAX__:__AL__;

	unsigned short cflags=__CFLAGS__;
	alu_sub(cflags,src,tmp,OperandSize);
	__CFLAGS__=cflags;

	if (AddressSize==32)
	{ 

		if (__DF__ == 0)
		{
			__EDI__ = __EDI__ + data_bytes;
		} else {
			__EDI__ = __EDI__  -data_bytes;
		}
	}
	else if (AddressSize==16)
	{ 

		if (__DF__ == 0)
		{
			__DI__ = __DI__ + data_bytes;
		} else {
			__DI__ = __DI__  -data_bytes;
		}
	}
	return true;

}
//
//For the first operand (SI or ESI), the DS register is used, unless a segment override byte is present. 
//The second operand (DI or EDI) must be addressable from the ES register; no segment override is possible. 
//
bool  CCPU::cmps_ (char override_seg,int OperandSize,int AddressSize) 
{
	assert(OperandSize==16||OperandSize==32||OperandSize==8);
	assert(AddressSize==16||AddressSize==32);
	int data_bytes=OperandSize>>3;

	char seg=(PREFIX_SEG(override_seg)==NO_SEGOVR)?DS:PREFIX_SEG(override_seg);
	unsigned long tmp,tmp1;
	unsigned addr_lo=(AddressSize==16)?__SI__:__ESI__;


	if(!read_mem_raw(seg,0,addr_lo,tmp, OperandSize,AddressSize))
	return false;

	addr_lo=(AddressSize==16)?__DI__:__EDI__;
	if(!read_mem_raw(ES,0,addr_lo,tmp1, OperandSize,AddressSize))
	return false;

	unsigned short cflags=__CFLAGS__;
	alu_sub(cflags,tmp,tmp1,OperandSize);
	__CFLAGS__=cflags;

	if (AddressSize==32)
	{ 

		if (__DF__ == 0)
		{
			__ESI__ = __ESI__ + data_bytes;
			__EDI__ = __EDI__ + data_bytes;
		} else {
			__ESI__ = __ESI__  -data_bytes;
			__EDI__ = __EDI__  -data_bytes;
		}
	}
	else if (AddressSize==16)
	{ 

		if (__DF__ == 0)
		{
			__SI__ = __SI__ + data_bytes;
			__DI__ = __DI__ + data_bytes;
		} else {
			__SI__ = __SI__  -data_bytes;
			__DI__ = __DI__  -data_bytes;
		}
	}
	return true;

}

//1.If the address-size attribute is 16 bits, use CX for the count register; 
//  if the address-size attribute is 32 bits, use ECX for the count register. 
//
//2.Check CX. If it is zero, exit the iteration, and move to the next instruction. 
//
//3.Acknowledge any pending interrupts. 
//
//4.Perform the string operation once. 
//
//5.Decrement CX or ECX by one; no flags are modified. 
//6.Check the zero flag if the string operation is SCAS or CMPS. If the repeat condition does not hold, 
//exit the iteration and move to the next instruction. 
//Exit the iteration if the prefix is REPE and ZF is 0 (the last comparison was not equal),
//or if the prefix is REPNE and ZF is one (the last comparison was equal). 
//
//7.Return to step 1 for the next iteration. 
//

bool CCPU::cmps_r (char override_seg,int OperandSize,int AddressSize)
{

	unsigned int CountReg;
	bool success=true;
	if( AddressSize == 16) CountReg=__CX__ ;
	else /* AddressSize = 32, use ECX for CountReg;*/ 
	CountReg=__ECX__ ;
	//log("rep cmps:%x\n",CountReg);
	//dump_mem(___DS___,__SI__,CountReg);
	//dump_mem(___ES___,__DI__,CountReg);

	while (CountReg !=0)
	{

	   //service pending interrupts (if any);
		if (!cmps_ ( override_seg,OperandSize,AddressSize)){ success = false; break;};
	   CountReg = CountReg - 1;

      //IF (instruction is REP/REPE/REPZ) AND (ZF=0)
      //THEN exit WHILE loop
	   if (__ZF__==0) break;
	}
	if( AddressSize == 16) __CX__ =CountReg;
	else /* AddressSize = 32, use ECX for CountReg;*/ 
	__ECX__ =CountReg;
	return success;
}
bool CCPU::cmps_rnz (char override_seg,int OperandSize,int AddressSize)
{

	unsigned int CountReg;
	bool success=true;

	if( AddressSize == 16) CountReg=__CX__ ;
	else /* AddressSize = 32, use ECX for CountReg;*/ 
	CountReg=__ECX__ ;

	while (CountReg !=0)
	{

	   //service pending interrupts (if any);
	   if (!cmps_ ( override_seg,OperandSize,AddressSize)){ success = false; break;};
	   CountReg = CountReg - 1;

      //IF (instruction is REP/REPE/REPZ) AND (ZF=0)
      //THEN exit WHILE loop
	   if (__ZF__==1) break;
	}
	if( AddressSize == 16) __CX__ =CountReg;
	else /* AddressSize = 32, use ECX for CountReg;*/ 
	__ECX__ =CountReg;
	return success;
}
bool CCPU::lods_r (char override_seg,int OperandSize,int AddressSize)
{

	unsigned int CountReg;
	bool success=true;

	if( AddressSize == 16) CountReg=__CX__ ;
	else /* AddressSize = 32, use ECX for CountReg;*/ 
	CountReg=__ECX__ ;

	while (CountReg !=0)
	{

	   //service pending interrupts (if any);
	   if (!lods_ ( override_seg,OperandSize,AddressSize)){ success = false; break;};
	   CountReg = CountReg - 1;
	}
	if( AddressSize == 16) __CX__ =CountReg;
	else /* AddressSize = 32, use ECX for CountReg;*/ 
	__ECX__ =CountReg;
	return success;

}
bool CCPU::stos_r (int OperandSize,int AddressSize)
{

	unsigned int CountReg;
	bool success=true;

	if( AddressSize == 16) CountReg=__CX__ ;
	else /* AddressSize = 32, use ECX for CountReg;*/ 
	CountReg=__ECX__ ;

	while (CountReg !=0)
	{

	   //service pending interrupts (if any);
	   if (!stos_ ( OperandSize,AddressSize)){ success = false; break;};
	   CountReg = CountReg - 1;
	}

	if( AddressSize == 16) __CX__ =CountReg;
	else /* AddressSize = 32, use ECX for CountReg;*/ 
	__ECX__ =CountReg;
	return success;
}
bool CCPU::scas_r (int OperandSize,int AddressSize)
{

	unsigned int CountReg;
	bool success=true;

	if( AddressSize == 16) CountReg=__CX__ ;
	else /* AddressSize = 32, use ECX for CountReg;*/ 
	CountReg=__ECX__ ;

	while (CountReg !=0)
	{

	   //service pending interrupts (if any);
	   if (!scas_ ( OperandSize,AddressSize)){ success = false; break;};
	   CountReg = CountReg - 1;

      //IF (instruction is REP/REPE/REPZ) AND (ZF=1)
      //THEN exit WHILE loop
	   if (__ZF__==0) break;
	}
	if( AddressSize == 16) __CX__ =CountReg;
	else /* AddressSize = 32, use ECX for CountReg;*/ 
	__ECX__ =CountReg;
	return success;
}
bool CCPU::scas_rnz (int OperandSize,int AddressSize)
{

	unsigned int CountReg;
	bool success=true;

	if( AddressSize == 16) CountReg=__CX__ ;
	else /* AddressSize = 32, use ECX for CountReg;*/ 
	CountReg=__ECX__ ;

	while (CountReg !=0)
	{

	   //service pending interrupts (if any);
	   if (!scas_ ( OperandSize,AddressSize)){ success = false; break;};
	   CountReg = CountReg - 1;

      //IF (instruction is REP/REPE/REPZ) AND (ZF=0)
      //THEN exit WHILE loop
	   if (__ZF__==1) break;
	}
	if( AddressSize == 16) __CX__ =CountReg;
	else /* AddressSize = 32, use ECX for CountReg;*/ 
	__ECX__ =CountReg;
	return success;
}
bool CCPU::movs_r (char override_seg,int OperandSize,int AddressSize)
{

	unsigned int CountReg;
	bool success=true;
	if( AddressSize == 16) CountReg=__CX__ ;
	else /* AddressSize = 32, use ECX for CountReg;*/ 
	CountReg=__ECX__ ;

	while (CountReg !=0)
	{

	   //service pending interrupts (if any);
	   if(!movs_ ( override_seg,OperandSize,AddressSize)){ success = false; break;};
	   CountReg = CountReg - 1;
	}

	if( AddressSize == 16) __CX__ =CountReg;
	else /* AddressSize = 32, use ECX for CountReg;*/ 
	__ECX__ =CountReg;
	return success;
}

//IF AddressSize = 16
//THEN use DI for dest-index;
//ELSE (* AddressSize = 32 *)
//   use EDI for dest-index;
//FI;
//IF (PE = 1) AND ((VM = 1) OR (CPL > IOPL))
//THEN (* Virtual 8086 mode, or protected mode with CPL > IOPL *)
//   IF NOT I-O-Permission (SRC, width(SRC))
//   THEN #GP(0);
//   FI;
//FI;
//IF byte type of instruction
//THEN
//   ES:[dest-index] := [DX]; (* Reads byte at DX from I/O address space *)
//   IF DF = 0 THEN IncDec := 1 ELSE IncDec := -1; FI;
//FI;
//IF OperandSize = 16
//THEN
//   ES:[dest-index] := [DX]; (* Reads word at DX from I/O address space *)
//   IF DF = 0 THEN IncDec := 2 ELSE IncDec := -2; FI;
//FI;
//IF OperandSize = 32
//THEN
//   ES:[dest-index] := [DX]; (* Reads dword at DX from I/O address space *)
//   IF DF = 0 THEN IncDec := 4 ELSE IncDec := -4; FI;
//FI;
//dest-index := dest-index + IncDec;



bool  CCPU::outs_ (char override_seg,int OperandSize,int AddressSize)
{
	unsigned short SRC=__DX__;

	assert(OperandSize==16||OperandSize==32||OperandSize==8);
	assert(AddressSize==16||AddressSize==32);
	int data_bytes=OperandSize>>3;

	char seg=(PREFIX_SEG(override_seg)==NO_SEGOVR)?DS:PREFIX_SEG(override_seg);

	unsigned long tmp;
	unsigned addr_lo=(AddressSize==16)?__SI__:__ESI__;

	if(!read_mem_raw(seg,0,addr_lo,tmp, OperandSize,AddressSize))
	return false;  

	if (((__PE__ == 1) && ((__CPL__ > __IOPL__) || (__VM__ == 1))))
	{ 
		/* Protected mode with __CPL__ > __IOPL__ or virtual-8086 mode */
		//Any I/O Permission Bit for I/O port being accessed == 1
		if (!check_iopermission(SRC,OperandSize))
		/* I/O operation is not allowed */
			GP_(0);
		return false;
	}

	if (OperandSize==8) biu.io_write_byte(SRC, (unsigned char)tmp);
	else if (OperandSize==16) biu.io_write_short(SRC, (unsigned short)tmp);
	else if (OperandSize==32) biu.io_write_int(SRC,tmp);

	if (AddressSize==32)
	{ 

		if (__DF__ == 0)
		{
			__ESI__ = __ESI__ + data_bytes;
		} else {
			__ESI__ = __ESI__  -data_bytes;
		}
	}
	else if (AddressSize==16)
	{ 

		if (__DF__ == 0)
		{
			__SI__ = __SI__ + data_bytes;
		} else {
			__SI__ = __SI__  -data_bytes;
		}
	}
	return true;
}
bool  CCPU::ins_ (int OperandSize ,int AddressSize)
{
	unsigned int  tmp;
	unsigned short SRC=__DX__;

	if (((__PE__ == 1) && ((__CPL__ > __IOPL__) || (__VM__ == 1))))
	{ 
		/* Protected mode with __CPL__ > __IOPL__ or virtual-8086 mode */
		//Any I/O Permission Bit for I/O port being accessed == 1
		if (!check_iopermission(SRC,OperandSize))
		/* I/O operation is not allowed */
			GP_(0);
		return false;
	}
	if (OperandSize==8) tmp=biu.io_read_byte(SRC);
	else if (OperandSize==16) tmp=biu.io_read_short(SRC);
	else if (OperandSize==32) tmp=biu.io_read_int(SRC);

	assert(OperandSize==16||OperandSize==32||OperandSize==8);
	assert(AddressSize==16||AddressSize==32);
	int data_bytes=OperandSize>>3;

	unsigned addr_lo=(AddressSize==16)?__DI__:__EDI__;
	 //The ES segment cannot be overridden with a segment override prefix.
	if(!write_mem_raw(ES,0,addr_lo,tmp, OperandSize,AddressSize))
	return false;

	if (AddressSize==32)
	{ 

		if (__DF__ == 0)
		{
			__EDI__ = __EDI__ + data_bytes;
		} else {
			__EDI__ = __EDI__  -data_bytes;
		}
	}
	else if (AddressSize==16)
	{ 

		if (__DF__ == 0)
		{
			__DI__ = __DI__ + data_bytes;
		} else {
			__DI__ = __DI__  -data_bytes;
		}
	}

	return true;
}
//no segment override is possible. 
bool  CCPU::outs_r (char override_seg,int OperandSize,int AddressSize)
{
	unsigned int CountReg;
	bool success=true;

	if( AddressSize == 16) CountReg=__CX__ ;
	else /* AddressSize = 32, use ECX for CountReg;*/ 
	CountReg=__ECX__ ;

	while (CountReg !=0)
	{

	   //service pending interrupts (if any);
	   if (!outs_ ( override_seg,OperandSize,AddressSize)){ success = false; break;};
	   CountReg = CountReg - 1;
	}
	if( AddressSize == 16) __CX__ =CountReg;
	else /* AddressSize = 32, use ECX for CountReg;*/ 
	__ECX__ =CountReg;
	return success;
}
bool  CCPU::ins_r ( int OperandSize,int AddressSize )
{
	unsigned int CountReg;
	bool success=true;

	if( AddressSize == 16) CountReg=__CX__ ;
	else /* AddressSize = 32, use ECX for CountReg;*/ 
	CountReg=__ECX__ ;
	//
	//unsigned long addr=( AddressSize == 16)? __DI__:__EDI__;
	//unsigned n=CountReg*(OperandSize>>3);
	//addr+=BASE(descriptors[ES]);


	while (CountReg !=0)
	{
	   //service pending interrupts (if any);
	   if (!ins_ ( OperandSize,AddressSize)){ success = false; break;};
	   CountReg = CountReg - 1;
	}
	if( AddressSize == 16) __CX__ =CountReg;
	else /* AddressSize = 32, use ECX for CountReg;*/ 
	__ECX__ =CountReg;

	//log0("read %x bytes from %xto %x:\n",n,__DX__,addr);
	//dump_mem(0,addr,n);
	return success;
}
