#include "cpu.h"
#include "log.h"
#pragma warning (disable:4018)

//__PE__==0, REAL-ADDRESS-MODE;	
void CCPU::iret_real (  int  OperandSize)
{
	unsigned short cs;
	unsigned long tempEFLAGS;

	if (OperandSize ==  32)
	{
		if (!check_stack_for_pop (12)) // if top 12 bytes of stack not within stack limits,#SS;
			SS (0);

//		if (instruction pointer not within code segment limits ) #GP(0);
		__NEXT_EIP__      = POPL();
		cs           = (unsigned short)POPL(); /* 32-bit pop, high-order 16-bits discarded */
		tempEFLAGS   = POPL();

		__EFLAGS__ = (tempEFLAGS & 0x257FD5) | (__EFLAGS__ & 0x1A0000);
		
		load_seg_real_or_v86 (CS,cs);

	} else { /* OperandSize ==  16 */
		if (!check_stack_for_pop (6)) // if top 6 bytes of stack not within stack limits,#SS;
			SS (0);

		__NEXT_EIP__		= POPW();
		     cs     = POPW(); /* 32-bit pop, high-order 16-bits discarded */
		__CFLAGS__	= POPW(); /* EFLAGS[0..15] */

		load_seg_real_or_v86 (CS,cs);
	}
}


/* __PE__==1, protected mode or virtual-8086 mode interrupt */
void CCPU::iret_protected (  int  OperandSize)
{
	unsigned int dt_base;
	unsigned short dt_limit;

	unsigned int TempEIP,TempEFLAGS;
	UEFLAGS &eflags = *(UEFLAGS *)&TempEFLAGS;

	if (__VM__ == 1) /* Virtual-8086 mode: __PE__=1, __VM__=1 */
		iret_from_v86(OperandSize);

	else if (__NT__ == 1) {/*__PE__=1, __VM__=0, __NT__=1 */
		/* __PE__=1, __VM__=1, __NT__=1:TASK-RETURN */
		iret_task( OperandSize);

	} else {
		SELECTOR	TempCS;
		if (OperandSize ==  32){
			if (!check_stack_for_pop (12)) //top 12 bytes of stack not within stack limits) 
				SS(0);            //   #SS(0)
			TempEIP				= POPL();
			MK_SHORT(TempCS)    = (unsigned short) POPL();
			TempEFLAGS			= POPL();

		} else { /* OperandSize ==  16 */
			if (!check_stack_for_pop (6))  //top 6 bytes of stack not within stack limits) 
				SS(0);            //   #SS(0)

			TempEIP				= POPW();
			MK_SHORT(TempCS)    = POPW();
			TempEFLAGS			= POPW();
			TempEIP				= TempEIP    & 0xFFFF;
			TempEFLAGS			= TempEFLAGS & 0xFFFF;
		}
	
		// elfags is a reference to TempEFLAGS
		if ( VM(eflags) == 1 && __CPL__==0 ) {
			// RETURN-TO-VIRTUAL-8086-MODE   
			iret_to_v86(TempCS,TempEIP,TempEFLAGS,OperandSize);

		} else {//PROTECTED-MODE-RETURN;
		/* __PE__=1, __VM__=0 in EFLAGS image */
			if (NULL_SELECTOR (TempCS)) GP(0); // if return code segment selector is null,GP(0);
			
			//if (return code segment selector addrsses descriptor beyond descriptor table limit) 
			dt_base  = IN_GDT(TempCS) ? gdt_base : ldt_base;
			dt_limit = IN_GDT(TempCS) ? gdt_limit: ldt_limit;
			if (SELECTOR_TOP(TempCS) > dt_limit) GP(MK_SHORT(TempCS));  //#GP(selector);

			//Read segment descriptor pointed to by the return code segment selector
			load_descriptor(dt_base,&CS_DESC,TempCS.index);


			if (!CODE_SEG (CS_DESC))    // if return code segment descriptor is not a code segment,
				GP (MK_SHORT (TempCS)); //     #GP(selector);

			if (RPL (TempCS) < __CPL__) // if return code segment selector RPL < CPL,
				GP (MK_SHORT (TempCS)); //     #GP(selector);

			if (CONFORMING (CS_DESC)          // if return code segment descriptor is conforming
				&& DPL (CS_DESC) > RPL (TempCS) ) // && return code segment DPL > return code segment selector RPL
				GP (MK_SHORT (TempCS));        //     #GP(selector);

			if (!PRESENT (CS_DESC))    // if return code segment descriptor is not present,
				NP (MK_SHORT (TempCS)); //     #NP(selector);

			if (RPL (TempCS) > __CPL__) // if return code segment selector RPL > CPL
			{// RETURN-OUTER-PRIVILEGE-LEVEL;
				iret_to_outer_level( TempCS,TempEIP,TempEFLAGS,OperandSize);
			} else  
				// RETURN-TO-SAME-PRIVILEGE-LEVEL
				/* __PE__=1, __VM__=0 in flags image, RPL=CPL */
				iret_same_level( TempCS,TempEIP,TempEFLAGS,OperandSize);
		}
	}
}

