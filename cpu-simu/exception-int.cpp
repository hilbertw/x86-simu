
#include <stdio.h>
#include "cpu.h"
#include "log.h"
//In virtual-8086 mode, the interrupts and exceptions are divided into three classes for the
//purposes of handling:
//
// Class 1¡ªAll processor-generated exceptions and all hardware interrupts, including the
//NMI interrupt and the hardware interrupts sent to the processor¡¯s external interrupt
//delivery pins. All class 1 exceptions and interrupts are handled by the protected-mode
//exception and interrupt handlers.

void  CCPU::exception_call(unsigned char exception_no,int OperandSize)
{
	dump_int(exception_no);
	if ((__PE__ == 0))
	{
		int_real86(exception_no);
	}
	else
		int_protected(exception_no,0,0,0, OperandSize);


	sync_fetch_eip(__NEXT_EIP__);
}


//
//IF PE=0
//THEN 
//GOTO REAL-ADDRESS-MODE;
//ELSE (* PE=1 *)
//IF (VM=1 AND IOPL < 3 AND INT n) 
//THEN 
//#GP(0);
//ELSE (* protected mode or virtual-8086 mode interrupt *)
//GOTO PROTECTED-MODE;
//FI;
//FI;

//In virtual-8086 mode, the interrupts and exceptions are divided into three classes for the
//purposes of handling:
//Class 2¡ªSpecial case for maskable hardware interrupts (Section 5.1.1.2., ¡°Maskable
//Hardware Interrupts¡±, in Chapter 5, Interrupt and Exception Handling) when the virtual
//mode extensions are enabled.
//Class 3¡ªAll software-generated interrupts, that is interrupts generated with the INT n
//instruction1.
//The method the processor uses to handle class 2 and 3 interrupts depends on the setting of the
//following flags and fields:
// IOPL field (bits 12 and 13 in the EFLAGS register)¡ªControls how class 3 software
//interrupts are handled when the processor is in virtual-8086 mode (refer to Section 2.3.,
//¡°System Flags and Fields in the EFLAGS Register¡±, in Chapter 2, System Architecture
//Overview). This field also controls the enabling of the VIF and VIP flags in the EFLAGS
//register when the VME flag is set. The VIF and VIP flags are provided to assist in the
//handling of class 2 maskable hardware interrupts.
// VME flag (bit 0 in control register CR4)¡ªEnables the virtual mode extension for the
//processor when set (refer to Section 2.5., ¡°Control Registers¡±, in Chapter 2, System Architecture
//Overview).
//
// Software interrupt redirection bit map (32 bytes in the TSS, refer to Figure
//16-5)¡ªContains 256 flags that indicates how class 3 software interrupts should be handled
//when they occur in virtual-8086 mode. A software interrupt can be directed either to the
//interrupt and exception handlers in the currently running 8086 program or to the protectedmode
//interrupt and exception handlers.
//
// The virtual interrupt flag (VIF) and virtual interrupt pending flag (VIP) in the EFLAGS
//register¡ªProvides virtual interrupt support for the handling of class 2 maskable
//hardware interrupts (refer to Section 16.3.2., ¡°Class 2¡ªMaskable Hardware Interrupt
//Handling in Virtual-8086 Mode Using the Virtual Interrupt Mechanism¡±).
void CCPU::int_call (unsigned char vector,int OperandSize)
{
	dump_int(vector);
	/* real-address or virtual-8086 mode */		
	//if(__PE__ == 0 || (__PE__ == 1 && __VM__ == 1)) 
	if ((__PE__ == 0)|| (__VM__ == 1&&!int_redirected(vector)))
	{
		int_real86(vector);
	}
	else
		int_protected(vector,1,0,0, OperandSize);


	sync_fetch_eip(__NEXT_EIP__);
}





//__PE__==0, REAL-ADDRESS-MODE;	
void CCPU::ex_w_errcode86 (unsigned int vector_number,unsigned short err_code)
{

	
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
	if ((__PE__ == 0))
	{
		ex_w_errcode86(vector,(unsigned short)errcode);
	}
	else
		int_protected(vector,false,errcode,true, OperandSize);


	sync_fetch_eip(__NEXT_EIP__);
}