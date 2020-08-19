
#include "cpu.h"


void CCPU::load_tr (SELECTOR selector)
{
	unsigned int dt_base;
	unsigned short dt_limit;
	DESCRIPTOR desc;
	
	//#GP(0) if the current privilege level is not 0;
	if (__CPL__!=0) GP(0);
	//
	dt_base  = IN_GDT(selector) ? gdt_base : ldt_base;
	dt_limit = IN_GDT(selector) ? gdt_limit: ldt_limit;
	
	if (selector.index == 0) GP(0)
	
	else if ( SELECTOR_TOP(selector) > dt_limit || ( selector.rpl != __CPL__) )
			GP(MK_SHORT(selector))
	else {
			load_descriptor(dt_base,&desc,selector.index);

			//GP(selector) 
			//if the object named by the source selector is not a TSS 
			//or is already busy; 


			if ( !TASKSTATE_SEG(desc)  ||desc.type==10)
				GP(MK_SHORT(selector))

		
			//#NP(selector) if the TSS is marked "not present"; 
			else if (!PRESENT (desc)) 
				NP(MK_SHORT(selector))
	

			//LTR loads the task register from the source register or memory location specified by the operand. 

			__TR__  = selector;
			tr_desc = desc;

			//The loaded task state segment is marked busy. 
			 tr_desc.type |=2;
			mmu.stdw(dt_base + (unsigned int)&((DESCRIPTOR*)NULL)[selector.index],0,DWORD0_OF(tr_desc));
			mmu.stdw(dt_base + (unsigned int)&((DESCRIPTOR*)NULL)[selector.index]+4,0,DWORD1_OF(tr_desc));
			//A task switch does not occur.

	}
}

