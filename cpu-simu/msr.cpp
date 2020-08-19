#include "cpu.h"
#include "log.h"
//MSR Address Register Name Function 
//0x40000070 HV_X64_MSR_EOI Accesses the APIC EOI 
//0x40000071 HV_X64_MSR_ICR Accesses the APIC ICR high and ICR low 
//0x40000072 HV_X64_MSR_TPR Access the APIC TPR 


void CCPU::rdmsr_()
{
		unsigned int msr_num;

		msr_num=__CX__;

#if LOG_MSR
		log("Read MSR %x:",msr_num);
#endif

		if(__CPL__!=0) GP(0)

		else if((__ECX__>>16)==0x8000)
		{
			if(msr_num>=MAX_MSR2) GP(0)
			else
			{
				REG_CODE reg;
				reg.reg_cat=RC_REGG32;
				reg.reg_no=AL;
				update_reg(reg,msr2[msr_num].lo);
				reg.reg_no=DL;
				update_reg(reg,msr2[msr_num].hi);

#if LOG_MSR
			log0("%x %x\n",msr2[msr_num].lo,msr2[msr_num].hi);
#endif
			}
		}
		else if((__ECX__>>16)==0xc001)
		{

			if(msr_num>=MAX_MSR1) GP(0)
			else
			{
				REG_CODE reg;
				reg.reg_cat=RC_REGG32;
				reg.reg_no=AL;
				update_reg(reg,msr3[msr_num].lo);
				reg.reg_no=DL;
				update_reg(reg,msr3[msr_num].hi);
#if LOG_MSR
			log0("%x %x\n",msr3[msr_num].lo,msr3[msr_num].hi);
#endif
			}
		}
		else 
		{

			if(msr_num>=MAX_MSR1) GP(0)
			else
			{
				REG_CODE reg;
				reg.reg_cat=RC_REGG32;
				reg.reg_no=AL;
				update_reg(reg,msr1[msr_num].lo);
				reg.reg_no=DL;
				update_reg(reg,msr1[msr_num].hi);
#if LOG_MSR
				log0("%x %x\n",msr1[msr_num].lo,msr1[msr_num].hi);
	#endif
			}
		}
}
void CCPU:: wrmsr_()
{
		unsigned int msr_num;

		msr_num=__CX__;
#if LOG_MSR
		log("Write MSR:%x %x,%x\n",msr_num,__EAX__,__EDX__);
#endif
		if(__CPL__!=0) GP(0)

		else if((__ECX__>>16)==0x8000)
		{
	


			if(msr_num>=MAX_MSR2) GP(0)
			else
			{
			msr2[msr_num].lo=__EAX__;
			msr2[msr_num].hi=__EDX__;
			}
		}
		else if((__ECX__>>16)==0xc001)
		{

			if(msr_num>=MAX_MSR3) GP(0)
			else
			{
			msr3[msr_num].lo=__EAX__;
			msr3[msr_num].hi=__EDX__;
			}
		}
		else 
		{

			if(msr_num>=MAX_MSR1) GP(0)
			else
			{
				msr1[msr_num].lo=__EAX__;
				msr1[msr_num].hi=__EDX__;

				if(msr_num==MSR_APICBASE)
				{
				   memory_interface.set_base(msr1[msr_num].lo&~0xfff);
#if LOG_MSR
				   log("set APICBASE:%08x %08x\n",msr1[msr_num].hi,msr1[msr_num].lo);
#endif
				}
			}
		}
}

void CCPU::init_msr()
{
	//
	// msr
	//
	//msr:00000200 edx:00000000 eax:00000006
	//msr:00000201 edx:000000FF eax:C0000800
	//msr:00000202 edx:00000000 eax:40000006
	//msr:00000203 edx:000000FF eax:E0000800
	//msr:00000204 edx:00000000 eax:60000006
	//msr:00000205 edx:000000FF eax:F0000800
	//msr:00000206 edx:00000000 eax:70000006
	//msr:00000207 edx:000000FF eax:F8000800
	//msr:00000208 edx:00000000 eax:00000000
	//msr:00000209 edx:00000000 eax:00000000
	//msr:0000020A edx:00000000 eax:00000000
	//msr:0000020B edx:00000000 eax:00000000
	//msr:0000020C edx:00000000 eax:00000000
	//msr:0000020D edx:00000000 eax:00000000
	//msr:0000020E edx:00000000 eax:00000000
	//msr:0000020F edx:00000000 eax:00000000
	memset(msr1,0,sizeof(msr1));
	memset(msr2,0,sizeof(msr2));
	memset(msr3,0,sizeof(msr3));
	msr1[MSR_MTRR_PHYSBASE0].hi=0x00000000;
	msr1[MSR_MTRR_PHYSBASE0].lo=0x00000006;

	msr1[MSR_MTRR_PHYSMASK0].hi=0x000000ff;
	msr1[MSR_MTRR_PHYSMASK0].lo=0xc0000800;

	msr1[MSR_MTRR_PHYSBASE1].hi=0x000000ff;
	msr1[MSR_MTRR_PHYSMASK1].lo=0xe0000800;

	msr1[MSR_MTRR_PHYSBASE2].hi=0x00000000;
	msr1[MSR_MTRR_PHYSBASE2].lo=0x60000006;

	msr1[MSR_MTRR_PHYSMASK2].hi=0x000000ff;
	msr1[MSR_MTRR_PHYSMASK2].lo=0xf0000800;

	msr1[MSR_MTRR_PHYSBASE3].hi=0x00000000;
	msr1[MSR_MTRR_PHYSBASE3].lo=0x70000006;

	msr1[MSR_MTRR_PHYSMASK3].hi=0x000000ff;
	msr1[MSR_MTRR_PHYSMASK3].lo=0xf8000800;

	//msr1[MSR_MTRR_PHYSBASE4].hi=0x00000000;
	//msr1[MSR_MTRR_PHYSBASE4].lo=0x00000000;

	//msr1[MSR_MTRR_PHYSMASK4].hi=0x00000000;
	//msr1[MSR_MTRR_PHYSMASK4].lo=0x00000000;

	//msr1[MSR_MTRR_PHYSBASE5].hi=0x00000000;
	//msr1[MSR_MTRR_PHYSBASE5].lo=0x00000000;

	//msr1[MSR_MTRR_PHYSMASK5].hi=0x00000000;
	//msr1[MSR_MTRR_PHYSMASK5].lo=0x00000000;

	//msr1[MSR_MTRR_PHYSBASE6].hi=0x00000000;
	//msr1[MSR_MTRR_PHYSBASE6].lo=0x00000000;

	//msr1[MSR_MTRR_PHYSMASK6].hi=0x00000000;
	//msr1[MSR_MTRR_PHYSMASK6].lo=0x00000000;

	//msr1[MSR_MTRR_PHYSBASE7].hi=0x00000000;
	//msr1[MSR_MTRR_PHYSBASE7].lo=0x00000000;

	//msr1[MSR_MTRR_PHYSMASK7].hi=0x00000000;
	//msr1[MSR_MTRR_PHYSMASK7].lo=0x00000000;

	msr1[MSR_APICBASE].lo= bsp?0xFEE00900:0xFEE00800;//BSP?,APIC Global

	msr1[0x17].hi=2<<0x12;
	//msr1[0x79].hi=;
}