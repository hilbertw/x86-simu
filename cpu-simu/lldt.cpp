

#include "cpu.h"
#define LDT_DESC tmp_desc1

void CCPU::load_ldt (SELECTOR selector)
{
	
	//#GP(0) if the current privilege level is not 0;
	if (__CPL__!=0) GP(0);
	//
	//if the selector operand does not point into the Global Descriptor Table, 
	if(!IN_GDT(selector))
		GP(MK_SHORT(selector));

	
	if (NULL_SELECTOR(selector))
	{
		___LDT___=0;
	}
	
	else if ( SELECTOR_TOP(selector) >= gdt_limit )
			GP(MK_SHORT(selector))
	else {
			load_descriptor(gdt_base,&LDT_DESC,selector.index);

		 //if the entry in the GDT is not a Local Descriptor Table; 
		if(LDT_DESC.type!=DESCRIPTOR_LDT)
			GP(MK_SHORT(selector))
		if (!PRESENT (LDT_DESC))             // if not present,
			NP (MK_SHORT (selector) );    //    #NP(selector);
		
		ldt_base=BASE(LDT_DESC);
		ldt_limit=LIMIT(LDT_DESC);

		ldt_seg=selector;
	}
}