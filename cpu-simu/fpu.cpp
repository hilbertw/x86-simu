#include <string.h>
#include <assert.h>
#include <math.h>

#include "log.h"
#include "fpu.h"

#define LOG_FPU 1

#define MASK_ZF 0x40
#define MASK_AF 0x10
#define MASK_PF 0x4
#define MASK_OF 0x800
#define MASK_CF 0x1 
#define MASK_SF 0x80


#ifdef __GNUC__
	typedef long long __int64;
#else
	
#endif

void CFPU::save_opcode(PINSN_BASIC_INFO p_insn)	
{
}
void CFPU::F2XM1_()
{
#if LOG_FPU
	log0(" 2^%le -1->",ST(0));
#endif
	//Compute 2^x-1
	//source operand is located in register ST(0) and the result is also stored in ST(0).
	//xy  2(y * log2x)
	ST(0)=exp(ST(0)* log((long double )2))-1;
#if LOG_FPU
	log0(" %le\n",ST(0));
#endif
}

void CFPU::FABS_()
{
#if LOG_FPU
	log0("fsbs %le ->",ST(0));
#endif
	//ST(0)  |ST(0)|
	ST(0)=fabs(ST(0));
#if LOG_FPU
	log0(" %le\n",ST(0));
#endif
}


void CFPU::FADD_STi0(int i)
{
#if LOG_FPU
	log0(" %le+%le->",ST(0),ST(i));
#endif
	//Add ST(i) to ST(0) and store result in ST(i)
	ST(i)=ST(i)+ST(0);
#if LOG_FPU
	log0(" %le\n",ST(i));
#endif
}

void CFPU::FADD_ST0i(int i)
{
#if LOG_FPU
	log0(" %le+%le->",ST(0),ST(i));
#endif
	//Add ST(0) to ST(i) and store result in ST(0)
	ST(0)=ST(i)+ST(0);
#if LOG_FPU
	log0(" %le\n",ST(0));
#endif
}

void CFPU::FADD_mem4r(char * buffer)
{
#if LOG_FPU
	log0(" %le+%le->",ST(0),*(float *)buffer);
#endif
	//Add m32real to ST(0) and store result in ST(0)
	ST(0)=ST(0)+*(float *)buffer;
#if LOG_FPU
	log0(" %le\n",ST(0));
#endif
}

void CFPU::FADD_mem8r(char * buffer)
{
#if LOG_FPU
	log0(" %le+%le->",ST(0),*(double *)buffer);
#endif
		//Add m32real to ST(0) and store result in ST(0)
	ST(0)=ST(0)+*(double *)buffer;
#if LOG_FPU
	log0(" %le\n",ST(0));
#endif
}

void CFPU::FADDP_STi0(int i)
{
#if LOG_FPU
	log0(" %le+%le->",ST(0),ST(i));
#endif
	//Add ST(i) to ST(0) and store result in ST(i)
	ST(i)=ST(i)+ST(0);
#if LOG_FPU
	log0(" %le\n",ST(i));
#endif
	// PopRegisterStack
	pop();

}

void CFPU::FBLD_mem10d(char * buffer)
{ 
#ifdef __GNUC__
	;
#else
	__asm int 3;
#endif
}

void CFPU::FBSTP_mem10d(char * buffer)
{ 
	// PopRegisterStack
	pop();
}

void CFPU::FCHS_()
{ 
	ST(0)=-ST(0);
}

void CFPU::FCLEX_()
{ 
	// clear exceptions
}


void CFPU::FCOMI_ST0i(unsigned short &cflags,int i) 
{
#if LOG_FPU
	log0(" %le>/= %le?",ST(0),ST(i));
#endif
	cflags &=~(MASK_PF|MASK_CF|MASK_ZF);
	if(ST(0)==ST(i))	cflags|=MASK_ZF;
	else if(ST(0)<ST(i))	cflags|=MASK_CF;
}

