#include "cpu.h"


#pragma warning (disable:4018)

void CCPU::near_ret (unsigned short imm, int OperandSize) 
{
	address_t __eip__;
	if (OperandSize ==  32) {
			// top 4 bytes of stack not within stack limits
			if (!check_stack_for_pop(4)){SS(0); return;} 
			
			__eip__ = Popl ();
			
	} else { /* OperandSize ==  16 */ 
			// top 2 bytes of stack not within stack limits
			if (!check_stack_for_pop(2)){SS(0); return;} 

			__eip__ = Popw();

			__eip__ = __eip__ & 0xffff;
			// the instruction pointer is not within code segment limit
			if (__eip__ > LIMIT (cs_desc) ) {GP(0); return;}
	}

	// if instruction has imm operand
	if (OperandSize ==  32) 
		__ESP__ = __ESP__ + imm;   /* release parameters from stack */
	else
		__SP__  = __SP__ + imm;

	__NEXT_EIP__ = __eip__;
	sync_fetch_eip(__eip__);
}	



/* real-address or virtual-8086 mode */		
//if(__PE__ == 0 || (__PE__ == 1 && __VM__ == 1)) 
void CCPU::far_ret86 (unsigned short imm, int OperandSize)
{
	address_t __eip__;
	unsigned short seg;

		/*OperandSize ==  32 */
	if (OperandSize ==  32) {
			// top 6 bytes of stack not within stack limits
			if (!check_stack_for_pop(6)){SS(0); return;} 

			__eip__ = Popl();
			seg     = (unsigned short)Popl(); /* padded with 16 high-order bits */

			load_seg_real_or_v86 (CS,seg);     /* DEST is ptr16:32 or [ m16:32] */
	} else {
		/* OperandSize ==  16 */
			// top 4 bytes of stack not within stack limits
			if (!check_stack_for_pop(4)){SS(0); return;} 

			__eip__  = Popw();
			seg    = Popw(); /* padded with 16 high-order bits */

			load_seg_real_or_v86 (CS,seg);               /* DEST is ptr16:16 or [ m16:16] */

			// TempEIP not within code segment limits
			if (__eip__ > LIMIT(cs_desc) ) GP(0); 

			__eip__ = __eip__;            /* DEST is ptr16:16 or [m16:16] */
			__eip__ = __eip__ & 0xFFFF; /* clear upper 16 bits */
	}

	// if instruction has imm operand
	if (OperandSize ==  32) 
		__ESP__ = __ESP__ + imm;   /* release parameters from stack */
	else
		__SP__  = __SP__ + imm;

	__NEXT_EIP__ = __eip__;
	sync_fetch_eip(__NEXT_EIP__);
}



/* Protected mode, not virtual-8086 mode */
// if(__PE__ == 1 AND __VM__ == 0) 	
void CCPU::far_ret (unsigned short imm,  int  OperandSize)
{

	/* real-address or virtual-8086 mode */		
	//__PE__ == 0 || (__PE__ == 1 && __VM__ == 1)

	if ((__PE__ == 0)|| (__VM__ == 1))
	{
		far_ret86(imm,OperandSize);
		return;
	}
	else
	{
		far_ret_protected(imm,OperandSize);
	}

}

