
#include "cpu.h"

/*!
     \brief:
      Notations && Format used in this Document
 */
/*!
     \brief:
      AAA - Ascii Adjust for Addition
      37 
*/
void CCPU::aaa_()
{
  	if ((__AL__ & 0x0f) > 9 || (__AF__  ==  1))
  	{
  		//__AL__ = (__AL__ + 6) & 0x0f;
  		//__AH__ = __AH__ + 1;
		__AX__ = __AX__ + 0x16;
		__AL__ = __AL__ & 0x0f;

  		__AF__ = 1;
  		__CF__ = 1;
  	} else {
  		__CF__ = 0;
  		__AF__ = 0;
  	}
 /* 	__AF__ && __CF__ as described above; __OF__, SF, ZF, && PF are undefined  */
}

/*!
     \brief:
      AAD - Ascii Adjust for Division
      D5 imm   
 */
void CCPU::aad_(unsigned char imm8)
{
	unsigned char tempAL = __AL__;
	unsigned char tempAH = __AH__;
	/* imm8 is set to 0AH for the AAD mnemonic */
	__AL__ = (tempAL + (tempAH * imm8)) & 0xFF; 
	__AH__ = 0;  	
/* SF, ZF, && PF as described in Appendix C; __OF__, __AF__, && __CF__ are undefined  */
}
/*!
     \brief:
      AAM - Ascii Adjust for Multiplication
      D4 0A
  */
void CCPU::aam_(unsigned char imm8)
{
  __AH__ = __AL__ / imm8;
  __AL__ = __AL__ % imm8;
 /* SF, ZF, && PF as described in Appendix C; __OF__, __AF__, && __CF__ are undefined  */
}

/*!
     \brief:
      AAS - Ascii Adjust for Subtraction
      3F        
*/  
void CCPU::aas_()
{
  if ((__AL__ & 0x0f) > 9 || (__AF__  ==  1))
  {
     __AL__ = __AL__ - 6;
     __AL__ = __AL__ & 0x0f;
     __AH__ = __AH__ - 1;
     __AF__ = 1;
     __CF__ = 1;
  } else {
     __CF__ = 0;
     __AF__ = 0;
  }
  /*SF, ZF, && PF as described in Appendix C; __OF__, __AF__, && __CF__ are undefined  */
}

/*!
     \brief:
      ARPL - Adjusted Requested Privilege Level of Selector
     63 /r     ARPL r/m16,r16   
  */
void CCPU:: arpl_(SELECTOR src, SELECTOR &dst)
{
  if (RPL (dst) < RPL (src))
  {
     __ZF__ = 1;
     RPL (dst) = RPL (src);
  } else {
     __ZF__ = 0;
  }
  
}



/*!
     \brief:
      BOUND - Array Index Bound Check
	Real-Address Mode Exceptions
	#UD The ARPL instruction is not recognized in real-address mode.      
  */
void CCPU::bound_(unsigned long LeftSRC,unsigned long RightSRC, int OperandSize)
{
     /* Under lower bound || over upper bound  
  	  #BR;
	  */
  //if ((LeftSRC < [RightSRC] || LeftSRC > [RightSRC + OperandSize/8])
	 // BR_(0)


}

#if 0
/*!
     \brief:
      CMPXCHG - Compare && Exchange
 */

void CCPU:: (cmpxchg)
{
	/* accumulator = __AL__, __AX__, or EAX, depending on whether */
	/* a byte, word, or doubleword comparison is being performed*/
	if (accumulator == DEST)
	{
		__ZF__ = 1;
		DEST = SRC
	} else {
		__ZF__ = 0
		accumulator = DEST
	}
}
/*!
     \brief:
      CMPXCHG8B - Compare && Exchange 8 bytes
 */
void CCPU:: (cmpxchg8b)
{ 
	if (EDX:EAX == DEST)
	{
		__ZF__ = 1
		DEST = ECX:EBX
	} else {
		__ZF__ = 0
		EDX:EAX = DEST 
	}
}


/*!
     \brief:
      CWD - Convert Word to Doubleword
 */

void CCPU:: (cwd)
{      
	if (OperandSize  ==  16) /* CWD instruction */
	{
   		if (__AX__ < 0 )
   			__DX__ = 0FFFFH; 
   		else  
   			__DX__ = 0; 
	} else { /* OperandSize  ==  32, CDQ instruction */
   		if (EAX < 0 )
   			EDX = 0FFFFFFFFH; 
   		else  
   			EDX = 0; 
   	}
}      

/*!
     \brief:
      CWDE - Convert Word to Extended Doubleword
*/

#endif

/*!
     \brief:
      DAA - Decimal Adjust for Addition
      27
*/

void CCPU:: daa_()
{
	if (((__AL__ & 0x0f) > 9) || (__AF__  ==  1))
	{
   		__AL__ = __AL__ + 6;
   		__AF__ = 1;
	} else {
   		__AF__ = 0;
	}
	
	if ((__AL__ > 0x9f) || (__CF__  ==  1))
	{
   		__AL__ = __AL__ + 0x60;
   		__CF__ = 1;
	} else { 
		__CF__ = 0;
	}
}

/*!
     \brief:
      DAS - Decimal Adjust for Subtraction
      2F  
*/
void CCPU:: das_()
{
	if ((__AL__ & 0x0f) > 9 || (__AF__  ==  1))
	{
   		__AL__ = __AL__ - 6;
   		__AF__ = 1;
	} else {
   		__AF__ = 0;
	}
	
	if ((__AL__ > 0x9f) || (__CF__  ==  1))
	{
   		__AL__ = __AL__ - 0x60;
   		__CF__ = 1;
	} else { 
		__CF__ = 0;
	}
}

#if 0
/*!
     \brief:
      DEC - Decrement
 */
void CCPU:: (div)
{
	DEST = src1 - 1;
}      
/*!
     \brief:
      DIV - Divide
 */
void CCPU:: (div)
{
	if (SRC == 0) #DE; /* divide error */
	
	if (OperandSize == 8) /* word/byte operation */
	{
		temp = __AX__ / SRC;
		if (temp > FFH) #DE; /* divide error */ ;

		__AL__ = temp;
		__AH__ = __AX__ MOD SRC;
	} else {
		if (OperandSize ==  16) /* doubleword/word operation */
		{
			temp = __DX__:__AX__ / SRC;
			if (temp > FFFFH) #DE; /* divide error */ ;
			else {
				__AX__ = temp;
				__DX__ = __DX__:__AX__ MOD SRC;
			}
		} else { /* quadword/doubleword operation */
			temp = EDX:EAX / SRC;
			if (temp > FFFFFFFFH) #DE; /* divide error */ ;
			else {
				EAX = temp;
				EDX = EDX:EAX MOD SRC;
			}
		}
	}
}
/*!
     \brief:

	EMMS〞Empty MMX? State
	0F 77
	Protected Mode Exceptions
	#UD If EM in CR0 is set.
	#NM If TS in CR0 is set.
	#MF If there is a pending FPU exception.
	Real-Address Mode Exceptions
	#UD If EM in CR0 is set.
	#NM If TS in CR0 is set.
	#MF If there is a pending FPU exception.
	Virtual-8086 Mode Exceptions
	#UD If EM in CR0 is set.
	#NM If TS in CR0 is set.
	#MF If there is a pending FPU exception.	
  */
void CCPU:: (emms)
{
	FPUTagWord = FFFFH;
}
/*!
     \brief:
      E__NT__ER - Make Stack Frame
 */