void CFPU::FCOMIP_ST0i(unsigned short &cflags,int i) 
{
#if LOG_FPU
	log0(" %le>/= %le?",ST(0),ST(i));
#endif
	cflags &=~(MASK_PF|MASK_CF|MASK_ZF);
	if(ST(0)==ST(i))	cflags|=MASK_ZF;
	else if(ST(0)<ST(i))	cflags|=MASK_CF;
	// PopRegisterStack
	pop();
}

void CFPU::FCOS_() 
{
	ST(0)=cos(ST(0));
}

void CFPU::FDECSTP_() {
	// PushRegisterStack
	push();
}

void  CFPU::FDIV_mem8r(char * buffer)
{
#if LOG_FPU
	log0("%le / %le->",ST(0),*(double *)buffer);	
#endif
	ST(0)=ST(0)/ *(double *)buffer;
#if LOG_FPU
	log0("%le\n",ST(0));
#endif
}
void  CFPU::FDIV_mem4r(char * buffer)
{
#if LOG_FPU
	log0("%le / %le->",ST(0),*(float *)buffer);	
#endif
	ST(0)=ST(0)/ *(float *)buffer;
#if LOG_FPU
	log0("%le\n",ST(0));
#endif
}

void CFPU::FDIV_STi0(int i) 
{
#if LOG_FPU
	log0(" %le/%le->",ST(i),ST(0));
#endif
	ST(i)=ST(i)/ST(0);	
#if LOG_FPU
	log0("%le\n",ST(i));
#endif
}
void CFPU::FDIV_ST0i(int i) 
{
#if LOG_FPU
	log0(" %le/%le->",ST(0),ST(i));
#endif
	ST(0)=ST(0)/ST(i);	
#if LOG_FPU
	log0("%le\n",ST(0));
#endif
}

void CFPU::FDIVP_STi0(int i) 
{
#if LOG_FPU
	log0(" %le/%le->",ST(i),ST(0));
#endif
	ST(i)=ST(i)/ST(0);	
#if LOG_FPU
	log0("%le\n",ST(i));
#endif
	pop();
}
void CFPU::FDIVR_mem4r(char * buffer) 
{ 
#if LOG_FPU
	log0(" %le/%le->",*(float *)buffer,ST(0));
#endif
	ST(0)=*(float *)buffer/ST(0);	
#if LOG_FPU
	log0(" %le\n",ST(0));
#endif
}

void CFPU::FDIVR_mem8r(char * buffer) 
{ 
#if LOG_FPU
	log0(" %le/%le->",*(double *)buffer,ST(0));
#endif
	ST(0)=*(double *)buffer/ST(0);		

#if LOG_FPU
	log0(" %le\n",ST(0));
#endif
}

void CFPU::FDIVR_STi0(int i) 
{
#if LOG_FPU
	log0(" %le/%le->",ST(0),ST(i));
#endif
	ST(i)=ST(0)/ST(i);	
#if LOG_FPU
	log0("%le\n",ST(i));
#endif
}


void CFPU::FDIVR_ST0i(int i) 
{ 
#if LOG_FPU
	log0(" %le/%le->",ST(i),ST(0));
#endif
	ST(0)=ST(i)/ST(0);	
#if LOG_FPU
	log0(" %le\n",ST(0));
#endif
}

void CFPU::FDIVRP_STi0(int i) 
{ 
#if LOG_FPU
	log0(" %le/%le->",ST(0),ST(i));
#endif
	ST(i)=ST(0)/ST(i);	
#if LOG_FPU
	log0(" %le\n",ST(i));
#endif
	pop();
}

void CFPU::FFREE_STi(int i) 
{ 
#ifdef __GNUC__
	;
#else
	__asm int 3;
#endif
}

void CFPU::FIADD_mem4i( unsigned long  data) 
{ 
#if LOG_FPU
	log0(" %d+%le->",data,ST(0));
#endif
	ST(0)+=data;
#if LOG_FPU
	log0(" %le\n",ST(0));
#endif
}
void CFPU::FIADD_mem2i(unsigned short data) 
{
#if LOG_FPU
	log0(" %d+%le->",data,ST(0));
#endif
	ST(0)+=data;	
#if LOG_FPU
	log0(" %le\n",ST(0));
#endif
}