void CCPU::iret_to_v86(SELECTOR  TempCS,unsigned long TempEIP,
					   unsigned long TempEFLAGS,int OperandSize)
{
	unsigned short   TempSS;
	/* __PE__=1, __VM__=1 in EFLAGS image */
    /* Interrupted procedure was in virtual-8086 mode*/
	if (!check_stack_for_pop (24)) //top 24 bytes of stack not within stack limits) 
		SS(0);            //   #SS(0)

//			if (instruction pointer not within code segment limits) #GP(0);
	//cs_desc=CS_DESC;
	//__CS__=TempCS;
	//copy_cs_desc(TempCS,CS_DESC_NO);
	load_seg_real_or_v86 (CS,MK_SHORT(TempCS)); 
	cpl=3;

	__NEXT_EIP__=TempEIP;
	__EFLAGS__ = TempEFLAGS;

	unsigned long TempESP= POPL();
	TempSS = (unsigned short)POPL();

	___ES___ = (unsigned short)POPL(); /* pop 2 words; throw away high-order word */
	___DS___ = (unsigned short)POPL(); /* pop 2 words; throw away high-order word */
	___FS___ = (unsigned short)POPL(); /* pop 2 words; throw away high-order word */
	___GS___ = (unsigned short)POPL(); /* pop 2 words; throw away high-order word */
	
	load_seg_real_or_v86 (SS,TempSS);       // SS  = TempSS;
	__ESP__ = TempESP;     
	/* Resume execution in Virtual-8086 mode */
}
void CCPU::iret_to_outer_level(SELECTOR  TempCS,unsigned long TempEIP,unsigned long   TempEFLAGS,int OperandSize)
{
	unsigned long TempESP;
	SELECTOR TempSS;
	UEFLAGS eflags=*(UEFLAGS *)&TempEFLAGS;
	if (OperandSize ==  32)
	{
		if (!check_stack_for_pop (8)) // if top 8 bytes on stack are not within limits,
			SS (0);          //    #SS(0);

		TempESP           = POPL ();
		MK_SHORT (TempSS) = (unsigned short)POPL ();  //Read return segment selector;
	} else { /* OperandSize=16 */
		if (!check_stack_for_pop (4)) // if top 4 bytes on stack are not within limits,
			SS (0);          //    #SS(0);


		TempESP           = POPW ();
		MK_SHORT (TempSS) = POPW ();  //Read return segment selector;
	}

	if (NULL_SELECTOR (TempSS)) GP(0);// stack segment selector is null,#GP(0);

	unsigned long dt_base  = IN_GDT(TempSS) ? gdt_base : ldt_base;
	unsigned short dt_limit = IN_GDT(TempSS) ? gdt_limit: ldt_limit;
	//if (return stack segment selector index is not within its descriptor table limits,
	if (SELECTOR_TOP(TempSS) > dt_limit) GP(MK_SHORT(TempSS));   //#GP(SSselector);

	//Read segment descriptor pointed to by return segment selector;
	load_descriptor(gdt_base,&SS_DESC,TempSS.index);
	// if stack segment selector RPL <> RPL of the return code segment selector
	//	OR the stack segment descriptor does not indicate a a writable data segment;
	//	OR stack segment DPL <> RPL of the return code segment selector
	if (RPL (TempSS) != RPL (TempCS) 
		|| (!WRITEABLE(SS_DESC)) 
		||(! DATA_SEG (SS_DESC))
		|| DPL (SS_DESC) != RPL (TempCS))
		GP(MK_SHORT(TempSS));   //#GP(SSselector);
		
	if (!PRESENT (SS_DESC))       // if stack segment is not present,
		SS(MK_SHORT (TempSS)); //#SS(SS selector);

//				if (tempEIP is not within code segment limit ) #GP(0);
	
	__NEXT_EIP__ = TempEIP;
	__ESP__ = TempESP;


	copy_cs_desc (TempCS,CS_DESC_NO);
	load_ss (TempSS);

	// eflags is areference tp TempEFLAGS;
	// __EFLAGS (__CF__, PF, __AF__, ZF, SF, __TF__, __DF__, __OF__, __NT__) = TempEFLAGS
	__CF__ = CF(eflags);
	__PF__ = PF(eflags);
	__AF__ = AF(eflags);
	__ZF__ = ZF(eflags);
	__SF__ = SF(eflags);
	__TF__ = TF(eflags);
	__DF__ = DF(eflags);
	__OF__ = OF(eflags);
	__NT__ = NT(eflags);

	if (OperandSize ==  32)
	{
		//EFLAGS(__RF__, AC, ID) = TempEFLAGS;
		__RF__ = RF(eflags);
		__AC__ = AC(eflags);
		__ID__ = ID(eflags);
	}
	if (__CPL__ <= IOPL(eflags))
	{
		//EFLAGS(IF) = TempEFLAGS;
		__IF__ = IF(eflags);
	}
	if (__CPL__ == 0)
	{
		// EFLAGS(IOPL) = TempEFLAGS;
		__IOPL__ = IOPL(eflags);

		if (OperandSize ==  32)
		{
			// EFLAGS(__VM__, VIF, VIP) = TempEFLAGS;
			__VM__ = VM(eflags);
			__VIF__ = VIF(eflags);
			__VIP__ = VIP(eflags);
		}
	}

	//__CPL__ = RPL of the return code segment selector;

	//FOR each of segment register (ES, FS, GS, and DS)
	//{
	//	if (segment register points to data or non-conforming code segment
	//		AND CPL > segment descriptor DPL /* stored in hidden part of segment register */
	//	{ /* segment register invalid */
	//		SegmentSelector = 0; /* null segment selector */
	//	}
	//}
	null_invalid_sreg_selector (__DS__);
	null_invalid_sreg_selector (__FS__);
	null_invalid_sreg_selector (__GS__);
	null_invalid_sreg_selector (__ES__);
}
void CCPU::iret_same_level(SELECTOR  TempCS,unsigned long TempEIP,unsigned long  tempEFLAGS,int OperandSize)
{
	//if (EIP is not within code segment limits ) #GP(0);
	
	__NEXT_EIP__ = TempEIP;
	//__CS__  = TempCS; /* segment descriptor information also loaded */
	//cs_desc = CS_DESC;
	copy_cs_desc(TempCS,CS_DESC_NO);
	
	//EFLAGS (__CF__, PF, __AF__, ZF, SF, __TF__, __DF__, __OF__, __NT__) = tempEFLAGS;
	__CF__ = CF((*(UEFLAGS*)&tempEFLAGS));
	__PF__ = PF((*(UEFLAGS*)&tempEFLAGS));
	__AF__ = AF((*(UEFLAGS*)&tempEFLAGS));
	__ZF__ = ZF((*(UEFLAGS*)&tempEFLAGS));
	__SF__ = SF((*(UEFLAGS*)&tempEFLAGS));
	__TF__ = TF((*(UEFLAGS*)&tempEFLAGS));
	__DF__ = DF((*(UEFLAGS*)&tempEFLAGS));
	__OF__ = OF((*(UEFLAGS*)&tempEFLAGS));
	__NT__ = NT((*(UEFLAGS*)&tempEFLAGS));

	if (OperandSize ==  32)
	{
		//EFLAGS(__RF__, AC, ID) = TempEFLAGS;
		__RF__ = RF((*(UEFLAGS*)&tempEFLAGS));
		__AC__ = AC((*(UEFLAGS*)&tempEFLAGS));
		__ID__ = ID((*(UEFLAGS*)&tempEFLAGS));
	}

	if (__CPL__ <= IOPL((*(UEFLAGS*)&tempEFLAGS)))
	{
		//EFLAGS(IF) = TempEFLAGS;
		__IF__ = IF((*(UEFLAGS*)&tempEFLAGS));
	}

	if (__CPL__ == 0)
	{
		// EFLAGS(IOPL) = TempEFLAGS;
		__IOPL__ = IOPL((*(UEFLAGS*)&tempEFLAGS));

		if (OperandSize ==  32)
		{
			// EFLAGS(__VM__, VIF, VIP) = TempEFLAGS;
			__VM__  = VM((*(UEFLAGS*)&tempEFLAGS));
			__VIF__ = VIF((*(UEFLAGS*)&tempEFLAGS));
			__VIP__ = VIP((*(UEFLAGS*)&tempEFLAGS));
		}
	}

} 

