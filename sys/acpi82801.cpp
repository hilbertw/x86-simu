#include "stdlib.h"
#include "acpi82801.h"
#include "smi82801.h"
#include "pci.h"
#include "log.h"
extern CPCIBus pci_bus;

CACPI82801  acpi82801;



unsigned char CACPI82801::wc_mask[0x71]={
	0x21,0x8d,0,0,
	0,0,0,0,
	0,0,0,0,//8
	0,0,0,0,
	0,0,0,0,//10
	0,0,0,0,
	0,0,0,0,//18
	0,0,0,0,
	0,0,0,0,//20
	0,0,0,0,
	0xfb,0x39,0xff,0xff,//28
	0,0,0,0,
	0,0,0,0,//30
	0x74,0x48,7,0,
	0,0,0xff,0xff,//38
	0,0,0,0,
	0xf0,0,0,0,//40
	0xff,0xff,0,0,
	0,0,0,0,//48
	0,0,0,0,
	0,0,0,0,//50
	0,0,0,0,
	0,0,0,0,//58
	0,0,0,0,
	0,0,0,0,//60
	0x84,0x1f,0x17,0,
	0,1,0,0,//68
	0,0,0,0,
	0//70
};
unsigned char CACPI82801::ro_mask[0x71]={
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,//8
	0,0,0,0,
	0,0,0,0,//10
	0,0,0,0,
	0,0,0,0,//18
	0,0,0,0,
	0,0,0,0,//20
	0,0,0,0,
	0,0,0,0,//28
	0,0,0,0,
	0,0,0,0,//30
	0,0,0,0,
	0,0,0,0,//38
	0,0,0,0,
	0,0,0,0,//40
	0xff,0xff,0,0,
	0,0,0,0,//48
	0,0,0,0,
	0,0,0,0,//50
	0,0,0,0,
	0,0,0,0,//58
	0,0,0,0,
	0,0,0,0,//60
	0,0,0,0,
	0,0,0,0,//68
	0,0,0,0,
	0//70
};

void CACPI82801::set_reg(int i, unsigned char v)
{
	//
	// tco registers
	//
	if(i==0)
	{
		acpi82801.r[0x60]=acpi82801[0x61]&0x3f;
	}
	if (i < sizeof(r))
	{

		//
		// BIOS has a corresponding bit, BIOS_RLS, which will cause an SCI and set this bit.
		//
		//
		// if writes to PM1_CNT(register 5),ifSLP_EN,ring machine to Sx
		//
		// cope with wc bits
		r[i] &= ~(v&wc_mask[i]);//  clear wc bits
		v &= ~wc_mask[i];

		// cope with readonly bits
		r[i] &= ro_mask[i];     // keep ro bits
		r[i] |= (v&~ro_mask[i]);// set r/w bits.
		//
		// DEVACT_STS！Device Activity Status Register,always active. 
		// by masking off writes to reg 44h,45h
		//
	}
}

static unsigned long tco_count,timer_count,periodic_count,tmr_cnt_mask;

//TCO SMI -TCO TIMEROUT No Yes none TIMEOUT,about 0.6s
void do_tco()
{
	tco_count++;
	if(tco_count==600000) // 0.6s
	{
		if((--acpi82801.r[0x60])==0)
		{
			invoke_smi(SMI_TCO);
		}
		tco_count=0;
	}
}

//
//Periodic timer expires No Yes PERIODIC_EN=1 PERIODIC_STS
//see GEN_PMCON_1！General PM Configuration 1 Register (PM！D31:F0)
//
//Periodic SMI# Rate Select (PER_SMI_SEL) ！ R/W. Set by software to control the rate the
//periodic SMI# is generated.
//00 = 1 minute
//01 = 32 seconds
//10 = 16 seconds
//11 = 8 seconds
unsigned long per_count[]=
{
	60,
	32,
	16,
	8
};
void do_acpi_periodic()
{
	DWORD GEN_PMCON_1;

	pci_bus.read_config_reg(0x80000000+(0x1f*8)+0xa0,(BYTE*)&GEN_PMCON_1,sizeof(GEN_PMCON_1));

	periodic_count++;// increase every sec
	if(periodic_count==per_count[GEN_PMCON_1&3])
	{
		periodic_count=0;
		invoke_smi(SMI_ACPI_PER);
	}
}
//
//ACPI Timer overflow (2.34 sec.) Yes Yes TMROF_EN=1 TMROF_STS
//
void do_acpi_timer()
{
	acpi82801.reg.ddPM1Timer.pm_timer.TMR_VAL+=0xffff;
		//(timer_count&1)+3;//3.579545MHz

	timer_count++;
	if(timer_count==2340000)
	{
		timer_count=0;
		//ACPI PM1a_EVT_BLK
		if(acpi82801.reg.PM1a_evt_blk.pm1_enable.TMR_EN)
		{
			acpi82801.reg.PM1a_evt_blk.pm1_states.TMR_STS=1;
			invoke_smi(SMI_ACPI_TIMER);
		}
	}

	//This is the timer carry status bit. This bit gets set any time the 23rd/31st bit of a 24/32-bit counter changes 
	//(whenever the MSB changes from clear to set or set to clear. While TMR_EN and TMR_STS are set, 
	//an interrupt event is raised.
	if((acpi82801.reg.ddPM1Timer.pm_timer.TMR_VAL&tmr_cnt_mask)==tmr_cnt_mask) 
	{
		tmr_cnt_mask^=0x800000;  // 23rd
		acpi82801.reg.PM1a_evt_blk.pm1_states.TMR_STS=1;
		//
		//While TMR_EN and TMR_STS are set, an interrupt event is raised.
		//
		if(acpi82801.reg.dwSMIControlEnable&0x40)
			acpi_interrupt(SMI_ACPI_TIMER);
	}
}



void enable_acpi(bool enable)
{
#if LOG_ACPI
	log0("Enable ACPI(%d)\n",enable);
#endif
	acpi82801.reg.PM1a_Cnt.pm1_scontrol.SCI_EN=enable?1:0;
}