void CFPU::FICOM_mem4i( unsigned long  data) 
{
#if LOG_FPU
	log0(" %le>/= %d?",ST(0),data);
#endif

	FPUStatusWord&=~((1<<FPU_SF_C0)|(1<<FPU_SF_C2)|(1<<FPU_SF_C3));
	//
	// format checking, if ST0unformatted, c3=c2=c0=1
	//
	if(ST(0)>data) FPUStatusWord|=(1<<FPU_SF_C0);//  C0=1;
	else if(ST(0)==data) FPUStatusWord|=(1<<FPU_SF_C3);//  C3=1;
}
void CFPU::FICOM_mem2i(unsigned short data) 
{ 
#if LOG_FPU
	log0(" %le>/= %d?",ST(0),data);
#endif

	FPUStatusWord&=~((1<<FPU_SF_C0)|(1<<FPU_SF_C2)|(1<<FPU_SF_C3));
	//
	// format checking, if ST0unformatted, c3=c2=c0=1
	//
	if(ST(0)>data) FPUStatusWord|=(1<<FPU_SF_C0);//  C0=1;
	else if(ST(0)==data) FPUStatusWord|=(1<<FPU_SF_C3);//  C3=1;
}

void CFPU::FICOMP_mem2i(unsigned short data) 
{ 
	FICOM_mem2i(data);
	pop();
}

void CFPU::FICOMP_mem4i( unsigned long  data) 
{
	FICOM_mem4i(data);
	// PopRegisterStack
	pop();
}
void CFPU::FIDIV_mem2i(unsigned short data) 
{
#if LOG_FPU
	log0("idiv %le/%d->",ST(0),data);
#endif
	ST(0)/=(short)data;	
#if LOG_FPU
	log0(" %le\n",ST(0));
#endif
}

void CFPU::FIDIV_mem4i( unsigned long  data) 
{
#if LOG_FPU
	log0(" %le/%d->",ST(0),data);
#endif
	ST(0)/=(int)data;	
#if LOG_FPU
	log0(" %le\n",ST(0));
#endif
}
void CFPU::FIDIVR_mem4i( unsigned long  data) 
{
#if LOG_FPU
	log0(" %d/%le->",data,ST(0));
#endif
	ST(0)=(int)data/ST(0);	
#if LOG_FPU
	log0(" %le\n",ST(0));
#endif
}
void CFPU::FIDIVR_mem2i(unsigned short data) 
{
#if LOG_FPU
	log0("idivr  %d/%le->",data,ST(0));
#endif
	ST(0)=(int)data/ST(0);
#if LOG_FPU
	log0(" %le\n",ST(0));
#endif
}

void CFPU::FILD_mem8i(unsigned long * data) 
{ 
	__int64 i64=*(__int64*)data;
	push();
	ST(0)=(double)i64;
#if LOG_FPU
	log0("load %le\n",ST(0));
#endif
}

void CFPU::FILD_mem2i(unsigned short data) 
{ 
	push();
	ST(0)=(short)data;	
#if LOG_FPU
	log0("load %le\n",ST(0));
#endif
}

void CFPU::FILD_mem4i( unsigned long  data) 
{
	push();
	ST(0)=(int)data;
#if LOG_FPU
	log0("load %le\n",ST(0));
#endif
}
void CFPU::FIMUL_mem4i( unsigned long  data) 
{ 
#if LOG_FPU
	log0("%le* %d->",ST(0),data);
#endif
	ST(0)*=(int)data;
#if LOG_FPU
	log0(" %le\n",ST(0));
#endif
}
void CFPU::FIMUL_mem2i(unsigned short data) 
{ 
#if LOG_FPU
	log0("%le* %d->",ST(0),data);
#endif
	ST(0)*=(short)data;
#if LOG_FPU
	log0(" %le\n",ST(0));
#endif
}

