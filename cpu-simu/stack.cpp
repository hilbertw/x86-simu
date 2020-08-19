#include "cpu.h"

extern struct reg_update_rec reg_update_table[10];
extern int reg_update_rec_num;
//
//The stack address-size attribute is controlled by the B-bit of the data-segment descriptor in the SS register. 
//A value of zero in the B-bit selects a stack address-size attribute of 16; a value of one s
//elects a stack address-size attribute of 32. 

bool     CCPU:: Push (unsigned long SOURCE, int  OperandSize)
{
	assert(OperandSize==16||OperandSize==32);
	int StackAddrSize =BIG_SEGMENT(ss_desc)?32:16;
	unsigned long top;

	//  if SP or ESP is 1, the 80386 shuts down due to a lack of stack space
	//
	//
	// type check. in load_ss. ss_desc has been checked: data seg, rtopdable, writable
	//
	//
	// limit check,#SS(0) if the new value of SP or ESP is outside the stack segment limit;
	//
	if(!check_stack_for_push(OperandSize>>3))
	{
		SS_(0);
		return false;
	}

	//
	// perform push
	//
	if( StackAddrSize == 16)
	{
	   if( OperandSize == 16 )
	   {
		  top= __SP__ - 2;

		  /*(SS:SP) := (SOURCE);  word assignment */
	   }
	   else
	   {
		  top= __SP__ - 4;
		 /* (SS:SP) := (SOURCE); dword assignment */
	   }



	   	RW_STATUS status;
	   if(status=mmu.st(BASE(ss_desc)+(unsigned short)top,__CPL__==3,(char *)&SOURCE,OperandSize>>3)) 
	   {
		   PFT_(status&0x1f);
		   return false;
		}
	   __SP__=(unsigned short)top;
	}
	else
	{/* StackAddrSize = 32 */
	   if( OperandSize == 16)
	   {
		  top= __ESP__ - 2;
		  /*(SS:ESP) := (SOURCE);  word assignment */
	   }
	   else
	   {
	 
		  top = __ESP__ - 4;
		  /*(SS:ESP) := (SOURCE);  dword assignment */
	   }

	   	RW_STATUS status;
	   if(status=mmu.st(BASE(ss_desc)+top,__CPL__==3,(char *)&SOURCE,OperandSize>>3)) 
	   {
		   PFT_(status&0x1f);
		   return false;
		}
	   __ESP__=top;
	}
	return true ;
}
bool  CCPU:: Pop ( unsigned long &DEST,int  OperandSize)
{
	assert(OperandSize==16||OperandSize==32);
	int StackAddrSize =BIG_SEGMENT(ss_desc)?32:16;
	unsigned long top;
	;

	//
	// type check. in load_ss. ss_desc has been checked: data seg, rtopdable, writtopble
	//
	//
	// limit check,#SS(0) if the new value of SP or ESP is outside the stack segment limit;
	//
	if(!check_stack_for_pop(OperandSize>>3))
    {
	   SS_(0);
	   return false;
	}

	//
	// perform push
	//
	if( StackAddrSize == 16)
	{
	   if (OperandSize == 16 )
	   {
		  top= __SP__ + 2;

		  /*DEST := (SS:SP); word assignment */
	   }
	   else
	   {
		  top= __SP__ + 4;
		 /* DEST := (SS:SP); dword assignment */
	   }


	   	RW_STATUS status;
	   if(status=mmu.ld(BASE(ss_desc)+__SP__,__CPL__==3,(char *)&DEST,OperandSize>>3)) 
	   {
		   PFT_(	status);
		   return false;
		}
	   __SP__=(unsigned short)top;
	}
	else
	{/* StackAddrSize = 32 */
	   if( OperandSize == 16)
	   {
		  top= __ESP__ + 2;
		  /*DEST := (SS:ESP);  word assignment */
	   }
	   else
	   {
	 
		  top = __ESP__ + 4;
		  /*DEST := (SS:ESP);  dword assignment */
	   }

		RW_STATUS status;
	   if(status=mmu.ld(BASE(ss_desc)+__ESP__,__CPL__==3,(char *)&DEST,OperandSize>>3)) 
		{
		   PFT_(status&0x1f);
		   return false;
		}
	   __ESP__=top;
	}
	return true;
}

