#include "cpu.h"

#pragma warning (disable:4018)

void CCPU::near_rel_call (int offset,int OperandSize) 
{
	address_t __eip__;

	if (OperandSize ==  32) /* DEST is rel32 */
			__eip__ = (__NEXT_EIP__ + offset) ;
	else
            __eip__ = ((__NEXT_EIP__ + offset) & 0xffff);
	// the instruction pointer is not within code segment limit
	if (__eip__ > LIMIT (cs_desc) ) GP(0); 

	if (OperandSize ==  32) {/* DEST is rel32 */
			// stack not large enough for a 4-byte return address
			if ( !check_stack_for_push(4) ) SS(0); 
			Pushl(__NEXT_EIP__); 
			
	} else { /* OperandSize ==  16 */ /* DEST is rel16 */
		    // stack not large enough for a 2-byte return address
			if (! check_stack_for_push(2) ) SS(0); 
			Pushw(__NEXT_EIP__);
	}

	__NEXT_EIP__ = __eip__;
	sync_fetch_eip(__eip__);
}

/* DEST is r/m32 */
void CCPU::near_abs_call (address_t __eip__ ,int OperandSize)
{
	//the instruction pointer is not within code segment limit
	if (__eip__ > LIMIT (cs_desc)) GP(0); 

	if (OperandSize ==  32)
	{
		// stack not large enough for a 4-byte return address
		if ( !check_stack_for_push(4) ) SS(0); 
		Pushl(__NEXT_EIP__); 

	} else { /* OperandSize ==  16 */
		// stack not large enough for a 2-byte return address
		if ( !check_stack_for_push(2) ) SS(0); 
		Pushw(__NEXT_EIP__); 
		__eip__ = __eip__ & 0xFFFF; /* DEST is r/m16 */
	}
	__NEXT_EIP__ = __eip__;
	sync_fetch_eip(__eip__);
}


/* real-address or virtual-8086 mode */		
//if(__PE__ == 0 || (__PE__ == 1 && __VM__ == 1)) 
void CCPU::far_call86 (unsigned short seg, unsigned int off32, int OperandSize)
{
		/*OperandSize ==  32 */
	if (OperandSize ==  32) {
			// stack not large enough for a 8-byte return address
			if ( !check_stack_for_push(8) ) SS(0); 

			if (off32 > LIMIT(cs_desc) ) GP(0); 

			Pushl(___CS___); /* padded with 16 high-order bits */
			Pushl(__NEXT_EIP__ );
			load_seg_real_or_v86 ( CS,seg );    /* DEST is ptr16:32 or [ m16:32] */
			
			__NEXT_EIP__ = off32;    /* DEST is ptr16:32 or [m16:32]  */
	} else {
		/* OperandSize ==  16 */
			// stack not large enough for a 4-byte return address
			if ( !check_stack_for_push(4) ) SS(0); 

			off32 = off32 & 0xFFFF; /* clear upper 16 bits */
			if (off32 > LIMIT(cs_desc) ) GP(0); 

			Pushw(___CS___);
			Pushw( __NEXT_EIP__ );

			load_seg_real_or_v86 (CS,seg);              /* DEST is ptr16:16 or [ m16:16] */

			__NEXT_EIP__ = off32;            /* DEST is ptr16:16 or [m16:16] */
	}
	sync_fetch_eip(__NEXT_EIP__);
}