void CFPU::FINCSTP_() 
{
	// PopRegisterStack
	pop();	
}



void CFPU::FISUB_mem4i( unsigned long  data) 
{
#if LOG_FPU
	log0(" %le- %d->",ST(0),data);
#endif
	ST(0)-=(int)data;
#if LOG_FPU
	log0(" %le\n",ST(0));
#endif
}
void CFPU::FISUB_mem2i(unsigned short data) 
{ 
#if LOG_FPU
	log0(" %le- %d->",ST(0),data);
#endif
	ST(0)-=(short)data;
#if LOG_FPU
	log0(" %le\n",ST(0));
#endif
}
void CFPU::FLD_mem8r(char * buffer) 
{ 
	push();
	ST(0)=*(double*)buffer;
	convert_fp64_to_fp80((FP64*)buffer,(FP80*)&st10[0]);

#if LOG_FPU
	log0("load %le\n",ST(0));
#endif
}

void CFPU::FLD_STi(int i) 
{ 
	double tmp=ST(i);
	push();
	ST(0)=tmp;
#if LOG_FPU
	log0("load %le\n",ST(0));
#endif
}

void CFPU::FLD_mem10r(char * buffer) 
{ 
	push();
	convert_fp80_to_fp64((FP80*)buffer,(FP64*)&ST(0));
#if LOG_FPU
	log0("load %le\n",ST(0));
#endif
}

void CFPU::FLD_mem4r(char * buffer) 
{ 
    push();
	ST(0)=*(float *)buffer;
#if LOG_FPU
	log0("load %le\n",ST(0));
#endif
}

void CFPU::FLD1_() 
{ 
	push();
	ST(0)=1.0;
#if LOG_FPU
	log0("load 1.0:%le\n",ST(0));
#endif
}

void CFPU::FLDCW_mem2i(unsigned short data) 
{ 
	FPUControlWord=data;
}

void CFPU::FLDENV_mem14(char * buffer) 
{ 
}

void CFPU::FLDL2E_() 
{
	push();
	ST(0)=1/log(2.0);
#if LOG_FPU
	log0("load log2(e):%le\n",ST(0));
#endif
}

void CFPU::FLDL2T_() 
{ 
	push();
	ST(0)=1/log10(2.0);
#if LOG_FPU
	log0("load log2(10):%le\n",ST(0));
#endif
}

void CFPU::FLDLG2_() 
{
	push();
	ST(0)=log10(2.0);
#if LOG_FPU
	log0("load log(2):%le\n",ST(0));
#endif
}

void CFPU::FLDLN2_() 
{
	push();
	ST(0)=log(2.0);
#if LOG_FPU
	log0("load ln(2):%le\n",ST(0));
#endif
}

void CFPU::FLDPI_() 
{
	push();
	ST(0)=2*asin(1.0);
#if LOG_FPU
	log0("load PI:%le\n",ST(0));
#endif
}

void CFPU::FLDZ_() 
{ 
	push();
	ST(0)=0;
#if LOG_FPU
	log0("load 0:%le\n",ST(0));
#endif
}


void CFPU::FMUL_mem8r(char * buffer)
{
#if LOG_FPU
	log0("%le * %le->",ST(0),*(double *)buffer);
#endif
	ST(0)=ST(0)**(double *)buffer;
#if LOG_FPU
	log0("%le\n",ST(0));
#endif
}

void CFPU::FMUL_mem4r(char * buffer)
{ 
#if LOG_FPU
	log0("%le * %le->",ST(0),*(float *)buffer);
#endif
	ST(0)=ST(0)**(float *)buffer;
#if LOG_FPU
	log0("%le\n",ST(0));
#endif
}

void CFPU::FMUL_ST0i(int i)
{ 
#if LOG_FPU
	log0("%le * %le->",ST(0),ST(i));
#endif
	ST(0)*=ST(i);
#if LOG_FPU
	log0("%le\n",ST(0));
#endif
}