// RETURN-FROM-VIRTUAL-8086-MODE:
void CCPU::iret_from_v86(int OperandSize)
{
	unsigned short TempCS;
	unsigned long TempEIP;

	/* Processor is in virtual-8086 mode when IRET is executed and stays in virtual-8086 mode */
	if (__IOPL__==3) /* Virtual mode: __PE__=1, __VM__=1, IOPL=3 */
	{ 
		if (OperandSize ==  32)
		{
			if (!check_stack_for_pop (12)) // if top 12 bytes of stack not within stack limits,#SS;
				SS (0);

			//	if (instruction pointer not within code segment limits ) #GP(0);
			TempEIP         = POPL();
			TempCS          = (unsigned short)POPL(); /* 32-bit pop, high-order 16-bits discarded */
			__EFLAGS__		= POPL();

			load_seg_real_or_v86 (CS,TempCS);
			/*__VM__,IOPL,VIP,and Vif (EFLAGS bits are not modified by pop */
		} else { /* OperandSize ==  16 */
			if (!check_stack_for_pop (12)) // if top 6 bytes of stack not within stack limits,#SS;
				SS (0);

			//if (instruction pointer not within code segment limits ) #GP(0);
			TempEIP     	= POPW();
			TempCS          = POPW(); /* 32-bit pop, high-order 16-bits discarded */
			__CFLAGS__		= POPW(); /* EFLAGS[0..15], IOPL not changed */

			load_seg_real_or_v86 (CS,TempCS);
		}

		__NEXT_EIP__	=TempEIP;
	}
}