void CCPU::far_call (unsigned short seg, unsigned int off32,int OperandSize)
{
	unsigned int dt_base;
	unsigned short dt_limit;
	/* real-address or virtual-8086 mode */		
	//if(__PE__ == 0 || (__PE__ == 1 && __VM__ == 1)) 
	if ((__PE__ == 0)|| (__VM__ == 1))
	{
		far_call86(seg,off32,OperandSize);
		return;
	}
/* Protected mode, not virtual-8086 mode */
// if(__PE__ == 1 AND __VM__ == 0) 	
	SELECTOR    cs        =  *(SELECTOR *)&seg;
	SELECTOR    tss;

	// segment selector in target operand is null?
	if (NULL_SELECTOR (cs)) GP(0); 

	// segment selector index not within descriptor table limits?
	dt_base  = IN_GDT(cs) ? gdt_base : ldt_base;
	dt_limit = IN_GDT(cs) ? gdt_limit: ldt_limit;

	if (SELECTOR_TOP(cs) > dt_limit) GP(MK_SHORT(cs));

	//Read type and access rights of selected segment descriptor;

	load_descriptor(dt_base,&CS_DESC,cs.index);

	// a conforming or nonconforming code segment
	if (CODE_SEG (CS_DESC) ) 
	{
		call_to_code_seg( cs,  off32,OperandSize);
	}
	else

	// TASK-STATE-SEGMENT:
	if (TSS_GATE(CS_DESC) ) 
	{
		//TSS DPL < CPL or TSS selector RPL or TSS descriptor indicates TSS not available)
		if (DPL (CS_DESC) < __CPL__ || DPL (CS_DESC) < RPL(cs) || !AVAILABLE (CS_DESC))
			GP (MK_SHORT (cs));

		//TSS is not present
		if (!PRESENT (CS_DESC)) NP (MK_SHORT (cs));  // TSS selector

		//SWITCH-TASKS (with nesting) to TSS
		switch_task (cs,CS_DESC_NO);
	} 
	else 

	//CALL-GATE:	
	if (CALL_GATE(CS_DESC) ) 
	{ 
		call_to_call_gate (cs);
	} 
	else 
	
	// TASK-GATE:
	if (TASK_GATE(CS_DESC)) {  
		//if (task gate DPL < CPL or RPL) #GP(task gate selector);
		if (DPL (CS_DESC) < __CPL__ || DPL (CS_DESC) < RPL(__CS__) )
			GP (MK_SHORT (cs));

		//task gate not present
		if (!PRESENT (CS_DESC) ) NP (MK_SHORT (cs));
	
		//Read the TSS segment selector in the task-gate descriptor;
		tss = GATE_SELECTOR (CS_DESC);

		//if (TSS segment selector local/global bit is set to local 
	    //     || index not within GDT limits)
		if (!tss.ti || SELECTOR_TOP(tss) > gdt_limit)
			GP(*(unsigned short *)&tss);

		//Access TSS descriptor in GDT;
		load_descriptor(gdt_base,&TSS_DESC,tss.index);
		//if (TSS descriptor specifies that the TSS is busy) 
		//     i.e. low-order 5 bits set to 00001	
		if (GATE_BUSYTSS (TSS_DESC)) 
			GP(*(unsigned short *)&tss);

		//TSS is not present
		if (!PRESENT (TSS_DESC)) // TSS selector
			NP(*(unsigned short *)&tss);

		//SWITCH-TASKS (with nesting) to TSS
		switch_task (tss,TSS_DESC_NO);

	} 
	else	
	{
		// segment type is not a conforming or nonconforming code segment, call gate,
		// task gate, or TSS
		GP (MK_SHORT (cs));

	}
}

void CCPU::call_to_code_seg( SELECTOR cs, address_t off32,int OperandSize)
{
	if (CONFORMING (CS_DESC)) { //CONFORMING-CODE-SEGMENT
		//IF DPL > CPL THEN #GP(new code segment selector); FI
		if (DPL(CS_DESC) > __CPL__) GP (MK_SHORT (cs)); 

		//IF segment not present THEN #NP(new code segment selector);
		if (!PRESENT (CS_DESC)) NP (MK_SHORT (cs)) ;

		if (OperandSize ==  32) {

			//if (the instruction pointer is not within code segment limit) #GP(0); 
			if (off32 > LIMIT(CS_DESC) ) GP(0); 


			RPL(cs) = __CPL__;
			copy_cs_desc (cs,CS_DESC_NO);
			/* segment descriptor information also loaded */

			__NEXT_EIP__ = off32;
		} else { /* OperandSize ==  16 */

			//if (the instruction pointer is not within code segment limit) #GP(0); 
			if (off32 > LIMIT(CS_DESC) ) GP(0); 

			RPL(cs) = __CPL__;
			copy_cs_desc (cs,CS_DESC_NO);
			/* segment descriptor information also loaded */

			__NEXT_EIP__ = off32 & 0xFFFF; /* clear upper 16 bits */
		}
	} else {  // NONCONFORMING-CODE-SEGMENT:
		//if (RPL > CPL) OR (DPL <> CPL), #GP(selector)
		if ((RPL(cs) > __CPL__) || (DPL (CS_DESC) != __CPL__)) GP (MK_SHORT (cs)) ;

		//if (segment not present) #NP(new code segment selector); 
		if (!PRESENT (CS_DESC)) NP (MK_SHORT (cs)) ;

		//if (the instruction pointer is not within code segment limit) #GP(0); 
		if (off32 > LIMIT(CS_DESC) ) GP(0); 

		if (OperandSize == 32){
			// stack not large enough for a 8-byte return address
			if (! check_stack_for_push(8) ) SS(0); 

			Pushl(___CS___); /* padded with 16 high-order bits */
			Pushl(__NEXT_EIP__);

			RPL(cs) = __CPL__;
			copy_cs_desc (cs,CS_DESC_NO);
			/* segment descriptor information also loaded */

			__NEXT_EIP__ = off32;
		} else { /* OperandSize ==  16 */
			// stack not large enough for a 4-byte return address
				if ( !check_stack_for_push(4) ) SS(0); 

			Pushw(___CS___);
			Pushw( __NEXT_EIP__ );

			RPL(cs) = __CPL__;
			copy_cs_desc (cs,CS_DESC_NO);
			/* segment descriptor information also loaded */

			__NEXT_EIP__ = off32 & 0xffff;
		}
	}
	sync_fetch_eip(__NEXT_EIP__);
}