void CFPU::FMUL_STi0(int i)
{ 
#if LOG_FPU
	log0("%le * %le->",ST(0),ST(i));
#endif
	ST(i)*=ST(0);
#if LOG_FPU
	log0("%le\n",ST(i));
#endif
}

void CFPU::FMULP_STi0(int i)
{ 
#if LOG_FPU
	log0("%le * %le->",ST(0),ST(i));
#endif
	ST(i)*=ST(0);
#if LOG_FPU
	log0("%le\n",ST(i));
#endif
	// PopRegisterStack
	pop();
}

void CFPU::FNINIT_()  
{
FPUControlWord=0x037F;
FPUStatusWord=0;
FPUTagWord=0xFFFF;
FPUDataPointer=0;
FPUInstructionPointer=0;
FPULastInstructionOpcode=0;

//C0, C1, C2, C3 cleared to 0.
FPUStatusWord &=~0x4700;
}
void CFPU::FNOP_()
{ 
}

unsigned short CFPU::FNSTCW_mem2i()
{ 
   return FPUStatusWord;
}
void CFPU::FNSTENV_mem14(char * buffer)
{ 
	memset(buffer,0,14);
}

unsigned short CFPU::FNSTSW_fmt_AX()
{

//DEST<-FPUStatusWord;
//The C0, C1, C2, and C3 are undefined.
 return FPUStatusWord;
}
unsigned short CFPU::FNSTSW_mem2i()
{ 
//DEST<-FPUStatusWord;
//The C0, C1, C2, and C3 are undefined.
 return FPUStatusWord;
}
void CFPU::FPATAN_() 
{
	ST(0)=atan(ST(1)/ST(0));
	pop();
}


void CFPU::FPREM_()
{ 
	int  q=0.5+ST(0)/ST(1);//nearest integer
	ST(0)=ST(0)-q*ST(1);  // IEEE remainder
}

void CFPU::FPREM1_()
{ 
	double q=ST(0)/ST(1);
	ST(0)=ST(0)-q*ST(1);
}

void CFPU::FPTAN_()
{ 
	ST(0)=tan(ST(0));
	push();
	ST(0)=1.0;
}

void CFPU::FRNDINT_()
{
	//round st0 to integer
	ST(0)=(int)ST(0);
}

void CFPU::FRSTOR_mem108(int OperandSize,char * buffer)
{
	;
}

void CFPU::FSAVE_mem108(int OperandSize,char * buffer)
{
}

void CFPU::FSCALE_()
{
}

void CFPU::FSIN_()
{
	ST(0)=sin(ST(0));
}

void CFPU::FSINCOS_()
{
	double cos_=cos(ST(0));
	ST(0)=sin(ST(0));
	push();
	ST(0)=cos_;
}

void CFPU::FSQRT_()
{
	ST(0)=sqrt(ST(0));
}

void CFPU::FST_mem8r(char * buffer)
{
#if LOG_FPU
	log0("store %le\n",ST(0));
#endif
	*(double *)buffer=ST(0);
}

void CFPU::FST_mem4r(char * buffer)
{
#if LOG_FPU
	log0("store %le\n",ST(0));
#endif
	*(float *)buffer=(float )ST(0);
}

void CFPU::FST_STi(int i)
{

	ST(i)=ST(0);
}

void CFPU::FSTP_mem4r(char * buffer)
{
#if LOG_FPU
	log0("store %le\n",ST(0));
#endif
	*(float *)buffer=(float )ST(0);
	pop();
}

void CFPU::FSTP_mem8r(char * buffer)
{
#if LOG_FPU
	log0("store %le\n",ST(0));
#endif
	*(double *)buffer=ST(0);
	pop();
}

void CFPU::FSTP_mem10r(char * buffer)
{
#if LOG_FPU
	log0("store %le\n",ST(0));
#endif
	convert_fp64_to_fp80((FP64*)&ST(0),(FP80*)buffer);
	pop();	
}

