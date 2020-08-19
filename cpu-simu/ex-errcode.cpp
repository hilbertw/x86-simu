#include "cpu.h"
#pragma warning (disable:4018)
#pragma warning (disable:4013)

//__PE__==0, REAL-ADDRESS-MODE;	
void CCPU::ex_w_errcode86 (unsigned int vector_number,unsigned short err_code)
{
	dump_int(vector_number);
	
	if (vector_number * 4 + 3 >= idt_limit)  //if (((DEST * 4) + 3) is not within IDT limit,
		GP(0);                               //   #GP; 

	if (!check_stack_for_push (6) )  //if stack not large enough for a 6-byte return information
		SS(0);             //  #SS; 

	Pushw (__CFLAGS__);     //EFLAGS[15:0]
	__IF__ =  0;           // Clear interrupt flag 
	__TF__ =  0;            // Clear trap flag 
	__AC__ =  0;            //Clear AC flag

	Pushw(___CS___);
	Pushw( __NEXT_EIP__ );
	/* No error codes are pushed */

	unsigned short cs = MEM_READW ( vector_number * 4 + 2);
	load_seg_real_or_v86(CS,cs);
    /* 16 bit offset AND 0000FFFFH */
	__NEXT_EIP__=
	__EIP__ = MEM_READW ( vector_number * 4); 

}

void CCPU::ex_w_errcode (unsigned short vector,unsigned long errcode,int OperandSize)
{
	dump_int(vector);
	/* real-address or virtual-8086 mode */		
	//if(__PE__ == 0 || (__PE__ == 1 && __VM__ == 1)) 
	if ((__PE__ == 0)|| (__VM__ == 1))
	{
		ex_w_errcode86(vector,(unsigned short)errcode);
	}
	else
		int_protected(vector,false,errcode,true, OperandSize);


	sync_fetch_eip(__NEXT_EIP__);
}