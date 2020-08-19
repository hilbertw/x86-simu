#include "cpu.h"
#include "log.h"
#pragma warning (disable:4018)


void CCPU::rdtsc_()
{
	//IF (CR4.TSD = 0) or (CPL = 0) or (CR0.PE = 0)
	//THEN
	//IF 64-Bit Mode
	//THEN
	//RAX[31:0] ¡û TimeStampCounter[31:0];
	//RAX[63:32] ¡û 0;
	//RDX[31:0] ¡û TimeStampCounter[63:32];
	//RDX[63:32] ¡û 0;
	//ELSE
	//EDX:EAX ¡û TimeStampCounter;
	//FI;
	//ELSE (* CR4.TSD = 1 and (CPL = 1, 2, or 3) and CR0.PE = 1 *)
	//#GP(0);
	//FI;

	if(__CR4__.tsd==0||__CPL__==0||__PE__==0)
	{
		REG_CODE reg;
		reg.reg_cat=RC_REGG32;
		reg.reg_no=AL;
		update_reg(reg,__TSC__.lo);
		reg.reg_no=DL;
		update_reg(reg,__TSC__.hi);

		//log("rdtsc:%x\n",__TSC__.lo);
	}
	else GP(0)
}

void CCPU::rdpmc_()
{
//(* P6 family processors and Pentium processor with MMX technology *)
//IF (ECX = 0 or 1) and ((CR4.PCE = 1) or (CPL = 0) or (CR0.PE = 0))
//THEN
//EAX ¡û PMC(ECX)[31:0];
//EDX ¡û PMC(ECX)[39:32];
//ELSE (* ECX is not 0 or 1 or CR4.PCE is 0 and CPL is 1, 2, or 3 and CR0.PE is 1 *)
//#GP(0);
//FI;
//(* Pentium 4 and Intel Xeon processor *)
//IF (ECX[30:0] = 0:17) and ((CR4.PCE = 1) or (CPL = 0) or (CR0.PE = 0))
//THEN IF ECX[31] = 0
//THEN IF 64-Bit Mode
//THEN
//RAX[31:0] ¡û PMC(ECX[30:0])[31:0]; (* 40-bit read *)
//RAX[63:32] ¡û 0;
//RDX[31:0] ¡û PMC(ECX[30:0])[39:32];
//RDX[63:32] ¡û 0;
//ELSE
//EAX ¡û PMC(ECX[30:0])[31:0]; (* 40-bit read *)
//EDX ¡û PMC(ECX[30:0])[39:32];
//FI;
//ELSE IF ECX[31] = 1
//THEN IF 64-Bit Mode
//THEN
//RAX[31:0] ¡û PMC(ECX[30:0])[31:0]; (* 32-bit read *)
//RAX[63:32] ¡û 0;
//RDX ¡û 0;
//ELSE
//EAX ¡û PMC(ECX[30:0])[31:0]; (* 32-bit read *)
//EDX ¡û 0;
//FI;
//FI;
//ELSE (* ECX[30:0] ¡Ù 0:17 or CR4.PCE = 0 and (CPL = 1, 2, or 3) and CR0.PE = 1 *)
//#GP(0);
//FI;

	if((__ECX__>=0&&__ECX__<=17)&&(__CR4__.pce==0||__CPL__==0||__PE__==0))
	{
		REG_CODE reg;
		reg.reg_cat=RC_REGG32;
		reg.reg_no=AL;
		update_reg(reg,0);
		reg.reg_no=DL;
		update_reg(reg,0);

		log("rdpmc\n");
	}
	else GP(0)
}