void CFPU::FSTP_STi(int i)
{
#if LOG_FPU
	log0("store %le\n",ST(0));
#endif
	ST(i)=ST(0);
	pop();	
}

void CFPU::FSUB_mem4r(char * buffer)
{
#if LOG_FPU
	log0(" %le-%le->",ST(0),*(float *)buffer);
#endif
	ST(0)=ST(0)-*(float *)buffer;
#if LOG_FPU
	log0("%le\n",ST(0));
#endif
}

void CFPU::FSUB_ST0i(int i)
{
#if LOG_FPU
	log0(" %le-%le->",ST(0),ST(i));
#endif
	ST(0)=ST(0)-ST(i);
#if LOG_FPU
	log0("%le\n",ST(0));
#endif
}

void CFPU::FSUB_mem8r(char * buffer)
{
#if LOG_FPU
	log0(" %le-%le->",ST(0),*(double *)buffer);
#endif
	ST(0)=ST(0)-*(double *)buffer;
#if LOG_FPU
	log0("%le\n",ST(0));
#endif
}

void CFPU::FSUB_STi0(int i)
{
#if LOG_FPU
	log0(" %le-%le->",ST(i),ST(0));
#endif
	ST(i)=ST(i)-ST(0);
#if LOG_FPU
	log0("%le\n",ST(i));
#endif
}

void CFPU::FSUBP_STi0(int i)
{
#if LOG_FPU
	log0(" %le-%le->",ST(i),ST(0));
#endif
	ST(i)=ST(i)-ST(0);
#if LOG_FPU
	log0("%le\n",ST(i));
#endif
	// pop
	pop();
}

void CFPU::FTST_()
{
#if LOG_FPU
	log0(" %le>/=0?\n",ST(0));
#endif
	FPUStatusWord&=~((1<<FPU_SF_C0)|(1<<FPU_SF_C2)|(1<<FPU_SF_C3));
	//
	// format checking, if ST0unformatted, c3=c2=c0=1
	//
	if(ST(0)>0) FPUStatusWord|=(1<<FPU_SF_C0);//  C0=1;
	else if(ST(0)==0) FPUStatusWord|=(1<<FPU_SF_C3);//  C3=1;

#if LOG_FPU
	log0(" ucom %le 0->%x\n",ST(0),FPUStatusWord);
#endif
}

void CFPU::FUCOM_STi(int i)
{
	FPUStatusWord &=~((1<<FPU_SF_C0)|(1<<FPU_SF_C2)|(1<<FPU_SF_C3));
	if(ST(0)==ST(i))	FPUStatusWord |=(1<<FPU_SF_C3);//  C3=1;
	else if(ST(0)<ST(i))	FPUStatusWord |=(1<<FPU_SF_C0);

#if LOG_FPU
	log0(" ucom %le %le->%x\n",ST(0),ST(i),FPUStatusWord);
#endif
}

void CFPU::FUCOMP_()
{
	FPUStatusWord &=~((1<<FPU_SF_C0)|(1<<FPU_SF_C2)|(1<<FPU_SF_C3));
	if(ST(0)==ST(1))	FPUStatusWord |=(1<<FPU_SF_C3);//  C3=1;
	else if(ST(0)<ST(1))	FPUStatusWord |=(1<<FPU_SF_C0);

#if LOG_FPU
	log0(" ucom %le %le->%x\n",ST(0),ST(1),FPUStatusWord);
#endif

	pop();
}
void CFPU::FUCOMPP_()
{
	FPUStatusWord &=~((1<<FPU_SF_C0)|(1<<FPU_SF_C2)|(1<<FPU_SF_C3));
	if(ST(0)==ST(1))	FPUStatusWord |=(1<<FPU_SF_C3);//  C3=1;
	else if(ST(0)<ST(1))	FPUStatusWord |=(1<<FPU_SF_C0);

#if LOG_FPU
	log0(" ucom %le %le->%x\n",ST(0),ST(1),FPUStatusWord);
#endif
	pop();
	pop();
}
void CFPU::FUCOMI_ST0i(unsigned short &cflags,int i)
{

	cflags &=~(MASK_PF|MASK_CF|MASK_ZF);
	if(ST(0)==ST(i))	cflags|=MASK_ZF;
	else if(ST(0)<ST(i))	cflags|=MASK_CF;
	//
	// un ordered...
	//
#if LOG_FPU
	log0(" comi %le,%le->%x\n",ST(i),ST(0),cflags);
#endif
}

