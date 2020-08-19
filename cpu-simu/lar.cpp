
#include "cpu.h"

#pragma warning (disable:4018)

unsigned long CCPU::lar_ (SELECTOR selector,int  OperandSize)

{
	unsigned long ret=0;
	unsigned int dt_base;
	unsigned short dt_limit;

	//Interrupt 6; LAR is unrecognized in Real Address Mode 
	dt_base  = IN_GDT(selector) ? gdt_base : ldt_base;
	dt_limit = IN_GDT(selector) ? gdt_limit: ldt_limit;
	
	if(  (selector.index == 0)||SELECTOR_TOP(selector) > dt_limit )
	{
		__ZF__=0;
	}
	else {
			load_descriptor(dt_base,&tmp_desc1,selector.index);

			if(CODE_SEG (tmp_desc1)
				&&!(CONFORMING(tmp_desc1)
				&&(RPL(selector)>DPL(tmp_desc1)||__CPL__>DPL(tmp_desc1))))
			{
			__ZF__=0;
			}
			else if(tmp_desc1.type==0
				||tmp_desc1.type==8
				||tmp_desc1.type==0xd
				||tmp_desc1.type==0xa)
			{
			  //0    Invalid                  Invalid
			  //1    Available 80286 TSS      Valid
			  //2    LDT                      Valid
			  //3    Busy 80286 TSS           Valid
			  //4    80286 call gate          Valid
			  //5    80286/80386 task gate    Valid
			  //6    80286 trap gate          Valid
			  //7    80286 interrupt gate     Valid
			  //8    Invalid                  Invalid
			  //9    Available 80386 TSS      Valid
			  //A    Invalid                  Invalid
			  //B    Busy 80386 TSS           Valid
			  //C    80386 call gate          Valid
			  //D    Invalid                  Invalid
			  //E    80386 trap gate          Valid
			  //F    80386 interrupt gate     Valid
				__ZF__=0;
			}
			else
			{
				__ZF__=0;
				ret=(OperandSize==16)?0xff00:0xf0ff00;

				ret&=DWORD1_OF(tmp_desc1);
			}
	}
	return ret;
}
