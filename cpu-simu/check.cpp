#include "cpu.h"


//Table 6-2. Useful Combinations of E, G, and B Bits
//
//Case:                    1         2         3         4
//
//Expansion Direction      U         U         D         D
//G-bit                    0         1         0         1
//B-bit                    X         X         0         1
//
//Lower bound is:
//0                        X         X
//LIMIT+1                                      X
//shl(LIMIT,12,1)+1                                      X
//
//Upper bound is:
//LIMIT                    X
//shl(LIMIT,12,1)                    X
//64K-1                                        X
//4G-1                                                   X
//
//Max seg size is:
//64K                      X
//64K-1                              X
//4G-4K                                        X
//4G                                                     X
//
//Min seg size is:
//0                        X         X
//4K                                           X         X
//
//shl (X, 12, 1) = shift X left by 12 bits inserting one-bits on the right
//
bool CCPU::check_limit ( DESCRIPTOR *desc, unsigned int low, unsigned int high)
{

	unsigned int t;
	unsigned int lower,upper;

	if (CODE_SEG (*desc)) 
		t = GRANULARITY (*desc) ? 1 : 0;
	else if (EXPAND_DOWN (*desc)) {
		t = GRANULARITY (*desc) ? 3 : 2;
	} else {
		t = GRANULARITY (*desc) ? 1 : 0;
	}

	switch (t) {
	case 0:
		lower = 0;
		upper = LIMIT (*desc);
		break;
	case 1:
		lower = 0;
		upper = (LIMIT (*desc) << 12) | 0xfff;
		break;
	case 2:
		lower = (LIMIT (*desc) + 1)&0xffff;
		upper =  0xffff;
		break;
	case 3:
		lower = ((LIMIT (*desc) << 12) |0xfff)+1;
		upper = 0xffffffff;
		break;
	}


	return low >= lower && high <= upper;
}

//Table 6-3. Interlevel Return Checks


//Type of Check                                  Exception   Error Code
//
//ESP is within current SS segment               SF          0
//ESP + 7 is within current SS segment           SF          0
//RPL of return CS is greater than CPL           GP          Return CS
//Return CS selector is not null                 GP          Return CS
//Return CS segment is within descriptor
//  table limit                                  GP          Return CS
//Return CS descriptor is a code segment         GP          Return CS
//Return CS segment is present                   NP          Return CS
//DPL of return nonconforming code
//  segment = RPL of return CS, or DPL of
//  return conforming code segment <= RPL
//  of return CS                                 GP          Return CS
//ESP + N + 15 is within SS segment
//N   Immediate Operand of RET N Instruction     SF          Return SS
//SS selector at ESP + N + 12 is not null        GP          Return SS
//SS selector at ESP + N + 12 is within
//  descriptor table limit                       GP          Return SS
//SS descriptor is writable data segment         GP          Return SS
//SS segment is present                          SF          Return SS
//Saved SS segment DPL = RPL of saved
//  CS                                           GP          Return SS
//Saved SS selector RPL = Saved SS
//  segment DPL                                  GP          Return SS