void CFPU::FUCOMIP_ST0i(unsigned short &cflags,int i)
{
	cflags &=~(MASK_PF|MASK_CF|MASK_ZF);
	if(ST(0)==ST(i))	cflags|=MASK_ZF;
	else if(ST(0)<ST(i))	cflags|=MASK_CF;

#if LOG_FPU
	log0(" comi %le,%le->%x\n",ST(0),ST(i),cflags);
#endif
	// PopRegisterStack
	pop();
}

void CFPU::FUCOMP_STi(int i)
{

	// compare st0, sti, and pop
	FPUStatusWord &=~((1<<FPU_SF_C0)|(1<<FPU_SF_C2)|(1<<FPU_SF_C3));
	if(ST(0)==ST(i))	FPUStatusWord |=(1<<FPU_SF_C3);
	else if(ST(0)<ST(i))	FPUStatusWord |=(1<<FPU_SF_C0);
	//else if(ST(0)==ST(i))	FPUStatusWord |=0x70;
	// PopRegisterStack

#if LOG_FPU
	log0(" ucom %le %le->%x\n",ST(0),ST(i),FPUStatusWord);
#endif
	pop();
}

void CFPU::FXAM_()
{
	//
	// unsupported   0
	// NaN           1
	// normal finity 2
	// infinity      3
	// zero,         4
	//empty,         5
	//denormal,      6
	//
    //
    // setting c3,c2,c0
    //

}

void CFPU::FXCH_STi(int i)
{
	//
	// exchange sti with st0
	//
	double temp;

#if LOG_FPU
	log0(" %le<->%le",ST(i),ST(0));
#endif
	temp=ST(0);
	ST(0)=ST(i);
	ST(i)=temp;
#if LOG_FPU
	log0(" ->%le,%le\n",ST(i),ST(0));
#endif

}

void CFPU::FXRSTOR_mem512(int OperandSize,char * buffer)
{
#ifdef __GNUC__
	;
#else
	__asm int 3;
#endif
}

void CFPU::FXSAVE_mem512(int OperandSize,char * buffer)
{
#ifdef __GNUC__
	;
#else
	__asm int 3;
#endif
}

//
// te
//
void CFPU::FXTRACT_()
{
	union
	{
	FP64 fp64;
	double f;
	double sig;
	};


	f=ST(0);
#if LOG_FPU
	log0("fxtract:%lf->",f);
#endif
	double exp=(fp64.exp_hi<<4)+fp64.exp_lo;
	fp64.exp_hi=0x3f;
	fp64.exp_lo=0xf;
	ST(0)=exp;
	push();
	ST(0)=sig;
#if LOG_FPU
	log0("%lf E %lf\n",sig,exp);
#endif
}

void CFPU::FYL2X_()
{
#if LOG_FPU
	log0("fyl2x:%lf,%lf->",ST(0),ST(1));
#endif
	ST(1)*=log(ST(0));
#if LOG_FPU
	log0("%lf\n",ST(1));
#endif
	pop();

}

// y*log(x+1)
void CFPU::FYL2XP1_()
{
#if LOG_FPU
	log0("fyl2x:%lf,%lf->",ST(0),ST(1));
#endif
	ST(1)*=log(ST(0)+1.0);
#if LOG_FPU
	log0("%lf\n",ST(1));
#endif
	pop();
}


void CFPU::FWAIT_()
{
//CheckForPendingUnmaskedFloatingPointExceptions;
//The C0, C1, C2, and C3 flags are undefined.

}




