#ifndef __fpu_h__
#define __fpu_h__
#include "common.h"

#pragma pack(push,1)

typedef struct  fp_80_def
{
	unsigned char fraction[8];// 64 bit
	unsigned  short exp:15;
	unsigned short sign:1;
} FP80;

typedef struct  fp_64_def
{
	unsigned char fraction_lo[6];// 52 bit
	unsigned  char fraction_hi:4;
	unsigned  char  exp_lo:4;
	unsigned  char  exp_hi:7;
	unsigned char sign:1;
} FP64;


void  __inline convert_fp64_to_fp80(FP64 * fp64,FP80 *fp80)
{
	fp80->fraction[0]=0;
	*(unsigned int *)(fp80->fraction+1)=*(unsigned int *)(fp64->fraction_lo)<<3;
	fp80->fraction[5]=(unsigned char)(*(unsigned short *)(fp64->fraction_lo+3)<<3);
	*(unsigned short *)(fp80->fraction+6)=(fp64->fraction_hi<<11)
		+(*(unsigned short *)(fp64->fraction_lo+5)<<3)
		+(fp64->fraction_lo[4]>>5);
	//fp64:The format is written with the significand having an implicit lead bit of value 1,
	//unless the exponent is stored with all zeros.
	if(fp64->exp_hi!=0||fp64->fraction_lo!=0)
	fp80->fraction[7]|=0x80;

	fp80->sign=fp64->sign;

	//The exponent field uses Excess-N representation, to be more exact Excess-16383, 
	fp80->exp= (fp64->exp_hi<<4)+fp64->exp_lo-0x3ff +0x3fff;
}

void __inline convert_fp80_to_fp64(FP80 *fp80,FP64 * fp64)
{

	//
	//fp64:The format is written with the significand having an implicit lead bit of value 1,
	//unless the exponent is stored with all zeros.
	//
	*(unsigned int *)(fp64->fraction_lo)=(*(unsigned int *)(fp80->fraction+1)>>3)
		+(fp80->fraction[5]<<29);

	*(unsigned short *)(fp64->fraction_lo+4)=(*(unsigned short *)(fp80->fraction+6)>>3);
	 //
	fp64->fraction_hi=fp80->fraction[7]>>3;

	// exponent is encoded using an offset binary representation, with the zero offset being 1023

	fp64->sign=fp64->sign;

	fp64->exp_lo=( fp80->exp-0x3fff +0x3ff)&0xf;
	fp64->exp_hi=( fp80->exp-0x3fff +0x3ff)>>4;
}


// fpu status word bits
#define FPU_SF_IE 0 // invalid operand
#define FPU_SF_DE 1 // denormalized operand
#define FPU_SF_ZE 2 // zero devid
#define FPU_SF_OE 3 // overflow
#define FPU_SF_UE 4 // underflow
#define FPU_SF_PE 5 // precision
#define FPU_SF_SF 6 // stack fault exception flag
#define FPU_SF_ES 7 // errpr summary
#define FPU_SF_C0 8 //  
#define FPU_SF_C1 9
#define FPU_SF_C2 10
#define FPU_SF_TOP 11
#define FPU_SF_C3 14
#define FPU_SF_B  15

typedef unsigned short FPU_STATUS,FPU_CONTROL;



// fpu control word bits
#define FPU_CF_IM 0 // invalid operand
#define FPU_CF_DM 1 // denormalized operand
#define FPU_CF_ZM 2 // zero devid
#define FPU_CF_OM 3 // overflow
#define FPU_CF_UM 4 // underflow
#define FPU_CF_PM 5 // precision
#define FPU_CF_PC 8 // precision control
#define FPU_CF_RC 9 // rounding control
#define FPU_CF_X 10 // infinity control



class CFPU
{
public:
	FPU_CONTROL FPUControlWord ;
	FPU_STATUS FPUStatusWord  ;
	unsigned short FPUTagWord;  
	unsigned long  FPUDataPointer  ;      // operand address
	unsigned short FPUDataSeg; 
	unsigned long  FPUInstructionPointer;  
	unsigned short FPUInsnSeg;
	unsigned short FPULastInstructionOpcode;
	
	
	FP80  st10[8];
	double st[8];


	CFPU():FPUStatusWord(0x38) {}

	void save_opcode(PINSN_BASIC_INFO p_insn);
	void FWAIT_();
	void fsave(char * buffer, int data_size, bool real_mode);
	void frestore(char * buffer, int data_size, bool real_mode);

 void push() 
{
// when top is at zero,  wrap arund to r7
if((FPUStatusWord &0x3800)==0)FPUStatusWord |=0x3800;
else FPUStatusWord -=0x800;
}

 void pop() 
{
// when top is at zero,  wrap arund to r7
if((FPUStatusWord &0x3800)==0x3800)FPUStatusWord &=~0x3800;
else FPUStatusWord +=0x800;
}

void FXRSTOR_mem512(int OperandSize,char *mem512); 
void FXSAVE_mem512(int OperandSize,char *mem512) ;

#include "fpu-inc.h"
};

#define ST(i) st[(((FPUStatusWord &0x3800)>>11)+i)&7]

#pragma pack(pop)
#endif //__fpu_h__