//#SS(0) if the starting or ending stack address is outside the stack segment limit; 
//#PF(fault-code) for a page fault 
//Before executing PUSHA or PUSHAD, the 80386 shuts down if SP or ESP equals 1, 3, or 5; 
//if SP or ESP equals 7, 9, 11, 13, or 15, exception 13 occurs 
bool     CCPU:: Pusha ( int  OperandSize)
{
	unsigned long Temp;

	if( OperandSize == 16)/* PUSHA instruction */
	{
		if(!check_stack_for_push(16))
		{
			SS_(0);
			return false;
		}

	   Temp = __SP__;
	   Push(__AX__,OperandSize);
	   Push(__CX__,OperandSize);
	   Push(__DX__,OperandSize);
	   Push(__BX__,OperandSize);
	   Push(Temp,OperandSize);
	   Push(__BP__,OperandSize);
	   Push(__SI__,OperandSize);
	   Push(__DI__,OperandSize);
	}else  /* OperandSize = 32, PUSHAD instruction */
	{
		if(!check_stack_for_push(32))
		{
			SS_(0);
			return false;
		}
	   Temp = __ESP__;
	   Push(__EAX__,OperandSize);
	   Push(__ECX__,OperandSize);
	   Push(__EDX__,OperandSize);
	   Push(__EBX__,OperandSize);
	   Push(Temp,OperandSize);
	   Push(__EBP__,OperandSize);
	   Push(__ESI__,OperandSize);
	   Push(__EDI__,OperandSize);
	}
	return true;
}

bool     CCPU:: Popa ( int  OperandSize)
{
	unsigned long Temp;
	if( OperandSize == 16) /* instruction = POPA */
	{
		if(!check_stack_for_pop(16))
		{
			SS_(0);
			return false;
		}
		Pop(Temp,OperandSize);
	   __DI__ = (unsigned short)Temp;
		 Pop(Temp,OperandSize);
	   __SI__ =  (unsigned short)Temp;
	   Pop(Temp,OperandSize);
	   __BP__ =  (unsigned short)Temp;
	   Pop(Temp,OperandSize);/* Skip SP */

	   Pop (Temp,OperandSize); 
	   __BX__ =  (unsigned short)Temp;
	   Pop(Temp,OperandSize);
	   __DX__ =  (unsigned short)Temp;
	   Pop(Temp,OperandSize);
	   __CX__ =  (unsigned short)Temp;
	   Pop(Temp,OperandSize);
	   __AX__ =  (unsigned short)Temp;

	}
	else
	{
		if(!check_stack_for_pop(32))
		{
			SS_(0);
			return false;
		}
		/* OperandSize = 32, instruction = POPAD */
	   Pop(Temp,OperandSize);
	   __EDI__ = Temp;
	   Pop(Temp,OperandSize);
	   __ESI__ = Temp;
	   Pop(Temp,OperandSize);
	   __EBP__ = Temp;
	   Pop(Temp,OperandSize);/* Skip ESP */
	   Pop (Temp,OperandSize); 
	   __EBX__ = Temp;
	   Pop(Temp,OperandSize);
	   __EDX__ = Temp;
	   Pop(Temp,OperandSize);
	   __ECX__ = Temp;
	   Pop(Temp,OperandSize);
	   __EAX__ = Temp;

	}
	return true;
}



bool CCPU:: check_stack_for_pop (int size) // for pop 'size' bytes
{
	unsigned long sp=BIG_SEGMENT(descriptors[SS])?__ESP__:__SP__;

	return check_limit(&descriptors[SS],sp,sp+size-1);
}

bool CCPU:: check_stack_for_push (int size) //for push 'size' bytes
{
	return BIG_SEGMENT(descriptors[SS])?
		check_limit(&descriptors[SS],(unsigned long)(__ESP__-size),(unsigned long)(__ESP__-1)):
		check_limit(&descriptors[SS],(unsigned short)(__SP__-size),(unsigned short)(__SP__-1));
}