unsigned short CFPU::FIST_mem2i()
{
#if LOG_FPU
	log0("istore %le\n",ST(0));
#endif
	return (unsigned short)ST(0);
}
unsigned long CFPU::FIST_mem4i()
{
#if LOG_FPU
	log0("istore %le\n",ST(0));
#endif
	return (unsigned long)ST(0);
}

unsigned short CFPU::FISTP_mem2i()
{
#if LOG_FPU
	log0("istore %le\n",ST(0));
#endif

	unsigned short s= (unsigned short)ST(0);
	// PopRegisterStack
	pop();
	return s;

}
unsigned long CFPU::FISTP_mem4i()
{
#if LOG_FPU
	log0("istore %le\n",ST(0));
#endif

	unsigned long s= (unsigned long)ST(0);
	//pop
	// PopRegisterStack
	pop();
	return s;
}

void CFPU::FISTP_mem8i( unsigned long * data)
{
#if LOG_FPU
	log0("istore %le\n",ST(0));
#endif

	data[1]=0;
	data[0]=(unsigned long)ST(0);
	// PopRegisterStack
	pop();
}
void CFPU::FISTTP_mem8i( unsigned long * data)
{
#if LOG_FPU
	log0("istore %le\n",ST(0));
#endif
	data[1]=0;
	data[0]=(unsigned long)ST(0);
	// PopRegisterStack
	pop();
}
unsigned short CFPU::FISTTP_mem2i()
{
#if LOG_FPU
	log0("istore %le\n",ST(0));
#endif

	unsigned short s= (unsigned short)ST(0);
	//pop
	// PopRegisterStack
	pop();
	return s;
}
unsigned long CFPU::FISTTP_mem4i()
{
#if LOG_FPU
	log0("istore %le\n",ST(0));
#endif
	unsigned long s= (unsigned long)ST(0);
	//pop
	// PopRegisterStack
	pop();
	return s;
}




void CFPU::FISUBR_mem2i(unsigned short data)
{
#if LOG_FPU
	log0(" %d-%le->",data,ST(0));
#endif
	ST(0)=data-ST(0);
#if LOG_FPU
	log0("%le\n",ST(0));
#endif
}
void CFPU::FISUBR_mem4i( unsigned long  data)
{
#if LOG_FPU
	log0(" %d-%le->",data,ST(0));
#endif
	ST(0)=data-ST(0);
#if LOG_FPU
	log0("%lf\n",ST(0));
#endif
}

void CFPU::FSUBR_mem8r(char * buffer)
{
#if LOG_FPU
	log0(" %le+%le->",*(double *)buffer,ST(0));
#endif
	ST(0)=*(double *)buffer-ST(0);
#if LOG_FPU
	log0("%le\n",ST(0));
#endif
}

void CFPU::FSUBR_ST0i(int i)
{
#if LOG_FPU
	log0(" %le-%le->",ST(i),ST(0));
#endif
	ST(0)=ST(i)-ST(0);
#if LOG_FPU
	log0("%le\n",ST(0));
#endif
}

void CFPU::FSUBR_mem4r(char * buffer)
{
#if LOG_FPU
	log0(" %le-%le->",*(float *)buffer,ST(0));
#endif
	ST(0)=*(float *)buffer-ST(0);
#if LOG_FPU
	log0("%le\n",ST(0));
#endif
}

void CFPU::FSUBR_STi0(int i)
{
#if LOG_FPU
	log0(" %le-%le->",ST(0),ST(i));
#endif
	ST(i)=ST(0)-ST(i);
#if LOG_FPU
	log0("%le\n",ST(i));
#endif
}

void CFPU::FSUBRP_STi0(int i)
{
#if LOG_FPU
	log0(" %le-%le->",ST(0),ST(i));
#endif
	ST(i)=ST(0)-ST(i);
#if LOG_FPU
	log0("%le\n",ST(i));
#endif
	pop();

}