void CCPU::iret_task(int OperandSize)
{
		unsigned short old_tss = ___TR___;
		SELECTOR tss;

		// Read segment selector in link field of current TSS;
		 MK_SHORT(tss) =MEM_READW (BASE (tr_desc) + OFFSET(TSS,link) );

		 if (IN_LDT (tss)               // if local/global bit is set to local
			|| tss.index > gdt_limit ) //   OR index not within GDT limits, 
			GP (MK_SHORT (tss));       //   #GP(TSS selector);
 
		// Access TSS for task specified in link field of current TSS;
		load_descriptor(gdt_base,&TSS_DESC,tss.index); 

		if (TSS_GATE (TSS_DESC)              // if TSS descriptor type is not TSS 
			|| GATE_BUSYTSS (TSS_DESC))    //  or the TSS is marked not busy,
			GP (MK_SHORT (tss));       //   #GP(TSS selector);

		if (!PRESENT (TSS_DESC))           // if TSS not present,
			NP (MK_SHORT (tss));       //   #NP(TSS selector);

		//SWITCH-TASKS (without nesting) to TSS specified in link field of current TSS;
		switch_task(tss,TSS_DESC_NO,false);

		//Mark the task just abandoned as NOT BUSY;
		unsigned short type=MEM_READW (gdt_base +(old_tss&0xfff0)+6 );
		type &= ~2; 
		MEM_WRITEW (gdt_base +(old_tss&0xfff0)+6,type );

//		if (EIP is not within code segment limit) #GP(0);
}

//IF PE==0
//THEN 
//GOTO REAL-ADDRESS-MODE:;
//ELSE 
//GOTO PROTECTED-MODE;
//FI;
void CCPU::iret_(int OperandSize)
{

	//log("IRET\n");
	if (__PE__ == 0)
	{
		iret_real(OperandSize);
	}
	else
		iret_protected(OperandSize);

	sync_fetch_eip(__NEXT_EIP__);
}