/* Protected mode, not virtual-8086 mode */
// __PE__ == 1 AND __VM__ == 0
void CCPU::far_ret_protected ( unsigned short imm,int OperandSize)
{
	unsigned int dt_base;
	unsigned short dt_limit;
	unsigned long __eip__;
	unsigned long addr_hi,addr_lo;

	SELECTOR    cs;

	unsigned long sp=BIG_SEGMENT(descriptors[SS])?__ESP__:__SP__;
	int stackaddr_size=BIG_SEGMENT(descriptors[SS])?32:16;
	int bytes_to_pop;


	gen_mem_addr_raw(sp,0,stackaddr_size,addr_hi,addr_lo);
	if(!read_mem_raw(SS,addr_hi,addr_lo,__eip__,OperandSize,stackaddr_size)) SS(0)
	if (OperandSize ==  32) {

		//	if (second doubleword on stack is not within stack limits ) #SS(0);
		gen_mem_addr_raw(sp,4,stackaddr_size,addr_hi,addr_lo);
		if(!read_mem_raw(SS,addr_hi,addr_lo,*(unsigned long *)&cs,16,stackaddr_size)) SS(0)


	}else /* OperandSize ==  16 */
	{
		// if (second word on stack is not within stack limits ) #SS(0);
		gen_mem_addr_raw(sp,2,stackaddr_size,addr_hi,addr_lo);
		if(!read_mem_raw(SS,addr_hi,addr_lo,*(unsigned long *)&cs,16,stackaddr_size)) SS(0)

		__eip__ = __eip__ & 0xFFFF;	
	}
   
	//if return code segment selector is null,GP(0);
	if (NULL_SELECTOR (cs)) GP(0);

	// segment selector index not within descriptor table limits?
	dt_base  = cs.ti ? gdt_base : ldt_base;
	dt_limit = cs.ti ? gdt_limit: ldt_limit;

	//if (return code segment selector addrsses descriptor beyond diescriptor table limit, #GP(selector);
	if (SELECTOR_TOP(cs) > dt_limit) GP(MK_SHORT(cs));

	//Obtain descriptor to which return code segment selector points from descriptor table
	load_descriptor(gdt_base,&CS_DESC,cs.index);

	//if (return code segment descriptor is not a code segment ) #GP(selector);
	if (!CODE_SEG (CS_DESC)) GP (MK_SHORT(cs));

	//if return code segment selector RPL < CPL, #GP(selector);
	if (RPL(cs) < __CPL__) GP (MK_SHORT(cs));

	//if return code segment descriptor is conforming
	//		AND return code segment DPL > return code segment selector RPL
	//		#GP(selector);
	if (CONFORMING (CS_DESC) && DPL(CS_DESC) > RPL(cs))
		GP(MK_SHORT(cs));

	//if return code segment descriptor is not present, #NP(selector); FI:
	if (!PRESENT (CS_DESC)) NP (MK_SHORT(cs));

	//if return code segment selector RPL == CPL
	if ((RPL(cs) == __CPL__)) {


		// RETURN-TO-SAME-PRIVILEGE-LEVEL
		//if the return instruction pointer is not within ther return 
		// code segment limit, #GP(0);
		if (__eip__ > LIMIT (CS_DESC)) GP(0);

 
		if (OperandSize ==  32) {	
			bytes_to_pop=8;

		} else {/* OperandSize=16 */

			bytes_to_pop=4;

		}
		copy_cs_desc (cs,DESC_TMP1); 

		if(stackaddr_size==32)
			__ESP__ = __ESP__ + imm+bytes_to_pop; /* release parameters from stack */
		else
			__SP__ = __SP__ + imm+bytes_to_pop; /* release parameters from stack */
	} else {
		//RETURN-OUTER-PRIVILEGE-LEVEL;

		SELECTOR    ss;
		unsigned long esp;

		if(OperandSize ==  32) {
			//if top (16 + SRC) bytes of stack are not within stack limit, #SS(0); 
			if (!check_stack_for_pop(16+imm)) SS(0)

			gen_mem_addr_raw(sp,8+imm,stackaddr_size,addr_hi,addr_lo);
			assert(read_mem_raw(SS,addr_hi,addr_lo,esp,16,stackaddr_size));
			gen_mem_addr_raw(sp,12+imm,stackaddr_size,addr_hi,addr_lo);

			bytes_to_pop= 16;

		} else { /*OperandSize=16 */
			// if top (8 + SRC) bytes of stack are not within stack limits,#SS(0);
			if (!check_stack_for_pop(8+imm)) SS(0)

			gen_mem_addr_raw(sp,4+imm,stackaddr_size,addr_hi,addr_lo);
			assert(read_mem_raw(SS,addr_hi,addr_lo,esp,16,stackaddr_size));
			gen_mem_addr_raw(sp,6+imm,stackaddr_size,addr_hi,addr_lo);

			bytes_to_pop= 8;
		}
		
		//Read return stack segment selector;
		assert(read_mem_raw(SS,addr_hi,addr_lo,*(unsigned long *)&ss,16,stackaddr_size));
		//if return stack segment selector is null, #GP(0);
		if (NULL_SELECTOR (ss)) GP(0);

		//if (return stack segment selector index is not within its descriptor table limits
		// #GP(selector);
		dt_base  = ss.ti ? gdt_base : ldt_base;
		dt_limit = ss.ti ? gdt_limit: ldt_limit;

		if (SELECTOR_TOP(ss) > dt_limit) GP (MK_SHORT (ss));
		
		load_descriptor(gdt_base,&SS_DESC,ss.index);
		//Read segment descriptor pointed to by return segment selector;
	
		//if returnstack segment selector RPL <> RPL of the return code segment selector
		// OR stack segment is not a writable data segment
		// OR stack segment descriptor DPL <> RPL of the return code segment selector
		//#GP(selector);
		if (RPL (ss) != RPL (cs) 
			|| !WRITEABLE (SS_DESC)
			|| DPL (SS_DESC) != RPL (cs))
			GP (MK_SHORT (ss));

		//if return stack segment not present ) #SS(StackSegmentSelector);
		if (!PRESENT (SS_DESC)) SS (MK_SHORT (ss));

		//if the return instruction pointer is not within the return code segment limit,#GP(0); 
		if (__eip__ > LIMIT (CS_DESC)) GP (0);



		//unsigned char new_CPL = RPL(cs);
	
		if (OperandSize ==  32){
				__ESP__ = esp; 
		} else {/* OperandSize=16 */
			__eip__ = __eip__ & 0xffff;
			/* segment descriptor information also loaded */
			__ESP__ = (unsigned short )esp;
		}
		
		__SS__=ss;
		ss_desc=SS_DESC;

		//RPL(cs) = new_CPL;
		copy_cs_desc (cs,DESC_TMP1); /* segment descriptor information also loaded */


		//FOR each of segment register (ES, FS, GS, and DS)
		// check:
		//  1) if (segment register points to data or non-conforming code segment
		//	   AND CPL > segment descriptor DPL; 
		//  2) if (segment selector index is not within descriptor table limits
		//	   OR segment descriptor indicates the segment is not a data or
		//	   readable code segment
		//	   OR if (the segment is a data or non-conforming code segment and the segment
		//	   descriptor¡¯s DPL < CPL or RPL of code segment¡¯s segment selector
		null_invalid_sreg_selector (__DS__);
		null_invalid_sreg_selector (__FS__);
		null_invalid_sreg_selector (__GS__);
		null_invalid_sreg_selector (__ES__);

		__ESP__ = __ESP__ + imm; /* release parameters from calling procedure¡¯s stack */
	}
	__NEXT_EIP__ = __eip__;
	sync_fetch_eip(__eip__);
}
	
