
#include <conio.h>
#include <stdio.h>
#include "cpu.h"
#include "util.h"
#include "pci.h"
#include "smi82801.h"
#include "acpi82801.h"
#include "log.h"
extern CCPU cpu[];
extern CPCIBus pci_bus;

static unsigned char smi_en_eos_mask=1;
static unsigned long timer_count;
static unsigned long smi_pending;
static SMI_CAUSE_DESC  smi_cause[]=
{
	{0x30,0x20,0x34,0x20},//APMC_WRITE
	{0x30,0x40,0x34,0x40},//SMI_SMI_TIMER
	{0x31,0x20,0x35,0x20},//SMI_TCO
	{0x02,0x01,0x00,0x01},//SMI_ACPI_TIMER
	{0x31,0x40,0x35,0x40},//SMI_ACPI_PER
};
//
// SMI causes: 
//
//1.Write to B2h register No Yes none APM_STS
//2.ACPI Timer overflow (2.34 sec.) Yes Yes TMROF_EN=1 TMROF_STS
//3.64 ms timer expires No Yes SWSMI_TMR_EN=1 SWSMI_TMR_STS
// 4.
void invoke_smi(enum smi_type type)
{
	// 
	//report status. e.g.set APM_STS
	//
	if((acpi82801[smi_cause[type].enable_reg]&smi_cause[type].enable_mask)!=0)
	{
		acpi82801.r[smi_cause[type].report_reg]|=smi_cause[type].report_mask;
		smi_pending|=(1<<type);
	}
	/*
	//
	//1. SCI_EN must be 1 to enable SCI. SCI_EN must be 0 to enable SMI.
	//2. SCI can be routed to cause interrupt 9:11 or 20:23 (20:23 only available in APIC mode).
	//3. GBL_SMI_EN must be 1 to enable SMI.
	//4. EOS must be written to 1 to re-enable SMI for the next one.
	//
	// if smi_pending now enabled, assert smi#
	//
	if(smi_pending!=0
		&&(acpi82801[0x30]&smi_en_eos_mask)==smi_en_eos_mask
		//&& (acpi82801[smi_cause[i].report_reg]&smi_cause[i].report_mask)!=0// e.g. apm_sts set?
		&& (acpi82801[smi_cause[type].enable_reg]&smi_cause[type].enable_mask)!=0)//e.g.APMC_EN bit set.?
	{
			smi_en_eos_mask=3;// after firt smi, eos must be set to have smi sserted
			cpu[0].assert_smi();
			//eos:
			//Once the ICH2 asserts SMI# low, the EOS bit is automatically cleared.
			//
			acpi82801.r[0x30]&=~2;
	}
	*/
}

void reset_smi()
{
	smi_en_eos_mask=1;
	timer_count=0;
	smi_pending=0;
}
//SWSMI_TMR_EN stays set until cleared by software.
//
//64 ms timer expires No Yes SWSMI_TMR_EN=1 SWSMI_TMR_STS
//
void do_smi_timer()
{
	timer_count++;
	if(timer_count==64000)// 64ms->us
	{
		timer_count=0;
		invoke_smi(SMI_SMI_TIMER);
	}
}
void do_smi()
{
	//
	//1. SCI_EN must be 1 to enable SCI. SCI_EN must be 0 to enable SMI.
	//2. SCI can be routed to cause interrupt 9:11 or 20:23 (20:23 only available in APIC mode).
	//3. GBL_SMI_EN must be 1 to enable SMI.
	//4. EOS must be written to 1 to re-enable SMI for the next one.
	//
	// if smi_pending now enabled, assert smi#
	//
	if(smi_pending!=0
		&&(acpi82801[0x30]&smi_en_eos_mask)==smi_en_eos_mask)
	{
		unsigned long x=smi_pending,i=0;
		bool found=false;
		while(x)
		{
	//1 = SMI# was generated by a write access to the APM control register with the 
			if((x&1)!=0 
				&& (acpi82801[smi_cause[i].report_reg]&smi_cause[i].report_mask)!=0// e.g. apm_sts set?
				/*&& (acpi82801[smi_cause[i].enable_reg]&smi_cause[i].enable_mask)!=0*/)//e.g.APMC_EN bit set.?
			{
				log("smi cause:%d %x\n",i,x);
				found=true;
				break;
			}
			i++;
			x>>=1;
		}
		if(found)
		{
			smi_en_eos_mask=3;// after firt smi, eos must be set to have smi sserted
			cpu[0].assert_smi();
			smi_pending=0;
			//eos:
			//Once the ICH2 asserts SMI# low, the EOS bit is automatically cleared.
			//
			acpi82801.r[0x30]&=~2;
		}
	}
}


