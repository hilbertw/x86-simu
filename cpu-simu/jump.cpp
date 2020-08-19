#include "cpu.h"


#pragma warning (disable:4018)

void CCPU::near_rel_jump (int offset,int OperandSize ) 
{
	address_t __eip__;

	__eip__ = (OperandSize ==  32) ?  (__NEXT_EIP__ + offset) 
		                           : ((__NEXT_EIP__ + offset) & 0xffff);

	// the instruction pointer is not within code segment limit
	if (__eip__ > LIMIT (cs_desc) ) GP(0); 

	__NEXT_EIP__ = __eip__;
	sync_fetch_eip(__eip__);
}

/* DEST is r/m32 */
void CCPU::near_abs_jump (address_t __eip__,int OperandSize )
{
	//the instruction pointer is not within code segment limit
	if (__eip__ > LIMIT (cs_desc)) GP(0); 

	if (OperandSize ==  32);
	else  /* OperandSize ==  16 */
			__eip__ = __eip__ & 0xFFFF; /* DEST is r/m16 */

	__NEXT_EIP__=__eip__;
	sync_fetch_eip(__eip__);

	//fetcher.set_fetch_address ((__PE__ == 0)|| (__VM__ == 1), BASE(cs_desc),LIMIT(cs_desc),__EIP__,((__PE__ == 0)|| (__VM__ == 1)?0:(__CPL__==3))); //real mode
}


/* real-address or virtual-8086 mode */		
//if(__PE__ == 0 || (__PE__ == 1 && __VM__ == 1)) 
void CCPU::far_jump86 (unsigned short seg, unsigned int off32,int OperandSize )
{
		/*OperandSize ==  32 */
	if (OperandSize ==  32) {
			load_seg_real_or_v86 (CS,seg);     /* DEST is ptr16:32 or [ m16:32] */
			if (off32 > LIMIT(cs_desc) ) GP(0); 
			__NEXT_EIP__ = off32;    /* DEST is ptr16:32 or [m16:32]  */
	} else {
		/* OperandSize ==  16 */
			load_seg_real_or_v86 (CS,seg);             /* DEST is ptr16:16 or [ m16:16] */
			if (off32 > LIMIT(cs_desc) ) GP(0); 

			__NEXT_EIP__ = off32;            /* DEST is ptr16:16 or [m16:16] */
			__NEXT_EIP__ = __NEXT_EIP__ & 0xFFFF; /* clear upper 16 bits */
	}


	sync_fetch_eip(__NEXT_EIP__);
	//fetcher.set_fetch_address (true, BASE(cs_desc),LIMIT(cs_desc),__EIP__,0); //real mode
}