//
//descriptor in CS_DESC
//
void CCPU::call_to_call_gate( SELECTOR gate_selector)
{
	unsigned int dt_base;
	unsigned short dt_limit;
	address_t off32;
	SELECTOR ss, cs;


	//if (call gate DPL < CPL or DPL < call gate selector RPL) #GP(call gate selector); 
	if (DPL (CS_DESC) < __CPL__ || DPL (CS_DESC) < RPL (gate_selector) )
		GP (MK_SHORT (gate_selector));

	//call gate not present
	if (!PRESENT (CS_DESC) ) NP (MK_SHORT (gate_selector));

	// Read Code segment selector from the call gate descriptor
	cs = GATE_SELECTOR (CS_DESC);

	//if (call gate code-segment selector is null) #GP(0); 
	if (NULL_SELECTOR (cs))	GP(0);

	//if (call gate code-segment selector index is outside descriptor table limits) #GP(code segment selector); 
	dt_base  = cs.ti ? gdt_base : ldt_base;
	dt_limit = cs.ti ? gdt_limit: ldt_limit;

	if (SELECTOR_TOP(cs) > dt_limit) GP( MK_SHORT(cs));

	//Read code segment descriptor;
	load_descriptor(gdt_base,&CALLGATE_DESC,cs.index);

	//if code-segment segment descriptor does not indicate a code segment
	// OR code-segment segment descriptor DPL > CPL
	if ( !CODE_SEG(CALLGATE_DESC) || DPL (CALLGATE_DESC) > __CPL__)    
		GP( MK_SHORT(cs));

	//code segment not present
	if (!PRESENT (CALLGATE_DESC)) NP(MK_SHORT(cs));

	// code segment is non-conforming AND DPL < CPL

	//MORE-PRIVILEGE:
	if ( !CONFORMING (CALLGATE_DESC) && DPL (CALLGATE_DESC) < __CPL__) 
	{
		unsigned int newESP,oldESP;

		SELECTOR newSS,oldSS;

		//if (current TSS is 32-bit TSS)
		if (tr_desc.type == DESCRIPTOR_FREETSS32
			|| tr_desc.type == DESCRIPTOR_BUSYTSS32){

			//TSSstackAddress = new code segment (DPL * 8) + 4
			//if ((TSSstackAddress + 7) > TSS limit) #TS(current TSS selector); 

			unsigned int TSSstackAddress = DPL (CALLGATE_DESC) * 8 + 4;
			// stack > TSS limit
			if ((TSSstackAddress + 7) > LIMIT (tr_desc)) 
				TS(___TR___); 
	
			newSS  = *(SELECTOR *)(TSSstackAddress + 4);
			newESP = TSSstackAddress;

		} else { /* TSS is 16-bit */
			//TSSstackAddress = new code segment (DPL * 4) + 2
			//if ((TSSstackAddress + 4) > TSS limit) #TS(current TSS selector); 

			unsigned int TSSstackAddress = DPL (CALLGATE_DESC) * 4 + 2;
			// stack > TSS limit
			if ((TSSstackAddress + 4) > LIMIT (tr_desc)) 
				TS(___TR___); 
			
			newESP = TSSstackAddress;
			newSS  = *(SELECTOR *)(TSSstackAddress + 2);
		}

		ss = __TSS__->ss;

		//stack segment selector is null
		if (NULL_SELECTOR (ss)) TS(MK_SHORT(ss)); 

		dt_base  = IN_GDT(ss) ? gdt_base : ldt_base;
		dt_limit = IN_GDT(ss) ? gdt_limit: ldt_limit;

		//stack segment selector index is not within its descriptor table limits
		if (SELECTOR_TOP(ss) > dt_limit) TS(MK_SHORT(ss)); 

		//Read stack segment descriptor;
		load_descriptor(dt_base,&SS_DESC,ss.index);

		if (RPL(ss) != DPL (CALLGATE_DESC)         //stack segment selector's RPL 1 DPL of code segment
			|| DPL(SS_DESC) != DPL (CALLGATE_DESC) //stack segment DPL <> DPL of code segment
			|| WRITEABLE (SS_DESC) )         //stack segment is not a writable data segment
			TS(MK_SHORT(ss)); 
	
		//stack segment not present
		if (!PRESENT (SS_DESC)) 
			SS(MK_SHORT(ss)); 

		// if 32 bit call gate
		if (CS_DESC.type == DESCRIPTOR_CALLGATE32)	{
			unsigned int temp;

			/*parameter count from call gate, masked to 5 bits;*/
			temp = GATE_PARAMCOUNT (CS_DESC) & 0x1f;

			// stack does not have room for parameters plus 16 bytes
			if (newESP+16+temp*4 > LIMIT (SS_DESC)) 
				SS(MK_SHORT(ss)); 

			//CallGate(InstructionPointer) not within code segment limit
			off32 = GATE_OFFSET (CALLGATE_DESC);
			if (off32 > LIMIT(CS_DESC) ) GP(0); 

			oldSS  = __SS__;
			oldESP =  __ESP__;

			int old_ss_base = BASE(SS_DESC);

			load_ss (newSS);
			/* segment descriptor information also loaded */
			__ESP__ = newESP;

			/* segment descriptor information also loaded */
			Pushl(MK_SHORT(oldSS)); /* from calling procedure */
			Pushl(oldESP); /* from calling procedure */

			while (temp) {
				unsigned int x = MEM_READL (old_ss_base + oldESP + temp * 4);
				/*parameters from calling procedure¡¯s stack, temp*/
				Pushl(x); 
				temp--;
			}
			Pushl(___CS___); /* return address to calling procedure */
			Pushl( __NEXT_EIP__); /* return address to calling procedure */

			copy_cs_desc (cs,CS_DESC_NO);
			__NEXT_EIP__ = off32;
		} else { /* CallGateSize = 16 */
			unsigned int temp;

			/*parameter count from call gate, masked to 5 bits;*/
			temp = GATE_PARAMCOUNT (CS_DESC) & 0x1f;

			// stack does not have room for parameters plus 8 bytes.#SS(SS selector); 
			if (newESP+8+temp*2 > LIMIT (SS_DESC)) 
				SS( MK_SHORT(ss)); 

			//if ((CallGate(InstructionPointer) AND FFFFH) not within code segment limit) #GP(0); 
			off32 = GATE_OFFSET (CALLGATE_DESC) & 0xffff;
			if (off32 > LIMIT(CS_DESC) ) GP(0); 

			oldSS  = __SS__;
			oldESP =  __ESP__;

			int old_ss_base = BASE(SS_DESC);

			load_ss (newSS);
			/* segment descriptor information also loaded */
			__ESP__ = newESP;

			/* segment descriptor information also loaded */
			Pushw(MK_SHORT(oldSS)); /* from calling procedure */
			Pushw(oldESP); /* from calling procedure */

			while (temp) {
				unsigned short x = MEM_READW (old_ss_base + oldESP + temp * 2);
				/*parameters from calling procedure¡¯s stack, temp*/
				Pushw(x); 
				temp--;
			}
			Pushw(___CS___); /* return address to calling procedure */
			Pushw( (unsigned short)__NEXT_EIP__ ); /* return address to calling procedure */

			copy_cs_desc (cs,CS_DESC_NO);
			__NEXT_EIP__ = off32;
		}

		//RPL(__CS__) = DPL(CS_DESC);

	}else {   //SAME-PRIVILEGE:

		RPL(cs) = __CPL__;

		if (CS_DESC.type == DESCRIPTOR_CALLGATE32)
		{
			// stack not large enough for a 8-byte return address
			if (!check_stack_for_push(8)) SS(0); 

			//if (the instruction pointer is not within code segment limit) #GP(0); 
			off32 = GATE_OFFSET (CALLGATE_DESC);
			if (off32 > LIMIT(CS_DESC) ) GP(0); 

			Pushl(___CS___);
			Pushl(__NEXT_EIP__); /* return address to calling procedure */

			copy_cs_desc (cs,CS_DESC_NO);
			__NEXT_EIP__ = GATE_OFFSET (CALLGATE_DESC); /* segment descriptor information also loaded */

		} else { /* CallGateSize = 16 */
			// stack not large enough for a 4-byte return address
			if (!check_stack_for_push(4)) SS(0); 

			//if (the instruction pointer is not within code segment limit) #GP(0); 
			off32 = (unsigned short)GATE_OFFSET (CALLGATE_DESC);

			if (off32 > LIMIT(CS_DESC) ) GP(0); 

			Pushw(___CS___);
			Pushw((unsigned short)__NEXT_EIP__); /* return address to calling procedure */

			copy_cs_desc (cs,CS_DESC_NO);
			__NEXT_EIP__ = GATE_OFFSET (CALLGATE_DESC) & 0xFFFF; /* segment descriptor information also loaded */
		}

	}
	sync_fetch_eip(__NEXT_EIP__);
}