bool CCPU:: CHECK_STACK2 (int lower,int upper)/* for far return */
{
	//unsigned long sp=BIG_SEGMENT(descriptors[SS])?__ESP__:__SP__;

	//return check_limit(&descriptors[SS],sp+lower,sp+upper);
	return BIG_SEGMENT(descriptors[SS])?
		check_limit(&descriptors[SS],(unsigned long)(__ESP__+lower),(unsigned long)(__ESP__+upper)):
		check_limit(&descriptors[SS],(unsigned short)(__SP__+lower),(unsigned short)(__SP__+upper));
}





bool     CCPU:: PUSH (unsigned long SOURCE, int  OperandSize)
{
	assert(OperandSize==16||OperandSize==32);
	int StackAddrSize =BIG_SEGMENT(ss_desc)?32:16;
	unsigned long top;

	//  if SP or ESP is 1, the 80386 shuts down due to a lack of stack space
	//
	//
	// type check. in load_ss. ss_desc has been checked: data seg, rtopdable, writable
	//
	//
	// limit check,#SS(0) if the new value of SP or ESP is outside the stack segment limit;
	//
	if(!check_stack_for_push(OperandSize>>3))
	{
		SS_(0);
		return false;
	}

	//
	// perform push
	//
	if( StackAddrSize == 16)
	{
	   if( OperandSize == 16 )
	   {
		  top= __SP__ - 2;

		  /*(SS:SP) := (SOURCE);  word assignment */
	   }
	   else
	   {
		  top= __SP__ - 4;
		 /* (SS:SP) := (SOURCE); dword assignment */
	   }



	   	RW_STATUS status;
	   if(status=mmu.st(BASE(ss_desc)+(unsigned short)top,0,(char *)&SOURCE,OperandSize>>3)) 
	   {
		   PFT_(status&0x1f);
		   return false;
		}
	   __SP__=(unsigned short)top;
	}
	else
	{/* StackAddrSize = 32 */
	   if( OperandSize == 16)
	   {
		  top= __ESP__ - 2;
		  /*(SS:ESP) := (SOURCE);  word assignment */
	   }
	   else
	   {
	 
		  top = __ESP__ - 4;
		  /*(SS:ESP) := (SOURCE);  dword assignment */
	   }

	   	RW_STATUS status;
	   if(status=mmu.st(BASE(ss_desc)+top,0,(char *)&SOURCE,OperandSize>>3)) 
	   {
		   PFT_(status&0x1f);
		   return false;
		}
	   __ESP__=top;
	}
	return true ;
}
bool  CCPU:: POP ( unsigned long &DEST,int  OperandSize)
{
	assert(OperandSize==16||OperandSize==32);
	int StackAddrSize =BIG_SEGMENT(ss_desc)?32:16;
	unsigned long top;
	;

	//
	// type check. in load_ss. ss_desc has been checked: data seg, rtopdable, writtopble
	//
	//
	// limit check,#SS(0) if the new value of SP or ESP is outside the stack segment limit;
	//
	if(!check_stack_for_pop(OperandSize>>3))
    {
	   SS_(0);
	   return false;
	}

	//
	// perform push
	//
	if( StackAddrSize == 16)
	{
	   if (OperandSize == 16 )
	   {
		  top= __SP__ + 2;

		  /*DEST := (SS:SP); word assignment */
	   }
	   else
	   {
		  top= __SP__ + 4;
		 /* DEST := (SS:SP); dword assignment */
	   }


	   	RW_STATUS status;
	   if(status=mmu.ld(BASE(ss_desc)+__SP__,0,(char *)&DEST,OperandSize>>3)) 
	   {
		   PFT_(	status);
		   return false;
		}
	   __SP__=(unsigned short)top;
	}
	else
	{/* StackAddrSize = 32 */
	   if( OperandSize == 16)
	   {
		  top= __ESP__ + 2;
		  /*DEST := (SS:ESP);  word assignment */
	   }
	   else
	   {
	 
		  top = __ESP__ + 4;
		  /*DEST := (SS:ESP);  dword assignment */
	   }

		RW_STATUS status;
	   if(status=mmu.ld(BASE(ss_desc)+__ESP__,0,(char *)&DEST,OperandSize>>3)) 
		{
		   PFT_(status&0x1f);
		   return false;
		}
	   __ESP__=top;
	}
	return true;
}