void CCPU:: (enter)
{
	NestingLevel = NestingLevel MOD 32;
	if (StackSize == 32)
	{
		Push(EBP) ;
		FrameTemp = ESP;
	} else { /* StackSize = 16*/
		Push(__BP__);
		FrameTemp = __SP__;
	}
	
	if (NestingLevel > 0)
	{
		for (i = 1; i<= (NestingLevel - 1);i++)
		{
			if (OperandSize == 32)
			{
				if (StackSize == 32
				{
					EBP = EBP - 4;
					Push([EBP]); /* doubleword push */
				} else { /* StackSize = 16*/
					__BP__ = __BP__ - 4;
					Push([__BP__]); /* doubleword push */
				}
			} else {/* OperandSize ==  16 */
				if (StackSize == 32)
				{	
					EBP = EBP - 2;
					Push([EBP]); /* word push */
				} else { /* StackSize = 16*/
					__BP__ = __BP__ - 2;
					Push([__BP__]); /* word push */
				}
			}
		}
		
		if (OperandSize == 32)
			Push(FrameTemp); /* doubleword push */
		else  /* OperandSize ==  16 */
			Push(FrameTemp); /* word push */
	}
	
	if (StackSize == 32)
	{
		EBP = FrameTemp
		ESP = EBP - Size;
	} else { /* StackSize = 16*/
		__BP__ = FrameTemp
		__SP__ = __BP__ - Size;
	}
}

/*!
     \brief:
      ESC - Escape
/*!
     \brief:
      Floating point instuctions - no descriptions
/*!
     \brief:
      HLT - Halt CPU
	The HLT instruction is a privileged instruction. When the processor is running in protected or
	virtual-8086 mode, the privilege level of a program or procedure must be 0 to execute the HLT
	instruction.
*/      
void CCPU:: (hlt)
{
}
/*!
     \brief:
      IDIV - Signed Integer Division
 */
void CCPU:: (idiv)
{
	if (SRC == 0) #DE; /* divide error */

	if (OperandSize ==8) /* word/byte operation */
	{
		temp = __AX__ / SRC; /* signed division */
		if ((temp > 7FH) OR (temp < 80H))
		/* if (a positive result is greater than 7FH or a negative result is less than 80H */
			#DE; /* divide error */ ;
		else {
			__AL__ = temp;
			__AH__ = __AX__ SignedModulus SRC;
		}
	} else if (OperandSize ==16) /* doubleword/word operation */
	{
		temp = __DX__:__AX__ / SRC; /* signed division */
		if ((temp > 7FFFH) OR (temp < 8000H))
		/* if (a positive result is greater than 7FFFH */
		/* or a negative result is less than 8000H */
			#DE; /* divide error */ ;
		else {
			__AX__ = temp;
			__DX__ = __DX__:__AX__ SignedModulus SRC;
		}
	} else { /* quadword/doubleword operation */
		temp = EDX:EAX / SRC; /* signed division */
		if ((temp > 7FFFFFFFH) OR (temp < 80000000H))
		/* if (a positive result is greater than 7FFFFFFFH */
		/* or a negative result is less than 80000000H */
			#DE; /* divide error */ ;
		else {
			EAX = temp;
			EDX = EDX:EAX SignedModulus SRC;
		}
	}
}

/*!
     \brief:
      IMUL - Signed Multiply

 */
void CCPU:: (imul)
{
	if (NumberOfOperands == 1)
	{
		if (OperandSize ==  8)
		{
			__AX__ = __AL__ * SRC /* signed multiplication */
			if (((__AH__ == 00H) OR (__AH__ == FFH))
			{
				 __CF__ = 0; __OF__ = 0;
			} else {
				__CF__ = 1; __OF__ = 1;
			}
		} else if (OperandSize ==  16)
		{
			__DX__:__AX__ = __AX__ * SRC /* signed multiplication */
			if (((__DX__ = 0000H) OR (__DX__ = FFFFH))
			{
				__CF__ = 0; __OF__ = 0;
			} else {
				__CF__ = 1; __OF__ = 1;
			}
		} else { /* OperandSize ==  32 */
			EDX:EAX = EAX * SRC /* signed multiplication */
			if (((EDX = 00000000H) OR (EDX = FFFFFFFFH))
			{
				__CF__ = 0; __OF__ = 0;
			} else {
				__CF__ = 1; __OF__ = 1;
			}
		}
	} else if (NumberOfOperands == 2)
	{
		temp = DEST * SRC /* signed multiplication; temp is double DEST size*/
		DEST = DEST * SRC /* signed multiplication */
		if (temp <> DEST)
		{
			__CF__ = 1; __OF__ = 1;
		} else {
			__CF__ = 0; __OF__ = 0;
		}
	} else { /* NumberOfOperands = 3 */
		DEST = SRC1 * SRC2 /* signed multiplication */
		temp = SRC1 * SRC2 /* signed multiplication; temp is double SRC1 size */
		if (temp <> DEST)
		{
			__CF__ = 1; __OF__ = 1;
		} else {
			__CF__ = 0; __OF__ = 0;
		}
	}
}
/*!
     \brief:
      IN - Input Byte || Word From Port
 */
void CCPU:: (in)
{
	if (((__PE__ == 1) AND ((__CPL__ > __IOPL__) OR (____VM____ == 1)))
	{ /* Protected mode with __CPL__ > __IOPL__ or virtual-8086 mode */
		if ((Any I/O Permission Bit for I/O port being accessed == 1)
		 /* I/O operation is not allowed */
			#GP(0);
		else /* I/O operation is allowed */
			DEST = SRC; /* Reads from selected I/O port */
	}
	else /*Real Mode or Protected Mode with CPL <= IOPL */
		DEST = SRC; /* Reads from selected I/O port */
}
/*!
     \brief:
      INC - Increment
 */
void CCPU:: (inc)
{
	DEST = DEST + 1;	
}

/*!
     \brief:
      INS - Input String from Port
 */
void CCPU:: (ins)
{	
	if (((__PE__ == 1) AND ((__CPL__ > __IOPL__) OR (____VM____ == 1)))
	{ /* Protected mode with __CPL__ > __IOPL__ or virtual-8086 mode */
		if (Any I/O Permission Bit for I/O port being accessed == 1)
			 /* I/O operation is not allowed */
			#GP(0);
		else /* I/O operation is allowed */
			DEST = SRC; /* Reads from I/O port */

	} else {/*Real Mode or Protected Mode with CPL <= IOPL */
		DEST = SRC; /* Reads from I/O port */
	
		if (byte transfer)
		{
			if (__DF__ == 0)
		 		(E)DI__ = (E)DI__ + 1;
			else
			 	(E)DI__ = (E)DI__ 每 1;
		} else if (word transfer)
		{
			if (__DF__ == 0)
				(E)DI__ = (E)DI__ + 2;
			else 
				(E)DI__ = (E)DI__ 每 2;
		} else {/* doubleword transfer */
			if (__DF__ == 0)
				(E)DI__ = (E)DI__ + 4;
			else 
				E)__DI__ = (E)DI__ 每 4;
		}
	}
}

/*!
     \brief:
      INT - Interrupt
 */
void CCPU:: (int)
{
	if ( __PE__==0 )
		GOTO REAL-ADDRESS-MODE;
	} else { /* __PE__=1 */
		if (__VM__ == 1 AND IOPL < 3 AND int n)
			#GP(0);
		else /* protected mode or virtual-8086 mode interrupt */
			GOTO PROTECTED-MODE;
	}

REAL-ADDRESS-MODE:
	if (((DEST * 4) + 3) is not within IDT limit ) #GP; 
	if (stack not large enough for a 6-byte return information ) #SS; 
	Push (EFLAGS[15:0]);
	__IF__ =  0; /* Clear interrupt flag */
	__TF__ = 0; /* Clear trap flag */
	AC = 0; /*Clear AC flag*/
	Push(CS);
	Push(IP);
	/* No error codes are pushed */
	CS = IDT(Descriptor (vector_number * 4), selector));
	EIP = IDT(Descriptor (vector_number * 4), offset)); /* 16 bit offset AND 0000FFFFH */


PROTECTED-MODE:
	if (((DEST * 8) + 7) is not within IDT limits
		OR selected IDT descriptor is not an interrupt-, trap-, or task-gate type
		#GP((DEST * 8) + 2 + EXT);
		/* EXT is bit 0 in error code */
	if (software interrupt /* generated by I__NT__ n, I__NT__ 3, or I__NT__O */
	{
		if (gate descriptor DPL < CPL) #GP((vector_number * 8) + 2 );
		/* __PE__=1, DPL<CPL, software interrupt */
	} 
	if (gate not present) #NP((vector_number * 8) + 2 + EXT); 
	if (task gate /* specified in the selected interrupt table descriptor */
		GOTO TASK-GATE;
	else 	
		GOTO TRAP-OR-INTERRUPT-GATE; /* __PE__=1, trap/interrupt gate */
	


TASK-GATE: /* __PE__=1, task gate */
	Read segment selector in task gate (IDT descriptor);
	if (local/global bit is set to local
		OR index not within GDT limits
		#GP(TSS selector);

	Access TSS descriptor in GDT;
	if (TSS descriptor specifies that the TSS is busy (low-order 5 bits set to 00001)) #GP(TSS selector);

	if (TSS not present) #NP(TSS selector);

	SWITCH-TASKS (with nesting) to TSS;
	if (interrupt caused by fault with error code
	{
		if (stack limit does not allow push of error code) #SS(0);
		Push(error code);
	}
	if (EIP not within code segment limit) #GP(0);


TRAP-OR-INTERRUPT-GATE
	Read segment selector for trap or interrupt gate (IDT descriptor);
	if (segment selector for code segment is null) #GP(0H + EXT); /* null selector with EXT flag set */

	if (segment selector is not within its descriptor table limits) #GP(selector + EXT);

	Read trap or interrupt handler descriptor;
	if (descriptor does not indicate a code segment OR code segment descriptor DPL > CPL
		#GP(selector + EXT);

	if (trap or interrupt gate segment is not present) #NP(selector + EXT);

	if (code segment is non-conforming AND DPL < CPL
	{ 
		if (__VM__=0
			goto I__NT__ER-PRIVILEGE-LEVEL-INTERRUPT;
			/* __PE__=1, interrupt or trap gate, nonconforming */
		/* code segment, DPL<CPL, __VM__=0 */
		} else { /* __VM__=1 */
			if (code segment DPL 1 0 ) #GP(new code segment selector); 
			GOTO INTERRUPT-FROM-VIRTUAL-8086-MODE;
			/* __PE__=1, interrupt or trap gate, DPL<CPL, __VM__=1 */
		}
	} else { /* __PE__=1, interrupt or trap gate, DPL 3 CPL */
		if __VM__ == 1) #GP(new code segment selector); 
		if (code segment is conforming OR code segment DPL = CPL
			goto INTRA-PRIVILEGE-LEVEL-INTERRUPT;
		else 
			#GP(CodeSegmentSelector + EXT);
			/* __PE__=1, interrupt or trap gate, nonconforming */
			/* code segment, DPL>CPL */
	}
I__NT__ER-PREVILEGE-LEVEL-INTERRUPT
	/* __PE__=1, interrupt or trap gate, non-conforming code segment, DPL<CPL */
	/* Check segment selector and descriptor for stack of new privilege level in current TSS */
	if (current TSS is 32-bit TSS
	{
		TSSstackAddress = (new code segment DPL * 8) + 4
		if ((TSSstackAddress + 7) > TSS limit) #TS(current TSS selector); 
		NewSS = TSSstackAddress + 4;
		NewESP = stack address;

	} else { /* TSS is 16-bit */
		TSSstackAddress = (new code segment DPL * 4) + 2
		if ((TSSstackAddress + 4) > TSS limit) #TS(current TSS selector); 
		NewESP = TSSstackAddress;
		NewSS = TSSstackAddress + 2;
	}
	if (segment selector is null ) #TS(EXT);
	if (segment selector index is not within its descriptor table limits
		OR segment selector's RPL 1 DPL of code segment, 
		#TS(SS selector + EXT);

	Read segment descriptor for stack segment in GDT or LDT;
	if (stack segment DPL 1 DPL of code segment,
		OR stack segment does not indicate writable data segment,
		#TS(SS selector + EXT);

	if (stack segment not present ) #SS(SS selector+EXT);
	if (32-bit gate)
	{
		if (new stack does not have room for 24 bytes (error code pushed)
			OR 20 bytes (no error code pushed)
			 #SS(segment selector + EXT);

	} else { /* 16-bit gate */
		if (new stack does not have room for 12 bytes (error code pushed)
			OR 10 bytes (no error code pushed);
			#SS(segment selector + EXT);
	}

	if (instruction pointer is not within code segment limits ) #GP(0);
	SS:ESP = TSS(NewSS:NewESP) /* segment descriptor information also loaded */
	if (32-bit gate)
		CS:EIP = Gate(CS:EIP); /* segment descriptor information also loaded */
	else /* 16-bit gate */
		CS:IP = Gate(CS:IP); /* segment descriptor information also loaded */

	if (32-bit gate)
	{
		Push(far pointer to old stack); /* old SS and ESP, 3 words padded to 4 */;
		Push(EFLAGS);
		Push(far pointer to return instruction); /* old CS and EIP, 3 words padded to 4*/;
		Push(ErrorCode); /* if (needed, 4 bytes */
	} else {/* 16-bit gate */
		Push(far pointer to old stack); /* old SS and __SP__, 2 words */;
		Push(EFLAGS(15..0));
		Push(far pointer to return instruction); /* old CS and IP, 2 words */;
		Push(ErrorCode); /* if (needed, 2 bytes */
	}
	CPL = CodeSegmentDescriptor(DPL);
	CS(RPL) = CPL;
	if (interrupt gate
	{ 
		__IF__ =  0 /* interrupt flag to 0 (disabled) */;
		__TF__ = 0;
		__VM__ = 0;
		__RF__ = 0;
		__NT__ = 0;
	}
	
INTERRUPT-FROM-VIRTUAL-8086-MODE:
/* Check segment selector and descriptor for privilege level 0 stack in current TSS */
	if (current TSS is 32-bit TSS
	{
		TSSstackAddress = (new code segment DPL * 8) + 4
		if ((TSSstackAddress + 7) > TSS limit) #TS(current TSS selector);
		NewSS = TSSstackAddress + 4;
		NewESP = stack address;
	} else { /* TSS is 16-bit */
		TSSstackAddress = (new code segment DPL * 4) + 2
		if ((TSSstackAddress + 4) > TSS limit) #TS(current TSS selector);
		NewESP = TSSstackAddress;
		NewSS = TSSstackAddress + 2;
	}
	if (segment selector is null ) #TS(EXT);
	if (segment selector index is not within its descriptor table limits
		OR segment selector's RPL 1 DPL of code segment,
		#TS(SS selector + EXT);

	Access segment descriptor for stack segment in GDT or LDT;
	if (stack segment DPL 1 DPL of code segment,
		OR stack segment does not indicate writable data segment,
		#TS(SS selector + EXT);

	if (stack segment not present ) #SS(SS selector+EXT);

	if (32-bit gate)
	{
		if (new stack does not have room for 40 bytes (error code pushed)
			OR 36 bytes (no error code pushed);
			#SS(segment selector + EXT);

	} else { /* 16-bit gate */
		if (new stack does not have room for 20 bytes (error code pushed)
			OR 18 bytes (no error code pushed);
			 #SS(segment selector + EXT);
	}

	if (instruction pointer is not within code segment limits ) #GP(0);
	tempEFLAGS = EFLAGS;
	__VM__ = 0;
	__TF__ = 0;
	__RF__ = 0;
	if (service through interrupt gate) 
		__IF__ =  0;
	TempSS = SS;
	TempESP = ESP;
	SS:ESP = TSS(SS0:ESP0); /* Change to level 0 stack segment */
	/* Following pushes are 16 bits for 16-bit gate and 32 bits for 32-bit gates */
	/* Segment selector pushes in 32-bit mode are padded to two words */
	Push(GS);
	Push(FS);
	Push(DS);
	Push(ES);
	Push(TempSS);
	Push(TempESP);
	Push(TempEFlags);
	Push(CS);
	Push(EIP);
	GS = 0; /*segment registers nullified, invalid in protected mode */
	FS = 0;
	DS = 0;
	ES = 0;
	CS = Gate(CS);
	if (OperandSize ==  32)
		EIP = Gate(instruction pointer);
	else  /* OperandSize is 16 */
		EIP = Gate(instruction pointer) AND 0000FFFFH;
	/* Starts execution of new routine in Protected Mode */

INTRA-PRIVILEGE-LEVEL-INTERRUPT:
	/* __PE__=1, DPL = CPL or conforming segment */
	if (32-bit gate)
	{
		if (current stack does not have room for 16 bytes (error code pushed)
			OR 12 bytes (no error code pushed); ) #SS(0);
	
	} else { /* 16-bit gate */
		if (current stack does not have room for 8 bytes (error code pushed)
			OR 6 bytes (no error code pushed); ) #SS(0);
	}
	
	if (instruction pointer not within code segment limit ) #GP(0);
	if (32-bit gate)
	{
		Push (EFLAGS);
		Push (far pointer to return instruction); /* 3 words padded to 4 */
		CS:EIP = Gate(CS:EIP); /* segment descriptor information also loaded */
		Push (ErrorCode); /* if (any */
	} else { /* 16-bit gate */
		Push (FLAGS);
		Push (far pointer to return location); /* 2 words */
		CS:IP = Gate(CS:IP); /* segment descriptor information also loaded */
		Push (ErrorCode); /* if (any */
	}
	CS(RPL) = CPL;
	if (interrupt gate
	{
		__IF__ =  0;
		__TF__ = 0;
		__NT__ = 0;
		__VM__ = 0;
		__RF__ = 0;
	}

}
/*!
     \brief:
     INVD〞Invalidate Internal Caches
     0F 08
     */
void CCPU:: (invd)
{
	Flush(InternalCaches);
	SignalFlush(ExternalCaches);
	/* Continue execution);
}

/*!
     \brief:
     INVLPG〞Invalidate TLB Entry
     0F 01/7
    */
void CCPU:: (invlpg)
{
	Flush(RelevantTLBEntries);
	 /* Continue execution);
}

/*!
     \brief:
	IRET/IRETD〞Interrupt Return
 */
void CCPU:: (iret)
{
	if (__PE__ == 0
		GOTO REAL-ADDRESS-MODE:;
	else 
		GOTO PROTECTED-MODE;

REAL-ADDRESS-MODE;
	if (OperandSize ==  32)
	{
		if (top 12 bytes of stack not within stack limits ) #SS;
		if (instruction pointer not within code segment limits ) #GP(0);
		EIP = Pop();
		CS = Pop(); /* 32-bit pop, high-order 16-bits discarded */
		tempEFLAGS = Pop();
		EFLAGS = (tempEFLAGS AND 257FD5H) OR (EFLAGS AND 1A0000H);
	} else { /* OperandSize ==  16 */
		if (top 6 bytes of stack are not within stack limits ) #SS;
		if (instruction pointer not within code segment limits ) #GP(0);
		EIP = Pop();
		EIP = EIP AND 0000FFFFH;
		CS = Pop(); /* 16-bit pop */
		EFLAGS[15:0] = Pop();
	}

PROTECTED-MODE:
	if (____VM____ == 1) /* Virtual-8086 mode: __PE__=1, __VM__=1 */
		GOTO RETURN-FROM-VIRTUAL-8086-MODE; /* __PE__=1, __VM__=1 */

	if (__NT__ == 1)
		GOTO TASK-RETURN;( *__PE__=1, __VM__=0, __NT__=1 */

	if (OperandSize ==  32)
	{
		if (top 12 bytes of stack not within stack limits) #SS(0)

		tempEIP = Pop();
		tempCS = Pop();
		tempEFLAGS = Pop();
	} else { /* OperandSize ==  16 */
		if (top 6 bytes of stack are not within stack limits) #SS(0);
		tempEIP = Pop();
		tempCS = Pop();
		tempEFLAGS = Pop();
		tempEIP = tempEIP AND FFFFH;
		tempEFLAGS = tempEFLAGS AND FFFFH;
	}
	
	if (tempEFLAGS(__VM__) = 1 AND CPL=0
		goto RETURN-TO-VIRTUAL-8086-MODE;
		/* __PE__=1, __VM__=1 in EFLAGS image */
	else 
		GOTO PROTECTED-MODE-RETURN;
		/* __PE__=1, __VM__=0 in EFLAGS image */

RETURN-FROM-VIRTUAL-8086-MODE:
/* Processor is in virtual-8086 mode when IRET is executed and stays in virtual-8086 mode */
	if (IOPL=3 /* Virtual mode: __PE__=1, __VM__=1, IOPL=3 */
	{ 
		if (OperandSize ==  32)
		{
			if (top 12 bytes of stack not within stack limits ) #SS(0);
			if (instruction pointer not within code segment limits ) #GP(0);
			EIP = Pop();
			CS = Pop(); /* 32-bit pop, high-order 16-bits discarded */
			EFLAGS = Pop();
			/*__VM__,IOPL,VIP,and Vif (EFLAGS bits are not modified by pop */
		} else { /* OperandSize ==  16 */
			if (top 6 bytes of stack are not within stack limits ) #SS(0);
			if (instruction pointer not within code segment limits ) #GP(0);
			EIP = Pop();
			EIP = EIP AND 0000FFFFH;
			CS = Pop(); /* 16-bit pop */
			EFLAGS[15:0] = Pop(); /* IOPL in EFLAGS is not modified by pop */
		}
	} else 
		#GP(0); /* trap to virtual-8086 monitor: __PE__=1, __VM__=1, IOPL<3 */
	

RETURN-TO-VIRTUAL-8086-MODE:
	/* Interrupted procedure was in virtual-8086 mode: __PE__=1, __VM__=1 in flags image */
	if (top 24 bytes of stack are not within stack segment limits) #SS(0);
	if (instruction pointer not within code segment limits) #GP(0);
	CS = tempCS;
	EIP = tempEIP;
	EFLAGS = tempEFLAGS
	TempESP = Pop();
	TempSS = Pop();
	ES = Pop(); /* pop 2 words; throw away high-order word */
	DS = Pop(); /* pop 2 words; throw away high-order word */
	FS = Pop(); /* pop 2 words; throw away high-order word */
	GS = Pop(); /* pop 2 words; throw away high-order word */
	SS:ESP = TempSS:TempESP;
	/* Resume execution in Virtual-8086 mode */

TASK-RETURN: /* __PE__=1, __VM__=1, __NT__=1 */
	Read segment selector in link field of current TSS;
	if (local/global bit is set to local
		OR index not within GDT limits, 
		#GP(TSS selector);

	Access TSS for task specified in link field of current TSS;
	if (TSS descriptor type is not TSS or if (the TSS is marked not busy) #GP(TSS selector);

	if (TSS not present) #NP(TSS selector);

	SWITCH-TASKS (without nesting) to TSS specified in link field of current TSS;
	Mark the task just abandoned as NOT BUSY;
	if (EIP is not within code segment limit) #GP(0);

PROTECTED-MODE-RETURN: /* __PE__=1, __VM__=0 in flags image */
	if (return code segment selector is null { GP(0);
	if (return code segment selector addrsses descriptor beyond descriptor table limit) #GP(selector;
	Read segment descriptor pointed to by the return code segment selector
	if (return code segment descriptor is not a code segment ) #GP(selector);
	if (return code segment selector RPL < CPL ) #GP(selector);
	if (return code segment descriptor is conforming
		AND return code segment DPL > return code segment selector RPL
		#GP(selector);
	if (return code segment descriptor is not present ) #NP(selector); FI:
	if (return code segment selector RPL > CPL
		GOTO RETURN-OUTER-PRIVILEGE-LEVEL;
	else
		GOTO RETURN-TO-SAME-PRIVILEGE-LEVEL

return-TO-SAME-PRIVILEGE-LEVEL: /* __PE__=1, __VM__=0 in flags image, RPL=CPL */
	if (EIP is not within code segment limits ) #GP(0);
	EIP = tempEIP;
	CS = tempCS; /* segment descriptor information also loaded */
	EFLAGS (__CF__, PF, __AF__, ZF, SF, __TF__, __DF__, __OF__, __NT__) = tempEFLAGS;
	if (OperandSize ==  32)
		EFLAGS(__RF__, AC, ID) = tempEFLAGS;

	if (CPL <= IOPL
		EFLAGS(IF) = tempEFLAGS;
		
	if (__CPL__ == 0)
	{
		EFLAGS(IOPL) = tempEFLAGS;
		if (OperandSize ==  32)
			EFLAGS(__VM__, VIF, VIP) = tempEFLAGS;
	}

RETURN-TO-OUTER-PRIVILGE-LEVEL:
	if (OperandSize ==  32)
	{
		if (top 8 bytes on stack are not within limits ) #SS(0);
	} else { /* OperandSize=16 */
		if (top 4 bytes on stack are not within limits ) #SS(0);
	}
	Read return segment selector;
	if (stack segment selector is null ) #GP(0);
	if (return stack segment selector index is not within its descriptor table limits) #GP(SSselector);
	Read segment descriptor pointed to by return segment selector;
	if (stack segment selector RPL 1 RPL of the return code segment selector
	if (stack segment selector RPL 1 RPL of the return code segment selector
		OR the stack segment descriptor does not indicate a a writable data segment;
		OR stack segment DPL 1 RPL of the return code segment selector
		#GP(SS selector);
		
	if (stack segment is not present ) #SS(SS selector);
	if (tempEIP is not within code segment limit ) #GP(0);
	EIP = tempEIP;
	CS = tempCS;
	EFLAGS (__CF__, PF, __AF__, ZF, SF, __TF__, __DF__, __OF__, __NT__) = tempEFLAGS;
	if (OperandSize ==  32)
		EFLAGS(__RF__, AC, ID) = tempEFLAGS;

	if (CPL <= IOPL)
		EFLAGS(IF) = tempEFLAGS;

	if (__CPL__ == 0)
	{
		EFLAGS(IOPL) = tempEFLAGS;
		if (OperandSize ==  32)
			EFLAGS(__VM__, VIF, VIP) = tempEFLAGS;
	}
	
	CPL = RPL of the return code segment selector;
	FOR each of segment register (ES, FS, GS, and DS)
	{
		if (segment register points to data or non-conforming code segment
			AND CPL > segment descriptor DPL /* stored in hidden part of segment register */
		{ /* segment register invalid */
			SegmentSelector = 0; /* null segment selector */
		}
	}
}
    
/*!
     \brief:
      JA/JNBE - Jump Above / Jump Not Below || Equal
 */
void CCPU:: (ja)
{
	if (condition
	{
   		EIP = EIP + SignExtend(rel8/16/32);
   		if (OperandSize  ==  16)
   			EIP = EIP && 0000FFFFH;
   	}
}

 */
/*!
     \brief:
      JAE/JNB - Jump Above || Equal / Jump on Not Below
 */
void CCPU:: (jae)
{
	if (condition
	{
   		EIP = EIP + SignExtend(rel8/16/32);
   		if (OperandSize  ==  16)
   			EIP = EIP && 0000FFFFH;
   	}
}      
/*!
     \brief:
      JB/JNAE - Jump Below / Jump Not Above || Equal
 */
void CCPU:: (jb)
{
	if (condition
	{
   		EIP = EIP + SignExtend(rel8/16/32);
   		if (OperandSize  ==  16)
   			EIP = EIP && 0000FFFFH;
   	}
}      

/*!
     \brief:
      JBE/JNA - Jump Below || Equal / Jump Not Above
 */
void CCPU:: (jbe)
{
	if (condition
	{
   		EIP = EIP + SignExtend(rel8/16/32);
   		if (OperandSize  ==  16)
   			EIP = EIP && 0000FFFFH;
   	}
}      
      
/*!
     \brief:
      JC - Jump on Carry
 */
void CCPU:: (jc)
{
	if (condition
	{
   		EIP = EIP + SignExtend(rel8/16/32);
   		if (OperandSize  ==  16)
   			EIP = EIP && 0000FFFFH;
   	}
}      
      
/*!
     \brief:
      JCXZ/JECXZ - Jump if (Register (E)CX__ is Zero
 */
void CCPU:: (jcxz)
{
	if (condition
	{
   		EIP = EIP + SignExtend(rel8/16/32);
   		if (OperandSize  ==  16)
   			EIP = EIP && 0000FFFFH;
   	}
}      
      
/*!
     \brief:
      JE/JZ - Jump Equal / Jump Zero
 */
void CCPU:: (je)
{
	if (condition
	{
   		EIP = EIP + SignExtend(rel8/16/32);
   		if (OperandSize  ==  16)
   			EIP = EIP && 0000FFFFH;
   	}
}      
      
/*!
     \brief:
      JG/JNLE - Jump Greater / Jump Not Less || Equal
 */
void CCPU:: (jg)
{
	if (condition
	{
   		EIP = EIP + SignExtend(rel8/16/32);
   		if (OperandSize  ==  16)
   			EIP = EIP && 0000FFFFH;
   	}
}      
      
/*!
     \brief:
      JGE/JNL - Jump Greater || Equal / Jump Not Less
 */
void CCPU:: (jge)
{
	if (condition
	{
   		EIP = EIP + SignExtend(rel8/16/32);
   		if (OperandSize  ==  16)
   			EIP = EIP && 0000FFFFH;
   	}
}      
      
/*!
     \brief:
      JL/JNGE - Jump Less / Jump Not Greater || Equal
 */
void CCPU:: (jl)
{
	if (condition
	{
   		EIP = EIP + SignExtend(rel8/16/32);
   		if (OperandSize  ==  16)
   			EIP = EIP && 0000FFFFH;
   	}
}      
      
/*!
     \brief:
      JLE/JNG - Jump Less || Equal / Jump Not Greater
 */
void CCPU:: (jle)
{
	if (condition
	{
   		EIP = EIP + SignExtend(rel8/16/32);
   		if (OperandSize  ==  16)
   			EIP = EIP && 0000FFFFH;
   	}
}      
      
/*!
     \brief:
      JMP - Unconditional Jump
 */
void CCPU:: (jmp)
{
	if (near jump
	{ 
		if (near relative jump)
			tempEIP = EIP + DEST; /* EIP is instruction following JMP instruction*/
		else  /* near absolute jump */
			tempEIP = DEST;
	
		if (tempEIP is beyond code segment limit ) #GP(0);
		if (OperandSize ==  32)
			EIP = tempEIP;
		else  /* OperandSize=16 */
			EIP = tempEIP AND 0000FFFFH;

	}
	
	if (far jump AND (__PE__ == 0 OR (__PE__ == 1 AND ____VM____ == 1)) /* real-address or virtual-8086 mode */
	{
		tempEIP = DEST(offset); /* DEST is ptr16:32 or [ m16:32] */

		if (tempEIP is beyond code segment limit ) #GP(0);
		CS = DEST(segment selector); /* DEST is ptr16:32 or [m16:32] */
		if (OperandSize ==  32)
			EIP = tempEIP; /* DEST is ptr16:32 or [m16:32] */
		else  /* OperandSize ==  16 */
			EIP = tempEIP AND 0000FFFFH; /* clear upper 16 bits */

	}
	
	if (far jump AND (__PE__ == 1 AND __VM__ = 0) /* Protected mode, not virtual-8086 mode */
	{
		if (effective address in the CS, DS, ES, FS, GS, or SS segment is illegal
			OR segment selector in target operand null
			#GP(0);
	
		if (segment selector index not within descriptor table limits) #GP(new selector);

		Read type and access rights of segment descriptor;
		if (segment type is not a conforming or nonconforming code segment, call gate,
			task gate, or TSS ) #GP(segment selector);
		Depending on type and access rights
		GO TO CONFORMING-CODE-SEGMENT;
		GO TO NONCONFORMING-CODE-SEGMENT;
		GO TO CALL-GATE;
		GO TO TASK-GATE;
		GO TO TASK-STATE-SEGMENT;
	} else {
		#GP(segment selector);
	}
	
CONFORMING-CODE-SEGMENT:
	if (DPL > CPL ) #GP(segment selector);
	if (segment not present ) #NP(segment selector);
	tempEIP = DEST(offset);
	if (OperandSize=16
		tempEIP = tempEIP AND 0000FFFFH;

	if (tempEIP not in code segment limit ) #GP(0);
	CS = DEST(SegmentSelector); /* segment descriptor information also loaded */
	CS(RPL) = CPL
	EIP = tempEIP;

NONCONFORMING-CODE-SEGMENT:
	if ((RPL > CPL) OR (DPL 1 CPL) ) #GP(code segment selector);
	if (segment not present ) #NP(segment selector);
	if (instruction pointer outside code segment limit ) #GP(0);
	tempEIP = DEST(offset);
	if (OperandSize=16
		tempEIP = tempEIP AND 0000FFFFH;
		
	if (tempEIP not in code segment limit ) #GP(0);
	CS = DEST(SegmentSelector); /* segment descriptor information also loaded */
	CS(RPL) = CPL
	EIP = tempEIP;

CALL-GATE:
	if (call gate DPL < CPL OR call gate DPL < call gate segment-selector RPL) #GP(call gate selector);
	if (call gate not present ) #NP(call gate selector);
	if (call gate code-segment selector is null ) #GP(0);
	if (call gate code-segment selector index is outside descriptor table limits) #GP(code segment selector);
	Read code segment descriptor;
	if (code-segment segment descriptor does not indicate a code segment
		OR code-segment segment descriptor is conforming and DPL > CPL
		OR code-segment segment descriptor is non-conforming and DPL 1 CPL
		#GP(code segment selector);
		
	if (code segment is not present ) #NP(code-segment selector);
	if (instruction pointer is not within code-segment limit ) #GP(0);
	tempEIP = DEST(offset);
	if (GateSize=16
		tempEIP = tempEIP AND 0000FFFFH;

	if (tempEIP not in code segment limit ) #GP(0);
	CS = DEST(SegmentSelector); /* segment descriptor information also loaded */
	CS(RPL) = CPL
	EIP = tempEIP;

TASK-GATE:
	if (task gate DPL < CPL OR task gate DPL < task gate segment-selector RPL) #GP(task gate selector);
	if (task gate not present ) #NP(gate selector);
	Read the TSS segment selector in the task-gate descriptor;
	if (TSS segment selector local/global bit is set to local
		OR index not within GDT limits
		OR TSS descriptor specifies that the TSS is busy
		#GP(TSS selector);
		
	if (TSS not present ) #NP(TSS selector);
	SWITCH-TASKS to TSS;
	if (EIP not within code segment limit ) #GP(0);

TASK-STATE-SEGMENT:
	if (TSS DPL < CPL 
		OR TSS DPL < TSS segment-selector RPL
		OR TSS descriptor indicates TSS not available
		#GP(TSS selector);
	if (TSS is not present ) #NP(TSS selector);
	SWITCH-TASKS to TSS
	if (EIP not within code segment limit ) #GP(0);
}

/*!
     \brief:
      JNC - Jump Not Carry
 */
void CCPU:: (jnc)
{
	if (condition
	{
   		EIP = EIP + SignExtend(rel8/16/32);
   		if (OperandSize  ==  16
   			EIP = EIP && 0000FFFFH;
   	}
}            
/*!
     \brief:
      JNE/JNZ - Jump Not Equal / Jump Not Zero
 */
void CCPU:: (jne)
{
	if (condition
	{
   		EIP = EIP + SignExtend(rel8/16/32);
   		if (OperandSize  ==  16
   			EIP = EIP && 0000FFFFH;
   	}
}            
/*!
     \brief:
      JNO - Jump Not Overflow
 */
void CCPU:: (jno)
{
	if (condition
	{
   		EIP = EIP + SignExtend(rel8/16/32);
   		if (OperandSize  ==  16
   			EIP = EIP && 0000FFFFH;
   	}
}            
/*!
     \brief:
      JNS - Jump Not Signed
 */
void CCPU:: (jns)
{
	if (condition
	{
   		EIP = EIP + SignExtend(rel8/16/32);
   		if (OperandSize  ==  16
   			EIP = EIP && 0000FFFFH;
   	}
}            
/*!
     \brief:
      JNP/JPO - Jump Not Parity / Jump Parity Odd
 */
void CCPU:: (jnp)
{
	if (condition
	{
   		EIP = EIP + SignExtend(rel8/16/32);
   		if (OperandSize  ==  16
   			EIP = EIP && 0000FFFFH;
   	}
}            
/*!
     \brief:
      JO - Jump on Overflow
 */
void CCPU:: (jo)
{
	if (condition
	{
   		EIP = EIP + SignExtend(rel8/16/32);
   		if (OperandSize  ==  16
   			EIP = EIP && 0000FFFFH;
   	}
}            
/*!
     \brief:
      JP/JPE - Jump on Parity / Jump on Parity Even
 */
void CCPU:: (jp)
{
	if (condition
	{
   		EIP = EIP + SignExtend(rel8/16/32);
   		if (OperandSize  ==  16
   			EIP = EIP && 0000FFFFH;
   	}
}            
/*!
     \brief:
      JS - Jump Signed
 */
void CCPU:: (js)
{
	if (condition
	{
   		EIP = EIP + SignExtend(rel8/16/32);
   		if (OperandSize  ==  16
   			EIP = EIP && 0000FFFFH;
   	}
}            


/*!
     \brief:
      LAR - Load Access Rights
 */
void CCPU:: (lar)
{      
	if (SRC(Offset) > descriptor table limit 
		__ZF__ = 0;
	Read segment descriptor;
	if (SegmentDescriptor(Type) 1 conforming code segment
		AND (CPL > DPL) OR (RPL > DPL)
		OR Segment type is not valid for instruction
		__ZF__ = 0
	else {
		if (OperandSize ==  32)
			DEST = [SRC] AND 00FxFF00H;
		else  /*OperandSize ==  16*/
			DEST = [SRC] AND FF00H;

	}
}      
/*!
     \brief:
      LDS - Load Pointer Using DS
  */
void CCPU:: (lds)
{       
	if ((OperandSize  ==  16)
	{
   		r16 = [Effective Address]; /* 16-bit transfer */
   		Sreg = [Effective Address + 2]; /* 16-bit transfer */
   		/* In Protected Mode, load the descriptor into the segment register */
	} else { /* OperandSize  ==  32 */
   		r32 = [Effective Address]; /* 32-bit transfer */
   		Sreg = [Effective Address + 4]; /* 16-bit transfer */
   		/* In Protected Mode, load the descriptor into the segment register */
	}
	
	if (ProtectedMode
	{ 
		if (SS is loaded
		{ 
			if (SegementSelector = null) #GP(0);

			else if (Segment selector index is not within descriptor table limits
				OR Segment selector RPL 1 CPL
				OR Access rights indicate nonwritable data segment
				OR DPL 1 CPL
				#GP(selector);
		
			else if (Segment marked not present) #SS(selector);
	
			SS = SegmentSelector(SRC);
			SS = SegmentDescriptor([SRC]);
		} else if (DS, ES, FS, or GS is loaded with non-null segment selector
		{
			if (Segment selector index is not within descriptor table limits
				OR Access rights indicate segment neither data nor readable code segment
				OR (Segment is data or nonconforming-code segment
				AND both RPL and CPL > DPL)
				#GP(selector);
			else if (Segment marked not present
				#NP(selector);

			SegmentRegister = SegmentSelector(SRC) AND RPL;
			SegmentRegister = SegmentDescriptor([SRC]);
		
		} else if (DS, ES, FS or GS is loaded with a null selector:
		{
			SegmentRegister = NullSelector; /*Clear descriptor valid bit;*/
			SegmentRegister(DescriptorValidBit) = 0; /*hidden flag; not accessible by software*/
		}
	}

	if ((Real-Address or Virtual-8086 Mode)
		SegmentRegister = SegmentSelector(SRC);
}
      
/*!
     \brief:
      LEA - Load Effective Address
 */
void CCPU:: (lea)
{
	if (OperandSize ==  16 AND AddressSize ==  16
		DEST = EffectiveAddress(SRC); /* 16-bit address */
	else if (OperandSize ==  16 AND AddressSize ==  32
	{
		temp = EffectiveAddress(SRC); /* 32-bit address */
		DEST = temp[0..15]; /* 16-bit address */
	} else if (OperandSize ==  32) AND AddressSize ==  16
	{
		temp = EffectiveAddress(SRC); /* 16-bit address */
		DEST = ZeroExtend(temp); /* 32-bit address */
	} else if (OperandSize ==  32) AND AddressSize ==  32
	{
		DEST = EffectiveAddress(SRC); /* 32-bit address */
	}
}
/*!
     \brief:
      LEAVE - Restore Stack for Procedure Exit
 */
void CCPU:: (leave)
{
	if (StackAddrSize  ==  16
		__SP__ = __BP__;
	else /* StackAddrSize  ==  32 */
   		ESP = EBP;

	if (OperandSize  ==  16)
		__BP__ = Pop();
	else /* OperandSize  ==  32 */
   		EBP = Pop();
}
/*!
     \brief:
      LES - Load Pointer Using ES
  */
void CCPU:: (les)
{
	if ((OperandSize  ==  16)
	{
   		r16 = [Effective Address]; /* 16-bit transfer */
   		Sreg = [Effective Address + 2]; /* 16-bit transfer */
   		/* In Protected Mode, load the descriptor into the segment register */
	} else { /* OperandSize  ==  32 */
   		r32 = [Effective Address]; /* 32-bit transfer */
   		Sreg = [Effective Address + 4]; /* 16-bit transfer */
   		/* In Protected Mode, load the descriptor into the segment register */
	}
	
	if (ProtectedMode
	{ 
		if (SS is loaded
		{ 
			if (SegementSelector = null) #GP(0);

			else if (Segment selector index is not within descriptor table limits
				OR Segment selector RPL 1 CPL
				OR Access rights indicate nonwritable data segment
				OR DPL 1 CPL
				#GP(selector);
		
			else if (Segment marked not present) #SS(selector);
	
			SS = SegmentSelector(SRC);
			SS = SegmentDescriptor([SRC]);
		} else if (DS, ES, FS, or GS is loaded with non-null segment selector
		{
			if (Segment selector index is not within descriptor table limits
				OR Access rights indicate segment neither data nor readable code segment
				OR (Segment is data or nonconforming-code segment
				AND both RPL and CPL > DPL)
				#GP(selector);
			else if (Segment marked not present
				#NP(selector);

			SegmentRegister = SegmentSelector(SRC) AND RPL;
			SegmentRegister = SegmentDescriptor([SRC]);
		
		} else if (DS, ES, FS or GS is loaded with a null selector:
		{
			SegmentRegister = NullSelector; /*Clear descriptor valid bit;*/
			SegmentRegister(DescriptorValidBit) = 0; /*hidden flag; not accessible by software*/
		}
	}

	if ((Real-Address or Virtual-8086 Mode)
		SegmentRegister = SegmentSelector(SRC);
}

/*!
     \brief:
      LFS - Load Pointer Using FS
 */
void CCPU:: (lfs)
{
	if ((OperandSize  ==  16)
	{
   		r16 = [Effective Address]; /* 16-bit transfer */
   		Sreg = [Effective Address + 2]; /* 16-bit transfer */
   		/* In Protected Mode, load the descriptor into the segment register */
	} else { /* OperandSize  ==  32 */
   		r32 = [Effective Address]; /* 32-bit transfer */
   		Sreg = [Effective Address + 4]; /* 16-bit transfer */
   		/* In Protected Mode, load the descriptor into the segment register */
	}
	
	if (ProtectedMode
	{ 
		if (SS is loaded
		{ 
			if (SegementSelector = null) #GP(0);

			else if (Segment selector index is not within descriptor table limits
				OR Segment selector RPL 1 CPL
				OR Access rights indicate nonwritable data segment
				OR DPL 1 CPL
				#GP(selector);
		
			else if (Segment marked not present) #SS(selector);
	
			SS = SegmentSelector(SRC);
			SS = SegmentDescriptor([SRC]);
		} else if (DS, ES, FS, or GS is loaded with non-null segment selector
		{
			if (Segment selector index is not within descriptor table limits
				OR Access rights indicate segment neither data nor readable code segment
				OR (Segment is data or nonconforming-code segment
				AND both RPL and CPL > DPL)
				#GP(selector);
			else if (Segment marked not present
				#NP(selector);

			SegmentRegister = SegmentSelector(SRC) AND RPL;
			SegmentRegister = SegmentDescriptor([SRC]);
		
		} else if (DS, ES, FS or GS is loaded with a null selector:
		{
			SegmentRegister = NullSelector; /*Clear descriptor valid bit;*/
			SegmentRegister(DescriptorValidBit) = 0; /*hidden flag; not accessible by software*/
		}
	}

	if ((Real-Address or Virtual-8086 Mode)
		SegmentRegister = SegmentSelector(SRC);
}
/*!
     \brief:
      LGDT - Load Global Descriptor Table
  */
void CCPU:: (lidt)
{          
	if (OperandSize ==  16
	{
		GDTR(Limit) = SRC[0:15];
		GDTR(Base) = SRC[16:47] AND 00FFFFFFH;
	} else { /* 32-bit Operand Size */
		GDTR(Limit) = SRC[0:15];
		GDTR(Base) = SRC[16:47];
	}
}
      
/*!
     \brief:
      LIDT - Load Interrupt Descriptor Table 
  */
void CCPU:: (lidt)
{    
	if (OperandSize ==  16
	{
		IDTR(Limit) = SRC[0:15];
		IDTR(Base) = SRC[16:47] AND 00FFFFFFH;
	} else { /* 32-bit Operand Size */
		IDTR(Limit) = SRC[0:15];
		IDTR(Base) = SRC[16:47];
	}
}
      
/*!
     \brief:
      LGS - Load Pointer Using GS 
 */
void CCPU:: (lfs)
{
	if ((OperandSize  ==  16)
	{
   		r16 = [Effective Address]; /* 16-bit transfer */
   		Sreg = [Effective Address + 2]; /* 16-bit transfer */
   		/* In Protected Mode, load the descriptor into the segment register */
	} else { /* OperandSize  ==  32 */
   		r32 = [Effective Address]; /* 32-bit transfer */
   		Sreg = [Effective Address + 4]; /* 16-bit transfer */
   		/* In Protected Mode, load the descriptor into the segment register */
	}
	
	if (ProtectedMode
	{ 
		if (SS is loaded
		{ 
			if (SegementSelector = null) #GP(0);

			else if (Segment selector index is not within descriptor table limits
				OR Segment selector RPL 1 CPL
				OR Access rights indicate nonwritable data segment
				OR DPL 1 CPL
				#GP(selector);
		
			else if (Segment marked not present) #SS(selector);
	
			SS = SegmentSelector(SRC);
			SS = SegmentDescriptor([SRC]);
		} else if (DS, ES, FS, or GS is loaded with non-null segment selector
		{
			if (Segment selector index is not within descriptor table limits
				OR Access rights indicate segment neither data nor readable code segment
				OR (Segment is data or nonconforming-code segment
				AND both RPL and CPL > DPL)
				#GP(selector);
			else if (Segment marked not present
				#NP(selector);

			SegmentRegister = SegmentSelector(SRC) AND RPL;
			SegmentRegister = SegmentDescriptor([SRC]);
		
		} else if (DS, ES, FS or GS is loaded with a null selector:
		{
			SegmentRegister = NullSelector; /*Clear descriptor valid bit;*/
			SegmentRegister(DescriptorValidBit) = 0; /*hidden flag; not accessible by software*/
		}
	}

	if ((Real-Address or Virtual-8086 Mode)
		SegmentRegister = SegmentSelector(SRC);
}      
/*!
     \brief:
      LLDT - Load Local Descriptor Table
  */
void CCPU:: (lldt)
{    
	if (SRC(Offset) > descriptor table limit ) #GP(segment selector);
	Read segment descriptor;
	if (SegmentDescriptor(Type) <> LDT ) #GP(segment selector);
	if (segment descriptor is not present ) #NP(segment selector);
	LDTR(SegmentSelector) = SRC;
	LDTR(SegmentDescriptor) = GDTSegmentDescriptor;
}    
/*!
     \brief:
      LMSW - Load Machine Status Word 
   */
void CCPU:: (lmsw)
{        
   CR0[0:3] = SRC[0:3];   
}
/*!
     \brief:
      LOCK - Lock Bus
	#UD If the LOCK prefix is used with an instruction not listed in the ※Description§
	section above. Other exceptions can be generated by the instruction
	that the LOCK prefix is being applied to.            
   */
void CCPU:: (lmsw)
{              
    AssertLOCK#  
}

/*!
     \brief:
      LODS - Load String
  */
void CCPU:: (lods)
{
	if (AddressSize  ==  16
		use __SI__ for source-index
	else  /* AddressSize  ==  32 */
   		use ESI for source-index;

	if ((byte load)
	{
		__AL__ = SRC; /* byte load */
		if (__DF__ == 0)
			(E)SI__ = (E)SI__ + 1;
		else 
			(E)SI__ = (E)SI__ 每 1;
	}
	} else if ((word load)
	{
		__AX__ = SRC; /* word load */
		if (__DF__ == 0)
			(E)SI__ = (E)SI__ + 2;
		else 
			(E)SI__ = (E)SI__ 每 2;
	
	} else { /* doubleword transfer */
		EAX = SRC; /* doubleword load */
		if (__DF__ == 0)
			(E)SI__ = (E)SI__ + 4;
		else 
			(E)SI__ = (E)SI__ 每 4;
	}

}
/*!
     \brief:
      LOOP - Decrement __CX__ && Loop if (__CX__ Not Zero
 */
void CCPU:: (loop)
{
	if (AddressSize ==  32
		Count is ECX;
	else  /* AddressSize ==  16 */
		Count is __CX__;

	Count = Count 每 1;

	if ((Count == 0)
		BranchCond = 1;
	else  
		BranchCond = 0;
	
	if (BranchCond = 1
	{
		EIP = EIP + SignExtend(DEST);
		if (OperandSize == 16
		{
			EIP = EIP AND 0000FFFFH;
		}
	} else {
		Terminate loop and continue program execution at EIP;
	}
}
/*!
     \brief:
      LOOPE/LOOPZ - Loop while (Equal / Loop while (Zero
  */
void CCPU:: (loope)
{
	if (AddressSize ==  32
		Count is ECX;
	else  /* AddressSize ==  16 */
		Count is __CX__;

	Count = Count 每 1;

	if ((instruction = LOOPE) OR (instruction = LOOPZ)
	{
		if ((__ZF__ =1) AND (Count == 0)
			BranchCond = 1;
		else  
			BranchCond = 0;
	}
	
	if (BranchCond = 1
	{
		EIP = EIP + SignExtend(DEST);
		if (OperandSize ==  16
			EIP = EIP AND 0000FFFFH;

	} else 
		Terminate loop and continue program execution at EIP;
}      
/*!
     \brief:
      LOOPNZ/LOOPNE - Loop while (Not Zero / Loop while (Not Equal
    */
void CCPU:: (loopnz)
{
	if (AddressSize ==  32
		Count is ECX;
	else  /* AddressSize ==  16 */
		Count is __CX__;

	Count = Count 每 1;

	if ((__ZF__ =0 ) AND (Count 1 0)
	{ 
		BranchCond = 1;
	} else { 
		BranchCond = 0;
	}
	
	if (BranchCond = 1
	{
		EIP = EIP + SignExtend(DEST);
		if (OperandSize ==  16
			EIP = EIP AND 0000FFFFH;
	} else 
		Terminate loop and continue program execution at EIP;
}    
/*!
     \brief:
      LSL - Load Segment Limit 
 */
void CCPU:: (lsl)
{
	if (SRC(Offset) > descriptor table limit
		__ZF__ = 0;
	Read segment descriptor;
	if (SegmentDescriptor(Type) 1 conforming code segment
		AND (CPL > DPL) OR (RPL > DPL)
		OR Segment type is not valid for instruction
		__ZF__ = 0
	else {
		temp = SegmentLimit([SRC]);
		if ((G = 1)
			temp = ShiftLeft(12, temp) OR 00000FFFH;

		if (OperandSize ==  32)
			DEST = temp;
		else /*OperandSize ==  16*/
			DEST = temp AND FFFFH;
	}
}      
/*!
     \brief:
      LSS - Load Pointer Using SS
      
/*!
     \brief:
      LTR - Load Task Register  
      0F 00
	#GP(0) If the current privilege level is not 0.      
 */
void CCPU:: (ltr)
{ 
	if (SRC(Offset) > descriptor table limit OR if (SRC(type) 1 global) #GP(segment selector);

	Read segment descriptor;
	if (segment descriptor is not for an available TSS ) #GP(segment selector);
	if (segment descriptor is not present ) #NP(segment selector);
	TSSsegmentDescriptor(busy) = 1;
	/* Locked read-modify-write operation on the entire descriptor when setting busy flag */
	TaskRegister(SegmentSelector) = SRC;
	TaskRegister(SegmentDescriptor) = TSSSegmentDescriptor;
}
/*!
     \brief:
      MOV - Move Byte || Word


MOV〞Move to/from Control Registers (Continued)

	DEST = SRC;

MOV〞Move to/from Debug Registers
Protected Mode Exceptions
#GP(0) If the current privilege level is not 0.
#UD If the DE (debug extensions) bit of CR4 is set and a MOV instruction is
executed involving DR4 or DR5.
#DB If any debug register is accessed while the GD flag in debug register DR7
is set.
*/
void CCPU:: (mov)
{
	if (((DE = 1) and (SRC or DEST = DR4 or DR5))
		#UD;
	else 
		DEST = SRC;
}

/*!
     \brief:
      MOVS - Move String 
 */
void CCPU:: (movs)
{
	DEST = SRC;
	if ((byte move)
	{ 
		if (__DF__ == 0)
		{
			(E)SI__ = (E)SI__ + 1;
			(E)DI__ = (E)DI__ + 1;
		} else {
			(E)SI__ = (E)SI__ 每 1;
			(E)DI__ = (E)DI__ 每 1;
		}
	} else if ((word move)
	{		
		if (__DF__ == 0)
		{
			(E)SI__ = (E)SI__ + 2;
			(E)DI__ = (E)DI__ + 2;
		} else {
			(E)SI__ = (E)SI__ 每 2;
			(E)DI__ = (E)DI__ 每 2;
		}
	} else {/* doubleword move*/
		if (__DF__ == 0)
		{
			(E)SI__ = (E)SI__ + 4;
			(E)DI__ = (E)DI__ + 4;
		} else {
			(E)SI__ = (E)SI__ 每 4;
			(E)DI__ = (E)DI__ 每 4;
		}
	}
}
/*!
     \brief:
      MOVSX - Move with Sign Extend 
  */
void CCPU:: (movsx)
{      
	DEST = SignExtend(SRC);      
}      
/*!
     \brief:
      MOVZX - Move with Zero Extend
 */
void CCPU:: (movzx)
{      
	DEST = ZeroExtend(SRC);      
}
/*!
     \brief:
      MUL - Unsigned Multiply
 */
void CCPU:: (mul)
{
	if (byte operation
		__AX__ = __AL__ * SRC
	else if (OperandSize ==  16/* word or doubleword operation */
	
		__DX__:__AX__ = __AX__ * SRC
	else /* OperandSize ==  32 */
		EDX:EAX = EAX * SRC
}

/*!
     \brief:
      NEG - Two's Complement Negation
 */
void CCPU:: (neg)
{
	if (DEST = 0
		__CF__ = 0
	else 
		__CF__ = 1;

	DEST = 每 (DEST)
}

/*!
     \brief:
      NOP - No operation
 */
void CCPU:: (nop)
{}
/*!
     \brief:
      NOT - One's Compliment Negation
 */
void CCPU:: (not)
{      
	DEST = NOT SRC;	
}
/*!
     \brief:
      OR - Inclusive Logical OR
 */
void CCPU:: (or)
{
	DEST = DEST OR SRC;
}      
      
/*!
     \brief:
      OUT - Output Data to Port

Notes
   *if (CPL < ==  IOPL
  **if (__CPL__ > __IOPL__ || if (in virtual 8086 mode
*/
void CCPU:: (out)
{
	if (((__PE__ == 1) AND ((__CPL__ > __IOPL__) OR (____VM____ == 1)))
	{ /* Protected mode with __CPL__ > __IOPL__ or virtual-8086 mode */
		if ((Any I/O Permission Bit for I/O port being accessed = 1)
		 /* I/O operation is not allowed */
			#GP(0);
		else /* I/O operation is allowed */
			DEST = SRC; /* Writes to selected I/O port */
	
	} else {/*Real Mode or Protected Mode with CPL <= IOPL */
		DEST = SRC; /* Writes to selected I/O port */
	}
}
/*!
     \brief:
      OUTS - Output String to Port
Notes
   *if (CPL < ==  IOPL
  **if (__CPL__ > __IOPL__ || if (in virtual 8086 mode
*/
void CCPU:: (outs)
{
	if (((__PE__ == 1) AND ((__CPL__ > __IOPL__) OR (____VM____ == 1)))
	{ /* Protected mode with __CPL__ > __IOPL__ or virtual-8086 mode */
		if ((Any I/O Permission Bit for I/O port being accessed = 1)
			 /* I/O operation is not allowed */
			#GP(0);
		else ( * I/O operation is allowed */
			DEST = SRC; /* Writes to I/O port */
	
	} else {(Real Mode or Protected Mode with CPL <= IOPL */
		DEST = SRC; /* Writes to I/O port */
	}
	
	if ((byte transfer)
	{ 
		if (__DF__ == 0)
			(E)SI__ = (E)SI__ + 1;
		else 
			(E)SI__ = (E)SI__ 每 1;
	} else {if (word transfer)
		if (__DF__ == 0)
			(E)SI__ = (E)SI__ + 2;
		else 
			(E)SI__ = (E)SI__ 每 2;
	} else {/* doubleword transfer */
	 
	 	if (__DF__ == 0)
			(E)SI__ = (E)SI__ + 4;
		else 
			(E)SI__ = (E)SI__ 每 4;
	}
}
/*!
     \brief:
      POP - Pop Word off Stack
 */
void CCPU:: (pop)
{
	if (StackAddrSize  ==  16
	{
   		if (OperandSize  ==  16
   		{
      			DEST = (SS:__SP__); /* copy a word */
      			__SP__ = __SP__ + 2;
   		} else { /* OperandSize  ==  32 */
      			DEST = (SS:__SP__); /* copy a dword */
      			__SP__ = __SP__ + 4;
   		}
	} else { /* StackAddrSize  ==  32 */
   		if (OperandSize  ==  16
   		{
      			DEST = (SS:ESP); /* copy a word */
      			ESP = ESP + 2;
   		} else { /* OperandSize  ==  32 */
      			DEST = (SS:ESP); /* copy a dword */
      			ESP = ESP + 4;
      		}
   	}

	if (SS is loaded;
	{
		if (segment selector is null) #GP(0);
		if (segment selector index is outside descriptor table limits
			OR segment selector's RPL 1 CPL
			OR segment is not a writable data segment
			OR DPL 1 CPL
			#GP(selector);
	
		if (segment not marked present) #SS(selector);
		else {
			SS = segment selector;
			SS = segment descriptor;
		}
	}
	if (DS, ES, FS or GS is loaded with non-null selector;
	{
		if (segment selector index is outside descriptor table limits
			OR segment is not a data or readable code segment
			OR ((segment is a data or nonconforming code segment)
			AND (both RPL and CPL > DPL))
			#GP(selector);
		if (segment not marked present) #NP(selector);
		else {
			SegmentRegister = segment selector;
			SegmentRegister = segment descriptor;
		}
	}
	if (DS, ES, FS or GS is loaded with a null selector;
	{
		SegmentRegister = segment selector;
		SegmentRegister = segment descriptor;
	}
}
/*!
     \brief:
      POPA/POPAD - Pop All Registers onto Stack
 */
void CCPU:: (popa)
{
	if (OperandSize  ==  16 /* instruction  ==  POPA */
	{
   		__DI__ = Pop();
   		__SI__ = Pop();
   		__BP__ = Pop();
   		throwaway = Pop (); /* Skip __SP__ */
   		__BX__ = Pop();
   		__DX__ = Pop();
   		__CX__ = Pop();
   		__AX__ = Pop();
	} else { /* OperandSize  ==  32, instruction  ==  POPAD */
   		EDI = Pop();
   		ESI = Pop();
   		EBP = Pop();
   		throwaway = Pop (); /* Skip ESP */
   		EBX = Pop();
   		EDX = Pop();
   		ECX = Pop();
   		EAX = Pop();
   	}
}
/*!
     \brief:
      POPF/POPFD - Pop Flags off Stack
 */
void CCPU:: (push)
{      
	if (__VM__=0 /* Not in Virtual-8086 Mode */
	{ 
		if (CPL=0
		{
			if (OperandSize ==  32);
				EFLAGS = Pop();
				/* All non-reserved flags except VIP, VIF, and __VM__ can be modified; */
				/* VIP and Vif (are cleared; __VM__ is unaffected*/
			else /* OperandSize ==  16 */
				EFLAGS[15:0] = Pop(); /* All non-reserved flags can be modified; */
		
		} else {/* CPL > 0 */
			if (OperandSize ==  32);
				EFLAGS = Pop()
				/* All non-reserved bits except IOPL, VIP, and Vif (can be modified; */
				/* IOPL is unaffected; VIP and Vif (are cleared; __VM__ is unaffected */
			else /* OperandSize ==  16 */
				EFLAGS[15:0] = Pop();
				/* All non-reserved bits except IOPL can be modified */
				/* IOPL is unaffected */
		}
	} else if (IOPL=3/* In Virtual-8086 Mode */
	{ 
		if (OperandSize ==  32)
			EFLAGS = Pop()
			/* All non-reserved bits except __VM__, __RF__, IOPL, VIP, and Vif (*/
			/* can be modified; __VM__, __RF__, IOPL, VIP, and Vif (are unaffected */
		 else 
			EFLAGS[15:0] = Pop()
			/* All non-reserved bits except IOPL can be modified */
			/* IOPL is unaffected */

	} else /* IOPL < 3 */
		#GP(0); /* trap to virtual-8086 monitor */
}
/*!
     \brief:
      PUSH - Push Word onto Stack
 */
void CCPU:: (push)
{
	if (StackAddrSize  ==  16
	{
   		if (OperandSize  ==  16 {
      			__SP__ = __SP__ - 2;
      			(SS:__SP__) = (SOURCE); /* word assignment */
   		} else {
      			__SP__ = __SP__ - 4;
      			(SS:__SP__) = (SOURCE); /* dword assignment */
   		}
	} else { /* StackAddrSize  ==  32 */
   		if (OperandSize  ==  16
   		{
      			ESP = ESP - 2;
      			(SS:ESP) = (SOURCE); /* word assignment */
   		} else {
      			ESP = ESP - 4;
      			(SS:ESP) = (SOURCE); /* dword assignment */
   		}
   	}
}

*/
/*!
     \brief:
      PUSHA/PUSHAD - Push All Registers onto Stack
 */
void CCPU:: (pusha)
{
	if (OperandSize  ==  16 /* PUSHA instruction */
	{
   		Temp = (__SP__);
   		Push(__AX__);
   		Push(__CX__);
   		Push(__DX__);
   		Push(__BX__);
   		Push(Temp);
   		Push(__BP__);
   		Push(__SI__);
   		Push(__DI__);
	} else { /* OperandSize  ==  32, PUSHAD instruction */
   		Temp = (ESP);
   		Push(EAX);
   		Push(ECX);
   		Push(EDX);
   		Push(EBX);
   		Push(Temp);
   		Push(EBP);
   		Push(ESI);
   		Push(EDI);
   	}
}
/*!
     \brief:
      PUSHF/PUSHFD - Push Flags onto Stack
*/
void CCPU:: (pushf)
{      
	if (( __PE__==0 )) OR (__PE__=1 AND ((__VM__=0) OR __VM__ == 1AND IOPL=3)))
	/* Real-Address Mode, Protected mode, or Virtual-8086 mode with IOPL equal to 3 */
	{
		if (OperandSize ==  32)
			push(EFLAGS AND 00FCFFFFH);
			/* __VM__ and __RF__ EFLAG bits are cleared in image stored on the stack*/
		else 
			push(EFLAGS); /* Lower 16 bits only */
	} else /* In Virtual-8086 Mode with IOPL less than 0 */
		#GP(0); /* Trap to virtual-8086 monitor */
}      
/*!
     \brief:
      RCL - Rotate Through Carry Left
 */
void CCPU:: (rcl)
{
	SIZE = OperandSize
	CASE (determine count) __OF__
	SIZE = 8: tempCOUNT = (COUNT AND 1FH) MOD 9;
	SIZE = 16: tempCOUNT = (COUNT AND 1FH) MOD 17;
	SIZE = 32: tempCOUNT = COUNT AND 1FH;
	ESAC;


	WHILE (tempCOUNT 1 0)
	{
		tempCF = MSB(DEST);
		DEST = (DEST * 2) + __CF__;
		__CF__ = tempCF;
		tempCOUNT = tempCOUNT 每 1;
	}

	if (COUNT = 1
		__OF__ = MSB(DEST) XOR __CF__;
	else 
		__OF__ is undefined;
}
/*!
     \brief:
      RCR - Rotate Through Carry Right
  */
  
/* RCL and RCR instructions */
void CCPU:: (rcl)
{
	SIZE = OperandSize
	CASE (determine count) __OF__
	SIZE = 8: tempCOUNT = (COUNT AND 1FH) MOD 9;
	SIZE = 16: tempCOUNT = (COUNT AND 1FH) MOD 17;
	SIZE = 32: tempCOUNT = COUNT AND 1FH;
	ESAC;


	/* RCR instruction operation */
	if (COUNT == 1)
		__OF__ = MSB(DEST) XOR __CF__;
	else 
		__OF__ is undefined;

	WHILE (tempCOUNT <> 0)
	{
		tempCF = LSB(SRC);
		DEST = (DEST / 2) + (__CF__ * 2SIZE);
		__CF__ = tempCF;
		tempCOUNT = tempCOUNT 每 1;
	}
 }
/*!
     \brief:
      RDMSR〞Read from Model Specific Register
      0F 32
  */ 
void CCPU:: (rdtsc)
{  
	EDX:EAX = MSR[ECX]; 
}  
/*!
     \brief:
      RDPMC〞Read Performance-Monitoring Counters
      0F 33
  */ 
void CCPU:: (rdtsc)
{      
	if ((ECX = 0 OR 1) AND ((CR4.PCE = 1) OR ((CR4.PCE = 0) AND (CPL=0)))
		EDX:EAX = PMC[ECX];
	} else {/* ECX is not 0 or 1 and/or CR4.PCE is 0 and CPL is 1, 2, or 3 */
		#GP(0)
}
/*!
     \brief:
      RDTSC〞Read Time-Stamp Counter
      0F 31
  */ 
void CCPU:: (rdtsc)
{      
	if ((CR4.TSD = 0) OR ((CR4.TSD = 1) AND (CPL=0))
		EDX:EAX = TimeStampCounter;
	} else {/* CR4 is 1 and CPL is 1, 2, or 3 */
		#GP(0)
}
  
/*!
     \brief:
      REP - Repeat String void CCPU:: (
      REPE/REPZ - Repeat Equal / Repeat Zero
      REPNE/REPNZ - Repeat Not Equal / Repeat Not Zero
  */
void CCPU:: (repe)
{      
	if (AddressSize ==  16
		use __CX__ for CountReg;
	} else {/* AddressSize ==  32 */
		use ECX for CountReg;

	WHILE CountReg <> 0
	{
		service pending interrupts (if (any);
		execute associated string instruction;
		CountReg = CountReg 每 1;
		if (CountReg = 0
			break;
		}
		if ((repeat prefix is REPZ or REPE) AND (ZF=0)
		OR (repeat prefix is REPNZ or REPNE) AND (ZF=1)
			break;

	}
}      
/*!
     \brief:
      RET/RE__TF__ - Return From Procedure
 */
void CCPU:: (ret)
{
	/* Near return */
	if (instruction = near return
	{
		if (OperandSize ==  32)
		{
			if (top 12 bytes of stack not within stack limits) #SS(0);
			EIP = Pop();
		else /* OperandSize ==  16 */
			if (top 6 bytes of stack not within stack limits) #SS(0)
			tempEIP = Pop();
			tempEIP = tempEIP AND 0000FFFFH;
			
			if (tempEIP not within code segment limits) #GP(0); 
			EIP = tempEIP;
		}
		
		if (instruction has immediate operand
		{
			if (StackAddressSize=32
				ESP = ESP + SRC; /* release parameters from stack */
			} else {/* StackAddressSize=16 */
				__SP__ = __SP__ + SRC; /* release parameters from stack */
		}
	}
	
	/* Real-address mode or virtual-8086 mode */
	if (((__PE__ == 0) OR (__PE__ == 1 AND ____VM____ == 1)) AND instruction = far return
	{
		if (OperandSize ==  32)
			if (top 12 bytes of stack not within stack limits ) #SS(0);
			EIP = Pop();
			CS = Pop(); /* 32-bit pop, high-order 16-bits discarded */
		} else {/* OperandSize ==  16 */
			if (top 6 bytes of stack not within stack limits ) #SS(0);
			tempEIP = Pop();
			tempEIP = tempEIP AND 0000FFFFH;
			if (tempEIP not within code segment limits ) #GP(0);
			EIP = tempEIP;
			CS = Pop(); /* 16-bit pop */
	
		if (instruction has immediate operand
			__SP__ = __SP__ + (SRC AND FFFFH); /* release parameters from stack */

	}
	
	/* Protected mode, not virtual-8086 mode */
	if ((__PE__ == 1 AND __VM__ = 0) AND instruction = far RET
	{
		if (OperandSize ==  32)
			if (second doubleword on stack is not within stack limits ) #SS(0);
		} else {/* OperandSize ==  16 */
			if (second word on stack is not within stack limits ) #SS(0);

		if (return code segment selector is null { GP(0);
		if (return code segment selector addrsses descriptor beyond diescriptor table limit, GP(selector;
		Obtain descriptor to which return code segment selector points from descriptor table
		if (return code segment descriptor is not a code segment ) #GP(selector);
		if (return code segment selector RPL < CPL ) #GP(selector);
		if (return code segment descriptor is conforming
			AND return code segment DPL > return code segment selector RPL
			#GP(selector);
		if (return code segment descriptor is not present ) #NP(selector); FI:
		if (return code segment selector RPL > CPL
			GOTO RETURN-OUTER-PRIVILEGE-LEVEL;
		} else {
			GOTO RETURN-TO-SAME-PRIVILEGE-LEVEL
	}
	
	
RETURN-SAME-PRIVILEGE-LEVEL:
	if (the return instruction pointer is not within ther return code segment limit) #GP(0);

	if (OperandSize ==  32)
	{
		EIP = Pop();
		CS = Pop(); /* 32-bit pop, high-order 16-bits discarded */
		ESP = ESP + SRC; /* release parameters from stack */
	} else {/* OperandSize=16 */
		EIP = Pop();
		EIP = EIP AND 0000FFFFH;
		CS = Pop(); /* 16-bit pop */
		ESP = ESP + SRC; /* release parameters from stack */
	}
RETURN-OUTER-PRIVILEGE-LEVEL:
	if (top (16 + SRC) bytes of stack are not within stack limits (OperandSize ==  32))
		OR top (8 + SRC) bytes of stack are not within stack limits (OperandSize=16)
		#SS(0); 

	Read return segment selector;
	if (stack segment selector is null ) #GP(0);
	if (return stack segment selector index is not within its descriptor table limits
		#GP(selector);
	Read segment descriptor pointed to by return segment selector;
	if (stack segment selector RPL 1 RPL of the return code segment selector
		OR stack segment is not a writable data segment
		OR stack segment descriptor DPL 1 RPL of the return code segment selector
		#GP(selector);
	if (stack segment not present ) #SS(StackSegmentSelector);
	if (the return instruction pointer is not within the return code segment limit ) #GP(0); FI:
	CPL = ReturnCodeSegmentSelector(RPL);
	
	if (OperandSize ==  32)
	{
		EIP = Pop();
		CS = Pop(); /* 32-bit pop, high-order 16-bits discarded */
		/* segment descriptor information also loaded */
		CS(RPL) = CPL;
		ESP = ESP + SRC; /* release parameters from called procedure＊s stack */
		tempESP = Pop();
		tempSS = Pop(); /* 32-bit pop, high-order 16-bits discarded */
		/* segment descriptor information also loaded */
		ESP = tempESP;
		SS = tempSS;
	} else {/* OperandSize=16 */
		EIP = Pop();
		EIP = EIP AND 0000FFFFH;
		CS = Pop(); /* 16-bit pop; segment descriptor information also loaded */
		CS(RPL) = CPL;
		ESP = ESP + SRC; /* release parameters from called procedure＊s stack */
		tempESP = Pop();
		tempSS = Pop(); /* 16-bit pop; segment descriptor information also loaded */
		/* segment descriptor information also loaded */
		ESP = tempESP;
		SS = tempSS;
	}
	
	FOR each of segment register (ES, FS, GS, and DS)
	{
		if (segment register points to data or non-conforming code segment
			AND CPL > segment descriptor DPL; /* DPL in hidden part of segment register */
			/* segment register invalid */
			SegmentSelector = 0; /* null segment selector */

	}
	
	For each of ES, FS, GS, and DS
	{
		if (segment selector index is not within descriptor table limits
			OR segment descriptor indicates the segment is not a data or
			readable code segment
			OR if (the segment is a data or non-conforming code segment and the segment
			descriptor＊s DPL < CPL or RPL of code segment＊s segment selector

			segment selector register = null selector;
	}
	ESP = ESP + SRC; /* release parameters from calling procedure＊s stack */
}
/*!
     \brief:
      ROL - Rotate Left
*/
void CCPU:: (rol)
{
	SIZE = OperandSize
	CASE (determine count) __OF__
	SIZE = 8: tempCOUNT = COUNT MOD 8;
	SIZE = 16: tempCOUNT = COUNT MOD 16;
	SIZE = 32: tempCOUNT = COUNT MOD 32;
	ESAC;      
/* ROL instruction operation */
	WHILE (tempCOUNT <> 0)
	{
		tempCF = MSB(DEST);
		DEST = (DEST * 2) + tempCF;
		tempCOUNT = tempCOUNT 每 1;
	}

	__CF__ = LSB(DEST);
	if (COUNT == 1)
		__OF__ = MSB(DEST) XOR __CF__;
	else 
		__OF__ is undefined;
}
/*!
     \brief:
      ROR - Rotate Right
*/
void CCPU:: (ror)
{
	SIZE = OperandSize
	CASE (determine count) __OF__
	SIZE = 8: tempCOUNT = COUNT MOD 8;
	SIZE = 16: tempCOUNT = COUNT MOD 16;
	SIZE = 32: tempCOUNT = COUNT MOD 32;
	ESAC;
      
	/* ROR instruction operation */
	WHILE (tempCOUNT <> 0)
	{
		tempCF = LSB(SRC);
		DEST = (DEST / 2) + (tempCF * 2SIZE);
		tempCOUNT = tempCOUNT 每 1;
	}

	__CF__ = MSB(DEST);
	if (COUNT == 1)
		__OF__ = MSB(DEST) XOR MSB - 1(DEST);
	else 
		__OF__ is undefined;
}
      
/*!
     \brief:      
	RSM〞Resume from System Management Mode      
	0F AA
	Protected Mode Exceptions
	#UD If an attempt	
*/
void CCPU:: (rsm)
{
	ReturnFromSSM;
	ProcessorState = Restore(SSMDump);	 	
}

/*!
     \brief:
      S__AL__/SHL - Shift Arithmetic Left / Shift Logical Left
 */
void CCPU:: (shl)
{
 	tempCOUNT = (COUNT AND 1FH);
	tempDEST = DEST;
	WHILE (tempCOUNT <> 0)
	{
		__CF__ = MSB(DEST);
	
		DEST = DEST * 2;
		tempCOUNT = tempCOUNT 每 1;
	}
	
	/* Determine overflow for the various instructions */
	if (COUNT == 1)
		__OF__ = MSB(DEST) XOR __CF__;
	else if (COUNT == 0)
		All flags remain unchanged;
	else /* COUNT neither 1 or 0 */
		__OF__ = undefined;
}
	
/*!
     \brief:
      SAR - Shift Arithmetic Right     
 */
void CCPU:: (sar)
{
 	tempCOUNT = (COUNT AND 1FH);
	tempDEST = DEST;
	WHILE (tempCOUNT <> 0)
	{
		__CF__ = LSB(DEST);
		
		DEST = DEST / 2 /*Signed divide, rounding toward negative infinity*/;
		tempCOUNT = tempCOUNT 每 1;
	}
	
	/* Determine overflow for the various instructions */
	if (COUNT == 1)
		__OF__ = 0;
	else if (COUNT == 0)
		All flags remain unchanged;
	else /* COUNT neither 1 or 0 */
		__OF__ = undefined;
}
/*!
     \brief:
      SBB - Subtract with Borrow
 */
void CCPU:: (sbb)
{
	DEST = src1 - src2 - __CF__;
}      
/*!
     \brief:
      SCAS - Scan String 
 */
void CCPU:: (scas)
{
	if (byte type of instruction
	{
   		__AL__ - [dest-index]; /* Compare byte in __AL__ && dest */
   		if (__DF__  ==  0 )
   			IndDec = 1 
   		else  
   			IncDec = -1; 
	} else  if (OperandSize  ==  16)
   	{
      		__AX__ - [dest-index]; /* compare word in __AL__ && dest */
      		if (__DF__  ==  0 )
      			IncDec = 2 
      		else  
      			IncDec = -2; 
   	} else { /* OperandSize  ==  32 */
      		EAX - [dest-index];/* compare dword in EAX & dest */
      		if (__DF__  ==  0 )
      			IncDec = 4 
      		else  
      			IncDec = -4; 
   	}

	dest-index = dest-index + IncDec
}

/*!
     \brief:
      SETAE/SETNB - Set if (Above || Equal / Set if (Not Below
  */
void CCPU:: (setae)
{         
	if (NB)
		DEST = 1
	else
	 	DEST = 0;
}
      
/*!
     \brief:
      SETB/SETNAE - Set if (Below / Set if (Not Above || Equal
  */
void CCPU:: (setb)
{         
	if (B)
		DEST = 1
	else
	 	DEST = 0;
}      
/*!
     \brief:
      SETBE/SETNA - Set if (Below || Equal / Set if (Not Above
  */
void CCPU:: (setbe)
{         
	if (BE)
		DEST = 1
	else
	 	DEST = 0;
}      
/*!
     \brief:
      SETE/SETZ - Set if (Equal / Set if (Zero
  */
void CCPU:: (sete)
{         
	if (ZF)
		DEST = 1
	else
	 	DEST = 0;
}      
/*!
     \brief:
      SETNE/SETNZ - Set if (Not Equal / Set if (Not Zero
  */
void CCPU:: (setnz)
{         
	if (NZ)
		DEST = 1
	else
	 	DEST = 0;
}      
/*!
     \brief:
      SETL/SETNGE - Set if (Less / Set if (Not Greater || Equal 
  */
void CCPU:: (setl)
{         
	if (NGE)
		DEST = 1
	else
	 	DEST = 0;
}      
/*!
     \brief:
      SETGE/SETNL - Set if (Greater || Equal / Set if (Not Less
  */
void CCPU:: (setnl)
{         
	if (NL)
		DEST = 1
	else
	 	DEST = 0;
}      
/*!
     \brief:
      SETLE/SETNG - Set if (Less || Equal / Set if (Not greater || Equal
  */
void CCPU:: (setle)
{         
	if (LE)
		DEST = 1
	else
	 	DEST = 0;
}      
/*!
     \brief:
      SETG/SETNLE - Set if (Greater / Set if (Not Less || Equal
  */
void CCPU:: (setg)
{         
	if (G)
		DEST = 1
	else
	 	DEST = 0;
}      
/*!
     \brief:
      SETS - Set if (Signed
  */
void CCPU:: (sets)
{         
	if (S)
		DEST = 1
	else
	 	DEST = 0;
}      
/*!
     \brief:
      SETNS - Set if (Not Signed
  */
void CCPU:: (setns)
{         
	if (NS)
		DEST = 1
	else
	 	DEST = 0;
}      
/*!
     \brief:
      SETC - Set if (Carry
  */
void CCPU:: (setc)
{         
	if (C)
		DEST = 1
	else
	 	DEST = 0;
}      
/*!
     \brief:
      SETNC - Set if (Not Carry
  */
void CCPU:: (setnc)
{         
	if (NC)
		DEST = 1
	else
	 	DEST = 0;
}      
/*!
     \brief:
      SETO - Set if (Overflow
  */
void CCPU:: (seto)
{         
	if (O)
		DEST = 1
	else
	 	DEST = 0;
}      
/*!
     \brief:
      SETNO - Set if (Not Overflow
  */
void CCPU:: (setno)
{         
	if (NO)
		DEST = 1
	else
	 	DEST = 0;
}      
/*!
     \brief:
      SETP/SETPE - Set if (Parity / Set if (Parity Even
  */
void CCPU:: (setp)
{         
	if (P)
		DEST = 1
	else
	 	DEST = 0;
}      
/*!
     \brief:
      SETNP/SETPO - Set if (No Parity / Set if (Parity Odd
  */
void CCPU:: (setnp)
{         
	if (NP)
		DEST = 1
	else
	 	DEST = 0;
}      
/*!
     \brief:
      SGDT - Store Global Descriptor Table
	Protected Mode Exceptions
	#UD If the destination operand is a register.      
 */
void CCPU:: (sgdt)
{ 

	 /* instruction is SGDT */
	if (OperandSize ==  16)
	{
		DEST[0:15] = GDTR(Limit);
		DEST[16:39] = GDTR(Base); /* 24 bits of base address loaded; */
		DEST[40:47] = 0;
	} else { /* 32-bit Operand Size */
		DEST[0:15] = GDTR(Limit);
		DEST[16:47] = GDTR(Base); /* full 32-bit base address loaded */
	}
}

/*!
     \brief:
      SHR - Shift Logical Right
  */   
void CCPU:: (shr)
{
 	tempCOUNT = (COUNT AND 1FH);
	tempDEST = DEST;
	WHILE (tempCOUNT <> 0)
	{
		__CF__ = LSB(DEST);
		DEST = DEST / 2 ; /* Unsigned divide */;

		tempCOUNT = tempCOUNT 每 1;
	}
	
	/* Determine overflow for the various instructions */
	if (COUNT == 1)
		__OF__ = MSB(tempDEST);
	else if (COUNT == 0)
		All flags remain unchanged;
	else /* COUNT neither 1 or 0 */
		__OF__ = undefined;
}
  
/*!
     \brief:
      SHLD/SHRD - Double Precision Shift
 */
void CCPU:: (shld)
{
	/* count is an unsigned integer corresponding to the last operand of the
	instruction, either an immediate byte || the byte in register CL */
	ShiftAmt = count % 32;
	inBits = register; /* Allow overlapped operands */
	if (ShiftAmt  ==  0
	{ 
	} else if (ShiftAmt > ==  OperandSize
   	{ /* Bad parameters */
      		r/m = UNDEFINED;
      		__CF__, __OF__, SF, ZF, __AF__, PF = UNDEFINED;
   	} else { /* Perform the shift */
      		__CF__ = BIT[Base, OperandSize - ShiftAmt];
         	/* Last bit shifted out on exit */
   		FOR i = OperandSize - 1 DOW__NT__O ShiftAmt
   		{
      			BIT[Base, i] = BIT[Base, i - ShiftAmt];
   		}
   		FOR i = ShiftAmt - 1 DOW__NT__O 0
   		{
      			BIT[Base, i] = BIT[inBits, i - ShiftAmt + OperandSize];
   		}
   		Set SF, ZF, PF (r/m);
      		/* SF, ZF, PF are set according to the value of the result */
   		__AF__ = UNDEFINED;
   	}
}

void CCPU:: (shrd)
{
	/* count is an unsigned integer corresponding to the last operand of the
	instruction, either an immediate byte || the byte in register CL */
	ShiftAmt = count % 32;
	inBits = register; /* Allow overlapped operands */
	if (ShiftAmt  ==  0
	{ 
	} else if (ShiftAmt >=  OperandSize)
	{ /* Bad parameters */
 		r/m = UNDEFINED;
      		__CF__, __OF__, SF, ZF, __AF__, PF = UNDEFINED;
   	} else { /* Perform the shift */
      		__CF__ = BIT[r/m, ShiftAmt - 1]; /* last bit shifted out on exit */
      		FOR i = 0 TO OperandSize - 1 - ShiftAmt
      		{
        		BIT[r/m, i] = BIT[r/m, i - ShiftAmt];
      		}
      		FOR i = OperandSize - ShiftAmt TO OperandSize - 1
      		{
        		BIT[r/m,i] = BIT[inBits,i+ShiftAmt - OperandSize];
      		}
      		Set SF, ZF, PF (r/m);
        		/* SF, ZF, PF are set according to the value of the result */
      		Set SF, ZF, PF (r/m);
      		__AF__ = UNDEFINED;
   	}
}

/*!
     \brief:
      SIDT - Store Interrupt Descriptor Table
 */
void CCPU:: (sidt)
{ 

	if (OperandSize ==  16)
	{
		DEST[0:15] = IDTR(Limit);
		DEST[16:39] = IDTR(Base); /* 24 bits of base address loaded; */
		DEST[40:47] = 0;
	} else { /* 32-bit Operand Size */
		DEST[0:15] = IDTR(Limit);
		DEST[16:47] = IDTR(Base); /* full 32-bit base address loaded */
	}
} 

/*!
     \brief:
      SLDT - Store Local Descriptor Table
 */
void CCPU:: (sldt)
{
	DEST = LDTR.SegmentSelector;
}
/*!
     \brief:
      SMSW - Store Machine Status Word
 */
void CCPU:: (smsw)
{
	DEST = CR0.MSW;   /* 15..0 */
}
/*!
     \brief:
      STC - Set Carry
 */
void CCPU:: (stc)
{
	__CF__ = 1;
}
/*!
     \brief:
      STD - Set Direction Flag
 */
void CCPU:: (std)
{
	__DF__ = 1;
}
/*!
     \brief:
      STI - Set Interrupt Flag
 */
void CCPU:: (sti)
{
	if ((__PE__ == 0) /* Executing in real address mode */
		__IF__ = 1; /*Set interrupt Flag */
	else /*Executing in protection mode or virtual 8086 mode*/
	if ((__VM__==0)  /*Protection mode */
	{
		if ((IOPL==3) __IF__ = 1;
		else if (CPL <= IOPL) __IF__ = 1;
		else #GP (0);
	} else /* virtual 8086 mode */
		#GP(0);
	
}
}
/*!
     \brief:
      STOS - Store String 
 */
void CCPU:: (stos)
{
	if (AddressSize  ==  16)
		use ES:__DI__ for DestReg
	else /* AddressSize  ==  32 */ 
		use ES:EDI for DestReg;

	if (byte type of instruction)
	{
		(ES:DestReg) = __AL__;
		if (__DF__  ==  0)
			DestReg = DestReg + 1;
		else  
			DestReg = DestReg - 1;
	} else if (OperandSize  ==  16
	{
		(ES:DestReg) = __AX__;
		if (__DF__  ==  0)
			DestReg = DestReg + 2;
		else  
			DestReg = DestReg - 2;
   	} else { /* OperandSize  ==  32 */
      	(ES:DestReg) = EAX;
      	if (__DF__  ==  0)
			DestReg = DestReg + 4;
      	else  
		  	DestReg = DestReg - 4;
   	}
}

/*!
     \brief:
      STR - Store Task Register
 */
void CCPU:: (str)
{
	DEST = TR.SegmentSelector;
}
/*!
     \brief:
      SUB - Subtract
 */
void CCPU:: (sub)
{
	if (SRC is a byte && DEST is a word || dword
		DEST = DEST - SignExtend(SRC);
	else  
		DEST = DEST - SRC;
}
/*!
     \brief:
      TEST - Test For Bit Pattern
 */
void CCPU:: (test)
{
	TEMP = src1 && src2;
	SF = MSB (TEMP);
	
	if (TEMP == 0) __ZF__ = 0;
	else __ZF__ = 1;

	PF = BitwiseXNOR( TEMP(0..7));
	__CF__ = 0;
	__OF__ = 0;

}
/*!
     \brief:
      VERR - Verify Read
 */
void CCPU:: (verr)
{
	if (src.offset > GDT.limit || src.offset > LDT.limit )
		__ZF__ = 0;

	Read segment descriptor;

	if ((SegmentDescriptor.Descriptortype == 0) /* system segment */
		||SegmentDescriptor.type != conforming code segment
		&& (CPL>DPL) || (RPL>DPL))
		__ZF__ = 0;
	else if (segment is readable for VERR )
		__ZF__ = 1;
}
/*!
     \brief:
      VERW - Verify Write
 */
void CCPU:: (verw)
{
	if (src.offset > GDT.limit 
		|| src.offset > LDT.limit )
		__ZF__ = 0;
	Read segment descriptor;

	if ((SegmentDescriptor.Descriptortype == 0) /* system segment */
		||SegmentDescriptor.type != conforming code segment)
		&& (CPL>DPL) || (RPL>DPL)
		__ZF__ = 0;
	else if (segment is writable for VERW)
		__ZF__ = 1;
}
/*!
     \brief:
      WAIT/FWAIT - Event Wait
 */
void CCPU:: (wait)
{
	Check for pending unmasked FP exceptions.
}
/*!
     \brief:
      WBINVD - Write-Back && Invalidate Cache
	0F 09
*/
void CCPU:: (wbinv)
{
	WriteBack (InternalCache);
	Flush (InternalCache);
	Signal (ExternalCache);
	SignalFlush (ExternalCache);
}
/*!
     \brief:
      WRMSR - Write to Model specific register
 */
void CCPU:: (wrmsr)
{
	MSR[ECX] = EDX:EAX;
}
/*!
     \brief:
      XADD - Add and exchange
 */
void CCPU:: (xadd)
{
	TEMP = DEST + SRC;
	SRC  = DEST;
	DEST = TEMP;
}


#endif


/*!
     \brief:
      LAHF - Load Register __AH__ From Flags
	__AH__ = SF:ZF:xx:__AF__:xx:PF:xx:__CF__;
 */
void CCPU::lahf_()
{
	REG_CODE reg;

	reg.reg_cat=RC_REGG8;
	reg.reg_no=AH;

	unsigned char c=(unsigned char )__CFLAGS__;
	c&=0xd5;
	update_reg(reg,c);

}


/*!
     \brief:
      SAHF - Store __AH__ Register into FLAGS
      EFLAGS(SF:ZF:0:__AF__:0:PF:1:__CF__) = __AH__;
 */
void CCPU::sahf_()
{      
	unsigned short c=__CFLAGS__;
	c&=0xff00;
	c|=__AH__;
	c&=0xffd7;
	c|=2;

	__CFLAGS__=c;
}