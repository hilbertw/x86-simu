#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

/*
Table 9-3. Double-Fault Detection Classes

Class           ID          Description

                 1          Debug exceptions
                 2          NMI
                 3          Breakpoint
Benign           4          Overflow
Exceptions       5          Bounds check
                 6          Invalid opcode
                 7          Coprocessor not available
                16          Coprocessor error

                 0          Divide error
                 9          Coprocessor Segment Overrun
Contributory    10          Invalid TSS
Exceptions      11          Segment not present
                12          Stack exception
                13          General protection

Page Faults     14          Page fault


*/
#define EX_DE           0
#define EX_DB           1
#define EX_NMI          2
#define EX_BR           3
#define EX_OV           4
#define EX_B            5
#define EX_UD           6
#define EX_7            7
#define EX_8            8
#define EX_9            9
#define EX_TSS          10
#define EX_NP           11 
#define EX_SS           12
#define EX_GP           13
#define EX_PF           14
#define EX_15           15 // intel use only
#define EX_387          16
#define EX_MF           16
#define EX_AC           17
#define EX_MC           18
#define EX_XM           19
#define EX_TOTAL        20

class CCPUException
{
public:
	static const int para_mask=0x27d00;// exceptions with error code
	static const int fault_mask=0xb7ee3;// exceptions can restart

	unsigned int exception_para[EX_TOTAL];
	unsigned int exception_status;

	CCPUException() : exception_status(0)
	{}

	void RaiseException( unsigned int type, unsigned int para)
	{
		exception_para[type] = para;
		exception_status |= (1<<type);
	}

	void ClearException( unsigned int type)
	{
		exception_status &= ~(1<<type);
	}

	void ClearException()
	{
		exception_status = 0;
	}

	bool IsExceptionPening()
	{
		return exception_status != 0;
	}

	unsigned int exceptions(){ return exception_status;}
};





/*
Table 9-6 summarizes the exceptions recognized by the 386. 
Description               Interrupt   Return Address  Exception     Function That Can Generate
                            Number      Points to       Type          the Exception
Faulting
Instruction

Divide error               0          YES             FAULT         DIV, IDIV

Debug exceptions           1
Some debug exceptions are traps and some are faults.  The exception   Any instruction
handler can determine which has occurred by examining DR6.  


Breakpoint                 3          NO              TRAP          One-byte INT 3
Overflow                   4          NO              TRAP          INTO
Bounds check               5          YES             FAULT         BOUND
Invalid opcode             6          YES             FAULT         Any illegal instruction
Coprocessor not available  7          YES             FAULT         ESC, WAIT
Double fault               8          YES             ABORT         Any instruction that can generate an exception 

Coprocessor Segment
Overrun                    9          NO              ABORT         Any operand of an ESC instruction that wraps around the end of a segment.

Invalid TSS               10          YES             FAULT         JMP, CALL, IRET, any interrupt
An invalid-TSS fault is not restartable if it occurs during the
processing of an external interrupt.        

Segment not present       11          YES             FAULT         Any segment-register modifier
Stack exception           12          YES             FAULT         Any memory reference thru SS
General Protection        13          YES             FAULT/ABORT
All GP faults are restartable. If the fault occurs while attempting to
vector to the handler for an external interrupt, the interrupted program is
restartable, but the interrupt may be lost.  Any memory reference or code
fetch

Page fault                14          YES             FAULT         Any memory reference or code fetch

Coprocessor error         16          YES             FAULT
Coprocessor errors are reported as a fault on the first ESC or WAIT
instruction executed after the ESC instruction that caused the error.        ESC, WAIT

Two-byte SW Interrupt     0-255       NO              TRAP          INT n


===============================================================================
Table 9-7 summarizes the error information that is available with each exception. 

Description                       Interrupt     Error Code
                                  Number

Divide error                       0            No
Debug exceptions                   1            No
Breakpoint                         3            No
Overflow                           4            No
Bounds check                       5            No
Invalid opcode                     6            No
Coprocessor not available          7            No
System error                       8            Yes (always 0)
Coprocessor Segment Overrun        9            No
Invalid TSS                       10            Yes
Segment not present               11            Yes
Stack exception                   12            Yes
General protection fault          13            Yes
Page fault                        14            Yes
Coprocessor error                 16            No
Two-byte SW interrupt             0-255         No



*/

#endif   /*    __EXCEPTION_H__  */