void CCPU::far_jump (unsigned short seg, unsigned int off32,int OperandSize )
{
	unsigned int dt_base;
	unsigned short dt_limit;

	/* real-address or virtual-8086 mode */		
	//if(__PE__ == 0 || (__PE__ == 1 && __VM__ == 1)) 
	if ((__PE__ == 0) 
		|| (__VM__ == 1))
	{
	far_jump86(seg,off32,OperandSize);
	return;
	}

/* Protected mode, not virtual-8086 mode */
// if(__PE__ == 1 AND __VM__ == 0) 	

	union
	{
	SELECTOR    cs;
	SELECTOR    tss;
	};

	cs=  *(SELECTOR *)&seg;
	// effective address in the CS/DS/ES/SS/FS/GS segment is illegal?

	// segment selector in target operand is null?
	if (NULL_SELECTOR (cs)) {GP(0);  return;}

	// segment selector index not within descriptor table limits?
	dt_base  = IN_GDT(cs) ? gdt_base : ldt_base;
	dt_limit = IN_GDT(cs) ? gdt_limit: ldt_limit;

	if (SELECTOR_TOP(cs) > dt_limit) GP(MK_SHORT(cs));

	//Read type and access rights of selected segment descriptor;
	load_descriptor(dt_base,&CS_DESC,cs.index);

	// a conforming or nonconforming code segment
	if (CODE_SEG(CS_DESC) ) {

		if (CONFORMING (CS_DESC)) { //CONFORMING-CODE-SEGMENT

			//Descriptor DPL must be <= CPL ELSE #GP(selector);
			if (DPL(CS_DESC) > __CPL__) GP (MK_SHORT (cs)); 

			if (!PRESENT (CS_DESC)) NP (MK_SHORT (cs)) ;

			if (OperandSize ==  32) {

				//if (the instruction pointer is not within code segment limit) #GP(0); 
				if (off32 > LIMIT(CS_DESC) ) GP(0); 

				/* segment descriptor information also loaded */
				RPL(cs) = __CPL__;
				__NEXT_EIP__=__EIP__ = off32;
				copy_cs_desc (cs,CS_DESC_NO);
			} else { /* OperandSize ==  16 */

				//if (the instruction pointer is not within code segment limit) #GP(0); 
				if (off32 > LIMIT(CS_DESC) ) GP(0); 

				/* segment descriptor information also loaded */
				RPL(cs) = __CPL__;
				__NEXT_EIP__=__EIP__ = off32 & 0xFFFF; /* clear upper 16 bits */
				copy_cs_desc (cs,CS_DESC_NO);
			}

		} else {  // NONCONFORMING-CODE-SEGMENT:
			//if (RPL > CPL) OR (DPL <> CPL), #GP(selector)
			if ((RPL(cs) > __CPL__) || (DPL (CS_DESC) != __CPL__)) GP (MK_SHORT (cs)) ;

			//if (segment not present) #NP(new code segment selector); 
			if (!PRESENT (CS_DESC)) NP (MK_SHORT (cs)) ;

			if (OperandSize == 32){


				//if (the instruction pointer is not within code segment limit) #GP(0); 
				if (off32 > LIMIT(CS_DESC) ) GP(0); 

				/* segment descriptor information also loaded */
		
				__NEXT_EIP__=__EIP__ = off32;
				RPL(cs) = __CPL__;
				copy_cs_desc (cs,CS_DESC_NO);
			} else { /* OperandSize ==  16 */

		

				//if (the instruction pointer is not within code segment limit) #GP(0); 
				if (off32 > LIMIT(CS_DESC) ) GP(0); 

				/* segment descriptor information also loaded */
				RPL(cs) = __CPL__;
				__NEXT_EIP__=__EIP__ = off32 & 0xffff;
				copy_cs_desc (cs,CS_DESC_NO);
			}

		}

	} else if (CS_DESC.type == DESCRIPTOR_FREETSS16
		    || CS_DESC.type == DESCRIPTOR_BUSYTSS16
			|| CS_DESC.type == DESCRIPTOR_FREETSS32
			|| CS_DESC.type == DESCRIPTOR_BUSYTSS32 ) 
	{


		// TASK-STATE-SEGMENT:
		//TSS DPL < CPL or TSS segment selector RPL or TSS descriptor indicates TSS not available)

		if (DPL (CS_DESC) < __CPL__ || DPL (CS_DESC) < RPL(tss) || !AVAILABLE (CS_DESC))
			GP (MK_SHORT (tss));

		//TSS is not present
		if (!PRESENT (CS_DESC)) NP (MK_SHORT (tss));  // TSS selector

		//SWITCH-TASKS (with nesting) to TSS
		// save old TSS
		switch_task(tss,DESC_TMP1 );
		//
		//if (the instruction pointer is not within code segment limit) #GP(0); 
	} else if (CS_DESC.type == DESCRIPTOR_CALLGATE16 
		|| CS_DESC.type == DESCRIPTOR_CALLGATE32 ) { //CALL-GATE:
		//if (call gate DPL < CPL or DPL < call gate selector RPL) #GP(call gate selector); 
		if (DPL (CS_DESC) < __CPL__ || DPL (CS_DESC) < RPL (cs) )
			GP (MK_SHORT (cs));

		//call gate not present
		if (!PRESENT (CS_DESC) ) NP (MK_SHORT (cs));
	
		cs = GATE_SELECTOR (CS_DESC);
		//if (call gate code-segment selector is null) #GP(0); 
		if (NULL_SELECTOR (cs))	GP(0);

		//if (call gate code-segment selector index is outside descriptor table limits) #GP(code segment selector); 
		dt_base  = IN_GDT(cs) ? gdt_base : ldt_base;
		dt_limit = IN_GDT(cs) ? gdt_limit: ldt_limit;

		if (SELECTOR_TOP(cs) > dt_limit) GP(MK_SHORT(cs));

		//Read code segment descriptor;
			load_descriptor(dt_base,&CS_DESC,cs.index);

		// if not a code segment
		// or code segment is conforming AND DPL > CPL
		// or code segment is non-conforming AND DPL<>CPL
		if ( (CS_DESC.type & 0x10)==0 
			|| (CONFORMING (CS_DESC) && DPL (CS_DESC) > __CPL__)
			|| (!CONFORMING (CS_DESC) && DPL (CS_DESC) != __CPL__) )

			GP(MK_SHORT(cs)); 
	
		//code segment not present
		if (!PRESENT (CS_DESC)) NP(MK_SHORT(cs));
	
		//Load EIP from call gate;
		off32=GATE_OFFSET(CS_DESC);

		// if instruction pointer not within code-segment limit, #GP(0)
		if (off32 > LIMIT (CS_DESC)) GP (MK_SHORT (cs));

		if (CS_DESC.type == DESCRIPTOR_CALLGATE32)
			__NEXT_EIP__=__EIP__ = off32;
		else
			__NEXT_EIP__=__EIP__ = off32 & 0xffff;

	   //Load CS register with new code-segment descriptor;
	   //Set RPL of CS to CPL
		RPL(cs) = __CPL__;
		copy_cs_desc (cs,DESC_TMP1);

	} else if (CS_DESC.type == DESCRIPTOR_TASKGATE) {  // TASK-GATE:
		//if (task gate DPL < CPL or task gate selector RPL) #GP(task gate selector);

		if (DPL (CS_DESC) < __CPL__ || DPL (CS_DESC) < RPL(cs) )
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
		// save old TSS
		switch_task(tss,TSS_DESC_NO);

		//if (the instruction pointer is not within code segment limit) #GP(0); 
		if (__EIP__ > LIMIT(cs_desc) ) GP(0); 

	} else
	// segment type is not a conforming or nonconforming code segment, call gate,
	// task gate, or TSS
		GP (MK_SHORT (cs));

	sync_fetch_eip(__NEXT_EIP__);
}
	

